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
#include <string.h>
#include <stdlib.h>

#include "ceUCAASpeakStripsScale.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakStripsScale
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakStripsScale::ceUCAASpeakStripsScale(ceConversationTopic *topic, ceCAActorSpeak *actorSpeak){
	if(!topic || !actorSpeak){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pOldStates = NULL;
	
	SetShortInfo("Scale strips");
	
	pTopic = topic;
	pActorSpeak = actorSpeak;
}

ceUCAASpeakStripsScale::~ceUCAASpeakStripsScale(){
	if(pOldStates){
		delete [] pOldStates;
	}
}



// Management
///////////////

void ceUCAASpeakStripsScale::SetStrips(const ceStripList &strips){
	if(pOldStates){
		delete [] pOldStates;
		pOldStates = NULL;
	}
	
	pStrips = strips;
	
	const int count = strips.GetCount();
	if(count > 0){
		int i;
		
		pOldStates = new sStrip[count];
		
		for(i=0; i<count; i++){
			const ceStrip &strip = *strips.GetAt(i);
			pOldStates[i].pause = strip.GetPause();
			pOldStates[i].duration = strip.GetDuration();
		}
	}
}

void ceUCAASpeakStripsScale::SetScaling(float scaling){
	if(scaling < 0.0f){
		pScaling = 0.0f;
		
	}else{
		pScaling = scaling;
	}
}



void ceUCAASpeakStripsScale::Undo(){
	const int count = pStrips.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		ceStrip &strip = *pStrips.GetAt(i);
		strip.SetPause(pOldStates[i].pause);
		strip.SetDuration(pOldStates[i].duration);
	}
	
	pTopic->NotifyActionChanged(pActorSpeak);
}

void ceUCAASpeakStripsScale::Redo(){
	const int count = pStrips.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		ceStrip &strip = *pStrips.GetAt(i);
		strip.SetPause(pOldStates[i].pause * pScaling);
		strip.SetDuration(pOldStates[i].duration * pScaling);
	}
	
	pTopic->NotifyActionChanged(pActorSpeak);
}
