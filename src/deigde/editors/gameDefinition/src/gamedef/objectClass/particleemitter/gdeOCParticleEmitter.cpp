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

#include <dragengine/common/exceptions.h>



// Class gdeOCParticleEmitter
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCParticleEmitter::gdeOCParticleEmitter() :
pCasting( true ){
}

gdeOCParticleEmitter::gdeOCParticleEmitter( const gdeOCParticleEmitter &emitter ) :
pPath( emitter.pPath ),
pPosition( emitter.pPosition ),
pRotation( emitter.pRotation ),
pBoneName( emitter.pBoneName ),
pCasting( emitter.pCasting )
{
	int i;
	
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = emitter.pPropertyNames[ i ];
	}
	
	for( i=etCasting; i<=etCasting; i++ ){
		pTriggerNames[ i ] = emitter.pTriggerNames[ i ];
	}
}

gdeOCParticleEmitter::~gdeOCParticleEmitter(){
}



// Management
///////////////

void gdeOCParticleEmitter::SetPath( const char *path ){
	pPath = path;
}

void gdeOCParticleEmitter::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCParticleEmitter::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCParticleEmitter::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void gdeOCParticleEmitter::SetCasting( bool casting ){
	pCasting = casting;
}



bool gdeOCParticleEmitter::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCParticleEmitter::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void gdeOCParticleEmitter::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool gdeOCParticleEmitter::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}



bool gdeOCParticleEmitter::IsTriggerSet( eTriggers trigger ) const{
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &gdeOCParticleEmitter::GetTriggerName( eTriggers trigger ) const{
	return pTriggerNames[ trigger ];
}

void gdeOCParticleEmitter::SetTriggerName( eTriggers trigger, const char *name ){
	pTriggerNames[ trigger ] = name;
}
