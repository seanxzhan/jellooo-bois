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

    //Getters and setters for constants
    float getkElastic();
    void setkElastic(float kElastic);
    float getdElastic();
    void setdElastic(float dCollision);

    float getkCollision();
    void setkCollision(float kCollision);
    float getdCollision();
    void setdCollision(float dCollision);

    float getMass();
    void setMass(float mass);

    float getGravity();
    void setGravity(float yValue);
private:
    virtual void generateVertexData() override;

    void calculateNormals();
    void loadVAO();

    void euler();

    //All the related member variables to keep track of
    float m_kElastic; // Hook's elasticity coefficient for all springs except collision springs
    float m_dElastic; // Damping coefficient for all springs except collision springs
    float m_kCollision; // Hook's elasticity coefficient for collision springs
    float m_dCollision; // Damping coefficient collision springs
    float m_mass; // mass of each of the control points, mass assumed to be equal for every control point

    float m_dt;

    glm::vec3 m_gravity;

    //Standardizations for how to index in comments
    std::vector<glm::vec3> m_points; //points
    std::vector<glm::vec3> m_normals; //normals for each of the 6 faces
    std::vector<glm::vec3> m_velocity; //velocities for each point
};

#endif // JELLOCUBE_H
