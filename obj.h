#ifndef OBJ_H
#define OBJ_H

#include "vector.h"
#include <QVector>
#include <float.h>
#include <QGLShaderProgram>
#include "resourceloader.h"

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

    struct Vertex
    {
        Vector3 coord;
        Vector3 ka;
       // Vector3 ks;
        Vector3 kd;
        Vertex() {
            coord = Vector3();
            ka = Vector3();
            kd = Vector3();
        }

        Vertex(float x, float y, float z, Vector3 _ka, Vector3 _kd) {
            coord = Vector3(x,y,z);
            ka = Vector3(_ka.x, _ka.y, _ka.z);
            kd = Vector3(_kd.x, _kd.y, _kd.z);
        }
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
    QVector<Vertex> vertices;
    QVector<Vector3> normals;
	QVector<Vector3> vertexNormals; //normals which directly correspond with a vertex
	QVector<int> vertexNormalContributions; //keeps track of how many vectors went into the average
    QVector<Triangle> triangles;
	QGLShaderProgram * m_toonShader;
    unsigned int m_vboBinding;

    void draw() const;
    void vboDraw() const;
    bool read(const QString &path);
    bool write(const QString &path) const;
	Vector3 generateNormal(Vertex a, Vertex b, Vertex c) const;
	void computeNormals();
private:
    Index getIndex(const QString &str) const;
    void initVbo();
};

#endif // OBJ_H
