/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

Canvas2D::Canvas2D() :
    // Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_brush(nullptr),
    m_brushType(-1)
{

}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);
}

void Canvas2D::settingsChanged() {
    // Process changes to the application settings.
//    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;

    int s_brushType = settings.brushType;

    if (s_brushType != m_brushType)
    {
        m_brushType = s_brushType;
        switch (s_brushType) {
            case BRUSH_CONSTANT:
//                std::cout << "brush type changed to constant" << std::endl;
                m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
                break;
            case BRUSH_LINEAR:
//                std::cout << "brush type changed to linear" << std::endl;
                m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
                break;
            case BRUSH_QUADRATIC:
//                std::cout << "brush type changed to quadratic" << std::endl;
                m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);
                break;
            case BRUSH_SMUDGE:
//                std::cout << "brush type changed to smudge" << std::endl;
                m_brush = std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
                break;
            default:
//                m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
                std::cout << "other brushes have no-impl" << std::endl;
        }
        return;
    }

    int curr_radius = m_brush->getRadius();
    int s_radius = settings.brushRadius;

    if (curr_radius != s_radius)
    {
//        std::cout << "brush radius changed" << std::endl;
        m_brush->setRadius(s_radius);
        return;
    }

    RGBA curr_RGBA = m_brush->getRGBA();
    RGBA s_RGBA = settings.brushColor;

    if (curr_RGBA != s_RGBA)
    {
//        std::cout << "brush rgba changed" << std::endl;
        m_brush->setRGBA(s_RGBA);
        return;
    }
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

//    std::cout << "Canvas2d::mouseDown() called" << std::endl;

    m_brush->brushDown(x,y,this);

//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y) {
    // [BRUSH] Mouse interaction for Brush.
//    std::cout << "Canvas2d::mouseDragged() called" << std::endl;

    m_brush->brushDragged(x,y,this);

}

void Canvas2D::mouseUp(int x, int y) {
    // [BRUSH] Mouse interaction for Brush.
//    std::cout << "Canvas2d::mouseUp() called" << std::endl;

    m_brush->brushUp(x,y,this);
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch(settings.filterType) {
        case FILTER_BLUR:
            // ...
            break;
            // fill in the rest
    }

    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(Camera *camera, int width, int height) {
    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.

    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




