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
 * nanobot.c
 *
 * This file contains the GLUT interface to nanobot, and is responsible for
 * initializing lights, and calling other initialization functions.
 *
 * Drawing, animation, rendering, and joint selection all take place in
 * other files.
 *************************************************************************/

#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include "materials.h"
#include "vector.h"
#include "draw.h"
#include "render.h"
#include "joint.h"
#include "animate.h"
#include "particles.h"

static void reshape(int width, int height);
static void keypress(unsigned char key, int x, int y);
static void specialkey(int key, int x, int y);
static void init(void);
static void render_scene(void);
static void mouse(int button, int state, int x, int y);
static void robot_think(int n); 
static void move(int x, int y);
static void init_menus(void);
static void menu_click(int val);

int fill = 1;
int xwidth = 0;
int yheight = 0;
static int mouse_x = 0;
static int mouse_y = 0;
int lights_on = 1;
static int particles_anim = 1;
static int particles_disp = 1;
/**************************************************************************/
/* main: all initialization and callback registration.		          */
/**************************************************************************/
int main( int argc, char *argv[] )
{
	glutInit( &argc, argv );
	glutInitWindowSize( 1024, 600 );
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow( "Nanobot - By Corey Edmunds" );
	glutDisplayFunc( render_scene );
	glutKeyboardFunc( keypress );
	glutReshapeFunc ( reshape );
	glutMouseFunc( mouse );
	glutMotionFunc( move );
	glutSpecialFunc ( specialkey );
	glutTimerFunc ( ROBOT_MS_PER_FRAME, robot_think, 1 );
	init();

	glutMainLoop();

	return( 0 );    /* NOTE: this is here only for ANSI requirements */
}

/**************************************************************************/
/* init:  initialize display modes and                                    */
/**************************************************************************/
static void init(void) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE); 
	glShadeModel(GL_SMOOTH);


	float light0[] = { 5, 5, 0, 1 };
	float diffuse[] = { 1, 1, 1, 1 };
	float specular[] = { 1, 1, 1, 1 };
	float ambient[] = { 0.2, 0.2, 0.2, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	float cutoff[] = { 30 };
	float ambient1[4] = { 0 };
	diffuse[1] = 0;
	diffuse[2] = 0;
	ambient[1] = 0;
	ambient[2] = 0;
	specular[1] = 0;
	specular[2] = 0;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 40);
	
	diffuse[0] = 0;
	diffuse[2] = 1;
	ambient[0] = 0;
	ambient[2] = 1;
	specular[0] = 0;
	specular[2] = 1;
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 40);
	
	float light3[] = { 0, 0, 0, 1 };
	float diffuse3[] = { 0, 0, 0, 0 };
	float specular3[] = { 0, 0, 0, 0 };
	float ambient3[] = {.75, .75, .75, 0 };
	glLightfv(GL_LIGHT3, GL_POSITION, light3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
	
	init_display_lists();
	init_joints();
	init_menus();
	init_particles();
}

/* handles animation submenu selections */
static void menu_animate_click(int val) {
	animate(val);
}

/* handles joint submenu selections */
static void menu_joint_click(int val) {
	if ( val >= 0 )
		joint_pick(val);
	else {
		if ( val == -1 )
			joint_select_all();
		if ( val == -2 )
			joint_select_none();
	}		
}

/* Handles main menu selections */
static void menu_click(int val) {
	keypress(val, 0, 0);
}

/* handles smoke colours */
static void menu_smoke_click(int val) {
	particle_color(val);
}

/* initializes the menus */
static void init_menus() {
	int animate_menu, joints_menu, smoke_menu;
	
	animate_menu = glutCreateMenu(menu_animate_click);
	glutAddMenuEntry("Standby", ANIM_STANDBY);
	glutAddMenuEntry("Fly", ANIM_FLY);
	glutAddMenuEntry("Dance", ANIM_DANCE);
	glutAddMenuEntry("Walk", ANIM_WALK);
	glutAddMenuEntry("Reboot", ANIM_RESET);
	
	joints_menu = glutCreateMenu(menu_joint_click);
	glutAddMenuEntry("All", -1);
	glutAddMenuEntry("None", -2);
	glutAddMenuEntry("BODY", SL_BODY);
	glutAddMenuEntry("L_THRUSTER", SL_L_THRUSTER);
	glutAddMenuEntry("R_THRUSTER", SL_R_THRUSTER); 
	glutAddMenuEntry("HEADLIGHTS", SL_HEADLIGHTS); 
	glutAddMenuEntry("CAMERA", SL_CAMERA); 
	glutAddMenuEntry("L_SOLARPANEL", SL_L_SOLARPANEL);
	glutAddMenuEntry("R_SOLARPANEL", SL_R_SOLARPANEL);
	glutAddMenuEntry("L_UPPERARM", SL_L_UPPERARM);
	glutAddMenuEntry("R_UPPERARM", SL_R_UPPERARM);
	glutAddMenuEntry("L_FOREARM", SL_L_FOREARM);
	glutAddMenuEntry("R_FOREARM", SL_R_FOREARM);
	glutAddMenuEntry("L_WRIST", SL_L_WRIST);
	glutAddMenuEntry("R_WRIST", SL_R_WRIST);
	glutAddMenuEntry("L_FINGERS", SL_L_FINGERS);
	glutAddMenuEntry("R_FINGERS", SL_R_FINGERS);
	glutAddMenuEntry("L_UPPERLEG", SL_L_UPPERLEG);
	glutAddMenuEntry("R_UPPERLEG", SL_R_UPPERLEG);
	glutAddMenuEntry("L_LOWERLEG", SL_L_LOWERLEG);
	glutAddMenuEntry("R_LOWERLEG", SL_R_LOWERLEG);
	glutAddMenuEntry("L_FOOT", SL_L_FOOT);
	glutAddMenuEntry("R_FOOT", SL_R_FOOT);
	glutAddMenuEntry("L_TOES", SL_L_TOES);
	glutAddMenuEntry("R_TOES", SL_R_TOES);
	
	smoke_menu = glutCreateMenu(menu_smoke_click);
	glutAddMenuEntry("White", SM_WHITE);
	glutAddMenuEntry("Light Grey", SM_LIGHTGREY);
	glutAddMenuEntry("Grey", SM_GREY);
	glutAddMenuEntry("Orange", SM_ORANGE);
	glutAddMenuEntry("Black", SM_BLACK);
	glutAddMenuEntry("Blue", SM_BLUE);
	glutAddMenuEntry("Red", SM_RED);
	glutAddMenuEntry("Green", SM_GREEN);
	
	glutCreateMenu(menu_click);
	glutAddSubMenu("Joints", joints_menu);
	glutAddSubMenu("Animate", animate_menu);
	glutAddSubMenu("Smoke", smoke_menu);
	glutAddMenuEntry("Switch Light (L)", 'l');
	glutAddMenuEntry("Freeze Smoke (F)", 'f');
	glutAddMenuEntry("Show Smoke (S)", 's');
	glutAddMenuEntry("Wireframe (W)", 'w');
	glutAddMenuEntry("Reset (R)", 'r');
	glutAddMenuEntry("Quit (Q)", 'q');
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}


/**************************************************************************/
/* keypress:  handle key presses                                          */
/**************************************************************************/
static void keypress(unsigned char key, int x, int y) {
	x = y = 1; /* shut up compiler */
	switch(key) {
		case 'Q':
		case 'q':
			exit(0);
		case 'w':
		case 'W':
			fill = !fill;
			break;
		case 'r':
		case 'R':
			animate(ANIM_RESET);
			break;
		case 'l':
		case 'L':
			if ( lights_on ) {
				glDisable(GL_LIGHT0);
				lights_on = 0;
			} else {
				glEnable(GL_LIGHT0);
				lights_on = 1;
			}
			break;
		case 'F':
		case 'f':
			particles_anim = !particles_anim;
			break;
		case 'S':
		case 's':
			particles_disp = !particles_disp;
	}
	glutPostRedisplay();
}

/**************************************************************************/
/* specialkey:  handle special keypresses                                 */
/**************************************************************************/
static void specialkey(int key, int x, int y) {
	x = y = 1; /* shut up compiler */
	switch(key) {
		case GLUT_KEY_LEFT:
			joint_rotate('x', SL_BODY, 5);
			break;
		case GLUT_KEY_RIGHT:
			joint_rotate('x', SL_BODY, -5);
			break;
		case GLUT_KEY_UP:
			joint_rotate('y', SL_BODY, -5);
			break;
		case GLUT_KEY_DOWN:
			joint_rotate('y', SL_BODY, 5);
			break;
	}
	glutPostRedisplay();
}

/**************************************************************************/
/* reshape:  Set the ortho and viewport                                   */
/**************************************************************************/
static void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(2,-2,-2,2,2,-2);
	gluPerspective(60, (float)width/(float)height, 1.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
	xwidth = width;
	yheight = height;
}

/* Progresses the robot animation state 1 frame */
static void robot_think(int n) {
	if ( n ) n = n; /* shut up compiler */
	glutTimerFunc ( ROBOT_MS_PER_FRAME, robot_think, 1 );
	animate_think();
	if ( particles_anim && particles_disp )
		particles_think();
	glutPostRedisplay();
}

/**************************************************************************/
/* draw:  Draw the scene                                                  */
/**************************************************************************/
static void render_scene(void) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glColor3f( 1.0, 1.0, 1.0);

        if ( fill ) {
                glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
        } else {
                glPolygonMode(GL_FRONT, GL_LINE);
		glDisable(GL_LIGHTING);
	}
	glLoadIdentity();

	gluLookAt(0,0,4,
		0,0,0,
		0,1,0);
	
	glPushMatrix();
		render_body();
	glPopMatrix();

	if ( particles_disp ) 
		particles_render();

	glLoadIdentity();
	draw_panel();
	glutSwapBuffers();
}

/* handles mouse movement -- calls joint_move to move selected joints */
static void move(int x, int y) {
	if ( mouse_x >= 0 && mouse_y >= 0 ) {
		/*printf("Rotating %d %d\n", x - mouse_x, y - mouse_y);*/
		joint_move(x - mouse_x, y - mouse_y);
		mouse_x = x;
		mouse_y = y;
	}
}

/**************************************************************************/
/* Mouse function -- pulled from the example from the lectures            */
/**************************************************************************/
static void mouse(int button, int state, int x, int y)
{
	GLuint selectBuff[64];
	GLint hits, viewport[4];
    
    	/* save the start mouse-positions for mouse-move deltas */
	if ( state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON ) {
   	 	mouse_x = x;
    		mouse_y = y;
    	} else {
    		mouse_x = -1;
    		mouse_y = -1;
    	} 
    
    	/* up-clicks, non-left clicks */
    	if ( state != GLUT_DOWN || button != GLUT_LEFT_BUTTON ) 
    		return;
    
	// 1. Def. the buffer that we are using to store infor.
	//    names of the object 
	// Select buffer parameters
	glSelectBuffer(64, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	// Enter to selection mode
	glDisable(GL_LIGHTING);

	// 2. Mod the PROJECTION MATRIX
	//    a.  Set the PROJE. as current matrix 
	glMatrixMode(GL_PROJECTION);
	//    b.  Since we need to go back to the original matrix
	glPushMatrix();
	//    c.  Erase what I had in the PROJECTION matrix
	glLoadIdentity();
	//    d.  Set-up pick matrix
	//        in the actual viewport
	//        Mouse coord. conver to go from mouse to window
	//        Aroun the point ( x, WindH - y ) with 2 pixel around
	gluPickMatrix(x, yheight-y, 2, 2, viewport);
	
	//    e.  Need to multiply with the original PROJECTION set in Reshape func.
	//        Equ. to glOrtho
	//glFrustum(-7, 7, -5, 5, 10, 200);
	gluPerspective(60, (float)xwidth/(float)yheight, 1.0, 15.0);
	
	// 3.  Start getting ready to draw:
	// 	a.  Set the current matrix to the MODELVIEW
	glMatrixMode(GL_MODELVIEW);

	//      b.  I do not want to draw in the COLOR_BUFFER_BIT
	//          I just want to render all the objects
	//          set the GL_SELECT
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	//      c.  Render all scene and fill selection buffer
	//          Call the draw function 
	render_scene();

        // 4.  From now on, send every to the COLOR_BUFFER_BIT	
	//     a.  Get the return value, contains the number of objc. inside the projection
	//         Back to normal rendering
	//         This will populate the buffer
	hits = glRenderMode(GL_RENDER);
	
	// 5.  Check the number of hits
	//     if is 0 --> nothing is hit!
	//     Check the third value of the buffer
	if(hits > 0) {
		//model_pick = selectBuff[3];
		//printf("%d hits selected %d\n", hits, selectBuff[3]);
		joint_pick(selectBuff[3]);
	}
	
	// 6.  Get ready to redraw everything in normal mode:
	//     a.  Need to leave the PROJECTION matrix in the way in was
	//     b.  Go back the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//     c.  Pop the matrix to go back to what you had.
	glPopMatrix();
	//     d.  Go back to the MODELVIEW to draw!
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
}

