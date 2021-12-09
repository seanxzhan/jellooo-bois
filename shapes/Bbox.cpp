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

void Bbox::drawBbox() {
    std::vector<GLfloat> lineData = {
        2.f, 2.f, 2.f, 2.f, 2.f, -2.f,
        2.f, 2.f, -2.f, -2.f, 2.f, -2.f,
        -2.f, 2.f, -2.f, -2.f, 2.f, 2.f,
        -2.f, 2.f, 2.f, 2.f, 2.f, 2.f};
    drawLine(lineData);

    lineData = {
        2.f, 1.f, 2.f, 2.f, 1.f, -2.f,
        2.f, 1.f, -2.f, -2.f, 1.f, -2.f,
        -2.f, 1.f, -2.f, -2.f, 1.f, 2.f,
        -2.f, 1.f, 2.f, 2.f, 1.f, 2.f};
    drawLine(lineData);

    lineData = {
        2.f, 0.f, 2.f, 2.f, 0.f, -2.f,
        2.f, 0.f, -2.f, -2.f, 0.f, -2.f,
        -2.f, 0.f, -2.f, -2.f, 0.f, 2.f,
        -2.f, 0.f, 2.f, 2.f, 0.f, 2.f};
    drawLine(lineData);

    lineData = {
        2.f, -1.f, 2.f, 2.f, -1.f, -2.f,
        2.f, -1.f, -2.f, -2.f, -1.f, -2.f,
        -2.f, -1.f, -2.f, -2.f, -1.f, 2.f,
        -2.f, -1.f, 2.f, 2.f, -1.f, 2.f};
    drawLine(lineData);

    // draw bottom
    lineData = {2.f, -2.f, 2.f, 2.f, -2.f, -2.f,
                2.f, -2.f, -2.f, -2.f, -2.f, -2.f,
                -2.f, -2.f, -2.f, -2.f, -2.f, 2.f,
                -2.f, -2.f, 2.f, 2.f, -2.f, 2.f};
    drawLine(lineData);

    // draw vertical pillars separately due to LINE_STRIP
    lineData = {2.f, 2.f, 2.f, 2.f, -2.f, 2.f};
    drawLine(lineData);
    lineData = {2.f, 2.f, -2.f, 2.f, -2.f, -2.f};
    drawLine(lineData);
    lineData = {-2.f, 2.f, -2.f, -2.f, -2.f, -2.f};
    drawLine(lineData);
    lineData = {-2.f, 2.f, 2.f, -2.f, -2.f, 2.f};
    drawLine(lineData);

    lineData = {2.f, 2.f, 1.f, 2.f, -2.f, 1.f};
    drawLine(lineData);
    lineData = {2.f, 2.f, -1.f, 2.f, -2.f, -1.f};
    drawLine(lineData);
    lineData = {-2.f, 2.f, -1.f, -2.f, -2.f, -1.f};
    drawLine(lineData);
    lineData = {-2.f, 2.f, 1.f, -2.f, -2.f, 1.f};
    drawLine(lineData);
    lineData = {2.f, 2.f, 0.f, 2.f, -2.f, 0.f};
    drawLine(lineData);
    lineData = {-2.f, 2.f, 0.f, -2.f, -2.f, 0.f};
    drawLine(lineData);

    lineData = {1.f, 2.f, 2.f, 1.f, -2.f, 2.f};
    drawLine(lineData);
    lineData = {1.f, 2.f, -2.f, 1.f, -2.f, -2.f};
    drawLine(lineData);
    lineData = {-1.f, 2.f, -2.f, -1.f, -2.f, -2.f};
    drawLine(lineData);
    lineData = {-1.f, 2.f, 2.f, -1.f, -2.f, 2.f};
    drawLine(lineData);
    lineData = {0.f, 2.f, 2.f, 0.f, -2.f, 2.f};
    drawLine(lineData);
    lineData = {0.f, 2.f, -2.f, 0.f, -2.f, -2.f};
    drawLine(lineData);
}
