#pragma once
#include <dirent.h>
#include <string>
#include <vector>
#include "Bitmap.hpp"
#include "Vector.hpp"

//subliminal:headers
int readInt(std::string txt);
//endsubliminal:headers

class Character
{
public:
    Character(char character, Bitmap* bitmap):character(character), bitmap(bitmap)
    {
    };
    char character;
    Bitmap* bitmap;
};

class ImageFont
{
public:

    ImageFont(std::string dir)
    {
        load(dir);
    };
    void load(std::string dir);
    std::vector<Character*> characters;
};
void ImageFont::load(std::string dirname)
{
    dirent *ent;
    DIR* dir = opendir(dirname.c_str());

    while((ent = readdir(dir)) != NULL)
    {
        if(ent -> d_type != 0) continue;
        std::string name = (std::string)ent ->d_name;
        std::string path = dirname + "/" + name;
        //printf("loading char %c:::%s\n", name.c_str()[0], path.c_str());
        characters.push_back(
            new Character( readInt(name) , new Bitmap(path))
        );
    }
}
int readInt(std::string txt)
{
    int ret = 0;
    int power = 0;
    int begin = '0', end = '9';
    for(int i = txt.size()-1; i >= 0 ; i--)
    {
        int code = (int)txt[i];
        if(begin <= code && end >= code)
        {
            int num = 1;
            for(int x = 0; x < power; x++) num *= 10;
            ret += (int)(code - begin) * num;
            power++;
        }
    }
    return ret;
}
class ImageText
{
public:
    ImageText(ImageFont* font):font(font)
    {
        text = "";
        pos = new Vector();
        rotation = new Rotation();
    };
    ImageText(ImageFont* _font, std::string _text, Vector* _pos):font(nullptr), pos(nullptr)
    {
        pos = _pos;
        font = _font;
        text = _text;
        rotation = new Rotation();
        vertify();
    };
    ImageText(ImageFont* _font, std::string _text, double _ps, Vector* _pos):font(nullptr), pos(nullptr)
    {
        pos = _pos;
        font = _font;
        text = _text;
        rotation = new Rotation();
        vertify();
        pointsize = _ps;
    };
    void vertify()
    {
        while(characters.size() > 0) characters.pop_back();
        for(auto c:text)
        {
            for(auto ch:font->characters)
            {
                if(ch->character == c)
                {
                    characters.push_back(ch);
                }
            }
        }
    }
    void setText(std::string _text)
    {
        text = _text;
        vertify();
    }
    void render() {
        double begin_x = pos -> x;//= -(double)characters.size()*height/2;
        for(auto ch:characters)
        {
            ch->bitmap->render(begin_x, pos->y, pointsize, pointsize);
            begin_x += pointsize;
        }
    }

    ImageFont* font;
    std::string text;
    Vector *pos;
    Rotation* rotation;
    std::vector<Character*> characters;
    double pointsize = 0.05;
};
