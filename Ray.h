#ifndef RAY_H
#define RAY_H

#include "Ponto.h"

struct TrianglePlane; // Resolve dependência circular

// Classe de raio para raycasting
struct Ray
{
    Ponto origin;
    Ponto direction;
    Ponto end;
    float length;

    Ray(Ponto _direction) : origin {}, direction {_direction}, length {}, end {}
    {
    }

    // Acha intersecção no plano (não ainda no triângulo)
    bool b_intersectPlane(TrianglePlane plane);

    // Para ser desenhada como um segmento de linha
    void draw();
};

#endif