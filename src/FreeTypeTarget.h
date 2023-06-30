#pragma once

struct FreeTypeSpan
{
    int16_t xRaster;
    uint16_t width;
    uint8_t opacity;
};

class FreeTypeTarget
{
public:
    virtual bool getClipBox(int32_t *xRasterMin, int32_t *yRasterMin, int32_t *xRasterMax, int32_t *yRasterMax)
    {
        return false;
    }
    
    virtual void drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans) = 0;
};
