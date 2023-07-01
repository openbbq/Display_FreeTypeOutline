#pragma once

#define STANDALONE_
#define FT_CONFIG_STANDARD_LIBRARY_H <stdlib.h>
#include "ftmisc.h"
#include "ftimage.h"
extern const FT_Raster_Funcs ft_standard_raster;
extern const FT_Raster_Funcs ft_grays_raster;

struct FT_Glyph_Metrics
{
    FT_Pos width;
    FT_Pos height;

    FT_Pos horiBearingX;
    FT_Pos horiBearingY;
    FT_Pos horiAdvance;

    FT_Pos vertBearingX;
    FT_Pos vertBearingY;
    FT_Pos vertAdvance;
};

struct FT_GlyphSlotRec_
{
    FT_Glyph_Metrics metrics;
    FT_Outline outline;
};

typedef struct FT_GlyphSlotRec_ *FT_GlyphSlot;

struct GlyphRange
{
    uint16_t first;
    uint16_t count;
};

struct GlyphFont
{
    FT_GlyphSlotRec_ *glyphs;
    GlyphRange *ranges;
};
