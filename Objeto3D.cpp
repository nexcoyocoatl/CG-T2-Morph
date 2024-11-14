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

            centroides.push_back(CalculaCentroide(faces.size() - 1));

            if (values.size() == 4)
            {
                tris.push_back(faces.size() - 1);
            }
            else if (values.size() == 5)
            {
                quads.push_back(faces.size() - 1);
            }
            else
            {
                ngons.push_back(faces.size() - 1);
            }
        }   
        // ignoramos outros tipos de items, no exercício não é necessário e vai só complicar mais

        values.clear();
    }
    f.close();

    std::cout << "Tris: " << tris.size() << " Quads: " << quads.size() << " Ngons: " << ngons.size() << "\n";

    // testes
    // TriangulaMesh();
    // TriangulaMesh();
    // SubdivideMesh(1);
    // TriangulaMesh();
    // SubdivideMesh(8);
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

// Recalcula todos centroides de todas as faces
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

// Faz o desenho dos centroides de cada face no formato de um um vertice
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

void SimplifyGeometry()
{

}

// WIP Divide face em 2
void SubdivideFaceEm2(size_t faceIndex)
{

}

// Subdivide uma face da mesh (acredito ser genérico)
void Objeto3D::SubdivideFace(size_t faceIndex)
{
    int num_vertices = faces[faceIndex].size();

    for (size_t i = 0; i < num_vertices; i++)
    {
        vertices.emplace_back(Ponto((vertices[faces[faceIndex][i]].getX() + vertices[faces[faceIndex][(i+1) % faces[faceIndex].size()]].getX())/2.0f, // (% faces[f].size()) volta a zero se passa do número máximo do vetor
                                    (vertices[faces[faceIndex][i]].getY() + vertices[faces[faceIndex][(i+1) % faces[faceIndex].size()]].getY())/2.0f,
                                    (vertices[faces[faceIndex][i]].getZ() + vertices[faces[faceIndex][(i+1) % faces[faceIndex].size()]].getZ())/2.0f));
    }

    vertices.push_back(centroides[faceIndex]); // Adiciona centroide aos vertices

    // Cria num_vertices-1 (1 até num_vertices) novos quads (porque o inicial será modificada depois)
    for (size_t i = 1; i < num_vertices; i++)
    {
        // WIP: Inserir próxima a original (com .insert()?)
        faces.push_back(std::vector<size_t>()); // Adiciona uma nova face

        faces.back().push_back((faces[faceIndex][i])); // Adiciona o primeiro vértice, de 1 até num_vertices
        faces.back().push_back((vertices.size()-(num_vertices + 1 - i))); // Adiciona o vértice anterior ao vértice i da face
        faces.back().push_back((vertices.size()-1)); // Centróide
        faces.back().push_back((vertices.size()-(num_vertices + 2 - i))); // Adiciona o vértice posterior ao vértice i da face

        centroides.push_back(CalculaCentroide(faces.size()-1)); // Calcula centroide da nova face
    }

    // Modifica face existente
    faces[faceIndex][1] = (vertices.size() - (num_vertices + 1)); // Index do primeiro vértice criado na subdivisão
    faces[faceIndex][2] = (vertices.size() - 1); // Index do centróide
    if (faces[faceIndex].size() < 4) // Se é um triângulo é necessário adicionar ao vetor, ao invés de atribuir um valor a um elemento existente
    {
        faces[faceIndex].push_back((vertices.size() - 2)); // Index do último vértice criado na subdivisão (adicionado antes do centróide)
    }
    else
    {
        faces[faceIndex][3] = (vertices.size() - 2); // Index do último vértice criado na subdivisão (adicionado antes do centróide)
    }

    centroides[faceIndex] = CalculaCentroide(faceIndex);
}

// Subdivide toda mesh determinadas vezes
void Objeto3D::SubdivideMesh(size_t n_times)
{
    for (size_t i = 0; i < n_times; i++)
    {    
        size_t num_faces = faces.size();
        
        for (size_t f = 0; f < num_faces; f++)
        {
            SubdivideFace(f);
        }
    }
}

// WIP: Talvez fazer genérico pra qualquer tipo de polígono?
void Objeto3D::TriangulaQuad(size_t faceIndex)
{
    // Ignora se não é quad
    if (faces[faceIndex].size() == 4)
    {
        faces[faceIndex].pop_back(); // Retira um dos vértices, formando um tri no lugar de um quad
        centroides[faceIndex] = CalculaCentroide(faceIndex); // Recalcula centróide da face que agora é um tri

        faces.push_back(std::vector<size_t>());
        faces.back().push_back(faces[faceIndex][2]);
        faces.back().push_back(faces[faceIndex][3]);
        faces.back().push_back(faces[faceIndex][0]);
        
        tris.push_back(faceIndex);
        tris.push_back(faces.size() - 1);
        centroides.push_back(CalculaCentroide(faces.size() - 1)); // Calcula centroide do novo tri
        
        quads.erase(std::remove(quads.begin(), quads.end(), faceIndex), quads.end()); // Apaga face do vetor de quads (precisa ser assim porque usa iterator)
    }
}

// Triangula todos quads da mesh
void Objeto3D::TriangulaMesh()
{
    size_t num_faces = faces.size();
    
    for (size_t f = 0; f < num_faces; f++)
    {
        TriangulaQuad(f);
    }
}

// Subdivide até chegar exatamente no número de faces determinado
void SubdivideUntil(size_t num_faces)
{

}

void Objeto3D::teste()
{
    std::cout << "Tris: " << tris.size() << " Quads: " << quads.size() << " Ngons: " << ngons.size() << "\n";
    std::cout << "Faces size: " << faces.size() << " Centroides: " << centroides.size() << "\n";

    // for (size_t i = 0; i < tris.size(); i++)
    // {
    //     std::cout << tris[i] << ", ";
    // }
    // std::cout << "\n";    
}