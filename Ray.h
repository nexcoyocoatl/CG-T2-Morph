#ifndef RAY_H
#define RAY_H

#include "Ponto.h"

struct TrianglePlane; // Resolve dependência circular

struct Ray
{
    Ponto origin;
    Ponto direction;
    Ponto end;
    float length;

    Ray(Ponto _direction) : origin {}, direction {_direction}, length {}, end {}
    {
    }

    bool b_intersectPlane(TrianglePlane plane);

    void draw();
};

#endif