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

typedef uint16_t FT_UShort;
typedef int32_t FT_Fixed;

typedef struct FT_Size_Metrics_
{
    FT_UShort x_ppem; /* horizontal pixels per EM               */
    FT_UShort y_ppem; /* vertical pixels per EM                 */

    FT_Fixed x_scale; /* scaling values used to convert font    */
    FT_Fixed y_scale; /* units to 26.6 fractional pixels        */

    FT_Pos ascender;    /* ascender in 26.6 frac. pixels          */
    FT_Pos descender;   /* descender in 26.6 frac. pixels         */
    FT_Pos height;      /* text height in 26.6 frac. pixels       */
    FT_Pos max_advance; /* max horizontal advance, in 26.6 pixels */

} FT_Size_Metrics;

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
    FT_Size_Metrics metrics;
};
