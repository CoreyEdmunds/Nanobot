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
#ifndef __DRAW_H
#define __DRAW_H
#include <math.h>

/* the display lists */
enum dl_parts { 
	DL_THRUSTER=1, 
	DL_TURBINE,
	DL_BODY,
	DL_HEADLIGHT_STICK,
	DL_HEADLIGHT,
	DL_SOLARPANEL_STICK,
	DL_SOLARPANEL,
	DL_CAMERA,
	DL_SHOULDER,
	DL_UPPERARM,
	DL_ELBOW,
	DL_FOREARM,
	DL_HAND,
	DL_FINGERS,
	DL_THUMB,
	DL_UPPERLEG,
	DL_LOWERLEG,
	DL_FOOT,
	DL_TOE
};

void init_display_lists(void);
void draw_panel(void);
void draw_axes(void);
#endif
