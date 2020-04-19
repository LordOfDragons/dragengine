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

#include "deoalEnvProbeLevel.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnvProbeLevel
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbeLevel::deoalEnvProbeLevel() :
pRayCount( 0 ),
pRayOpeningAngle( 0.0f ),
pReverberationTimeMedium( 0.0f ),
pReverberationTimeLow( 0.0f ),
pReverberationTimeHigh( 0.0f ),
pEchoDelay( 0.0f ),
pRoomVolume( 0.0f ),
pRoomSurface( 0.0f ),
pRoomSabineLow( 0.0f ),
pRoomSabineMedium( 0.0f ),
pRoomSabineHigh( 0.0f ){
}

deoalEnvProbeLevel::~deoalEnvProbeLevel(){
}



// Manegement
///////////////

void deoalEnvProbeLevel::SetRayCount( int rayCount ){
	pRayCount = rayCount;
}

void deoalEnvProbeLevel::SetRayOpeningAngle( float angle ){
	pRayOpeningAngle = angle;
}

void deoalEnvProbeLevel::SetReverberationTime( float timeLow, float timeMedium, float timeHigh ){
	pReverberationTimeLow = timeLow;
	pReverberationTimeMedium = timeMedium;
	pReverberationTimeHigh = timeHigh;
}

void deoalEnvProbeLevel::SetEcho( float delay ){
	pEchoDelay = delay;
}

void deoalEnvProbeLevel::SetRoom(float volume, float surface, float sabineLow,
float sabineMedium, float sabineHigh ){
	pRoomVolume = volume;
	pRoomSurface = surface;
	pRoomSabineLow = sabineLow;
	pRoomSabineMedium = sabineMedium;
	pRoomSabineHigh = sabineHigh;
}
