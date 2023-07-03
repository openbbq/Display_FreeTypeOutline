#pragma once

#include <Display.h>
#include <display/Kind.h>
#include <display/Font.h>

#include "FreeTypeFont.h"

namespace display
{
    class Font_FTO : public Font, public FreeTypeFont
    {
    public:
        Font_FTO(const GlyphFont &font) : FreeTypeFont(font) {}
        Font_FTO(const GlyphFont *font) : FreeTypeFont(*font) {}
        Font_FTO(const GlyphFont &font, float size) : FreeTypeFont(font), _size(size) {}
        Font_FTO(const GlyphFont *font, float size) : FreeTypeFont(*font), _size(size) {}

        template <typename... _Args>
        static FontPtr create(_Args &&...__args) { return std::make_shared<Font_FTO>(std::forward<_Args>(__args)...); }

        int kind() const override { return Kind::ID<Font_FTO>(); }
        Rect measure(const String &text) const override;
        void draw(DrawContext *ctx, const Rect &clipBox, Color background, Color foreground, int16_t x, int16_t y, const String &text) const override;

    private:
        float _size;
    };
} // namespace display
