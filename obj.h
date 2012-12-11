#ifndef OBJ_H
#define OBJ_H

#include "vector.h"
#include <QVector>
#include <float.h>

struct Triangle;

// A simple parser that reads and writes Wavefront .obj files
class OBJ
{
public:

    struct Index
    {
        int vertex;
        int normal;

        Index() : vertex(-1), normal(-1) {}
        Index(int vertex, int normal) : vertex(vertex), normal(normal) {}
    };

    struct Triangle
    {
        Index a, b, c;

        Triangle() {}
        Triangle(const Index &a, const Index &b, const Index &c) : a(a), b(b), c(c) {}
    };

    struct BoundingBox
    {
        Vector3 center;
        float maxX, minX, maxY, minY, maxZ, minZ;
        BoundingBox() : maxX(-FLT_MAX), minX(FLT_MAX), maxY(-FLT_MAX), minY(FLT_MAX), maxZ(-FLT_MAX), minZ(FLT_MAX) {}
        BoundingBox(Vector3 center,  float maxX,float minX,float maxY, float minY,float maxZ,float minZ) :
                center(center), maxX(maxX), minX(minX), maxY(maxY), minY(minY), maxZ(maxZ), minZ(minZ) {}
    };

    BoundingBox boundingBox;
    QVector<Vector3> vertices;
    QVector<Vector3> normals;
    QVector<Triangle> triangles;

    unsigned int m_vboBinding;

    void draw() const;
    void vboDraw() const;
    bool read(const QString &path);
    bool write(const QString &path) const;
private:
    Index getIndex(const QString &str) const;
    void initVbo();
};

#endif // OBJ_H
