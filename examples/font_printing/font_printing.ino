
#include <Arduino.h>
#include <Adafruit_ILI9341.h>

#include <FreeType_GFX.h>
#include <FreeTypeRaster.h>
#include <FreeTypePrinter.h>

#include <Outlines/Roboto-Light.h>
#include <Outlines/MaterialIcons-Regular.h>

// namespace roboto::light
// {
//     FT_GlyphSlotRec_ font_glyphs[] = {
//         roboto::light::u0030, // '0'
//         roboto::light::u0031,
//         roboto::light::u0032,
//         roboto::light::u0033,
//         roboto::light::u0034,
//         roboto::light::u0035,
//         roboto::light::u0036,
//         roboto::light::u0037,
//         roboto::light::u0038,
//         roboto::light::u0039,
//     };
//     GlyphRange font_ranges[] = {
//         {0x0030, 10}, // '0'-'9'
//     };
//     GlyphFont font = {font_glyphs, font_ranges};
// }

#define TFT_CS 33
#define TFT_RST 27
#define TFT_DC 12

FreeType_GFX<Adafruit_ILI9341> tft(TFT_CS, TFT_DC, TFT_RST);
FreeTypePrinter printer;

void Adafruit_GFX_drawFastHLine(int32_t x, int32_t y, uint32_t w, uint16_t color, void *param)
{
    ((Adafruit_GFX *)param)->drawFastHLine(x, y, w, color);
}

void setup()
{
    Serial.begin(115200);
    tft.begin();

    printer.callback(Adafruit_GFX_drawFastHLine, &tft);
    printer.setFont(roboto::light::font);

    
    tft.fillScreen(ILI9341_WHITE);

    printer.setCursor(20, 60);

    printer.setTextColor(ILI9341_DARKGREEN, ILI9341_WHITE);
    printer.print("404");

    printer.setCursor(5, 95);

    printer.setTextColor(ILI9341_DARKGREY, ILI9341_WHITE);
    printer.print("Hello, ");
    printer.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    printer.print("world");
    printer.setTextColor(ILI9341_DARKGREY, ILI9341_WHITE);
    printer.print("!");

    printer.setCursor(50, 150);

    printer.setTextColor(UINT565_COLOR(0x60, 0x02, 0xEE), ILI9341_WHITE);
    printer.print(materialicons::regular::uE834);

    printer.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    printer.print(materialicons::regular::uE835);
    printer.print(materialicons::regular::uEB3A);

    printer.setCursor(5, 200);

    printer.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    printer.print(materialicons::regular::uF0A8);
    printer.print(materialicons::regular::uF062);
    printer.print(materialicons::regular::uEACA);

    printer.setTextColor(ILI9341_RED, ILI9341_WHITE);
    printer.print(materialicons::regular::uEA47);
}

void loop()
{
    delay(2000);
}
