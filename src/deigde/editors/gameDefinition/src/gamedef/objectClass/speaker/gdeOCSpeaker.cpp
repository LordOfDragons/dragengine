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

#include "gdeOCSpeaker.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCSpeaker
///////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSpeaker::gdeOCSpeaker() :
pLooping( true ),
pPlaying( true ),
pVolume( 1.0f ),
pRange( 1.0f ),
pRollOff( 1.0f ),
pDistanceOffset( 0.0f ),
pPlaySpeed( 1.0f ){
}

gdeOCSpeaker::gdeOCSpeaker( const gdeOCSpeaker &speaker ) :
pPathSound( speaker.pPathSound ),
pPosition( speaker.pPosition ),
pRotation( speaker.pRotation ),
pBoneName( speaker.pBoneName ),
pLooping( speaker.pLooping ),
pPlaying( speaker.pPlaying ),
pVolume( speaker.pVolume ),
pRange( speaker.pRange ),
pRollOff( speaker.pRollOff ),
pDistanceOffset( speaker.pDistanceOffset ),
pPlaySpeed ( speaker.pPlaySpeed )
{
	int i;
	for( i=0; i<EP_COUNT; i++ ){
		pPropertyNames[ i ] = speaker.pPropertyNames[ i ];
	}
	for( i=0; i<ET_COUNT; i++ ){
		pTriggerNames[ i ] = speaker.pTriggerNames[ i ];
	}
}

gdeOCSpeaker::~gdeOCSpeaker(){
}



// Management
///////////////

void gdeOCSpeaker::SetPathSound( const char *path ){
	pPathSound = path;
}

void gdeOCSpeaker::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCSpeaker::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCSpeaker::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void gdeOCSpeaker::SetLooping( bool looping ){
	pLooping = looping;
}

void gdeOCSpeaker::SetPlaying( bool playing ){
	pPlaying = playing;
}

void gdeOCSpeaker::SetVolume( float volume ){
	pVolume = decMath::max( volume, 0.0f );
}

void gdeOCSpeaker::SetRange( float range ){
	pRange = decMath::max( range, 0.0f );
}

void gdeOCSpeaker::SetRollOff( float rollOff ){
	pRollOff = decMath::max( rollOff, 0.0f );
}

void gdeOCSpeaker::SetDistanceOffset( float distanceOffset ){
	pDistanceOffset = decMath::max( distanceOffset, 0.0f );
}

void gdeOCSpeaker::SetPlaySpeed( float playSpeed ){
	pPlaySpeed = playSpeed;
}



bool gdeOCSpeaker::IsPropertySet( eProperties property ) const{
	if( property < 0 || property >= EP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCSpeaker::GetPropertyName( eProperties property ) const{
	if( property < 0 || property >= EP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	return pPropertyNames[ property ];
}

void gdeOCSpeaker::SetPropertyName( eProperties property, const char *name ){
	if( property < 0 || property >= EP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	pPropertyNames[ property ] = name;
}

bool gdeOCSpeaker::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<EP_COUNT; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	return false;
}



bool gdeOCSpeaker::IsTriggerSet( eTriggers trigger ) const{
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &gdeOCSpeaker::GetTriggerName( eTriggers trigger ) const{
	return pTriggerNames[ trigger ];
}

void gdeOCSpeaker::SetTriggerName( eTriggers trigger, const char *name ){
	pTriggerNames[ trigger ] = name;
}
