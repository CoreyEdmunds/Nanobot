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
 * This file contains the nanobot's joints, and all of the joint 
 * manipulation functions.
 *************************************************************************/
#include "joint.h"
#include "vector.h"
#include <string.h>
#include <math.h>
#include <limits.h>

/* nanobot's initial constraints and angles */ 
const joint_info joints_base[JOINTCOUNT] = {
	{ /* SL_BODY */
		.xrot = 0,
		.xr_min = INT_MIN, 
		.xr_max = INT_MAX,
		.yrot = 0,
		.yr_min = INT_MIN, 
		.yr_max = INT_MAX,
		.zrot = 0,
		.zr_min = -10,
		.zr_max = 10,
		.selected = 0 
	},
	{ /* SL_L_THRUSTER */
		.xrot = 0,
		.xr_min = 0, 
		.xr_max = 0,
		.yrot = 0,
		.yr_min = -180,
		.yr_max = 180,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0 
	},
	{ /* SL_L_THRUSTER */
		.xrot = 0,
		.xr_min = 0,
		.xr_max = 0,
		.yrot = 0,
		.yr_min = -180,
		.yr_max = 180,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0 
	},
	{ /* SL_HEADLIGHTS */
		.xrot = 0,
		.xr_min = -180,
		.xr_max = 180,
		.yrot = 0,
		.yr_min = -180,
		.yr_max = 180,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_CAMERA */
		.xrot = 0,
		.xr_min = -40,
		.xr_max = 40,
		.yrot = 0,
		.yr_min = -40,
		.yr_max = 40,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_SOLARPANEL */
		.xrot = 0,
		.xr_min = -180,
		.xr_max = 180,
		.yrot = 0,
		.yr_min = -45,
		.yr_max = 45,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},	
	{ /* SL_R_SOLARPANEL */
		.xrot = 0,
		.xr_min = -180,
		.xr_max = 180,
		.yrot = 0,
		.yr_min = -45,
		.yr_max = 45,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_UPPERARM */
		.xrot = 90,
		.xr_min = -90,
		.xr_max = 90,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_UPPERARM */
		.xrot = 90,
		.xr_min = -90,
		.xr_max = 90,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_FOREARM */
		.xrot = 0,
		.xr_min = -90,
		.xr_max = 90,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_FOREARM */
		.xrot = 0,
		.xr_min = -90,
		.xr_max = 90,
		.yrot = 90,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_WRIST */
		.xrot = 0,
		.xr_min = -90,
		.xr_max = 90,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_WRIST */
		.xrot = 0,
		.xr_min = -90,
		.xr_max = 90,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_FINGERS */
		.xrot = 0,
		.xr_min = -90,
		.xr_max = 20,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_FINGERS */
		.xrot = -90,
		.xr_min = -90,
		.xr_max = 20,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_UPPERLEG */
		.xrot = 0,
		.xr_min = -10,
		.xr_max = 10,
		.yrot = 0,
		.yr_min = -45,
		.yr_max = 45,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_UPPERLEG */
		.xrot = 0,
		.xr_min = -10,
		.xr_max = 10,
		.yrot = 0,
		.yr_min = -45,
		.yr_max = 45,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_LOWERLEG */
		.xrot = 0,
		.xr_min = 0,
		.xr_max = 0,
		.yrot = 0,
		.yr_min = 0,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_LOWERLEG */
		.xrot = 0,
		.xr_min = 0,
		.xr_max = 0,
		.yrot = 0,
		.yr_min = 0,
		.yr_max = 90,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_FOOT */
		.xrot = 0,
		.xr_min = -35,
		.xr_max = 35,
		.yrot = 0,
		.yr_min = -35,
		.yr_max = 35,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_FOOT */
		.xrot = 0,
		.xr_min = -35,
		.xr_max = 35,
		.yrot = 0,
		.yr_min = -35,
		.yr_max = 35,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_L_TOES */
		.xrot = 0,
		.xr_min = 0,
		.xr_max = 0,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 0,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	},
	{ /* SL_R_TOES */
		.xrot = 0,
		.xr_min = 0,
		.xr_max = 0,
		.yrot = 0,
		.yr_min = -90,
		.yr_max = 0,
		.zrot = 0,
		.zr_min = 0,
		.zr_max = 0,
		.selected = 0
	}
};

/* the current joint configuration */
joint_info joints[JOINTCOUNT];

/* initializes the joints */
void init_joints() {
	memcpy(joints, joints_base, sizeof(joints));
}

/* selects all joints */
void joint_select_all() {
	unsigned int i;
	
	for ( i = 0; i < sizeof(joints)/sizeof(joint_info); i++) {
		joints[i].selected = 1;
	}
}

/* deselects all joints */
void joint_select_none() {
	unsigned int i;
	
	for ( i = 0; i < sizeof(joints)/sizeof(joint_info); i++) {
		joints[i].selected = 0;
	}
}

/* Used to get a joint's rotation about an axis (with macro mumbo jumbo) */
float joint_rotation(int axis, enum joint_label joint) {
	if ( joint >= JOINTCOUNT)
		return 0;
	if ( axis == 'y' )	
		return joints[joint].yrot;
	if ( axis == 'x' )
		return joints[joint].xrot;
	if ( axis == 'z' )
		return joints[joint].zrot;
	return 0;
}

/* Used to rotate a joint with the keyboard */
void joint_rotate(int axis, enum joint_label joint, int rotation) {
	if ( joint >= JOINTCOUNT)
		return;
	if ( axis == 'y' ) {
		joints[joint].yrot += rotation;
		joints[joint].yrot = fmod(joints[joint].yrot, 360);
	} else if ( axis == 'x' ) {
		joints[joint].xrot += rotation;
		joints[joint].xrot = fmod(joints[joint].xrot, 360);
	} else if ( axis == 'z' ) {
		joints[joint].zrot += rotation;
		joints[joint].zrot = fmod(joints[joint].zrot, 360);
	}
	return;
}

/* Used by animation to smoothly place a joint in to position */
int joint_xs_place(enum joint_label j, float s, float x) {
	float o = joints[j].xrot;
	
	if ( fabs(joints[j].xrot - x) > s ) 
		joints[j].xrot -= s * SIGN(joints[j].xrot - x);
	else 
		joints[j].xrot = x;

	
	if ( joints[j].xrot > joints[j].xr_max )
		joints[j].xrot = joints[j].xr_max;
	if ( joints[j].xrot < joints[j].xr_min )
		joints[j].xrot = joints[j].xr_min;
		
	return o != joints[j].xrot;
}

/* Used by animation to smoothly place a joint in to position */
int joint_ys_place(enum joint_label j, float s, float y) {
	float o = joints[j].yrot;
	
	if ( fabs(joints[j].yrot - y) > s ) 
		joints[j].yrot -= s * SIGN(joints[j].yrot - y);
	else 
		joints[j].yrot = y;
	
	if ( joints[j].yrot > joints[j].yr_max )
		joints[j].yrot = joints[j].yr_max;
	if ( joints[j].yrot < joints[j].yr_min )
		joints[j].yrot = joints[j].yr_min;
		
	return o != joints[j].yrot;
}

/* Used by animation to smoothly place a joint in to position */
int joint_zs_place(enum joint_label j, float s, float z) {
	float o = joints[j].zrot; 
	
	if ( fabs(joints[j].zrot - z) > s ) 
		joints[j].zrot -= s * SIGN(joints[j].zrot - z);
	else 
		joints[j].zrot = z;
	
	if ( joints[j].zrot < joints[j].zr_min )
		joints[j].zrot = joints[j].zr_min;
	if ( joints[j].zrot > joints[j].zr_max )
		joints[j].zrot = joints[j].zr_max;
	return o != joints[j].zrot;
}


/* Used by animation to smoothly place a joint in to position */
int joint_s_place(enum joint_label j, float s, float x, float y, float z) {
	int changed = 0;
	
	changed |= joint_xs_place(j, s, x);
	changed |= joint_ys_place(j, s, y);
	changed |= joint_zs_place(j, s, z);
	
	return changed;
}

/* Used for mouse manipulation of all the selected joints */
void joint_move(int x, int y) {
	unsigned int i;
	
	for ( i = 0; i < sizeof(joints)/sizeof(joint_info); i++) {
		if ( joints[i].selected ) {
			joints[i].xrot += x;
			joints[i].yrot += y;
			if ( joints[i].xrot > joints[i].xr_max )
				joints[i].xrot = joints[i].xr_max;
			if ( joints[i].xrot < joints[i].xr_min )
				joints[i].xrot = joints[i].xr_min;
			if ( joints[i].yrot > joints[i].yr_max )
				joints[i].yrot = joints[i].yr_max;
			if ( joints[i].yrot < joints[i].yr_min )
				joints[i].yrot = joints[i].yr_min;
			/* make them roll at 360 degrees */
			joints[i].xrot = fmod(joints[i].xrot, 360);
			joints[i].yrot = fmod(joints[i].yrot, 360);
			joints[i].zrot = fmod(joints[i].zrot, 360);
		}
	}
}

/* Used in some macro mumbo jumbo to do a check with an array access */
int joint_selected_(enum joint_label joint) {
	if ( joint >= JOINTCOUNT)
		return 0;
	return joints[joint].selected;
}

/* Used to select a joint on mouse click */
void joint_pick(enum joint_label joint) {
	if ( joint >= JOINTCOUNT)
		return;
	
	joints[joint].selected = !joints[joint].selected;
}
