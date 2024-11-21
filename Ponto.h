#ifndef PONTO_H
#define PONTO_H

class Ponto
{
public:
    float x, y, z;
    Ponto();
    Ponto(float _x, float _y, float _z);
    void set(float _x, float _y, float _z);

    Ponto operator+(const Ponto &p2) const;
    Ponto operator-(const Ponto &p2) const;
    Ponto operator*(const Ponto &p2) const;
    Ponto operator*(const float scalar) const;

    static float dotProduct(const Ponto* p1, const Ponto* p2);
    static Ponto crossProduct(const Ponto* p1, const Ponto* p2);

    void rotacionaX(float angulo);
    void rotacionaY(float angulo);
    void rotacionaZ(float angulo);

    static bool intersec2d(Ponto k, Ponto l, Ponto m, Ponto n);
    // bool ha_intersec(Ponto k, Ponto l, Ponto m, Ponto n);
};

#endif