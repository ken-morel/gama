#ifndef GANA_COLOR_H_INCLUDED
#define GANA_COLOR_H_INCLUDED


typedef unsigned int Color;

Color ColorMake(int r, int g, int b) {
    return (Color)(b+256*g+256*256*r);
}
int* ColorDecompose(Color c) {
    int* col = (int*)malloc(3*sizeof(int));
    col[2] = c % 256;
    col[1] = (c/256) % 256;
    col[0] = (c/(256*256)) % 256;
    return col;
}
float* ColorDecomposef(Color c) {
    float* col = (float*)malloc(3*sizeof(float));
    col[2] = (float)(c % 256) / 255.0f;
    c /= 256;
    col[1] = (float)(c % 256) / 255.0f;
    c /= 256;
    col[0] = (float)(c % 256) / 255.0f;
    return col;
}


#define ALICEBLUE            (Color)0xf0f8ff
#define ANTIQUEWHITE         (Color)0xfaebd7
#define AQUA                 (Color)0x00ffff
#define AQUAMARINE           (Color)0x7fffd4
#define AZURE                (Color)0xf0ffff
#define BEIGE                (Color)0xf5f5dc
#define BISQUE               (Color)0xffe4c4
#define BLACK                (Color)0x000000
#define BLANCHEDALMOND       (Color)0xffebcd
#define BLUE                 (Color)0x0000ff
#define BLUEVIOLET           (Color)0x8a2be2
#define BROWN                (Color)0xa52a2a
#define BURLYWOOD            (Color)0xdeb887
#define CADETBLUE            (Color)0x5f9ea0
#define CHARTREUSE           (Color)0x7fff00
#define CHOCOLATE            (Color)0xd2691e
#define CORAL                (Color)0xff7f50
#define CORNFLOWERBLUE       (Color)0x6495ed
#define CORNSILK             (Color)0xfff8dc
#define CRIMSON              (Color)0xdc143c
#define CYAN                 (Color)0x00ffff
#define DARKBLUE             (Color)0x00008b
#define DARKCYAN             (Color)0x008b8b
#define DARKGOLDENROD        (Color)0xb8860b
#define DARKGRAY             (Color)0xa9a9a9
#define DARKGREY             (Color)0xa9a9a9
#define DARKGREEN            (Color)0x006400
#define DARKKHAKI            (Color)0xbdb76b
#define DARKMAGENTA          (Color)0x8b008b
#define DARKOLIVEGREEN       (Color)0x556b2f
#define DARKORANGE           (Color)0xff8c00
#define DARKORCHID           (Color)0x9932cc
#define DARKRED              (Color)0x8b0000
#define DARKSALMON           (Color)0xe9967a
#define DARKSEAGREEN         (Color)0x8fbc8f
#define DARKSLATEBLUE        (Color)0x483d8b
#define DARKSLATEGRAY        (Color)0x2f4f4f
#define DARKSLATEGREY        (Color)0x2f4f4f
#define DARKTURQUOISE        (Color)0x00ced1
#define DARKVIOLET           (Color)0x9400d3
#define DEEPPINK             (Color)0xff1493
#define DEEPSKYBLUE          (Color)0x00bfff
#define DIMGRAY              (Color)0x696969
#define DIMGREY              (Color)0x696969
#define DODGERBLUE           (Color)0x1e90ff
#define FIREBRICK            (Color)0xb22222
#define FLORALWHITE          (Color)0xfffaf0
#define FORESTGREEN          (Color)0x228b22
#define FUCHSIA              (Color)0xff00ff
#define GAINSBORO            (Color)0xdcdcdc
#define GHOSTWHITE           (Color)0xf8f8ff
#define GOLD                 (Color)0xffd700
#define GOLDENROD            (Color)0xdaa520
#define GRAY                 (Color)0x808080
#define GREY                 (Color)0x808080
#define GREEN                (Color)0x008000
#define GREENYELLOW          (Color)0xadff2f
#define HONEYDEW             (Color)0xf0fff0
#define HOTPINK              (Color)0xff69b4
#define INDIANRED            (Color)0xcd5c5c
#define INDIGO               (Color)0x4b0082
#define IVORY                (Color)0xfffff0
#define KHAKI                (Color)0xf0e68c
#define LAVENDER             (Color)0xe6e6fa
#define LAVENDERBLUSH        (Color)0xfff0f5
#define LAWNGREEN            (Color)0x7cfc00
#define LEMONCHIFFON         (Color)0xfffacd
#define LIGHTBLUE            (Color)0xadd8e6
#define LIGHTCORAL           (Color)0xf08080
#define LIGHTCYAN            (Color)0xe0ffff
#define LIGHTGOLDENRODYELLOW (Color)0xfafad2
#define LIGHTGRAY            (Color)0xd3d3d3
#define LIGHTGREY            (Color)0xd3d3d3
#define LIGHTGREEN           (Color)0x90ee90
#define LIGHTPINK            (Color)0xffb6c1
#define LIGHTSALMON          (Color)0xffa07a
#define LIGHTSEAGREEN        (Color)0x20b2aa
#define LIGHTSKYBLUE         (Color)0x87cefa
#define LIGHTSLATEGRAY       (Color)0x778899
#define LIGHTSLATEGREY       (Color)0x778899
#define LIGHTSTEELBLUE       (Color)0xb0c4de
#define LIGHTYELLOW          (Color)0xffffe0
#define LIME                 (Color)0x00ff00
#define LIMEGREEN            (Color)0x32cd32
#define LINEN                (Color)0xfaf0e6
#define MAGENTA              (Color)0xff00ff
#define MAROON               (Color)0x800000
#define MEDIUMAQUAMARINE     (Color)0x66cdaa
#define MEDIUMBLUE           (Color)0x0000cd
#define MEDIUMORCHID         (Color)0xba55d3
#define MEDIUMPURPLE         (Color)0x9370db
#define MEDIUMSEAGREEN       (Color)0x3cb371
#define MEDIUMSLATEBLUE      (Color)0x7b68ee
#define MEDIUMSPRINGGREEN    (Color)0x00fa9a
#define MEDIUMTURQUOISE      (Color)0x48d1cc
#define MEDIUMVIOLETRED      (Color)0xc71585
#define MIDNIGHTBLUE         (Color)0x191970
#define MINTCREAM            (Color)0xf5fffa
#define MISTYROSE            (Color)0xffe4e1
#define MOCCASIN             (Color)0xffe4b5
#define NAVAJOWHITE          (Color)0xffdead
#define NAVY                 (Color)0x000080
#define OLDLACE              (Color)0xfdf5e6
#define OLIVE                (Color)0x808000
#define OLIVEDRAB            (Color)0x6b8e23
#define ORANGE               (Color)0xffa500
#define ORANGERED            (Color)0xff4500
#define ORCHID               (Color)0xda70d6
#define PALEGOLDENROD        (Color)0xeee8aa
#define PALEGREEN            (Color)0x98fb98
#define PALETURQUOISE        (Color)0xafeeee
#define PALEVIOLETRED        (Color)0xdb7093
#define PAPAYAWHIP           (Color)0xffefd5
#define PEACHPUFF            (Color)0xffdab9
#define PERU                 (Color)0xcd853f
#define PINK                 (Color)0xffc0cb
#define PLUM                 (Color)0xdda0dd
#define POWDERBLUE           (Color)0xb0e0e6
#define PURPLE               (Color)0x800080
#define REBECCAPURPLE        (Color)0x663399
#define RED                  (Color)0xff0000
#define ROSYBROWN            (Color)0xbc8f8f
#define ROYALBLUE            (Color)0x4169e1
#define SADDLEBROWN          (Color)0x8b4513
#define SALMON               (Color)0xfa8072
#define SANDYBROWN           (Color)0xf4a460
#define SEAGREEN             (Color)0x2e8b57
#define SEASHELL             (Color)0xfff5ee
#define SIENNA               (Color)0xa0522d
#define SILVER               (Color)0xc0c0c0
#define SKYBLUE              (Color)0x87ceeb
#define SLATEBLUE            (Color)0x6a5acd
#define SLATEGRAY            (Color)0x708090
#define SLATEGREY            (Color)0x708090
#define SNOW                 (Color)0xfffafa
#define SPRINGGREEN          (Color)0x00ff7f
#define STEELBLUE            (Color)0x4682b4
#define TAN                  (Color)0xd2b48c
#define TEAL                 (Color)0x008080
#define THISTLE              (Color)0xd8bfd8
#define TOMATO               (Color)0xff6347
#define TURQUOISE            (Color)0x40e0d0
#define VIOLET               (Color)0xee82ee
#define WHEAT                (Color)0xf5deb3
#define WHITE                (Color)0xffffff
#define WHITESMOKE           (Color)0xf5f5f5
#define YELLOW               (Color)0xffff00
#define YELLOWGREEN          (Color)0x9acd32

#endif // GANA_COLOR_H_INCLUDED
