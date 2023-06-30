#pragma once

#include "FreeTypeTarget.h"

class FreeTypeBuffer : public FreeTypeTarget
{
public:
    FreeTypeBuffer(uint8_t *buffer, uint16_t size) : _buffer(buffer), _size(size) {}

    bool reset(int x, int y, int width, int height);
    void cursor(int x, int y);

    bool getClipBox(int32_t *xRasterMin, int32_t *yRasterMin, int32_t *xRasterMax, int32_t *yRasterMax) override;
    void drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans) override;

protected:
    void raster_to_buffer(int32_t rx, int32_t ry, int32_t *bx, int32_t *by);
    void buffer_to_raster(int32_t bx, int32_t by, int32_t *rx, int32_t *ry);

private:
    uint8_t *_buffer;
    uint16_t _size;

    int16_t _x;
    int16_t _y;
    uint16_t _width;
    uint16_t _height;

    int32_t sx, sy, ox, oy;
};
