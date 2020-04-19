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

#include "debpRig.h"
#include "dePhysicsBullet.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// class debpRig
//////////////////

// constructor, destructor
////////////////////////////

debpRig::debpRig( dePhysicsBullet &bullet, const deRig &rig ) :
pBullet( bullet ),
pRig( rig ),
pMass( 0.0f ),
pInvMass( 0.0f )
{
	pCalculateMass();
}

debpRig::~debpRig(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void debpRig::pCleanUp(){
}

void debpRig::pCalculateMass(){
	const int boneCount = pRig.GetBoneCount();
	int i;
	
	pMass = 0.0f;
	pInvMass = 0.0f;
	
	for( i=0; i<boneCount; i++ ){
		pMass += pRig.GetBoneAt( i ).GetMass();
	}
	
	if( pMass > FLOAT_SAFE_EPSILON ){
		pInvMass = 1.0f / pMass;
	}
}
