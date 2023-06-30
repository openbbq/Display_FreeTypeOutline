
#include <Arduino.h>
#include "Display_FreeTypeOutline.h"
#include "FreeTypeRaster.h"

void *ft_alloc(FT_Memory memory, long size)
{
    void *block = malloc(size);
    return block;
}

void *ft_realloc(FT_Memory memory, long cur_size, long new_size, void *block)
{
    return realloc(block, new_size);
}

void ft_free(FT_Memory memory, void *block)
{
    free(block);
}

FT_MemoryRec_ _memory = {nullptr, ft_alloc, ft_free, ft_realloc};

FreeTypeRaster::~FreeTypeRaster()
{
    reset();
}

bool FreeTypeRaster::begin()
{
    reset();

    FT_Error error = ft_grays_raster.raster_new(&_memory, &_raster);
    if (error)
    {
        log_e("ft_grays_raster raster_new error: %d", error);
        return false;
    }

    return true;
}

void FreeTypeRaster::reset()
{
    if (_raster)
    {
        ft_grays_raster.raster_done(_raster);
        _raster = nullptr;
    }
}

void target_callback(int y, int c_span, const FT_Span *spans, void *callbackData)
{
    ((FreeTypeTarget *)callbackData)->drawSpans(y, c_span, (FreeTypeSpan *)spans);
}

FT_Error
FreeTypeRaster::render(const FT_Outline &outline, FreeTypeTarget *target)
{
    FT_BBox clipBox;
    if (target->getClipBox((int32_t *)&clipBox.xMin, (int32_t *)&clipBox.yMin, (int32_t *)&clipBox.xMax, (int32_t *)&clipBox.yMax))
    {
        return render(outline, &clipBox, target_callback, target);
    }
    return render(outline, nullptr, target_callback, target);
}

FT_Error FreeTypeRaster::render(const FT_Outline &outline, const FT_BBox *clipBox, FT_SpanFunc callback, void *callbackData)
{
    FT_Raster_Params params = {0};
    params.source = &outline;
    params.gray_spans = callback;
    params.user = callbackData;
    params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;

    if (clipBox)
    {
        params.clip_box = *clipBox;
    }
    else if (outline.n_points == 0)
    {
        params.clip_box = {0};
    }
    else
    {
        FT_Pos xMin, yMin, xMax, yMax;

        FT_Vector *vec = outline.points;
        FT_Vector *limit = vec + outline.n_points;

        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        for (; vec < limit; vec++)
        {
            FT_Pos x, y;

            x = vec->x;
            if (x < xMin)
                xMin = x;
            if (x > xMax)
                xMax = x;

            y = vec->y;
            if (y < yMin)
                yMin = y;
            if (y > yMax)
                yMax = y;
        }

        log_v("outline bounds %d %d %d %d", xMin, yMin, xMax, yMax);

        params.clip_box.xMin = xMin >> 6;
        params.clip_box.yMin = yMin >> 6;
        params.clip_box.xMax = (xMax + 63) >> 6;
        params.clip_box.yMax = (yMax + 63) >> 6;
    }

    log_v("raster_render clip_box {xMin:%d yMin:%d xMax:%d yMax:%d}, ",
          params.clip_box.xMin,
          params.clip_box.yMin,
          params.clip_box.xMax,
          params.clip_box.yMax);

    if (!_raster)
    {
        if (!begin())
        {
            return -1;
        }
    }

    FT_Error error = ft_grays_raster.raster_render(_raster, &params);
    if (error)
    {
        log_w("ft_grays_raster raster_render error: %d", error);
        return error;
    }

    return 0;
}
