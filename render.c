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
 * render.c/h
 *
 * This file contains all of the routines responsible for rendering 
 * nanobot.  The routines only contain positioning (rotate/translate),
 * labelling, picking code.  Display lists are called to draw things; 
 * nothing in here is drawn directly.
 *************************************************************************/
#include <GL/gl.h>
#include <GL/glut.h>
#include "materials.h"
#include "draw.h"
#include "render.h"
#include "joint.h"

/* toggles wireframe mode on/off for an object that may be selected */
void set_wire(enum joint_label joint, int on) {
	extern int fill;
	if ( fill ) {
		if ( on ) {
			SELECTED(joint) {
				glPolygonMode(GL_FRONT, GL_LINE);
				glDisable(GL_LIGHTING);
				glColor3f(0.5, 0.75, 0.75);
			}
		} else {
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
		}
	} else {
		if ( on ) {
			SELECTED(joint) {
				glColor3f(0.5, 0.75, 0.75);
			}
		} else {
			glColor3f(1, 1, 1);
		}
	}
}

void render_l_solar_panel(void) {
	glLoadName(SL_L_SOLARPANEL);
	
	set_wire(SL_L_SOLARPANEL, 1);
	glPushMatrix();
		glRotatef(X_ROT(SL_L_SOLARPANEL), 0, 1, 0);
		glCallList(DL_SOLARPANEL_STICK);
		glRotatef(Y_ROT(SL_L_SOLARPANEL), 0, 0, 1);
		glCallList(DL_SOLARPANEL);
	glPopMatrix();
	set_wire(SL_L_SOLARPANEL, 0);
}

void render_r_solar_panel(void) {
	glLoadName(SL_R_SOLARPANEL);
	set_wire(SL_R_SOLARPANEL, 1);
	glPushMatrix();
		glRotatef(X_ROT(SL_R_SOLARPANEL), 0, 1, 0);
		glCallList(DL_SOLARPANEL_STICK);
		glRotatef(Y_ROT(SL_R_SOLARPANEL), 0, 0, 1);
		glCallList(DL_SOLARPANEL);
	glPopMatrix();
	set_wire(SL_R_SOLARPANEL, 0);
}

void render_headlights(void) {
	float position[] = { 0, 0, 0 };
	float direction[] = { 0, 0, 1 };
	
	glLoadName(SL_HEADLIGHTS);
	set_wire(SL_HEADLIGHTS, 1);
	/* yeah so the coordinates are all translated here...*/
	glRotatef(X_ROT(SL_HEADLIGHTS), 0, 1, 0);
	glCallList(DL_HEADLIGHT_STICK);
	glTranslatef(-0.1, 0, -0.025);
	/* yeah so the coordinates are all translated here...*/
	glRotatef(Y_ROT(SL_HEADLIGHTS), 1, 0, 0);
	glCallList(DL_HEADLIGHT);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glTranslatef(0.2, 0, 0);
	glCallList(DL_HEADLIGHT);
	glLightfv(GL_LIGHT2, GL_POSITION, position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);
	set_wire(SL_HEADLIGHTS, 0);
}

void render_camera(void) {
	glLoadName(SL_CAMERA);
	set_wire(SL_CAMERA, 1);
	glRotatef(X_ROT(SL_CAMERA), 0, 1, 0);
	glRotatef(Y_ROT(SL_CAMERA), 1, 0, 0);
	glCallList(DL_CAMERA);
	set_wire(SL_CAMERA, 0);
}

void render_fingers(int id) {
	glLoadName(SL_L_FINGERS + id);
	set_wire(SL_L_FINGERS + id, 1);
	glRotatef(X_ROT(SL_L_FINGERS + id)/2, 1, 0, 0);
	glCallList(DL_FINGERS);
	glRotatef(-X_ROT(SL_L_FINGERS + id), 1, 0, 0);
	glCallList(DL_THUMB);
	set_wire(SL_R_FINGERS + id, 0);
}

void render_hand(int id) {
	glLoadName(SL_L_WRIST + id);
	set_wire(SL_L_WRIST + id, 1);
	glRotatef(X_ROT(SL_L_WRIST + id), 0, 1, 0);
	glRotatef(Y_ROT(SL_L_WRIST + id), 1, 0, 0);
	glCallList(DL_HAND);
	set_wire(SL_L_WRIST + id, 0);
	render_fingers(id);
}

void render_forearm(int id) {
	glLoadName(SL_L_FOREARM + id);
	set_wire(SL_L_FOREARM + id, 1);
	glCallList(DL_ELBOW);
	glRotatef(Y_ROT(SL_L_FOREARM + id), 0, 1, 0);
	glCallList(DL_FOREARM);
	set_wire(SL_L_FOREARM + id, 0);
	render_hand(id);
}

void render_shoulder(int id) {
	glLoadName(SL_L_UPPERARM + id);
	set_wire(SL_L_UPPERARM + id, 1);
	glRotatef(Y_ROT(SL_L_UPPERARM + id), 0, 0, 1);
	glCallList(DL_SHOULDER);
	glRotatef(X_ROT(SL_L_UPPERARM + id), 1, 0, 0);
	glCallList(DL_UPPERARM);
	set_wire(SL_L_UPPERARM + id, 0);
	render_forearm(id);
}

void render_toes(int id) {
	glLoadName(SL_L_TOES+id);
	set_wire(SL_L_TOES+id, 1);
	glPushMatrix();
		glRotatef(32, 0, 1, 0);
		glTranslatef(0, 0, 0.35);
		glRotatef(Y_ROT(SL_L_TOES+id), 1, 0, 0);
		glCallList(DL_TOE);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-32, 0, 1, 0);
		glTranslatef(0, 0, 0.35);
		glRotatef(Y_ROT(SL_L_TOES+id), 1, 0, 0);
		glCallList(DL_TOE);
	glPopMatrix();
	glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, 0, 0.35);
		glRotatef(Y_ROT(SL_L_TOES+id), 1, 0, 0);
		glCallList(DL_TOE);
	glPopMatrix();
	set_wire(SL_L_TOES+id, 0);
}

void render_foot(int id) {
	glLoadName(SL_L_FOOT+id);
	set_wire(SL_L_FOOT+id, 1);
	glRotatef(Y_ROT(SL_L_FOOT+id), 1, 0, 0);
	glRotatef(X_ROT(SL_L_FOOT+id), 0, 1, 0);
	glCallList(DL_FOOT);
	set_wire(SL_L_FOOT+id, 0);
	render_toes(id);
}

void render_lower_leg(int id) {
	glLoadName(SL_L_LOWERLEG+id);
	set_wire(SL_L_LOWERLEG+id, 1);
	glRotatef(Y_ROT(SL_L_LOWERLEG+id), 1, 0, 0);
	glCallList(DL_LOWERLEG);
	set_wire(SL_L_LOWERLEG+id, 0);
	render_foot(id);
}

void render_r_upper_leg(void) {
	//glTranslatef(0.025, -0.15, 1);
	glRotatef(X_ROT(SL_R_UPPERLEG), 0, 1, 0);
	glRotatef(Y_ROT(SL_R_UPPERLEG), 1, 0, 0);
	glTranslatef(0.025, -0.30, 0);
	glLoadName(SL_R_UPPERLEG);
	set_wire(SL_R_UPPERLEG, 1);
	glCallList(DL_UPPERLEG);
	set_wire(SL_R_UPPERLEG, 0);
	render_lower_leg(1);
}

void render_l_upper_leg(void) {
	//glTranslatef(0.025, -0.15, 1);
	glRotatef(X_ROT(SL_L_UPPERLEG), 0, 1, 0);
	glRotatef(Y_ROT(SL_L_UPPERLEG), 1, 0, 0);
	glTranslatef(-0.025, -0.30, 0);
	glLoadName(SL_L_UPPERLEG);
	set_wire(SL_L_UPPERLEG, 1);
	glRotatef(180, 0, 1, 0);
	glCallList(DL_UPPERLEG);
	glRotatef(-180, 0, 1, 0);
	set_wire(SL_L_UPPERLEG, 0);
	/* the following undoes a translation that is present in the
	 * display list.  It moves the origin to the axis of the knee. */
	glTranslatef(0, 0, -0.27); 
	render_lower_leg(0);
}

void render_body(void) {
	/* the body is drawn last to make selection more usable, however,
	 * the rotations have to be applied first because everything is
	 * attached to the body... */
	glRotatef(X_ROT(SL_BODY), 0, 1, 0);
	glRotatef(Y_ROT(SL_BODY), 1, 0, 0);
	glRotatef(Z_ROT(SL_BODY), 0, 0, 1);
	
	render_turbines();

	glPushMatrix();
		glTranslatef(0,0.75,0.50);
		render_headlights();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.2,0.75,-0.30);
		glRotatef(90, 0, 1, 0);
		render_l_solar_panel();
		glRotatef(-90, 0, 1, 0);
		glTranslatef(-0.4, 0, 0);
		glRotatef(-90, 0, 1, 0);
		render_r_solar_panel();
	glPopMatrix();

	glPushMatrix();
		glRotatef(-25, 1, 0, 0);
		glTranslatef(0, 0, 1);
		render_camera();
	glPopMatrix();
		
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, 0, 1.0);
		render_shoulder(0);
	glPopMatrix();	

	glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, 0, 1.0);
		render_shoulder(1);
	glPopMatrix();	

	glPushMatrix();
		glRotatef(-50, 0, 0, 1);
		glTranslatef(1.0, 0, 0);
		glRotatef(50, 0, 0, 1);
		render_r_upper_leg();
	glPopMatrix();
	
	glPushMatrix();
		glRotatef(-180+50, 0, 0, 1);
		glTranslatef(1.0, 0, 0);
		glRotatef(180-50, 0, 0, 1);
		render_l_upper_leg();
	glPopMatrix();

	glLoadName(SL_BODY);
	set_wire(SL_BODY, 1);
	glCallList(DL_BODY);	
	set_wire(SL_BODY, 0);
	
	glLoadName(-12315435);
}

/* this is gross.  Renders the turbines so they spin about an axis that runs
   through one of the pylons.  The translations mimic the pylon positioning. */
void render_turbines(void) {
	extern int anim_seq;
	glLoadName(SL_L_THRUSTER);
	set_wire(SL_L_THRUSTER, 1);
	glPushMatrix();
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
		glCallList(DL_THRUSTER);
		glRotatef(anim_seq * 4, 0, 0, 1);
		glCallList(DL_TURBINE);
	glPopMatrix();
	set_wire(SL_L_THRUSTER, 0);
	glLoadName(SL_R_THRUSTER);
	set_wire(SL_R_THRUSTER, 1);
	glPushMatrix();
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
		glCallList(DL_THRUSTER);
		glRotatef(anim_seq * 4, 0, 0, 1);
		glCallList(DL_TURBINE);
	glPopMatrix();
	set_wire(SL_R_THRUSTER, 0);
}
