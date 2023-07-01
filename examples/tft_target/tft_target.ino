
#include <Arduino.h>
#include <Adafruit_ILI9341.h>

#include <FreeType_GFX.h>
#include <FreeTypeRaster.h>

#include <Outlines/MaterialIcons-Regular.h>
#include <Outlines/Roboto-Light.h>

#define TFT_CS 33
#define TFT_RST 27
#define TFT_DC 12

FreeType_GFX<Adafruit_ILI9341> tft(TFT_CS, TFT_DC, TFT_RST);

FreeTypeRaster raster;

void setup()
{
    Serial.begin(115200);
    tft.begin();
}

void loop()
{
    tft.fillScreen(ILI9341_WHITE);

    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.setCursor(20, 60);
    raster.render(roboto::light::u0034.outline, &tft);

    tft.setCursor(50, 60);
    raster.render(roboto::light::u0030.outline, &tft);

    tft.setCursor(80, 60);
    raster.render(roboto::light::u0034.outline, &tft);

    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.setCursor(50, 130);
    raster.render(roboto::light::u047E.outline, &tft);

    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
    tft.setCursor(80, 130);
    raster.render(roboto::light::u04DC.outline, &tft);

    tft.setTextColor(UINT565_COLOR(0x60, 0x02, 0xEE), ILI9341_WHITE);
    tft.setCursor(50, 170);
    raster.render(materialicons::regular::uE834.outline, &tft);

    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setCursor(110, 170);
    raster.render(materialicons::regular::uE835.outline, &tft);

    delay(2000);
}
