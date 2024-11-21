#ifndef RAY_H
#define RAY_H

#include "Ponto.h"

#include "Plane.cpp"

struct Ray
{
    Ponto origin;
    Ponto direction;
    Ponto end;
    float length;

    Ray(Ponto _direction) : origin {}, direction {_direction}, length {}, end {}
    {
    }

    bool b_intersectPlane(Plane plane);

    void draw();
};

#endif