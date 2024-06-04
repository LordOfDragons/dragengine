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
