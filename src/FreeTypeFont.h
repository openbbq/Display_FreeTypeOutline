#pragma once

#include "third_party/freetype/ft.h"

class FreeTypeFont
{
public:
    FreeTypeFont(const GlyphFont &font) : _font(font) {}
    FreeTypeFont(const GlyphFont *font) : _font(*font) {}

    FT_GlyphSlot getGlyphSlot(int index) const
    {
        return &_font.glyphs[index];
    }

    FT_GlyphSlot findCharCode(uint16_t charCode) const
    {
        GlyphRange *range = _font.ranges;
        int index = 0;
        while (range->count)
        {
            if (charCode >= range->first && charCode < range->first + range->count)
            {
                index += charCode - range->first;
                return &_font.glyphs[index];
            }
            index += range->count;
            range++;
        }
        return nullptr;
    }

protected:
    GlyphFont _font;
};
