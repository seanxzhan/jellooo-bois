#ifndef JELLOUTIL_H
#define JELLOUTIL_H

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
int indexFromFace(int i, int j, int dim, FACE face);

}

#endif // JELLOUTIL_H
