#ifndef JELLOCUBE_H
#define JELLOCUBE_H

#include "Shape.h"

enum FACE {
    BOTTOM,
    TOP,
    FRONT,
    BACK,
    LEFT,
    RIGHT,
};

inline int to1D(int r, int c, int d, int width, int height) {
    return d * (width * height) + r * (width) + c;
}

//Convention for indexing points
//Bottom - [dim-1][i][j]
//Top - [0][i][j]
//Front - [i][j][0]
//Back - [i][j][dim-1]
//Left - [i][0][j]
//Right - [i][dim-1][j]
//*Note that some go the opposite way - I'm not sure why but this makes it work so dont' touch it lol
inline int indexPoints(int i, int j, int dim, FACE face) {
    switch(face) {
        case BOTTOM: {
            return to1D(dim-1, i, dim-1-j, dim, dim);
        }
        case TOP: {
            return to1D(0, i, j, dim, dim);
        }
        case FRONT: {
            return to1D(i, j, 0, dim, dim);
        }
        case BACK: {
            return to1D(i, dim-1-j, dim-1, dim, dim);
        }
        case LEFT: {
            return to1D(i, 0, dim-1-j, dim, dim);
        }
        case RIGHT: {
            return to1D(i, dim-1, j, dim, dim);
        }
    }
}

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
    void loadVAO();

    //All the related member variables to keep track of
    double kElastic; // Hook's elasticity coefficient for all springs except collision springs
    double dElastic; // Damping coefficient for all springs except collision springs
    double kCollision; // Hook's elasticity coefficient for collision springs
    double dCollision; // Damping coefficient collision springs
    double mass; // mass of each of the control points, mass assumed to be equal for every control point

    //Standardizations for how to index in comments
    std::vector<glm::vec3> points; //points for each point
    std::vector<glm::vec3> normals; //normals for each of the 6 faces
    std::vector<glm::vec3> velocity; //velocities for each point
    std::vector<glm::vec3> acceleration; //acceleration for each point
};

#endif // JELLOCUBE_H
