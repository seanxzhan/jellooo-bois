#ifndef JELLOUTIL_H
#define JELLOUTIL_H

#include <vector>
#include <glm/glm.hpp>
#include "GL/glew.h"

#include<memory>

enum FACE {
    BOTTOM,
    TOP,
    FRONT,
    BACK,
    LEFT,
    RIGHT,
};

namespace JelloUtil {

int to1D(int r, int c, int d, int width, int height);

//Convention for indexing points
//Bottom - [dim-1][i][j]
//Top - [0][i][j]
//Front - [i][j][0]
//Back - [i][j][dim-1]
//Left - [i][0][j]
//Right - [i][dim-1][j]
//*Note that some go the opposite way - I'm not sure why but this makes it work so dont' touch it lol
int indexFromFace(int i, int j, int dim, FACE face);

int pEquals(float a, float b);

//Returns whether or not is in range, inclusive
bool isInRange(int x, int low, int high);

glm::vec3 applyDampen(float m_dElastic, glm::vec3 a, glm::vec3 b, glm::vec3 t1_vec, glm::vec3 t2_vec);

glm::vec3 applyHooke(float m_kElastic, float rest_len, glm::vec3 a, glm::vec3 b);

void computeAcceleration(int param_1,
                         float m_kElastic,
                         float m_dElastic,
                         float m_kCollision,
                         float m_dCollision,
                         float m_mass,
                         const glm::vec3 &m_gravity,
                         std::vector<glm::vec3> &points,
                         std::vector<glm::vec3> &velocity,
                         std::vector<glm::vec3> &acceleration);

void rk4(float dt,
         int m_param1,
         float m_kElastic,
         float m_dElastic,
         float m_kCollision,
         float m_dCollision,
         float m_mass,
         const glm::vec3 &m_gravity,
         std::vector<glm::vec3> &points,
         std::vector<glm::vec3> &velocity);

}

#endif // JELLOUTIL_H
