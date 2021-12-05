#include "JelloCube.h"
#include <iostream>

JelloCube::JelloCube(int param1):
    Shape(param1,param1)
{

}

JelloCube::~JelloCube(){}

void JelloCube::initialize(){
    int num_control_points = pow(m_param1,3);
    points.reserve(num_control_points);
    velocity.reserve(num_control_points);
    acceleration.reserve(num_control_points);

    initializeOpenGLShapeProperties();

}

//Should update positions and call on
void JelloCube::tick(float current) {
    std::cout << "Jello Cube" << std::endl;
    std::cout << current << std::endl;
}
