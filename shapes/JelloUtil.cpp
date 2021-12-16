#include "JelloUtil.h"
#include "math.h"
#include "Settings.h"
#include "gl/shaders/ShaderAttribLocations.h"

const float epsilon {0.0005f};

namespace JelloUtil {

int to1D(int r, int c, int d, int width, int height) {
    return d * (width * height) + r * (width) + c;
}

int indexFromFace(int i, int j, int dim, FACE face) {
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

int pEquals(float a, float b) {
    return abs(a - b) <= epsilon;
}

bool isInRange(int x, int low, int high) {
    return low <= x && x <= high;
}

glm::vec3 applyDampen(float m_dElastic, glm::vec3 a, glm::vec3 b, glm::vec3 t1_vec, glm::vec3 t2_vec) {
    glm::vec3 l = glm::normalize(a - b);
    return -m_dElastic * glm::dot(t1_vec-t2_vec, l) * l;
}

glm::vec3 applyHooke(float m_kElastic, float rest_len, glm::vec3 a, glm::vec3 b) {
    glm::vec3 l = a - b;
    return -m_kElastic * (glm::length(l) - rest_len) * glm::normalize(l);
}

void computeAcceleration(int param_1,
                         float m_kElastic,
                         float m_dElastic,
                         float m_kCollision,
                         float m_dCollision,
                         float m_mass,
                         const glm::vec3 &m_gravity,
                         std::vector<glm::vec3> &points,
                         std::vector<glm::vec3> &velocity,
                         std::vector<glm::vec3> &acceleration) {
    int dim = param_1 + 1;
    float rest_length = 1.f / (dim-1), rest_shear, rest_bend, rest_diag;
    rest_shear = rest_length * sqrt(2);
    rest_diag = rest_length * sqrt(3);
    rest_bend = rest_length * 2;
    for (int k = 0; k < dim; k++) {
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    glm::vec3 F = glm::vec3(0.f);
                    glm::vec3 fCollide = glm::vec3(0.f);
                    int index = to1D(i, j, k, dim, dim);

                    //Structural
                    if (isInRange(i + 1, 0, dim - 1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j, k, dim, dim)], velocity[index], velocity[to1D(i+1, j, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_length, points[index], points[to1D(i+1, j, k, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim - 1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j, k, dim, dim)], velocity[index], velocity[to1D(i-1, j, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_length, points[index], points[to1D(i-1, j, k, dim, dim)]);
                    }
                    if (isInRange(j + 1, 0, dim - 1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j+1, k, dim, dim)], velocity[index], velocity[to1D(i, j+1, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_length, points[index], points[to1D(i, j+1, k, dim, dim)]);
                    }
                    if (isInRange(j - 1, 0, dim - 1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j-1, k, dim, dim)], velocity[index], velocity[to1D(i, j-1, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_length, points[index], points[to1D(i, j-1, k, dim, dim)]);
                    }
                    if (isInRange(k + 1, 0, dim - 1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j, k+1, dim, dim)], velocity[index], velocity[to1D(i, j, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_length, points[index], points[to1D(i, j, k+1, dim, dim)]);
                    }
                    if (isInRange(k - 1, 0, dim - 1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j, k-1, dim, dim)], velocity[index], velocity[to1D(i, j, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_length, points[index], points[to1D(i, j, k-1, dim, dim)]);
                    }

                    //Shear
                    if (isInRange(i + 1, 0, dim-1) && isInRange(j + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j+1, k, dim, dim)], velocity[index], velocity[to1D(i+1, j+1, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i+1, j+1, k, dim, dim)]);
                    }
                    if (isInRange(i + 1, 0, dim-1) && isInRange(j - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j-1, k, dim, dim)], velocity[index], velocity[to1D(i+1, j-1, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i+1, j-1, k, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(j + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j+1, k, dim, dim)], velocity[index], velocity[to1D(i-1, j+1, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i-1, j+1, k, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(j - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j-1, k, dim, dim)], velocity[index], velocity[to1D(i-1, j-1, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i-1, j-1, k, dim, dim)]);
                    }
                    if (isInRange(j + 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j+1, k+1, dim, dim)], velocity[index], velocity[to1D(i, j+1, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i, j+1, k+1, dim, dim)]);
                    }
                    if (isInRange(j - 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j-1, k+1, dim, dim)], velocity[index], velocity[to1D(i, j-1, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i, j-1, k+1, dim, dim)]);
                    }
                    if (isInRange(j + 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j+1, k-1, dim, dim)], velocity[index], velocity[to1D(i, j+1, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i, j+1, k-1, dim, dim)]);
                    }
                    if (isInRange(j - 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j-1, k-1, dim, dim)], velocity[index], velocity[to1D(i, j-1, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i, j-1, k-1, dim, dim)]);
                    }
                    if (isInRange(i + 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j, k+1, dim, dim)], velocity[index], velocity[to1D(i+1, j, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i+1, j, k+1, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j, k+1, dim, dim)], velocity[index], velocity[to1D(i-1, j, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i-1, j, k+1, dim, dim)]);
                    }
                    if (isInRange(i + 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j, k-1, dim, dim)], velocity[index], velocity[to1D(i+1, j, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i+1, j, k-1, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j, k-1, dim, dim)], velocity[index], velocity[to1D(i-1, j, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_shear, points[index], points[to1D(i-1, j, k-1, dim, dim)]);
                    }

                    //Bend
                    if (isInRange(i + 2, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+2, j, k, dim, dim)], velocity[index], velocity[to1D(i+2, j, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_bend, points[index], points[to1D(i+2, j, k, dim, dim)]);
                    }
                    if (isInRange(i - 2, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-2, j, k, dim, dim)], velocity[index], velocity[to1D(i-2, j, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_bend, points[index], points[to1D(i-2, j, k, dim, dim)]);
                    }
                    if (isInRange(j + 2, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j+2, k, dim, dim)], velocity[index], velocity[to1D(i, j+2, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_bend, points[index], points[to1D(i, j+2, k, dim, dim)]);
                    }
                    if (isInRange(j - 2, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j-2, k, dim, dim)], velocity[index], velocity[to1D(i, j-2, k, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_bend, points[index], points[to1D(i, j-2, k, dim, dim)]);
                    }
                    if (isInRange(k + 2, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j, k+2, dim, dim)], velocity[index], velocity[to1D(i, j, k+2, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_bend, points[index], points[to1D(i, j, k+2, dim, dim)]);
                    }
                    if (isInRange(k - 2, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i, j, k-2, dim, dim)], velocity[index], velocity[to1D(i, j, k-2, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_bend, points[index], points[to1D(i, j, k-2, dim, dim)]);
                    }

                    //Diagonals
                    if (isInRange(i + 1, 0, dim-1) && isInRange(j + 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j+1, k+1, dim, dim)], velocity[index], velocity[to1D(i+1, j+1, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i+1, j+1, k+1, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(j + 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j+1, k+1, dim, dim)], velocity[index], velocity[to1D(i-1, j+1, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i-1, j+1, k+1, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(j - 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j-1, k+1, dim, dim)], velocity[index], velocity[to1D(i-1, j-1, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i-1, j-1, k+1, dim, dim)]);
                    }
                    if (isInRange(i + 1, 0, dim-1) && isInRange(j - 1, 0, dim-1) && isInRange(k + 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j-1, k+1, dim, dim)], velocity[index], velocity[to1D(i+1, j-1, k+1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i+1, j-1, k+1, dim, dim)]);
                    }
                    if (isInRange(i + 1, 0, dim-1) && isInRange(j - 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j-1, k-1, dim, dim)], velocity[index], velocity[to1D(i+1, j-1, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i+1, j-1, k-1, dim, dim)]);
                    }
                    if (isInRange(i + 1, 0, dim-1) && isInRange(j + 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i+1, j+1, k-1, dim, dim)], velocity[index], velocity[to1D(i+1, j+1, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i+1, j+1, k-1, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(j + 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j+1, k-1, dim, dim)], velocity[index], velocity[to1D(i-1, j+1, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i-1, j+1, k-1, dim, dim)]);
                    }
                    if (isInRange(i - 1, 0, dim-1) && isInRange(j - 1, 0, dim-1) && isInRange(k - 1, 0, dim-1)) {
                        F += applyDampen(m_dElastic, points[index], points[to1D(i-1, j-1, k-1, dim, dim)], velocity[index], velocity[to1D(i-1, j-1, k-1, dim, dim)]);
                        F += applyHooke(m_kElastic, rest_diag, points[index], points[to1D(i-1, j-1, k-1, dim, dim)]);
                    }

                    //Bounding box
                    if (points[index].x > 2) {
                        fCollide.x += -m_dCollision * velocity[index].x + m_kCollision*std::fabs(points[index].x - 2) * -1;
                    }

                    if (points[index].x < -2) {
                        fCollide.x += -m_dCollision * velocity[index].x + m_kCollision*std::fabs(points[index].x + 2);
                    }

                    if (points[index].y > 2) {
                        fCollide.y += -m_dCollision * velocity[index].y + m_kCollision*std::fabs(points[index].y - 2) * -1;
                    }

                    if (points[index].y < -2) {
                        fCollide.y += -m_dCollision * velocity[index].y + m_kCollision*std::fabs(points[index].y + 2);
                    }

                    if (points[index].z > 2) {
                        fCollide.z += -m_dCollision * velocity[index].z + m_kCollision*std::fabs(points[index].z - 2) * -1;
                    }

                    if (points[index].z < -2) {
                        fCollide.z += -m_dCollision * velocity[index].z + m_kCollision*std::fabs(points[index].z + 2);
                    }
                    // TODO: We're using one plane, so I can factor out the math, but keeping it for demo purposes laterrrr

                    if (settings.usePlane) {
                        // 3 Points to Define a Plane
                        glm::vec3 a(2, -2, -2);
                        glm::vec3 b(-2, 2, -2);
                        glm::vec3 c(-2,-2, 2);

                        glm::vec3 planeCross = glm::cross(c-b, a-b);
                        glm::vec3 planeNormal = glm::normalize(planeCross);

                        glm::vec3 currentPoint = points[index];
                        float D;

                        if (  // Intersects Plane
                                (D = planeNormal.x * (currentPoint.x - a.x) +
                                  planeNormal.y * (currentPoint.y - a.y) +
                                  planeNormal.z * (currentPoint.z - a.z)) < 0) {

                            // Dampen Velocity
    //                        fCollide += -1.f * 0.5f * velocity[index];
                            fCollide += -1.f * m_dCollision * velocity[index];

                            // m_kCollision * Distance from point to plane * Normal
                            float distToPlane = fabs(glm::dot(planeNormal, currentPoint - a));
    //                                            fCollide += 50 * distToPlane * planeNormal;
                            fCollide += m_kCollision * distToPlane * planeNormal;
                        }
                    }


                    F += fCollide;
                    //Force Field Calculation - by default exerts gravity everywhere
                    //In the future this should taken from as an input
                    F += m_gravity;

                    acceleration[index] = F * 1.0f/m_mass;

                }
          }
    }
}

void rk4(float dt,
         int m_param1,
         float m_kElastic,
         float m_dElastic,
         float m_kCollision,
         float m_dCollision,
         float m_mass,
         const glm::vec3 &m_gravity,
         std::vector<glm::vec3> &points,
         std::vector<glm::vec3> &velocity) {
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);

    std::vector<glm::vec3> buffer_points;
    std::vector<glm::vec3> buffer_velocity;
    buffer_points.reserve(num_control_points);
    buffer_velocity.reserve(num_control_points);

    std::vector<glm::vec3> acceleration;
    acceleration.reserve(num_control_points);

    std::vector<glm::vec3> points1;
    std::vector<glm::vec3> velocity1;
    points1.reserve(num_control_points);
    velocity1.reserve(num_control_points);

    std::vector<glm::vec3> points2;
    std::vector<glm::vec3> velocity2;
    points2.reserve(num_control_points);
    velocity2.reserve(num_control_points);

    std::vector<glm::vec3> points3;
    std::vector<glm::vec3> velocity3;
    points3.reserve(num_control_points);
    velocity3.reserve(num_control_points);

    std::vector<glm::vec3> points4;
    std::vector<glm::vec3> velocity4;
    points4.reserve(num_control_points);
    velocity4.reserve(num_control_points);

    computeAcceleration(
                m_param1, m_kElastic, m_dElastic,
                m_kCollision, m_dCollision, m_mass, m_gravity,
                points, velocity, acceleration);
    for (int i = 0; i < num_control_points; i++) {
        points1[i] = dt * velocity[i];
        velocity1[i] = dt * acceleration[i];
        buffer_points[i] = 0.5f * points1[i];
        buffer_velocity[i] = 0.5f * velocity1[i];
        buffer_points[i] += points[i];
        buffer_velocity[i] += velocity[i];
    }

    computeAcceleration(
                m_param1, m_kElastic, m_dElastic,
                m_kCollision, m_dCollision, m_mass, m_gravity,
                buffer_points, buffer_velocity, acceleration);
    for (int i = 0; i < num_control_points; i++) {
        points2[i] = dt * buffer_velocity[i];
        velocity2[i] = dt * acceleration[i];
        buffer_points[i] = 0.5f * points2[i];
        buffer_velocity[i] = 0.5f * velocity2[i];
        buffer_points[i] += points[i];
        buffer_velocity[i] += velocity[i];
    }

    computeAcceleration(
                m_param1, m_kElastic, m_dElastic,
                m_kCollision, m_dCollision, m_mass, m_gravity,
                buffer_points, buffer_velocity, acceleration);
    for (int i = 0; i < num_control_points; i++) {
        points3[i] = dt * buffer_velocity[i];
        velocity3[i] = dt * acceleration[i];
        buffer_points[i] = 0.5f * points3[i];
        buffer_velocity[i] = 0.5f * velocity3[i];
        buffer_points[i] += points[i];
        buffer_velocity[i] += velocity[i];
    }

    computeAcceleration(
                m_param1, m_kElastic, m_dElastic,
                m_kCollision, m_dCollision, m_mass, m_gravity,
                buffer_points, buffer_velocity, acceleration);
    for (int i = 0; i < num_control_points; i++) {
        points4[i] = dt * buffer_velocity[i];
        velocity4[i] = dt * acceleration[i];

        buffer_points[i] = 2.f * points2[i];
        buffer_velocity[i] = 2.f * points3[i];
        buffer_points[i] += buffer_velocity[i];
        buffer_points[i] += points1[i];
        buffer_points[i] += points4[i];
        buffer_points[i] /= 6.f;
        points[i] += buffer_points[i];

        buffer_points[i] = 2.f * velocity2[i];
        buffer_velocity[i] = 2.f * velocity3[i];
        buffer_points[i] += buffer_velocity[i];
        buffer_points[i] += velocity1[i];
        buffer_points[i] += velocity4[i];
        buffer_points[i] /= 6.f;
        velocity[i] += buffer_points[i];
    }
}

}
