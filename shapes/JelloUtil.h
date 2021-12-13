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

}

#endif // JELLOUTIL_H
