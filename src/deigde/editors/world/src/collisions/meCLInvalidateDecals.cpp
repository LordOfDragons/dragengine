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
