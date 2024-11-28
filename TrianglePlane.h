#ifndef  TRIANGLEPLANE_H
#define  TRIANGLEPLANE_H

#include "Ponto.h"

struct Ray; // Resolve dependência circular

#include <iostream>

struct TrianglePlane
{
    // Vértices e normal do triângulo
    Ponto v0, v1, v2;
    Ponto n;
    // Constante "d" do plano
    float d;

    // Único vetor necessário do triângulo para encontrar intersecção
    Ponto v0v1;
    
    TrianglePlane(Ponto _v0, Ponto _v1, Ponto _v2);

    // Encontra intersecção pelo cálculo de um suposto centro nas coordenadas barimétricas do triângulo
    bool b_intersectTriangle(Ponto p);
};

#endif