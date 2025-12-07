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

#include "ceUCAASpeakGestureMove.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakGestureMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakGestureMove::ceUCAASpeakGestureMove(ceConversationTopic *topic, ceCAActorSpeak *actorSpeak,
ceStrip *gesture, int newIndex){
	if(! topic || ! actorSpeak || ! gesture){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pGesture = NULL;
	pNewIndex = newIndex;
	pOldIndex = actorSpeak->GetGestureList().IndexOf(gesture);
	
	if(pOldIndex == -1) DETHROW(deeInvalidParam);
	if(pNewIndex < 0 || pNewIndex >= actorSpeak->GetGestureList().GetCount()) DETHROW(deeInvalidParam);
	if(pNewIndex == pOldIndex) DETHROW(deeInvalidParam);
	
	SetShortInfo("Move Gesture");
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pGesture = gesture;
	gesture->AddReference();
}

ceUCAASpeakGestureMove::~ceUCAASpeakGestureMove(){
	if(pGesture){
		pGesture->FreeReference();
	}
	if(pActorSpeak){
		pActorSpeak->FreeReference();
	}
	if(pTopic){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakGestureMove::Undo(){
	pActorSpeak->GetGestureList().MoveTo(pGesture, pOldIndex);
	pTopic->NotifyActionChanged(pActorSpeak);
}

void ceUCAASpeakGestureMove::Redo(){
	pActorSpeak->GetGestureList().MoveTo(pGesture, pNewIndex);
	pTopic->NotifyActionChanged(pActorSpeak);
}
