/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include <cmath>

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::brushDown(int x, int y, Canvas2D *canvas) {
}

void ConstantBrush::brushUp(int x, int y, Canvas2D *canvas) {
}

void ConstantBrush::makeMask() {
    // [BRUSH] Set up the mask for your Constant brush here...
}


