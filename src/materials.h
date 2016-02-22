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
 * materials.c/h
 *
 * This file contains a materials library, and a method to apply a material
 * with ease.
 *************************************************************************/
#ifndef __MATERIALS_H
#define __MATERIALS_H

/* a material structure */
typedef struct {
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shinyness[1];
} material_t;

/* the available materials */
enum mat_type {
	MAT_CHROME,
	MAT_OBSIDIAN,
	MAT_BRONZE,
	MAT_BRASS,
	MAT_BL_PLASTIC,
	MAT_WHITE,
	MAT_SILVER,
	MAT_LIGHTBULB,
	MAT_REDLED,
	MAT_GREENLED,
	MAT_WHITELED,
	MAT_SMOKE
};

void apply_mat ( enum mat_type type );
#endif
