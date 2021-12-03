/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <cmath>
#include <iostream>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::brushDown(int x, int y, Canvas2D *canvas) {
}

void LinearBrush::brushUp(int x, int y, Canvas2D *canvas){

}

void LinearBrush::makeMask() {
    // [BRUSH] Set up the mask for your Linear brush here...
}


