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
 * animate.c/h
 *
 * This file contains all of the animation routines.  It is called from 
 * nanobot.c, and applies joint transformations using functions from 
 * joint.c
 *************************************************************************/
#include "animate.h"
#include "math.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "joint.h"

/* the animation type that is currently running */
static enum animation animation = ANIM_STANDBY;

/* the animation sequence number - the animation loop functions are 
 * periodic, and 'subscribe' to the animation sequence.  Incremented
 * in anim_think */
int anim_seq;

/* contains the current animation state -- where the joint is moving to */
static joint_info anim_state[JOINTCOUNT];

/* the sine animation loop */
static float sineloop(float amplitude, float period, float phase) {
	return amplitude * cos((anim_seq/ROBOT_PERIOD * M_2PI + phase) * period);
}

/* the random animation 'loop' */
static float randloop(float now, float max, float min, float amplitude, float period) {
	period *= ROBOT_PERIOD;
	if ( anim_seq % (int)period != 0 )
		return now;
	//printf("now org = %f", now);
	float rndval = (((float)rand())/((float)RAND_MAX) - 0.5);
	now = now + rndval * amplitude;
	//printf(" now rnd=%f scaled rnd=%f", now, rndval * amplitude); 
	if ( now > max )
		now = max;
	else if ( now < min )
		now = min;
	//printf(" now cla=%f rndval=%f amplitude=%f\n", now, rndval, amplitude);
	return now;
}

/* the sawtooth/triangle wave animation loop */
static float sawloop(float amplitude, float period) {
	float val;
	period *= ROBOT_PERIOD;
	val = fmod(anim_seq, period)/period;
	if ( val > 0.5 ) 
		val = 1-val;
	val = val - 0.5;
	return val * amplitude * 2.0;
}

/* the standby animation sequence */
static void standby(void) {
}

/* a method that moves all of the joints towards where they should be for 
 * animation.  Returns 0 when no change occured. */
static int move_joints(float speed) {
	int i;
	int changed = 0;
	
	for(i=0; i<JOINTCOUNT; i++) {
		changed |= joint_s_place(
			i, 
			speed,
			anim_state[i].xrot,
			anim_state[i].yrot,
			anim_state[i].zrot
		);
	}

	return changed;
}

/* Re-sets the animation state */
static void clear_animations(void) {
	extern joint_info joints_base[JOINTCOUNT];
	memcpy(anim_state, joints_base, sizeof(joint_info) * JOINTCOUNT);
}

/* Starts the reset animation */
static void start_reset(void) {
	clear_animations();
}

/* Runs the reset animation */
static void reset(void) {
	int changed;
	
	changed = move_joints(0.8);
	
	if ( !changed ) 
		animation = ANIM_STANDBY;
}

/* starts the flying animation */
static void start_fly(void) {
	clear_animations();
	anim_state[SL_L_UPPERARM].xrot = -15;
	anim_state[SL_L_UPPERARM].yrot = -45;
	anim_state[SL_R_UPPERARM].xrot = -15;
	anim_state[SL_R_UPPERARM].yrot = 45;
	anim_state[SL_R_FOREARM].yrot = 0;
	anim_state[SL_R_FOREARM].yrot = 0;
	anim_state[SL_R_FINGERS].xrot = 0;
	anim_state[SL_R_FINGERS].yrot = 0;
	anim_state[SL_L_UPPERLEG].yrot = 45;
	anim_state[SL_R_UPPERLEG].yrot = 45;
	anim_state[SL_L_LOWERLEG].yrot = 90;
	anim_state[SL_R_LOWERLEG].yrot = 90;
	anim_state[SL_L_FOOT].yrot = 45;
	anim_state[SL_R_FOOT].yrot = 45;
	anim_state[SL_CAMERA].yrot = 40;
	anim_state[SL_HEADLIGHTS].yrot = 5;
	anim_state[SL_L_THRUSTER].yrot = -15;
	anim_state[SL_R_THRUSTER].yrot = -15;
	anim_state[SL_L_SOLARPANEL].xrot = 45;
	anim_state[SL_L_SOLARPANEL].yrot = -15;
	anim_state[SL_R_SOLARPANEL].xrot = -45;
	anim_state[SL_R_SOLARPANEL].yrot = 15;
}

/* Runs the flying animation */
static void fly(void) {
	/* don't override global rotations */
	anim_state[SL_BODY].xrot = X_ROT(SL_BODY);
	anim_state[SL_BODY].yrot = Y_ROT(SL_BODY);
	anim_state[SL_L_UPPERARM].xrot = -15 + sineloop(15, 3, 0);
	anim_state[SL_R_UPPERARM].xrot = -15 + sineloop(15, 3, 0);
	anim_state[SL_L_WRIST].yrot = sineloop(40, 3, 0);
	anim_state[SL_R_WRIST].yrot = sineloop(40, 3, 0);
	anim_state[SL_CAMERA].xrot = sawloop(40, 0.2)+20;
	anim_state[SL_HEADLIGHTS].xrot = sawloop(360, 1) + 180;
	anim_state[SL_L_FOOT].yrot = randloop(anim_state[SL_L_FOOT].yrot, 45, 10, 20, 0.05);
	anim_state[SL_R_FOOT].yrot = randloop(anim_state[SL_R_FOOT].yrot, 45, 10, 20, 0.05);
	anim_state[SL_L_FOOT].xrot = randloop(anim_state[SL_L_FOOT].xrot, 20, -20, 20, 0.05);
	anim_state[SL_R_FOOT].xrot = randloop(anim_state[SL_R_FOOT].xrot, 20, -20, 20, 0.05);
 	anim_state[SL_L_SOLARPANEL].yrot = randloop(anim_state[SL_L_SOLARPANEL].yrot, 0, -30, 10, 0.1);
	anim_state[SL_R_SOLARPANEL].yrot = randloop(anim_state[SL_R_SOLARPANEL].yrot, 30, 0, 10, 0.1);
	move_joints(2);
}

/* Starts the dancing animation */
static void start_dance(void) {
	clear_animations();
}

/* Runs the dancing animation */
static void dance(void) {
	/* don't override global rotations */
	anim_state[SL_BODY].xrot = X_ROT(SL_BODY);
	anim_state[SL_BODY].yrot = Y_ROT(SL_BODY);
	anim_state[SL_R_FINGERS].xrot = sineloop(45, 5, 0);
	anim_state[SL_HEADLIGHTS].yrot = sineloop(90, 5, 0)+45;
	anim_state[SL_HEADLIGHTS].xrot = sawloop(360, 3) + 180;
	anim_state[SL_CAMERA].yrot = sineloop(40, 5, 0)+40;
	anim_state[SL_L_TOES].yrot = sineloop(40, 5, 0);
	anim_state[SL_R_FOREARM].yrot = 90 + sineloop(10, 5, -M_PI/2-0.4);
	anim_state[SL_L_THRUSTER].yrot = sineloop(10, 5, 0);
	anim_state[SL_R_THRUSTER].yrot = sineloop(10, 5, 0);
	move_joints(5);
}

/* Starts the walking animation */
static void start_walk(void) {
	clear_animations();
	anim_state[SL_CAMERA].yrot = 30;
	anim_state[SL_HEADLIGHTS].yrot = 25;
	anim_state[SL_L_UPPERARM].yrot = -90;
	anim_state[SL_R_UPPERARM].yrot = 90;
	anim_state[SL_R_FOREARM].yrot = 0;
}

/* Runs the walking animation */
static void walk(void) {
	/* don't override global rotations */
	anim_state[SL_BODY].xrot = X_ROT(SL_BODY);
	anim_state[SL_BODY].yrot = Y_ROT(SL_BODY);
	anim_state[SL_L_UPPERLEG].yrot = -sineloop(22.5, 2, 0) - 22.5;
	anim_state[SL_L_LOWERLEG].yrot = sineloop(22.5, 2, 0) + 22.5;
	anim_state[SL_R_UPPERLEG].yrot = -sineloop(22.5, 2, M_PI/2) - 22.5;
	anim_state[SL_R_LOWERLEG].yrot = sineloop(22.5, 2, M_PI/2) + 22.5;
	anim_state[SL_R_TOES].yrot = sineloop(22, 2, 0) -22;
	anim_state[SL_L_TOES].yrot = sineloop(22, 2, M_PI/2) -22;
	anim_state[SL_BODY].xrot = sineloop(10, 2, 0);
	anim_state[SL_CAMERA].xrot = -sineloop(10, 2, 0);
	anim_state[SL_HEADLIGHTS].xrot = -sineloop(10, 2, 0);
	anim_state[SL_R_FINGERS].xrot = sineloop(45, 10, 0);
	anim_state[SL_L_FINGERS].xrot = sineloop(45, 10, 0);
	move_joints(2);
}

/* Progresses the animation state 1 frame */
void animate_think(void) {
	anim_seq++;
	switch(animation) {
		case ANIM_STANDBY:
			standby();
			break;
		case ANIM_RESET:
			reset();
			break;
		case ANIM_FLY:
			fly();
			break;
		case ANIM_DANCE:
			dance();
			break;
		case ANIM_WALK:
			walk();
			break;
	}
}

/* Changes the animation sequence */
void animate(enum animation anim) {
	/* stupidity check */
	switch(anim) {
		case ANIM_STANDBY:
			animation = anim;
			break;
		case ANIM_RESET:
			animation = anim;
			start_reset();
			break;
		case ANIM_FLY:
			animation = anim;
			start_fly();
			break;
		case ANIM_DANCE:
			animation = anim;
			start_dance();
			break;
		case ANIM_WALK:
			animation = anim;
			start_walk();
			break;
		default:
			printf("%s %d:  Invalid animation\n", __FILE__, __LINE__);
	}
}

/* Retrives the animation sequence */
enum animation get_animation(void) {
	return animation;
}
