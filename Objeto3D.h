#ifndef OBJETO_3D
#define OBJETO_3D

#include "Ponto.h"

#include <vector>
#include <string>

class Objeto3D
{
private:
    std::vector<Ponto> vertices;
    std::vector<std::vector <size_t>> faces;
    std::vector<Ponto> centroides;

    std::vector<size_t> quads;
    std::vector<size_t> tris;
    std::vector<size_t> ngons;

    Ponto position;
    float rotation[4] {0.0f, 0.0f, 0.0f, 0.0f};

public:
    Objeto3D();
    float getRotationAngle();
    void setRotation(float angle, float x, float y, float z);
    void LoadFile(std::string file);
    void DesenhaVertices();
    void DesenhaWireframe();
    void Desenha();

    Ponto CalculaCentroide(size_t faceIndex);
    void RecalculaCentroides();
    void DesenhaCentroides();

    void SubdivideFace(size_t faceIndex);
    void SubdivideMesh(size_t n_times);
    void TriangulaQuad(size_t faceIndex);
    void TriangulaMesh();

    void teste();
};

#endif