#ifndef SpecialBrush_Header
#define SpecialBrush_Header

#include "QuadraticBrush.h"

/**
 * @class SmudgeBrush
 *
 * Smudge brush, which smudges color values across the canvas as the mouse is dragged.
 */
class SmudgeBrush : public QuadraticBrush {

public:
    SmudgeBrush(RGBA color, int radius);
    ~SmudgeBrush();

    void setRadius(int radius) override;

    void brushDown(int x, int y, Canvas2D *canvas) override;
    void brushDragged(int x, int y, Canvas2D *canvas) override;
    void brushUp(int x, int y, Canvas2D *canvas) override;

protected:
    //! Copy paint in the bounding box of the brush from canvas to m_paint
    void pickUpPaint(int x, int y, Canvas2D* canvas);
};

#endif
