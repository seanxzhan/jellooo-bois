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
    m_points.reserve(num_control_points);
    m_velocity.reserve(num_control_points);
    m_normals.reserve(dim * dim * 6);

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
                m_points.push_back(start + glm::vec3(j * incr, i * -incr, k * -incr));
            }
        }
    }

    // Convention for indexing into normals
    // 6 2D slices, each slice is of dimension dim x dim for each face
    // normals [i][j][FACE] gives the normal at (i,j) given enum FACE

    //Below code is temporary until calculateNormals is filled in
//    for (int face = 0; face < 6; face++) {
//        for (int i = 0; i < dim; i++) {
//            for (int j = 0; j < dim; j++) {
//                FACE side = (FACE)face;
//                switch(side) {
//                    case BOTTOM: {
//                        m_normals.push_back(glm::vec3(0.f, -1.f, 0.f));
//                        break;
//                    }
//                    case TOP: {
//                        m_normals.push_back(glm::vec3(0.f, 1.f, 0.f));
//                        break;
//                    }
//                    case FRONT: {
//                        m_normals.push_back(glm::vec3(0.f, 0.f, 1.f));
//                        break;
//                    }
//                    case BACK: {
//                        m_normals.push_back(glm::vec3(0.f, 0.f, -1.f));
//                        break;
//                    }
//                    case LEFT: {
//                        m_normals.push_back(glm::vec3(-1.f, 0.f, 0.f));
//                        break;
//                    }
//                    case RIGHT: {
//                        m_normals.push_back(glm::vec3(1.f, 0.f, 0.f));
//                        break;
//                    }
//                }
//            }
//        }
//    }

    //Just debugging faces
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < dim - 1; i++) {
            for (int j = 0; j < dim - 1; j++) {
                FACE side = (FACE)face;
                int index = indexFromFace(i, j, dim, side);
                std::cout << index << ", ";
                index = indexFromFace(i, j + 1, dim, side);
                std::cout << index << ", ";
                index = indexFromFace(i + 1, j + 1, dim, side);
                std::cout << index << ", ";
                index = indexFromFace(i + 1, j, dim, side);
                std::cout << index << ", ";
            }
        }
        std::cout << std::endl;
    }

    //Load VAO for each of the 6 faces with points and normals
    calculateNormals();
    loadVAO();

    initializeOpenGLShapeProperties();

}

//Computes normals for points at arbitrary points
void JelloCube::calculateNormals() {
    int dim = m_param1 + 1;
    m_normals.clear();
    for (int i = 0; i < m_normals.size(); i++) {
        m_normals[i] = glm::vec3(0.f);
    }

    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < dim - 1; i++) {
            for (int j = 0; j < dim - 1; j++) {
                FACE side = (FACE)face;
                glm::vec3 point1 = m_points[indexFromFace(i, j, dim, side)];
                glm::vec3 point2 = m_points[indexFromFace(i, j + 1, dim, side)];
                glm::vec3 point3 = m_points[indexFromFace(i + 1, j + 1, dim, side)];
                glm::vec3 point4 = m_points[indexFromFace(i + 1, j, dim, side)];

                //Top triangle
                glm::vec3 v1 = point1 - point2;
                glm::vec3 v2 = point3 - point2;
                glm::vec3 normal = glm::cross(v1, v2);
                m_normals[to1D(i, j, face, dim, dim)] += normal;
                m_normals[to1D(i, j + 1, face, dim, dim)] += normal;
                m_normals[to1D(i + 1, j + 1, face, dim, dim)] += normal;

                //Bottom Triangle
                v1 = point1 - point4;
                v2 = point3 - point4;
                normal = glm::cross(v2, v1);
                m_normals[to1D(i, j, face, dim, dim)] += normal;
                m_normals[to1D(i + 1, j + 1, face, dim, dim)] += normal;
                m_normals[to1D(i + 1, j, face, dim, dim)] += normal;
            }
        }
    }

    for (int i = 0; i < m_normals.size(); i++) {
        m_normals[i] = glm::normalize(m_normals[i]);
    }
}

//Should load the VAO given arbitrary positions of each cube point
void JelloCube::loadVAO() {
    //Calculate for each face
    int dim = m_param1 + 1;
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < dim - 1; i++) {
            for (int j = 0; j < dim - 1; j++) {
                FACE side = (FACE)face;
                glm::vec3 point1 = m_points[indexFromFace(i, j, dim, side)];
                glm::vec3 normal1 = m_normals[to1D(i, j, face, dim, dim)];
                glm::vec3 point2 = m_points[indexFromFace(i, j+1, dim, side)];
                glm::vec3 normal2 = m_normals[to1D(i, j+1, face, dim, dim)];
                glm::vec3 point3 = m_points[indexFromFace(i+1, j+1, dim, side)];
                glm::vec3 normal3 = m_normals[to1D(i+1, j+1, face, dim, dim)];
                glm::vec3 point4 = m_points[indexFromFace(i+1, j, dim, side)];
                glm::vec3 normal4 = m_normals[to1D(i+1, j, face, dim, dim)];
                pushRectangleAsFloats(
                            point1, normal1,
                            point2, normal2,
                            point3, normal3,
                            point4, normal4);
            }
        }
    }
}

void JelloCube::computeAcceleration(std::vector<glm::vec3> &points,
                                    std::vector<glm::vec3> &velocity,
                                    std::vector<glm::vec3> &acceleration) {

}

void JelloCube::euler() {
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);

    std::vector<glm::vec3> acceleration;
    acceleration.reserve(num_control_points);

    computeAcceleration(m_points, m_velocity, acceleration);

    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
                m_points[to1D(i, j, k, dim, dim)] += dt * m_velocity[to1D(i, j, k, dim, dim)];
                m_velocity[to1D(i, j, k, dim, dim)] += dt * acceleration[to1D(i, j, k, dim, dim)];
            }
        }
    }

}

//Is it possible to optimize this thing?
void JelloCube::rk4() {
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);

    std::vector<glm::vec3> buffer_points;
    std::vector<glm::vec3> buffer_velocity;
    buffer_points.reserve(num_control_points);
    buffer_velocity.reserve(num_control_points);

    std::vector<glm::vec3> acceleration;
    acceleration.reserve(num_control_points);

    std::vector<glm::vec3> points1;
    std::vector<glm::vec3> velocity1;
    points1.reserve(num_control_points);
    velocity1.reserve(num_control_points);

    std::vector<glm::vec3> points2;
    std::vector<glm::vec3> velocity2;
    points2.reserve(num_control_points);
    velocity2.reserve(num_control_points);

    std::vector<glm::vec3> points3;
    std::vector<glm::vec3> velocity3;
    points3.reserve(num_control_points);
    velocity3.reserve(num_control_points);

    std::vector<glm::vec3> points4;
    std::vector<glm::vec3> velocity4;
    points4.reserve(num_control_points);
    velocity4.reserve(num_control_points);

    computeAcceleration(m_points, m_velocity, acceleration);
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int index = to1D(i, j, k, dim, dim);
                points1[index] = dt * m_velocity[index];
                velocity1[index] = dt * acceleration[index];
                buffer_points[index] = 0.5f * points1[index];
                buffer_velocity[index] = 0.5f * velocity1[index];
                buffer_points[index] += m_points[index];
                buffer_velocity[index] += m_velocity[index];
            }
        }
    }

    computeAcceleration(buffer_points, buffer_velocity, acceleration);
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int index = to1D(i, j, k, dim, dim);
                points2[index] = dt * buffer_velocity[index];
                velocity2[index] = dt * acceleration[index];
                buffer_points[index] = 0.5f * points2[index];
                buffer_velocity[index] = 0.5f * velocity2[index];
                buffer_points[index] += m_points[index];
                buffer_velocity[index] += m_velocity[index];
            }
        }
    }

    computeAcceleration(buffer_points, buffer_velocity, acceleration);
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int index = to1D(i, j, k, dim, dim);
                points3[index] = dt * buffer_velocity[index];
                velocity3[index] = dt * acceleration[index];
                buffer_points[index] = 0.5f * points3[index];
                buffer_velocity[index] = 0.5f * velocity3[index];
                buffer_points[index] += m_points[index];
                buffer_velocity[index] += m_velocity[index];
            }
        }
    }

    computeAcceleration(buffer_points, buffer_velocity, acceleration);
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int index = to1D(i, j, k, dim, dim);
                points4[index] = dt * buffer_velocity[index];
                velocity4[index] = dt * acceleration[index];

                buffer_points[index] = 2.f * points2[index];
                buffer_velocity[index] = 2.f * points3[index];
                buffer_points[index] += buffer_velocity[index];
                buffer_points[index] += points1[index];
                buffer_points[index] += points4[index];
                buffer_points[index] /= 6.f;
                m_points[index] += buffer_points[index];

                buffer_points[index] = 2.f * velocity2[index];
                buffer_velocity[index] = 2.f * velocity3[index];
                buffer_points[index] += buffer_velocity[index];
                buffer_points[index] += velocity1[index];
                buffer_points[index] += velocity4[index];
                buffer_points[index] /= 6.f;
                m_velocity[index] += buffer_points[index];
            }
        }
    }
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
                m_points[to1D(i, j, k, dim, dim)].y += increment;
            }
        }
    }

    calculateNormals();
    m_vertexData.clear();
    loadVAO();
    initializeOpenGLShapeProperties();
}
