#include <Arduino.h>
#include "FreeTypePrinter.h"

FreeTypeRaster sharedRaster;

GlyphRange emptyRange = {0, 0};
GlyphFont emptyFont = {nullptr, &emptyRange};

FreeTypePrinter::FreeTypePrinter()
    : _raster(&sharedRaster), _wrapper(emptyFont), _font(&_wrapper)
{
}

FreeTypePrinter::FreeTypePrinter(FreeTypeRaster &raster)
    : _raster(&raster), _wrapper(emptyFont), _font(&_wrapper)
{
}

FT_Error FreeTypePrinter::printChar(char ch)
{
    return printUnicode((uint16_t)(uint8_t)ch);
}

void FreeTypePrinter::spanFunc(int y,
                               int count,
                               const FT_Span *spans,
                               void *user)
{
    ((FreeTypePrinter *)user)->drawSpans(y, count, (FreeTypeSpan *)spans);
}

FT_Error FreeTypePrinter::printUnicode(uint16_t ch)
{
    FT_GlyphSlot glyph = _font->findCharCode(ch);
    if (!glyph)
    {
        log_w("font does not contain U+%04X", ch);
    }
    return print(glyph);
}

FT_Error FreeTypePrinter::print(FT_GlyphSlot glyph)
{
    log_v("render FT_GlyphSlot 0x%08x at {%d,%d}", glyph, _x, _y);
    // TODO: clipbox
    FT_Error err = _raster->render(glyph->outline, nullptr, FreeTypePrinter::spanFunc, this);
    if (err)
    {
        log_d("FT_Error %d rendering", err);
        return err;
    }
    _x += glyph->metrics.horiAdvance >> 6;
    log_v("after FT_GlyphSlot 0x%08x at {%d,%d}", glyph, _x, _y);
    return 0;
}

#define UINT565_COLOR(r, g, b) ((((uint16_t)b) >> 3) |            \
                                ((((uint16_t)g) << 3) & 0x07E0) | \
                                ((((uint16_t)r) << 8) & 0xf800))

#define UINT565_R(c) ((uint16_t(c) & 0xf800) >> 8)
#define UINT565_G(c) ((uint16_t(c) & 0x07E0) >> 3)
#define UINT565_B(c) ((uint16_t(c) & 0x001F) << 3)

// callback from raster
void FreeTypePrinter::drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans)
{
    int32_t y = _y - yRaster;

    log_v("yRaster:%d y:%d spans:%d", yRaster, y, count);
    for (int i = 0; i != count; ++i)
    {
        int32_t x = _x + spans[i].xRaster;
        uint16_t fg_alpha = spans[i].opacity;
        uint16_t fg = this->_foreground;
        uint16_t bg = this->_background;
        uint16_t bg_alpha = 255 - fg_alpha;

        // NOTE: boost foreground for appearance
        bg_alpha = bg_alpha * 3 / 4;
        fg_alpha = 255 - bg_alpha;

        uint16_t c = UINT565_COLOR((UINT565_R(fg) * fg_alpha + UINT565_R(bg) * bg_alpha) >> 8,
                                   (UINT565_G(fg) * fg_alpha + UINT565_G(bg) * bg_alpha) >> 8,
                                   (UINT565_B(fg) * fg_alpha + UINT565_B(bg) * bg_alpha) >> 8);
        _cb(x, y, spans[i].width, c, _param);
    }
}
