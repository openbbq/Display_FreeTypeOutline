#pragma once

#include "FreeTypeTarget.h"

#include "third_party/freetype/ft.h"

class FreeTypeRaster
{
public:
    ~FreeTypeRaster();

    bool begin();
    void reset();

    FT_Error render(const FT_Outline &source, FreeTypeTarget *target);

    FT_Error render(const FT_Outline &source, const FT_BBox *clipBox, FT_SpanFunc callback, void *callbackData = nullptr);

private:
    FT_Raster _raster = nullptr;
};
