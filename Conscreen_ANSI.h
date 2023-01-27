
//TODO: NowUanC
#pragma once
#include <stdint.h>

typedef struct
{
        uint8_t r,g,b;
} Conscreen_color;

#define CONSCREEN_ANSI_COLOR(R,G,B) (Conscreen_color){R,G,B}


typedef struct
{
        uint8_t normal:1; // 0
        uint8_t invert:1; // 7 | 27
        uint8_t strike:1; // 9 | 29
        uint8_t overline:1; // 53 | 5

        int8_t intensity:2; //  2 faint < 0; 22 normal = 0; 1 bold > 0
        uint8_t underline:2; //  24 non | 4 single | 21 double

        Conscreen_color forground,
                        background;
} Conscreen_ansi;

typedef struct info_ANSI ANSI;

#define Conscreen_ansi_default(R,G,B) (Conscreen_ansi){0,0,0,0,0,0, {R,G,B}, { 0,0,0 }}

bool Conscreen_ansi_change(Conscreen_ansi old, Conscreen_ansi new, Conscreen_char *buffer, size_t max);

