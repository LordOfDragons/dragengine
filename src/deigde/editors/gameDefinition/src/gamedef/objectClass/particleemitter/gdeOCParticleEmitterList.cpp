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

#include "gdeOCParticleEmitter.h"
#include "gdeOCParticleEmitterList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCParticleEmitter
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCParticleEmitterList::gdeOCParticleEmitterList(){
}

gdeOCParticleEmitterList::gdeOCParticleEmitterList( const gdeOCParticleEmitterList &list ) :
pParticleEmitters( list.pParticleEmitters ){
}

gdeOCParticleEmitterList::~gdeOCParticleEmitterList(){
}



// Management
///////////////

int gdeOCParticleEmitterList::GetCount() const{
	return pParticleEmitters.GetCount();
}

gdeOCParticleEmitter *gdeOCParticleEmitterList::GetAt( int index ) const{
	return ( gdeOCParticleEmitter* )pParticleEmitters.GetAt( index );
}

int gdeOCParticleEmitterList::IndexOf( gdeOCParticleEmitter *emitter ) const{
	return pParticleEmitters.IndexOf( emitter );
}

bool gdeOCParticleEmitterList::Has( gdeOCParticleEmitter *emitter ) const{
	return pParticleEmitters.Has( emitter );
}

void gdeOCParticleEmitterList::Add( gdeOCParticleEmitter *emitter ){
	if( ! emitter || Has( emitter ) ){
		DETHROW( deeInvalidParam );
	}
	pParticleEmitters.Add( emitter );
}

void gdeOCParticleEmitterList::Remove( gdeOCParticleEmitter *emitter ){
	pParticleEmitters.Remove( emitter );
}

void gdeOCParticleEmitterList::RemoveAll(){
	pParticleEmitters.RemoveAll();
}



gdeOCParticleEmitterList &gdeOCParticleEmitterList::operator=( const gdeOCParticleEmitterList &list ){
	pParticleEmitters = list.pParticleEmitters;
	return *this;
}
