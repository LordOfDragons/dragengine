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

deoalSpeakerList::deoalSpeakerList(){
}

deoalSpeakerList::~deoalSpeakerList(){
}



// Management
///////////////

int deoalSpeakerList::GetCount() const{
	return pSpeakers.GetCount();
}

deoalASpeaker *deoalSpeakerList::GetAt(int index) const{
	return (deoalASpeaker*)pSpeakers.GetAt(index);
}

bool deoalSpeakerList::Has(deoalASpeaker *speaker) const{
	return pSpeakers.Has(speaker);
}

int deoalSpeakerList::IndexOf(deoalASpeaker *speaker) const{
	return pSpeakers.IndexOf(speaker);
}

void deoalSpeakerList::Add(deoalASpeaker *speaker){
	pSpeakers.Add(speaker);
}

void deoalSpeakerList::Remove(deoalASpeaker *speaker){
	pSpeakers.RemoveFrom(pSpeakers.IndexOf(speaker));
}

void deoalSpeakerList::RemoveIfExisting(deoalASpeaker *speaker){
	const int index = pSpeakers.IndexOf(speaker);
	if(index != -1){
		pSpeakers.RemoveFrom(index);
	}
}

void deoalSpeakerList::RemoveAll(){
	pSpeakers.RemoveAll();
}

void deoalSpeakerList::FlagAll(bool flag){
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->SetFlag(flag);
	}
}

void deoalSpeakerList::EnableAll(bool enabled){
	// WARNING Called during synchronization time from main thread.
	
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->SetEnabled(enabled);
	}
}

void deoalSpeakerList::FlagToEnabledAll(){
	// WARNING Called during synchronization time from main thread.
	
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalASpeaker &speaker = *((deoalASpeaker*)pSpeakers.GetAt(i));
		speaker.SetEnabled(speaker.GetFlag());
	}
}

void deoalSpeakerList::UpdateAll(){
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->PrepareProcessAudio();
	}
}

void deoalSpeakerList::UpdateEffectsAll(){
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->UpdateEffects();
	}
}



// Operators
//////////////

deoalSpeakerList &deoalSpeakerList::operator=(const deoalSpeakerList &list){
	pSpeakers = list.pSpeakers;
	return *this;
}

deoalSpeakerList &deoalSpeakerList::operator+=(const deoalSpeakerList &list){
	pSpeakers += list.pSpeakers;
	return *this;
}

deoalSpeakerList deoalSpeakerList::operator+(const deoalSpeakerList &list) const{
	deoalSpeakerList combined;
	combined.pSpeakers = pSpeakers + list.pSpeakers;
	return combined;
}

bool deoalSpeakerList::operator==(const deoalSpeakerList &list) const{
	const int count = pSpeakers.GetCount();
	if(count != list.pSpeakers.GetCount()){
		return false;
	}
	
	int i;
	for(i=0; i<count; i++){
		if(! list.pSpeakers.Has(pSpeakers.GetAt(i))){
			return false;
		}
	}
	
	return true;
}

bool deoalSpeakerList::operator!=(const deoalSpeakerList &list) const{
	return ! operator==(list);
}
