/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
//#include <iostream>

CamtransCamera::CamtransCamera()
{
    // Initialize like the demo (lab 07)
    m_near = 1;
    m_far = 30;
    m_thetaH = 60;
    m_aspectRatio = 1;
    m_eye = glm::vec4(2, 2, 2, 1);
    // Using this instead of explicit look vector member variable
    m_w = -1.0f * glm::normalize(
                glm::vec4(0, 0, 0, 1) - m_eye);
    m_up = glm::vec4(0, 1, 0, 0);

    m_v = m_up - glm::dot(m_up, m_w)*m_w;
    m_v = glm::normalize(m_v);
    m_u = glm::vec4(
                glm::cross(glm::vec3(m_v), glm::vec3(m_w)),
                    0);

    updateViewMatrix();
    updateProjectionMatirx();
}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatirx();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    glm::mat4x4 val = m_perspectiveTransformation * m_scaleMatrix;
    return val;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -1.f * m_w;
}

glm::vec4 CamtransCamera::getUp() const {
//    return m_up;
    return m_v;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;

    m_w = -1.f * look;
    m_w = glm::normalize(m_w);

    m_v = m_up - glm::dot(m_up, m_w)*m_w;
    m_v = glm::normalize(m_v);
    m_u = glm::vec4(
                glm::cross(glm::vec3(m_v), glm::vec3(m_w)),
                    0);

    updateViewMatrix();
    updateProjectionMatirx();

}

void CamtransCamera::setHeightAngle(float h) {
    m_thetaH = h;
    updateProjectionMatirx();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye = m_eye + v;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    float radians = glm::radians(degrees);
    glm::vec4 y = glm::vec4(0, 1, 0, 0);
    glm::vec4 z = glm::vec4(0, 0, 1, 0);

    glm::mat4x4 rotateX = glm::mat4x4(
                glm::vec4(1, 0, 0, 0),
                glm::vec4(0, glm::cos(radians), glm::sin(radians), 0),
                glm::vec4(0, -1.f * glm::sin(radians), glm::cos(radians), 0),
                glm::vec4(0, 0, 0, 1));

    y = rotateX * y;
    z = rotateX * z;

    glm::mat4x4 cameraToWorldM = glm::inverse(getViewMatrix());
    m_v = cameraToWorldM * y;
    m_w = cameraToWorldM * z;
    m_v = glm::normalize(m_v);
    m_w = glm::normalize(m_w);
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    float radians = glm::radians(degrees);
    glm::vec4 x = glm::vec4(1, 0, 0, 0);
    glm::vec4 z = glm::vec4(0, 0, 1, 0);

    glm::mat4x4 rotateY = glm::mat4x4(
                glm::vec4(glm::cos(radians), 0, -1.f * glm::sin(radians), 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(glm::sin(radians), 0, glm::cos(radians), 0),
                glm::vec4(0, 0, 0, 1));

    x = rotateY * x;
    z = rotateY * z;

    glm::mat4x4 cameraToWorldM = glm::inverse(getViewMatrix());
    m_u= cameraToWorldM * x;
    m_w = cameraToWorldM * z;
    m_u = glm::normalize(m_u);
    m_w = glm::normalize(m_w);
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    float radians = glm::radians(degrees);
    glm::vec4 x = glm::vec4(1, 0, 0, 0);
    glm::vec4 y = glm::vec4(0, 1, 0, 0);

    glm::mat4x4 rotateZ = glm::mat4x4(
                glm::vec4(glm::cos(radians), glm::sin(radians), 0, 0),
                glm::vec4(-1.f * glm::sin(radians), glm::cos(radians), 0, 0),
                glm::vec4(0, 0, 1, 0),
                glm::vec4(0, 0, 0, 1));

    x = rotateZ * x;
    y = rotateZ * y;

    glm::mat4x4 cameraToWorldM = glm::inverse(getViewMatrix());
    m_u = cameraToWorldM * x;
    m_v = cameraToWorldM * y;
    m_u = glm::normalize(m_u);
    m_v = glm::normalize(m_v);
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatirx();
}

void CamtransCamera::updateProjectionMatirx() {
    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {
    // Remember to homogenize points are applying this transform
    float c = -1.f * m_near/m_far;
    m_perspectiveTransformation = glm::mat4x4(
                glm::vec4(1, 0, 0, 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(0, 0, -1.f/(1+c), -1),
                glm::vec4(0, 0, 1.f*c/(1+c), 0));
}

void CamtransCamera::updateScaleMatrix() {
    float val = glm::tan(glm::radians(m_thetaH)/2);
    m_scaleMatrix = glm::mat4x4(
                glm::vec4(1.f/(m_far * m_aspectRatio * val), 0, 0, 0),
                glm::vec4(0, 1.f/(m_far * val), 0, 0),
                glm::vec4(0, 0, 1.f/m_far, 0),
                glm::vec4(0, 0, 0, 1));
}

void CamtransCamera::updateViewMatrix() {
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix() {
    m_rotationMatrix = glm::mat4x4(
                glm::vec4(m_u.x, m_v.x, m_w.x, 0),
                glm::vec4(m_u.y, m_v.y, m_w.y, 0),
                glm::vec4(m_u.z, m_v.z, m_w.z, 0),
                glm::vec4(0, 0, 0, 1));
}

void CamtransCamera::updateTranslationMatrix() {
    m_translationMatrix = glm::mat4x4(
                glm::vec4(1, 0, 0, 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(0, 0, 1, 0),
                glm::vec4(-1.f * m_eye.x, -1.f * m_eye.y, -1.f * m_eye.z, 1));
}
