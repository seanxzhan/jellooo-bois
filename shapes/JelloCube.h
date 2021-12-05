#ifndef JELLOCUBE_H
#define JELLOCUBE_H

#include "Shape.h"

class JelloCube : public Shape
{
public:
    JelloCube(int param1);
    ~JelloCube();
    void tick(float current) override;

private:
    void initialize();

    //All the related member variables to keep track of
    double kElastic; // Hook's elasticity coefficient for all springs except collision springs
    double dElastic; // Damping coefficient for all springs except collision springs
    double kCollision; // Hook's elasticity coefficient for collision springs
    double dCollision; // Damping coefficient collision springs
    double mass; // mass of each of the control points, mass assumed to be equal for every control point
    std::vector<glm::vec3> points; //points for each point
    std::vector<glm::vec3> velocity; //velocities for each point
    std::vector<glm::vec3> acceleration; //acceleration for each point
};

#endif // JELLOCUBE_H
