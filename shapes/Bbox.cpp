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
    drawLine(lineData, 8);

    lineData = {
        2.f, 1.f, 2.f, 2.f, 1.f, -2.f,
        2.f, 1.f, -2.f, -2.f, 1.f, -2.f,
        -2.f, 1.f, -2.f, -2.f, 1.f, 2.f,
        -2.f, 1.f, 2.f, 2.f, 1.f, 2.f};
    drawLine(lineData, 8);

    lineData = {
        2.f, 0.f, 2.f, 2.f, 0.f, -2.f,
        2.f, 0.f, -2.f, -2.f, 0.f, -2.f,
        -2.f, 0.f, -2.f, -2.f, 0.f, 2.f,
        -2.f, 0.f, 2.f, 2.f, 0.f, 2.f};
    drawLine(lineData, 8);

    lineData = {
        2.f, -1.f, 2.f, 2.f, -1.f, -2.f,
        2.f, -1.f, -2.f, -2.f, -1.f, -2.f,
        -2.f, -1.f, -2.f, -2.f, -1.f, 2.f,
        -2.f, -1.f, 2.f, 2.f, -1.f, 2.f};
    drawLine(lineData, 8);

    // draw bottom
    lineData = {2.f, -2.f, 2.f, 2.f, -2.f, -2.f,
                2.f, -2.f, -2.f, -2.f, -2.f, -2.f,
                -2.f, -2.f, -2.f, -2.f, -2.f, 2.f,
                -2.f, -2.f, 2.f, 2.f, -2.f, 2.f};
    drawLine(lineData, 8);

    // draw vertical pillars separately due to LINE_STRIP
    lineData = {2.f, 2.f, 2.f, 2.f, -2.f, 2.f};
    drawLine(lineData, 2);
    lineData = {2.f, 2.f, -2.f, 2.f, -2.f, -2.f};
    drawLine(lineData, 2);
    lineData = {-2.f, 2.f, -2.f, -2.f, -2.f, -2.f};
    drawLine(lineData, 2);
    lineData = {-2.f, 2.f, 2.f, -2.f, -2.f, 2.f};
    drawLine(lineData, 2);

    lineData = {2.f, 2.f, 1.f, 2.f, -2.f, 1.f};
    drawLine(lineData, 2);
    lineData = {2.f, 2.f, -1.f, 2.f, -2.f, -1.f};
    drawLine(lineData, 2);
    lineData = {-2.f, 2.f, -1.f, -2.f, -2.f, -1.f};
    drawLine(lineData, 2);
    lineData = {-2.f, 2.f, 1.f, -2.f, -2.f, 1.f};
    drawLine(lineData, 2);
    lineData = {2.f, 2.f, 0.f, 2.f, -2.f, 0.f};
    drawLine(lineData, 2);
    lineData = {-2.f, 2.f, 0.f, -2.f, -2.f, 0.f};
    drawLine(lineData, 2);

    lineData = {1.f, 2.f, 2.f, 1.f, -2.f, 2.f};
    drawLine(lineData, 2);
    lineData = {1.f, 2.f, -2.f, 1.f, -2.f, -2.f};
    drawLine(lineData, 2);
    lineData = {-1.f, 2.f, -2.f, -1.f, -2.f, -2.f};
    drawLine(lineData, 2);
    lineData = {-1.f, 2.f, 2.f, -1.f, -2.f, 2.f};
    drawLine(lineData, 2);
    lineData = {0.f, 2.f, 2.f, 0.f, -2.f, 2.f};
    drawLine(lineData, 2);
    lineData = {0.f, 2.f, -2.f, 0.f, -2.f, -2.f};
    drawLine(lineData, 2);
}

void Bbox::drawLine(std::vector<GLfloat> &line, int num_vertices) {
    setVertexData(&line[0], line.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINE_STRIP, num_vertices);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}
