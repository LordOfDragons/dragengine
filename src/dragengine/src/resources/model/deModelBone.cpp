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

#include "deModelBone.h"

#include "../../common/exceptions.h"



// Class deModelBone
//////////////////////

// Constructor, Destructor
////////////////////////////

deModelBone::deModelBone( const char *name ) :
pName( name ),
pParent( -1 ){
}

deModelBone::~deModelBone(){
}



// Management
//////////////

void deModelBone::SetParent( int bone ){
	if( bone < -1 ){
		DETHROW( deeInvalidParam );
	}
	pParent = bone;
}

void deModelBone::SetPosition( const decVector &position ){
	pPosition = position;
}

void deModelBone::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deModelBone::SetMatrix( const decMatrix &matrix ){
	pMatrix = matrix;
	pInverseMatrix = pMatrix.Invert();
}
