#define GLEW_STATIC

//#include <GL/glew.h>
#include "obj.h"
#include <qgl.h>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "vector.h"
#include <QGLShaderProgram>
//#include "qgl.h"

#ifndef __APPLE__
extern "C" {
    GLAPI void APIENTRY glBindBuffer (GLenum target, GLuint buffer);
    GLAPI void APIENTRY glGenBuffers (GLsizei n, GLuint *buffers);
    GLAPI void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
}
#endif

bool OBJ::read(const QString &path)
{
    // Open the file
    QString obj = QString(path);

    QFile file(obj);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return false;
    QTextStream f(&file);
    QString line;

    vertices.clear();
    normals.clear();
	vertexNormals.clear();
	vertexNormalContributions.clear();
    triangles.clear();
    boundingBox = BoundingBox();

    // Read the file
    QRegExp spaces("\\s+");
    Vector3 currKA;
    Vector3 currKD;
    do {
        line = f.readLine().trimmed();
        QStringList parts = line.split(spaces);
        if (parts.isEmpty()) continue;
        if (parts[0] == "ka" && parts.count() >= 4) {
            float x = parts[1].toFloat();
            float y = parts[2].toFloat();
            float z = parts[3].toFloat();
            currKA = Vector3(x,y,z);
		} else if(parts[0] == "kd" && parts.count() >= 4) {
            float x = parts[1].toFloat();
            float y = parts[2].toFloat();
			float z = parts[3].toFloat();
            currKD = Vector3(x,y,z);
		} else if (parts[0] == "v" && parts.count() >= 4) {
            float x = parts[1].toFloat();
            float y = parts[2].toFloat();
            float z = parts[3].toFloat();

            vertices += Vertex(x,y,z,currKA,currKD);
			vertexNormals += Vector3(0,0,0);
			vertexNormalContributions += 0;
            //Bounding box info to position camera correctly
            boundingBox.maxX = max(x, boundingBox.maxX );
            boundingBox.minX = min(x, boundingBox.minX );
            boundingBox.maxY = max(y, boundingBox.maxY );
            boundingBox.minY = min(y, boundingBox.minY );
            boundingBox.maxZ = max(z, boundingBox.maxZ );
            boundingBox.minZ = min(z, boundingBox.minZ );
        } else if (parts[0] == "vn" && parts.count() >= 4) {
            normals += Vector3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
        } else if (parts[0] == "f" && parts.count() >= 4) {
            // Convert polygons into triangle fans
            Index a = getIndex(parts[1]);
            Index b = getIndex(parts[2]);
            for (int i = 3; i < parts.count(); i++) {
                Index c = getIndex(parts[i]);
                triangles += Triangle(a, b, c);
                b = c;
            }
        }
    } while (!line.isNull());

    boundingBox.center = Vector3((boundingBox.maxX + boundingBox.minX) / 2,
                                 (boundingBox.maxY + boundingBox.minY) / 2,
                                 (boundingBox.maxZ + boundingBox.minZ) / 2);
	computeNormals();
    initVbo();

    return true;
}

void OBJ::computeNormals()
{
	for(int i = 0; i < triangles.size() ; i++) {
		Triangle tri = triangles.at(i);
		Vector3 normal = this->generateNormal(vertices.at(tri.a.vertex), vertices.at(tri.b.vertex), vertices.at(tri.c.vertex));
		vertexNormals[tri.a.vertex] = (vertexNormalContributions[tri.a.vertex]*vertexNormals[tri.a.vertex] + normal)/(vertexNormalContributions[tri.a.vertex]+1);
		vertexNormalContributions[tri.a.vertex] += 1;
		vertexNormals[tri.b.vertex] = (vertexNormalContributions[tri.b.vertex]*vertexNormals[tri.b.vertex] + normal)/(vertexNormalContributions[tri.b.vertex]+1);
		vertexNormalContributions[tri.b.vertex] += 1;
		vertexNormals[tri.c.vertex] = (vertexNormalContributions[tri.c.vertex]*vertexNormals[tri.c.vertex] + normal)/(vertexNormalContributions[tri.c.vertex]+1);
		vertexNormalContributions[tri.c.vertex] += 1;
	}
}

void OBJ::draw() const
{
    glBegin(GL_TRIANGLES);
    /**
     * --------------------------------------
     *          Your code here (2.2)
     * --------------------------------------
     * You need to draw all the faces of the model
     * in here.
     *
     */
	//m_toonShader->bind();
    int size = triangles.size();
    for(int i = 0; i<size; i++){
        Triangle tri = triangles.at(i);
		glBegin(GL_TRIANGLES);
        glColor3f(vertices.at(tri.a.vertex).kd.x, vertices.at(tri.a.vertex).kd.y, vertices.at(tri.a.vertex).kd.z);
		glNormal3f(vertexNormals[tri.a.vertex].x, vertexNormals[tri.a.vertex].y, vertexNormals[tri.a.vertex].z);
		glVertex3f(vertices.at(tri.a.vertex).coord.x, vertices.at(tri.a.vertex).coord.y, vertices.at(tri.a.vertex).coord.z);
		glColor3f(vertices.at(tri.b.vertex).kd.x, vertices.at(tri.b.vertex).kd.y, vertices.at(tri.b.vertex).kd.z);
		glNormal3f(vertexNormals[tri.b.vertex].x, vertexNormals[tri.b.vertex].y, vertexNormals[tri.b.vertex].z);
		glVertex3f(vertices.at(tri.b.vertex).coord.x, vertices.at(tri.b.vertex).coord.y, vertices.at(tri.b.vertex).coord.z);
		glColor3f(vertices.at(tri.c.vertex).kd.x, vertices.at(tri.c.vertex).kd.y, vertices.at(tri.c.vertex).kd.z);
		glNormal3f(vertexNormals[tri.c.vertex].x, vertexNormals[tri.c.vertex].y, vertexNormals[tri.c.vertex].z);
		glVertex3f(vertices.at(tri.c.vertex).coord.x, vertices.at(tri.c.vertex).coord.y, vertices.at(tri.c.vertex).coord.z);
    }
	//m_toonShader->release();
    glEnd();
}


void OBJ::initVbo(){
    /**
     * --------------------------------------
     *          Your code here (3.1)
     * --------------------------------------
     * Here you are setting up the VBO and sending
     * the data to the graphics card. This is called
     * after the rest of the object has been initialized
     * so you can rightly expect that the triangles, vertices
     * and normals lists are completely filled in.
     */
	/*int buffersize = triangles.size()*3*2*3;

    glGenBuffers(1, &m_vboBinding);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBinding);
    float* data = new float[buffersize];
    for(int i =0; i<triangles.size(); i+=1){
        int index = i*18;
        Triangle tri = triangles.at(i);

		data[index]=vertices.at(tri.a.vertex).coord.x;
		data[index+1]=vertices.at(tri.a.vertex).coord.y;
		data[index+2]=vertices.at(tri.a.vertex).coord.z;

		data[index+3]=vertexNormals.at(tri.a.vertex).x;
		data[index+4]=vertexNormals.at(tri.a.vertex).y;
		data[index+5]=vertexNormals.at(tri.a.vertex).z;



		//printf("red is %f\n", vertices.at(tri.a.vertex).kd.x);
		data[index+6]=vertices.at(tri.b.vertex).coord.x;
		data[index+7]=vertices.at(tri.b.vertex).coord.y;
		data[index+8]=vertices.at(tri.b.vertex).coord.z;

		data[index+9]=vertexNormals.at(tri.b.vertex).x;
		data[index+10]=vertexNormals.at(tri.b.vertex).y;
		data[index+11]=vertexNormals.at(tri.b.vertex).z;



		data[index+12]= vertices.at(tri.c.vertex).coord.x;
		data[index+13]=vertices.at(tri.c.vertex).coord.y;
		data[index+14]=vertices.at(tri.c.vertex).coord.z;

		data[index+15]=vertexNormals.at(tri.c.vertex).x;
		data[index+16]=vertexNormals.at(tri.c.vertex).y;
		data[index+17]=vertexNormals.at(tri.c.vertex).z;


	}
    unsigned int dataSize = buffersize*(sizeof(float));
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;*/
}

void OBJ::vboDraw() const
{
    /**
     * --------------------------------------
     *          Your code here (3.2)
     * --------------------------------------
     * You need to draw all the faces of the model
     * using the vbo.
     *
     */

   /* glBindBuffer(GL_ARRAY_BUFFER, m_vboBinding);
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 6*(sizeof(float)), (void*) 0);
    glNormalPointer(GL_FLOAT, 6*(sizeof(float)), (void*) (3*sizeof(float)));
	glDrawArrays(GL_TRIANGLES,0 ,triangles.size()*3*2*3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

Vector3 OBJ::generateNormal(Vertex v1, Vertex v2, Vertex v3) const {
	/* generates a vector perpendicular to the plane of v1, v2, and v3
	   (assuming they are given in counterclockwise order)  */
	Vector3 vec1 = v2.coord - v1.coord;
	Vector3 vec2 = v3.coord - v1.coord;
	Vector3 norm = vec2.cross(vec1);
	norm.normalize();
	return norm;
}

static QString str(const Vector2 &v) { return QString("%1 %2").arg(v.x).arg(v.y); }
static QString str(const Vector3 &v) { return QString("%1 %2 %3").arg(v.x).arg(v.y).arg(v.z); }

static QString str(const OBJ::Index &i)
{
    if (i.normal >= 0) {
        return QString("%1//%2").arg(i.vertex + 1).arg(i.normal + 1);
    } else {
        return QString("%1").arg(i.vertex + 1);
    }
}

bool OBJ::write(const QString &path) const
{
    // Open the file
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
    QTextStream f(&file);

    // Write the file
    foreach (const Vertex &vertex, vertices) f << "v " << str(vertex.coord) << '\n';
    foreach (const Vector3 &normal, normals) f << "vn " << str(normal) << '\n';
    foreach (const Triangle &tri, triangles) f << "f " << str(tri.a) << ' ' << str(tri.b) << ' ' << str(tri.c) << '\n';

    return true;
}

inline int relativeIndex(int index, int count)
{
    return index >= 0 ? index - 1 : count + index;
}

OBJ::Index OBJ::getIndex(const QString &str) const
{
    QStringList parts = str.split('/');
    int vertex = parts.count() > 0 ? relativeIndex(parts[0].toInt(), vertices.count()) : -1;
	if(parts.count() == 0) {
		printf("%s\n", str.toStdString().c_str());
	}
	int normal = parts.count() > 2 ? relativeIndex(parts[2].toInt(), normals.count()) : -1;
    return Index(vertex,normal);
}
