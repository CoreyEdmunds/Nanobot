/************************************************************************** 
 * Nanobot:  A simple, interactive robot rendered using OpenGL
 * Copyright (C) 2007, Corey Edmunds (corey.edmunds@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *************************************************************************/

#include "particles.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "joint.h"
#include "draw.h"	
#include "materials.h"
#ifdef NO_SMOKELIGHT
	#define PARTICLE_COUNT 512
#else
	#define PARTICLE_COUNT 4000
#endif
#define PARTICLE_WIDTH 0.24
#define MAX_LIFESPAN 200


static double particle_depth(particle_t *p);
static int particle_compar(const void *vp1, const void *vp2);
static void sort_particles(void);


static particle_t particles[PARTICLE_COUNT];

static enum smoke_color sm_color = SM_LIGHTGREY;

static float colors[8][3] = {
	{1, 1, 1}, 	/*SM_WHITE*/
	{0.85, 0.85, 0.85},/*SM_LIGHTGREY*/
	{0.5, 0.5, 0.5},/*SM_GREY*/
	{1, 0.65, 0 }, 	/*SM_ORANGE */
	{0, 0, 0},	/*SM_BLACK*/
	{0, 0, 1},	/*SM_BLUE*/
	{1, 0, 0},	/*SM_RED*/
	{0, 1, 0}	/*SM_GREEN*/
};

/* the modelview matrix for 1 nacelle */
static float matrix1[16];
/* the modelview matrix for 2nd nacelle */
static float matrix2[16];
/* the look-at matrix, used for depth sorting */
static float lookat[16];

/* Positions and directions of the nacelles (particle sources) */
static vec4f p1;
static vec4f v1;
static vec4f p2;
static vec4f v2;

/* The smoke texture */
static int texid;

static void load_matrixes(float *matrix, int left) {
	glPushMatrix();
		glLoadIdentity();	
		glRotatef(X_ROT(SL_BODY), 0, 1, 0);
		glRotatef(Y_ROT(SL_BODY), 1, 0, 0);
		glRotatef(Z_ROT(SL_BODY), 0, 0, 1);
		if ( left ) {
			/* translate out to pylon sphere position */
			glRotatef(90, 0, 1, 0);
			glRotatef(-40, 1, 0, 0);
			glTranslatef(0, 0, 0.90);
			/* rotate to the thruster attachment angle */
			glRotatef(10, 1, 0, 0);
			/* translate thruster out */
			glTranslatef(0, 0, 0.55);
			/* apply animation/rotation to thruster */
			glRotatef(Y_ROT(SL_L_THRUSTER), 0, 0, 1);
			/* unrotate the thruster so the back end looks straight */
			glRotatef(30, 1, 0, 0);
			/* flip thruster around so it points the right way */
			glRotatef(180, 0, 0, 1);
			/* flip the thruster 90 degrees so it points the right way */
			glRotatef(90, 0, 1, 0);
		} else {
			/* translate out to pylon sphere position */
			glRotatef(-90, 0, 1, 0);
			glRotatef(-40, 1, 0, 0);
			glTranslatef(0, 0, 0.90);
			/* rotate to the thruster attachment angle */
			glRotatef(10, 1, 0, 0);
			/* translate thruster out */
			glTranslatef(0, 0, 0.55);
			/* apply animation/rotation to thruster */
			glRotatef(-Y_ROT(SL_R_THRUSTER), 0, 0, 1);
			/* unrotate the thruster so the back end looks straight */
			glRotatef(30, 1, 0, 0);
			/* flip the thruster 90 degrees so it points the right way */
			glRotatef(90, 0, 1, 0);
		}
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
}

vec4f m_mult(float *m, vec4f v) {
	vec4f r;
	int i;
	int j;

	for(i=0; i<4; i++) {
		r.v[i] = 0;
		for(j=0; j<4; j++) 
			r.v[i] += m[j*4+i] * v.v[j];
	}	

	return r;
}

static double particle_depth(particle_t *p) {
	vec4f pos = m_mult(lookat, (vec4f)p->pos);
	//double d = sqrt(p->pos.x * p->pos.x + p->pos.y * p->pos.y + (p->pos.z - 4.0)*(p->pos.z - 4.0)); 
	return pos.p.z;
}

static int particle_compar(const void *vp1, const void *vp2) {
	particle_t *pp1 = (particle_t*)vp1;
	particle_t *pp2 = (particle_t*)vp2;

	return pp2->depth < pp1->depth;
}

static void sort_particles(void) {
	qsort(particles, PARTICLE_COUNT, sizeof(particle_t), particle_compar);
}

void particles_render(void) {
	extern int lights_on;
	int i;
	float diffuse[4] = { 1, 1, 1, 0 };
	float mag;
	sort_particles();

#ifdef NO_SMOKELIGHT	
	glDisable(GL_LIGHTING);
#else
	if ( lights_on ) {
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT3);
	}
	apply_mat(MAT_SMOKE);
#endif
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);

	for(i=0; i<PARTICLE_COUNT; i++) {
		if ( particles[i].lifespan <= 0 )
			continue;
#ifndef NO_SMOKELIGHT
			mag = sqrt(
				particles[i].pos.x*particles[i].pos.x + 
				particles[i].pos.y*particles[i].pos.y + 
				particles[i].pos.z*particles[i].pos.z);
			mag /= 2;
			glNormal3f(
				-particles[i].pos.x/mag,
				-particles[i].pos.y/mag,
				-particles[i].pos.z/mag
			);
		
			diffuse[0] = colors[sm_color][0]; 
			diffuse[1] = colors[sm_color][1]; 
			diffuse[2] = colors[sm_color][2];
			diffuse[3] = 0.5 * (float)particles[i].lifespan / ((float)MAX_LIFESPAN);
			glMaterialfv(GL_FRONT, GL_AMBIENT, diffuse);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
#else
			glColor4f(
				colors[sm_color][0], 
				colors[sm_color][1], 
				colors[sm_color][2], 
				0.5 * (float)particles[i].lifespan / ((float)MAX_LIFESPAN)
			);
#endif
			glTexCoord2f(0,1);
			glVertex3f(
				particles[i].pos.x - PARTICLE_WIDTH, 
				particles[i].pos.y + PARTICLE_WIDTH, 
				particles[i].pos.z
			);
			glTexCoord2f(0,0);
			glVertex3f(
				particles[i].pos.x -PARTICLE_WIDTH, 
				particles[i].pos.y - PARTICLE_WIDTH, 
				particles[i].pos.z
			);
			glTexCoord2f(1,0);
			glVertex3f(
				particles[i].pos.x + PARTICLE_WIDTH, 
				particles[i].pos.y - PARTICLE_WIDTH, 
				particles[i].pos.z
			);
			glTexCoord2f(1,1);
			glVertex3f(
				particles[i].pos.x + PARTICLE_WIDTH, 
				particles[i].pos.y + PARTICLE_WIDTH, 
				particles[i].pos.z
			);
	}
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if ( lights_on ) {
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT3);
	}
}

void init_particles(void) {
	unsigned char tex[32][32][1];
	int i, j;
	
	gluLookAt(0,0,4,
		0,0,0,
		0,1,0);
		
	glGetFloatv(GL_MODELVIEW_MATRIX, lookat);
	
	for ( i = 0; i<32; i++ ) 
		for( j=0; j<32; j++ ) {
			int opacity = 255 - 255 * sqrt((i-16)*(i-16) + (j-16)*(j-16))/16.0;
			if ( opacity < 0 ) 
				opacity = 0;
			if ( opacity > 255 )
				opacity = 255;
			tex[i][j][0] = opacity;
		}
	glEnable(GL_TEXTURE_2D);
	texid = 0;
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 32, 32, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	glDisable(GL_TEXTURE_2D);
	memset(particles, 0, sizeof(particles));
}

void particle_recycle(particle_t *p) {
	vec4f pos;
	vec4f vel;
	
	if ( rand() > RAND_MAX/2 ) {
		pos = p1;
		vel = v1;
	} else {
		pos = p2;
		vel = v2;
	}
	
	p->pos.x = pos.p.x + ((float)rand())/((float)RAND_MAX) * 0.30 - 0.15;
	p->pos.y = pos.p.y + ((float)rand())/((float)RAND_MAX) * 0.30 - 0.15;
	p->pos.z = pos.p.z;
	p->vel.x = vel.p.x/10.0 + ((float)rand())/((float)RAND_MAX) * 0.01 - 0.005;
	p->vel.y = vel.p.y/10.0 + ((float)rand())/((float)RAND_MAX) * 0.01 - 0.005;
	p->vel.z = vel.p.z/10.0 + ((float)rand())/((float)RAND_MAX) * 0.01 - 0.005;
	p->lifespan = rand()%(MAX_LIFESPAN/3) + MAX_LIFESPAN/3;
}

void particle_think(particle_t *p) {
	p->vel.y += 0.001;
	p->pos.x += p->vel.x;
	p->pos.y += p->vel.y;
	p->pos.z += p->vel.z;
	
	if ( p->pos.y < -1.5 ) {
		p->vel.y *= -0.1;
		p->vel.x += 2 * (((float)rand())/((float)RAND_MAX) * p->vel.y - 0.5 * p->vel.y);
		p->vel.z += 2 * (((float)rand())/((float)RAND_MAX) * p->vel.y - 0.5 * p->vel.y);
		p->vel = normalize(p->vel);
		p->vel.x /= 10.0;
		p->vel.y /= 10.0;
		p->vel.z /= 10.0;
	
	}
	
	p->depth = particle_depth(p);
	p->lifespan--;
}

void particle_color(enum smoke_color color) {
	if ( color > sizeof(colors)/3 )
		return;
	sm_color = color;
}

void particles_think(void) {
	vec4f tmp;
	load_matrixes(matrix1, 0);
	load_matrixes(matrix2, 1);

	tmp.p.x = 0;
	tmp.p.y = 0;
	tmp.p.z = 0;
	tmp.p.w = 1;
	
	p1 = m_mult(matrix1, tmp);
	p2 = m_mult(matrix2, tmp);
	tmp.p.z = -1;
	v1 = m_mult(matrix1, tmp);
	v2 = m_mult(matrix2, tmp);
	VECTSUB(v1.p, v1.p, p1.p);
	VECTSUB(v2.p, v2.p, p2.p);
	
	int i;
	for(i=0; i<PARTICLE_COUNT; i++) {
		if ( particles[i].lifespan <= 0 ) 
			particle_recycle(&particles[i]);
		particle_think(&particles[i]);
	}
}
