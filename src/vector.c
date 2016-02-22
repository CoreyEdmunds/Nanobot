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
#include "vector.h"
#include <math.h>

/**************************************************************************/
/* normalize:  normalizes a vector                                        */
/**************************************************************************/
vec3d normalize(vec3d vec) {
	double len;

	len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	if ( len == 0 )
		len = 1;

	VECTDIV(vec, len);

	return vec;
}

/**************************************************************************/
/* normal: computes and normalizes a normal (calculated using two vectors */
/**************************************************************************/
vec3d normal(vec3d o, vec3d p1, vec3d p2) {
	vec3d a;
	vec3d b;
	vec3d n;

	VECTSUB(a, p1, o);
	VECTSUB(b, o, p2);	
	CROSSPROD(n, a, b);

	n = normalize(n);
	return n;
}
