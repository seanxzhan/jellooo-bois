#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

protected:

    CS123SceneGlobalData m_global;
    std::vector<CS123SceneLightData> m_scene_lights;
    std::vector<std::pair<CS123ScenePrimitive, glm::mat4x4>> m_object_data;

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

private:

    // recursive helper to accumulate CTM and add primitives
    static void loadObjectsHelper(Scene *sceneToFill, CS123SceneNode* parent, glm::mat4x4 CTM);

    // helper to calculate CTM for list of Scene Transforms
    // assumes last index represents 1st transformation applied
    static glm::mat4x4 calcCTM(std::vector<CS123SceneTransformation*> list);

    // helper to calculate transformation matrix for CS123SceneTransformation
    static glm::mat4x4 calcTransformMatrix(CS123SceneTransformation *inp);

};

#endif // SCENE_H
