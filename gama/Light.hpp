#ifndef GAMA_LIGHT_HPP_INCLUDED
#define GAMA_LIGHT_HPP_INCLUDED
#include <gl/gl.h>
enum class LightType {DIFFUSE=GL_DIFFUSE, SPECULAR = GL_SPECULAR, AMBIENT=GL_AMBIENT};
class Light {
public:
    Light(int id_):pos(nullptr), color(nullptr), type(LightType::DIFFUSE)
    {
        id = Light::get(id_);
        pos = new Vector(0, 0, -1);
        color = new Color(WHITE);
        intensity = 0.1f;
    }
    Light(int id_, LightType type):pos(nullptr), color(nullptr), type(type)
    {
        id = Light::get(id_);
        pos = new Vector(0, 0, -1);
        color = new Color(WHITE);
        intensity = 1.0f;
    }
    Light(int id_, LightType type, int r, int g, int b, float _intensity):pos(nullptr), color(new Color(r, g, b)), type(type)
    {
        id = Light::get(id_);
        pos = new Vector(0, 0, -1);
        intensity = _intensity;
    }
    Light(int id_, LightType type, double x, double y, double z):pos(new Vector(x, y, z)), color(nullptr), type(type)
    {
        id = Light::get(id_);
        color = new Color(WHITE);
        intensity = 0.1f;
    }
    Light(int id_, LightType type, int r, int g, int b, float _intensity, double x, double y, double z):pos(new Vector(x, y, z)), color(new Color(r, g, b)), type(type)
    {
        id = Light::get(id_);
        intensity = _intensity;
    }
    void enable() const
    {
        glEnable(id);

        float* col = color->array();
        float params[] = {col[0], col[1], col[2], intensity};

        glLightfv(id, (GLenum)type, params);

        float *lpos = pos -> farray();

        glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    }
    void disable() const
    {
        glDisable(id);
    }

    static GLenum get(int number) {
        switch(number) {
        case 0: return GL_LIGHT0;
        case 1: return GL_LIGHT1;
        case 2: return GL_LIGHT2;
        case 3: return GL_LIGHT3;
        case 4: return GL_LIGHT4;
        case 5: return GL_LIGHT5;
        case 6: return GL_LIGHT6;
        case 7: return GL_LIGHT7;
        }
        return GL_LIGHT0;
    }
    Vector *pos;
    Color* color;
    LightType type;
    float intensity;
    GLenum id;
};

#endif
