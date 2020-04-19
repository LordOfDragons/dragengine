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
#include "deoalEnvProbeListenerSmooth.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnvProbeListenerSmooth
//////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbeListenerSmooth::deoalEnvProbeListenerSmooth(){
	const float adjustTime = 0.5f;
	
	pReflectedLow.SetAdjustRange( 1.0f );
	pReflectedLow.SetAdjustTime( adjustTime );
	
	pReflectedMedium.SetAdjustRange( 1.0f );
	pReflectedMedium.SetAdjustTime( adjustTime );
	
	pReflectedHigh.SetAdjustRange( 1.0f );
	pReflectedHigh.SetAdjustTime( adjustTime );
	
	pReflectionDelay.SetAdjustRange( 0.1f );
	pReflectionDelay.SetAdjustTime( adjustTime );
	
	pReflectionPan.SetAdjustRange( 1.0f );
	pReflectionPan.SetAdjustTime( adjustTime );
	
	pReverberationGainLow.SetAdjustRange( 1.0f );
	pReverberationGainLow.SetAdjustTime( adjustTime );
	
	pReverberationGainMedium.SetAdjustRange( 1.0f );
	pReverberationGainMedium.SetAdjustTime( adjustTime );
	
	pReverberationGainHigh.SetAdjustRange( 1.0f );
	pReverberationGainHigh.SetAdjustTime( adjustTime );
	
	pReverberationDelay.SetAdjustRange( 0.1f );
	pReverberationDelay.SetAdjustTime( adjustTime );
	
	pReverberationPan.SetAdjustRange( 1.0f );
	pReverberationPan.SetAdjustTime( adjustTime );
	
	pReverberationTimeLow.SetAdjustRange( 20.0f );
	pReverberationTimeLow.SetAdjustTime( adjustTime );
	
	pReverberationTimeMedium.SetAdjustRange( 20.0f );
	pReverberationTimeMedium.SetAdjustTime( adjustTime );
	
	pReverberationTimeHigh.SetAdjustRange( 20.0f );
	pReverberationTimeHigh.SetAdjustTime( adjustTime );
	
	pEchoDelay.SetAdjustRange( 0.2f );
	pEchoDelay.SetAdjustTime( adjustTime );
}

deoalEnvProbeListenerSmooth::deoalEnvProbeListenerSmooth( const deoalEnvProbeListenerSmooth &listener ) :
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
pEchoDelay( listener.pEchoDelay ){
}

deoalEnvProbeListenerSmooth::~deoalEnvProbeListenerSmooth(){
}



// Manegement
///////////////

void deoalEnvProbeListenerSmooth::Set( const deoalEnvProbeListener &listener ){
	SetGoal( listener );
	
	pReflectedLow.SetValue( listener.GetReflectedLow() );
	pReflectedMedium.SetValue( listener.GetReflectedMedium() );
	pReflectedHigh.SetValue( listener.GetReflectedHigh() );
	pReflectionDelay.SetValue( listener.GetReflectionDelay() );
	pReflectionPan.SetValue( listener.GetReflectionPan() );
	pReverberationGainLow.SetValue( listener.GetReverberationGainLow() );
	pReverberationGainMedium.SetValue( listener.GetReverberationGainMedium() );
	pReverberationGainHigh.SetValue( listener.GetReverberationGainHigh() );
	pReverberationDelay.SetValue( listener.GetReverberationDelay() );
	pReverberationPan.SetValue( listener.GetReverberationPan() );
	pReverberationTimeMedium.SetValue( listener.GetReverberationTimeMedium() );
	pReverberationTimeLow.SetValue( listener.GetReverberationTimeLow() );
	pReverberationTimeHigh.SetValue( listener.GetReverberationTimeHigh() );
	pEchoDelay.SetValue( listener.GetEchoDelay() );
}

void deoalEnvProbeListenerSmooth::SetGoal( const deoalEnvProbeListener &listener ){
	pReflectedLow.SetGoal( listener.GetReflectedLow() );
	pReflectedMedium.SetGoal( listener.GetReflectedMedium() );
	pReflectedHigh.SetGoal( listener.GetReflectedHigh() );
	pReflectionDelay.SetGoal( listener.GetReflectionDelay() );
	pReflectionPan.SetGoal( listener.GetReflectionPan() );
	pReverberationGainLow.SetGoal( listener.GetReverberationGainLow() );
	pReverberationGainMedium.SetGoal( listener.GetReverberationGainMedium() );
	pReverberationGainHigh.SetGoal( listener.GetReverberationGainHigh() );
	pReverberationDelay.SetGoal( listener.GetReverberationDelay() );
	pReverberationPan.SetGoal( listener.GetReverberationPan() );
	pReverberationTimeLow.SetGoal( listener.GetReverberationTimeLow() );
	pReverberationTimeMedium.SetGoal( listener.GetReverberationTimeMedium() );
	pReverberationTimeHigh.SetGoal( listener.GetReverberationTimeHigh() );
	pEchoDelay.SetGoal( listener.GetEchoDelay() );
}

void deoalEnvProbeListenerSmooth::Update( float elapsed ){
	pReflectedLow.Update( elapsed );
	pReflectedMedium.Update( elapsed );
	pReflectedHigh.Update( elapsed );
	pReflectionDelay.Update( elapsed );
	pReflectionPan.Update( elapsed );
	pReverberationGainLow.Update( elapsed );
	pReverberationGainMedium.Update( elapsed );
	pReverberationGainHigh.Update( elapsed );
	pReverberationDelay.Update( elapsed );
	pReverberationPan.Update( elapsed );
	pReverberationTimeLow.Update( elapsed );
	pReverberationTimeMedium.Update( elapsed );
	pReverberationTimeHigh.Update( elapsed );
	pEchoDelay.Update( elapsed );
}

void deoalEnvProbeListenerSmooth::AssignTo( deoalEnvProbeListener &listener ){
	listener.SetReflected( pReflectedLow.GetValue(), pReflectedMedium.GetValue(),
		pReflectedHigh.GetValue(), pReflectionDelay.GetValue() );
	listener.SetReflectionPan( pReflectionPan.GetValue() );
	listener.SetReverberation( pReverberationGainLow.GetValue(),
		pReverberationGainMedium.GetValue(), pReverberationGainHigh.GetValue(),
		pReverberationDelay.GetValue() );
	listener.SetReverberationPan( pReverberationPan.GetValue() );
	listener.SetReverberationTime( pReverberationTimeLow.GetValue(),
		pReverberationTimeMedium.GetValue(), pReverberationTimeHigh.GetValue() );
	listener.SetEcho( pEchoDelay.GetValue() );
}



// Operators
//////////////

deoalEnvProbeListenerSmooth &deoalEnvProbeListenerSmooth::operator=(
const deoalEnvProbeListenerSmooth &listener ){
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
	
	return *this;
}
