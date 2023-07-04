#include <Arduino.h>
#include "Font_FTO.h"
#include "FreeTypePrinter.h"

namespace display
{
    Rect Font_FTO::measure(const String &text) const
    {
        FT_Pos horiAdvance = 0;
        for (auto it = text.begin(); it != text.end(); it++)
        {
            FT_GlyphSlot glyph = this->findCharCode((uint8_t)*it);
            horiAdvance += glyph->metrics.horiAdvance;
        }

        int asc = (_font.metrics.ascender + 63) >> 6;
        int desc = (-_font.metrics.descender + 63) >> 6;

        // TODO(loudej) left and right glyph overflow
        // also - ascender and descender are not the same as line height
        Rect rc(0, -asc, (horiAdvance + 63) >> 6, desc);
        log_v("measured %s for %s", rc.toString().c_str(), text.c_str());
        return rc;
    }

    void Font_FTO_span(int32_t x, int32_t y, uint32_t w, uint32_t color, void *param)
    {
        DrawContext *ctx = (DrawContext *)param;
        ctx->draw(Rect(x, y, x + w, y + 1), color);
    }

    void Font_FTO::draw(DrawContext *ctx, const Rect &clipBox, Color background, Color foreground, int16_t x, int16_t y, const String &text) const
    {
        log_d("draw {%d,%d} in %s for %s", x,y,clipBox.toString().c_str(), text.c_str());

        FreeTypePrinter printer;
        printer.callback(Font_FTO_span, ctx);
        printer.setForeground(foreground);
        printer.setBackground(background);
        printer.setCursor(x, y);
        printer.setFont(this);
        // TODO(loudej) setFontSize here also to scale outlines from their native size
        // TODO(loudej) set clip box so rasterizer does less work
        printer.print(text.c_str());
    }
} // namespace display
