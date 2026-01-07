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

#include "deoalASpeaker.h"
#include "deoalSpeakerList.h"
#include "../microphone/deoalAMicrophone.h"
#include "../world/deoalAWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/world/deWorld.h>


// Class deoalSpeakerList
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalSpeakerList::deoalSpeakerList(const decTOrderedSet<deoalASpeaker*> &set) :
decTOrderedSet<deoalASpeaker*>(set){
}


// Management
///////////////

void deoalSpeakerList::FlagAll(bool flag){
	Visit([&](deoalASpeaker *speaker){
		speaker->SetFlag(flag);
	});
}

void deoalSpeakerList::EnableAll(bool enabled){
	// WARNING Called during synchronization time from main thread.
	
	Visit([&](deoalASpeaker *speaker){
		speaker->SetEnabled(enabled);
	});
}

void deoalSpeakerList::FlagToEnabledAll(){
	// WARNING Called during synchronization time from main thread.
	
	Visit([&](deoalASpeaker *speaker){
		speaker->SetEnabled(speaker->GetFlag());
	});
}

void deoalSpeakerList::UpdateAll(){
	Visit([&](deoalASpeaker *speaker){
		speaker->PrepareProcessAudio();
	});
}

void deoalSpeakerList::UpdateEffectsAll(){
	Visit([&](deoalASpeaker *speaker){
		speaker->UpdateEffects();
	});
}
