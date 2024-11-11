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

Linha::Linha(Ponto a, Ponto b)
    : m_a {a}, m_b {b}
{
}

Linha::Linha(float x1, float y1, float z1, float x2, float y2, float z2)
    : m_a {Ponto(x1, y1, z1)}, m_b {Ponto(x2, y2, z2)}
{
}

// Desenha a linha na tela atual
void Linha::desenhaLinha()
{
    glBegin(GL_LINES);
    
    glVertex3f(m_a.getX(), m_a.getY(), m_a.getZ());
    glVertex3f(m_b.getX(), m_b.getY(), m_b.getZ());

    glEnd();
}