#include "Shape.h"
#include "stdio.h"

Shape::Shape() {
    m_param1 = 0;
    m_param2 = 0;
    m_vertex_count = 0;
    m_vertex = NULL;
}
Shape::~Shape() {
    freeVertexes();
}
void Shape::renderSelf(int param1, int param2) {
}

void Shape::vertexAndNorm(float x, float y, float z, int coord_ind) {
}

void Shape::calculateVertexes(int param1, int param2) {
}

void Shape::drawTriangles() {
    for(int i=0;i<m_vertex_count;i++) {
        Vertex3f *tmp = m_vertex[i];
        glNormal3f(tmp->m_nx, tmp->m_ny, tmp->m_nz);
        glVertex3f(tmp->m_x, tmp->m_y, tmp->m_z);
    }
}

void Shape::freeVertexes() {
    for (int i=0;i<m_vertex_count;i++) {
        delete m_vertex[i];
    }
    if (m_vertex != NULL) {
        free (m_vertex);
    }
}

void Shape::renderNormals() {
    for(int i=0;i<m_vertex_count;i++) {
        Vertex3f *tmp = m_vertex[i];
		float3 normal = float3(tmp->m_nx, tmp->m_ny, tmp->m_nz, 0);
		float3 pt = float3(tmp->m_x, tmp->m_y, tmp->m_z, 1);
        s->renderNormal(pt, normal);
    }
}
