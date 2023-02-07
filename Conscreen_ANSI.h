
//TODO: NowUanC
#pragma once
#include <stdint.h>
#include "Conscreen_string.h"

typedef struct
{
        uint8_t r,g,b;
} Conscreen_color;

#define CONSCREEN_RGRGB(R,G,B) (Conscreen_color){R,G,B}


typedef struct
{
        uint8_t nec: 1;
        uint8_t normal:1; // 0
        uint8_t invert:1; // 7 | 27
        uint8_t strike:1; // 9 | 29
        uint8_t overline:1; // 53 | 5

        int8_t intensity:2; //  2 faint < 0; 22 normal = 0; 1 bold > 0
        uint8_t underline:2; //  24 non | 4 single | 21 double

        Conscreen_color forground,
                        background;
} Conscreen_ansi;

#define Conscreen_ansi_default(R,G,B) (const Conscreen_ansi){0,0,0,0,0,0,0, {R,G,B}, { 0,0,0 }}

void Conscreen_ansi_change(Conscreen_ansi old_ansi, Conscreen_ansi new_ansi, Conscreen_string out);
void Conscreen_ansi_changeB(Conscreen_ansi new_ansi, Conscreen_string out);


#define ESC STR("\033")
#define CSI STR("[")
