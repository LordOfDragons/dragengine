/* 
 * Drag[en]gine AI Module
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
#include <string.h>

#include "dedaiPathFinderFunnel.h"

#include <dragengine/common/exceptions.h>



// Class dedaiPathFinderFunnel
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiPathFinderFunnel::dedaiPathFinderFunnel(){
	pMesh = NULL;
	pLeftDist = 0.0f;
	pRightDist = 0.0f;
}

dedaiPathFinderFunnel::~dedaiPathFinderFunnel(){
}



// Management
///////////////

void dedaiPathFinderFunnel::SetMesh( dedaiSpaceMesh *mesh ){
	pMesh = mesh;
}

void dedaiPathFinderFunnel::SetOrigin( const decVector &origin ){
	pOrigin = origin;
	//pLeftDist = origin * pLeftNormal;
	//pRightDist = origin * pRightNormal;
}

void dedaiPathFinderFunnel::SetGoal( const decVector &goal ){
	pGoal = goal;
}

void dedaiPathFinderFunnel::SetLeftCorner( const decVector &corner ){
	pLeftCorner = corner;
}

void dedaiPathFinderFunnel::SetLeftNormal( const decVector &normal ){
	pLeftNormal = normal;
	pLeftDist = pOrigin * normal;
}

void dedaiPathFinderFunnel::SetRightCorner( const decVector &corner ){
	pRightCorner = corner;
}

void dedaiPathFinderFunnel::SetRightNormal( const decVector &normal ){
	pRightNormal = normal;
	pRightDist = pOrigin * normal;
}



void dedaiPathFinderFunnel::UpdateLeftPlane( const decVector &faceNormal ){
	float len;
	
	pLeftNormal = faceNormal % ( pLeftCorner - pOrigin );
	len = pLeftNormal.Length();
	if( len > FLOAT_SAFE_EPSILON ){
		pLeftNormal /= len;
		
	}else{
		pLeftNormal = faceNormal % ( pLeftCorner - pRightCorner );
		len = pLeftNormal.Length();
		if( len > FLOAT_SAFE_EPSILON ){
			pLeftNormal /= len;
			
		}else{
			pLeftNormal.SetZero();
		}
	}
	pLeftDist = pOrigin * pLeftNormal;
}

void dedaiPathFinderFunnel::UpdateRightPlane( const decVector &faceNormal ){
	float len;
	
	pRightNormal = faceNormal % ( pOrigin - pRightCorner );
	len = pRightNormal.Length();
	if( len > FLOAT_SAFE_EPSILON ){
		pRightNormal /= len;
		
	}else{
		pRightNormal = faceNormal % ( pRightCorner - pLeftCorner );
		len = pRightNormal.Length();
		if( len > FLOAT_SAFE_EPSILON ){
			pRightNormal /= len;
			
		}else{
			pRightNormal.SetZero();
		}
	}
	pRightDist = pOrigin * pRightNormal;
}

void dedaiPathFinderFunnel::Transform( const decMatrix &matrix ){
	pOrigin = matrix * pOrigin;
	
	pLeftCorner = matrix * pLeftCorner;
	pLeftNormal = matrix.TransformNormal( pLeftNormal );
	pLeftDist = pOrigin * pLeftNormal;
	
	pRightCorner = matrix * pRightCorner;
	pRightNormal = matrix.TransformNormal( pRightNormal );
	pRightDist = pOrigin * pRightNormal;
}
