#include "OpenGLShape.h"
#include "gl/datatype/VAO.h"
#include "gl/shaders/ShaderAttribLocations.h"

#include <iostream>

using namespace CS123::GL;

OpenGLShape::OpenGLShape() :
    m_VAO(nullptr),
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_numVertices(0)
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

void OpenGLShape::drawPandL() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->drawPL(VBO::GEOMETRY_LAYOUT::LAYOUT_POINTS, VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, m_cutoff);
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
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}

/**
 * @param data - Vector of floats containing the vertex data.
 * @param size - number of elements in the array.
 * @param drawMode - Drawing mode
 * @param numVertices - Number of vertices to be rendered.
 */
void OpenGLShape::setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices) {
    m_data = data;
    m_size = size;
    m_drawMode = drawMode;
    m_numVertices = numVertices;
}


/**
 * @param name OpenGL handle to the attribute location. These are specified in ShaderAttribLocations.h
 * @param numElementsPerVertex Number of elements per vertex. Must be 1, 2, 3 or 4 (e.g. position = 3 for x,y,z)
 * @param offset Offset in BYTES from the start of the array to the beginning of the first element
 * @param type Primitive type (FLOAT, INT, UNSIGNED_BYTE)
 * @param normalize
 */
void OpenGLShape::setAttribute(GLuint name, GLuint numElementsPerVertex, int offset,
                               VBOAttribMarker::DATA_TYPE type, bool normalize) {
    m_markers.push_back(VBOAttribMarker(name, numElementsPerVertex, offset, type, normalize));
}

void OpenGLShape::buildVAO() {
    CS123::GL::VBO vbo = VBO(m_data, m_size, m_markers, m_drawMode);
    m_VAO = std::make_unique<VAO>(vbo, m_numVertices);
}

void OpenGLShape::drawPoints(std::vector<GLfloat> &points) {
    int num_vertices = points.size() / 3;
    setVertexData(&points[0], points.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_POINTS, num_vertices);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}

void OpenGLShape::drawLines(std::vector<GLfloat> &line) {
    int num_vertices = line.size() / 3;
    setVertexData(&line[0], line.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, num_vertices);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

void OpenGLShape::drawTriangleStrips(std::vector<GLfloat> &data) {
    int num_vertices = data.size() / 3;
    setVertexData(&data[0], data.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, num_vertices);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();
}

void OpenGLShape::drawPointsAndLines(const std::vector<GLfloat> &points,
                                     const std::vector<GLfloat> &lines) {
    int total_num_vertices = (int) points.size() / 3 + (int) lines.size() / 3;
    m_cutoff = (int) points.size() / 3;
    std::vector<GLfloat> AB = points;
    AB.insert(AB.end(), lines.begin(), lines.end());
    setVertexData(&AB[0], AB.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, total_num_vertices);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}
