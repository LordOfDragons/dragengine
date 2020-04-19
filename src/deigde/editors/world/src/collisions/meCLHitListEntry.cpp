/* 
 * Drag[en]gine IGDE World Editor
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

#include "meCLHitListEntry.h"
#include "../world/decal/meDecal.h"
#include "../world/meWorld.h"
#include "../world/navspace/meNavigationSpace.h"
#include "../world/object/meObject.h"
#include "../world/objectshape/meObjectShape.h"

#include <dragengine/common/exceptions.h>



// Class meCLHitListEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

meCLHitListEntry::meCLHitListEntry() :
pObject( NULL ),
pObjectShape( NULL ),
pDecal( NULL ),
pNavSpace( NULL ),
pHTNavSpacePoint( -1 ),
pDistance( 0.0f ){
}

meCLHitListEntry::~meCLHitListEntry(){
	Clear();
}



// Management
///////////////

void meCLHitListEntry::Clear(){
	SetObject( NULL );
	SetObjectShape( NULL );
	SetDecal( NULL );
	SetNavigationSpace( NULL );
	SetHTNavSpacePoint( -1 );
	pDistance = 0.0f;
	pNormal.Set( 0.0f, 1.0f, 0.0f );
}

void meCLHitListEntry::SetObject( meObject *object ){
	pObject = object;
}

void meCLHitListEntry::SetObjectShape( meObjectShape *objectShape ){
	if( objectShape == pObjectShape ){
		return;
	}
	
	if( pObjectShape ){
		pObjectShape->FreeReference();
	}
	
	pObjectShape = objectShape;
	
	if( objectShape ){
		objectShape->AddReference();
	}
}

void meCLHitListEntry::SetDecal( meDecal *decal ){
	if( decal == pDecal ){
		return;
	}
	
	if( pDecal ){
		pDecal->FreeReference();
	}
	
	pDecal = decal;
	
	if( decal ){
		decal->AddReference();
	}
}

void meCLHitListEntry::SetNavigationSpace( meNavigationSpace *navspace ){
	if( navspace == pNavSpace ){
		return;
	}
	
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
	
	pNavSpace = navspace;
	
	if( navspace ){
		navspace->AddReference();
	}
}

void meCLHitListEntry::SetHTNavSpacePoint( int point ){
	pHTNavSpacePoint = point;
}

void meCLHitListEntry::SetDistance( float distance ){
	pDistance = distance;
}

void meCLHitListEntry::SetNormal( const decVector &normal ){
	pNormal = normal;
}

void meCLHitListEntry::SortDecals(){
	// TODO
	DETHROW( deeInvalidParam );
}

int meCLHitListEntry::CompareTo( const meCLHitListEntry &entry ) const{
	const float difference = entry.GetDistance() - pDistance;
	
	if( difference > FLOAT_SAFE_EPSILON ){
		return -1;
	}
	if( difference < -FLOAT_SAFE_EPSILON ){
		return 1;
	}
	
	if( entry.pDecal && pDecal && entry.pObject == pObject && entry.pObjectShape == pObjectShape
	&& entry.pNavSpace == pNavSpace && entry.pHTNavSpacePoint == pHTNavSpacePoint ){
		int myIndex = 0;
		int otherIndex = 0;
		
		if( pObject ){
			myIndex = pObject->IndexOfDecal( pDecal );
			otherIndex = entry.pObject->IndexOfDecal( entry.pDecal );
			
			if( myIndex < otherIndex ){
				return 1;
				
			}else if( myIndex > otherIndex ){
				return -1;
			}
		}
	}
	
	return 0;
}
