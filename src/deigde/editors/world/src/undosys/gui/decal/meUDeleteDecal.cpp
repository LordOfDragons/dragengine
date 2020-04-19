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

#include "meUDeleteDecal.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteDecal
/////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteDecal::meUDeleteDecal( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	decString text;
	
	const meDecalList &selection = world->GetSelectionDecal().GetSelected();
	int count = selection.GetCount();
	
	pWorld = world;
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
		
		SetShortInfo( "Delete Decals" );
		if( pDecalCount > 1 ){
			text.Format( "%d decals", pDecalCount );
		}else{
			text = "1 object";
		}
		SetLongInfo( text );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUDeleteDecal::~meUDeleteDecal(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void meUDeleteDecal::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	meDecal *decal;
	int i;
	
	selection.Reset();
	for( i=0; i<pDecalCount; i++ ){
		decal = pDecals[ i ]->GetDecal();
		
		if( pDecals[ i ]->GetParentObject() ){
			pDecals[ i ]->GetParentObject()->InsertDecalAt( decal, pDecals[ i ]->GetIndex() );
		}
		
		pWorld->AddDecal( decal );
		selection.Add( decal );
	}
	selection.ActivateNext();
}

void meUDeleteDecal::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	meDecal *decal;
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		decal = pDecals[ i ]->GetDecal();
		
		selection.Remove( decal );
		if( decal->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveDecal( pDecals[ i ]->GetDecal() );
		
		if( pDecals[ i ]->GetParentObject() ){
			pDecals[ i ]->GetParentObject()->RemoveDecal( decal );
		}
	}
}



// Private Functions
//////////////////////

void meUDeleteDecal::pCleanUp(){
	if( pDecals ){
		while( pDecalCount > 0 ){
			delete pDecals[ pDecalCount - 1 ];
			pDecalCount--;
		}
		delete [] pDecals;
	}
}
