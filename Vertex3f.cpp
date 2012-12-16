#include "Vertex3f.h"

Vertex3f::Vertex3f(float _x, float _y, float _z, float _nx, float _ny, float _nz)
{
    m_x = _x;
    m_y = _y;
    m_z = _z;
    m_nx = _nx;
    m_ny = _ny;
    m_nz = _nz;
}

Vertex3f::~Vertex3f() {};

