#include "ShapesSphere.h"
#include "stdlib.h"
#include "math.h"
#include "qgl.h"

//represents a sphere of radius 0.5 centered at the origin
ShapesSphere::ShapesSphere()
{
}

ShapesSphere::~ShapesSphere()
{
}

float ShapesSphere::sphereX(float r, float theta, float phi) {
    return r*cos(theta)*sin(phi);
}
float ShapesSphere::sphereY(float r, float theta, float phi) {
    return r*sin(theta)*sin(phi);
}
float ShapesSphere::sphereZ(float r, float theta, float phi) {
    return r*cos(phi);
}

void ShapesSphere::vertexAndNorm(float x, float y, float z, int coord_ind) {
    float norm = sqrt(x*x + y*y + z*z);
    m_vertex[coord_ind] = new Vertex3f(x,y,z,x/norm,y/norm,z/norm);
}

void ShapesSphere::calculateVertexes(int p1, int p2) {
    int coord_ind = 0;
    int param1 = max(2, p1);
    int param2 = max(3, p2);
    freeVertexes();
    m_vertex_count = 6*param1*param2;
    m_vertex = (Vertex3f**) malloc(m_vertex_count*sizeof(Vertex3f*));
    //points sampled from phi = [0,param1)*phi_leg
    //				and theta = [0,param2)*theta_leg
    // with param1 = max(2, param1) and
    //		param2 = max(3, param2)
    float pi = atan(1)*4.0;
    float phi_leg =  pi/param1;
    float theta_leg = 2*pi/param2;
    float r = 0.5;

    for(int j=0; j<param1; j++) {
            float phi = j*phi_leg;
            for(int i=0;i<param2;i++) {
                float theta = i*theta_leg;
                //upper-left triangle
                vertexAndNorm(sphereX(r,theta,phi), sphereY(r,theta,phi), sphereZ(r,theta,phi), coord_ind++);
                vertexAndNorm(sphereX(r,theta,phi+phi_leg), sphereY(r,theta,phi+phi_leg), sphereZ(r,theta,phi+phi_leg), coord_ind++);
                vertexAndNorm(sphereX(r,theta+theta_leg,phi), sphereY(r,theta+theta_leg,phi), sphereZ(r,theta+theta_leg,phi), coord_ind++);
                //bottom-right triangle
                vertexAndNorm(sphereX(r,theta,phi+phi_leg), sphereY(r,theta,phi+phi_leg), sphereZ(r,theta,phi+phi_leg), coord_ind++);
                vertexAndNorm(sphereX(r,theta+theta_leg,phi+phi_leg), sphereY(r,theta+theta_leg,phi+phi_leg), sphereZ(r,theta+theta_leg,phi+phi_leg), coord_ind++);
                vertexAndNorm(sphereX(r,theta+theta_leg,phi), sphereY(r,theta+theta_leg,phi), sphereZ(r,theta+theta_leg,phi), coord_ind++);
            }
    }
}

void ShapesSphere::renderSelf(int param1, int param2)
{
  if (param1 != this->m_param1 || param2 != this->m_param2) {
        m_param1 = param1;
        m_param2 = param2;
        calculateVertexes(param1, param2);
  }
  drawTriangles();
}
