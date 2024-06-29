#include "Vector.hpp"
#include "Color.hpp"
#include "ImageFont.hpp"
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#ifndef GAMA_SHAPE_HPP_INCLUDED
#define GAMA_SHAPE_HPP_INCLUDED
    class Shape
    {
    protected:
        Shape() = default;

    };

    class Cube: public Shape {
    public:
        Cube():pos(nullptr), size(nullptr) {
            pos = new Vector();
            size = new Vector();
            rotation = new Rotation(0, 0, 0);
            color = new Color(200, 150, 100);
        }
        Cube(Vector* pos, Vector* size):pos(pos), size(size)
        {
            rotation = new Rotation(0, 0, 0);
            color = new Color(200, 150, 100);
        }
        void update(double theta) {
            pos->update(theta);
            size->update(theta);
            rotation->update(theta);
        }
        void render() const;
        Vector *pos, *size;
        Rotation* rotation;
        Color* color;
        bool mesh = false;
    };
    void Cube::render() const {
        pos->enable();
        rotation->enable();
            double sx = size -> x / 2;
            double sy = size -> y / 2;
            double sz = size -> z / 2;
            /*double points[8][3] = {
                {pos->x-sx, pos->y-sy, pos->z-sz}, {pos->x+sx, pos->y-sy, pos->z-sz},
                {pos->x-sx, pos->y+sy, pos->z-sz}, {pos->x+sx, pos->y+sy, pos->z-sz},

                {pos->x-sx, pos->y-sy, pos->z+sz}, {pos->x+sx, pos->y-sy, pos->z+sz},
                {pos->x-sx, pos->y+sy, pos->z+sz}, {pos->x+sx, pos->y+sy, pos->z+sz},
            };*/
            double points[8][3] = {
                {-sx, -sy, -sz}, {+sx, -sy, -sz},
                {-sx, +sy, -sz}, {+sx, +sy, -sz},

                {-sx, -sy, +sz}, {+sx, -sy, +sz},
                {-sx, +sy, +sz}, {+sx, +sy, +sz},
            };
            glBegin(GL_QUADS);
                //glMaterialfv(GL_FRONT, GL_AMBIENT, color->array());
                //glMaterialfv(GL_FRONT, GL_DIFFUSE, color->array());
                glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
                glColor3fv(color->array());
                //back
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3dv(points[4]);
                glVertex3dv(points[5]);
                glVertex3dv(points[7]);
                glVertex3dv(points[6]);
                //front
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3dv(points[0]);
                glVertex3dv(points[1]);
                glVertex3dv(points[3]);
                glVertex3dv(points[2]);
                //right
                glNormal3f(1.0f, 0.0f, 0.0f);
                glVertex3dv(points[1]);
                glVertex3dv(points[5]);
                glVertex3dv(points[7]);
                glVertex3dv(points[3]);
                //left
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3dv(points[0]);
                glVertex3dv(points[4]);
                glVertex3dv(points[6]);
                glVertex3dv(points[2]);
                //bottom
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3dv(points[6]);
                glVertex3dv(points[7]);
                glVertex3dv(points[3]);
                glVertex3dv(points[2]);
                //top
                glNormal3f(0.0f, -1.0f, 0.0f);
                glVertex3dv(points[4]);
                glVertex3dv(points[5]);
                glVertex3dv(points[1]);
                glVertex3dv(points[0]);
            glEnd();
            glBegin(GL_LINES);
                if(mesh)
                {
                    int edges[12][2] = {
                        {0, 1},
                        {1, 3},
                        {3, 2},
                        {0, 2},

                        {0, 4},
                        {1, 5},
                        {2, 6},
                        {3, 7},

                        {4, 5},
                        {5, 7},
                        {7, 6},
                        {4, 6},
                    };

                    for(int i = 0; i < 12; i++)
                    {
                        glVertex3dv(points[edges[i][0]]);
                        glVertex3dv(points[edges[i][1]]);
                    }
                }
            glEnd();
        pos->disable();
        rotation->disable();
    }









    class Sphere {
    public:
        Sphere():pos(nullptr), color(nullptr), rotation(nullptr)
        {
            rotation = new Rotation();
            pos = new Vector();
            color = new Color();
            radius = 1.0;
            quadric = gluNewQuadric();
        }
        Sphere(Vector* pos, double radius):pos(pos), radius(radius), color(nullptr), rotation(nullptr)
        {
            rotation = new Rotation();
            color = new Color();
            quadric = gluNewQuadric();
        }
        Sphere(Vector* pos, Color* color, double radius):pos(pos), radius(radius), color(color), rotation(nullptr)
        {
            rotation = new Rotation();
            quadric = gluNewQuadric();
        }
        void update(double theta)
        {
            pos->update(theta);
            rotation->update(theta);
        }
        void render() const;

        Vector* pos;
        double radius;
        int tesselation = -1;
        Color* color;
        Rotation* rotation;
        GLUquadric *quadric;
    };
    void Sphere::render() const
    {
        pos->enable();
        rotation->enable();
        int stacks = tesselation;
        if(tesselation == -1)
            stacks = radius * 25;
        if(tesselation < 10) stacks = 10;
        int slices = stacks;
        glColor3fv(color->array());
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, color->array());
        glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
        //glMaterialfv(GL_FRONT, GL_AMBIENT, color->array());
        gluSphere(quadric, radius, slices, stacks);
        pos->disable();
        rotation->disable();
    }





    class Cylinder {
    public:
        Cylinder():pos(nullptr), color(nullptr), rotation(nullptr)
        {
            rotation = new Rotation();
            pos = new Vector();
            color = new Color();
            radius = 1.0;
            height = 1.0;
            quadric = gluNewQuadric();
        }
        Cylinder(Vector* pos, double radius, double height):pos(pos), color(nullptr), rotation(nullptr)
        {
            rotation = new Rotation();
            color = new Color();
            quadric = gluNewQuadric();
            radius = radius;
            height = height;
        }
        Cylinder(Vector* pos, Color* color, double radius, double height):pos(pos), radius(radius), height(height), color(color), rotation(nullptr)
        {
            rotation = new Rotation();
            quadric = gluNewQuadric();
        }
        void update(double theta)
        {
            pos->update(theta);
            rotation->update(theta);
        }
        void render() const;

        Vector* pos;
        double radius, height;
        int tesselation = -1;
        Color* color;
        Rotation* rotation;
        GLUquadric *quadric;
    };
    void Cylinder::render() const
    {
        pos->enable();
        rotation->enable();
        int stacks = tesselation;
        if(tesselation == -1)
            stacks = radius * 25;
        if(tesselation < 10) stacks = 10;
        int slices = stacks;
        glColor3fv(color->array());
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, color->array());
        glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
        //glMaterialfv(GL_FRONT, GL_AMBIENT, color->array());
        gluCylinder(quadric, radius, radius, height, slices, stacks);
        pos->disable();
        rotation->disable();
    };

    class gRectangle
    {
    public:
        gRectangle():pos(nullptr), size(nullptr), color(nullptr), rotation(nullptr)
        {
            pos = new Vector();
            size = new Vector();
            color = new Color();
            rotation = new Rotation();
        }
        gRectangle(double x, double y, double w, double h):pos(nullptr), size(nullptr), color(nullptr), rotation(nullptr)
        {
            pos = new Vector(x, y, 0);
            size = new Vector(w, h, 0);
            color = new Color();
            rotation = new Rotation();
        }
        void update(double theta)
        {
            pos->update(theta);
            size->update(theta);
            rotation->update(theta);
        }
        void render() const
        {
            glColor3fv(color->array());
            glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
            double
                sx = size->x / 2,
                sy = size->y / 2;
            pos->enable();
            rotation->enable();
            glBegin(GL_QUADS);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex2d(-sx, sy);
                glVertex2d(sx, sy);
                glVertex2d(sx, -sy);
                glVertex2d(-sx, -sy);
            glEnd();

            pos->disable();
            rotation->disable();
        }

        Vector *pos, *size;
        Color *color;
        Rotation *rotation;
    };


    class gCircle
    {
    public:
        gCircle():pos(nullptr), color(nullptr), rotation(nullptr)
        {
            pos = new Vector();
            radius = 1;
            color = new Color();
            rotation = new Rotation();
        }
        gCircle(double x, double y, double _radius):pos(nullptr), color(nullptr), rotation(nullptr)
        {
            radius = _radius;
            pos = new Vector(x, y, 0);
            color = new Color();
            rotation = new Rotation();
        }
        void update(double theta)
        {
            pos->update(theta);
            rotation->update(theta);
        }
        void render() const
        {
            int slices = 100;
            glColor3fv(color->array());
            glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
            //pos->enable();
            rotation->enable();
            glBegin(GL_TRIANGLE_FAN);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex2d(0, 0);

                for(int i = 0; i < slices + 1; i++)
                {
                    double theta = 6.285714285714286*((double)i / (double)slices);
                    double
                        x = radius * sin(theta),
                        y = radius * cos(theta);
                    glVertex2d(x, y);
                }
            glEnd();

            //pos->disable();
            rotation->disable();
        }
        static void arc(double rx, double ry, double r, double begin, double end, int stacks)
        {
            if(begin > end)
            {
                double temp = end;
                end = begin;
                begin = temp;
            }
            begin *= (22/7)/180;
            end *= (22/7)/180;
            glBegin(GL_TRIANGLE_FAN);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex2d(rx, ry);

                for(int i = 0; i < stacks + 1; i++)
                {
                    double theta = 6.285714285714286*((double)i / (double)stacks);
                    if(theta < end && theta > begin)
                        continue;
                    double
                        px = r * sin(theta),
                        py = r * cos(theta);
                    glVertex2d(rx+px, ry+py);
                }
            glEnd();
        }

        Vector *pos;
        Color *color;
        Rotation *rotation;
        int tesselation = -1;
        double radius;
    };


    class gRoundRectangle
    {
    public:
        gRoundRectangle():pos(nullptr), size(nullptr), color(nullptr), rotation(nullptr)
        {
            pos = new Vector();
            size = new Vector();
            color = new Color();
            rotation = new Rotation();
            radius = 1;
        }
        gRoundRectangle(double x, double y, double w, double h, double r):pos(nullptr), size(nullptr), color(nullptr), rotation(nullptr)
        {
            pos = new Vector(x, y, 0);
            size = new Vector(w, h, 0);
            color = new Color();
            rotation = new Rotation();
            radius = r;
        }
        void update(double theta)
        {
            pos->update(theta);
            size->update(theta);
            rotation->update(theta);
        }
        void render() const
        {
            glColor3fv(color->array());
            glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
            double
                sx = size->x / 2,
                sy = size->y / 2;
            pos->enable();
            rotation->enable();
            glBegin(GL_QUADS);
                //top bottom
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex2d(radius-sx, sy);
                glVertex2d(sx-radius, sy);
                glVertex2d(sx-radius, -sy);
                glVertex2d(radius-sx, -sy);
                //left right
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex2d(-sx, sy-radius);
                glVertex2d(sx, sy-radius);
                glVertex2d(sx, radius-sy);
                glVertex2d(-sx, radius-sy);
            glEnd();

            int slices = 100;
            gCircle::arc(radius-sx, sy-radius, radius, 180, 270, slices);
            gCircle::arc(sx-radius, sy-radius, radius, 270, 360, slices);
            gCircle::arc(sx-radius, radius-sy, radius,   0,  90, slices);
            gCircle::arc(radius-sx, radius-sy, radius,   0,  90, slices);

            pos->disable();
            rotation->disable();
        }

        Vector *pos, *size;
        Color *color;
        Rotation *rotation;
        double radius;
    };

class FPSWidget
{
public:
    FPSWidget(ImageFont* font):text(nullptr)
    {
        text = new ImageText(
            font,
            "FPS: ",
            0.08,
            new Vector(-1, -1+0.08, 0)
        );
        rect = new gRoundRectangle(-1, -1+0.08, 0.08*3, 0.08, 0.02);
        rect->color->set(PALEVIOLETRED);
    };
    void update(int fps)
    {
        std::string fpstxt = "";
        int p = 0;
        while(fps >= pow(10, p))
        {
            fpstxt = (char)(
                ((int)'0')
                + (fps / (int)pow(10, p))%10
            ) + fpstxt;
            p++;
        };
        text->setText(" fps: " + fpstxt);
        rect->size->x = text->text.size()*text->pointsize;
        rect->pos->x = -1 + rect->size->x/2;
    }
    void render() const
    {
        text->render();
        rect->render();
    }

    ImageText* text;
    gRoundRectangle *rect;
};



#endif
