#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

using namespace std;
ofstream outfile;

FT_Error error;

static void Panic(const char *message)
{
    fprintf(stderr, "%s\n  error code = 0x%04x\n", message, error);
    exit(1);
}

void usage()
{
    Panic("nope");
}

typedef enum OutputFormat_
{
    OUTPUT_HEADER = 0,
    OUTPUT_BINARY

} OutputFormat;

void put(ofstream &o, int8_t n)
{
    o.put(n);
}
void put(ofstream &o, int16_t n)
{
    o.put(n).put(n >> 8);
}
void put(ofstream &o, int32_t n)
{
    o.put(n).put(n >> 8).put(n >> 16).put(n >> 24);
}
void put(ofstream &o, uint32_t n)
{
    o.put(n).put(n >> 8).put(n >> 16).put(n >> 24);
}
void put(ofstream &o, FT_Pos n)
{
    o.put(n).put(n >> 8).put(n >> 16).put(n >> 24);
}

FT_ULong charcode;

struct UName
{
    UName(uint16_t cc) : charcode(cc) {}

    uint16_t charcode;
};

std::ostream &operator<<(std::ostream &out, UName const &value)
{
    return out << "u" << setfill('0') << setw(4) << hex << uppercase << value.charcode;
}

struct FirstAndCount
{
    uint16_t first;
    uint16_t count;
};

std::ostream &operator<<(std::ostream &out, FirstAndCount const &value)
{
    return out << "{0x" << setfill('0') << setw(4) << hex << uppercase << value.first << ", " << dec << value.count << "}";
}

std::ostream &operator<<(std::ostream &out, FT_Glyph_Metrics const &metrics)
{
    return out << "{" << dec
               << metrics.width << ","
               << metrics.height << ","
               << metrics.horiBearingX << ","
               << metrics.horiBearingY << ","
               << metrics.horiAdvance << ","
               << metrics.vertBearingX << ","
               << metrics.vertBearingY << ","
               << metrics.vertAdvance
               << "}";
}

std::ostream &operator<<(std::ostream &out, FT_Outline const &outline)
{
    return out << "{"
               << dec << outline.n_contours << ", "
               << dec << outline.n_points << ", "
               << "u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_points, "
               << "u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_tags, "
               << "u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_contours, "
               << "0x" << hex << outline.flags
               << "}";
}

std::ostream &operator<<(std::ostream &out, FT_GlyphSlot slot)
{
    return out << "{" << slot->metrics << ", " << slot->outline << "}";
}

std::ostream &operator<<(std::ostream &out, FT_Size_Metrics const &metrics)
{
    return out << "{" << endl
               << setw(8) << setfill(' ') << metrics.x_ppem << ", // x_ppem" << endl
               << setw(8) << setfill(' ') << metrics.y_ppem << ", // y_ppem" << endl
               << setw(8) << setfill(' ') << metrics.x_scale << ", // x_scale" << endl
               << setw(8) << setfill(' ') << metrics.y_scale << ", // y_scale" << endl
               << setw(8) << setfill(' ') << metrics.ascender << ", // ascender (" << metrics.ascender / 64.0 << "px)" << endl
               << setw(8) << setfill(' ') << metrics.descender << ", // descender (" << metrics.descender / 64.0 << "px)" << endl
               << setw(8) << setfill(' ') << metrics.height << ", // height (" << metrics.height / 64.0 << "px)" << endl
               << setw(8) << setfill(' ') << metrics.max_advance << "  // max_advance (" << metrics.max_advance / 64.0 << "px)" << endl
               << "}";
}

int main(int argc, const char *const *argv)
{
    FT_Library library;
    error = FT_Init_FreeType(&library);

    FT_Face face;

    ofstream outfile;
    OutputFormat format = OUTPUT_HEADER;
    bool include_unicode = false;
    bool include_font = true;
    int point_size = 36;
    const char *ns = nullptr;

    while (argc > 1 && argv[1][0] == '-')
    {
        const char *arg = argv[1];
        switch (arg[1])
        {
        case 'o':
            if (arg[2] == 0)
            {
                if (argc < 2)
                    usage();

                arg = argv[2];
                argv++;
                argc--;
            }
            cerr << "Output " << arg << endl;
            outfile.open(arg, ios::out | ios::binary);
            break;

        case 'p':
            if (arg[2] == 0)
            {
                if (argc < 2)
                    usage();

                arg = argv[2];
                argv++;
                argc--;
            }
            cerr << "Point size" << arg << endl;
            point_size = atoi(arg);
            break;

        case 'n':
            if (arg[2] == 0)
            {
                if (argc < 2)
                    usage();

                arg = argv[2];
                argv++;
                argc--;
            }
            else
            {
                arg += 2;
            }
            cerr << "Namespace " << arg << endl;
            ns = arg;
            break;

        case 'w':
            format = OUTPUT_HEADER;
            switch (arg[2])
            {
            case 'H':
                format = OUTPUT_HEADER;
                break;
            case 'B':
                format = OUTPUT_BINARY;
                break;
            case 0:
                break;
            default:
                usage();
            }
            break;

        case 'i':
        case 'x':
        {
            bool value = arg[1] == 'i';
            for (arg += 2; *arg; arg++)
            {
                switch (*arg)
                {
                case 'U':
                    include_unicode = value;
                    break;
                case 'F':
                    include_font = value;
                    break;
                default:
                    usage();
                }
            }
            break;
        }

        default:
            usage();
        }

        argc--;
        argv++;
    }

    for (--argc, argv++; argc > 0; argc--, argv++)
    {
        cerr << "Processing " << argv[0] << endl;

        FT_Face face;
        error = FT_New_Face(library, argv[0], 0, &face);
        if (error)
        {
            Panic("FT_New_Face failed");
        }

        error = FT_Set_Char_Size(face,
                                 0,
                                 point_size << 6,
                                 144,
                                 144);
        if (error)
        {
            Panic("Could not set character size");
        }
        bool load_unscaled = false;

        if (ns && format == OUTPUT_HEADER)
        {
            outfile << endl
                    << "namespace " << ns << " {" << endl
                    << endl;

            outfile << "/* FT_Size_Metrics " << face->size->metrics << " */" << endl
                    << endl;
        }

        int chn = 0;
        // for (int i = 0; i < face->num_charmaps; i++)
        // {
        //    FT_Set_Charmap(face, face->charmaps[i]);
        // FT_ULong charcode;
        std::vector<uint16_t> charcodes;
        FT_UInt gindex;
        charcode = FT_Get_First_Char(face, &gindex);
        while (gindex)
        {
            // printf("      0x%04lx => %d\n", charcode, gindex);

            if (charcode < 0x10000 && (include_unicode || (charcode >= 0x20 && charcode < 0x7f)))
            {
                UName uname(charcode);
                charcodes.push_back(charcode);
                ++chn;
                error = FT_Load_Glyph(face,
                                      gindex,
                                      FT_LOAD_NO_BITMAP);
                if (error)
                {
                    printf("Fail with error 0x%04x\n", error);
                }

                switch (format)
                {
                case OUTPUT_HEADER:
                    outfile << "FT_Vector " << uname << "_points["
                            << dec << face->glyph->outline.n_points << "] = {";
                    for (int i = 0; i != face->glyph->outline.n_points; ++i)
                    {
                        outfile << dec << "{"
                                << face->glyph->outline.points[i].x << ","
                                << face->glyph->outline.points[i].y << "},";
                    }
                    outfile << "};" << endl;

                    outfile << "char " << uname << "_tags["
                            << dec << face->glyph->outline.n_points << "] = {";
                    for (int i = 0; i != face->glyph->outline.n_points; ++i)
                    {
                        outfile << "0x" << hex << (int)face->glyph->outline.tags[i] << ",";
                    }
                    outfile << "};" << endl;

                    outfile << "short " << uname << "_contours["
                            << dec << face->glyph->outline.n_contours << "] = {";
                    for (int i = 0; i != face->glyph->outline.n_contours; ++i)
                    {
                        outfile << dec << (int)face->glyph->outline.contours[i] << ",";
                    }
                    outfile << "};" << endl;

                    outfile << "FT_GlyphSlotRec_ " << uname
                            << " = " << face->glyph << ";" << endl
                            << endl;

                    break;

                case OUTPUT_BINARY:
                    put(outfile, face->glyph->outline.n_contours);
                    put(outfile, face->glyph->outline.n_points);
                    put(outfile, face->glyph->outline.flags);
                    for (int i = 0; i != face->glyph->outline.n_contours; ++i)
                    {
                        put(outfile, face->glyph->outline.contours[i]);
                    }
                    for (int i = 0; i != face->glyph->outline.n_points; ++i)
                    {
                        put(outfile, face->glyph->outline.points[i].x);
                        put(outfile, face->glyph->outline.points[i].y);
                        put(outfile, face->glyph->outline.tags[i]);
                    }
                }
            }

            charcode = FT_Get_Next_Char(face, charcode, &gindex);
        }

        if (include_font)
        {
            outfile << "FT_GlyphSlotRec_ font_glyphs[] = {" << endl;
            for (auto it = charcodes.begin(); it != charcodes.end(); ++it)
            {
                outfile << "  " << UName(*it) << ",";
                if (*it >= 0x20 && *it < 0x7f)
                {
                    outfile << " // '" << (char)*it << "'";
                }
                outfile << endl;
            }
            outfile << "}; // font_glyphs" << endl
                    << endl;

            FirstAndCount entry;
            outfile << "GlyphRange font_ranges[] = {" << endl;
            for (auto it = charcodes.begin(); it != charcodes.end(); ++it)
            {
                if (it == charcodes.begin())
                {
                    entry.first = *it;
                    entry.count = 1;
                    continue;
                }
                if (entry.first + entry.count == *it)
                {
                    entry.count += 1;
                    continue;
                }
                outfile << "  " << entry << "," << endl;
                entry.first = *it;
                entry.count = 1;
            }
            outfile << "  " << entry << "," << endl;
            outfile << "  " << FirstAndCount{0, 0} << "," << endl;
            outfile << "}; // font_ranges" << endl
                    << endl;

            outfile << "GlyphFont font = {font_glyphs, font_ranges, "
                    << face->size->metrics
                    << "};" << endl
                    << endl;
        }

        FT_Done_Face(face);

        if (ns && format == OUTPUT_HEADER)
        {
            outfile << endl
                    << "} // namespace " << ns << endl;
        }
    }

    FT_Done_FreeType(library);

    outfile.close();
    return 0;
}
