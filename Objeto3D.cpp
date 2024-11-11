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

#include "Objeto3D.h"

#include <fstream>

// teste
#include <iostream>

Objeto3D::Objeto3D()
    : vertices {}, faces {}, position {Ponto(0.0, 0.0, 0.0)}
{
}

float Objeto3D::getRotationAngle()
{
    return rotation[3];
}

void Objeto3D::setRotation(float angle, float x, float y, float z)
{
    rotation[3] = angle;
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
}

void Objeto3D::LoadFile(std::string file)
{
    std::ifstream f (file);

    // leitor de .obj baseado na descrição em https://en.wikipedia.org/wiki/Wavefront_.obj_file    
    
    std::string line;

    while (std::getline(f, line))
    {
        if (line[0] != 'v' && line[0] != 'f')
        {
            continue;
        }

        std::vector<std::string> values;
        std::string temp {};
        
        for (size_t i = 0; i < ((int)line.size()); i++)
        {
            if (line[i] == ' ' || line[i] == '\n')
            {
                if (!temp.empty())
                {
                    values.push_back(temp);
                    temp.clear();
                }
            }
            else
            {
                temp += line[i];
            }
        }
        values.push_back(temp);
        temp.clear();        

        if (line[0] == 'v' && line[1] == ' ')
        {
            // item é um vértice, os outros elementos da linha são a posição dele
            vertices.push_back(Ponto(std::stof(values[1]), std::stof(values[2]), std::stof(values[3])));
        }

        if (line[0] == 'f')
        {
            // item é uma face, os outros elementos da linha são dados sobre os vértices dela
            faces.push_back(std::vector<int>()); // inicializa um novo vector de inteiros vazio

            for (size_t i = 1; i < values.size(); i++)
            {
                faces.back().push_back(std::stoi(values[i].substr(0, values[i].find('/', 0))) - 1); // primeiro elemento é índice do vértice da face
                // ignoramos textura e normal
            }              
        }   
        // ignoramos outros tipos de items, no exercício não é necessário e vai só complicar mais

        values.clear();
    }
    f.close();
}

void Objeto3D::DesenhaVertices()
{
    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.1, 0.1, 0.8);
    glPointSize(8);

    glBegin(GL_POINTS);
    for (Ponto v : vertices)
    {
        glVertex3f(v.getX(), v.getY(), v.getZ());
    }
    glEnd();
    
    glPopMatrix();
}

void Objeto3D::DesenhaWireframe()
{
    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0, 0, 0);
    glLineWidth(2);
    
    for (std::vector<int> f : faces)
    {
        glBegin(GL_LINE_LOOP);
        for (int iv : f)
        {
            glVertex3f(vertices[iv].getX(), vertices[iv].getY(), vertices[iv].getZ());
        }
        glEnd();
    }

    glPopMatrix();
}

void Objeto3D::Desenha()
{
    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.34, 0.34, 0.34);
    glLineWidth(2);
    
    for (std::vector<int> f : faces)
    {
        glBegin(GL_TRIANGLE_FAN);
        for (int iv : f)
        {
            glVertex3f(vertices[iv].getX(), vertices[iv].getY(), vertices[iv].getZ());
        }            
        glEnd();
    }
    
    glPopMatrix();
}


