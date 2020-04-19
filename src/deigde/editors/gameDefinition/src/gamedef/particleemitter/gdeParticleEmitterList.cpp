/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeParticleEmitter.h"
#include "gdeParticleEmitterList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeParticleEmitter
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeParticleEmitterList::gdeParticleEmitterList(){
}

gdeParticleEmitterList::gdeParticleEmitterList( const gdeParticleEmitterList &list ) :
pParticleEmitters( list.pParticleEmitters ){
}

gdeParticleEmitterList::~gdeParticleEmitterList(){
}



// Management
///////////////

int gdeParticleEmitterList::GetCount() const{
	return pParticleEmitters.GetCount();
}

gdeParticleEmitter *gdeParticleEmitterList::GetAt( int index ) const{
	return ( gdeParticleEmitter* )pParticleEmitters.GetAt( index );
}

gdeParticleEmitter *gdeParticleEmitterList::GetWithPath( const char *path ) const{
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeParticleEmitter * const emitter = ( gdeParticleEmitter* )pParticleEmitters.GetAt( i );
		
		if( emitter->GetPath() == path ){
			return emitter;
		}
	}
	
	return NULL;
}

int gdeParticleEmitterList::IndexOf( gdeParticleEmitter *emitter ) const{
	return pParticleEmitters.IndexOf( emitter );
}

int gdeParticleEmitterList::IndexOfWithPath( const char *path ) const{
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeParticleEmitter* )pParticleEmitters.GetAt( i ) )->GetPath() == path ){
			return i;
		}
	}
	
	return -1;
}

bool gdeParticleEmitterList::Has( gdeParticleEmitter *emitter ) const{
	return pParticleEmitters.Has( emitter );
}

bool gdeParticleEmitterList::HasWithPath( const char *path ) const{
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeParticleEmitter* )pParticleEmitters.GetAt( i ) )->GetPath() == path ){
			return true;
		}
	}
	
	return false;
}

void gdeParticleEmitterList::Add( gdeParticleEmitter *emitter ){
	if( ! emitter || HasWithPath( emitter->GetPath() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pParticleEmitters.Add( emitter );
}

void gdeParticleEmitterList::Remove( gdeParticleEmitter *emitter ){
	pParticleEmitters.Remove( emitter );
}

void gdeParticleEmitterList::RemoveAll(){
	pParticleEmitters.RemoveAll();
}



gdeParticleEmitterList &gdeParticleEmitterList::operator=( const gdeParticleEmitterList &list ){
	pParticleEmitters = list.pParticleEmitters;
	return *this;
}
