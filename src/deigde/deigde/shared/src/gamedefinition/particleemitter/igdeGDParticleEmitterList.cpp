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

#include "igdeGDParticleEmitter.h"
#include "igdeGDParticleEmitterList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDParticleEmitterList::igdeGDParticleEmitterList(){
}

igdeGDParticleEmitterList::~igdeGDParticleEmitterList(){
}



// Management
///////////////

int igdeGDParticleEmitterList::GetCount() const{
	return pEmitters.GetCount();
}

igdeGDParticleEmitter *igdeGDParticleEmitterList::GetAt( int index ) const{
	return ( igdeGDParticleEmitter* )pEmitters.GetAt( index );
}

igdeGDParticleEmitter *igdeGDParticleEmitterList::GetWithPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pEmitters.GetCount();
	igdeGDParticleEmitter *emitter;
	int i;
	
	for( i=0; i<count; i++ ){
		emitter = ( igdeGDParticleEmitter* )pEmitters.GetAt( i );
		
		if( emitter->GetPath().Equals( path ) ){
			return emitter;
		}
	}
	
	return NULL;
}

igdeGDParticleEmitter *igdeGDParticleEmitterList::GetWithName( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pEmitters.GetCount();
	igdeGDParticleEmitter *emitter;
	int i;
	
	for( i=0; i<count; i++ ){
		emitter = ( igdeGDParticleEmitter* )pEmitters.GetAt( i );
		
		if( emitter->GetName().Equals( name ) ){
			return emitter;
		}
	}
	
	return NULL;
}

int igdeGDParticleEmitterList::IndexOf( igdeGDParticleEmitter *emitter ) const{
	return pEmitters.IndexOf( emitter );
}

int igdeGDParticleEmitterList::IndexOfWithPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDParticleEmitter* )pEmitters.GetAt( i ) )->GetPath().Equals( path ) ){
			return i;
		}
	}
	
	return -1;
}

int igdeGDParticleEmitterList::IndexOfWithName( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDParticleEmitter* )pEmitters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

int igdeGDParticleEmitterList::IndexOfWithPathOrName( const char *path, const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDParticleEmitter &emitter = *( ( igdeGDParticleEmitter* )pEmitters.GetAt( i ) );
		
		if( emitter.GetPath().Equals( path ) || emitter.GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDParticleEmitterList::Has( igdeGDParticleEmitter *emitter ) const{
	return pEmitters.Has( emitter );
}

bool igdeGDParticleEmitterList::HasWithPath( const char *path ) const{
	return IndexOfWithPath( path ) != -1;
}

bool igdeGDParticleEmitterList::HasWithName( const char *name ) const{
	return IndexOfWithName( name ) != -1;
}

bool igdeGDParticleEmitterList::HasWithPathOrName( const char *path, const char *name ) const{
	return IndexOfWithPathOrName( path, name ) != -1;
}

void igdeGDParticleEmitterList::Add( igdeGDParticleEmitter *emitter ){
	if( ! emitter || HasWithPathOrName( emitter->GetPath().GetString(), emitter->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pEmitters.Add( emitter );
}

void igdeGDParticleEmitterList::InsertAt( igdeGDParticleEmitter *emitter, int index ){
	if( ! emitter ){
		DETHROW( deeInvalidParam );
	}
	
	pEmitters.Insert( emitter, index );
}

void igdeGDParticleEmitterList::MoveTo( igdeGDParticleEmitter *emitter, int index ){
	pEmitters.Move( emitter, index );
}

void igdeGDParticleEmitterList::Remove( igdeGDParticleEmitter *emitter ){
	pEmitters.Remove( emitter );
}

void igdeGDParticleEmitterList::RemoveAll(){
	pEmitters.RemoveAll();
}



igdeGDParticleEmitterList &igdeGDParticleEmitterList::operator=( const igdeGDParticleEmitterList &list ){
	pEmitters = list.pEmitters;
	return *this;
}
