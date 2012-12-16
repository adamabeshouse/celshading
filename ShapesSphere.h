#ifndef SHAPESSPHERE_H
#define SHAPESSPHERE_H

#include "Shape.h"
class ShapesSphere : public Shape
{
public:
    ShapesSphere();
    ~ShapesSphere();
    void renderSelf(int param1, int param2);//overrides Shape; recalculates vertexes if necessary, and renders all of them
    void calculateVertexes(int param1, int param2);//overrides Shape; calculates vertexes
    float sphereX(float r, float theta, float phi);//transforms spherical coordinates into cartesian X
    float sphereY(float r, float theta, float phi);//transforms spherical coordinates into cartesian Y
    float sphereZ(float r, float theta, float phi);//transforms spherical coordinates into cartesian Z
	void vertexAndNorm(float x, float y, float z, int coord_ind);//adds a vertex and its corresponding norm to m_vertex
};

#endif // SHAPESSPHERE_H
