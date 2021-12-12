#ifndef BBOX_H
#define BBOX_H

#include "Shape.h"

class Bbox : public Shape
{
public:
    Bbox();
    ~Bbox();
    void drawBbox();
    void drawFloor();

    void tick(float current) override;
    virtual void setParam1(int inp) override;
    virtual void setParam2(int inp) override;

private:
    virtual void generateVertexData() override;
};


#endif // BBOX_H
