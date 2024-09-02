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
