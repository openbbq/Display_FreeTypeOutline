#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
// gcc gen.cpp $(pkg-config --cflags freetype2 --libs) && ./a.out >out.txt

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

int main(int argc, const char *const *argv)
{
    FT_Library library;
    error = FT_Init_FreeType(&library);

    FT_Face face;

    ofstream outfile;
    OutputFormat format = OUTPUT_HEADER;
    bool only_ascii = false;
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

        case 'a':
            only_ascii = true;
            break;

        default:
            usage();
        }

        argc--;
        argv++;
    }

    for (--argc, argv++; argc > 0; argc--, argv++)
    {
        cerr << "Processing " << argv[0] << endl;

        if (ns && format == OUTPUT_HEADER)
        {
            outfile << endl
                    << "namespace " << ns << " {" << endl
                    << endl;
        }

        FT_Face face;
        error = FT_New_Face(library, argv[0], 0, &face);
        if (error)
        {
            Panic("FT_New_Face failed");
        }

        error = FT_Set_Char_Size(face,
                                 0,
                                 36 << 6,
                                 72,
                                 72);
        if (error)
        {
            Panic("Could not set character size");
        }
        bool load_unscaled = false;

        int chn = 0;
        // for (int i = 0; i < face->num_charmaps; i++)
        // {
        //    FT_Set_Charmap(face, face->charmaps[i]);
        FT_ULong charcode;
        FT_UInt gindex;
        charcode = FT_Get_First_Char(face, &gindex);
        while (gindex)
        {
            // printf("      0x%04lx => %d\n", charcode, gindex);

            if (only_ascii == false || (charcode >= 0x20 && charcode < 0x7f))
            {
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
                    outfile << "FT_Vector u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_points["
                            << dec << face->glyph->outline.n_points << "] = {";
                    for (int i = 0; i != face->glyph->outline.n_points; ++i)
                    {
                        outfile << dec << "{"
                                << face->glyph->outline.points[i].x << ","
                                << face->glyph->outline.points[i].y << "},";
                    }
                    outfile << "};" << endl;

                    outfile << "char u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_tags["
                            << dec << face->glyph->outline.n_points << "] = {";
                    for (int i = 0; i != face->glyph->outline.n_points; ++i)
                    {
                        outfile << "0x" << hex << (int)face->glyph->outline.tags[i] << ",";
                    }
                    outfile << "};" << endl;

                    outfile << "short u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_contours["
                            << dec << face->glyph->outline.n_contours << "] = {";
                    for (int i = 0; i != face->glyph->outline.n_contours; ++i)
                    {
                        outfile << dec << (int)face->glyph->outline.contours[i] << ",";
                    }
                    outfile << "};" << endl;

                    outfile << "FT_Outline u" << setfill('0') << setw(4) << hex << uppercase << charcode
                            << " = {"
                            << dec << face->glyph->outline.n_contours << ", "
                            << dec << face->glyph->outline.n_points << ", "
                            << "u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_points, "
                            << "u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_tags, "
                            << "u" << setfill('0') << setw(4) << hex << uppercase << charcode << "_contours, "
                            << "0x" << hex << face->glyph->outline.flags
                            << "};" << endl
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
