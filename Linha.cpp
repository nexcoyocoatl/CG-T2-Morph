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

#include "Linha.h"

Linha::Linha(Ponto _a, Ponto _b)
    : a {_a}, b {_b}
{
}

Linha::Linha(float x1, float y1, float z1, float x2, float y2, float z2)
    : a {Ponto(x1, y1, z1)}, b {Ponto(x2, y2, z2)}
{
}

// Desenha a linha na tela atual
void Linha::desenhaLinha()
{
    glBegin(GL_LINES);
    
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);

    glEnd();
}