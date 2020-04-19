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

#include <stdlib.h>

#include "deOcclusionMeshWeight.h"
#include "../../common/exceptions.h"



// Class deOcclusionMeshWeight
////////////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMeshWeight::deOcclusionMeshWeight(){
	pBone = 0;
	pWeight = 0.0f;
}

deOcclusionMeshWeight::~deOcclusionMeshWeight(){
}



// Management
///////////////

void deOcclusionMeshWeight::SetBone( int bone ){
	if( bone < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pBone = bone;
}

void deOcclusionMeshWeight::SetWeight( float weight ){
	if( pWeight < 0.0f ){
		pWeight = 0.0f;
		
	}else if( pWeight > 1.0f ){
		pWeight = 1.0f;
		
	}else{
		pWeight = weight;
	}
}
