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

#include "ceCAActorSpeak.h"
#include "../strip/ceStrip.h"
#include "../ceConversation.h"
#include "../../langpack/ceLangPack.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceCAActorSpeak
/////////////////////////

// Constructor, destructor
////////////////////////////

ceCAActorSpeak::ceCAActorSpeak(deEngine *engine) : ceConversationAction(eatActorSpeak){
	if(!engine){
		DETHROW(deeInvalidParam);
	}
	
	pEngine = engine;
	pEngSound = nullptr;
	pLoaded = true;
	
	pMinSpeechTime = 0.0f;
	pUseSpeechAnimation = true;
}

ceCAActorSpeak::ceCAActorSpeak(const ceCAActorSpeak &action) : ceConversationAction(action){
	pEngine = action.GetEngine();
	pEngSound = nullptr;
	pLoaded = true;
	
	pActor = action.pActor;
	pTextBoxText = action.pTextBoxText;
	pTextBoxTextTranslate = action.pTextBoxTextTranslate;
	pTextBoxTextStyle = action.pTextBoxTextStyle;
	pPathSound = action.pPathSound;
	pMovement = action.pMovement;
	action.pWords.Visit([&](const ceStrip &s){ pWords.Add(ceStrip::Ref::New(s)); });
	action.pFacePoses.Visit([&](const ceStrip &s){ pFacePoses.Add(ceStrip::Ref::New(s)); });
	action.pGestures.Visit([&](const ceStrip &s){ pGestures.Add(ceStrip::Ref::New(s)); });
	action.pBodyLookAt.Visit([&](const ceStrip &s){ pBodyLookAt.Add(ceStrip::Ref::New(s)); });
	action.pHeadLookAt.Visit([&](const ceStrip &s){ pHeadLookAt.Add(ceStrip::Ref::New(s)); });
	action.pEyesLookAt.Visit([&](const ceStrip &s){ pEyesLookAt.Add(ceStrip::Ref::New(s)); });
	pMinSpeechTime = action.pMinSpeechTime;
	pUseSpeechAnimation = action.pUseSpeechAnimation;
	
	pEngSound = action.pEngSound;
	pLoaded = action.pLoaded;
}

ceCAActorSpeak::~ceCAActorSpeak(){
}



// Management
///////////////

const deSound::Ref &ceCAActorSpeak::GetEngineSound(){
	if(!pLoaded){
		pLoadSound();
	}
	
	return pEngSound;
}



void ceCAActorSpeak::SetActor(const char *id){
	pActor = id;
}

void ceCAActorSpeak::SetTextBoxText(const decUnicodeString &text){
	pTextBoxText = text;
}

void ceCAActorSpeak::SetTextBoxTextTranslate(const char *text){
	pTextBoxTextTranslate = text;
}

decUnicodeString ceCAActorSpeak::ResolveTextBoxText(const ceConversation &conversation) const{
	if(!pTextBoxTextTranslate.IsEmpty()){
		const ceLangPack * const langpack = conversation.GetLanguagePack();
		if(langpack){
			const decUnicodeString *text = nullptr;
			if(langpack->GetEntries().GetAt(pTextBoxTextTranslate, text)){
				return *text;
			}
		}
	}
	return pTextBoxText;
}

void ceCAActorSpeak::SetTextBoxTextStyle(const char *style){
	pTextBoxTextStyle = style;
}

void ceCAActorSpeak::SetPathSound(const char *path){
	if(!pPathSound.Equals(path)){
		pPathSound = path;
		pEngSound = nullptr;
		pLoaded = false;
	}
}

void ceCAActorSpeak::SetMovement(const char *movement){
	if(!movement){
		DETHROW(deeInvalidParam);
	}
	
	pMovement = movement;
}

void ceCAActorSpeak::SetMinSpeechTime(float minSpeechTime){
	if(minSpeechTime < 0.0f){
		pMinSpeechTime = 0.0f;
		
	}else{
		pMinSpeechTime = minSpeechTime;
	}
}

void ceCAActorSpeak::SetUseSpeechAnimation(bool useSpeechAnimation){
	pUseSpeechAnimation = useSpeechAnimation;
}



ceConversationAction::Ref ceCAActorSpeak::CreateCopy() const{
	return ceCAActorSpeak::Ref::New(*this);
}



// Private Functions
//////////////////////

void ceCAActorSpeak::pLoadSound(){
	deSound::Ref sound;
	
	try{
		if(!pPathSound.IsEmpty()){
			sound = pEngine->GetSoundManager()->LoadSound(pPathSound.GetString(), "/", false);
		}
		
		if(sound && sound->GetChannelCount() != 1){
			pEngine->GetLogger()->LogWarnFormat(LOGSOURCE,
				"Channel count %i instead of 1. Sound file not used.", sound->GetChannelCount());
			sound = nullptr;
		}
		
	}catch(const deException &e){
		sound = nullptr;
		pEngine->GetLogger()->LogException(LOGSOURCE, e);
	}
	pEngSound = sound;
	
	pLoaded = true;
}
