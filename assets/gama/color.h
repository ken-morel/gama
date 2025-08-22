#ifndef GAMA_COLOR_INCLUDED
#define GAMA_COLOR_INCLUDED

#include <GL/gl.h>
#include <stdio.h>

typedef unsigned int Color;

#define ALICEBLUE (Color)0xF0F8FFFF
#define ANTIQUEWHITE (Color)0xFAEBD7FF
#define AQUA (Color)0x00FFFFFF
#define AQUAMARINE (Color)0x7FFFD4FF
#define AZURE (Color)0xF0FFFFFF
#define BEIGE (Color)0xF5F5DCFF
#define BISQUE (Color)0xFFE4C4FF
#define BLACK (Color)0x000000FF
#define BLANCHEDALMOND (Color)0xFFEBCDFF
#define BLUE (Color)0x0000FFFF
#define BLUEVIOLET (Color)0x8A2BE2FF
#define BROWN (Color)0xA52A2AFF
#define BURLYWOOD (Color)0xDEB887FF
#define CADETBLUE (Color)0x5F9EA0FF
#define CHARTREUSE (Color)0x7FFF00FF
#define CHOCOLATE (Color)0xD2691EFF
#define CORAL (Color)0xFF7F50FF
#define CORNFLOWERBLUE (Color)0x6495EDFF
#define CORNSILK (Color)0xFFF8DCFF
#define CRIMSON (Color)0xDC143CFF
#define CYAN (Color)0x00FFFFFF
#define DARKBLUE (Color)0x00008BFF
#define DARKCYAN (Color)0x008B8BFF
#define DARKGOLDENROD (Color)0xB8860BFF
#define DARKGRAY (Color)0xA9A9A9FF
#define DARKGREY (Color)0xA9A9A9FF
#define DARKGREEN (Color)0x006400FF
#define DARKKHAKI (Color)0xBDB76BFF
#define DARKMAGENTA (Color)0x8B008BFF
#define DARKOLIVEGREEN (Color)0x556B2FFF
#define DARKORANGE (Color)0xFF8C00FF
#define DARKORCHID (Color)0x9932CCFF
#define DARKRED (Color)0x8B0000FF
#define DARKSALMON (Color)0xE9967AFF
#define DARKSEAGREEN (Color)0x8FBC8FFF
#define DARKSLATEBLUE (Color)0x483D8BFF
#define DARKSLATEGRAY (Color)0x2F4F4FFF
#define DARKSLATEGREY (Color)0x2F4F4FFF
#define DARKTURQUOISE (Color)0x00CED1FF
#define DARKVIOLET (Color)0x9400D3FF
#define DEEPPINK (Color)0xFF1493FF
#define DEEPSKYBLUE (Color)0x00BFFFFF
#define DIMGRAY (Color)0x696969FF
#define DIMGREY (Color)0x696969FF
#define DODGERBLUE (Color)0x1E90FFFF
#define FIREBRICK (Color)0xB22222FF
#define FLORALWHITE (Color)0xFFFAF0FF
#define FORESTGREEN (Color)0x228B22FF
#define FUCHSIA (Color)0xFF00FFFF
#define GAINSBORO (Color)0xDCDCDCFF
#define GHOSTWHITE (Color)0xF8F8FFFF
#define GOLD (Color)0xFFD700FF
#define GOLDENROD (Color)0xDAA520FF
#define GRAY (Color)0x808080FF
#define GREY (Color)0x808080FF
#define GREEN (Color)0x008000FF
#define GREENYELLOW (Color)0xADFF2FFF
#define HONEYDEW (Color)0xF0FFF0FF
#define HOTPINK (Color)0xFF69B4FF
#define INDIANRED (Color)0xCD5C5CFF
#define INDIGO (Color)0x4B0082FF
#define IVORY (Color)0xFFFFF0FF
#define KHAKI (Color)0xF0E68CFF
#define LAVENDER (Color)0xE6E6FAFF
#define LAVENDERBLUSH (Color)0xFFF0F5FF
#define LAWNGREEN (Color)0x7CFC00FF
#define LEMONCHIFFON (Color)0xFFFACDFF
#define LIGHTBLUE (Color)0xADD8E6FF
#define LIGHTCORAL (Color)0xF08080FF
#define LIGHTCYAN (Color)0xE0FFFFFF
#define LIGHTGOLDENRODYELLOW (Color)0xFAFAD2FF
#define LIGHTGRAY (Color)0xD3D3D3FF
#define LIGHTGREY (Color)0xD3D3D3FF
#define LIGHTGREEN (Color)0x90EE90FF
#define LIGHTPINK (Color)0xFFB6C1FF
#define LIGHTSALMON (Color)0xFFA07AFF
#define LIGHTSEAGREEN (Color)0x20B2AAFF
#define LIGHTSKYBLUE (Color)0x87CEFAFF
#define LIGHTSLATEGRAY (Color)0x778899FF
#define LIGHTSLATEGREY (Color)0x778899FF
#define LIGHTSTEELBLUE (Color)0xB0C4DEFF
#define LIGHTYELLOW (Color)0xFFFFE0FF
#define LIME (Color)0x00FF00FF
#define LIMEGREEN (Color)0x32CD32FF
#define LINEN (Color)0xFAF0E6FF
#define MAGENTA (Color)0xFF00FFFF
#define MAROON (Color)0x800000FF
#define MEDIUMAQUAMARINE (Color)0x66CDAAFF
#define MEDIUMBLUE (Color)0x0000CDFF
#define MEDIUMORCHID (Color)0xBA55D3FF
#define MEDIUMPURPLE (Color)0x9370DBFF
#define MEDIUMSEAGREEN (Color)0x3CB371FF
#define MEDIUMSLATEBLUE (Color)0x7B68EEFF
#define MEDIUMSPRINGGREEN (Color)0x00FA9AFF
#define MEDIUMTURQUOISE (Color)0x48D1CCFF
#define MEDIUMVIOLETRED (Color)0xC71585FF
#define MIDNIGHTBLUE (Color)0x191970FF
#define MINTCREAM (Color)0xF5FFFAFF
#define MISTYROSE (Color)0xFFE4E1FF
#define MOCCASIN (Color)0xFFE4B5FF
#define NAVAJOWHITE (Color)0xFFDEADFF
#define NAVY (Color)0x000080FF
#define OLDLACE (Color)0xFDF5E6FF
#define OLIVE (Color)0x808000FF
#define OLIVEDRAB (Color)0x6B8E23FF
#define ORANGE (Color)0xFFA500FF
#define ORANGERED (Color)0xFF4500FF
#define ORCHID (Color)0xDA70D6FF
#define PALEGOLDENROD (Color)0xEEE8AAFF
#define PALEGREEN (Color)0x98FB98FF
#define PALETURQUOISE (Color)0xAFEEEEFF
#define PALEVIOLETRED (Color)0xDB7093FF
#define PAPAYAWHIP (Color)0xFFEFD5FF
#define PEACHPUFF (Color)0xFFDAB9FF
#define PERU (Color)0xCD853FFF
#define PINK (Color)0xFFC0CBFF
#define PLUM (Color)0xDDA0DDFF
#define POWDERBLUE (Color)0xB0E0E6FF
#define PURPLE (Color)0x800080FF
#define REBECCAPURPLE (Color)0x663399FF
#define RED (Color)0xFF0000FF
#define ROSYBROWN (Color)0xBC8F8FFF
#define ROYALBLUE (Color)0x4169E1FF
#define SADDLEBROWN (Color)0x8B4513FF
#define SALMON (Color)0xFA8072FF
#define SANDYBROWN (Color)0xF4A460FF
#define SEAGREEN (Color)0x2E8B57FF
#define SEASHELL (Color)0xFFF5EEFF
#define SIENNA (Color)0xA0522DFF
#define SILVER (Color)0xC0C0C0FF
#define SKYBLUE (Color)0x87CEEBFF
#define SLATEBLUE (Color)0x6A5ACDFF
#define SLATEGRAY (Color)0x708090FF
#define SLATEGREY (Color)0x708090FF
#define SNOW (Color)0xFFFAFAFF
#define SPRINGGREEN (Color)0x00FF7FFF
#define STEELBLUE (Color)0x4682B4FF
#define TAN (Color)0xD2B48CFF
#define TEAL (Color)0x008080FF
#define THISTLE (Color)0xD8BFD8FF
#define TOMATO (Color)0xFF6347FF
#define TURQUOISE (Color)0x40E0D0FF
#define VIOLET (Color)0xEE82EEFF
#define WHEAT (Color)0xF5DEB3FF
#define WHITE (Color)0xFFFFFFFF
#define WHITESMOKE (Color)0xF5F5F5FF
#define YELLOW (Color)0xFFFF00FF
#define YELLOWGREEN (Color)0x9ACD32FF

float GetColorRed(Color col) {
  unsigned short a1 = (col & 0xFF000000) >> 24;
  return (float)a1 / 255.0f;
}
float GetColorGreen(Color col) {
  unsigned short a1 = (col & 0x00FF0000) >> 16;
  return (float)a1 / 255.0f;
}
float GetColorBlue(Color col) {
  unsigned short a1 = (col & 0x0000FF00) >> 8;
  return (float)a1 / 255.0f;
}
float GetColorAlpha(Color col) {
  unsigned short a1 = (col & 0x000000FF);
  return (float)a1 / 255.0f;
}
void SetClearColor(Color col) {
  glClearColor(GetColorRed(col), GetColorGreen(col), GetColorBlue(col),
               GetColorAlpha(col));
}
void SetGLColor(Color col) {

  glColor3f(GetColorRed(col), GetColorGreen(col), GetColorBlue(col));
}
#endif // GAMA_COLOR_INCLUDED
