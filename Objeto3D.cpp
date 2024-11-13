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
#include <algorithm>

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
        
        for (size_t i = 0; i < line.size(); i++)
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
            faces.push_back(std::vector<size_t>()); // inicializa um novo vector de size_t vazio

            for (size_t i = 1; i < values.size(); i++)
            {
                faces.back().push_back(std::stoi(values[i].substr(0, values[i].find('/', 0))) - 1); // primeiro elemento é índice do vértice da face
            }

            centroides.push_back(CalculaCentroide(faces.size()-1));

            if (values.size() == 4)
            {
                tris.push_back(faces.size()-1);
            }
            else if (values.size() == 5)
            {
                quads.push_back(faces.size()-1);
            }
            else
            {
                ngons.push_back(faces.size()-1);
            }
        }   
        // ignoramos outros tipos de items, no exercício não é necessário e vai só complicar mais

        values.clear();
    }
    f.close();

    std::cout << "Tris: " << tris.size() << " Quads: " << quads.size() << " Ngons: " << ngons.size() << "\n";

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
    
    for (std::vector<size_t> f : faces)
    {
        glBegin(GL_LINE_LOOP);
        for (size_t iv : f)
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
    
    for (std::vector<size_t> f : faces)
    {
        glBegin(GL_TRIANGLE_FAN);
        for (size_t iv : f)
        {
            glVertex3f(vertices[iv].getX(), vertices[iv].getY(), vertices[iv].getZ());
        }            
        glEnd();
    }
    
    glPopMatrix();
}

Ponto Objeto3D::CalculaCentroide(size_t faceIndex)
{
    float n = static_cast<float>(faces[faceIndex].size());
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    for (size_t iv : faces[faceIndex])
    {
        x += vertices[iv].getX();
        y += vertices[iv].getY();
        z += vertices[iv].getZ();
    }
    return Ponto(x/n, y/n, z/n);
}

void Objeto3D::RecalculaCentroides()
{
    if (!centroides.empty())
    {
        centroides.clear();
    }

    centroides.reserve(faces.size());

    for (size_t i = 0; i < faces.size(); i++)
    {
        centroides[i] = CalculaCentroide(i);
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
    // std::vector<std::vector <size_t>> new_faces;

    // std::vector<size_t[3]> subdivided_faces; // quads, mas um deles sempre é o centróide

    // for (std::vector<size_t> f : faces)
    for (size_t f = 0; f < faces.size(); f++)
    {
        // subdivided_faces.clear();

        float n = static_cast<float>(faces[f].size());

        for (size_t i = 0; i < faces[f].size()-1; i++)
        {
            vertices.emplace_back(Ponto((vertices[faces[f][i]].getX() + vertices[faces[f][(i+1) % faces[f].size()]].getX())/2.0f, // (% faces[f].size()) volta a zero se passa do número máximo do vetor
                                        (vertices[faces[f][i]].getY() + vertices[faces[f][(i+1) % faces[f].size()]].getY())/2.0f,
                                        (vertices[faces[f][i]].getZ() + vertices[faces[f][(i+1) % faces[f].size()]].getZ())/2.0f));
        }

    //     vertices.push_back(centroides[f]); // Adiciona centroide aos vertices

    //     // Cria n novos quads
    //     for (size_t i = 0; i < faces[f].size(); i++)
    //     {
    //         new_faces.push_back(std::vector<size_t>());

    //         new_faces.back().push_back(static_cast<float>(vertices.size()-1)); // adiciona número do vértice do centroide como primeiro da face (último do vetor)
    //         new_faces.back().push_back(static_cast<size_t>((vertices.size()-1)-i-1)); // último vérice a ser adicionado antes do centróide menos offset de i menos centroide
    //         new_faces.back().push_back(static_cast<size_t>(faces[f][i])); // vertice que já existia
    //         new_faces.back().push_back(static_cast<size_t>((vertices.size()-1)-n-1-i));
    //     }
    }

    // faces.clear();
    // faces = new_faces; // TO DO: ARRUMAR (MUITO INEFICIENTE)

    // CalculaCentroides();
}

void Objeto3D::TriangulaQuad(size_t faceIndex)
{
    faces[faceIndex].pop_back(); // Retira um dos vértices, formando um tri no lugar de um quad
    centroides[faceIndex] = CalculaCentroide(faceIndex); // Recalcula centróide da face que agora é um tri

    faces.push_back(std::vector<size_t>());
    faces.back().push_back(faces[faceIndex][2]);
    faces.back().push_back(faces[faceIndex][3]);
    faces.back().push_back(faces[faceIndex][0]);
    
    // // TO DO: problema
    tris.push_back(faceIndex);
    tris.push_back(faces.size()-1);
    centroides.push_back(CalculaCentroide(faces.size()-1)); // Calcula centroide do novo tri
    
    quads.erase(std::remove(quads.begin(), quads.end(), faceIndex), quads.end()); // Apaga face do vetor quads (precisa ser assim porque usa iterator)
}

void Objeto3D::teste()
{
    std::cout << "Tris: " << tris.size() << " Quads: " << quads.size() << " Ngons: " << ngons.size() << "\n";
    std::cout << "Faces size: " << faces.size() << " Centroides: " << centroides.size() << "\n";

    for (size_t i = 0; i < tris.size(); i++)
    {
        std::cout << tris[i] << ", ";
    }
    std::cout << "\n";    
}