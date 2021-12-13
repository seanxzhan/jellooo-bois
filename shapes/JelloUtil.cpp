#include "JelloUtil.h"
#include "math.h"
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

}
