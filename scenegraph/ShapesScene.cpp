#include "ShapesScene.h"
#include "Camera.h"
#include "shapes/OpenGLShape.h"
#include "Settings.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>

#include <sstream>
#include <iostream>

#include "shapes/ExampleShape.h"

using namespace CS123::GL;
#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"

#include "ResourceLoader.h"
#include "shapes/ExampleShape.h"

#include "gl/shaders/ShaderAttribLocations.h"

// added by Marc - from lab 11
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ShapesScene::ShapesScene(int width, int height) :
    m_shape(nullptr),
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

    //added by Marc
    loadSkyboxShader();
    m_cubeMapTexture = setSkyboxUniforms(m_skyboxShader.get());
    loadJelloShader();

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
    m_material.shininess = 64;
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

    m_jelloShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void ShapesScene::loadSkyboxShader() {
    // using Shader vs CS123Shader bc we don't need light + material info
    // Loading in Skybox Shader
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/skybox.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/skybox.frag");

    m_skyboxShader = std::make_unique<Shader>(vertexSource, fragmentSource);

//      Loading CubeMap data
    m_skyboxCube = std::make_unique<ExampleShape>(1,1);
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

void ShapesScene::render(SupportCanvas3D *context) {
    // Clear the screen in preparation for the next frame. (Use a gray background instead of a
    // black one for drawing wireframe or normals so they will show up against the background.)
    setClearColor();

    renderSkybox(context);

    // TODO: Update to UI Settings Eventually
    bool usePhong = false; // if false uses our jello shader :D

    if (usePhong) {
        renderPhongPass(context);

        if (settings.drawWireframe) {
            renderWireframePass(context);
        }

        if (settings.drawNormals) {
            renderNormalsPass(context);
        }
    } else {
        renderJelloPass(context);
    }
}

// Need to confirm this works for both orbiting and camtrans camera D:
void ShapesScene::renderJelloPass(SupportCanvas3D *context) {
    m_jelloShader->bind();

    // Pass in uniforms for view, projection, model (mat4(1.0))
    setMatrixUniforms(m_jelloShader.get(), context);
    // Pass in our environment map
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // may not need this idk
    m_shape->draw();
    m_jelloShader->unbind();
}

void ShapesScene::renderSkybox(SupportCanvas3D *context) {

    glFrontFace(GL_CW);

    glDepthMask(GL_FALSE);
    m_skyboxShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_skyboxShader->setUniform("projection", context->getCamera()->getProjectionMatrix());
    m_skyboxShader->setUniform("view", context->getCamera()->getViewMatrix());

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_skyboxCube->draw();
    m_skyboxShader->unbind();
    glDepthMask(GL_TRUE);

    glFrontFace(GL_CCW);
}

unsigned int ShapesScene::setSkyboxUniforms(Shader *shader) {
    // [NOTE] Need to use absolute paths
    std::vector<std::string> faces = {"C://Users//marcm//Documents//cs1230//jello-final//textures//MarriottMadisonWest//posx.jpg",
                                      "C://Users//marcm//Documents//cs1230//jello-final//textures//MarriottMadisonWest//negx.jpg",
                                      "C://Users//marcm//Documents//cs1230//jello-final//textures//MarriottMadisonWest//posy.jpg",
                                      "C://Users//marcm//Documents//cs1230//jello-final//textures//MarriottMadisonWest//negy.jpg",
                                      "C://Users//marcm//Documents//cs1230//jello-final//textures//MarriottMadisonWest//posz.jpg",
                                      "C://Users//marcm//Documents//cs1230//jello-final//textures//MarriottMadisonWest//negz.jpg"};

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
        m_shape->draw();
    }
}

void ShapesScene::clearLights() {
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void ShapesScene::setLights(const glm::mat4 viewMatrix) {
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    m_light.dir = glm::inverse(viewMatrix) * m_lightDirection;

    clearLights();
    m_phongShader->setLight(m_light);
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

    if (settings.shapeType != m_shapeType) {
        switch (settings.shapeType) {
            case SHAPE_CUBE:
                std::cout << "shape type: jellooo cube" << std::endl;
                m_shape = std::make_unique<ExampleShape>(settings.shapeParameter1, settings.shapeParameter2);
            break;
            case SHAPE_CYLINDER:
                std::cout << "shape type: jellooo cylinder" << std::endl;
                m_shape = std::make_unique<ExampleShape>(settings.shapeParameter1, settings.shapeParameter2);
            break;
            case SHAPE_CONE:
                std::cout << "shape type: jellooo cone" << std::endl;
                m_shape = std::make_unique<ExampleShape>(settings.shapeParameter1, settings.shapeParameter2);
            break;
            case SHAPE_SPHERE:
                std::cout << "shape type: jellooo sphere" << std::endl;
                 m_shape = std::make_unique<ExampleShape>(settings.shapeParameter1, settings.shapeParameter2);
            break;
            default:
                std::cout << "shape type: these shapes have no-impl" << std::endl;
                m_shape = std::make_unique<ExampleShape>(settings.shapeParameter1, settings.shapeParameter2);
            break;
        }
        m_shapeType = settings.shapeType;
    }
}

