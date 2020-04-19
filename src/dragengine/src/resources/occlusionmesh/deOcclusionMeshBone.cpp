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

#include <stdio.h>
#include <stdlib.h>

#include "deOcclusionMeshBone.h"

#include "../../common/exceptions.h"



// Class deOcclusionMeshBone
//////////////////////////////

// Constructor, Destructor
////////////////////////////

deOcclusionMeshBone::deOcclusionMeshBone(){
	pParent = -1;
}

deOcclusionMeshBone::~deOcclusionMeshBone(){
}



// Management
//////////////

void deOcclusionMeshBone::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
}

void deOcclusionMeshBone::SetParent( int bone ){
	if( bone < -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pParent = bone;
}

void deOcclusionMeshBone::SetPosition( const decVector &position ){
	pPosition = position;
}

void deOcclusionMeshBone::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}
