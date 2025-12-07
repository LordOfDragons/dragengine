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

#include "deoalEnvProbeLevel.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnvProbeLevel
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbeLevel::deoalEnvProbeLevel() :
pRayCount(0),
pRayOpeningAngle(0.0f),
pReverberationTimeMedium(0.0f),
pReverberationTimeLow(0.0f),
pReverberationTimeHigh(0.0f),
pEchoDelay(0.0f),
pRoomVolume(0.0f),
pRoomSurface(0.0f),
pRoomSabineLow(0.0f),
pRoomSabineMedium(0.0f),
pRoomSabineHigh(0.0f){
}

deoalEnvProbeLevel::~deoalEnvProbeLevel(){
}



// Manegement
///////////////

void deoalEnvProbeLevel::SetRayCount(int rayCount){
	pRayCount = rayCount;
}

void deoalEnvProbeLevel::SetRayOpeningAngle(float angle){
	pRayOpeningAngle = angle;
}

void deoalEnvProbeLevel::SetReverberationTime(float timeLow, float timeMedium, float timeHigh){
	pReverberationTimeLow = timeLow;
	pReverberationTimeMedium = timeMedium;
	pReverberationTimeHigh = timeHigh;
}

void deoalEnvProbeLevel::SetEcho(float delay){
	pEchoDelay = delay;
}

void deoalEnvProbeLevel::SetRoom(float volume, float surface, float sabineLow,
float sabineMedium, float sabineHigh){
	pRoomVolume = volume;
	pRoomSurface = surface;
	pRoomSabineLow = sabineLow;
	pRoomSabineMedium = sabineMedium;
	pRoomSabineHigh = sabineHigh;
}
