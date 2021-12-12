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

void Bbox::drawFloor() {
    std::vector<GLfloat> floorData = {
        -2.f, -2.f, -2.f,
        -2.f, -2.f, 2.f,
        2.f, -2.f, -2.f,
        2.f, -2.f, 2.f
    };
    drawTriangleStrips(floorData);
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
