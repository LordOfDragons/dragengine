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

#include <dragengine/common/exceptions.h>



// Class igdeGDCParticleEmitter
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCParticleEmitter::igdeGDCParticleEmitter() :
pCasting( true ){
}

igdeGDCParticleEmitter::igdeGDCParticleEmitter( const igdeGDCParticleEmitter &emitter ) :
pPath( emitter.pPath ),
pPosition( emitter.pPosition ),
pOrientation( emitter.pOrientation ),
pBoneName( emitter.pBoneName ),
pCasting( emitter.pCasting )
{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = emitter.pPropertyNames[ i ];
	}
	for( i=0; i<=etCasting; i++ ){
		pTriggerNames[ i ] = emitter.pTriggerNames[ i ];
	}
}

igdeGDCParticleEmitter::~igdeGDCParticleEmitter(){
}



// Management
///////////////

void igdeGDCParticleEmitter::SetPath( const char *path ){
	pPath = path;
}

void igdeGDCParticleEmitter::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCParticleEmitter::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCParticleEmitter::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void igdeGDCParticleEmitter::SetCasting( bool casting ){
	pCasting = casting;
}



bool igdeGDCParticleEmitter::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCParticleEmitter::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCParticleEmitter::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool igdeGDCParticleEmitter::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	return false;
}



bool igdeGDCParticleEmitter::IsTriggerSet( eTriggers trigger ) const{
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &igdeGDCParticleEmitter::GetTriggerName( eTriggers trigger ) const{
	return pTriggerNames[ trigger ];
}

void igdeGDCParticleEmitter::SetTriggerName( eTriggers trigger, const char *name ){
	pTriggerNames[ trigger ] = name;
}
