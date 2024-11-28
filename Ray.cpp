#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>
#include <cmath>

#include "Ray.h"
#include "Ponto.h"
#include "TrianglePlane.h"

bool Ray::b_intersectPlane(TrianglePlane plane)
{
    // Encontra denominador primeiro como guard clause para poupar processamento e também não dividir por zero
    float denominator = Ponto::dotProduct(&plane.n, &this->direction);
    if (std::abs(denominator) <= 1e-4f)
    {
        // std::cout << "MENOR QUE ZERO\n";
        length = 0;
        return false;
    }

    // Contrário da implementação usual de raycast
    // Nesse caso, só encontra as faces que estão ao contrário da direção do raio
    if (-denominator > 1e-4f)
    {
        length = 0;
        return false;
    }

    // Encontra a escalar da reta através da combinação algébrica das equações da reta e do plano
    length = -(Ponto::dotProduct(&plane.n, &this->origin) + plane.d) / denominator;

    // Se não está dentro do plano
    if (length <= 1e-4f)
    {
        length = 0;
        return false;
    }

    // Equação da reta para encontrar o ponto exato de intersecção no plano
    end = origin + (direction * length);

    // Se chegou aqui, existe intersecção com o plano infinito
    // Agora testa para interseção no triângulo
    if (!plane.b_intersectTriangle(end))
    {
        return false;
    }

    return true;
}

// Equação de desenho
void Ray::draw()
{
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex3f(direction.x, direction.y, direction.z);
        glVertex3f(end.x, end.y, end.z);
    glEnd();
}