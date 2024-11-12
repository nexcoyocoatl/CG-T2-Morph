#ifndef OBJETO_3D
#define OBJETO_3D

#include "Ponto.h"

#include <vector>
#include <string>

class Objeto3D
{
private:
    std::vector<Ponto> vertices;
    std::vector<std::vector <int>> faces;
    std::vector<Ponto> centroides;
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
    void CalculaCentroides();
    void DesenhaCentroides();
    void SubdivideMesh();
};

#endif