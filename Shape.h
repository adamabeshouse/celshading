#ifndef SHAPE_H
#define SHAPE_H
#include "qgl.h"
#include "math.h"
#include "Vertex3f.h"
#define max(x,y) (((x)>(y))?(x):(y))
#define in_range(f,a,b) ((f)>=(a) && (f)<=(b))

class Shape
{
public:
    Shape();
    virtual ~Shape();
    virtual void calculateVertexes(int param1, int param2);//calculates the shape's vertexes and populates them into m_vertex
    virtual void renderSelf(int param1, int param2);//calls calculateVertexes() if params have changed, then calls drawTriangles()
    virtual void vertexAndNorm(float x, float y, float z, int coord_ind);//some shapes will use this - it is for shapes for which every vertex has a norm which is determined by some formula, so it makes sense to abstract that task out.
    Vertex3f **m_vertex;//the vertexes that represent the shape
    int m_vertex_count;//the number of vertexes last time it was calculated
	virtual void renderNormals();//a method that allows ShapesScene to delegate rendering normals, and in general to allow for recursive calling of this method for cleaner delegation/modularization
protected:
    int m_param1;//represents the value of param1 last time triangles were calculated
    int m_param2;//represents the value of param2 last time triangles were calculated
    void drawTriangles();//iterates through m_vertex and draws all of them
    void freeVertexes();//a method that deallocates all the memory associated with m_vertex
};


#endif // SHAPE_H
