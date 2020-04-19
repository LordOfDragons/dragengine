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

#include "meUDeleteNavSpace.h"
#include "meUndoDataNavSpace.h"
#include "../../../world/meWorld.h"
#include "../../../world/navspace/meNavigationSpace.h"
#include "../../../world/navspace/meNavigationSpaceSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteNavSpace
////////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteNavSpace::meUDeleteNavSpace( meWorld *world ) :
pWorld( NULL ),
pNavSpaces( NULL ),
pNavSpaceCount( 0 )
{
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const meNavigationSpaceList &list = world->GetSelectionNavigationSpace().GetSelected();
	int count = list.GetCount();
	decString text;
	
	SetShortInfo( "Delete Objects" );
	if( count > 1 ){
		text.Format( "%i objects", count );
		
	}else{
		text = "1 object";
	}
	SetLongInfo( text );
	
	try{
		pWorld = world;
		world->AddReference();
		
		if( count > 0 ){
			pNavSpaces = new meUndoDataNavSpace*[ count ];
			
			while( pNavSpaceCount < count ){
				pNavSpaces[ pNavSpaceCount ] = new meUndoDataNavSpace( list.GetAt( pNavSpaceCount ) );
				pNavSpaceCount++;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUDeleteNavSpace::~meUDeleteNavSpace(){
	pCleanUp();
}



// Management
///////////////

void meUDeleteNavSpace::Undo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	int i;
	
	selection.Reset();
	
	for( i=0; i<pNavSpaceCount; i++ ){
		meNavigationSpace * const navspace = pNavSpaces[ i ]->GetNavSpace();
		
		pWorld->AddNavSpace( navspace );
		selection.Add( navspace );
		
		pWorld->NotifyNavSpaceCountChanged();
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyNavSpaceSelectionChanged();
}

void meUDeleteNavSpace::Redo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	int i;
	
	for( i=0; i<pNavSpaceCount; i++ ){
		meNavigationSpace * const navspace = pNavSpaces[ i ]->GetNavSpace();
		
		selection.Remove( navspace );
		if( navspace->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveNavSpace( pNavSpaces[ i ]->GetNavSpace() );
		
		pWorld->NotifyNavSpaceCountChanged();
	}
	
	pWorld->NotifyNavSpaceSelectionChanged();
}



// Private Functions
//////////////////////

void meUDeleteNavSpace::pCleanUp(){
	if( pNavSpaces ){
		while( pNavSpaceCount > 0 ){
			pNavSpaceCount--;
			delete pNavSpaces[ pNavSpaceCount ];
		}
		
		delete [] pNavSpaces;
	}
	
	if( pWorld ){
		pWorld->FreeReference();
	}
}
