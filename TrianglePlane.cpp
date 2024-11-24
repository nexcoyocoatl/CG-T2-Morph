#include "TrianglePlane.h"

#include "Ponto.h"

TrianglePlane::TrianglePlane(Ponto _v0, Ponto _v1, Ponto _v2)
{    
    v0 = _v0;
    v1 = _v1;
    v2 = _v2;

    v0v1 = v1 - v0;
    Ponto v0v2 = v2 - v0;
    
    n = Ponto::crossProduct(&v0v1, &v0v2);

    // Ponto normalizedN = n/Ponto::abs(n);

    // std::cout << "n: " << n.x << "," << n.y << "," << n.z << " normalizado: " << normalizedN.x << "," << normalizedN.y << "," << normalizedN.z << "\n";

    d = -(Ponto::dotProduct(&n, &v0));

    // std::cout << "d = " << d << "\n";
}

bool TrianglePlane::b_intersectTriangle(Ponto p)
{
    Ponto center;
    float area = Ponto::fisqrt(n) * 2; // Ao invés de sqrt(n)/2, faz 1/sqrt(n)*2 porque o cálculo é muito mais rápido

    // Cálculo baricêntrico pelos 3 triângulos formados pela face triangular

    // Calcula "u" (triângulo BCP)
    Ponto v1p = p - v1;
	Ponto v1v2 = v2 - v1;
    center = Ponto::crossProduct(&v1v2, &v1p);
    // float u = (Ponto::fisqrt(center) * 2) / area; // Não é necessário para isso, apenas para outros algoritmos de avaliação de cor
    if (Ponto::dotProduct(&n, &center) < 0)
    {
        return false; // p está no lado errado
    }

    // Calcula "v" (triângulo CAP)
    Ponto v2p = p - v2;
	Ponto v2v0 = v0 - v2;
    center = Ponto::crossProduct(&v2v0, &v2p);
    // float v = (Ponto::fisqrt(center) * 2) / area;
    if (Ponto::dotProduct(&n, &center) < 0)
    {
        return false; // p está no lado errado
    }

    // Terceira aresta
    Ponto v0p = p - v0;
	// Vec3f v0v1 = v1 - v0; // Já definido anteriorment
    center = Ponto::crossProduct(&v0v1, &v0p);
    if (Ponto::dotProduct(&n, &center) < 0)
    {
        return false; // p está no lado errado
    }

    return true;
}
