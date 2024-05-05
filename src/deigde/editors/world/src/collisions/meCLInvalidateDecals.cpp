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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meCLInvalidateDecals.h"
#include "meCLHitListEntry.h"
#include "../world/meWorld.h"
#include "../world/decal/meDecal.h"

#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>



// Class meCLInvalidateDecals::Helper
/////////////////////////////////////

meCLInvalidateDecals::Helper::Helper( meWorld *world ) :
pVisitor( world ? new meCLInvalidateDecals( world ) : NULL ){
}

meCLInvalidateDecals::Helper::~Helper(){
	if( pVisitor ){
		delete pVisitor;
	}
}

void meCLInvalidateDecals::Helper::Collect( const decDVector &position, const decVector &minExtend,
const decVector &maxExtend, const decQuaternion &orientation ){
	if( pVisitor ){
		pVisitor->Collect( position, minExtend, maxExtend, orientation );
	}
}

void meCLInvalidateDecals::Helper::Collect( igdeWObject &wobject ){
	if( pVisitor ){
		pVisitor->Collect( wobject );
	}
}

void meCLInvalidateDecals::Helper::InvalidateDecals(){
	if( pVisitor ){
		pVisitor->InvalidateDecals();
	}
}



// Class meCLInvalidateDecals
/////////////////////////////

// Constructor, destructor
////////////////////////////

meCLInvalidateDecals::meCLInvalidateDecals( meWorld *world ) : meCLCollect( world ){
	SetCollectObjects( false );
	SetCollectDecals( true );
}

meCLInvalidateDecals::~meCLInvalidateDecals(){
}



// Management
///////////////

void meCLInvalidateDecals::InvalidateDecals(){
	decPointerSet set;
	int i, count = GetCollectedElements().GetEntryCount();
	for( i=0; i<count; i++ ){
		set.AddIfAbsent( GetCollectedElements().GetEntryAt( i )->GetDecal() );
	}
	
	count = set.GetCount();
	for( i=0; i<count; i++ ){
		( ( meDecal* )set.GetAt( i ) )->InvalidateDecals();
	}
}

void meCLInvalidateDecals::Collect( const decDVector &position, const decVector &minExtend,
const decVector &maxExtend, const decQuaternion &orientation ){
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmDecals );
	
	GetWorld()->CollisionTestBox( position, minExtend, maxExtend, orientation,
		this, decCollisionFilter( collisionCategory, collisionFilter ) );
}

void meCLInvalidateDecals::Collect( igdeWObject &wobject ){
	Collect( wobject.GetPosition(), wobject.GetBoxMinExtend(),
		wobject.GetBoxMaxExtend(), wobject.GetOrientation() );
}
