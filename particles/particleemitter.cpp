#include "particleemitter.h"
#include "common.h"
#include "ShapesSphere.h"

ParticleEmitter::ParticleEmitter(GLuint textureId, float3 color, float3 velocity,
                                 float3 force, float scale, float fuzziness, float speed,
                                 unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_textureID(textureId), m_speed(speed),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
                    m_force(force)
{
	m_velocity = float3(0,1,0);
	m_fuzziness = 10;
	m_scale = 8;
	m_textureID = textureId;
    m_particles = new Particle[maxParticles];
    resetParticles();

    for (unsigned i = 0; i < m_maxParticles; ++i)
        m_particles[i].active = false;
}

ParticleEmitter::~ParticleEmitter()
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
void ParticleEmitter::resetParticle(unsigned i)
{
    m_particles[i].pos.zero();
    m_particles[i].life = 1.0;
	m_particles[i].decay = urand(0.01, 0.15);
	m_particles[i].color.r = m_color.x;
	m_particles[i].color.g = m_color.y;
	m_particles[i].color.b = m_color.z;
	m_particles[i].force.x = urand(-m_fuzziness*0.01f, m_fuzziness*0.01f);
	m_particles[i].force.y = 1;//m_force.y + urand(-m_fuzziness*0.1f, m_fuzziness*0.1f);
	m_particles[i].force.z = urand(-m_fuzziness*0.01f, m_fuzziness*0.01f);
	m_particles[i].dir.x = m_velocity.x + urand(-m_fuzziness, m_fuzziness);
	m_particles[i].dir.y = m_velocity.y + urand(-m_fuzziness, m_fuzziness);
	m_particles[i].dir.z = m_velocity.z + urand(-m_fuzziness, m_fuzziness);


    //Continue filling out code here
}

/**
  * Resets all particles in this emitter to their initial states
  */
void ParticleEmitter::resetParticles()
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
void ParticleEmitter::updateParticles()
{
 for(unsigned i = 0; i < m_maxParticles; ++i){
  if (!m_particles[i].active) {
   m_particles[i].active = true;
   this->resetParticle(i);
  } else {
   m_particles[i].pos.x += m_particles[i].dir.x*m_speed;
   m_particles[i].pos.y += m_particles[i].dir.y*m_speed;
   m_particles[i].pos.z += m_particles[i].dir.z*m_speed;
   m_particles[i].dir.x += m_particles[i].force.x;
   m_particles[i].dir.y += m_particles[i].force.y;
   m_particles[i].dir.z += m_particles[i].force.z;
   m_particles[i].life -= m_particles[i].decay;
   if(m_particles[i].life <= 0) {
    m_particles[i].active = false;
   }
   if(m_particles[i].pos.y > 15) {
	   m_particles[i].color -= (m_particles[i].color - float3(0.612,0.512,0.412))/10;
   } else {
	   m_particles[i].color -= (m_particles[i].color - float3(0.839, 0.361, 0.169) - float3(urand(0,0.2), urand(0,0.2), urand(0,0.2)));
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
void ParticleEmitter::drawParticles()
{
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_BLEND);
 //glBindTexture(GL_TEXTURE_2D, m_textureID);
 for(unsigned i=0; i<m_maxParticles; ++i) {
  if(m_particles[i].active) {
   float x = m_particles[i].pos.x;
   float y = m_particles[i].pos.y;
   float z = m_particles[i].pos.z;


   //frontside
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glPushMatrix();
   float scalef = m_particles[i].life*m_scale;
   glLoadIdentity();

   glTranslatef(x,y,z);
   glScalef(scalef, scalef, scalef);
   glBegin(GL_TRIANGLES);
   m_sphere.renderSelf(3,3);
   glEnd();
   glPopMatrix();
   /*glVertex3f(x,y,z);
   glTexCoord2f(0,1);
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x,y+m_scale,z);
   glTexCoord2f(0,0);
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x+m_scale,y+m_scale,z);
   glTexCoord2f(1,0);
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x+m_scale,y,z);
   glTexCoord2f(1,1);


   //backside
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x+m_scale,y,z);
   glTexCoord2f(1,1);
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x+m_scale,y+m_scale,z);
   glTexCoord2f(1,0);
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x,y+m_scale,z);
   glTexCoord2f(0,0);
   glColor4f(m_particles[i].color.r, m_particles[i].color.g, m_particles[i].color.b, sqrt(m_particles[i].life));
   glVertex3f(x,y,z);
   glTexCoord2f(0,1);*/

  }
 }

    //Put your code here
}
