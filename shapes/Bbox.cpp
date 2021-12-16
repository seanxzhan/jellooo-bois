#include "Bbox.h"
#include "gl/shaders/ShaderAttribLocations.h"

Bbox::Bbox():
    Shape(0, 0)
{}

Bbox::~Bbox(){}

void Bbox::tick(float current) {
}

void Bbox::setParam1(int inp) {
}

void Bbox::setParam2(int inp) {
}

void Bbox::generateVertexData(){
}

void Bbox::setGravity(float scale, glm::vec3 new_direction) {
}

void Bbox::drawFloor() {
    std::vector<GLfloat> floorData = {
        -2.f, -2.f, -2.f,
        -2.f, -2.f, 2.f,
        2.f, -2.f, -2.f,
        2.f, -2.f, 2.f
    };
    drawTriangleStrips(floorData);
    glCullFace(GL_FRONT);
    drawTriangleStrips(floorData);
    glCullFace(GL_BACK);
}

void Bbox::drawPlane() {
    std::vector<GLfloat> planeData = {
        -2.f, 2.f, -2.f, // b
        -2.f, -2.f, 2.f, // c
        2.f, -2.f, -2.f // a
//        2.f, -2.f, 2.f
    };
    drawTriangleStrips(planeData);
    glCullFace(GL_FRONT);
    drawTriangleStrips(planeData);
    glCullFace(GL_BACK);
}

void Bbox::drawBbox() {
    std::vector<GLfloat> lineData = {
        2.f, 2.f, 2.f, 2.f, 2.f, -2.f,
        2.f, 2.f, -2.f, -2.f, 2.f, -2.f,
        -2.f, 2.f, -2.f, -2.f, 2.f, 2.f,
        -2.f, 2.f, 2.f, 2.f, 2.f, 2.f,
        2.f, 1.f, 2.f, 2.f, 1.f, -2.f,
        2.f, 1.f, -2.f, -2.f, 1.f, -2.f,
        -2.f, 1.f, -2.f, -2.f, 1.f, 2.f,
        -2.f, 1.f, 2.f, 2.f, 1.f, 2.f,
        2.f, 0.f, 2.f, 2.f, 0.f, -2.f,
        2.f, 0.f, -2.f, -2.f, 0.f, -2.f,
        -2.f, 0.f, -2.f, -2.f, 0.f, 2.f,
        -2.f, 0.f, 2.f, 2.f, 0.f, 2.f,
        2.f, -1.f, 2.f, 2.f, -1.f, -2.f,
        2.f, -1.f, -2.f, -2.f, -1.f, -2.f,
        -2.f, -1.f, -2.f, -2.f, -1.f, 2.f,
        -2.f, -1.f, 2.f, 2.f, -1.f, 2.f,
        2.f, -2.f, 2.f, 2.f, -2.f, -2.f,
        2.f, -2.f, -2.f, -2.f, -2.f, -2.f,
        -2.f, -2.f, -2.f, -2.f, -2.f, 2.f,
        -2.f, -2.f, 2.f, 2.f, -2.f, 2.f,
        2.f, 2.f, 2.f, 2.f, -2.f, 2.f,
        2.f, 2.f, -2.f, 2.f, -2.f, -2.f,
        -2.f, 2.f, -2.f, -2.f, -2.f, -2.f,
        -2.f, 2.f, 2.f, -2.f, -2.f, 2.f,
        2.f, 2.f, 1.f, 2.f, -2.f, 1.f,
        2.f, 2.f, -1.f, 2.f, -2.f, -1.f,
        -2.f, 2.f, -1.f, -2.f, -2.f, -1.f,
        -2.f, 2.f, 1.f, -2.f, -2.f, 1.f,
        2.f, 2.f, 0.f, 2.f, -2.f, 0.f,
        -2.f, 2.f, 0.f, -2.f, -2.f, 0.f,
        1.f, 2.f, 2.f, 1.f, -2.f, 2.f,
        1.f, 2.f, -2.f, 1.f, -2.f, -2.f,
        -1.f, 2.f, -2.f, -1.f, -2.f, -2.f,
        -1.f, 2.f, 2.f, -1.f, -2.f, 2.f,
        0.f, 2.f, 2.f, 0.f, -2.f, 2.f,
        0.f, 2.f, -2.f, 0.f, -2.f, -2.f};
    drawLines(lineData);
}
