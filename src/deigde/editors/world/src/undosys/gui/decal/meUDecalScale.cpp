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

#include "meUDecalScale.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalScale
////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalScale::meUDecalScale( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	
	const meDecalList &selection = world->GetSelectionDecal().GetSelected();
	int count = selection.GetCount();
	
	SetShortInfo( "Scale Decals" );
	
	pWorld = NULL;
	
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		if( count > 0 ){
			pDecals = new meUndoDataDecal*[ count ];
			if( ! pDecals ) DETHROW( deeOutOfMemory );
			
			while( pDecalCount < count ){
				pDecals[ pDecalCount ] = new meUndoDataDecal( selection.GetAt( pDecalCount ) );
				if( ! pDecals[ pDecalCount ] ) DETHROW( deeOutOfMemory );
				pDecalCount++;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUDecalScale::~meUDecalScale(){
	pCleanUp();
}



// Management
///////////////

void meUDecalScale::Undo(){
	meDecal *decal;
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		decal = pDecals[ i ]->GetDecal();
		decal->SetPosition( pDecals[ i ]->GetOldPosition() );
		decal->SetSize( pDecals[ i ]->GetOldSize() );
		pWorld->NotifyDecalGeometryChanged( decal );
	}
}

void meUDecalScale::Redo(){
	bool modifyPosition = GetModifyPosition();
	bool modifySize = GetModifySize();
	decDVector position;
	float oldScalingZ;
	decVector scaling;
	meDecal *decal;
	int d;
	
	for( d=0; d<pDecalCount; d++ ){
		decal = pDecals[ d ]->GetDecal();
		
		position = pDecals[ d ]->GetOldPosition();
		scaling = pDecals[ d ]->GetOldSize();
		oldScalingZ = scaling.z;
		
		TransformElement( position, scaling );
		
		if( modifySize ){
			decal->SetSize( decVector( scaling.x, scaling.y, oldScalingZ ) );
		}
		
		if( modifyPosition ){
			decal->SetPosition( position );
		}
		
		pWorld->NotifyDecalGeometryChanged( decal );
	}
}

void meUDecalScale::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meUDecalScale::pCleanUp(){
	if( pDecals ){
		while( pDecalCount > 0 ){
			pDecalCount--;
			delete pDecals[ pDecalCount ];
		}
		
		delete [] pDecals;
	}
	
	if( pWorld ) pWorld->FreeReference();
}
