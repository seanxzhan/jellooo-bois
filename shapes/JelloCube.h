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
    glm::vec3 applyDampen(glm::vec3 a, glm::vec3 b, glm::vec3 t1_vec, glm::vec3 t2_vec);
    glm::vec3 applyHooke(float rest_len, glm::vec3 a, glm::vec3 b);
    //void applyForces(glm::vec3 *F, double rest_len, int i, int j, int k)
    void computeAcceleration(std::vector<glm::vec3> &points,
                             std::vector<glm::vec3> &velocity,
                             std::vector<glm::vec3> &acceleration);
    void euler();
    void rk4();

    //All the related member variables to keep track of
    float m_kElastic; // Hook's elasticity coefficient for all springs except collision springs
    float m_dElastic; // Damping coefficient for all springs except collision springs
    float m_kCollision; // Hook's elasticity coefficient for collision springs
    float m_dCollision; // Damping coefficient collision springs
    float m_mass; // mass of each of the control points, mass assumed to be equal for every control point

    float dt;

    glm::vec3 m_gravity;

    //Standardizations for how to index in comments
    std::vector<glm::vec3> m_points; //points
    std::vector<glm::vec3> m_normals; //normals for each of the 6 faces
    std::vector<glm::vec3> m_velocity; //velocities for each point
};

#endif // JELLOCUBE_H
