//subliminal:headers

//endsubliminal:headers



#ifndef GAMA_VECTOR_HPP_INCLUDED
#define GAMA_VECTOR_HPP_INCLUDED
    #include <gl/gl.h>
    #include <algorithm>
    #include <iostream>
    class Point {
    public:
        Point():x(0), y(0), z(0)
        {};
        Point(double x, double y,double z):x(x), y(y), z(z)
        {};
        void set(double val[3]) {
            x = val[0];
            y = val[1];
            z = val[2];
        }
        void set(double _x, double _y, double _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }
        float* farray() const {
            float *arr = (float*)malloc(3*sizeof(float));
            arr[0] = (float)x;
            arr[1] = (float)y;
            arr[2] = (float)z;
            return arr;
        }
        double* array() const {
            double *arr = (double*)malloc(3*sizeof(double));
            arr[0] = x;
            arr[1] = y;
            arr[2] = z;
            return arr;
        }

        double x, y, z;
    };

    class Vector: public Point
    {
    public:
        Vector():Point(), vel(0), acc(0)
        {
            vel = new Point();
            acc = new Point();
        }
        Vector(double x, double y,double z):Point(x, y, z), vel(0), acc(0)
        {
            vel = new Point();
            acc = new Point();
        }
        void enable() const {
            //std::cout << "x: " << x << "y: " << y << "z: " << z;
            glTranslated(x, y, z);
        }
        void disable() const {
            glTranslated(-x, -y, -z);
        }
        void update(double theta)
        {
            x += vel->x * theta;
            y += vel->y * theta;
            z += vel->z * theta;

            vel->x += acc->x * theta;
            vel->y += acc->y * theta;
            vel->z += acc->z * theta;
        }
        Point *vel, *acc;
    };
    class Rotation: public Point
    {
    public:
        Rotation():Point(), vel(0), acc(0)
        {
            vel = new Point();
            acc = new Point();
        }
        Rotation(double x, double y,double z):Point(x, y, z), vel(0), acc(0)
        {
            vel = new Point();
            acc = new Point();
        }
        void enable() const {
            double m = std::max(x, std::max(y, z));
            if(m == 0) return;
            glRotated(m, x / m, y / m, z / m);
        };
        void disable() const {
            double m = std::max(x, std::max(y, z));
            if(m == 0) return;
            glRotated(-m, x / m, y / m, z / m);
        };
        void update(double theta)
        {
            x += vel->x * theta;
            y += vel->y * theta;
            z += vel->z * theta;

            vel->x += acc->x * theta;
            vel->y += acc->y * theta;
            vel->z += acc->z * theta;
        }
        Point *vel, *acc;
    };

#endif
