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
 * vector.c/h
 *
 * Simple vector manipulations
 *************************************************************************/
#ifndef __VECTOR_H
#define __VECTOR_H

/* extract a sign from a number */
#define SIGN(x) (((x)<0)?-1:1)
/* useful */
#define M_2PI (M_PI*2.0)

/* a point structure */
typedef struct { 
	double x;
	double y;
	double z;
} vec3d; 

/* a point structure */
typedef struct { 
	float x;
	float y;
	float z;
	float w;
} vec4fp; 

typedef union {
	vec4fp p;
	float v[4];
} vec4f;


/* subtracts two vectors */
#define VECTSUB(out, p1, p2) { \
	(out).x = (p1).x - (p2).x; \
	(out).y = (p1).y - (p2).y; \
	(out).z = (p1).z - (p2).z; \
}

/* adds two vectors */
#define VECTADD(out, p1, p2) { \
	out.x = p1.x + p2.x; \
	out.y = p1.y + p2.y; \
	out.z = p1.z + p2.z; \
}

/* divides two vectors */
#define VECTDIV(out, n) {\
	out.x /= n; \
	out.y /= n; \
	out.z /= n; \
}

/* performs a cross product operation on two vectors.  Note that
   out must be a different vector than a and b. */
#define CROSSPROD(out, a, b) { \
	out.x = a.y * b.z - a.z * b.y; \
	out.y = a.z * b.x - a.x * b.z; \
	out.z = a.x * b.y - a.y * b.x; \
}

vec3d normal(vec3d o, vec3d p1, vec3d p2);
vec3d normalize(vec3d vec);
#endif
