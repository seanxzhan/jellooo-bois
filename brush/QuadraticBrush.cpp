/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <cmath>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::brushDown(int x, int y, Canvas2D *canvas) {
}

void QuadraticBrush::brushUp(int x, int y, Canvas2D *canvas) {
    // no-impl
}

void QuadraticBrush::makeMask() {
    // [BRUSH] Set up the mask for your Quadratic brush here...
}


