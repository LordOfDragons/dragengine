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

ceUCAASpeakStripsScale::ceUCAASpeakStripsScale( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak ){
	if( ! topic || ! actorSpeak ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pOldStates = NULL;
	
	SetShortInfo( "Scale strips" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakStripsScale::~ceUCAASpeakStripsScale(){
	if( pOldStates ){
		delete [] pOldStates;
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

void ceUCAASpeakStripsScale::SetStrips( const ceStripList &strips ){
	if( pOldStates ){
		delete [] pOldStates;
		pOldStates = NULL;
	}
	
	pStrips = strips;
	
	const int count = strips.GetCount();
	if( count > 0 ){
		int i;
		
		pOldStates = new sStrip[ count ];
		
		for( i=0; i<count; i++ ){
			const ceStrip &strip = *strips.GetAt( i );
			pOldStates[ i ].pause = strip.GetPause();
			pOldStates[ i ].duration = strip.GetDuration();
		}
	}
}

void ceUCAASpeakStripsScale::SetScaling( float scaling ){
	if( scaling < 0.0f ){
		pScaling = 0.0f;
		
	}else{
		pScaling = scaling;
	}
}



void ceUCAASpeakStripsScale::Undo(){
	const int count = pStrips.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceStrip &strip = *pStrips.GetAt( i );
		strip.SetPause( pOldStates[ i ].pause );
		strip.SetDuration( pOldStates[ i ].duration );
	}
	
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakStripsScale::Redo(){
	const int count = pStrips.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceStrip &strip = *pStrips.GetAt( i );
		strip.SetPause( pOldStates[ i ].pause * pScaling );
		strip.SetDuration( pOldStates[ i ].duration * pScaling );
	}
	
	pTopic->NotifyActionChanged( pActorSpeak );
}
