#ifndef ConstantBrush_Header
#define ConstantBrush_Header

#include "Brush.h"

/**
 * @class ConstantBrush
 *
 * Constant brush with equal mask distribution in a circle.
 */
class ConstantBrush
    : public Brush
{

public:
    ConstantBrush(RGBA color, int radius);
    ~ConstantBrush();

    void brushDown(int x, int y, Canvas2D *canvas) override;
    void brushUp(int x, int y, Canvas2D *canvas) override;

protected:
    void makeMask() override; // Constructs the mask for this brush.
};

#endif
