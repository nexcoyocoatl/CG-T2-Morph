#include "Ponto.h"
#include <cmath>

#include <iostream>

#define PI 3.14159265359

Ponto::Ponto() : x {}, y {}, z{}
{
}

Ponto::Ponto(float _x, float _y, float _z)
    : x {_x}, y {_y}, z {_z}
{
}

void Ponto::set(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Ponto& operator+=(Ponto &p1, const Ponto &p2)
{
    p1.x += p2.x;
    p1.y += p2.y;
    p1.z += p2.z;
    return p1;
}

Ponto& operator-=(Ponto &p1, const Ponto &p2)
{
    p1.x -= p2.x;
    p1.y -= p2.y;
    p1.z -= p2.z;
    return p1;
}

Ponto& operator*=(Ponto &p1, const Ponto &p2)
{
    p1.x *= p2.x;
    p1.y *= p2.y;
    p1.z *= p2.z;
    return p1;
}

Ponto& operator*=(Ponto &p1, const int s)
{
    p1.x *= s;
    p1.y *= s;
    p1.z *= s;
    return p1;
}

Ponto& operator/=(Ponto &p1, const Ponto &p2)
{
    p1.x /= p2.x;
    p1.y /= p2.y;
    p1.z /= p2.z;
    return p1;
}

// Ponto operator+(const Ponto &p1, const Ponto &p2)
// {
//     return Ponto(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
// }

// Ponto operator-(const Ponto &p1, const Ponto &p2)
// {
//     return Ponto(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
// }

// Ponto operator*(const Ponto &p1, const Ponto &p2)
// {
//     return Ponto(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
// }

Ponto Ponto::operator+(const Ponto &p2) const
{
    return Ponto(this->x + p2.x, this->y + p2.y, this->z + p2.z);
}

Ponto Ponto::operator-(const Ponto &p2) const
{
    return Ponto(this->x - p2.x, this->y - p2.y, this->z - p2.z);
}

Ponto Ponto::operator*(const Ponto &p2) const
{
    return Ponto(this->x * p2.x, this->y * p2.y, this->z * p2.z);
}

Ponto Ponto::operator*(float scalar) const
{
    return Ponto(this->x * scalar, this->y * scalar, this->z * scalar);
}

float Ponto::dotProduct(const Ponto* p1, const Ponto* p2)
{
    // std::cout << "dotproduct: " << (p1->x * p2->x) + (p1->y * p2->y) + (p1->z * p2->z) <<  "\n";
    // std::cout << "p1: " << p1->x << "," << p1->y << "," << p1->z << "\n";
    // std::cout << "p2: " << p2->x << "," << p2->y << "," << p2->z << "\n";

    return ((p1->x * p2->x) + (p1->y * p2->y) + (p1->z * p2->z));
}

Ponto Ponto::crossProduct(const Ponto* p1, const Ponto* p2)
{

    float x = (p1->y * p2->z) - (p1->z * p2->y);
    float y = (p1->z * p2->x) - (p1->x * p2->z);
    float z = (p1->x * p2->y) - (p1->y * p2->x);

    // std::cout << "crossproduct: " << x << ", " << y << ", " << z <<  "\n";

    return Ponto(x, y, z);
}

void Ponto::rotacionaX(float angulo)
{
    float anguloRad = angulo * PI/180.0f;
    float xr;
    float yr;
    x = xr;
    y = yr;
}

void Ponto::rotacionaY(float angulo)
{
    float anguloRad = angulo * PI/180.0f;
    float xr =  x*cos(anguloRad) + z*sin(anguloRad);
    float zr = -x*sin(anguloRad) + z*cos(anguloRad);
    x = xr;
    y = zr;
}

void Ponto::rotacionaZ(float angulo)
{
    float anguloRad = angulo * PI/180.0f;
    float xr = x*cos(anguloRad) - y*sin(anguloRad);
    float yr = x*sin(anguloRad) + y*cos(anguloRad);
    x = xr;
    y = yr;
}

/*
# ********************************************************************** #
#                                                                        #
#  Calcula a interseccao entre 2 retas (no plano "XY" Z = 0)             #
#                                                                        #
# k : ponto inicial da reta 1                                            #
# l : ponto final da reta 1                                              #
# m : ponto inicial da reta 2                                            #
# n : ponto final da reta 2                                              #
#                                                                        #
# Retorna:                                                               #
# 0, se não houver interseccao ou 1, caso haja                           #
# int, valor do parâmetro no ponto de interseção (sobre a reta KL)       #
# int, valor do parâmetro no ponto de interseção (sobre a reta MN)       #
#                                                                        #
##########################################################################
*/
bool Ponto::intersec2d(Ponto k, Ponto l, Ponto m, Ponto n) // TO-DO: É estática?
{
    float det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0f)
    {
        return false; // não há intersecção
    }

    float s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det;
    float t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det;

    return (s >= 0.0f) and (s <= 1.0f) and (t >= 0.0f) and (t <= 1.0f);
}

/*
# **********************************************************************
# HaInterseccao(k: Ponto, l: Ponto, m: Ponto, n: Ponto)
# Detecta interseccao entre os pontos
#
# **********************************************************************
*/
// bool Ponto::ha_intersec(Ponto k, Ponto l, Ponto m, Ponto n)
// {
//     bool result = intersec2d(k,  l,  m,  n);

//     if not ret: return False

//     return s>=0.0 and s <=1.0 and t>=0.0 and t<=1.0
// };