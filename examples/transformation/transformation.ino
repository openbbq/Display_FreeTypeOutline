
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

class FreeTypeOutline : public FT_Outline
{
public:
    FreeTypeOutline(const FT_Outline &source, const FT_Vector &orig, const FT_Vector &horz, const FT_Vector &vert) : FT_Outline(source)
    {
        points = new FT_Vector[n_points];

        FT_Vector *it1 = source.points;
        FT_Vector *it2 = points;
        for (short n = n_points; n; it1++, it2++, n--)
        {
            it2->x = (orig.x + it1->x * horz.x + it1->y * vert.x) / 64;
            it2->y = (orig.y + it1->x * horz.y + it1->y * vert.y) / 64;
        }
    }

    ~FreeTypeOutline()
    {
        delete points;
    }
};

void loop()
{
    uint32_t m = millis();

    tft.fillScreen(ILI9341_WHITE);

    roboto::light::u04DC;
    for (int i = 0; i != 10; ++i)
    {
        double scale = 64 * (i + 1) / 2.0;
        double theta = i * (1.0 + (m % 100) / 50.0) + m;
        FreeTypeOutline adjusted(materialicons::regular::uE834.outline,
                                 FT_Vector{0, 0},
                                 FT_Vector{cos(theta) * scale, sin(theta) * scale},
                                 FT_Vector{-sin(theta) * scale, cos(theta) * scale});

        tft.setTextColor(UINT565_COLOR(0x60, 0x02, 0xEE), ILI9341_WHITE);
        tft.setCursor(tft.width() / 2, tft.height() / 2);
        raster.render(adjusted, &tft);
    }

    delay(2000);
}
