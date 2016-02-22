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
#include "materials.h"
#include <GL/gl.h>

/* materials library */
static const material_t materials[] = {
	{ /* MAT_CHROME */
		.ambient = { 0.25, 0.25, 0.25, 1 },
		.diffuse = { 0.4, 0.4, 0.4, 1 },
		.specular = { 0.774597, 0.774597, 0.774597, 1 },
		.shinyness = { 128 * 0.60 }
	},
	{ /* MAT_OBSIDIAN */
		.ambient = { 0.05375, 0.05, 0.06625, 1 },
		.diffuse = { 0.18275, 0.17, 0.22525, 1 },	
		.specular = { 0.332741, 0.328634, 0.346435, 1 },	
		.shinyness = { 128 * 0.30 }
	},
	{ /* MAT_BRONZE */
		.ambient = { 0.2125, 0.1275, 0.054, 1 },
		.diffuse = { 0.714, 0.4284, 0.18144, 1 },
		.specular = { 0.393548, 0.271906, 0.166721, 1 },
		.shinyness = { 128 * 0.20 }
	},
	{ /* MAT_BRASS */
		.ambient = { 0.329412, 0.223529, 0.027451, 1 },
		.diffuse = { 0.780392, 0.568627, 0.113725, 1},
		.specular = { 0.992157,	0.941176, 0.807843, 1},
		.shinyness = { 128 * 0.21794872 }
	},
	{ /* MAT_BL_PLASTIC */
		.ambient = {0.0, 0.0, 0.0, 1 },
		.diffuse = {0.01, 0.01, 0.01, 1 },
		.specular = {0.50, 0.50, 0.50, 1 },
		.shinyness = {  128 * 0.25 }
	},
	{ /* MAT_WHITE */
		.ambient = { 1, 1, 1, 1 },
		.diffuse = { 1, 1, 1, 1 },
		.specular = { 1, 1, 1, 1 },	
		.shinyness = { 100 }
	},
	{ /* MAT_SILVER */
		.ambient = { 0.19225, 0.19225, 0.19225, 1 },
		.diffuse = { 0.50754, 0.50754, 0.50754, 1 },
		.specular = { 0.508273, 0.508273, 0.508273, 1 },
		.shinyness = { 128 * 0.4 }
	},
	{ /* MAT_LIGHTBULB */
		.ambient = { 0.05375, 0.05, 0.06625, 1 },
		.diffuse = { 0.18275, 0.17, 0.22525, 1 },	
		.specular = { 0.332741, 0.328634, 0.346435, 1 },
		.emission = { 0.9, 0.9, 1.0, 1 },
		.shinyness = { 128 * 0.30 }
	},
	{ /* MAT_REDLED */
		.ambient = { 0.05375, 0.05, 0.06625, 1 },
		.diffuse = { 0.18275, 0.17, 0.22525, 1 },	
		.specular = { 0.332741, 0.328634, 0.346435, 1 },
		.emission = { 0.7, 0, 0, 1 },
		.shinyness = { 128 * 0.30 }
	},
	{ /* MAT_GREENLED */
		.ambient = { 0.05375, 0.05, 0.06625, 1 },
		.diffuse = { 0.18275, 0.17, 0.22525, 1 },	
		.specular = { 0.332741, 0.328634, 0.346435, 1 },
		.emission = { 0, 0.7, 0, 1},
		.shinyness = { 128 * 0.30 }
	},
	{ /* MAT_WHITELED */
		.ambient = { 0.05375, 0.05, 0.06625, 1 },
		.diffuse = { 0.18275, 0.17, 0.22525, 1 },	
		.specular = { 0.332741, 0.328634, 0.346435, 1 },
		.emission = { 0.7, 0.7, 0.7, 1 },
		.shinyness = { 128 * 0.30 }
	}, /* MAT_SMOKE */
	{
		.ambient = { 0.75, 0.75, 0.75, 1 },
		.diffuse = { 1, 1, 1, 1 },
		.specular = { 0 },
		.emission = { 0 },
		.shinyness = { 0 }
	}
};


/* applies a material */
void apply_mat ( enum mat_type type ) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, materials[type].ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[type].diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, materials[type].emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materials[type].specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materials[type].shinyness);
}
