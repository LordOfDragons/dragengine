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

#include "igdeGDCSpeaker.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCSpeaker
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCSpeaker::igdeGDCSpeaker() :
pLooping( true ),
pPlaying( true ),
pVolume( 1.0f ),
pRange( 1.0f ),
pRollOff( 1.0f ),
pDistanceOffset( 0.0f ),
pPlaySpeed( 1.0f ){
}

igdeGDCSpeaker::igdeGDCSpeaker( const igdeGDCSpeaker &speaker ) :
pPathSound( speaker.pPathSound ),
pPosition( speaker.pPosition ),
pOrientation( speaker.pOrientation ),
pBoneName( speaker.pBoneName ),
pLooping( speaker.pLooping ),
pPlaying( speaker.pPlaying ),
pVolume( speaker.pVolume ),
pRange( speaker.pRange ),
pRollOff( speaker.pRollOff ),
pDistanceOffset( speaker.pDistanceOffset ),
pPlaySpeed( speaker.pPlaySpeed )
{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = speaker.pPropertyNames[ i ];
	}
	for( i=0; i<=etMuted; i++ ){
		pTriggerNames[ i ] = speaker.pTriggerNames[ i ];
	}
}

igdeGDCSpeaker::~igdeGDCSpeaker(){
}



// Management
///////////////

void igdeGDCSpeaker::SetPathSound( const char *path ){
	pPathSound = path;
}

void igdeGDCSpeaker::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCSpeaker::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCSpeaker::SetBoneName( const char* boneName ){
	pBoneName = boneName;
}

void igdeGDCSpeaker::SetLooping( bool looping ){
	pLooping = looping;
}

void igdeGDCSpeaker::SetPlaying( bool playing ){
	pPlaying = playing;
}

void igdeGDCSpeaker::SetVolume( float volume ){
	pVolume = decMath::max( volume, 0.0f );
}

void igdeGDCSpeaker::SetRange( float range ){
	pRange = decMath::max( range, 0.0f );
}

void igdeGDCSpeaker::SetRollOff( float rollOff ){
	pRollOff = decMath::max( rollOff, 0.0f );
}

void igdeGDCSpeaker::SetDistanceOffset( float distanceOffset ){
	pDistanceOffset = decMath::max( distanceOffset, 0.0f );
}

void igdeGDCSpeaker::SetPlaySpeed( float playSpeed ){
	pPlaySpeed = playSpeed;
}



bool igdeGDCSpeaker::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCSpeaker::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCSpeaker::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}



bool igdeGDCSpeaker::IsTriggerSet( eTriggers trigger ) const{
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &igdeGDCSpeaker::GetTriggerName( eTriggers trigger ) const{
	return pTriggerNames[ trigger ];
}

void igdeGDCSpeaker::SetTriggerName( eTriggers trigger, const char *name ){
	pTriggerNames[ trigger ] = name;
}

bool igdeGDCSpeaker::HasPropertyWithName( const char *name ) const{
	int j;
	
	for( j=0; j<=epAttachRotation; j++ ){
		if( pPropertyNames[ j ] == name ){
			return true;
		}
	}
	
	return false;
}
