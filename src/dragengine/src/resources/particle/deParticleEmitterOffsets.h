/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEPARTICLEEMITTEROFFSETS_H_
#define _DEPARTICLEEMITTEROFFSETS_H_

/**
 * \brief Particle Emitter Offsets.
 * Offset and structure size definitions for use by modules to index into the
 * deParticleEmitter::sParticle struct. These defines respect alignment
 * properties of the target platform and compiler used to compile the
 * dragengine libraries.
 */
#define DEPE_SPARTICLE_SIZE							40

#define DEPE_SPARTICLE_OFFSET_LIFETIME				0
#define DEPE_SPARTICLE_OFFSET_POSITION_X			4
#define DEPE_SPARTICLE_OFFSET_POSITION_Y			8
#define DEPE_SPARTICLE_OFFSET_POSITION_Z			12
#define DEPE_SPARTICLE_OFFSET_LINEAR_VELOCITY		16
#define DEPE_SPARTICLE_OFFSET_ANGULAR_VELOCITY		20
#define DEPE_SPARTICLE_OFFSET_CAST_SIZE				24
#define DEPE_SPARTICLE_OFFSET_CAST_EMISSIVITY		28

#define DEPE_SPARTICLE_OFFSET_LINEAR_DIRECTION_X	32
#define DEPE_SPARTICLE_OFFSET_LINEAR_DIRECTION_Y	33
#define DEPE_SPARTICLE_OFFSET_LINEAR_DIRECTION_Z	34
#define DEPE_SPARTICLE_OFFSET_ROTATION				35
#define DEPE_SPARTICLE_OFFSET_CAST_RED				36
#define DEPE_SPARTICLE_OFFSET_CAST_GREEN			37
#define DEPE_SPARTICLE_OFFSET_CAST_BLUE				38
#define DEPE_SPARTICLE_OFFSET_CAST_TRANSPARENCY		39

#define DEPE_SPARTICLE_BLOCK1_OFFSET				0
#define DEPE_SPARTICLE_BLOCK1_SIZE					16
#define DEPE_SPARTICLE_BLOCK2_OFFSET				16
#define DEPE_SPARTICLE_BLOCK2_SIZE					16
#define DEPE_SPARTICLE_BLOCK3_OFFSET				32
#define DEPE_SPARTICLE_BLOCK3_SIZE					4
#define DEPE_SPARTICLE_BLOCK4_OFFSET				36
#define DEPE_SPARTICLE_BLOCK4_SIZE					4

/*
#define DEPE_SPARTICLE_SIZE							28

#define DEPE_SPARTICLE_OFFSET_LIFETIME				0
#define DEPE_SPARTICLE_OFFSET_POSITION_X			4
#define DEPE_SPARTICLE_OFFSET_POSITION_Y			8
#define DEPE_SPARTICLE_OFFSET_POSITION_Z			12
#define DEPE_SPARTICLE_OFFSET_LINEAR_DIRECTION_X	16
#define DEPE_SPARTICLE_OFFSET_LINEAR_DIRECTION_Y	17
#define DEPE_SPARTICLE_OFFSET_LINEAR_DIRECTION_Z	18
#define DEPE_SPARTICLE_OFFSET_ANGULAR_VELOCITY		19
#define DEPE_SPARTICLE_OFFSET_LINEAR_VELOCITY		20
#define DEPE_SPARTICLE_OFFSET_ROTATION				21
#define DEPE_SPARTICLE_OFFSET_CAST_SIZE				22
#define DEPE_SPARTICLE_OFFSET_CAST_EMISSIVITY		23
#define DEPE_SPARTICLE_OFFSET_CAST_RED				24
#define DEPE_SPARTICLE_OFFSET_CAST_GREEN			25
#define DEPE_SPARTICLE_OFFSET_CAST_BLUE				26
#define DEPE_SPARTICLE_OFFSET_CAST_TRANSPARENCY		27

#define DEPE_SPARTICLE_BLOCK1_OFFSET				0
#define DEPE_SPARTICLE_BLOCK1_SIZE					16
#define DEPE_SPARTICLE_BLOCK2_OFFSET				16
#define DEPE_SPARTICLE_BLOCK2_SIZE					4
#define DEPE_SPARTICLE_BLOCK3_OFFSET				20
#define DEPE_SPARTICLE_BLOCK3_SIZE					4
#define DEPE_SPARTICLE_BLOCK4_OFFSET				24
#define DEPE_SPARTICLE_BLOCK4_SIZE					4
*/

#endif
