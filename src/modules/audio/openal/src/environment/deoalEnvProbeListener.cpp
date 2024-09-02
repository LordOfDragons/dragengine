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
