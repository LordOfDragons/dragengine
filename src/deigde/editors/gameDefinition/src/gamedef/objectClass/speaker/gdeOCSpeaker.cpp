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
