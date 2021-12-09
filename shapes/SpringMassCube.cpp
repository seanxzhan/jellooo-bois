#include "SpringMassCube.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>

SpringMassCube::SpringMassCube():
    Shape(7, 7)
{
    generateVertexData();
}

SpringMassCube::SpringMassCube(int param1, int param2):
    Shape(param1,param2)
{
    generateVertexData();
}

SpringMassCube::~SpringMassCube(){}

void SpringMassCube::setParam1(int inp) {
    m_param1 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
}

void SpringMassCube::setParam2(int inp) {
    m_param2 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
}

void SpringMassCube::generateVertexData(){
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);
    m_points.reserve(num_control_points);

    //Initialize points
    float incr = 1.f / m_param1;

    //Convention for indexing into cube
    // points[0][0][0] is (-0.5f, 0.5f, 0.5f)
    // points[dim - 1][dim - 1][dim - 1] is (0.5f, -0.5f, -0.5f)
//    glm::vec3 start = glm::vec3(-0.5f, 0.5f, 0.5f);
    std::vector<GLfloat> start = {-0.5f, 0.5f, 0.5f};
    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
//                m_points.push_back(start + glm::vec3(j * incr, i * -incr, k * -incr));
                m_points.push_back(start[0] + j * incr);
                m_points.push_back(start[1] + i * -incr);
                m_points.push_back(start[2] + k * -incr);
            }
        }
    }
}

void SpringMassCube::drawPoints(std::vector<GLfloat> &points) {
    int num_points = points.size() / 3;
    setVertexData(&points[0], points.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_POINTS, num_points);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

void SpringMassCube::tick(float current) {
    //This just goes up and down - should involve call to compute acceleration and using RK4 integration
    float increment = sin(current) / 60;
    int dim = m_param1 + 1;
    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
//                m_points[to1D(i, j, k, dim, dim)].y += increment;
                m_points[3 * to1D(i, j, k, dim, dim) + 1] += increment;
            }
        }
    }

    drawPoints(m_points);
}
