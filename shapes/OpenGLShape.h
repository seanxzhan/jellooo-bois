#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include "GL/glew.h"

#include<memory>
#include <vector>

#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

namespace CS123 { namespace GL {
class VAO;
}}

class OpenGLShape
{
public:
    OpenGLShape();
    virtual ~OpenGLShape();
    void draw();

    // Added by Marc - from Lab11
    /** Initialize the VBO with the given vertex data. */
    void setVertexData(GLfloat *data, int size, CS123::GL::VBO::GEOMETRY_LAYOUT drawMode, int numVertices);

    /** Enables the specified attribute and calls glVertexAttribPointer with the given arguments. */
    void setAttribute(GLuint index, GLuint numElementsPerVertex, int offset, CS123::GL::VBOAttribMarker::DATA_TYPE type,
                      bool normalize);

    /** Build the VAO given the specified vertex data and atrributes */
    void buildVAO();

protected:
    /**
     * initializes the relavant openGL properties for the shape
     * don't worry about what exactly this function is doing, you'll learn more about that later in the course!
     * look at ExampleShape.cpp for it's demonstrated usage
     */
    void initializeOpenGLShapeProperties();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;

    // Added by Marc - from Lab11
    GLfloat *m_data;                            /// vector of floats containing the vertex data.
    GLsizeiptr m_size;                          /// size of the data array, in bytes.
    CS123::GL::VBO::GEOMETRY_LAYOUT m_drawMode;            /// drawing mode
    int m_numVertices;                          /// number of vertices to be rendered
    std::vector<CS123::GL::VBOAttribMarker> m_markers;     /// list of VBOAttribMarkers that describe how the data is laid out.
};

#endif // OPENGLSHAPE_H
