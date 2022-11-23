/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceCAActorSpeak.h"
#include "../strip/ceStrip.h"

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

ceCAActorSpeak::ceCAActorSpeak( deEngine *engine ) : ceConversationAction( eatActorSpeak ){
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	pEngine = engine;
	pEngSound = NULL;
	pLoaded = true;
	
	pMinSpeechTime = 0.0f;
	pUseSpeechAnimation = true;
}

ceCAActorSpeak::ceCAActorSpeak( const ceCAActorSpeak &action ) : ceConversationAction( action ){
	pEngine = action.GetEngine();
	pEngSound = NULL;
	pLoaded = true;
	
	pActor = action.pActor;
	pTextBoxText = action.pTextBoxText;
	pTextBoxTextTranslate = action.pTextBoxTextTranslate;
	pTextBoxTextStyle = action.pTextBoxTextStyle;
	pPathSound = action.pPathSound;
	pMovement = action.pMovement;
	pWords.AddCopyFrom( action.pWords );
	pFacePoses.AddCopyFrom( action.pFacePoses );
	pGestures.AddCopyFrom( action.pGestures );
	pBodyLookAt.AddCopyFrom( action.pBodyLookAt );
	pHeadLookAt.AddCopyFrom( action.pHeadLookAt );
	pEyesLookAt.AddCopyFrom( action.pEyesLookAt );
	pMinSpeechTime = action.pMinSpeechTime;
	pUseSpeechAnimation = action.pUseSpeechAnimation;
	
	pEngSound = action.pEngSound;
	if( pEngSound ){
		pEngSound->AddReference();
	}
	pLoaded = action.pLoaded;
}

ceCAActorSpeak::~ceCAActorSpeak(){
	if( pEngSound ){
		pEngSound->FreeReference();
	}
}



// Management
///////////////

deSound *ceCAActorSpeak::GetEngineSound(){
	if( ! pLoaded ){
		pLoadSound();
	}
	
	return pEngSound;
}



void ceCAActorSpeak::SetActor( const char *id ){
	pActor = id;
}

void ceCAActorSpeak::SetTextBoxText( const decUnicodeString &text ){
	pTextBoxText = text;
}

void ceCAActorSpeak::SetTextBoxTextTranslate( const char *text ){
	pTextBoxTextTranslate = text;
}

void ceCAActorSpeak::SetTextBoxTextStyle( const char *style ){
	pTextBoxTextStyle = style;
}

void ceCAActorSpeak::SetPathSound( const char *path ){
	if( ! pPathSound.Equals( path ) ){
		pPathSound = path;
		
		if( pEngSound ){
			pEngSound->FreeReference();
			pEngSound = NULL;
		}
		pLoaded = false;
	}
}

void ceCAActorSpeak::SetMovement( const char *movement ){
	if( ! movement ){
		DETHROW( deeInvalidParam );
	}
	
	pMovement = movement;
}

void ceCAActorSpeak::SetMinSpeechTime( float minSpeechTime ){
	if( minSpeechTime < 0.0f ){
		pMinSpeechTime = 0.0f;
		
	}else{
		pMinSpeechTime = minSpeechTime;
	}
}

void ceCAActorSpeak::SetUseSpeechAnimation( bool useSpeechAnimation ){
	pUseSpeechAnimation = useSpeechAnimation;
}



ceConversationAction *ceCAActorSpeak::CreateCopy() const{
	return new ceCAActorSpeak( *this );
}



// Private Functions
//////////////////////

void ceCAActorSpeak::pLoadSound(){
	deSound *sound = NULL;
	
	try{
		if( ! pPathSound.IsEmpty() ){
			sound = pEngine->GetSoundManager()->LoadSound( pPathSound.GetString(), "/", false );
		}
		
		if( sound && sound->GetChannelCount() != 1 ){
			pEngine->GetLogger()->LogWarnFormat( LOGSOURCE,
				"Channel count %i instead of 1. Sound file not used.", sound->GetChannelCount() );
			sound->FreeReference();
			sound = NULL;
		}
		
	}catch( const deException &e ){
		if( sound ){
			sound->FreeReference();
			sound = NULL;
		}
		pEngine->GetLogger()->LogException( LOGSOURCE, e );
	}
	
	if( pEngSound ){
		pEngSound->FreeReference();
	}
	pEngSound = sound;
	
	pLoaded = true;
}
