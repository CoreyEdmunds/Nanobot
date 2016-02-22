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
#ifndef __RENDER_H
#define __RENDER_H
#include "joint.h"

void set_wire(enum joint_label joint, int on);
void render_l_solar_panel(void);
void render_r_solar_panel(void);
void render_camera(void);
void render_fingers(int id);
void render_hand(int id);
void render_forearm(int id);
void render_shoulder(int id);
void render_toes(int id);
void render_foot(int id);
void render_lower_leg(int id);
void render_r_upper_leg(void);
void render_l_upper_leg(void);
void render_solar_panel(void);
void render_headlights(void);
void render_body(void);
void render_turbines(void);
void render_turbine(GLUquadric *quad, float dia, float len);
void render_thruster(GLUquadric *quad, float od, float id, float len);

#endif
