#ifndef LinearBrush_Header
#define LinearBrush_Header


#include "Brush.h"

/**
 * @class LinearBrush
 *
 * Linear brush, whose mask has linear falloff.
 */
class LinearBrush
    : public Brush
{

public:
    LinearBrush(RGBA color, int radius);
    ~LinearBrush();

    void brushDown(int x, int y, Canvas2D *canvas) override;
    void brushUp(int x, int y, Canvas2D *canvas) override;

protected:
    void makeMask() override; // Constructs the mask for this brush.
};

#endif
