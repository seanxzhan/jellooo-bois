#include "ExampleShape.h"
#include <iostream>

ExampleShape::ExampleShape()
{
}

ExampleShape::ExampleShape(int param1) :
    m_param1(param1)
{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */
    m_vertexData = CUBE_DATA_POSITIONS;

    /**
     * Initialize the OpenGLShapeProperties so that the shape is ready to be drawn.
     * Note that this does not actually draw the shape, that happens in ShapesScene.cpp
     */
    initializeOpenGLShapeProperties();
}

ExampleShape::~ExampleShape()
{
}

void ExampleShape::setGravity(float scale, glm::vec3 new_direction) {
}

//current time in seconds
void ExampleShape::tick(float current) {
//    float increment = sin(current) / 60;
//    int length = m_vertexData.size() / 6;
//    for (int i = 1; i < length; i += 6) {
//        m_vertexData[i] += increment;
//    }

//    initializeOpenGLShapeProperties();
//    std::cout << current << std::endl;
}
