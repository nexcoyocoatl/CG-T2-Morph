#ifndef LINHA_H
#define LINHA_H

#include "Ponto.h"

class Linha
{
private:
    Ponto m_a;
    Ponto m_b;
    
public:
    Linha(Ponto a, Ponto b);
    Linha(float x1, float y1, float z1, float x2, float y2, float z2);
    void desenhaLinha();
};


#endif