#include "JelloCube.h"
#include <iostream>

JelloCube::JelloCube():
    Shape(7, 7),
    dt(0.001)
{
    generateVertexData();
}

JelloCube::JelloCube(int param1, int param2):
    Shape(param1,param2)
{
    generateVertexData();
}

JelloCube::~JelloCube(){}

void JelloCube::setParam1(int inp) {
    m_param1 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
}

void JelloCube::setParam2(int inp) {
    m_param2 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
}

void JelloCube::generateVertexData(){
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);
    points.reserve(num_control_points);
    velocity.reserve(num_control_points);
    normals.reserve(dim * dim * 6);

    //Initialize points
    float incr = 1.f / m_param1;

    //Convention for indexing into cube
    // points[0][0][0] is (-0.5f, 0.5f, 0.5f)
    // points[dim - 1][dim - 1][dim - 1] is (0.5f, -0.5f, -0.5f)
    glm::vec3 start = glm::vec3(-0.5f, 0.5f, 0.5f);
    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
                points.push_back(start + glm::vec3(j * incr, i * -incr, k * -incr));
            }
        }
    }

    // Convention for indexing into normals
    // 6 2D slices, each slice is of dimension dim x dim for each face
    // normals [i][j][FACE] gives the normal at (i,j) given enum FACE
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                FACE side = (FACE)face;
                switch(side) {
                    case BOTTOM: {
                        normals.push_back(glm::vec3(0.f, -1.f, 0.f));
                        break;
                    }
                    case TOP: {
                        normals.push_back(glm::vec3(0.f, 1.f, 0.f));
                        break;
                    }
                    case FRONT: {
                        normals.push_back(glm::vec3(0.f, 0.f, 1.f));
                        break;
                    }
                    case BACK: {
                        normals.push_back(glm::vec3(0.f, 0.f, -1.f));
                        break;
                    }
                    case LEFT: {
                        normals.push_back(glm::vec3(-1.f, 0.f, 0.f));
                        break;
                    }
                    case RIGHT: {
                        normals.push_back(glm::vec3(1.f, 0.f, 0.f));
                        break;
                    }
                }
            }
        }
    }

    //Load VAO for each of the 6 faces with points and normas
    loadVAO();

    initializeOpenGLShapeProperties();

}

//void Shape::pushRectangleAsFloats(
//        const glm::vec3 &inp1, const glm::vec3 &norm1,
//        const glm::vec3 &inp2, const glm::vec3 &norm2,
//        const glm::vec3 &inp3, const glm::vec3 &norm3,
//        const glm::vec3 &inp4, const glm::vec3 &norm4)

//Should load the VAO given arbitrary positions of each cube point
void JelloCube::loadVAO() {
    //Calculate for each face
    int dim = m_param1 + 1;
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < dim - 1; i++) {
            for (int j = 0; j < dim - 1; j++) {
                FACE side = (FACE)face;
                glm::vec3 point1 = points[indexPoints(i, j, dim, side)];
                glm::vec3 normal1 = normals[to1D(i, j, face, dim, dim)];
                glm::vec3 point2 = points[indexPoints(i, j+1, dim, side)];
                glm::vec3 normal2 = normals[to1D(i, j+1, face, dim, dim)];
                glm::vec3 point3 = points[indexPoints(i+1, j+1, dim, side)];
                glm::vec3 normal3 = normals[to1D(i+1, j+1, face, dim, dim)];
                glm::vec3 point4 = points[indexPoints(i+1, j, dim, side)];
                glm::vec3 normal4 = normals[to1D(i+1, j, face, dim, dim)];
                pushRectangleAsFloats(
                            point1, normal1,
                            point2, normal2,
                            point3, normal3,
                            point4, normal4);
            }
        }
    }
}

void JelloCube::computeAcceleration(std::vector<glm::vec3> &acceleration) {

}

void JelloCube::euler() {
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);

    std::vector<glm::vec3> acceleration;
    acceleration.reserve(num_control_points);

    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
                points[to1D(i, j, k, dim, dim)] += dt * velocity[to1D(i, j, k, dim, dim)];
                velocity[to1D(i, j, k, dim, dim)] += dt * acceleration[to1D(i, j, k, dim, dim)];
            }
        }
    }

}

void JelloCube::rk4() {

}

//Should update positions and call on loadVAO and initializeOpenGLShapeProperties() to prep for drawing again
void JelloCube::tick(float current) {
//    std::cout << "Jello Cube" << std::endl;
//    std::cout << current << std::endl;

    //This just goes up and down - should involve call to compute acceleration and using RK4 integration
    float increment = sin(current) / 60;
    int dim = m_param1 + 1;
    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
                points[to1D(i, j, k, dim, dim)].y += increment;
            }
        }
    }
    loadVAO();
    initializeOpenGLShapeProperties();
}
