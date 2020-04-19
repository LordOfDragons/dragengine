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
#include <string.h>

#include "igdeGDCParticleEmitter.h"
#include "igdeGDCParticleEmitterList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCParticleEmitter
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCParticleEmitterList::igdeGDCParticleEmitterList(){
}

igdeGDCParticleEmitterList::igdeGDCParticleEmitterList( const igdeGDCParticleEmitterList &list ){
	pEmitters = list.pEmitters;
}

igdeGDCParticleEmitterList::~igdeGDCParticleEmitterList(){
}



// Management
///////////////

int igdeGDCParticleEmitterList::GetCount() const{
	return pEmitters.GetCount();
}

igdeGDCParticleEmitter *igdeGDCParticleEmitterList::GetAt( int index ) const{
	return ( igdeGDCParticleEmitter* )pEmitters.GetAt( index );
}

int igdeGDCParticleEmitterList::IndexOf( igdeGDCParticleEmitter *emitter ) const{
	return pEmitters.IndexOf( emitter );
}

bool igdeGDCParticleEmitterList::Has( igdeGDCParticleEmitter *emitter ) const{
	return pEmitters.Has( emitter );
}

void igdeGDCParticleEmitterList::Add( igdeGDCParticleEmitter *emitter ){
	if( ! emitter ){
		DETHROW( deeInvalidParam );
	}
	
	pEmitters.Add( emitter );
}

void igdeGDCParticleEmitterList::InsertAt( igdeGDCParticleEmitter *emitter, int index ){
	if( ! emitter ){
		DETHROW( deeInvalidParam );
	}
	
	pEmitters.Insert( emitter, index );
}

void igdeGDCParticleEmitterList::MoveTo( igdeGDCParticleEmitter *emitter, int index ){
	pEmitters.Move( emitter, index );
}

void igdeGDCParticleEmitterList::Remove( igdeGDCParticleEmitter *emitter ){
	pEmitters.Remove( emitter );
}

void igdeGDCParticleEmitterList::RemoveAll(){
	pEmitters.RemoveAll();
}



void igdeGDCParticleEmitterList::SetToDeepCopyFrom( const igdeGDCParticleEmitterList &list ){
	const int count = list.GetCount();
	igdeGDCParticleEmitter *emitter = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			emitter = new igdeGDCParticleEmitter( *list.GetAt( i ) );
			Add( emitter );
			emitter->FreeReference();
			emitter = NULL;
		}
		
	}catch( const deException & ){
		if( emitter ){
			emitter->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCParticleEmitterList &igdeGDCParticleEmitterList::operator=( const igdeGDCParticleEmitterList &list ){
	pEmitters = list.pEmitters;
	return *this;
}
