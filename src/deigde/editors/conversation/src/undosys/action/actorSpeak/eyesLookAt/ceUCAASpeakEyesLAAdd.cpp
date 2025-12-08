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

#include "ceUCAASpeakEyesLAAdd.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakEyesLAAdd
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakEyesLAAdd::ceUCAASpeakEyesLAAdd(ceConversationTopic::Ref topic, ceCAActorSpeak::Ref actorSpeak, ceStrip::Ref eyesLookAt, int index){
	if(!topic || !actorSpeak || !eyesLookAt){
		DETHROW(deeInvalidParam);
	}
	if(index < 0 || index > actorSpeak->GetEyesLookAtList().GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pEyesLA = NULL;
	pIndex = index;
	
	SetShortInfo("Actor Speak Add EyesLookAt");
	
	pTopic = topic;
	pActorSpeak = actorSpeak;
	pEyesLA = eyesLookAt;
}

ceUCAASpeakEyesLAAdd::~ceUCAASpeakEyesLAAdd(){
}



// Management
///////////////

void ceUCAASpeakEyesLAAdd::Undo(){
	pActorSpeak->GetEyesLookAtList().Remove(pEyesLA);
	pTopic->NotifyActionChanged(pActorSpeak);
}

void ceUCAASpeakEyesLAAdd::Redo(){
	pActorSpeak->GetEyesLookAtList().InsertAt(pEyesLA, pIndex);
	pTopic->NotifyActionChanged(pActorSpeak);
}
