#include "JelloUtil.h"

namespace JelloUtil {

int to1D(int r, int c, int d, int width, int height) {
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

}
