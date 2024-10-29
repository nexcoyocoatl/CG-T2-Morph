#ifndef PONTO_H
#define PONTO_H

class Ponto
{
private:
    float m_x, m_y, m_z;

public:
    Ponto(float x, float y, float z);
    void set(float x, float y, float z);

    float getX();
    float getY();
    float getZ();

    Ponto __add__(float x, float y, float z);
    Ponto __sub__(float x, float y, float z);
    Ponto __mul__(int n);

    void rotacionaX(float angulo);
    void rotacionaY(float angulo);
    void rotacionaZ(float angulo);

    static bool intersec2d(Ponto k, Ponto l, Ponto m, Ponto n);
    // bool ha_intersec(Ponto k, Ponto l, Ponto m, Ponto n);
};

#endif