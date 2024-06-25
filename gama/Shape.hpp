#include "Vector.hpp"
#include "Color.hpp"
#include <math.h>
#include <gl/gl.h>
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
        }
        Sphere(Vector* pos, double radius):pos(pos), radius(radius), color(nullptr), rotation(nullptr)
        {
            rotation = new Rotation();
            color = new Color();
        }
        Sphere(Vector* pos, Color* color, double radius):pos(pos), radius(radius), color(color), rotation(nullptr)
        {
            rotation = new Rotation();
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
    };
    void Sphere::render() const
    {
        pos->enable();
        rotation->enable();
        int stacks = tesselation;
        if(tesselation == -1)
            stacks = radius * 25;
        if(tesselation < 10) stacks = 10;
        int slices = 100;
        glColor3fv(color->array());
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, color->array());
        glMaterialfv(GL_FRONT, GL_SPECULAR, color->array());
        //glMaterialfv(GL_FRONT, GL_AMBIENT, color->array());
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i < stacks; i++) {
            float theta1 = i * M_PI / stacks;
            float theta2 = (i + 1) * M_PI / stacks;

            for (int j = 0; j <= slices; j++) {
                double phi = j * 2 * M_PI / slices;

                double x1 = radius * sin(theta1) * cos(phi);
                double y1 = radius * cos(theta1);
                double z1 = radius * sin(theta1) * sin(phi);

                double x2 = radius * sin(theta2) * cos(phi);
                double y2 = radius * cos(theta2);
                double z2 = radius * sin(theta2) * sin(phi);

                glVertex3d(x1, y1, z1);
                glVertex3d(x2, y2, z2);
                glNormal3d((x1+x2)/2, (y1+y2)/2, (z1+z2)/2);
            }
        }
        glEnd();
        glFlush();
        pos->disable();
        rotation->disable();
    }
#endif
