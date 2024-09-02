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

#include "ceUCAASpeakEyesLASet.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakEyesLASet
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakEyesLASet::ceUCAASpeakEyesLASet( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *eyesLookAt, ceStrip *newStrip ){
	if( ! topic || ! actorSpeak || ! eyesLookAt || ! newStrip ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pEyesLA = NULL;
	pOldStrip = NULL;
	pNewStrip = NULL;
	
	SetShortInfo( "Set eyes look-at" );
	
	pOldStrip = new ceStrip( *eyesLookAt );
	
	pNewStrip = newStrip;
	newStrip->AddReference();
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pEyesLA = eyesLookAt;
	eyesLookAt->AddReference();
}

ceUCAASpeakEyesLASet::~ceUCAASpeakEyesLASet(){
	if( pNewStrip ){
		pNewStrip->FreeReference();
	}
	if( pOldStrip ){
		pOldStrip->FreeReference();
	}
	if( pEyesLA ){
		pEyesLA->FreeReference();
	}
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakEyesLASet::Undo(){
	*pEyesLA = *pOldStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakEyesLASet::Redo(){
	*pEyesLA = *pNewStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}
