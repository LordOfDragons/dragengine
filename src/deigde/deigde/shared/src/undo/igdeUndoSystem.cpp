/* 
 * Drag[en]gine IGDE
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

#include "igdeUndo.h"
#include "igdeUndoReference.h"
#include "igdeUndoSystem.h"
#include "../editableentity/igdeEditableEntity.h"

#include <dragengine/common/exceptions.h>



// Class igdeUndoSystem
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeUndoSystem::igdeUndoSystem( igdeEditableEntity *editableEntity ) :
pEditableEntity( editableEntity ),
pRedoCount( 0 ),
pMaxUndos( 100 ),
pMaxMemory( 0 ){
}

igdeUndoSystem::~igdeUndoSystem(){
}



// Management
///////////////

int igdeUndoSystem::GetCount() const{
	return pUndos.GetCount();
}

igdeUndo *igdeUndoSystem::GetAt( int index ) const{
	return ( igdeUndo* )pUndos.GetAt( pUndos.GetCount() - 1 - index );
}

igdeUndo *igdeUndoSystem::GetTop() const{
	return ( igdeUndo* )pUndos.GetAt( 0 );
}

void igdeUndoSystem::Add( igdeUndo *undo, bool runRedo ){
	if( ! undo ){
		DETHROW( deeInvalidParam );
	}
	
	if( pMaxUndos == 0 ){
		return;
	}
	
	RemoveAllRedoable();
	
	if( runRedo ){
		undo->Redo();
	}
	
	if( pUndos.GetCount() == pMaxUndos ){
		pUndos.RemoveFrom( 0 );
	}
	pUndos.Add( undo );
	
	pEditableEntity->NotifyUndoChanged();
}

void igdeUndoSystem::RemoveAll(){
	if( pUndos.GetCount() == 0 ){
		return;
	}
	
	pUndos.RemoveAll();
	pRedoCount = 0;
	pEditableEntity->NotifyUndoChanged();
}



void igdeUndoSystem::SetMaxUndoCount( int maxUndos ){
	if( maxUndos < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( maxUndos == pMaxUndos ){
		return;
	}
	
	RemoveAll();
	pMaxUndos = maxUndos;
}

void igdeUndoSystem::SetMaxMemory( int maxMemory ){
	if( maxMemory < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pMaxUndos = maxMemory;
}



int igdeUndoSystem::GetUndoableCount() const{
	return pUndos.GetCount() - pRedoCount;
}

void igdeUndoSystem::Undo(){
	if( pUndos.GetCount() <= pRedoCount ){
		return;
	}
	
	igdeUndo &undo = *( ( igdeUndo* )pUndos.GetAt( pUndos.GetCount() - 1 - pRedoCount ) );
	
	// undo the given action. the undo action is responsible to do its work alike that in
	// the case of an exception the system is still in a working state but not necessary
	// in an undoable state. hence in the case of an exception we reset the undo system.
	// better loose the possibility to undo than loosing a stable system and accidently
	// save it.
	try{
		undo.Undo();
		
	}catch( const deException & ){
		RemoveAll();
		throw;
	}
	
	pRedoCount++;
	pEditableEntity->NotifyUndoChanged();
}

void igdeUndoSystem::Redo(){
	if( pRedoCount == 0 ){
		return;
	}
	
	igdeUndo &undo = *( ( igdeUndo* )pUndos.GetAt( pUndos.GetCount() - pRedoCount ) );
	
	// redo the given action. the redo action is responsible to do its work alike that in the
	// case of an exception the system is still in a working state but not necessary in a
	// redoable state. hence in the case of an exception we reset the undo system. better
	// loose the possibility to / redo than loosing a stable system and accidently save it.
	try{
		undo.Redo();
		
	}catch( const deException & ){
		RemoveAll();
		throw;
	}
	
	pRedoCount--;
	pEditableEntity->NotifyUndoChanged();
}

void igdeUndoSystem::RemoveAllRedoable(){
	if( pRedoCount == 0 ){
		return;
	}
	
	for( ; pRedoCount>0; pRedoCount-- ){
		pUndos.RemoveFrom( pUndos.GetCount() - 1 );
	}
	
	pEditableEntity->NotifyUndoChanged();
}
