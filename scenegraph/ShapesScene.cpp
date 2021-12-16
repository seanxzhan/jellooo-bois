#include "ShapesScene.h"
#include "Camera.h"
#include "shapes/OpenGLShape.h"
#include "Settings.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>

#include <sstream>
#include <iostream>

#include "shapes/ExampleShape.h"
#include "shapes/ExampleShape2.h"

using namespace CS123::GL;
#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"
#include "gl/shaders/ShaderAttribLocations.h"

#include "ResourceLoader.h"
#include "shapes/ExampleShape.h"
#include "shapes/JelloCube.h"
#include "shapes/Bbox.h"
#include "shapes/SpringMassCube.h"


#include "gl/shaders/ShaderAttribLocations.h"

// added by Marc - from lab 11
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ShapesScene::ShapesScene(int width, int height) :
    m_shape(nullptr),
    m_bbox(std::make_unique<Bbox>()),
    m_shapeParameter1(-1),
    m_shapeParameter2(-1),
    m_width(width),
    m_height(height),
    m_simType(-1),
    m_shapeType(-1)
{
    initializeSceneMaterial();
    initializeSceneLight();
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
    loadTestShader();

    //added by Marc
    loadSkyboxShader();
    m_cubeMapTexture = setSkyboxUniforms(m_skyboxShader.get());
    loadJelloShader();
    //glDisable(GL_DEPTH_TEST);

    // [SHAPES] Allocate any additional memory you need...
}

ShapesScene::~ShapesScene()
{
    // Pro-tip: If you use smart pointers properly, this destructor should be empty
}

void ShapesScene::initializeSceneMaterial() {
    // Use a shiny orange material
    m_material.clear();
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1.0f;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 10;
}

void ShapesScene::initializeSceneLight() {
    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LightType::LIGHT_DIRECTIONAL;
    m_light.dir = m_lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;
}

void ShapesScene::loadJelloShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/glass.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/glass.frag");

    m_jelloShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void ShapesScene::loadSkyboxShader() {
    // using Shader vs CS123Shader bc we don't need light + material info
    // Loading in Skybox Shader
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/skybox.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/skybox.frag");

    m_skyboxShader = std::make_unique<Shader>(vertexSource, fragmentSource);

//      Loading CubeMap data
    m_skyboxCube = std::make_unique<ExampleShape2>(1,1);
}

void ShapesScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void ShapesScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void ShapesScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void ShapesScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void ShapesScene::loadTestShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/bbox.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/bbox.frag");
    m_testShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void ShapesScene::render(SupportCanvas3D *context) {
    // Clear the screen in preparation for the next frame. (Use a gray background instead of a
    // black one for drawing wireframe or normals so they will show up against the background.)
    glm::mat4x4 viewMat = context->getCamera()->getViewMatrix();
    viewMat = glm::transpose(viewMat);

    glm::vec3 worldSpaceDown = glm::vec3(0, -1, 0);
    m_shape->setGravity(settings.gravity,
                        glm::inverse(glm::transpose(glm::mat3x3(viewMat)))*worldSpaceDown);

    setClearColor();

    if (m_usePhong) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_testShader->bind();
        setMatrixUniforms(m_testShader.get(), context);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_bbox->drawBbox();
        glm::vec3 color = glm::vec3(0.1, 0.8, 0.1);
        m_testShader->setUniform("color", color);
        m_bbox->drawFloor();
        m_testShader->setUniform("color", glm::vec3(0.7, 0.1, 0.7));
        m_bbox->drawPlane();
        m_testShader->setUniform("color", color);
        m_testShader->unbind();

        renderPhongPass(context);

        if (settings.drawWireframe) {
            renderWireframePass(context);
        }

        if (settings.drawNormals) {
            renderNormalsPass(context);
        }
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        glDisable(GL_DEPTH_TEST);
//        glDisable(GL_POLYGON_OFFSET_LINE);

        renderSkybox(context);

        m_testShader->bind();
        setMatrixUniforms(m_testShader.get(), context);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_bbox->drawBbox();
        glm::vec3 color = glm::vec3(0.1, 0.8, 0.1);
        m_testShader->setUniform("color", color);
        m_bbox->drawFloor();
        m_testShader->setUniform("color", glm::vec3(0.7, 0.1, 0.7));
        m_bbox->drawPlane();
        m_testShader->setUniform("color", color);
        m_testShader->unbind();

        renderJelloPass(context);

//        glEnable(GL_DEPTH_TEST);
//        // Move the polygons back a bit so lines are still drawn even though they are coplanar with the
//        // polygons they came from, which will be drawn before them.
//        glEnable(GL_POLYGON_OFFSET_LINE);
//        glPolygonOffset(-1, -1);
    }
  
}

// Need to confirm this works for both orbiting and camtrans camera D:
void ShapesScene::renderJelloPass(SupportCanvas3D *context) {
    m_jelloShader->bind();

    // clearLights()
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_jelloShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    // setLights()
    m_light.dir = glm::inverse(context->getCamera()->getViewMatrix()) * m_lightDirection;

    // clearLights();
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_jelloShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
    m_jelloShader->setLight(m_light);

    m_jelloShader->setUniform("useLighting", settings.useLighting);
    int color = 0;
    if (settings.jelloColor == JC_Red) {
        color = 1;
    } else if (settings.jelloColor == JC_Green) {
        color = 2;
    } else if (settings.jelloColor == JC_Blue) {
        color = 0;
    } else {
        // white
        color = 3;
    }
    m_jelloShader->setUniform("jelloColor", color);

    // Pass in uniforms for view, projection, model (mat4(1.0))
    setMatrixUniforms(m_jelloShader.get(), context);
    // Pass in our environment map
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // may not need this idk
    m_shape->draw();
    m_jelloShader->unbind();
}

void ShapesScene::renderSkybox(SupportCanvas3D *context) {

//    glFrontFace(GL_CW);

//    glDepthMask(GL_FALSE);
    m_skyboxShader->bind();

    glFrontFace(GL_CW);
    glDepthMask(GL_FALSE);

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_skyboxShader->setUniform("projection", context->getCamera()->getProjectionMatrix());
    m_skyboxShader->setUniform("view", glm::mat4(glm::mat3(context->getCamera()->getViewMatrix())));

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_skyboxCube->draw();

    glDepthMask(GL_TRUE);
    glFrontFace(GL_CCW);

    m_skyboxShader->unbind();
//    glDepthMask(GL_TRUE);

//    glFrontFace(GL_CCW);
}

unsigned int ShapesScene::setSkyboxUniforms(Shader *shader) {
    // [NOTE] Need to use absolute paths
    std::vector<std::string> faces =  {"/Users/seanzhan/course/cs1230/jellooo-bois/textures/bridge-waterfall/posx.jpg",
                                       "/Users/seanzhan/course/cs1230/jellooo-bois/textures/bridge-waterfall/negx.jpg",
                                       "/Users/seanzhan/course/cs1230/jellooo-bois/textures/bridge-waterfall/posy.jpg",
                                       "/Users/seanzhan/course/cs1230/jellooo-bois/textures/bridge-waterfall/negy.jpg",
                                       "/Users/seanzhan/course/cs1230/jellooo-bois/textures/bridge-waterfall/posz.jpg",
                                       "/Users/seanzhan/course/cs1230/jellooo-bois/textures/bridge-waterfall/negz.jpg"};

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
//        std::cout << width << std::endl;
//        std::cout << height << std::endl;
//        std::cout << nrChannels << std::endl;
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            std::cout << "Possible Fix: Change texture paths in ShapesScene.cpp :D" << std::endl;
            free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void ShapesScene::renderPhongPass(SupportCanvas3D *context) {
    m_phongShader->bind();

        // -------------------------------------------------
//    if (m_usePhong) {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    }
    clearLights();
    setLights(context->getCamera()->getViewMatrix());
    setPhongSceneUniforms();
    setMatrixUniforms(m_phongShader.get(), context);
    renderGeometryAsFilledPolygons();

    m_phongShader->unbind();
}

void ShapesScene::setPhongSceneUniforms() {
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->applyMaterial(m_material);
}

void ShapesScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
    shader->setUniform("m", glm::mat4(1.0f));
}

void ShapesScene::renderGeometryAsFilledPolygons() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    renderGeometry();
}

void ShapesScene::renderWireframePass(SupportCanvas3D *context) {
    m_wireframeShader->bind();
    setMatrixUniforms(m_wireframeShader.get(), context);
    renderGeometryAsWireframe();
    m_wireframeShader->unbind();
}

void ShapesScene::renderGeometryAsWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float lineWidth[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
    glLineWidth(lineWidth[1]);
    renderGeometry();
}

void ShapesScene::renderNormalsPass (SupportCanvas3D *context) {
    // Render the lines.
    m_normalsShader->bind();
    setMatrixUniforms(m_normalsShader.get(), context);
    renderGeometryAsWireframe();
    m_normalsShader->unbind();

    // Render the arrows.
    m_normalsArrowShader->bind();
    setMatrixUniforms(m_normalsArrowShader.get(), context);
    renderGeometryAsFilledPolygons();
    m_normalsArrowShader->unbind();
}

void ShapesScene::renderGeometry() {
    if (m_shape) {
        if (m_shapeType == SHAPE_SPRING_MASS_CUBE) {
            m_shape->drawPandL();
        } else {
            m_shape->draw();
        }
    }
}

void ShapesScene::clearLights() {
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
        m_testShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void ShapesScene::setLights(const glm::mat4 viewMatrix) {
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    m_light.dir = glm::inverse(viewMatrix) * m_lightDirection;

    clearLights();
    m_phongShader->setLight(m_light);
    m_testShader->setLight(m_light);
}

void ShapesScene::settingsChanged() {
    // Change SIMULATION Type Here based on Settings
    // TODO: eventually add if statements and member variables to check what type of setting was changed

    // Type
    if (settings.simType != m_simType) {
        switch (settings.simType) {
            case SIM_JELLO_SIM:
                std::cout << "sim type: jello sim" << std::endl;
            break;
            case SIM_STATIC_CUBE:
                std::cout << "sim type: static cube" << std::endl;
            break;
            case SIM_EXAMPLE:
                std::cout << "sim type: example" << std::endl;
            break;
            default:
                std::cout << "sim type: these sim types have no-impl" << std::endl;
            break;
        }
        m_simType = settings.simType;
    }
    // TODO: check if params are the same

        m_shapeParameter1 = settings.shapeParameter1;
        m_shapeParameter2 = settings.shapeParameter2;
        switch (settings.shapeType) {
            case SHAPE_CUBE:
                std::cout << "shape type: phong cube" << std::endl;
                m_usePhong = true;
                m_shape = std::make_unique<JelloCube>(m_shapeParameter1, m_shapeParameter2, settings.kElastic, settings.dElastic, settings.kCollision, settings.dCollision, settings.mass, settings.gravity);
            break;
            case SHAPE_JELLO_CUBE:
                std::cout << "shape type: jello cube" << std::endl;
                m_usePhong = false;
                m_shape = std::make_unique<JelloCube>(m_shapeParameter1, m_shapeParameter2, settings.kElastic, settings.dElastic, settings.kCollision, settings.dCollision, settings.mass, settings.gravity);
            break;
            case SHAPE_SPRING_MASS_CUBE:
                std::cout << "shape type: spring mass cube" << std::endl;
                m_usePhong = true;
                m_shape = std::make_unique<SpringMassCube>(m_shapeParameter1, m_shapeParameter2, settings.kElastic, settings.dElastic, settings.kCollision, settings.dCollision, settings.mass, settings.gravity);
            break;
            case SHAPE_CYLINDER:
                std::cout << "shape type: jellooo cylinder" << std::endl;
                m_shape = std::make_unique<ExampleShape>(m_shapeParameter1, m_shapeParameter2);
            break;
            case SHAPE_CONE:
                std::cout << "shape type: jellooo cone" << std::endl;
                m_shape = std::make_unique<ExampleShape>(m_shapeParameter1, m_shapeParameter2);
            break;
            case SHAPE_SPHERE:
                std::cout << "shape type: jellooo sphere" << std::endl;
                 m_shape = std::make_unique<ExampleShape>(m_shapeParameter1, m_shapeParameter2);
            break;
            default:
                std::cout << "shape type: these shapes have no-impl" << std::endl;
                m_shape = std::make_unique<ExampleShape>(m_shapeParameter1, m_shapeParameter2);
            break;
        }
        m_shapeType = settings.shapeType;
}

void ShapesScene::tick(float current) {
    if (m_simType != SIM_STATIC_CUBE){
        m_shape->tick(current);
    }
}

