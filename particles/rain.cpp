#include "rain.h"
#include "common.h"
#include "ShapesSphere.h"

Rain::Rain(GLuint textureId, float3 color, float3 velocity,
           float3 force, float scale, float fuzziness, float speed,
           unsigned maxParticles) :
m_maxParticles(maxParticles), m_textureID(textureId), m_speed(speed),
m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
m_force(force)
{
m_velocity = float3(0,-5,0);
m_fuzziness = 10;
m_scale = 4;
m_textureID = textureId;
m_maxParticles = 300;
m_particles = new Particle[300];
resetParticles();

for (unsigned i = 0; i < m_maxParticles; ++i)
m_particles[i].active = false;
}

Rain::~Rain()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }
}

/**
  * You need to fill this in.
  *
  * Resets the particle at the given index to its initial state. Should reset the
  * position, direction, force, color, life, and decay of the particle.
  */
void Rain::resetParticle(unsigned i)
{
    m_particles[i].pos = float3(urand(-100, 100), urand(100,200), urand(-100, 100));
    m_particles[i].life = 1.0;
    m_particles[i].decay = urand(0.01, 0.15);
	m_particles[i].color.r = 0.1;
	m_particles[i].color.g = 0.1;
	m_particles[i].color.b = 0.44;
    m_particles[i].force.x = urand(-0.05, 0.05) + urand(-m_fuzziness*0.01f, m_fuzziness*0.01f);
    m_particles[i].force.y = -0.7;//m_force.y + urand(-m_fuzziness*0.1f, m_fuzziness*0.1f);
    m_particles[i].force.z = urand(-0.05,0.05) + urand(-m_fuzziness*0.01f, m_fuzziness*0.01f);
    m_particles[i].dir.x = m_velocity.x;// + urand(-m_fuzziness, m_fuzziness);
    m_particles[i].dir.y = m_velocity.y;// + urand(-m_fuzziness, m_fuzziness);
    m_particles[i].dir.z = m_velocity.z;// + urand(-m_fuzziness, m_fuzziness);


    //Continue filling out code here
}

/**
  * Resets all particles in this emitter to their initial states
  */
void Rain::resetParticles()
{
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

/**
  * You need to fill this in.
  *
  * Performs one step of the particle simulation. Should perform all physics
  * calculations and maintain the life property of each particle.
  */


void Rain::updateParticles()
{
 for(unsigned i = 0; i < m_maxParticles; ++i){
  if (!m_particles[i].active) {
   m_particles[i].active = true;
   this->resetParticle(i);
  } else {
    m_particles[i].pos.y += m_velocity.y;
    m_particles[i].pos.x += m_velocity.x;
    m_particles[i].pos.z += m_velocity.z;
   m_particles[i].life -= m_particles[i].decay;
   if(m_particles[i].pos.y<=0) {
    m_particles[i].active = false;
   }


  }
 }
}

/**
  * You need to fill this in.
  *
  * Draws each particle as a small, texture-mapped square of side-length m_scale.
  * Each square should be in the X/Y plane at Z = the particle's position's Z-coordinate.
  */
void Rain::drawParticles()
{
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_BLEND);
   //glBindTexture(GL_TEXTURE_2D, m_textureID);
   for(unsigned i=0; i<m_maxParticles; ++i) {
   if(m_particles[i].active) {
   float x = m_particles[i].pos.x;
   float y = m_particles[i].pos.y;
   float z = m_particles[i].pos.z;


   //frontside
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, .2);
   glPushMatrix();
   float scalef = .15;
   glLoadIdentity();

   glTranslatef(x,y,z);
   glScalef(scalef, 8*scalef, scalef);
   glBegin(GL_TRIANGLES);
   m_sphere.renderSelf(4,4);
   glEnd();
   glPopMatrix();

  }
 }

    //Put your code here
}

