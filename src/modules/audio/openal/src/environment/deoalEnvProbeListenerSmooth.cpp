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
#include "deoalEnvProbeListenerSmooth.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnvProbeListenerSmooth
//////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbeListenerSmooth::deoalEnvProbeListenerSmooth(){
	const float adjustTime = 0.5f;
	
	pReflectedLow.SetAdjustRange(1.0f);
	pReflectedLow.SetAdjustTime(adjustTime);
	
	pReflectedMedium.SetAdjustRange(1.0f);
	pReflectedMedium.SetAdjustTime(adjustTime);
	
	pReflectedHigh.SetAdjustRange(1.0f);
	pReflectedHigh.SetAdjustTime(adjustTime);
	
	pReflectionDelay.SetAdjustRange(0.1f);
	pReflectionDelay.SetAdjustTime(adjustTime);
	
	pReflectionPan.SetAdjustRange(1.0f);
	pReflectionPan.SetAdjustTime(adjustTime);
	
	pReverberationGainLow.SetAdjustRange(1.0f);
	pReverberationGainLow.SetAdjustTime(adjustTime);
	
	pReverberationGainMedium.SetAdjustRange(1.0f);
	pReverberationGainMedium.SetAdjustTime(adjustTime);
	
	pReverberationGainHigh.SetAdjustRange(1.0f);
	pReverberationGainHigh.SetAdjustTime(adjustTime);
	
	pReverberationDelay.SetAdjustRange(0.1f);
	pReverberationDelay.SetAdjustTime(adjustTime);
	
	pReverberationPan.SetAdjustRange(1.0f);
	pReverberationPan.SetAdjustTime(adjustTime);
	
	pReverberationTimeLow.SetAdjustRange(20.0f);
	pReverberationTimeLow.SetAdjustTime(adjustTime);
	
	pReverberationTimeMedium.SetAdjustRange(20.0f);
	pReverberationTimeMedium.SetAdjustTime(adjustTime);
	
	pReverberationTimeHigh.SetAdjustRange(20.0f);
	pReverberationTimeHigh.SetAdjustTime(adjustTime);
	
	pEchoDelay.SetAdjustRange(0.2f);
	pEchoDelay.SetAdjustTime(adjustTime);
}

deoalEnvProbeListenerSmooth::deoalEnvProbeListenerSmooth(const deoalEnvProbeListenerSmooth &listener) :
pReflectedLow(listener.pReflectedLow),
pReflectedMedium(listener.pReflectedMedium),
pReflectedHigh(listener.pReflectedHigh),
pReflectionDelay(listener.pReflectionDelay),
pReflectionPan(listener.pReflectionPan),
pReverberationGainLow(listener.pReverberationGainLow),
pReverberationGainMedium(listener.pReverberationGainMedium),
pReverberationGainHigh(listener.pReverberationGainHigh),
pReverberationDelay(listener.pReverberationDelay),
pReverberationPan(listener.pReverberationPan),

pReverberationTimeLow(listener.pReverberationTimeLow),
pReverberationTimeMedium(listener.pReverberationTimeMedium),
pReverberationTimeHigh(listener.pReverberationTimeHigh),
pEchoDelay(listener.pEchoDelay){
}

deoalEnvProbeListenerSmooth::~deoalEnvProbeListenerSmooth(){
}



// Manegement
///////////////

void deoalEnvProbeListenerSmooth::Set(const deoalEnvProbeListener &listener){
	SetGoal(listener);
	
	pReflectedLow.SetValue(listener.GetReflectedLow());
	pReflectedMedium.SetValue(listener.GetReflectedMedium());
	pReflectedHigh.SetValue(listener.GetReflectedHigh());
	pReflectionDelay.SetValue(listener.GetReflectionDelay());
	pReflectionPan.SetValue(listener.GetReflectionPan());
	pReverberationGainLow.SetValue(listener.GetReverberationGainLow());
	pReverberationGainMedium.SetValue(listener.GetReverberationGainMedium());
	pReverberationGainHigh.SetValue(listener.GetReverberationGainHigh());
	pReverberationDelay.SetValue(listener.GetReverberationDelay());
	pReverberationPan.SetValue(listener.GetReverberationPan());
	pReverberationTimeMedium.SetValue(listener.GetReverberationTimeMedium());
	pReverberationTimeLow.SetValue(listener.GetReverberationTimeLow());
	pReverberationTimeHigh.SetValue(listener.GetReverberationTimeHigh());
	pEchoDelay.SetValue(listener.GetEchoDelay());
}

void deoalEnvProbeListenerSmooth::SetGoal(const deoalEnvProbeListener &listener){
	pReflectedLow.SetGoal(listener.GetReflectedLow());
	pReflectedMedium.SetGoal(listener.GetReflectedMedium());
	pReflectedHigh.SetGoal(listener.GetReflectedHigh());
	pReflectionDelay.SetGoal(listener.GetReflectionDelay());
	pReflectionPan.SetGoal(listener.GetReflectionPan());
	pReverberationGainLow.SetGoal(listener.GetReverberationGainLow());
	pReverberationGainMedium.SetGoal(listener.GetReverberationGainMedium());
	pReverberationGainHigh.SetGoal(listener.GetReverberationGainHigh());
	pReverberationDelay.SetGoal(listener.GetReverberationDelay());
	pReverberationPan.SetGoal(listener.GetReverberationPan());
	pReverberationTimeLow.SetGoal(listener.GetReverberationTimeLow());
	pReverberationTimeMedium.SetGoal(listener.GetReverberationTimeMedium());
	pReverberationTimeHigh.SetGoal(listener.GetReverberationTimeHigh());
	pEchoDelay.SetGoal(listener.GetEchoDelay());
}

void deoalEnvProbeListenerSmooth::Update(float elapsed){
	pReflectedLow.Update(elapsed);
	pReflectedMedium.Update(elapsed);
	pReflectedHigh.Update(elapsed);
	pReflectionDelay.Update(elapsed);
	pReflectionPan.Update(elapsed);
	pReverberationGainLow.Update(elapsed);
	pReverberationGainMedium.Update(elapsed);
	pReverberationGainHigh.Update(elapsed);
	pReverberationDelay.Update(elapsed);
	pReverberationPan.Update(elapsed);
	pReverberationTimeLow.Update(elapsed);
	pReverberationTimeMedium.Update(elapsed);
	pReverberationTimeHigh.Update(elapsed);
	pEchoDelay.Update(elapsed);
}

void deoalEnvProbeListenerSmooth::AssignTo(deoalEnvProbeListener &listener){
	listener.SetReflected(pReflectedLow.GetValue(), pReflectedMedium.GetValue(),
		pReflectedHigh.GetValue(), pReflectionDelay.GetValue());
	listener.SetReflectionPan(pReflectionPan.GetValue());
	listener.SetReverberation(pReverberationGainLow.GetValue(),
		pReverberationGainMedium.GetValue(), pReverberationGainHigh.GetValue(),
		pReverberationDelay.GetValue());
	listener.SetReverberationPan(pReverberationPan.GetValue());
	listener.SetReverberationTime(pReverberationTimeLow.GetValue(),
		pReverberationTimeMedium.GetValue(), pReverberationTimeHigh.GetValue());
	listener.SetEcho(pEchoDelay.GetValue());
}



// Operators
//////////////

deoalEnvProbeListenerSmooth &deoalEnvProbeListenerSmooth::operator=(
const deoalEnvProbeListenerSmooth &listener){
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
