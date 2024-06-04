/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
