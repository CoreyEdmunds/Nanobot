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


/**************************************************************************
 * draw.c/h
 *
 * This file contains all the drawing routines.  The drawing routines
 * actually draw all of nanobot's parts.  However, they are all static!
 *
 * Every piece of nanobot is loaded in to a separate display list, so the 
 * parts may only be used by executing the display lists for each part.
 *************************************************************************/

#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include "materials.h"
#include "draw.h"
#include "vector.h"
/* for drawing the animation mode */
#include "animate.h"

/* the quad is initialized elsewhere.  I guess. */
static GLUquadric *quad;

/* these functions are local to this file.  Display lists are used to
 * render things */
static void draw_shoulder(void);
static void draw_upper_arm(void);
static void draw_elbow(void);
static void draw_forearm(void);
static void draw_fingers(void);
static void draw_thumb(void);
static void draw_hand(void);
static void draw_finger(float scale);
static void draw_box(float x, float y, float z);
static void draw_camera(void);
static void draw_antenna(void);
static void draw_body(void);
static void draw_upper_leg(void);
static void draw_lower_leg(void);
static void draw_foot(void);
static void draw_toe(void);
static void draw_headlight(void);
static void draw_headlight_stick(void);
static void draw_solarpanel(void);
static void draw_solarpanel_stick(void);
static void draw_stick_joint(void);
static void draw_closed_cylinder(float dia, float l, int slices, int stacks);
static void draw_pylon(void);
static void draw_thruster(float od, float id, float len);
static void draw_turbine(float dia, float len);
static void draw_string(int x, int y, void *font, char *str);

/**************************************************************************/
/* Initialize our display lists
 **************************************************************************/
void init_display_lists(void) {
	apply_mat(MAT_CHROME);

	quad = gluNewQuadric();
	gluQuadricNormals (quad, GLU_SMOOTH);

	glNewList(DL_THRUSTER, GL_COMPILE);
		draw_thruster(0.35, 0.29, 1.7*0.35);
	glEndList();
	glNewList(DL_TURBINE, GL_COMPILE);
		draw_turbine(0.29, 1.7*0.35);
	glEndList();
	glNewList(DL_BODY, GL_COMPILE);
		draw_body();
	glEndList();
	glNewList(DL_HEADLIGHT_STICK, GL_COMPILE);
		draw_headlight_stick();
	glEndList();
	glNewList(DL_HEADLIGHT, GL_COMPILE);
		draw_headlight();
	glEndList();
	glNewList(DL_SOLARPANEL_STICK, GL_COMPILE);
		draw_solarpanel_stick();
	glEndList();
	glNewList(DL_SOLARPANEL, GL_COMPILE);
		draw_solarpanel();
	glEndList();
	glNewList(DL_CAMERA, GL_COMPILE);
		draw_camera();
	glEndList();
	glNewList(DL_SHOULDER, GL_COMPILE);
		draw_shoulder();
	glEndList();
	glNewList(DL_UPPERARM, GL_COMPILE);
		draw_upper_arm();
	glEndList();
	glNewList(DL_ELBOW, GL_COMPILE);
		draw_elbow();
	glEndList();
	glNewList(DL_FOREARM, GL_COMPILE);
		draw_forearm();
	glEndList();
	glNewList(DL_HAND, GL_COMPILE);
		draw_hand();
	glEndList();
	glNewList(DL_FINGERS, GL_COMPILE);
		draw_fingers();
	glEndList();
	glNewList(DL_THUMB, GL_COMPILE);
		draw_thumb();
	glEndList();
	glNewList(DL_UPPERLEG, GL_COMPILE);
		draw_upper_leg();
	glEndList();
	glNewList(DL_LOWERLEG, GL_COMPILE);
		draw_lower_leg();
	glEndList();
	glNewList(DL_FOOT, GL_COMPILE);
		draw_foot();
	glEndList();
	glNewList(DL_TOE, GL_COMPILE);
		draw_toe();
	glEndList();
}

/**************************************************************************/
/* Draws the panel thing - note - this is not in a display list. */
/* It subscribes to the animation state to display the animation state */
/**************************************************************************/
void draw_panel(void) {
	extern int xwidth, yheight;
	char buf[200];
	int y=15;
	char *mode;
	/* swap coordinate systems to pixel units */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, xwidth, yheight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	/* disable lights and load a color */
	glDisable(GL_LIGHTING);
	glColor3f(0.75,0.75,1);

	switch(get_animation()) {
		case ANIM_STANDBY:
			mode = "standing by";
			break;
		case ANIM_RESET:
			mode = "rebooting";
			break;
		case ANIM_FLY:
			mode = "flying";
			break;
		case ANIM_DANCE:
			mode = "dancing";
			break;
		case ANIM_WALK:
			mode = "walking";
			break;
		default:
			mode = "confused";
			break;
	}

	/* draw the panel text */
	snprintf(buf, sizeof(buf), "Nanobot is %s", mode);
	draw_string(10, y, GLUT_BITMAP_8_BY_13, buf);

	/* re-enable lights */
	glEnable(GL_LIGHTING);

	/* restore the scene's projection matrix */
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void draw_axes(void) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 0, 0);
	glEnd();
	glEnable(GL_LIGHTING);
}

static void draw_solarpanel(void) {
	int i;
	gluCylinder(quad, 0.01, 0.01, 0.05, 10, 10);
	
	glPushMatrix();
		apply_mat(MAT_OBSIDIAN);
		glTranslatef(0, 0.005, 0.10);
		for(i=0; i<12; i++) {
			draw_box(0.10, 0.005, 0.025);
			glTranslatef(0, 0, 0.037);
		}
	glPopMatrix();
	
	glTranslatef(0.0, 0, 0.25 + 0.05);
	glScalef(0.15, 0.01, 0.5);
	apply_mat(MAT_SILVER);
	glutSolidCube(1.0);
}

static void draw_solarpanel_stick(void) {
	apply_mat(MAT_CHROME);
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.01, 0.01, 0.5, 10, 10);
	glPopMatrix();

	glTranslatef(0, 0.5, 0);

	draw_stick_joint();
}

static void draw_headlight_stick(void) {
	apply_mat(MAT_CHROME);
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.01, 0.01, 0.5, 10, 10);
	glPopMatrix();
	glTranslatef(0, 0.5, 0);

	draw_stick_joint();

	gluCylinder(quad, 0.01, 0.01, 0.2, 10, 10);

	glTranslatef(0, 0, 0.2);

	draw_stick_joint();

	glPushMatrix();
		glTranslatef(-0.1, 0, 0);
		glRotatef(90, 0, 1, 0);
		gluCylinder(quad, 0.01, 0.01, 0.2, 10, 10);
	glPopMatrix();
}

/**************************************************************************/
/* Renders a string - note - not in a display list
 **************************************************************************/
static void draw_string(int x, int y, void *font, char *str) {
        glRasterPos3i(x, y, 1);
        while(*str) {
                glutBitmapCharacter(font, *str);
                str++;
        }
}

static void draw_turbine(float dia, float len) {
	int i;

	float cyl_d = dia/2.5;
	float cyl_offs = dia/8;
	float cyl_h = len/2 + cyl_offs;

	apply_mat(MAT_BRONZE);

	gluSphere(quad, dia / 3, 20, 40);
	glutSolidTorus(0.02, dia, 40, 40);
	glPushMatrix();
		glTranslatef(0, 0, -cyl_h + cyl_offs);
		gluCylinder(quad, cyl_d, cyl_d, cyl_h, 30, 30);
		gluQuadricOrientation(quad, GLU_INSIDE);
		gluDisk(quad, 0, cyl_d, 30, 30);
		gluQuadricOrientation(quad, GLU_OUTSIDE);

		glTranslatef(0, 0, cyl_h);
		gluDisk(quad, 0, cyl_d, 30, 30);
	glPopMatrix();

	float w = dia/10;
	float n = 20; 

	for(i=0; i<n; i++) {
		glRotatef(360.0/(float)n, 0, 0, 1);
		glBegin(GL_TRIANGLES);
			glNormal3f(0, 1.0/sqrt(2), 1.0/sqrt(2));
			glVertex3f(0, 0, 0);
			glVertex3f(dia, w, w);
			glVertex3f(dia, -w, -w);
			glNormal3f(0, -1.0/sqrt(2), -1.0/sqrt(2));
			glVertex3f(dia, -w, -w);
			glVertex3f(dia, w, w);
			glVertex3f(0, 0, 0);
		glEnd();
	}
		
	apply_mat(MAT_CHROME);
}

static void draw_thruster(float od, float id, float len) {
	apply_mat(MAT_CHROME);
	/* move from center to rear for the cylinders */
	glTranslatef(0, 0, -len/2);
	gluCylinder(quad, od, od, len, 40, 40);
	gluQuadricOrientation(quad, GLU_INSIDE);
	gluCylinder(quad, id, id, len, 40, 40);
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	/* draw torus at rear */
	glutSolidTorus((od-id)/2, id + (od-id)/2, 40, 40);

	/* draw the back support */
	glPushMatrix();
		apply_mat(MAT_BRONZE);
		/* move back in a bit */
		glTranslatef(0, 0, 0.015);
		glRotatef(90, 1, 0, 0);
		glPushMatrix();
			glScalef(0.1, 0.1, 2.1);
			glutSolidCube(id);
		glPopMatrix();
		glRotatef(90, 0, 1, 0);
		glPushMatrix();
			glScalef(0.1, 0.1, 2.1);
			glutSolidCube(id);
		glPopMatrix();
		apply_mat(MAT_CHROME);
	glPopMatrix();


	/* draw torus at front */
	glTranslatef(0, 0, len);
	glutSolidTorus((od-id)/2, id + (od-id)/2, 40, 40);

	/* translate to center and draw blades */
	glTranslatef(0, 0, -len/2);
}

static void draw_pylon(void) {
	apply_mat(MAT_CHROME);
	gluSphere(quad, 0.25, 20, 20);

	glPushMatrix();
		glTranslatef(0, 0, 0.15);
		glRotatef(17, 1, 0, 0);
		gluCylinder(quad, 0.15, 0.15, 0.1, 20, 20);
	glPopMatrix();
}

static void draw_closed_cylinder(float dia, float l, int slices, int stacks) {
	glPushMatrix();
		glTranslatef(0, 0, -l/2);
		gluCylinder(quad, dia, dia, l, slices, stacks);
		gluQuadricOrientation(quad, GLU_INSIDE);
		gluDisk(quad, 0, dia, slices, stacks);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		glTranslatef(0,0, l);
		gluDisk(quad, 0, dia, slices, stacks);
	glPopMatrix();
}

static void draw_stick_joint(void) {
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		draw_closed_cylinder(0.035, 0.06, 5, 5);
	glPopMatrix();
}

static void draw_headlight(void) {
	apply_mat(MAT_CHROME);
	draw_closed_cylinder(0.04, 0.2, 10 ,10);
	glPushMatrix();
		glTranslatef(0, 0, 0.1);
		apply_mat(MAT_LIGHTBULB);
		gluSphere(quad, 0.03, 10, 10);
	glPopMatrix();
}

static void draw_antenna(void) {
	apply_mat(MAT_CHROME);
	glPushMatrix();
		glScalef(3, 1, 1);
		glutSolidCube(0.035);
	glPopMatrix();

	glTranslatef(0.025, 0, 0);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(quad, 0.01, 0.01, 0.70, 10, 10);
}

static void draw_camera(void) {
	glPushMatrix();
		apply_mat(MAT_BL_PLASTIC);
		gluSphere(quad, 0.15, 20, 20);
		glTranslatef(0, 0, 0.07 + 0.05);
		gluCylinder(quad, 0.08, 0.08, 0.10, 10, 10);
		apply_mat(MAT_OBSIDIAN);
		gluSphere(quad, 0.08, 10, 10);
		apply_mat(MAT_BL_PLASTIC);
		gluQuadricOrientation(quad, GLU_INSIDE);
		glTranslatef(0, 0, 0.04);
		gluCylinder(quad, 0.045, 0.045, 0.05, 10, 10);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		glTranslatef(0, 0, 0.04);
		gluDisk(quad, 0.045, 0.08, 10, 10);
	glPopMatrix();
}

static void draw_box(float x, float y, float z) {
	glPushMatrix();
		glScalef(x, y, z);
		glutSolidCube(1.0);
	glPopMatrix();
}

static void draw_finger(float scale) {
	glPushMatrix();		
		draw_box(0.01, 0.01, 0.1 * scale);
		glTranslatef(-0.0025, 0, 0);
		gluSphere(quad, 0.010, 5, 5);
		glTranslatef(0, 0, 0.025 * scale);
		gluSphere(quad, 0.010, 5, 5);	
		glTranslatef(0, 0, 0.025 * scale);
		gluSphere(quad, 0.010, 5, 5);
	glPopMatrix();
}

static void draw_fingers(void) {
	glPushMatrix();
		glTranslatef(0.03, 0.015, 0.05);
		draw_finger(0.8);
		glTranslatef(-0.018, 0, 0);
		draw_finger(1);
		glTranslatef(-0.018, 0, 0);
		draw_finger(1);
		glTranslatef(-0.018, 0, 0);
		draw_finger(0.8);
	glPopMatrix();
}

static void draw_thumb(void) {
	glPushMatrix();
		glTranslatef(0.015, -0.015, 0.05);\
		draw_finger(0.6);
	glPopMatrix();
}

static void draw_hand(void) {
	gluSphere(quad, 0.055, 15, 15);
	glTranslatef(0, 0, 0.055);
	glTranslatef(0, 0, 0.025);
	draw_box(0.06, 0.03, 0.05);
}

static void draw_forearm(void) {
	/* move out from center of cylinder */
	glTranslatef(0, 0, 0.11);
	gluCylinder(quad, 0.05, 0.10, 0.1, 15, 15);
	glTranslatef(0, 0, 0.1);
	gluCylinder(quad, 0.10, 0.10, 0.20, 15, 15);
	glTranslatef(0, 0, 0.20);
	gluCylinder(quad, 0.10, 0.05, 0.05, 15, 15);
	glTranslatef(0, 0, 0.05);
}

static void draw_elbow(void) {
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		draw_closed_cylinder(0.11, 0.25, 15, 15);
	glPopMatrix();
}

static void draw_upper_arm(void) {
	glTranslatef(-0.125, 0, 0.025);
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		gluCylinder(quad, 0.100, 0.100, 0.25, 15, 15);
	glPopMatrix();
	glTranslatef(0.13, 0, 0.25);
	draw_box(0.22, 0.20, 0.5);
	glTranslatef(0, 0, 0.25);
}

static void draw_shoulder(void) {
	apply_mat(MAT_CHROME);
	glPushMatrix();
		draw_box(0.25, 0.25, 0.02);
		glTranslatef(0.125, 0, 0.150);
		glRotatef(90, 0, 1, 0);
		draw_closed_cylinder(0.125, 0.02, 15, 15);
		glPushMatrix();
			glTranslatef(0.065, 0, 0);
			draw_box(0.17, 0.25, 0.02);
		glPopMatrix();
		glTranslatef(0, 0, -0.25);
		draw_closed_cylinder(0.125, 0.02, 15, 15);
		glPushMatrix();
			glTranslatef(0.065, 0, 0);
			draw_box(0.17, 0.25, 0.02);
		glPopMatrix();
	glPopMatrix();
	glTranslatef(0, 0, 0.125);
}

static void draw_toe(void) {
	glPushMatrix();
		glTranslatef(0.05, 0, 0);
		gluSphere(quad, 0.03, 10, 10);
		glTranslatef(-0.1, 0, 0);
		gluSphere(quad, 0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, -0.035, 0.15);
		glRotatef(90, 0, 1, 0);
		glRotatef(10, 0, 0, 1);
		glScalef(3, 1, 1);
		draw_closed_cylinder(0.05, 0.1, 15, 15);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, -0.035, 0.08);
		draw_box(0.1, 0.1, 0.15);	
	glPopMatrix();
}

static void draw_foot(void) {
	gluSphere(quad, 0.27/2, 20, 20);
	glTranslatef(0, -0.05, 0);
	glPushMatrix();
		glTranslatef(0, -0.05, 0);
		glRotatef(90, 1, 0, 0);
		draw_closed_cylinder(0.35, 0.10, 30, 30);
	glPopMatrix();
	glTranslatef(0, -0.015, 0); /* for toe-pivot point position */
}

static void draw_lower_leg(void) {
	float width = 0.27/2;
	float depth = 0.40;
	float box_height = 0.15;
	

	
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		draw_closed_cylinder(0.06, depth, 15, 15);
	glPopMatrix();
	/* - 0.01 to make a space/crack in joint */
	glTranslatef(0, -box_height/2 - 0.007, width);
	draw_box(depth, box_height, width * 2);
	glTranslatef(0, -box_height/2, 0);
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		draw_closed_cylinder(width, depth, 30, 30);
	glPopMatrix();
	glTranslatef(0, -width, 0);	
}

static void draw_upper_leg(void) {
	float inc;
	int i;
	double angle;

	float width = 0.27/2;
	float depth = 0.40;
	float offset = 0.30;
	double startAngle = 0;
	double endAngle = M_PI;
	float box_height = 0.01;
	int sides = 40;
	vec3d p1;
	vec3d p2;
	vec3d v1;
	vec3d v2;
	vec3d n;
	
	if ( sides < 3 )
		return;

	inc = (endAngle - startAngle)/(double)sides;

	/* divide depth by two, since this function draws the semicylinder 
	 * at 0,0,0 */
	depth /= 2.0;
	
	
	glPushMatrix();
		/* rotate sideways */
		glRotatef(-90, 0, 1, 0);	

		angle = startAngle;
		/*glColor3f(1.0, 0.0, 0.0);*/
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0,0,1);
			glVertex3f(0,0,depth);
			for(i=0; i<=sides; i++) {
				glVertex3f(cos(angle)*width, sin(angle)*width, depth);
				angle += inc;
			}
		glEnd();

		angle = endAngle;
		/*glColor3f(0.0, 1.0, 0.0);*/
		glBegin(GL_TRIANGLE_FAN);	
			glNormal3f(0, 0, -1);
			glVertex3f(0, offset, -depth);
			for(i=0; i<=sides; i++) {
				glVertex3f(cos(angle)*width, sin(angle)*width + offset, -depth);
				angle -= inc;
			}
		glEnd();

		angle = endAngle;
		/*glColor3f(0.0, 0.0, 1.0);*/
		glBegin(GL_TRIANGLE_STRIP);
			for(i=0; i<=sides; i++) {
				p1.x = cos(angle)*width;
				p1.y = sin(angle)*width + offset;
				p1.z = -depth;
				p2.x = cos(angle)*width;
				p2.y = sin(angle)*width;
				p2.z = depth;
				
				/* get a vector between the two points */
				VECTSUB(v1, p1, p2);
				
				/* trig..calculate a tangent to the circle */
				v2.x = cos(-(0.5 * M_PI - angle));
				v2.y = sin(-(0.5 * M_PI - angle));
				v2.z = 0;
				
				/* use the tangent and the vector to get a normal */
				CROSSPROD(n, v2, v1);
				normalize(n);
				
				glNormal3dv((double*)&n);
				
				glVertex3d(p1.x, p1.y, p1.z);
				glVertex3d(p2.x, p2.y, p2.z);
				angle -= inc;
			}
		glEnd();
		/* debugging normals ... it turns out my math was right, and 
		 * I was feeding vectors in to a function that expects points ...
		glBegin(GL_LINES);
			for(i=0; i<=sides; i++) {
				p1.x = cos(angle)*width;
				p1.y = sin(angle)*width + offset;
				p1.z = -depth;
				p2.x = cos(angle)*width;
				p2.y = sin(angle)*width;
				p2.z = depth;
				
				VECTSUB(v1, p1, p2);
				
				v2.x = cos(-(0.5 * M_PI - angle));
				v2.y = sin(-(0.5 * M_PI - angle));
				v2.z = 0;
				
				CROSS	glTranslatef(0, -box_height, -width);PROD(n, v2, v1);
				normalize(n);
				
				glNormal3dv((double*)&n);
				
				glColor3f(1, 0, 0);
				glVertex3f(p1.x, p1.y, p1.z);
				glVertex3f(p1.x - n.x, p1.y - n.y, p1.z - n.z);
				glColor3f(0, 1, 0);
				glVertex3f(p1.x, p1.y, p1.z);
				glVertex3f(p1.x - v1.x, p1.y + v1.y, p1.z - v1.z);
				glColor3f(0, 0, 1);
				glVertex3f(p1.x, p1.y, p1.z);
				glVertex3f(p1.x - v2.x, p1.y - v2.y, p1.z - v2.z);
				glColor3f(1, 1, 1);
				angle -= inc;
			}
		glEnd();
		*/
	glPopMatrix();
	
	/* the square part of the leg */
	glBegin(GL_QUADS);
		/* wide-wall front */
		glNormal3f(0, 0, 1);
		glVertex3f(-depth,0, width);
		glVertex3f(-depth, -box_height, width);
		glVertex3f(depth, -box_height, width);
		glVertex3f(depth, offset, width);
		
		/* wide wall back */
		glNormal3f(0, 0, -1);
		glVertex3f(depth, offset, -width);
		glVertex3f(depth, -box_height, -width);
		glVertex3f(-depth, -box_height, -width);
		glVertex3f(-depth,0, -width);
		
		/* narrow tall wall right */
		glNormal3f(1, 0, 0);
		glVertex3f(depth, offset, width);
		glVertex3f(depth, -box_height, width);
		glVertex3f(depth, -box_height, -width);
		glVertex3f(depth, offset, -width);
		
		/* narrow tall wall left */
		glNormal3f(-1, 0, 0);
		glVertex3f(-depth, 0, -width);
		glVertex3f(-depth, -box_height, -width);
		glVertex3f(-depth, -box_height, width);
		glVertex3f(-depth,0, width);
		
		/* floor of the box */
		glNormal3f(0, -1, 0);
		glVertex3f(depth, -box_height, width);
		glVertex3f(-depth, -box_height, width);
		glVertex3f(-depth, -box_height, -width);
		glVertex3f(depth, -box_height, -width);
		
	glEnd();
	//-box_height/2 - offset/2
	glTranslatef(0, -box_height, -width);
		
}

static void draw_body(void) {
	float od, id;
	apply_mat(MAT_CHROME);
	/* rotate the sphere so that the poles are in the 
	 * armpits...minor wireframe visual thing */
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		gluSphere(quad, 1, 200, 200);
	glPopMatrix();
	
	od=0.83;
	id=od - 0.03;
	glPushMatrix();
		glTranslatef(0, 0.7, 0);
		glRotatef(90, 1, 0, 0);
		glutSolidTorus((od-id)/2, id + (od-id)/2, 20, 40);
		glTranslatef(0, 0, (od-id)/2);
		gluDisk(quad, (id+od)/2 - 0.25, (id+od)/2, 40, 10);
		glTranslatef(0, 0, -(od-id));
		gluQuadricOrientation(quad, GLU_INSIDE);
		gluDisk(quad, (id+od)/2 - 0.25, (id+od)/2, 40, 10);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
	glPopMatrix();

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glRotatef(-40, 1, 0, 0);
		glTranslatef(0,0,0.90);
		draw_pylon();
	glPopMatrix();
	glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		glRotatef(-40, 1, 0, 0);
		glTranslatef(0,0,0.90);
		draw_pylon();
	glPopMatrix();

	glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glTranslatef(0.9,0.50,0);
		draw_antenna();
	glPopMatrix();
	
	glPushMatrix();
		glRotatef(-50, 0, 0, 1);
		glTranslatef(1, 0, 0);
		gluSphere(quad, 0.20, 20, 20);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-180+50, 0, 0, 1);
		glTranslatef(1, 0, 0);
		gluSphere(quad, 0.20, 20, 20);
	glPopMatrix();
	
	/* the camera recepticle */
	apply_mat(MAT_SILVER);
	glPushMatrix();
		glRotatef(-25, 1, 0, 0);
		glTranslatef(0, 0, 0.95);
		gluCylinder(quad, 0.20, 0.20, 0.05, 20, 20);
		glTranslatef(0, 0, 0.05);
		gluDisk(quad, 0.10, 0.20, 20, 5);
	glPopMatrix();
	
	/* the little knob on the belly */
	glPushMatrix();
		glRotatef(-30, 0, 1, 0);
		glRotatef(10, 1, 0, 0);
		glTranslatef(0, 0, 1-0.006);
		glutSolidTorus(0.012, 0.055, 20, 20);
		draw_box(0.11, 0.024, 0.024);
		draw_box(0.024, 0.11, 0.024);
		gluSphere(quad, 0.024, 5, 5);
	glPopMatrix();
	
	/* the ring on the side */
	glPushMatrix();
		glRotatef(40, 0, 1, 0);
		glRotatef(15, 1, 0, 0);
		glTranslatef(0, 0, 1-0.025);
		glutSolidTorus(0.008, 0.2, 20, 20);
	glPopMatrix();
	
	glPushMatrix();
		apply_mat(MAT_BL_PLASTIC);
		glRotatef(40, 0, 1, 0);
		glRotatef(-25, 1, 0, 0);
		glTranslatef(0, 0, 1);
		draw_box(0.3, 0.07, 0.05);
		glTranslatef(0, 0, 0.025);
		apply_mat(MAT_REDLED);
		gluSphere(quad, 0.016, 10, 10);
		glTranslatef(-0.08, 0, 0);
		apply_mat(MAT_GREENLED);
		gluSphere(quad, 0.016, 10, 10);
		glTranslatef(0.16, 0, 0);
		apply_mat(MAT_WHITELED);
		gluSphere(quad, 0.016, 10, 10);
	glPopMatrix();
}
