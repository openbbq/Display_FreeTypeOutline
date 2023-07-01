#pragma once

#include <Adafruit_GFX.h>
#include <FreeTypeTarget.h>

#define UINT565_COLOR(r, g, b) ((((uint16_t)b) >> 3) |            \
                                ((((uint16_t)g) << 3) & 0x07E0) | \
                                ((((uint16_t)r) << 8) & 0xf800))

#define UINT565_R(c) ((uint16_t(c) & 0xf800) >> 8)
#define UINT565_G(c) ((uint16_t(c) & 0x07E0) >> 3)
#define UINT565_B(c) ((uint16_t(c) & 0x001F) << 3)

template <typename TGFX>
class FreeType_GFX : public TGFX, public FreeTypeTarget
{
public:
    using TGFX::TGFX; // inherit constructors

    void setDrawSpanBlending(bool value) { drawSpanBlending = value; }
    bool getDrawSpanBlending() const { return drawSpanBlending; }

    uint16_t getPixel(int16_t x, int16_t y) const;

    void drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans) override
    {
        uint16_t fg = this->textcolor;
        uint16_t bg = this->textbgcolor;

        int16_t y = this->cursor_y - yRaster;

        log_v("yRaster:%d y:%d spans:%d", yRaster, y, count);
        for (; count; --count, ++spans)
        {
            int16_t x = this->cursor_x + spans->xRaster;
            log_v("xRaster:%d x:%d width:%d opacity:%d", spans->xRaster, x, spans->width, spans->opacity);

            if (spans->opacity == 255)
            {
                this->drawFastHLine(x, y, spans->width, fg);
                continue;
            }

            uint16_t fg_alpha = spans->opacity;
            uint16_t bg_alpha = 255 - fg_alpha;

            if (drawSpanBlending)
            {
                for (uint16_t pixels = spans->width; pixels; --pixels, ++x)
                {
                    bg = this->getPixel(x, y);
                    uint16_t c = UINT565_COLOR((UINT565_R(fg) * fg_alpha + UINT565_R(bg) * bg_alpha) >> 8,
                                               (UINT565_G(fg) * fg_alpha + UINT565_G(bg) * bg_alpha) >> 8,
                                               (UINT565_B(fg) * fg_alpha + UINT565_B(bg) * bg_alpha) >> 8);
                    this->drawPixel(x, y, c);
                }
                continue;
            }

            uint16_t c = UINT565_COLOR((UINT565_R(fg) * fg_alpha + UINT565_R(bg) * bg_alpha) >> 8,
                                       (UINT565_G(fg) * fg_alpha + UINT565_G(bg) * bg_alpha) >> 8,
                                       (UINT565_B(fg) * fg_alpha + UINT565_B(bg) * bg_alpha) >> 8);

            log_v("drawFastHLine %d,%d,%d,%d",x, y, spans->width, c);
            this->drawFastHLine(x, y, spans->width, c);
        }
    }

protected:
    bool drawSpanBlending = false;
};

template <typename TGFX>
uint16_t FreeType_GFX<TGFX>::getPixel(int16_t x, int16_t y) const { return this->textbgcolor; }

template <>
uint16_t FreeType_GFX<GFXcanvas16>::getPixel(int16_t x, int16_t y) const { return GFXcanvas16::getPixel(x, y); }
