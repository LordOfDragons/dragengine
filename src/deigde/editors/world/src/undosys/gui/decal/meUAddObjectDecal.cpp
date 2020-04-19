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

#include "meUAddObjectDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"
#include "dragengine/common/exceptions.h"



// Class meUAddObjectDecal
////////////////////////////

// Constructor, destructor
////////////////////////////

meUAddObjectDecal::meUAddObjectDecal( meWorld *world, meObject *parentObject ){
	if( ! world || ! parentObject ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pDecal = NULL;
	
	pParentObject = parentObject;
	parentObject->AddReference();
	
	try{
		pDecal = new meDecal( world->GetEnvironment() );
		SetShortInfo( "Add Object Decal" );
		UpdateInfos();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUAddObjectDecal::~meUAddObjectDecal(){
	pCleanUp();
}



// Management
///////////////

void meUAddObjectDecal::UpdateInfos(){
	SetLongInfo( "" );
}



// Undo and Redo operations
/////////////////////////////

void meUAddObjectDecal::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	selection.Remove( pDecal );
	if( pDecal->GetActive() ) selection.ActivateNext();
	
	pParentObject->RemoveDecal( pDecal );
}

void meUAddObjectDecal::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	pParentObject->AddDecal( pDecal );
	
	selection.Reset();
	selection.Add( pDecal );
	selection.SetActive( pDecal );
}



// Private Functions
//////////////////////

void meUAddObjectDecal::pCleanUp(){
	if( pDecal ) pDecal->FreeReference();
	if( pParentObject ) pParentObject->FreeReference();
}
