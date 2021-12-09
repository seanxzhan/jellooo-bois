#ifndef SPRINGMASSCUBE_H
#define SPRINGMASSCUBE_H

#include "Shape.h"
#include "JelloUtil.h"

using namespace JelloUtil;

class SpringMassCube : public Shape
{
public:
    SpringMassCube();
    SpringMassCube(int param1, int param2);
    ~SpringMassCube();
    void tick(float current) override;

    virtual void setParam1(int inp) override;
    virtual void setParam2(int inp) override;

private:
    virtual void generateVertexData() override;

    std::vector<GLfloat> m_points;
    void drawPoints(std::vector<GLfloat> &points);
};

#endif // SPRINGMASSCUBE_H
