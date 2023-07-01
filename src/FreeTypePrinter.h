#pragma once

#include "FreeTypeTarget.h"
#include "FreeTypeRaster.h"
#include "FreeTypeFont.h"

// contains references to a raster, font [glyph metrics and outlines],
// cursor, orientation?, colors
//
// provides spanfunc for raster to call, draw spans onto graphics device
//
class FreeTypePrinter
{
public:
    FreeTypePrinter();
    FreeTypePrinter(FreeTypeRaster &raster);

    void callback(void (*cb)(int32_t x, int32_t y, uint32_t w, uint32_t color, void *param), void *param)
    {
        _cb = cb;
        _param = param;
    }

    void setFont(FreeTypeFont &value) { _font = &value; }
    void setFont(FreeTypeFont *value) { _font = value; }
    void setFont(const GlyphFont &value) { setFont(_wrapper = FreeTypeFont(value)); }
    void setFont(const GlyphFont *value) { setFont(_wrapper = FreeTypeFont(value)); }
    FreeTypeFont *getFont() const { return _font; }

    void setForeground(uint16_t value) { _foreground = value; }
    uint16_t getForeground() const { return _foreground; }

    void setBackground(uint16_t value) { _background = value; }
    uint16_t getBackground() const { return _background; }

    void setTextColor(uint16_t foreground, uint16_t background)
    {
        setForeground(foreground);
        setBackground(background);
    }

    void setCursor(int16_t x, int16_t y)
    {
        _x = x;
        _y = y;
    }

    void getCursor(int16_t *x, int16_t *y) const
    {
        *x = _x;
        *y = _y;
    }

    FT_Error printChar(char ch);
    FT_Error printUnicode(uint16_t ch);
    FT_Error print(FT_GlyphSlot glyph);
    FT_Error print(FT_GlyphSlotRec_ &glyph) { return print(&glyph); }

    void print(const char *text)
    {
        while (*text)
        {
            printChar(*text++);
        }
    }

protected:
    static void spanFunc(int y, int count, const FT_Span *spans, void *user);
    void drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans);

    void (*_cb)(int32_t x, int32_t y, uint32_t w, uint32_t color, void *param);
    void *_param;

    FreeTypeRaster *_raster;
    FreeTypeFont *_font;
    FreeTypeFont _wrapper;

    int16_t _x = 0;
    int16_t _y = 0;
    uint16_t _foreground = 0xffffff;
    uint16_t _background = 0x000000;
};
