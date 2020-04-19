/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalRayCacheRayHit.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayCacheRayHit
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoalRayCacheRayHit::deoalRayCacheRayHit() :
pDistance( 0.0f ),
pFaceIndex( 0 ){
}

deoalRayCacheRayHit::~deoalRayCacheRayHit(){
}



// Management
///////////////

void deoalRayCacheRayHit::SetDistance( float distance ){
	pDistance = distance;
}

void deoalRayCacheRayHit::SetFaceIndex( int faceIndex ){
	pFaceIndex = faceIndex;
}

void deoalRayCacheRayHit::SetForwardFacing( bool forwardFacing ){
	pForwardFacing = forwardFacing;
}

void deoalRayCacheRayHit::SetPoint( const decVector &point ){
	pPoint = point;
}

void deoalRayCacheRayHit::SetNormal( const decVector &normal ){
	pNormal = normal;
}
