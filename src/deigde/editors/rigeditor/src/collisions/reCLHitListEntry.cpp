/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <math.h>
#include <stdlib.h>

#include "reCLHitListEntry.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/push/reRigPush.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/constraint/reRigConstraint.h"

#include <dragengine/common/exceptions.h>



// Class reCLHitListEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

reCLHitListEntry::reCLHitListEntry(){
	pBone = NULL;
	pShape = NULL;
	pPush = NULL;
	pConstraint = NULL;
	pDistance = 0.0f;
}

reCLHitListEntry::~reCLHitListEntry(){
	Clear();
}



// Management
///////////////

void reCLHitListEntry::Clear(){
	if( pBone ){
		pBone->FreeReference();
		pBone = NULL;
	}
	
	if( pShape ){
		pShape->FreeReference();
		pShape = NULL;
	}
	
	if( pConstraint ){
		pConstraint->FreeReference();
		pConstraint = NULL;
	}
	
	if( pPush ){
		pPush->FreeReference();
		pPush = NULL;
	}
	
	pDistance = 0.0f;
}

void reCLHitListEntry::SetBone( reRigBone *bone ){
	if( bone != pBone ){
		if( pBone ) pBone->FreeReference();
		
		pBone = bone;
		
		if( bone ) bone->AddReference();
	}
}

void reCLHitListEntry::SetShape( reRigShape *shape ){
	if( shape != pShape ){
		if( pShape ) pShape->FreeReference();
		
		pShape = shape;
		
		if( shape ) shape->AddReference();
	}
}

void reCLHitListEntry::SetConstraint( reRigConstraint *constraint ){
	if( constraint != pConstraint ){
		if( pConstraint ) pConstraint->FreeReference();
		
		pConstraint = constraint;
		
		if( constraint ) constraint->AddReference();
	}
}

void reCLHitListEntry::SetPush( reRigPush *push ){
	if( push != pPush ){
		if( pPush ) pPush->FreeReference();
		
		pPush = push;
		
		if( push ) push->AddReference();
	}
}

void reCLHitListEntry::SetDistance( float distance ){
	pDistance = distance;
}

void reCLHitListEntry::SetNormal( const decVector &normal ){
	pNormal = normal;
}

int reCLHitListEntry::CompareTo( const reCLHitListEntry &entry ) const{
	float difference = entry.GetDistance() - pDistance;
	
	if( difference > 1e-5f ){
		return -1;
		
	}else if( difference < -1e-5f ){
		return 1;
		
	}else{
		return 0;
	}
}
