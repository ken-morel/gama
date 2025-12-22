#pragma once

#include "stdlib.h"

/**
 * @brief Type definition for color values in RGBA format.
 */
typedef unsigned int gmColor;

/**
 * @brief Extracts the red component from a color.
 * @param col The color to extract from.
 * @return The red component (0-255).
 */
#define gm_red(col) ((col & 0xFF000000) >> 24)

/**
 * @brief Extracts the green component from a color.
 * @param col The color to extract from.
 * @return The green component (0-255).
 */
#define gm_green(col) ((col & 0x00FF0000) >> 16)

/**
 * @brief Extracts the blue component from a color.
 * @param col The color to extract from.
 * @return The blue component (0-255).
 */
#define gm_blue(col) ((col & 0x0000FF00) >> 8)

/**
 * @brief Extracts the alpha component from a color.
 * @param col The color to extract from.
 * @return The alpha component (0-255).
 */
#define gm_alpha(col) (col & 0x000000FF)

/**
 * @brief Creates a color from RGBA components.
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 * @param a Alpha component (0-255).
 * @return A gmColor value.
 */
static inline gmColor gm_rgba(int r, int g, int b, int a) {
  return (unsigned)(abs(r) % 256) << 24 | (unsigned)(abs(g) % 256) << 16 |
         (unsigned)(abs(b) % 256) << 8 | (unsigned)(abs(a) % 256);
}

/**
 * @brief Creates a color from RGB components.
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 * @return A gmColor value.
 */
static inline gmColor gm_rgb(int r, int g, int b) {
  return (unsigned)(abs(r) % 256) << 24 | (unsigned)(abs(g) % 256) << 16 |
         (unsigned)(abs(b) % 256) << 8 | 0xFF;
}
/**
 * @brief Sets the red component of a color.
 * @param col The original color.
 * @param red The new red component (0-255).
 * @return A new color with the updated red component.
 */
static inline gmColor gm_set_red(gmColor col, int red) {
  return (col & 0x00FFFFFF) | (unsigned)(abs(red) % 256) << 24;
}

/**
 * @brief Sets the green component of a color.
 * @param col The original color.
 * @param green The new green component (0-255).
 * @return A new color with the updated green component.
 */
static inline gmColor gm_set_green(gmColor col, int green) {
  return (col & 0xFF00FFFF) | (unsigned)(abs(green) % 256) << 16;
}

/**
 * @brief Sets the blue component of a color.
 * @param col The original color.
 * @param blue The new blue component (0-255).
 * @return A new color with the updated blue component.
 */
static inline gmColor gm_set_blue(gmColor col, int blue) {
  return (col & 0xFFFF00FF) | (unsigned)(abs(blue) % 256) << 8;
}

/**
 * @brief Sets the alpha component of a color.
 * @param col The original color.
 * @param alpha The new alpha component (0-255).
 * @return A new color with the updated alpha component.
 */
static inline gmColor gm_set_alpha(gmColor col, int alpha) {
  return (col & 0xFFFFFF00) | (unsigned)(abs(alpha) % 256);
}

/* Predefined color constants */
/**
 * @name Predefined Colors
 * @{
 */

/**
 * @brief Opaque color (full alpha).
 */
#define GM_OPAQUE (gmColor)0xFFFFFFFF

/**
 * @brief Transparent color (zero alpha).
 */
#define GM_TRANSPARENT (gmColor)0xFFFFFF00

/**
 * @brief Translucent color (partial alpha).
 */
#define GM_TRANSLUCENT (gmColor)0xFFFFFF99

/**
 * @brief The official Gama brand color.
 */
#define GM_GAMA (gmColor)0xAA77AAFF

/**
 * @brief Alice blue color.
 */
#define GM_ALICEBLUE (gmColor)0xF0F8FFFF

/**
 * @brief Antique white color.
 */
#define GM_ANTIQUEWHITE (gmColor)0xFAEBD7FF

/**
 * @brief Aqua color.
 */
#define GM_AQUA (gmColor)0x00FFFFFF

/**
 * @brief Aquamarine color.
 */
#define GM_AQUAMARINE (gmColor)0x7FFFD4FF

/**
 * @brief Azure color.
 */
#define GM_AZURE (gmColor)0xF0FFFFFF

/**
 * @brief Beige color.
 */
#define GM_BEIGE (gmColor)0xF5F5DCFF

/**
 * @brief Bisque color.
 */
#define GM_BISQUE (gmColor)0xFFE4C4FF

/**
 * @brief Black color.
 */
#define GM_BLACK (gmColor)0x000000FF

/**
 * @brief Blanched almond color.
 */
#define GM_BLANCHEDALMOND (gmColor)0xFFEBCDFF

/**
 * @brief Blue color.
 */
#define GM_BLUE (gmColor)0x0000FFFF

/**
 * @brief Blue violet color.
 */
#define GM_BLUEVIOLET (gmColor)0x8A2BE2FF

/**
 * @brief Brown color.
 */
#define GM_BROWN (gmColor)0xA52A2AFF

/**
 * @brief Burlywood color.
 */
#define GM_BURLYWOOD (gmColor)0xDEB887FF

/**
 * @brief Cadet blue color.
 */
#define GM_CADETBLUE (gmColor)0x5F9EA0FF

/**
 * @brief Chartreuse color.
 */
#define GM_CHARTREUSE (gmColor)0x7FFF00FF

/**
 * @brief Chocolate color.
 */
#define GM_CHOCOLATE (gmColor)0xD2691EFF

/**
 * @brief Coral color.
 */
#define GM_CORAL (gmColor)0xFF7F50FF

/**
 * @brief Cornflower blue color.
 */
#define GM_CORNFLOWERBLUE (gmColor)0x6495EDFF

/**
 * @brief Cornsilk color.
 */
#define GM_CORNSILK (gmColor)0xFFF8DCFF

/**
 * @brief Crimson color.
 */
#define GM_CRIMSON (gmColor)0xDC143CFF

/**
 * @brief Cyan color.
 */
#define GM_CYAN (gmColor)0x00FFFFFF

/**
 * @brief Dark blue color.
 */
#define GM_DARKBLUE (gmColor)0x00008BFF

/**
 * @brief Dark cyan color.
 */
#define GM_DARKCYAN (gmColor)0x008B8BFF

/**
 * @brief Dark goldenrod color.
 */
#define GM_DARKGOLDENROD (gmColor)0xB8860BFF

/**
 * @brief Dark gray color.
 */
#define GM_DARKGRAY (gmColor)0xA9A9A9FF

/**
 * @brief Dark grey color (same as dark gray).
 */
#define GM_DARKGREY (gmColor)0xA9A9A9FF

/**
 * @brief Dark green color.
 */
#define GM_DARKGREEN (gmColor)0x006400FF

/**
 * @brief Dark khaki color.
 */
#define GM_DARKKHAKI (gmColor)0xBDB76BFF

/**
 * @brief Dark magenta color.
 */
#define GM_DARKMAGENTA (gmColor)0x8B008BFF

/**
 * @brief Dark olive green color.
 */
#define GM_DARKOLIVEGREEN (gmColor)0x556B2FFF

/**
 * @brief Dark orange color.
 */
#define GM_DARKORANGE (gmColor)0xFF8C00FF

/**
 * @brief Dark orchid color.
 */
#define GM_DARKORCHID (gmColor)0x9932CCFF

/**
 * @brief Dark red color.
 */
#define GM_DARKRED (gmColor)0x8B0000FF

/**
 * @brief Dark salmon color.
 */
#define GM_DARKSALMON (gmColor)0xE9967AFF

/**
 * @brief Dark sea green color.
 */
#define GM_DARKSEAGREEN (gmColor)0x8FBC8FFF

/**
 * @brief Dark slate blue color.
 */
#define GM_DARKSLATEBLUE (gmColor)0x483D8BFF

/**
 * @brief Dark slate gray color.
 */
#define GM_DARKSLATEGRAY (gmColor)0x2F4F4FFF

/**
 * @brief Dark slate grey color (same as dark slate gray).
 */
#define GM_DARKSLATEGREY (gmColor)0x2F4F4FFF

/**
 * @brief Dark turquoise color.
 */
#define GM_DARKTURQUOISE (gmColor)0x00CED1FF

/**
 * @brief Dark violet color.
 */
#define GM_DARKVIOLET (gmColor)0x9400D3FF

/**
 * @brief Deep pink color.
 */
#define GM_DEEPPINK (gmColor)0xFF1493FF

/**
 * @brief Deep sky blue color.
 */
#define GM_DEEPSKYBLUE (gmColor)0x00BFFFFF

/**
 * @brief Dim gray color.
 */
#define GM_DIMGRAY (gmColor)0x696969FF

/**
 * @brief Dim grey color (same as dim gray).
 */
#define GM_DIMGREY (gmColor)0x696969FF

/**
 * @brief Dodger blue color.
 */
#define GM_DODGERBLUE (gmColor)0x1E90FFFF

/**
 * @brief Fire brick color.
 */
#define GM_FIREBRICK (gmColor)0xB22222FF

/**
 * @brief Floral white color.
 */
#define GM_FLORALWHITE (gmColor)0xFFFAF0FF

/**
 * @brief Forest green color.
 */
#define GM_FORESTGREEN (gmColor)0x228B22FF

/**
 * @brief Fuchsia color.
 */
#define GM_FUCHSIA (gmColor)0xFF00FFFF

/**
 * @brief Gainsboro color.
 */
#define GM_GAINSBORO (gmColor)0xDCDCDCFF

/**
 * @brief Ghost white color.
 */
#define GM_GHOSTWHITE (gmColor)0xF8F8FFFF

/**
 * @brief Gold color.
 */
#define GM_GOLD (gmColor)0xFFD700FF

/**
 * @brief Goldenrod color.
 */
#define GM_GOLDENROD (gmColor)0xDAA520FF

/**
 * @brief Gray color.
 */
#define GM_GRAY (gmColor)0x808080FF

/**
 * @brief Grey color (same as gray).
 */
#define GM_GREY (gmColor)0x808080FF

/**
 * @brief Green color.
 */
#define GM_GREEN (gmColor)0x008000FF

/**
 * @brief Green yellow color.
 */
#define GM_GREENYELLOW (gmColor)0xADFF2FFF

/**
 * @brief Honeydew color.
 */
#define GM_HONEYDEW (gmColor)0xF0FFF0FF

/**
 * @brief Hot pink color.
 */
#define GM_HOTPINK (gmColor)0xFF69B4FF

/**
 * @brief Indian red color.
 */
#define GM_INDIANRED (gmColor)0xCD5C5CFF

/**
 * @brief Indigo color.
 */
#define GM_INDIGO (gmColor)0x4B0082FF

/**
 * @brief Ivory color.
 */
#define GM_IVORY (gmColor)0xFFFFF0FF

/**
 * @brief Khaki color.
 */
#define GM_KHAKI (gmColor)0xF0E68CFF

/**
 * @brief Lavender color.
 */
#define GM_LAVENDER (gmColor)0xE6E6FAFF

/**
 * @brief Lavender blush color.
 */
#define GM_LAVENDERBLUSH (gmColor)0xFFF0F5FF

/**
 * @brief Lawn green color.
 */
#define GM_LAWNGREEN (gmColor)0x7CFC00FF

/**
 * @brief Lemon chiffon color.
 */
#define GM_LEMONCHIFFON (gmColor)0xFFFACDFF

/**
 * @brief Light blue color.
 */
#define GM_LIGHTBLUE (gmColor)0xADD8E6FF

/**
 * @brief Light coral color.
 */
#define GM_LIGHTCORAL (gmColor)0xF08080FF

/**
 * @brief Light cyan color.
 */
#define GM_LIGHTCYAN (gmColor)0xE0FFFFFF

/**
 * @brief Light goldenrod yellow color.
 */
#define GM_LIGHTGOLDENRODYELLOW (gmColor)0xFAFAD2FF

/**
 * @brief Light gray color.
 */
#define GM_LIGHTGRAY (gmColor)0xD3D3D3FF

/**
 * @brief Light grey color (same as light gray).
 */
#define GM_LIGHTGREY (gmColor)0xD3D3D3FF

/**
 * @brief Light green color.
 */
#define GM_LIGHTGREEN (gmColor)0x90EE90FF

/**
 * @brief Light pink color.
 */
#define GM_LIGHTPINK (gmColor)0xFFB6C1FF

/**
 * @brief Light salmon color.
 */
#define GM_LIGHTSALMON (gmColor)0xFFA07AFF

/**
 * @brief Light sea green color.
 */
#define GM_LIGHTSEAGREEN (gmColor)0x20B2AAFF

/**
 * @brief Light sky blue color.
 */
#define GM_LIGHTSKYBLUE (gmColor)0x87CEFAFF

/**
 * @brief Light slate gray color.
 */
#define GM_LIGHTSLATEGRAY (gmColor)0x778899FF

/**
 * @brief Light slate grey color (same as light slate gray).
 */
#define GM_LIGHTSLATEGREY (gmColor)0x778899FF

/**
 * @brief Light steel blue color.
 */
#define GM_LIGHTSTEELBLUE (gmColor)0xB0C4DEFF

/**
 * @brief Light yellow color.
 */
#define GM_LIGHTYELLOW (gmColor)0xFFFFE0FF

/**
 * @brief Lime color.
 */
#define GM_LIME (gmColor)0x00FF00FF

/**
 * @brief Lime green color.
 */
#define GM_LIMEGREEN (gmColor)0x32CD32FF

/**
 * @brief Linen color.
 */
#define GM_LINEN (gmColor)0xFAF0E6FF

/**
 * @brief Magenta color.
 */
#define GM_MAGENTA (gmColor)0xFF00FFFF

/**
 * @brief Maroon color.
 */
#define GM_MAROON (gmColor)0x800000FF

/**
 * @brief Medium aquamarine color.
 */
#define GM_MEDIUMAQUAMARINE (gmColor)0x66CDAAFF

/**
 * @brief Medium blue color.
 */
#define GM_MEDIUMBLUE (gmColor)0x0000CDFF

/**
 * @brief Medium orchid color.
 */
#define GM_MEDIUMORCHID (gmColor)0xBA55D3FF

/**
 * @brief Medium purple color.
 */
#define GM_MEDIUMPURPLE (gmColor)0x9370DBFF

/**
 * @brief Medium sea green color.
 */
#define GM_MEDIUMSEAGREEN (gmColor)0x3CB371FF

/**
 * @brief Medium slate blue color.
 */
#define GM_MEDIUMSLATEBLUE (gmColor)0x7B68EEFF

/**
 * @brief Medium spring green color.
 */
#define GM_MEDIUMSPRINGGREEN (gmColor)0x00FA9AFF

/**
 * @brief Medium turquoise color.
 */
#define GM_MEDIUMTURQUOISE (gmColor)0x48D1CCFF

/**
 * @brief Medium violet red color.
 */
#define GM_MEDIUMVIOLETRED (gmColor)0xC71585FF

/**
 * @brief Midnight blue color.
 */
#define GM_MIDNIGHTBLUE (gmColor)0x191970FF

/**
 * @brief Mint cream color.
 */
#define GM_MINTCREAM (gmColor)0xF5FFFAFF

/**
 * @brief Misty rose color.
 */
#define GM_MISTYROSE (gmColor)0xFFE4E1FF

/**
 * @brief Moccasin color.
 */
#define GM_MOCCASIN (gmColor)0xFFE4B5FF

/**
 * @brief Navajo white color.
 */
#define GM_NAVAJOWHITE (gmColor)0xFFDEADFF

/**
 * @brief Navy color.
 */
#define GM_NAVY (gmColor)0x000080FF

/**
 * @brief Old lace color.
 */
#define GM_OLDLACE (gmColor)0xFDF5E6FF

/**
 * @brief Olive color.
 */
#define GM_OLIVE (gmColor)0x808000FF

/**
 * @brief Olive drab color.
 */
#define GM_OLIVEDRAB (gmColor)0x6B8E23FF

/**
 * @brief Orange color.
 */
#define GM_ORANGE (gmColor)0xFFA500FF

/**
 * @brief Orange red color.
 */
#define GM_ORANGERED (gmColor)0xFF4500FF

/**
 * @brief Orchid color.
 */
#define GM_ORCHID (gmColor)0xDA70D6FF

/**
 * @brief Pale goldenrod color.
 */
#define GM_PALEGOLDENROD (gmColor)0xEEE8AAFF

/**
 * @brief Pale green color.
 */
#define GM_PALEGREEN (gmColor)0x98FB98FF

/**
 * @brief Pale turquoise color.
 */
#define GM_PALETURQUOISE (gmColor)0xAFEEEEFF

/**
 * @brief Pale violet red color.
 */
#define GM_PALEVIOLETRED (gmColor)0xDB7093FF

/**
 * @brief Papaya whip color.
 */
#define GM_PAPAYAWHIP (gmColor)0xFFEFD5FF

/**
 * @brief Peach puff color.
 */
#define GM_PEACHPUFF (gmColor)0xFFDAB9FF

/**
 * @brief Peru color.
 */
#define GM_PERU (gmColor)0xCD853FFF

/**
 * @brief Pink color.
 */
#define GM_PINK (gmColor)0xFFC0CBFF

/**
 * @brief Plum color.
 */
#define GM_PLUM (gmColor)0xDDA0DDFF

/**
 * @brief Powder blue color.
 */
#define GM_POWDERBLUE (gmColor)0xB0E0E6FF

/**
 * @brief Purple color.
 */
#define GM_PURPLE (gmColor)0x800080FF

/**
 * @brief Rebecca purple color.
 */
#define GM_REBECCAPURPLE (gmColor)0x663399FF

/**
 * @brief Red color.
 */
#define GM_RED (gmColor)0xFF0000FF

/**
 * @brief Rosy brown color.
 */
#define GM_ROSYBROWN (gmColor)0xBC8F8FFF

/**
 * @brief Royal blue color.
 */
#define GM_ROYALBLUE (gmColor)0x4169E1FF

/**
 * @brief Saddle brown color.
 */
#define GM_SADDLEBROWN (gmColor)0x8B4513FF

/**
 * @brief Salmon color.
 */
#define GM_SALMON (gmColor)0xFA8072FF

/**
 * @brief Sandy brown color.
 */
#define GM_SANDYBROWN (gmColor)0xF4A460FF

/**
 * @brief Sea green color.
 */
#define GM_SEAGREEN (gmColor)0x2E8B57FF

/**
 * @brief Sea shell color.
 */
#define GM_SEASHELL (gmColor)0xFFF5EEFF

/**
 * @brief Sienna color.
 */
#define GM_SIENNA (gmColor)0xA0522DFF

/**
 * @brief Silver color.
 */
#define GM_SILVER (gmColor)0xC0C0C0FF

/**
 * @brief Sky blue color.
 */
#define GM_SKYBLUE (gmColor)0x87CEEBFF

/**
 * @brief Slate blue color.
 */
#define GM_SLATEBLUE (gmColor)0x6A5ACDFF

/**
 * @brief Slate gray color.
 */
#define GM_SLATEGRAY (gmColor)0x708090FF

/**
 * @brief Slate grey color (same as slate gray).
 */
#define GM_SLATEGREY (gmColor)0x708090FF

/**
 * @brief Snow color.
 */
#define GM_SNOW (gmColor)0xFFFAFAFF

/**
 * @brief Spring green color.
 */
#define GM_SPRINGGREEN (gmColor)0x00FF7FFF

/**
 * @brief Steel blue color.
 */
#define GM_STEELBLUE (gmColor)0x4682B4FF

/**
 * @brief Tan color.
 */
#define GM_TAN (gmColor)0xD2B48CFF

/**
 * @brief Teal color.
 */
#define GM_TEAL (gmColor)0x008080FF

/**
 * @brief Thistle color.
 */
#define GM_THISTLE (gmColor)0xD8BFD8FF

/**
 * @brief Tomato color.
 */
#define GM_TOMATO (gmColor)0xFF6347FF

/**
 * @brief Turquoise color.
 */
#define GM_TURQUOISE (gmColor)0x40E0D0FF

/**
 * @brief Violet color.
 */
#define GM_VIOLET (gmColor)0xEE82EEFF

/**
 * @brief Wheat color.
 */
#define GM_WHEAT (gmColor)0xF5DEB3FF

/**
 * @brief White color.
 */
#define GM_WHITE (gmColor)0xFFFFFFFF

/**
 * @brief White smoke color.
 */
#define GM_WHITESMOKE (gmColor)0xF5F5F5FF

/**
 * @brief Yellow color.
 */
#define GM_YELLOW (gmColor)0xFFFF00FF

/**
 * @brief Yellow green color.
 */
#define GM_YELLOWGREEN (gmColor)0x9ACD32FF

/**
 * @}
 */
