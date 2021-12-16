#ifndef EXAMPLESHAPE2_H
#define EXAMPLESHAPE2_H

#include "OpenGLShape.h"
#include <QTimer>

// hand-written cube points and normals.. if only there were a way to do this procedurally
#define CUBE_DATA_POSITIONS_2 {\
        -10.f,-10.f,-10.f, \
        -1.f, 0.f, 0.f, \
        -10.f,-10.f, 10.f,\
        -1.f, 0.f, 0.f, \
        -10.f, 10.f, 10.f, \
        -1.f, 0.f, 0.f, \
        10.f, 10.f,-10.f, \
        0.f, 0.f, -1.f, \
        -10.f,-10.f,-10.f,\
        0.f, 0.f, -1.f, \
        -10.f, 10.f,-10.f, \
        0.f, 0.f, -1.f, \
        10.f,-10.f, 10.f, \
        0.f, -1.f, 0.f, \
        -10.f,-10.f,-10.f, \
        0.f, -1.f, 0.f, \
        10.f,-10.f,-10.f, \
        0.f, -1.f, 0.f, \
        10.f, 10.f,-10.f, \
        0.f, 0.f, -1.f, \
        10.f,-10.f,-10.f, \
        0.f, 0.f, -1.f, \
        -10.f,-10.f,-10.f, \
        0.f, 0.f, -1.f, \
        -10.f,-10.f,-10.f, \
        -1.f, 0.f, 0.f, \
        -10.f, 10.f, 10.f,\
        -1.f, 0.f, 0.f, \
        -10.f, 10.f,-10.f,\
        -1.f, 0.f, 0.f, \
        10.f,-10.f, 10.f,\
        0.f, -1.f, 0.f, \
        -10.f,-10.f, 10.f,\
        0.f, -1.f, 0.f, \
        -10.f,-10.f,-10.f,\
        0.f, -1.f, 0.f, \
        -10.f, 10.f, 10.f,\
        0.f, 0.f, 1.f, \
        -10.f,-10.f, 10.f,\
        0.f, 0.f, 1.f, \
        10.f,-10.f, 10.f,\
        0.f, 0.f, 1.f, \
        10.f, 10.f, 10.f,\
        1.f, 0.f, 0.f, \
        10.f,-10.f,-10.f,\
        1.f, 0.f, 0.f, \
        10.f, 10.f,-10.f,\
        1.f, 0.f, 0.f, \
        10.f,-10.f,-10.f,\
        1.f, 0.f, 0.f, \
        10.f, 10.f, 10.f,\
        1.f, 0.f, 0.f, \
        10.f,-10.f, 10.f,\
        1.f, 0.f, 0.f, \
        10.f, 10.f, 10.f,\
        0.f, 1.f, 0.f, \
        10.f, 10.f,-10.f,\
        0.f, 1.f, 0.f, \
        -10.f, 10.f,-10.f,\
        0.f, 1.f, 0.f, \
        10.f, 10.f, 10.f,\
        0.f, 1.f, 0.f, \
        -10.f, 10.f,-10.f,\
        0.f, 1.f, 0.f, \
        -10.f, 10.f, 10.f,\
        0.f, 1.f, 0.f, \
        10.f, 10.f, 10.f,\
        0.f, 0.f, 1.f, \
        -10.f, 10.f, 10.f,\
        0.f, 0.f, 1.f, \
        10.f,-10.f, 10.f, \
        0.f, 0.f, 1.f}


class ExampleShape2 : public OpenGLShape
{
public:
    ExampleShape2();
    ExampleShape2(int param1, int param2);
    ~ExampleShape2();
    void tick(float current) override;
    void setGravity(float scale, glm::vec3 new_direction) override;

private:
    int m_param1;
    int m_param2;

};


#endif // EXAMPLESHAPE2_H
