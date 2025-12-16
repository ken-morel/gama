#pragma once

#include <stdlib.h>

typedef unsigned int gmColor;

#define gm_red(col) ((col & 0xFF000000) >> 24)
#define gm_green(col) ((col & 0x00FF0000) >> 16)
#define gm_blue(col) ((col & 0x0000FF00) >> 8)
#define gm_alpha(col) (col & 0x000000FF)

static inline gmColor gm_rgba(int r, int g, int b, int a) {
  return (unsigned)(abs(r) % 256) << 24 | (unsigned)(abs(g) % 256) << 16 |
         (unsigned)(abs(b) % 256) << 8 | (unsigned)(abs(a) % 256);
}

static inline gmColor gm_set_red(gmColor col, int red) {
  return (col & 0x00FFFFFF) | (unsigned)(abs(red) % 256) << 24;
}
static inline gmColor gm_set_green(gmColor col, int green) {
  return (col & 0xFF00FFFF) | (unsigned)(abs(green) % 256) << 16;
}
static inline gmColor gm_set_blue(gmColor col, int blue) {
  return (col & 0xFFFF00FF) | (unsigned)(abs(blue) % 256) << 8;
}
static inline gmColor gm_set_alpha(gmColor col, int alpha) {
  return (col & 0xFFFFFF00) | (unsigned)(abs(alpha) % 256);
}

#define GM_ALICEBLUE (gmColor)0xF0F8FFFF
#define GM_ANTIQUEWHITE (gmColor)0xFAEBD7FF
#define GM_AQUA (gmColor)0x00FFFFFF
#define GM_AQUAMARINE (gmColor)0x7FFFD4FF
#define GM_AZURE (gmColor)0xF0FFFFFF
#define GM_BEIGE (gmColor)0xF5F5DCFF
#define GM_BISQUE (gmColor)0xFFE4C4FF
#define GM_BLACK (gmColor)0x000000FF
#define GM_BLANCHEDALMOND (gmColor)0xFFEBCDFF
#define GM_BLUE (gmColor)0x0000FFFF
#define GM_BLUEVIOLET (gmColor)0x8A2BE2FF
#define GM_BROWN (gmColor)0xA52A2AFF
#define GM_BURLYWOOD (gmColor)0xDEB887FF
#define GM_CADETBLUE (gmColor)0x5F9EA0FF
#define GM_CHARTREUSE (gmColor)0x7FFF00FF
#define GM_CHOCOLATE (gmColor)0xD2691EFF
#define GM_CORAL (gmColor)0xFF7F50FF
#define GM_CORNFLOWERBLUE (gmColor)0x6495EDFF
#define GM_CORNSILK (gmColor)0xFFF8DCFF
#define GM_CRIMSON (gmColor)0xDC143CFF
#define GM_CYAN (gmColor)0x00FFFFFF
#define GM_DARKBLUE (gmColor)0x00008BFF
#define GM_DARKCYAN (gmColor)0x008B8BFF
#define GM_DARKGOLDENROD (gmColor)0xB8860BFF
#define GM_DARKGRAY (gmColor)0xA9A9A9FF
#define GM_DARKGREY (gmColor)0xA9A9A9FF
#define GM_DARKGREEN (gmColor)0x006400FF
#define GM_DARKKHAKI (gmColor)0xBDB76BFF
#define GM_DARKMAGENTA (gmColor)0x8B008BFF
#define GM_DARKOLIVEGREEN (gmColor)0x556B2FFF
#define GM_DARKORANGE (gmColor)0xFF8C00FF
#define GM_DARKORCHID (gmColor)0x9932CCFF
#define GM_DARKRED (gmColor)0x8B0000FF
#define GM_DARKSALMON (gmColor)0xE9967AFF
#define GM_DARKSEAGREEN (gmColor)0x8FBC8FFF
#define GM_DARKSLATEBLUE (gmColor)0x483D8BFF
#define GM_DARKSLATEGRAY (gmColor)0x2F4F4FFF
#define GM_DARKSLATEGREY (gmColor)0x2F4F4FFF
#define GM_DARKTURQUOISE (gmColor)0x00CED1FF
#define GM_DARKVIOLET (gmColor)0x9400D3FF
#define GM_DEEPPINK (gmColor)0xFF1493FF
#define GM_DEEPSKYBLUE (gmColor)0x00BFFFFF
#define GM_DIMGRAY (gmColor)0x696969FF
#define GM_DIMGREY (gmColor)0x696969FF
#define GM_DODGERBLUE (gmColor)0x1E90FFFF
#define GM_FIREBRICK (gmColor)0xB22222FF
#define GM_FLORALWHITE (gmColor)0xFFFAF0FF
#define GM_FORESTGREEN (gmColor)0x228B22FF
#define GM_FUCHSIA (gmColor)0xFF00FFFF
#define GM_GAINSBORO (gmColor)0xDCDCDCFF
#define GM_GHOSTWHITE (gmColor)0xF8F8FFFF
#define GM_GOLD (gmColor)0xFFD700FF
#define GM_GOLDENROD (gmColor)0xDAA520FF
#define GM_GRAY (gmColor)0x808080FF
#define GM_GREY (gmColor)0x808080FF
#define GM_GREEN (gmColor)0x008000FF
#define GM_GREENYELLOW (gmColor)0xADFF2FFF
#define GM_HONEYDEW (gmColor)0xF0FFF0FF
#define GM_HOTPINK (gmColor)0xFF69B4FF
#define GM_INDIANRED (gmColor)0xCD5C5CFF
#define GM_INDIGO (gmColor)0x4B0082FF
#define GM_IVORY (gmColor)0xFFFFF0FF
#define GM_KHAKI (gmColor)0xF0E68CFF
#define GM_LAVENDER (gmColor)0xE6E6FAFF
#define GM_LAVENDERBLUSH (gmColor)0xFFF0F5FF
#define GM_LAWNGREEN (gmColor)0x7CFC00FF
#define GM_LEMONCHIFFON (gmColor)0xFFFACDFF
#define GM_LIGHTBLUE (gmColor)0xADD8E6FF
#define GM_LIGHTCORAL (gmColor)0xF08080FF
#define GM_LIGHTCYAN (gmColor)0xE0FFFFFF
#define GM_LIGHTGOLDENRODYELLOW (gmColor)0xFAFAD2FF
#define GM_LIGHTGRAY (gmColor)0xD3D3D3FF
#define GM_LIGHTGREY (gmColor)0xD3D3D3FF
#define GM_LIGHTGREEN (gmColor)0x90EE90FF
#define GM_LIGHTPINK (gmColor)0xFFB6C1FF
#define GM_LIGHTSALMON (gmColor)0xFFA07AFF
#define GM_LIGHTSEAGREEN (gmColor)0x20B2AAFF
#define GM_LIGHTSKYBLUE (gmColor)0x87CEFAFF
#define GM_LIGHTSLATEGRAY (gmColor)0x778899FF
#define GM_LIGHTSLATEGREY (gmColor)0x778899FF
#define GM_LIGHTSTEELBLUE (gmColor)0xB0C4DEFF
#define GM_LIGHTYELLOW (gmColor)0xFFFFE0FF
#define GM_LIME (gmColor)0x00FF00FF
#define GM_LIMEGREEN (gmColor)0x32CD32FF
#define GM_LINEN (gmColor)0xFAF0E6FF
#define GM_MAGENTA (gmColor)0xFF00FFFF
#define GM_MAROON (gmColor)0x800000FF
#define GM_MEDIUMAQUAMARINE (gmColor)0x66CDAAFF
#define GM_MEDIUMBLUE (gmColor)0x0000CDFF
#define GM_MEDIUMORCHID (gmColor)0xBA55D3FF
#define GM_MEDIUMPURPLE (gmColor)0x9370DBFF
#define GM_MEDIUMSEAGREEN (gmColor)0x3CB371FF
#define GM_MEDIUMSLATEBLUE (gmColor)0x7B68EEFF
#define GM_MEDIUMSPRINGGREEN (gmColor)0x00FA9AFF
#define GM_MEDIUMTURQUOISE (gmColor)0x48D1CCFF
#define GM_MEDIUMVIOLETRED (gmColor)0xC71585FF
#define GM_MIDNIGHTBLUE (gmColor)0x191970FF
#define GM_MINTCREAM (gmColor)0xF5FFFAFF
#define GM_MISTYROSE (gmColor)0xFFE4E1FF
#define GM_MOCCASIN (gmColor)0xFFE4B5FF
#define GM_NAVAJOWHITE (gmColor)0xFFDEADFF
#define GM_NAVY (gmColor)0x000080FF
#define GM_OLDLACE (gmColor)0xFDF5E6FF
#define GM_OLIVE (gmColor)0x808000FF
#define GM_OLIVEDRAB (gmColor)0x6B8E23FF
#define GM_ORANGE (gmColor)0xFFA500FF
#define GM_ORANGERED (gmColor)0xFF4500FF
#define GM_ORCHID (gmColor)0xDA70D6FF
#define GM_PALEGOLDENROD (gmColor)0xEEE8AAFF
#define GM_PALEGREEN (gmColor)0x98FB98FF
#define GM_PALETURQUOISE (gmColor)0xAFEEEEFF
#define GM_PALEVIOLETRED (gmColor)0xDB7093FF
#define GM_PAPAYAWHIP (gmColor)0xFFEFD5FF
#define GM_PEACHPUFF (gmColor)0xFFDAB9FF
#define GM_PERU (gmColor)0xCD853FFF
#define GM_PINK (gmColor)0xFFC0CBFF
#define GM_PLUM (gmColor)0xDDA0DDFF
#define GM_POWDERBLUE (gmColor)0xB0E0E6FF
#define GM_PURPLE (gmColor)0x800080FF
#define GM_REBECCAPURPLE (gmColor)0x663399FF
#define GM_RED (gmColor)0xFF0000FF
#define GM_ROSYBROWN (gmColor)0xBC8F8FFF
#define GM_ROYALBLUE (gmColor)0x4169E1FF
#define GM_SADDLEBROWN (gmColor)0x8B4513FF
#define GM_SALMON (gmColor)0xFA8072FF
#define GM_SANDYBROWN (gmColor)0xF4A460FF
#define GM_SEAGREEN (gmColor)0x2E8B57FF
#define GM_SEASHELL (gmColor)0xFFF5EEFF
#define GM_SIENNA (gmColor)0xA0522DFF
#define GM_SILVER (gmColor)0xC0C0C0FF
#define GM_SKYBLUE (gmColor)0x87CEEBFF
#define GM_SLATEBLUE (gmColor)0x6A5ACDFF
#define GM_SLATEGRAY (gmColor)0x708090FF
#define GM_SLATEGREY (gmColor)0x708090FF
#define GM_SNOW (gmColor)0xFFFAFAFF
#define GM_SPRINGGREEN (gmColor)0x00FF7FFF
#define GM_STEELBLUE (gmColor)0x4682B4FF
#define GM_TAN (gmColor)0xD2B48CFF
#define GM_TEAL (gmColor)0x008080FF
#define GM_THISTLE (gmColor)0xD8BFD8FF
#define GM_TOMATO (gmColor)0xFF6347FF
#define GM_TURQUOISE (gmColor)0x40E0D0FF
#define GM_VIOLET (gmColor)0xEE82EEFF
#define GM_WHEAT (gmColor)0xF5DEB3FF
#define GM_WHITE (gmColor)0xFFFFFFFF
#define GM_WHITESMOKE (gmColor)0xF5F5F5FF
#define GM_YELLOW (gmColor)0xFFFF00FF
#define GM_YELLOWGREEN (gmColor)0x9ACD32FF
