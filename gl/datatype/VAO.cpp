#include "VAO.h"

#include "VBO.h"
#include "IBO.h"

namespace CS123 { namespace GL {

VAO::VAO(const VBO &vbo, int startingNumberOfVerticesToRender, int endingNumberOfVerticesToRender) :
    m_drawMethod(DRAW_ARRAYS),
    m_handle(0),
    m_startingNumVertices(startingNumberOfVerticesToRender),
    m_endingNumVertices(endingNumberOfVerticesToRender),
    m_size(0),
    m_triangleLayout(vbo.triangleLayout())
{
    glGenVertexArrays(1, &m_handle);

    bind();
    vbo.bindAndEnable();
    unbind();
    vbo.unbind();
}

VAO::VAO(const VBO &vbo, const IBO &ibo, int numberOfVerticesToRender) :
    m_drawMethod(DRAW_INDEXED),
    m_handle(0),
    m_endingNumVertices(numberOfVerticesToRender),
    m_size(0),
    m_triangleLayout(vbo.triangleLayout())
{
    // TODO [OPTIONAL]
    // There's another way of drawing with OpenGL that uses IBOs,
    // or Index Buffer Objects.  Feel free to look them up or ask us
    // about them if you're curious!
    // This constructor should be almost identical to the one above,
    // just also bind the IBO after binding the vbo (and unbind it)
}

VAO::VAO(VAO &&that) :
    m_VBO(std::move(that.m_VBO)),
    m_drawMethod(that.m_drawMethod),
    m_startingNumVertices(that.m_startingNumVertices),
    m_endingNumVertices(that.m_endingNumVertices),
    m_size(that.m_size),
    m_triangleLayout(that.m_triangleLayout)
{
    that.m_handle = 0;
}

VAO& VAO::operator=(VAO &&that) {
    this->~VAO();

    m_VBO = std::move(that.m_VBO);
    m_drawMethod = that.m_drawMethod;
    m_handle = that.m_handle;
    m_startingNumVertices = that.m_startingNumVertices;
    m_endingNumVertices = that.m_endingNumVertices;
    m_size = that.m_size;
    m_triangleLayout = that.m_triangleLayout;

    that.m_handle = 0;

    return *this;
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_handle);
}


void VAO::draw() {
    draw(m_startingNumVertices, m_endingNumVertices);
}

void VAO::draw(int start, int end) {
    glEnable(GL_PROGRAM_POINT_SIZE);
    switch(m_drawMethod) {
        case VAO::DRAW_ARRAYS:
            glDrawArrays(VBO::GEOMETRY_LAYOUT::LAYOUT_POINTS, 0, start);
            glDrawArrays(VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, start, end);
            break;
        case VAO::DRAW_INDEXED:
            // TODO [OPTIONAL]
            // If you want to use IBO's, you'll need to call glDrawElements here
            break;
    }
}

void VAO::bind() {
    glBindVertexArray(m_handle);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

}}
