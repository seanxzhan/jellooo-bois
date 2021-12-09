#include "OpenGLShape.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"

#include <iostream>

using namespace CS123::GL;

OpenGLShape::OpenGLShape() :
    m_VAO(nullptr),
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_startNumVertices(0),
    m_endNumVertices(0)
{

}

OpenGLShape::~OpenGLShape()
{
}

void OpenGLShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void OpenGLShape::initializeOpenGLShapeProperties() {
    const int numFloatsPerVertex = 6;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, 0, numVertices);
}

void OpenGLShape::setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode,
                                int start, int end) {
    // Store the vertex data and other values to be used later when constructing the VAO
    m_data = data;
    m_size = size;
    m_drawMode = drawMode;
    m_startNumVertices = start;
    m_endNumVertices = end;
}

void OpenGLShape::setAttribute(
         GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type, bool normalize) {
    m_markers.push_back(VBOAttribMarker(index, numElementsPerVertex, offset, type, normalize));
}

void OpenGLShape::buildVAO() {
    CS123::GL::VBO vbo = VBO(m_data, m_size, m_markers, m_drawMode);
    m_VAO = std::make_unique<VAO>(vbo, m_startNumVertices, m_endNumVertices);
}

void OpenGLShape::drawPoints(std::vector<GLfloat> &points, int start, int end) {
//    int num_points = points.size() / 3;
    setVertexData(&points[0], points.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_POINTS, start, end);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

void OpenGLShape::drawLine(std::vector<GLfloat> &line, int start, int end) {
//    int num_vertices = line.size() / 3;
    setVertexData(&line[0], line.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINE_STRIP, start, end);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

void OpenGLShape::drawLines(std::vector<GLfloat> &line, int start, int end) {
//    int num_vertices = line.size() / 3;
    setVertexData(&line[0], line.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, start, end);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

void OpenGLShape::drawPointsAndLines(std::vector<GLfloat> &points, std::vector<GLfloat> &lines) {
    int total_num_vertices = (int) points.size() / 3 + (int) lines.size() / 3;
    std::vector<GLfloat> AB;
    AB.resize(points.size());
    for (int i = 0; i < points.size(); i++) {
        AB[i] = points[i];
    }
    std::cout << "before: " << std::to_string(AB.size()) << std::endl;
    AB.insert(AB.end(), lines.begin(), lines.end());
    std::cout << "after: " << std::to_string(AB.size()) << std::endl;
    setVertexData(&AB[0], AB.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_POINTS, (int) points.size() / 3, total_num_vertices);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    buildVAO();
//    draw();
//    setVertexData(&points[0], lines.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, (int) points.size() / 3, total_num_vertices);
//    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

