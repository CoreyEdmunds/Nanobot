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

#include "vector.h"

enum smoke_color {
	SM_WHITE,
	SM_LIGHTGREY,
	SM_GREY,
	SM_ORANGE,
	SM_BLACK,
	SM_BLUE,
	SM_RED,
	SM_GREEN
};

typedef struct {
	vec4fp pos;
	vec3d vel;
	int lifespan;
	double depth;
} particle_t;

void particles_render(void);
void particle_recycle(particle_t *p);
void particle_think(particle_t *p);
void particles_think(void);
void init_particles(void);
void particle_color(enum smoke_color color);

vec4f m_mult(float *m, vec4f v);
