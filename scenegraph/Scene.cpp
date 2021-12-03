#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    CS123SceneGlobalData globalData;
    parser->getGlobalData(globalData);
    sceneToFill->setGlobal(globalData);

    // QUESTIONS
    // is vector<pairs> fine for object data and vector for scene lights fine?
    // ^ need to initialize in constructor for style D:
    // can we assume transformations are stored 0 to 1 like top to bottom in scenegraph file
    // is it worth optimizing with resizing/reserver (can resize to vector size + size(prims) in recursive helper)
    // ^ same for lights

    // Note: TEST when intersect comes out

    int numLights = 8;
    CS123SceneLightData lightData;
    for (int i = 0; i < numLights; i++) {
        bool lightFound = parser->getLightData(i, lightData);
        if (lightFound) sceneToFill->addLight(lightData);
    }

    CS123SceneNode *root = parser->getRootNode();
    loadObjectsHelper(sceneToFill, root, glm::mat4x4(1.0f));
}


void Scene::loadObjectsHelper(Scene *sceneToFill, CS123SceneNode* parent, glm::mat4x4 prevCTM) {
    // Accumulates CTM from previous nodes and current node
    glm::mat4x4 CTM = prevCTM * calcCTM(parent->transformations);

    // Adds each primitive at this node
    std::vector<CS123ScenePrimitive*> primitives = parent->primitives;
    for (CS123ScenePrimitive* obj : primitives) {
        sceneToFill->addPrimitive(*obj, CTM);
    }

    // Recursive Calls on each child node
    std::vector<CS123SceneNode*> children = parent->children;
    for (CS123SceneNode* child : children){
        loadObjectsHelper(sceneToFill, child, CTM);

    }
}

glm::mat4x4 Scene::calcCTM(std::vector<CS123SceneTransformation*> list) {
    glm::mat4x4 CTM = glm::mat4(1.0f);
    int numTransforms = list.size();
    for (int i = 0; i < numTransforms; i++){
        CTM = CTM * calcTransformMatrix(list.at(i));
    }
    return CTM;
}

glm::mat4x4 Scene::calcTransformMatrix(CS123SceneTransformation *inp) {
    TransformationType type = inp->type;
    switch(type){
        case TRANSFORMATION_TRANSLATE:
            return glm::translate(inp->translate);
        case TRANSFORMATION_SCALE:
            return glm::scale(inp->scale);
        case TRANSFORMATION_ROTATE:
            return glm::rotate(inp->angle, inp->rotate);
        case TRANSFORMATION_MATRIX:
            return inp->matrix;
    }
}


void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_object_data.push_back(std::pair<CS123ScenePrimitive, glm::mat4x4>(scenePrimitive, matrix));
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_scene_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

