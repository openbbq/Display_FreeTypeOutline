#include <Arduino.h>
#include "FreeTypeBuffer.h"

bool FreeTypeBuffer::reset(
    int x,
    int y,
    int width,
    int height)
{
    if (width * height > _size)
    {
        return false;
    }

    _x = x;
    _y = y;
    _width = width;
    _height = height;
    memset(_buffer, 0, width * height);

    sx = 1;
    sy = -1;
    ox = 0 - _x;
    oy = _height - _y;

    log_v("_x:%d _y:%d _width:%d _height:%d", _x, _y, _width, _height);
    log_v("sx:%d sy:%d ox:%d oy:%d", sx, sy, ox, oy);

    return true;
}

void FreeTypeBuffer::cursor(
    int x,
    int y)
{
    ox = x - _x;
    oy = y - _y;
    log_v("ox:%d x:%d _x:%d oy:%d y:%d _y:%d", ox, x, _x, oy, y, _y);
}

bool FreeTypeBuffer::getClipBox(int32_t *xMin, int32_t *yMin, int32_t *xMax, int32_t *yMax)
{
    buffer_to_raster(0, _height - 1, xMin, yMin);
    buffer_to_raster(_width, -1, xMax, yMax);
    return true;
}

void FreeTypeBuffer::raster_to_buffer(int32_t rx, int32_t ry, int32_t *bx, int32_t *by)
{
    *bx = rx * sx + ox;
    *by = ry * sy + oy;
}

void FreeTypeBuffer::buffer_to_raster(int32_t bx, int32_t by, int32_t *rx, int32_t *ry)
{
    *rx = (bx - ox) / sx;
    *ry = (by - oy) / sy;

    log_v("sx:%d sy:%d ox:%d oy:%d", sx, sy, ox, oy);
    log_v("bx:%d by:%d rx:%d ry:%d", bx, by, *rx, *ry);
}

void FreeTypeBuffer::drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans)
{
    int32_t y = yRaster * sy + oy;

    if (y < 0 || y >= _height)
    {
        log_e("invalid y:%d from %d:yRaster", y, yRaster);
        return;
    }

    for (; count; --count, ++spans)
    {
        int32_t x = spans->xRaster * sx + ox;
        if (x < 0 || x + spans->width > _width)
        {
            log_e("invalid x:%d from %d:xRaster %d:width ", x, spans->xRaster, spans->width);
            return;
        }

        uint8_t *p = &_buffer[_width * y + x];
        for (uint16_t n = spans->width; n; --n, ++p)
        {
            *p = spans->opacity;
        }
    }
}
