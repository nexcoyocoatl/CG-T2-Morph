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

#include "TrianglePlane.h"
// #include "Ray.h"

#include <fstream>
#include <algorithm>

// teste
#include <iostream>

Objeto3D::Objeto3D()
    : vertices {}, faces {}, position {Ponto(0.0f, 0.0f, 0.0f)}
{
}

Objeto3D::Objeto3D(std::string filename)
    : vertices {}, faces {}, position {Ponto(0.0f, 0.0f, 0.0f)}
{
    LoadFile(filename);
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

size_t Objeto3D::getNFaces()
{
    return faces.size();
}

size_t Objeto3D::getNTris()
{
    return tris.size();
}

size_t Objeto3D::getNQuads()
{
    return quads.size();
}

size_t Objeto3D::getNNgons()
{
    return ngons.size();
}

size_t Objeto3D::getNVertices()
{
    return vertices.size();
}

Ponto* Objeto3D::getVertice(size_t i)
{
    return &vertices[i];
}

// std::vector <size_t> Objeto3D::getFace(size_t iv)
// {
//     std::vector <size_t> temp;
//     copy(faces[iv].begin(), faces[iv].end(), back_inserter(temp));
//     return temp;
// }

std::vector <size_t>* Objeto3D::getFace(size_t iv)
{
    return &faces[iv];
}

// Plane Objeto3D::getPlaneFromFace(size_t iv)
// {
//     Ponto v0v1 = vertices[faces[iv][1]] - vertices[faces[iv][0]];
//     Ponto v0v2 = vertices[faces[iv][2]] - vertices[faces[iv][0]];
    
//     Ponto n = Ponto::crossProduct(&v0v1, &v0v2);

//     float d = -(Ponto::dotProduct(&n, &vertices[faces[iv][0]]));

//     return Plane(n, d);
// }

TrianglePlane Objeto3D::getTrianglePlane(size_t iv)
{
    return TrianglePlane(vertices[faces[iv][0]], vertices[faces[iv][1]], vertices[faces[iv][2]]);
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
    // SubdivideMesh(1);
    // TriangulaMesh();
    SubdivideMesh(1);
    TriangulaMesh();

    // Para teste dos planos
    // TriangulaFace(0);
    // TriangulaFace(1);
    // TriangulaFace(2);

    // Subdivide em
    // SubdivideFaceEm(0, 8); // Octagono
    // SubdivideFaceEm(1, 16); // Hexadecagono
    // SubdivideFaceEm(2, 5); // Pentagono
}

void Objeto3D::DesenhaVertices()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.1f, 0.1f, 0.8f);
    glPointSize(8.0f);

    glBegin(GL_POINTS);
    for (Ponto v : vertices)
    {
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
    
    glPopMatrix();
}

void Objeto3D::DesenhaWireframe()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    
    for (std::vector<size_t> f : faces)
    {
        glBegin(GL_LINE_LOOP);
        for (size_t iv : f)
        {
            glVertex3f(vertices[iv].x, vertices[iv].y, vertices[iv].z);
        }
        glEnd();
    }

    glPopMatrix();
}

void Objeto3D::Desenha()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.34f, 0.34f, 0.34f);
    glLineWidth(2.0f);
    
    for (std::vector<size_t> f : faces)
    {
        glBegin(GL_TRIANGLE_FAN);
        for (size_t iv : f)
        {            
            glVertex3f(vertices[iv].x, vertices[iv].y, vertices[iv].z);
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
        x += vertices[iv].x;
        y += vertices[iv].y;
        z += vertices[iv].z;
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
        centroides.emplace_back(CalculaCentroide(i));
    }
}

// Faz o desenho dos centroides de cada face no formato de um um vertice
void Objeto3D::DesenhaCentroides()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation[3], rotation[0], rotation[1], rotation[2]);
    glColor3f(0.8f, 0.1f, 0.1f);
    glPointSize(8.0f);

    glBegin(GL_POINTS);
    for (Ponto c : centroides)
    {
        glVertex3f(c.x, c.y, c.z);
    }
    glEnd();  

    glPopMatrix();
}

// WIP: Não sei se vou fazer
void Objeto3D::SimplifyGeometry()
{

}

// Divide determinadas vezes (funciona, mas dá bug no draw, por alguma razão, como se fizesse TRIANGLE_FAN)
void Objeto3D::SubdivideFaceEm(size_t faceIndex, size_t times)
{
    int num_vertices = faces[faceIndex].size();

    // Se não é uma face, se divide por uma vez só, ou sé é mais que o número de vértices, ignora
    if (num_vertices < 3 || times == 1 || times > num_vertices)
    {
        return;
    }

    // Cria novos vértices
    for (size_t i = 0; i < times; i++)
    {
        vertices.emplace_back(Ponto((vertices[faces[faceIndex][i]].x + vertices[faces[faceIndex][(i+1) % faces[faceIndex].size()]].x)/2.0f, // (% faces[f].size()) volta a zero se passa do número máximo do vetor
                                    (vertices[faces[faceIndex][i]].y + vertices[faces[faceIndex][(i+1) % faces[faceIndex].size()]].y)/2.0f,
                                    (vertices[faces[faceIndex][i]].z + vertices[faces[faceIndex][(i+1) % faces[faceIndex].size()]].z)/2.0f));
    }

    vertices.push_back(centroides[faceIndex]); // Adiciona centroide da face original aos vertices

    // Cria num_vertices-1 (1 até num_vertices) novos quads (porque o inicial será modificado depois)
    for (size_t i = 1; i < times; i++)
    {
        faces.push_back(std::vector<size_t>(4)); // Adiciona uma nova face

        faces.back()[0] = faces[faceIndex][i]; // Adiciona o primeiro vértice, de 1 até num_vertices
        faces.back()[1] = vertices.size()-(times + 1 - i); // Adiciona o vértice anterior ao vértice i da face
        faces.back()[2] = vertices.size()-1; // Centróide
        faces.back()[3] = vertices.size()-(times + 2 - i); // Adiciona o vértice posterior ao vértice i da face

        centroides.push_back(CalculaCentroide(faces.size()-1)); // Calcula centroide da nova face
    }

    // Modifica face existente
    for (size_t i = 0; i < times-1; i++)
    {
        faces[faceIndex].erase(faces[faceIndex].begin()+1);
    }
    faces[faceIndex].insert(faces[faceIndex].begin() + 1, vertices.size() - 2); // Último vértice a ser criado na subdivisão
    faces[faceIndex].insert(faces[faceIndex].begin() + 1, vertices.size() - 1); // Index do centróide
    faces[faceIndex].insert(faces[faceIndex].begin() + 1, (vertices.size() - (times + 1))); // Index do primeiro vértice criado na subdivisão

    centroides[faceIndex] = CalculaCentroide(faceIndex);

    if (num_vertices == 4)
    {
        return;
    }

    if (num_vertices > 4)
    {
        ngons.erase(std::remove(ngons.begin(), ngons.end(), faceIndex), ngons.end());
    }
    else
    {
        tris.erase(std::remove(tris.begin(), tris.end(), faceIndex), tris.end());
    }

    quads.push_back(faceIndex);
}

// Subdivide uma face da mesh (genérico)
void Objeto3D::SubdivideFace(size_t faceIndex)
{
    int num_vertices = faces[faceIndex].size();

    SubdivideFaceEm(faceIndex, num_vertices);
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

// Triangula apenas quads
void Objeto3D::TriangulaQuad(size_t faceIndex)
{
    // Ignora se não é quad
    if (faces[faceIndex].size() != 4)
    {
        return;
    }
    
    faces[faceIndex].pop_back(); // Retira um dos vértices, formando um tri no lugar de um quad
    centroides[faceIndex] = CalculaCentroide(faceIndex); // Recalcula centróide da face que agora é um tri

    faces.push_back(std::vector<size_t>(3));
    faces.back()[0] = faces[faceIndex][2];
    faces.back()[1] = faces[faceIndex][3];
    faces.back()[2] = faces[faceIndex][0];
    
    tris.push_back(faceIndex);
    tris.push_back(faces.size() - 1);
    centroides.push_back(CalculaCentroide(faces.size() - 1)); // Calcula centroide do novo tri
    
    quads.erase(std::remove(quads.begin(), quads.end(), faceIndex), quads.end()); // Apaga face do vetor de quads (precisa ser assim porque usa iterator)
}

void Objeto3D::TriangulaFace(size_t faceIndex)
{
    size_t num_vertices = faces[faceIndex].size();

    // Se é triângulo, ou até aresta ou vértice, já retorna
    if (num_vertices <= 3)
    {
        return;
    }

    if (num_vertices == 4)
    {
        TriangulaQuad(faceIndex);
        return;
    }

    // Cria um novo vetor para modificar a face original (ineficiente, mas funciona relativamente bem)
    std::vector<size_t> centerFace;

    // Cria as "orelhas" a partir da face original
    for (size_t i = 0; i < num_vertices - 1; i += 2)
    {        
        centerFace.push_back(faces[faceIndex][i]);

        faces.push_back(std::vector<size_t>(3));
        faces.back()[0] = faces[faceIndex][i];
        faces.back()[1] = faces[faceIndex][i + 1];
        faces.back()[2] = faces[faceIndex][(i + 2) % num_vertices];
        tris.push_back(faces.size() - 1);

        // Calcula o centróide do triângulo
        centroides.push_back(CalculaCentroide(faces.size() - 1));
    }

    // Adiciona os vértices que faltam a face original    
    centerFace.push_back(faces[faceIndex][num_vertices - 1]);
    if (num_vertices % 2 != 0)
    {
        centerFace.push_back(faces[faceIndex][num_vertices - 2]);
    }
    
    // Move os elementos pra face original
    faces[faceIndex].clear();
    std::move(centerFace.begin(), centerFace.begin() + centerFace.size()-1, std::back_inserter(faces[faceIndex]));
    centerFace.clear();

    // Recalcula centróide da face original modificada
    centroides[faceIndex] = CalculaCentroide(faceIndex); // Recalcula centróide da face que agora é central às "orelhas"

    // Faz recursivamente até que só existam triângulos
    if (faces[faceIndex].size() > 3)
    {
        TriangulaFace(faceIndex);
    }
    else
    {
        ngons.erase(std::remove(ngons.begin(), ngons.end(), faceIndex), ngons.end());
        tris.push_back(faceIndex);
    }
}

// Triangula todos quads da mesh
void Objeto3D::TriangulaMesh()
{
    size_t num_faces = faces.size();
    
    for (size_t f = 0; f < num_faces; f++)
    {
        // TriangulaQuad(f);
        TriangulaFace(f);
    }
}

// Subdivide até chegar exatamente no número de faces determinado
void Objeto3D::SubdivideUntil(size_t num_faces)
{
    if (faces.size() > num_faces)
    {
        return;
    }

    if (ngons.size() > 0)
    {
        size_t ngons_verts = 0;

        for (size_t ngon : ngons)
        {
            ngons_verts += faces[ngon].size();
        }

    
        // if (ngons_verts * )
    }

    // while (faces.size())
}



void Objeto3D::teste()
{
    std::cout << "Tris: " << tris.size() << " Quads: " << quads.size() << " Ngons: " << ngons.size() << "\n";
    std::cout << "Faces size: " << faces.size() << " Centroides: " << centroides.size() << "\n";

    // size_t n_tris = 0;
    // size_t n_quads = 0;
    // size_t n_ngons = 0;
    // size_t n_wtf = 0;

    // for (size_t i = 0; i < faces.size(); i++)
    // {
    //     if (faces[i].size() == 3)
    //     {
    //         n_tris++;
    //     }
    //     else if (faces[i].size() == 4)
    //     {
    //         n_quads++;
    //     }
    //     else if (faces[i].size() > 4)
    //     {
    //         n_ngons++;
    //     }
    //     else
    //     {
    //         n_wtf++;
    //     }
    // }

    // std::cout << "Tris: " << n_tris << " Quads: " << n_quads << " Ngons: " << n_ngons << " wtf?!: " << n_wtf << "\n";
    

    // for (size_t i = 0; i < tris.size(); i++)
    // {
    //     std::cout << tris[i] << ", ";
    // }
    // std::cout << "\n";    
}