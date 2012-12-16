#ifndef VERTEX3F_H
#define VERTEX3F_H

class Vertex3f
{
public:
    Vertex3f(float _x, float _y, float _z, float _nx, float _ny, float _nz);
    ~Vertex3f();
    float m_x;
    float m_y;
    float m_z;
    float m_nx;
    float m_ny;
    float m_nz;
};

#endif // VERTEX3F_H
