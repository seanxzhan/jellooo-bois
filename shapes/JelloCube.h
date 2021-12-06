#ifndef JELLOCUBE_H
#define JELLOCUBE_H

#include "Shape.h"
#include "JelloUtil.h"

using namespace JelloUtil;

class JelloCube : public Shape
{
public:
    JelloCube();
    JelloCube(int param1, int param2);
    ~JelloCube();
    void tick(float current) override;

    virtual void setParam1(int inp) override;
    virtual void setParam2(int inp) override;

private:
    virtual void generateVertexData() override;

    void calculateNormals();
    void loadVAO();
    void computeAcceleration(std::vector<glm::vec3> &points,
                             std::vector<glm::vec3> &velocity,
                             std::vector<glm::vec3> &acceleration);
    void euler();
    void rk4();

    //All the related member variables to keep track of
    double kElastic; // Hook's elasticity coefficient for all springs except collision springs
    double dElastic; // Damping coefficient for all springs except collision springs
    double kCollision; // Hook's elasticity coefficient for collision springs
    double dCollision; // Damping coefficient collision springs
    double mass; // mass of each of the control points, mass assumed to be equal for every control point

    float dt;

    //Standardizations for how to index in comments
    std::vector<glm::vec3> m_points; //points
    std::vector<glm::vec3> m_normals; //normals for each of the 6 faces
    std::vector<glm::vec3> m_velocity; //velocities for each point
};

#endif // JELLOCUBE_H
