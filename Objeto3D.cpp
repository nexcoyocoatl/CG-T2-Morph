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
    : vertices {}, faces {}, position {Ponto(0.0f, 0.0f, 0.0f)}
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

    CalculaCentroides();

    SubdivideMesh();
}

void Objeto3D::DesenhaVertices()
{
    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.1f, 0.1f, 0.8f);
    glPointSize(8.0f);

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
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    
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
    glColor3f(0.34f, 0.34f, 0.34f);
    glLineWidth(2.0f);
    
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

void Objeto3D::CalculaCentroides()
{
    for (std::vector<int> f : faces)
    {
        float n = static_cast<float>(f.size());
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        for (int iv : f)
        {
            x += vertices[iv].getX();
            y += vertices[iv].getY();
            z += vertices[iv].getZ();
        }
        centroides.emplace_back(Ponto(x/n, y/n, z/n));
    }
}

void Objeto3D::DesenhaCentroides()
{
    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.8f, 0.1f, 0.1f);
    glPointSize(8.0f);

    glBegin(GL_POINTS);
    for (Ponto c : centroides)
    {
        glVertex3f(c.getX(), c.getY(), c.getZ());
    }
    glEnd();  

    glPopMatrix();
}

void Objeto3D::SubdivideMesh()
{
    // std::vector<std::vector <int>> new_faces;

    // std::vector<int[3]> subdivided_faces; // quads, mas um deles sempre é o centróide

    // for (std::vector<int> f : faces)
    for (size_t f = 0; f < faces.size(); f++)
    {
        // subdivided_faces.clear();

        float n = static_cast<float>(faces[f].size());

        // TO DO: Usar % talvez, ao invés de fazer duas vezes?
        for (size_t i = 0; i < faces[f].size()-1; i++)
        {
            vertices.emplace_back(Ponto((vertices[faces[f][i]].getX() + vertices[faces[f][i+1]].getX())/2.0f,
                                        (vertices[faces[f][i]].getY() + vertices[faces[f][i+1]].getY())/2.0f,
                                        (vertices[faces[f][i]].getZ() + vertices[faces[f][i+1]].getZ())/2.0f));
        }
        vertices.emplace_back(Ponto((vertices[faces[f][faces[f].size()-1]].getX() + vertices[faces[f][0]].getX())/2.0f,
                                        (vertices[faces[f][faces[f].size()-1]].getY() + vertices[faces[f][0]].getY())/2.0f,
                                        (vertices[faces[f][faces[f].size()-1]].getZ() + vertices[faces[f][0]].getZ())/2.0f));

    //     vertices.push_back(centroides[f]); // Adiciona centroide aos vertices

    //     // Cria n novos quads
    //     for (size_t i = 0; i < faces[f].size(); i++)
    //     {
    //         new_faces.push_back(std::vector<int>());

    //         new_faces.back().push_back(static_cast<float>(vertices.size()-1)); // adiciona número do vértice do centroide como primeiro da face (último do vetor)
    //         new_faces.back().push_back(static_cast<int>((vertices.size()-1)-i-1)); // último vérice a ser adicionado antes do centróide menos offset de i menos centroide
    //         new_faces.back().push_back(static_cast<int>(faces[f][i])); // vertice que já existia
    //         new_faces.back().push_back(static_cast<int>((vertices.size()-1)-n-1-i));
    //     }
    }

    // faces.clear();
    // faces = new_faces; // TO DO: ARRUMAR (MUITO INEFICIENTE)

    // CalculaCentroides();
}
