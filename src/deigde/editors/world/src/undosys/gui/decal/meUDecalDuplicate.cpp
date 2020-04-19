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
#include <unistd.h>

#include "meUDecalDuplicate.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalDuplicate
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalDuplicate::meUDecalDuplicate( meWorld *world, const decVector &offset ) :
pWorld( NULL ),
pDecals( NULL ),
pDecalCount( 0 )
{
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const meDecalList &list = world->GetSelectionDecal().GetSelected();
	int count = list.GetCount();
	meDecal *duplicate = NULL;
	meDecal *decal;
	
	SetShortInfo( "Duplicate decals" );
	
	pWorld = NULL;
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		if( count > 0 ){
			pDecals = new meUndoDataDecal*[ count ];
			
			while( pDecalCount < count ){
				decal = list.GetAt( pDecalCount );
				
				duplicate = new meDecal( *decal );
				duplicate->SetPosition( decal->GetPosition() + offset );
				pDecals[ pDecalCount ] = new meUndoDataDecal( duplicate );
				pDecalCount++;
				
				duplicate->FreeReference();
				duplicate = NULL;
			}
		}
		
	}catch( const deException & ){
		if( duplicate ){
			duplicate->FreeReference();
		}
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUDecalDuplicate::~meUDecalDuplicate(){
	pCleanUp();
}



// Management
///////////////

void meUDecalDuplicate::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		meDecal * const decal = pDecals[ i ]->GetDecal();
		
		selection.Remove( decal );
		if( decal->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveDecal( pDecals[ i ]->GetDecal() );
		
		//pWorld->NotifyDecalRemoved( decal );
	}
	pWorld->NotifyDecalCountChanged();
	
	pWorld->NotifyDecalSelectionChanged();
}

void meUDecalDuplicate::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	int i;
	
	selection.Reset();
	
	for( i=0; i<pDecalCount; i++ ){
		meDecal * const decal = pDecals[ i ]->GetDecal();
		
		pWorld->AddDecal( decal );
		selection.Add( decal );
		
		//pWorld->NotifyDecalAdded( decal );
	}
	pWorld->NotifyDecalCountChanged();
	
	selection.ActivateNext();
	pWorld->NotifyDecalSelectionChanged();
}



// Private Functions
//////////////////////

void meUDecalDuplicate::pCleanUp(){
	if( pDecals ){
		while( pDecalCount > 0 ){
			pDecalCount--;
			delete pDecals[ pDecalCount ];
		}
		
		delete [] pDecals;
	}
	
	if( pWorld ){
		pWorld->FreeReference();
	}
}
