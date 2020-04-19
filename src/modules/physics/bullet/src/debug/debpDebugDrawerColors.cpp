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

#include "debpDebugDrawerColors.h"



namespace debpDebugDrawerColors{
	const decColor colorLow( 0.25f, 0.25f, 0.25f );
	
	const decColor colliderFill( 1.0f, 0.5f, 0.0f, 0.1f );
	const decColor colliderEdge( colliderFill, 0.8f );
	
	const decColor colliderLowFill( colliderFill * 0.25f + colorLow * 0.75f, 0.1f );
	const decColor colliderLowEdge( colliderLowFill, 0.8f );
	
	
	
	const decColor colliderBoneFill( colliderFill );
	const decColor colliderBoneEdge( colliderBoneFill, 0.8f );
	
	const decColor colliderBoneLowFill( colliderBoneFill * 0.25f + colorLow * 0.75f );
	const decColor colliderBoneLowEdge( colliderBoneLowFill, 0.8f );
	
	
	
	const decColor touchSensorFill( 0.0f, 1.0f, 1.0f, 0.1f );
	const decColor touchSensorEdge( touchSensorFill, 0.8f );
}
