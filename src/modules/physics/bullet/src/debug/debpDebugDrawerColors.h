/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPDEBUGDRAWERCOLORS_H_
#define _DEBPDEBUGDRAWERCOLORS_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Debug drawer colors.
 */
namespace debpDebugDrawerColors{
	extern const decColor colliderFill;
	extern const decColor colliderEdge;
	
	extern const decColor colliderLowFill;
	extern const decColor colliderLowEdge;
	
	
	
	extern const decColor colliderBoneFill;
	extern const decColor colliderBoneEdge;
	
	extern const decColor colliderBoneLowFill;
	extern const decColor colliderBoneLowEdge;
	
	
	
	extern const decColor touchSensorFill;
	extern const decColor touchSensorEdge;
};

#endif
