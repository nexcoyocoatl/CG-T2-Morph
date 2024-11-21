#ifndef  PLANE_H
#define  PLANE_H

#include "Ponto.h"

// #include <iostream>

struct Plane
{
    Ponto n;
    float d;

    Plane()
    {        
    }

    Plane(Ponto _n, float _d) : n {_n}, d {_d}
    {
        // std::cout << "n: " << n.x << "," << n.y << "," << n.z << " d: " << d << "\n";
    }
};

#endif