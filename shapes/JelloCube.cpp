#include "JelloCube.h"
#include "JelloUtil.h"
#include <iostream>

JelloCube::JelloCube():
    Shape(8,8),
    m_kElastic(200),
    m_dElastic(0.15),
    m_kCollision(200),
    m_dCollision(0.25),
    m_mass(0.001953),
    m_dt(0.001),
    m_gravity(glm::vec3(0.f, -1.f, 0.f))
{
    generateVertexData();
}

JelloCube::JelloCube(int param1, int param2):
    Shape(param1,param2),
    m_kElastic(1000),
    m_dElastic(0.25),
    m_kCollision(200),
    m_dCollision(0.25),
    m_mass(0.001953),
    m_dt(0.001),
    m_gravity(glm::vec3(0.f, -1.f, 0.f))
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

float JelloCube::getkElastic() {
    return m_kElastic;
}

void JelloCube::setkElastic(float kElastic) {
    m_kElastic = kElastic;
}

float JelloCube::getdElastic() {
    return m_dElastic;
}

void JelloCube::setdElastic(float dCollision) {
    m_dCollision = dCollision;
}

float JelloCube::getkCollision() {
    return m_kCollision;
}

void JelloCube::setkCollision(float kCollision) {
    m_kCollision = kCollision;
}

float JelloCube::getdCollision() {
    return m_dCollision;
}

void JelloCube::setdCollision(float dCollision) {
    m_dCollision = dCollision;
}

float JelloCube::getMass() {
    return m_mass;
}

void JelloCube::setMass(float mass) {
    m_mass = mass;
}

float JelloCube::getGravity() {
    return m_gravity.y;
}

void JelloCube::setGravity(float yValue) {
    m_gravity.y = yValue;
}

void JelloCube::generateVertexData(){
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);
    m_points.reserve(num_control_points);
    m_velocity.reserve(num_control_points);
    m_velocity.insert(m_velocity.begin(), num_control_points, glm::vec3(30.f, 30.f, 30.f));
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

    //Load VAO for each of the 6 faces with points and normals
    calculateNormals();
    loadVAO();

    initializeOpenGLShapeProperties();

}

// Convention for indexing into normals
// 6 2D slices, each slice is of dimension dim x dim for each face
// normals [i][j][FACE] gives the normal at (i,j) given enum FACE

//Computes normals for points at arbitrary points
void JelloCube::calculateNormals() {
    int dim = m_param1 + 1;
    int total = 6 * dim * dim;
    for (int i = 0; i < total; i++) {
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

    for (int i = 0; i < total; i++) {
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

void JelloCube::euler() {
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);

    std::vector<glm::vec3> acceleration;
    acceleration.reserve(num_control_points);

    computeAcceleration(
                m_param1, m_kElastic, m_dElastic,
                m_kCollision, m_dCollision, m_mass, m_gravity,
                m_points, m_velocity, acceleration);

    for (int i = 0; i < num_control_points; i ++) {
        m_points[i] += m_dt * m_velocity[i];
        m_velocity[i] += m_dt * acceleration[i];
    }

}

//Should update positions and call on loadVAO and initializeOpenGLShapeProperties() to prep for drawing again
void JelloCube::tick(float current) {
    rk4(m_dt, m_param1, m_kElastic, m_dElastic, m_kCollision, m_dCollision,
        m_mass, m_gravity, m_points, m_velocity);
    calculateNormals();
    m_vertexData.clear();
    loadVAO();
    initializeOpenGLShapeProperties();
}
