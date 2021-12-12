#include "SpringMassCube.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>
#include "Settings.h"

SpringMassCube::SpringMassCube(int param1, int param2):
    Shape(param1,param2)
{
    generateVertexData();
}

SpringMassCube::~SpringMassCube(){}

void SpringMassCube::setParam1(int inp) {
    m_param1 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
}

void SpringMassCube::setParam2(int inp) {
    m_param2 = (inp < 1) ? 1 : inp;
    m_vertexData.clear();
    generateVertexData();
}

void SpringMassCube::generateVertexData(){
    int dim = m_param1 + 1;
    int num_control_points = pow(dim,3);
    m_points.reserve(num_control_points);

    //Initialize points
    float incr = 1.f / m_param1;

    //Convention for indexing into cube
    // points[0][0][0] is (-0.5f, 0.5f, 0.5f)
    // points[dim - 1][dim - 1][dim - 1] is (0.5f, -0.5f, -0.5f)
//    glm::vec3 start = glm::vec3(-0.5f, 0.5f, 0.5f);
    std::vector<GLfloat> start = {-0.5f, 0.5f, 0.5f};
    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
//                m_points.push_back(start + glm::vec3(j * incr, i * -incr, k * -incr));
                m_points.push_back(start[0] + j * incr);
                m_points.push_back(start[1] + i * -incr);
                m_points.push_back(start[2] + k * -incr);
            }
        }
    }
}

void SpringMassCube::tick(float current) {
    //This just goes up and down - should involve call to compute acceleration and using RK4 integration
    float increment = sin(current) / 60;
    int dim = m_param1 + 1;
    //k depth (z)
    for (int k = 0; k < dim; k++) {
        //i is the row (y)
        for (int i = 0; i < dim; i++) {
            //j is the column (x)
            for (int j = 0; j < dim; j++) {
//                m_points[to1D(i, j, k, dim, dim)].y += increment;
                m_points[3 * to1D(i, j, k, dim, dim) + 1] += increment;
            }
        }
    }

    switch (settings.cnnctnType) {
        case C_STRUCT:
            m_structural_cnnctns.clear();
            make_structural_connections();
            drawPointsAndLines(m_points, m_structural_cnnctns);
        break;
        case C_SHEAR:
            m_shear_cnnctns.clear();
            make_shear_connections();
            drawPointsAndLines(m_points, m_shear_cnnctns);
        break;
        case C_BEND:
            m_bend_cnnctns.clear();
            make_bend_connections();
            drawPointsAndLines(m_points, m_bend_cnnctns);
        break;
        default:
            std::cout << "you should never see this message" << std::endl;
        break;
    }
}

std::vector<int> SpringMassCube::get_neighbor(int j, int i, int k,
                                              int offset_j, int offset_i, int offset_k) {
    int new_j = j + offset_j;
    int new_i = i + offset_i;
    int new_k = k + offset_k;
    if (!((new_i>m_param1) || (new_i<0) || (new_j>m_param1) || (new_j<0) || (new_k>m_param1) || (new_k<0))
//            uncomment the following two lines to get a bounding box!! crazy
//            && ((i==0) || (i==m_param1) || (j==0) || (j==m_param1) || (k==0) || (k==m_param1))
//            && ((new_i==0) || (new_i==m_param1) || (new_j==0) || (new_j==m_param1) || (new_k==0) || (new_k==m_param1))
            ) {
        std::vector<int> ret = {j, i, k, new_j, new_i, new_k};
        return ret;
    } else {
        std::vector<int> ret = {-1};
        return ret;
    }
}

void SpringMassCube::add_to_connections(std::vector<GLfloat> &connections,
                                        const std::vector<int> &indices) {
    if (indices[0] != -1) {
        int dim = m_param1 + 1;
        // first push the original point
        connections.push_back(
                    m_points[3*to1D(
                        indices[1], indices[0], indices[2],
                        dim, dim)]);
        connections.push_back(
                    m_points[3*to1D(
                        indices[1], indices[0], indices[2],
                        dim, dim)+1]);
        connections.push_back(
                    m_points[3*to1D(
                        indices[1], indices[0], indices[2],
                        dim, dim)+2]);
        // then push the neighboring point
        connections.push_back(
                    m_points[3*to1D(
                        indices[4], indices[3], indices[5],
                        dim, dim)]);
        connections.push_back(
                    m_points[3*to1D(
                        indices[4], indices[3], indices[5],
                        dim, dim)+1]);
        connections.push_back(
                    m_points[3*to1D(
                        indices[4], indices[3], indices[5],
                        dim, dim)+2]);
    }
}

void SpringMassCube::make_structural_connections() {
    // edge case: abs(pos(i)) == abs(pos(j)) == abs(pos(k)) == 0.5
    int dim = m_param1 + 1;
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                add_to_connections(m_structural_cnnctns, get_neighbor(j, i, k, 1, 0, 0));
                add_to_connections(m_structural_cnnctns, get_neighbor(j, i, k, 0, 1, 0));
                add_to_connections(m_structural_cnnctns, get_neighbor(j, i, k, 0, 0, 1));
                add_to_connections(m_structural_cnnctns, get_neighbor(j, i, k, -1, 0, 0));
                add_to_connections(m_structural_cnnctns, get_neighbor(j, i, k, 0, -1, 0));
                add_to_connections(m_structural_cnnctns, get_neighbor(j, i, k, 0, 0, -1));
            }
        }
    }
}

void SpringMassCube::make_shear_connections() {
    // edge case: abs(pos(i)) == abs(pos(j)) == abs(pos(k)) == 0.5
    int dim = m_param1 + 1;
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, 1, 0));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, 1, 0));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, -1, 0));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, -1, 0));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 0, 1, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 0, -1, 1));

                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 0, -1, -1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 0, 1, -1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, 0, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, 0, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, 0, -1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, 0, -1));

                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, 1, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, 1, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, -1, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, -1, 1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, 1, -1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, 1, -1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, -1, -1, -1));
                add_to_connections(m_shear_cnnctns, get_neighbor(j, i, k, 1, -1, -1));
            }
        }
    }
}

void SpringMassCube::make_bend_connections() {
    // edge case: abs(pos(i)) == abs(pos(j)) == abs(pos(k)) == 0.5
    int dim = m_param1 + 1;
    for (int k = 0; k < dim; k++) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                add_to_connections(m_bend_cnnctns, get_neighbor(j, i, k, 2, 0, 0));
                add_to_connections(m_bend_cnnctns, get_neighbor(j, i, k, 0, 2, 0));
                add_to_connections(m_bend_cnnctns, get_neighbor(j, i, k, 0, 0, 2));
                add_to_connections(m_bend_cnnctns, get_neighbor(j, i, k, -2, 0, 0));
                add_to_connections(m_bend_cnnctns, get_neighbor(j, i, k, 0, -2, 0));
                add_to_connections(m_bend_cnnctns, get_neighbor(j, i, k, 0, 0, -2));
            }
        }
    }
}
