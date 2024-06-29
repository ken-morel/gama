#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include "Vector.hpp"
#include <iostream>
#include <gl/gl.h>

//subliminal:headers

//endsubliminal:headers

class Bitmap
{
public:
	Bitmap():data(nullptr)
	{
		pos = new Vector();
		size = new Vector();
		rotation = new Rotation();
	}
	Bitmap(std::string path)
	{
		data = stbi_load(path.c_str(), &width, &height, &bit_depth, 4);
		if(data == NULL) std::cout << "no";
		pos = new Vector();
		size = new Vector(2, 2, 0);
		rotation = new Rotation();
	}
	void render();
	void render(double rx, double ry, double rwidth, double rheight);
	unsigned char *data;
	int width, height, bit_depth;
	Vector *pos, *size;
	Rotation *rotation;
};

void Bitmap::render()
{
	double
		pw = size->x / width,
		ph = size->y / height,
		begin_x = pos->x - (size->x / 2),
		begin_y = pos->y - (size->y / 2);
	glBegin(GL_QUADS);
		for(int x = 0; x < width; x++)
		{
			for(int y  = 0; y < height; y++)
			{
				int px = x, py = height-y-1;
				float color[3] = {
					(float)data[py*width*4 + px*4 + 0] / 255.0f,
					(float)data[py*width*4 + px*4 + 1] / 255.0f,
					(float)data[py*width*4 + px*4 + 2] / 255.0f,
				};
				float alpha = (float)data[py*width*4 + px*4 + 3] / 255.0f;
				if(alpha == 0.0f) continue;
				glNormal3f(0.0f, 0.0f, -1.0f);
				glColor3fv(color);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
				glMaterialfv(GL_FRONT, GL_SPECULAR, color);
				glVertex3d(begin_x+pw*x    , begin_y+ph*y, pos->z);
				glVertex3d(begin_x+pw*(x+1), begin_y+ph*y, pos->z);
				glVertex3d(begin_x+pw*(x+1), begin_y+ph*(y+1), pos->z);
				glVertex3d(begin_x+pw*x    , begin_y+ph*(y+1), pos->z);
			}
		}
	glEnd();
}

void Bitmap::render(double rx, double ry, double rwidth, double rheight)
{
	double
		pw = rwidth / width,
		ph = rheight / height,
		begin_x = rx - (rwidth / 2),
		begin_y = ry - (rheight / 2);
	glBegin(GL_QUADS);
		for(int x = 0; x < width; x++)
		{
			for(int y  = 0; y < height; y++)
			{
				int px = x, py = height-y-1;
				float color[3] = {
					(float)data[py*width*4 + px*4 + 0] / 255.0f,
					(float)data[py*width*4 + px*4 + 1] / 255.0f,
					(float)data[py*width*4 + px*4 + 2] / 255.0f,
				};
				float alpha = (float)data[py*width*4 + px*4 + 3] / 255.0f;
				if(alpha == 0.0f) continue;
				glNormal3f(0.0f, 0.0f, -1.0f);
				glColor3fv(color);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
				glMaterialfv(GL_FRONT, GL_SPECULAR, color);
				glVertex3d(begin_x+pw*x    , begin_y+ph*y, pos->z);
				glVertex3d(begin_x+pw*(x+1), begin_y+ph*y, pos->z);
				glVertex3d(begin_x+pw*(x+1), begin_y+ph*(y+1), pos->z);
				glVertex3d(begin_x+pw*x    , begin_y+ph*(y+1), pos->z);
			}
		}
	glEnd();
}
