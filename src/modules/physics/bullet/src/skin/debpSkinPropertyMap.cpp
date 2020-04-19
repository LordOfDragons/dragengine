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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debpSkinPropertyMap.h"

#include <dragengine/common/exceptions.h>



// Class debpSkinPropertyMap
//////////////////////////////

// Constructor, destructor
////////////////////////////

debpSkinPropertyMap::debpSkinPropertyMap(){
}

debpSkinPropertyMap::~debpSkinPropertyMap(){
}



// Management
///////////////

int debpSkinPropertyMap::GetTypeFor( const char* type ) const{
	if( strcmp( type, "particle.size.cast" ) == 0 ){
		return eptParticleSizeCast;
		
	}else if( strcmp( type, "particle.size.lifetime" ) == 0 ){
		return eptParticleSizeLifetime;
		
	}else if( strcmp( type, "particle.color.cast" ) == 0 ){
		return eptParticleColorCast;
		
	}else if( strcmp( type, "particle.color.lifetime" ) == 0 ){
		return eptParticleColorLifetime;
		
	}else if( strcmp( type, "particle.transparency.cast" ) == 0 ){
		return eptParticleTranspCast;
		
	}else if( strcmp( type, "particle.transparency.lifetime" ) == 0 ){
		return eptParticleTranspLifetime;
		
	}else if( strcmp( type, "bounciness" ) == 0 ){
		return eptBounciness;
		
	}else{
		return eptUnsupported;
	}
}
