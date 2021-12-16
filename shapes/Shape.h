#ifndef SHAPE_H
#define SHAPE_H

#include "OpenGLShape.h"
#include <cmath>

// Used to represent 3D shapes
class Shape : public OpenGLShape
{
public:
    Shape(int param1);
    virtual ~Shape();

    //Getters
    int getParam1();
    int getParam2();

    //Setters
    virtual void setParam1(int inp);
    virtual void setParam2(int inp);

    void tick(float current) override;
    void setGravity(float scale, glm::vec3 gravity) override;

protected:
    int m_param1;
    int m_param2;

    // generates vertex and normal data
    virtual void generateVertexData() = 0;

    // Pushes vec3 and normal as floats to m_vertexData
    void pushVertexAsFloats(const glm::vec3 &inp, const glm::vec3 &norm);

    // Helper to push a rectangle counter-clockwise as 2 trianngles
    void pushRectangleAsFloats(
            const glm::vec3 &inp1, const glm::vec3 &norm1,
            const glm::vec3 &inp2, const glm::vec3 &norm2,
            const glm::vec3 &inp3, const glm::vec3 &norm3,
            const glm::vec3 &inp4, const glm::vec3 &norm4);

};

#endif // SHAPE_H
