/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalSoundRaySegment.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRaySegment
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRaySegment::deoalSoundRaySegment() :
pDirection( 0.0f, 0.0f, 1.0f ),
pLength( 0.0f ),
pDistance( 0.0f ),
pGainLow( 0.0f ),
pGainMedium( 0.0f ),
pGainHigh( 0.0f ),
pAbsorptionSumLow( 0.0f ),
pAbsorptionSumMedium( 0.0f ),
pAbsorptionSumHigh( 0.0f ),
pBounceCount( 0 ),
pTransmittedCount( 0 ){
}

deoalSoundRaySegment::deoalSoundRaySegment( const deoalSoundRaySegment &segment ) :
pPosition( segment.pPosition ),
pDirection( segment.pDirection ),
pNormal( segment.pNormal ),
pLength( segment.pLength ),
pDistance( segment.pDistance ),
pGainLow( segment.pGainLow ),
pGainMedium( segment.pGainMedium ),
pGainHigh( segment.pGainHigh ),
pAbsorptionSumLow( segment.pAbsorptionSumLow ),
pAbsorptionSumMedium( segment.pAbsorptionSumMedium ),
pAbsorptionSumHigh( segment.pAbsorptionSumHigh ),
pBounceCount( segment.pBounceCount ),
pTransmittedCount( segment.pTransmittedCount ){
}



// Manegement
/////////////

void deoalSoundRaySegment::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoalSoundRaySegment::SetDirection( const decVector &direction ){
	pDirection = direction;
}

void deoalSoundRaySegment::SetNormal( const decVector &normal ){
	pNormal = normal;
}

void deoalSoundRaySegment::SetLength( float length ){
	pLength = length;
}

void deoalSoundRaySegment::SetDistance( float distance ){
	pDistance = distance;
}

void deoalSoundRaySegment::SetGainLow( float gain ){
	pGainLow = gain;
}

void deoalSoundRaySegment::SetGainMedium( float gain ){
	pGainMedium = gain;
}

void deoalSoundRaySegment::SetGainHigh( float gain ){
	pGainHigh = gain;
}

void deoalSoundRaySegment::SetAbsorptionSumLow( float absorption ){
	   pAbsorptionSumLow = absorption;
}

void deoalSoundRaySegment::SetAbsorptionSumMedium( float absorption ){
	   pAbsorptionSumMedium = absorption;
}

void deoalSoundRaySegment::SetAbsorptionSumHigh( float absorption ){
	   pAbsorptionSumHigh = absorption;
}

void deoalSoundRaySegment::SetBounceCount( int count ){
	pBounceCount = count;
}

void deoalSoundRaySegment::SetTransmittedCount( int count ){
	pTransmittedCount = count;
}



deoalSoundRaySegment &deoalSoundRaySegment::operator=( const deoalSoundRaySegment &segment ){
	pPosition = segment.pPosition;
	pDirection = segment.pDirection;
	pNormal = segment.pNormal;
	pLength = segment.pLength;
	pDistance = segment.pDistance;
	pGainLow = segment.pGainLow;
	pGainMedium = segment.pGainMedium;
	pGainHigh = segment.pGainHigh;
	pAbsorptionSumLow = segment.pAbsorptionSumLow;
	pAbsorptionSumMedium = segment.pAbsorptionSumMedium;
	pAbsorptionSumHigh = segment.pAbsorptionSumHigh;
	pBounceCount = segment.pBounceCount;
	pTransmittedCount = segment.pTransmittedCount;
	return *this;
}
