#include "Shape.h"
#include <iostream>

Shape::Shape(int param1, int param2):
    m_param1((param1 < 1) ? 1 : param1),
    m_param2((param2 < 1) ? 1 : param2)
{

}

Shape::~Shape()
{
}

int Shape::getParam1() {
    return m_param1;
}

int Shape::getParam2() {
    return m_param2;
}

void Shape::setParam1(int inp) {
    m_param1 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
    initializeOpenGLShapeProperties();
}

void Shape::setParam2(int inp) {
    m_param2 = (inp < 1) ? 1 : inp;
//    std::cout << m_param2 << std::endl;
    m_vertexData.clear();
    generateVertexData();
    initializeOpenGLShapeProperties();
}


// Also normalized norm
void Shape::pushVertexAsFloats(const glm::vec3 &inp, const glm::vec3 &norm)
{
    glm::vec3 nNorm = glm::normalize(norm);

    m_vertexData.push_back(inp.x);
    m_vertexData.push_back(inp.y);
    m_vertexData.push_back(inp.z);

    m_vertexData.push_back(nNorm.x);
    m_vertexData.push_back(nNorm.y);
    m_vertexData.push_back(nNorm.z);
}

// assumes v1 is closer to x-axis + topright if we face
// the plane formed by points - other vectors follow clockwise
void Shape::pushRectangleAsFloats(
        const glm::vec3 &inp1, const glm::vec3 &norm1,
        const glm::vec3 &inp2, const glm::vec3 &norm2,
        const glm::vec3 &inp3, const glm::vec3 &norm3,
        const glm::vec3 &inp4, const glm::vec3 &norm4)
{
    pushVertexAsFloats(inp1, norm1);
    pushVertexAsFloats(inp4, norm4);
    pushVertexAsFloats(inp3, norm3);
    pushVertexAsFloats(inp1, norm1);
    pushVertexAsFloats(inp3, norm3);
    pushVertexAsFloats(inp2, norm2);
}

void Shape::tick(float current) {
    std::cout << "Shape:";
    std::cout << current << std::endl;
}

void Shape::setGravity(float scale, glm::vec3 gravity) {
};
