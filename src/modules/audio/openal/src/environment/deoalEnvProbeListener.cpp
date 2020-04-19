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

#include "deoalEnvProbeListener.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnvProbeListener
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbeListener::deoalEnvProbeListener() :
pReflectedLow( 0.0f ),
pReflectedMedium( 0.0f ),
pReflectedHigh( 0.0f ),
pReflectionDelay( 0.0f ),
pReverberationGainLow( 0.0f ),
pReverberationGainMedium( 0.0f ),
pReverberationGainHigh( 0.0f ),
pReverberationDelay( 0.0f ),

pReverberationTimeLow( 0.0f ),
pReverberationTimeMedium( 0.0f ),
pReverberationTimeHigh( 0.0f ),
pEchoDelay( 0.0f ){
}

deoalEnvProbeListener::deoalEnvProbeListener( const deoalEnvProbeListener &listener ) :
pReflectedLow( listener.pReflectedLow ),
pReflectedMedium( listener.pReflectedMedium ),
pReflectedHigh( listener.pReflectedHigh ),
pReflectionDelay( listener.pReflectionDelay ),
pReflectionPan( listener.pReflectionPan ),
pReverberationGainLow( listener.pReverberationGainLow ),
pReverberationGainMedium( listener.pReverberationGainMedium ),
pReverberationGainHigh( listener.pReverberationGainHigh ),
pReverberationDelay( listener.pReverberationDelay ),
pReverberationPan( listener.pReverberationPan ),

pReverberationTimeLow( listener.pReverberationTimeLow ),
pReverberationTimeMedium( listener.pReverberationTimeMedium ),
pReverberationTimeHigh( listener.pReverberationTimeHigh ),
pEchoDelay( listener.pEchoDelay ),

pImpulseResponse( listener.pImpulseResponse ) /*,
pEnergyHistogram( listener.pEnergyHistogram ) */{
}

deoalEnvProbeListener::~deoalEnvProbeListener(){
}



// Manegement
///////////////

void deoalEnvProbeListener::SetReflected( float gainLow, float gainMedium, float gainHigh, float delay ){
	pReflectedLow = gainLow;
	pReflectedMedium = gainMedium;
	pReflectedHigh = gainHigh;
	pReflectionDelay = delay;
}

void deoalEnvProbeListener::SetReflectionPan( const decVector &pan ){
	pReflectionPan = pan;
}

void deoalEnvProbeListener::SetReverberation( float gainLow, float gainMedium, float gainHigh, float delay ){
	pReverberationGainLow = gainLow;
	pReverberationGainMedium = gainMedium;
	pReverberationGainHigh = gainHigh;
	pReverberationDelay = delay;
}

void deoalEnvProbeListener::SetReverberationPan( const decVector &pan ){
	pReverberationPan = pan;
}

void deoalEnvProbeListener::SetReverberationTime( float timeLow, float timeMedium, float timeHigh ){
	pReverberationTimeLow = timeLow;
	pReverberationTimeMedium = timeMedium;
	pReverberationTimeHigh = timeHigh;
}

void deoalEnvProbeListener::SetEcho( float delay ){
	pEchoDelay = delay;
}



void deoalEnvProbeListener::AddScaled( const deoalEnvProbeListener &listener, float factor ){
	pReflectedLow += listener.pReflectedLow * factor;
	pReflectedMedium += listener.pReflectedMedium * factor;
	pReflectedHigh += listener.pReflectedHigh * factor;
	pReflectionDelay += listener.pReflectionDelay * factor;
	pReflectionPan += listener.pReflectionPan * factor;
	pReverberationGainLow += listener.pReverberationGainLow * factor;
	pReverberationGainMedium += listener.pReverberationGainMedium * factor;
	pReverberationGainHigh += listener.pReverberationGainHigh * factor;
	pReverberationDelay += listener.pReverberationDelay * factor;
	pReverberationPan += listener.pReverberationPan * factor;
	
	pReverberationTimeMedium += listener.pReverberationTimeMedium * factor;
	pReverberationTimeLow += listener.pReverberationTimeLow * factor;
	pReverberationTimeHigh += listener.pReverberationTimeHigh * factor;
	pEchoDelay += listener.pEchoDelay * factor;
	
	//pEnergyHistogram.BlendAddScaled( listener.pEnergyHistogram, factor );
}

void deoalEnvProbeListener::Normalize( float factor ){
	pReflectedLow *= factor;
	pReflectedMedium *= factor;
	pReflectedHigh *= factor;
	pReflectionDelay *= factor;
	pReflectionPan *= factor;
	pReverberationGainLow *= factor;
	pReverberationGainMedium *= factor;
	pReverberationGainHigh *= factor;
	pReverberationDelay *= factor;
	pReverberationPan *= factor;
	
	pReverberationTimeMedium *= factor;
	pReverberationTimeLow *= factor;
	pReverberationTimeHigh *= factor;
	pEchoDelay *= factor;
	
	//pEnergyHistogram *= factor;
}

void deoalEnvProbeListener::Clear(){
	pReflectedLow = 0.0f;
	pReflectedMedium = 0.0f;
	pReflectedHigh = 0.0f;
	pReflectionDelay = 0.0f;
	pReflectionPan.SetZero();
	pReverberationGainLow = 0.0f;
	pReverberationGainMedium = 0.0f;
	pReverberationGainHigh = 0.0f;
	pReverberationDelay = 0.0f;
	pReverberationPan.SetZero();
	
	pReverberationTimeMedium = 0.0f;
	pReverberationTimeLow = 0.0f;
	pReverberationTimeHigh = 0.0f;
	pEchoDelay = 0.0f;
	
	pImpulseResponse.Clear();
	//pEnergyHistogram.Clear();
}



// Operators
//////////////

deoalEnvProbeListener &deoalEnvProbeListener::operator=( const deoalEnvProbeListener &listener ){
	pReflectedLow = listener.pReflectedLow;
	pReflectedMedium = listener.pReflectedMedium;
	pReflectedHigh = listener.pReflectedHigh;
	pReflectionDelay = listener.pReflectionDelay;
	pReflectionPan = listener.pReflectionPan;
	pReverberationGainLow = listener.pReverberationGainLow;
	pReverberationGainMedium = listener.pReverberationGainMedium;
	pReverberationGainHigh = listener.pReverberationGainHigh;
	pReverberationDelay = listener.pReverberationDelay;
	pReverberationPan = listener.pReverberationPan;
	
	pReverberationTimeMedium = listener.pReverberationTimeMedium;
	pReverberationTimeLow = listener.pReverberationTimeLow;
	pReverberationTimeHigh = listener.pReverberationTimeHigh;
	pEchoDelay = listener.pEchoDelay;
	
	pImpulseResponse = listener.pImpulseResponse;
	//pEnergyHistogram = listener.pEnergyHistogram;
	
	return *this;
}

deoalEnvProbeListener &deoalEnvProbeListener::operator+=( const deoalEnvProbeListener &listener ){
	pReflectedLow += listener.pReflectedLow;
	pReflectedMedium += listener.pReflectedMedium;
	pReflectedHigh += listener.pReflectedHigh;
	pReflectionDelay += listener.pReflectionDelay;
	pReflectionPan += listener.pReflectionPan;
	pReverberationGainLow += listener.pReverberationGainLow;
	pReverberationGainMedium += listener.pReverberationGainMedium;
	pReverberationGainHigh += listener.pReverberationGainHigh;
	pReverberationDelay += listener.pReverberationDelay;
	pReverberationPan += listener.pReverberationPan;
	
	pReverberationTimeMedium += listener.pReverberationTimeMedium;
	pReverberationTimeLow += listener.pReverberationTimeLow;
	pReverberationTimeHigh += listener.pReverberationTimeHigh;
	pEchoDelay += listener.pEchoDelay;
	
	//pEnergyHistogram.Add( listener.pEnergyHistogram );
	
	return *this;
}
