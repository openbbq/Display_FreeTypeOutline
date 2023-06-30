
#include <Arduino.h>
#include <Adafruit_ILI9341.h>

#include <FreeTypeRaster.h>

#include <Outlines/MaterialIcons-Regular.h>
#include <Outlines/Roboto-Light.h>

#define TFT_CS 33
#define TFT_RST 27
#define TFT_DC 12

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

FreeTypeRaster raster;

class FreeTypeGFX : public FreeTypeTarget
{
public:
    FreeTypeGFX(Adafruit_GFX &graphics) : _graphics(&graphics) {}

    void setTextColor(uint16_t c, uint16_t bg)
    {
        textcolor = c;
        textbgcolor = bg;
    }

    bool getClipBox(int32_t *xRasterMin, int32_t *yRasterMin, int32_t *xRasterMax, int32_t *yRasterMax) { return false; }
    void drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans) override;

private:
    Adafruit_GFX *_graphics;
    uint16_t textcolor;   ///< 16-bit background color for print()
    uint16_t textbgcolor; ///< 16-bit text color for print()
};

#define ILI9341_COLOR(r, g, b) ((((uint16_t)b) >> 3) |            \
                                ((((uint16_t)g) << 3) & 0x07E0) | \
                                ((((uint16_t)r) << 8) & 0xf800))

#define ILI9341_R(c) ((uint16_t(c) & 0xf800) >> 8)
#define ILI9341_G(c) ((uint16_t(c) & 0x07E0) >> 3)
#define ILI9341_B(c) ((uint16_t(c) & 0x001F) << 3)

void FreeTypeGFX::drawSpans(int32_t yRaster, int32_t count, const FreeTypeSpan *spans)
{
    uint16_t fg = textcolor;
    uint16_t bg = textbgcolor;

    int16_t y = _graphics->getCursorY() - yRaster;
    for (; count; --count, ++spans)
    {
        int16_t x = _graphics->getCursorX() + spans->xRaster;

        uint16_t c = fg;
        if (spans->opacity != 255)
        {
            uint16_t fg_alpha = spans->opacity;
            uint16_t bg_alpha = 255 - fg_alpha;
            c = ILI9341_COLOR((ILI9341_R(fg) * fg_alpha + ILI9341_R(bg) * bg_alpha) >> 8,
                              (ILI9341_G(fg) * fg_alpha + ILI9341_G(bg) * bg_alpha) >> 8,
                              (ILI9341_B(fg) * fg_alpha + ILI9341_B(bg) * bg_alpha) >> 8);
        }

        _graphics->drawFastHLine(x, y, spans->width, c);
    }
}

FreeTypeGFX target(tft);

void setup()
{
    Serial.begin(115200);
    tft.begin();
}

void loop()
{
    tft.fillScreen(ILI9341_WHITE);

    target.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.setCursor(20, 60);
    raster.render(roboto::light::u0034, &target);

    tft.setCursor(50, 60);
    raster.render(roboto::light::u0030, &target);

    tft.setCursor(80, 60);
    raster.render(roboto::light::u0034, &target);

    target.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.setCursor(50, 130);
    raster.render(roboto::light::u047E, &target);

    target.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
    tft.setCursor(80, 130);
    raster.render(roboto::light::u04DC, &target);

    target.setTextColor(ILI9341_COLOR(0x60, 0x02, 0xEE), ILI9341_WHITE);
    tft.setCursor(50, 170);
    raster.render(materialicons::regular::uE834, &target);

    target.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setCursor(110, 170);
    raster.render(materialicons::regular::uE835, &target);

    delay(2000);
}
