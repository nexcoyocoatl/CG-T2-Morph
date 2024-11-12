#include "Ponto.h"
#include <cmath>

#include <iostream>

#define PI 3.14159265359

Ponto::Ponto(float x, float y, float z)
    : m_x {x}, m_y {y}, m_z {z}
{
}

float Ponto::getX()
{
    return m_x;
}
float Ponto::getY()
{
    return m_y;
}
float Ponto::getZ()
{
    return m_z;
}

void Ponto::set(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

Ponto Ponto::__add__(float x, float y, float z)
{
    x += m_x;
    y += m_y;
    return Ponto(x, y, 0.0f);
}

Ponto Ponto::__sub__(float x, float y, float z)
{
    x -= m_x;
    y -= m_y;
    return Ponto(x, y, 0.0f);
}

Ponto Ponto::__mul__(int n)
{
    int x, y {};
    x *= n;
    y *= n;
    return Ponto(x, y, 0.0f);
}

void Ponto::rotacionaX(float angulo)
{
    float anguloRad = angulo * PI/180.0f;
    float xr;
    float yr;
    m_x = xr;
    m_y = yr;
}

void Ponto::rotacionaY(float angulo)
{
    float anguloRad = angulo * PI/180.0f;
    float xr =  m_x*cos(anguloRad) + m_z*sin(anguloRad);
    float zr = -m_x*sin(anguloRad) + m_z*cos(anguloRad);
    m_x = xr;
    m_y = zr;
}

void Ponto::rotacionaZ(float angulo)
{
    float anguloRad = angulo * PI/180.0f;
    float xr = m_x*cos(anguloRad) - m_y*sin(anguloRad);
    float yr = m_x*sin(anguloRad) + m_y*cos(anguloRad);
    m_x = xr;
    m_y = yr;
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
    float det = (n.getX() - m.getX()) * (l.getY() - k.getY())  -  (n.getY() - m.getY()) * (l.getX() - k.getX()); // usar getter pra cada valor de eixo?

    if (det == 0.0f)
    {
        return false; // não há intersecção
    }

    float s = ((n.getX() - m.getX()) * (m.getY() - k.getY()) - (n.getY() - m.getY()) * (m.getX() - k.getX()))/ det;
    float t = ((l.getX() - k.getX()) * (m.getY() - k.getY()) - (l.getY() - k.getY()) * (m.getX() - k.getX()))/ det;

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