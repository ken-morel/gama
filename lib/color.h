#pragma once

#include <stdlib.h>

typedef unsigned int gama_color;

#define gama_color_red(col) ((col & 0xFF000000) >> 24)
#define gama_color_green(col) ((col & 0x00FF0000) >> 16)
#define gama_color_blue(col) ((col & 0x0000FF00) >> 8)
#define gama_color_alpha(col) (col & 0x000000FF)

static inline gama_color gama_color_from_rgba(int r, int g, int b, int a) {
  return (unsigned)(abs(r) % 256) << 24 | (unsigned)(abs(g) % 256) << 16 |
         (unsigned)(abs(b) % 256) << 8 | (unsigned)(abs(a) % 256);
}

static inline void gama_color_set_red(int *col, int red) {
  *col &= 0x00FFFFFF;
  *col |= (unsigned)(abs(red) % 256) << 24;
}
static inline void gama_color_set_green(int *col, int green) {
  *col &= 0xFF00FFFF;
  *col |= (unsigned)(abs(green) % 256) << 16;
}
static inline void gama_color_set_blue(int *col, int blue) {
  *col &= 0xFFFF00FF;
  *col |= (unsigned)(abs(blue) % 256) << 8;
}
static inline void gama_color_set_alpha(int *col, int alpha) {
  *col &= 0xFFFFFF00;
  *col |= (unsigned)(abs(alpha) % 256);
}

#define ALICEBLUE (gama_color)0xF0F8FFFF
#define ANTIQUEWHITE (gama_color)0xFAEBD7FF
#define AQUA (gama_color)0x00FFFFFF
#define AQUAMARINE (gama_color)0x7FFFD4FF
#define AZURE (gama_color)0xF0FFFFFF
#define BEIGE (gama_color)0xF5F5DCFF
#define BISQUE (gama_color)0xFFE4C4FF
#define BLACK (gama_color)0x000000FF
#define BLANCHEDALMOND (gama_color)0xFFEBCDFF
#define BLUE (gama_color)0x0000FFFF
#define BLUEVIOLET (gama_color)0x8A2BE2FF
#define BROWN (gama_color)0xA52A2AFF
#define BURLYWOOD (gama_color)0xDEB887FF
#define CADETBLUE (gama_color)0x5F9EA0FF
#define CHARTREUSE (gama_color)0x7FFF00FF
#define CHOCOLATE (gama_color)0xD2691EFF
#define CORAL (gama_color)0xFF7F50FF
#define CORNFLOWERBLUE (gama_color)0x6495EDFF
#define CORNSILK (gama_color)0xFFF8DCFF
#define CRIMSON (gama_color)0xDC143CFF
#define CYAN (gama_color)0x00FFFFFF
#define DARKBLUE (gama_color)0x00008BFF
#define DARKCYAN (gama_color)0x008B8BFF
#define DARKGOLDENROD (gama_color)0xB8860BFF
#define DARKGRAY (gama_color)0xA9A9A9FF
#define DARKGREY (gama_color)0xA9A9A9FF
#define DARKGREEN (gama_color)0x006400FF
#define DARKKHAKI (gama_color)0xBDB76BFF
#define DARKMAGENTA (gama_color)0x8B008BFF
#define DARKOLIVEGREEN (gama_color)0x556B2FFF
#define DARKORANGE (gama_color)0xFF8C00FF
#define DARKORCHID (gama_color)0x9932CCFF
#define DARKRED (gama_color)0x8B0000FF
#define DARKSALMON (gama_color)0xE9967AFF
#define DARKSEAGREEN (gama_color)0x8FBC8FFF
#define DARKSLATEBLUE (gama_color)0x483D8BFF
#define DARKSLATEGRAY (gama_color)0x2F4F4FFF
#define DARKSLATEGREY (gama_color)0x2F4F4FFF
#define DARKTURQUOISE (gama_color)0x00CED1FF
#define DARKVIOLET (gama_color)0x9400D3FF
#define DEEPPINK (gama_color)0xFF1493FF
#define DEEPSKYBLUE (gama_color)0x00BFFFFF
#define DIMGRAY (gama_color)0x696969FF
#define DIMGREY (gama_color)0x696969FF
#define DODGERBLUE (gama_color)0x1E90FFFF
#define FIREBRICK (gama_color)0xB22222FF
#define FLORALWHITE (gama_color)0xFFFAF0FF
#define FORESTGREEN (gama_color)0x228B22FF
#define FUCHSIA (gama_color)0xFF00FFFF
#define GAINSBORO (gama_color)0xDCDCDCFF
#define GHOSTWHITE (gama_color)0xF8F8FFFF
#define GOLD (gama_color)0xFFD700FF
#define GOLDENROD (gama_color)0xDAA520FF
#define GRAY (gama_color)0x808080FF
#define GREY (gama_color)0x808080FF
#define GREEN (gama_color)0x008000FF
#define GREENYELLOW (gama_color)0xADFF2FFF
#define HONEYDEW (gama_color)0xF0FFF0FF
#define HOTPINK (gama_color)0xFF69B4FF
#define INDIANRED (gama_color)0xCD5C5CFF
#define INDIGO (gama_color)0x4B0082FF
#define IVORY (gama_color)0xFFFFF0FF
#define KHAKI (gama_color)0xF0E68CFF
#define LAVENDER (gama_color)0xE6E6FAFF
#define LAVENDERBLUSH (gama_color)0xFFF0F5FF
#define LAWNGREEN (gama_color)0x7CFC00FF
#define LEMONCHIFFON (gama_color)0xFFFACDFF
#define LIGHTBLUE (gama_color)0xADD8E6FF
#define LIGHTCORAL (gama_color)0xF08080FF
#define LIGHTCYAN (gama_color)0xE0FFFFFF
#define LIGHTGOLDENRODYELLOW (gama_color)0xFAFAD2FF
#define LIGHTGRAY (gama_color)0xD3D3D3FF
#define LIGHTGREY (gama_color)0xD3D3D3FF
#define LIGHTGREEN (gama_color)0x90EE90FF
#define LIGHTPINK (gama_color)0xFFB6C1FF
#define LIGHTSALMON (gama_color)0xFFA07AFF
#define LIGHTSEAGREEN (gama_color)0x20B2AAFF
#define LIGHTSKYBLUE (gama_color)0x87CEFAFF
#define LIGHTSLATEGRAY (gama_color)0x778899FF
#define LIGHTSLATEGREY (gama_color)0x778899FF
#define LIGHTSTEELBLUE (gama_color)0xB0C4DEFF
#define LIGHTYELLOW (gama_color)0xFFFFE0FF
#define LIME (gama_color)0x00FF00FF
#define LIMEGREEN (gama_color)0x32CD32FF
#define LINEN (gama_color)0xFAF0E6FF
#define MAGENTA (gama_color)0xFF00FFFF
#define MAROON (gama_color)0x800000FF
#define MEDIUMAQUAMARINE (gama_color)0x66CDAAFF
#define MEDIUMBLUE (gama_color)0x0000CDFF
#define MEDIUMORCHID (gama_color)0xBA55D3FF
#define MEDIUMPURPLE (gama_color)0x9370DBFF
#define MEDIUMSEAGREEN (gama_color)0x3CB371FF
#define MEDIUMSLATEBLUE (gama_color)0x7B68EEFF
#define MEDIUMSPRINGGREEN (gama_color)0x00FA9AFF
#define MEDIUMTURQUOISE (gama_color)0x48D1CCFF
#define MEDIUMVIOLETRED (gama_color)0xC71585FF
#define MIDNIGHTBLUE (gama_color)0x191970FF
#define MINTCREAM (gama_color)0xF5FFFAFF
#define MISTYROSE (gama_color)0xFFE4E1FF
#define MOCCASIN (gama_color)0xFFE4B5FF
#define NAVAJOWHITE (gama_color)0xFFDEADFF
#define NAVY (gama_color)0x000080FF
#define OLDLACE (gama_color)0xFDF5E6FF
#define OLIVE (gama_color)0x808000FF
#define OLIVEDRAB (gama_color)0x6B8E23FF
#define ORANGE (gama_color)0xFFA500FF
#define ORANGERED (gama_color)0xFF4500FF
#define ORCHID (gama_color)0xDA70D6FF
#define PALEGOLDENROD (gama_color)0xEEE8AAFF
#define PALEGREEN (gama_color)0x98FB98FF
#define PALETURQUOISE (gama_color)0xAFEEEEFF
#define PALEVIOLETRED (gama_color)0xDB7093FF
#define PAPAYAWHIP (gama_color)0xFFEFD5FF
#define PEACHPUFF (gama_color)0xFFDAB9FF
#define PERU (gama_color)0xCD853FFF
#define PINK (gama_color)0xFFC0CBFF
#define PLUM (gama_color)0xDDA0DDFF
#define POWDERBLUE (gama_color)0xB0E0E6FF
#define PURPLE (gama_color)0x800080FF
#define REBECCAPURPLE (gama_color)0x663399FF
#define RED (gama_color)0xFF0000FF
#define ROSYBROWN (gama_color)0xBC8F8FFF
#define ROYALBLUE (gama_color)0x4169E1FF
#define SADDLEBROWN (gama_color)0x8B4513FF
#define SALMON (gama_color)0xFA8072FF
#define SANDYBROWN (gama_color)0xF4A460FF
#define SEAGREEN (gama_color)0x2E8B57FF
#define SEASHELL (gama_color)0xFFF5EEFF
#define SIENNA (gama_color)0xA0522DFF
#define SILVER (gama_color)0xC0C0C0FF
#define SKYBLUE (gama_color)0x87CEEBFF
#define SLATEBLUE (gama_color)0x6A5ACDFF
#define SLATEGRAY (gama_color)0x708090FF
#define SLATEGREY (gama_color)0x708090FF
#define SNOW (gama_color)0xFFFAFAFF
#define SPRINGGREEN (gama_color)0x00FF7FFF
#define STEELBLUE (gama_color)0x4682B4FF
#define TAN (gama_color)0xD2B48CFF
#define TEAL (gama_color)0x008080FF
#define THISTLE (gama_color)0xD8BFD8FF
#define TOMATO (gama_color)0xFF6347FF
#define TURQUOISE (gama_color)0x40E0D0FF
#define VIOLET (gama_color)0xEE82EEFF
#define WHEAT (gama_color)0xF5DEB3FF
#define WHITE (gama_color)0xFFFFFFFF
#define WHITESMOKE (gama_color)0xF5F5F5FF
#define YELLOW (gama_color)0xFFFF00FF
#define YELLOWGREEN (gama_color)0x9ACD32FF
