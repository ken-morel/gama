#include "Vector.hpp"
#include "Color.hpp"
#include <gl/gl.h>
#ifndef GAMA_SHAPE_HPP_INCLUDED
#define GAMA_SHAPE_HPP_INCLUDED value
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
#endif
