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
    Ponto position;
    float rotation[4] {0.0, 0.0, 0.0, 0.0};

public:
    Objeto3D();
    float getRotationAngle();
    void setRotation(float angle, float x, float y, float z);
    void LoadFile(std::string file);
    void DesenhaVertices();
    void DesenhaWireframe();
    void Desenha();
};

#endif