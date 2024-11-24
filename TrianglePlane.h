#ifndef  TRIANGLEPLANE_H
#define  TRIANGLEPLANE_H

#include "Ponto.h"

struct Ray; // Resolve dependência circular

#include <iostream>

struct TrianglePlane
{
    Ponto v0, v1, v2;
    Ponto n;
    float d;

    Ponto v0v1;
    
    TrianglePlane(Ponto _v0, Ponto _v1, Ponto _v2);

    bool b_intersectTriangle(Ponto p);
};

#endif