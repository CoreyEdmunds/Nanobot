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
 * joint.c/h
 *
 * This file contains the nanobot's joints, and all of the joint 
 * manipulation functions.
 *************************************************************************/
#ifndef __JOINT_H
#define __JOINT_H

#define JOINTCOUNT 23
/* the joint-parts */
enum joint_label { 
	SL_BODY,
	SL_L_THRUSTER, 
	SL_R_THRUSTER, 
	SL_HEADLIGHTS, 
	SL_CAMERA, 
	SL_L_SOLARPANEL,
	SL_R_SOLARPANEL,
	/* note - arm/leg drawing procedures depend on all
	 * the right enumeration values following the left */
	SL_L_UPPERARM,
	SL_R_UPPERARM,
	SL_L_FOREARM,
	SL_R_FOREARM,
	SL_L_WRIST,
	SL_R_WRIST,
	SL_L_FINGERS,
	SL_R_FINGERS,
	SL_L_UPPERLEG,
	SL_R_UPPERLEG,
	SL_L_LOWERLEG,
	SL_R_LOWERLEG,
	SL_L_FOOT,
	SL_R_FOOT,
	SL_L_TOES,
	SL_R_TOES
};

/* the joint-configuration (angles and limits) */
typedef struct {
	float xrot, xr_min, xr_max;
	float yrot, yr_min, yr_max;
	float zrot, zr_min, zr_max;

	int selected;
} joint_info;

void pick_joint(enum joint_label joint);
int joint_selected_(enum joint_label joint);
float joint_rotation(int axis, enum joint_label joint);
void joint_select_all(void);
void joint_select_none(void);
void init_joints(void);
void joint_pick(enum joint_label joint);
void joint_move(int x, int y);
void joint_rotate(int axis, enum joint_label joint, int rotation);
int joint_xs_place(enum joint_label j, float s, float x);
int joint_ys_place(enum joint_label j, float s, float y);
int joint_zs_place(enum joint_label j, float s, float z);
int joint_s_place(enum joint_label j, float s, float x, float y, float z);

/*#define joint_selected(j) (joints[j].selected)*/
#define joint_selected(j) joint_selected_(j)
#define SELECTED(j) if(joint_selected(j))
#define Y_ROT(j) (joint_rotation('y', j))
#define X_ROT(j) (joint_rotation('x', j))
#define Z_ROT(j) (joint_rotation('z', j))
#endif
