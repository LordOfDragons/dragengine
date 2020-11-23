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

#include "debpShape.h"

#include "dragengine/common/exceptions.h"



// Class debpShape
////////////////////

// Constructor, destructor
////////////////////////////

debpShape::debpShape( int type, decShape *shape ) :
pType( type ),
pShape( shape ),
pCollisionVolume( NULL )
{
	if( type < estSphere || type > estCapsule || ! shape ){
		DETHROW( deeInvalidParam );
	}
}

debpShape::~debpShape(){
}



// Management
///////////////

void debpShape::SetCollisionVolume( debpDCollisionVolume *collisionVolume ){
	pCollisionVolume = collisionVolume;
}

float debpShape::UniformScale( const decDMatrix &matrix ){
	const decDVector scale( matrix.GetScale() );
	return ( float )( ( scale.x + scale.y + scale.z ) / 3.0 );
}
