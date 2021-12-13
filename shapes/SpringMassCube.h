#ifndef SPRINGMASSCUBE_H
#define SPRINGMASSCUBE_H

#include "Shape.h"
#include "JelloUtil.h"

using namespace JelloUtil;

class SpringMassCube : public Shape
{
public:
    SpringMassCube(int param1, int param2);
    ~SpringMassCube();
    void tick(float current) override;

    virtual void setParam1(int inp) override;
    virtual void setParam2(int inp) override;

private:
    virtual void generateVertexData() override;
    std::vector<int> get_neighbor(int j, int i, int k,
                                  int offset_j, int offset_i, int offset_k);
    void add_to_connections(std::vector<GLfloat> &connections,
                            const std::vector<int> &indices);

    // format: x, y, z, x, y, z, ...
    std::vector<GLfloat> m_points;
    // format: point 1 -- point 2, point 2 -- point 3, ...
    std::vector<GLfloat> m_structural_cnnctns;
    std::vector<GLfloat> m_shear_cnnctns;
    std::vector<GLfloat> m_bend_cnnctns;
    void make_structural_connections();
    void make_shear_connections();
    void make_bend_connections();
};

#endif // SPRINGMASSCUBE_H
