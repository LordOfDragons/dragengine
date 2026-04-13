/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoalPAudioCaptureVolume.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>


// Class deoalPAudioCaptureVolume
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPAudioCaptureVolume::deoalPAudioCaptureVolume(deAudioOpenAL &oal) :
deoalParameterFloat(oal)
{
	SetName("audioCaptureVolume");
	SetDescription("Scale volume of captured audio samples."
		" Values above 1 amplify the volume (quiet microphones)."
		" Values below 1 attenuate the volume (loud microphones).");
	SetType(deModuleParameter::eptRanged);
	SetMinimumValue(0.0f);
	SetMaximumValue(1.5f);
	SetValueStepSize(0.01f);
	
	SetCategory(ecBasic);
	SetDisplayName("Audio Capture Volume");
	SetDefaultValue("1");
}

deoalPAudioCaptureVolume::~deoalPAudioCaptureVolume() = default;


// Management
///////////////

float deoalPAudioCaptureVolume::GetParameterFloat(){
	return pOal.GetConfiguration().GetAudioCaptureVolume();
}

void deoalPAudioCaptureVolume::SetParameterFloat(float value){
	pOal.GetConfiguration().SetAudioCaptureVolume(value);
}
