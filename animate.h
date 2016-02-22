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
#define ROBOT_MS_PER_FRAME 25
#define ROBOT_FRAMES_PER_MS (1.0/ROBOT_MS_PER_FRAME)
#define ROBOT_FRAMES_PER_S (1000.0 * ROBOT_FRAMES_PER_MS)
/* 5 second robot period */
#define ROBOT_PERIOD ( 5.0 * ROBOT_FRAMES_PER_S )

enum animation {
	ANIM_STANDBY,
	ANIM_RESET,
	ANIM_FLY,
	ANIM_DANCE,
	ANIM_WALK
};

void animate_think(void);
void animate(enum animation anim);
enum animation get_animation(void);
