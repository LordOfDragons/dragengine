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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cePlaybackActor.h"
#include "../textbox/ceTextBoxText.h"
#include "../textbox/ceTextBox.h"

#include <dragengine/common/exceptions.h>



// Class cePlaybackActor
//////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackActor::cePlaybackActor(){
	pSpeechLength = 0.0f;
	pElapsedTime = 0.0f;
	pTextBox = NULL;
	pTextBoxText = NULL;
}

cePlaybackActor::~cePlaybackActor(){
	SetTextBoxText( NULL );
}



// Management
///////////////

void cePlaybackActor::SetSpeechLength( float length ){
	pSpeechLength = length;
}

void cePlaybackActor::SetElapsedTime( float elapsed ){
	pElapsedTime = elapsed;
}

void cePlaybackActor::SetTextBox( ceTextBox *textBox ){
	pTextBox = textBox;
}

void cePlaybackActor::SetTextBoxText( ceTextBoxText *text ){
	if( text != pTextBoxText ){
		if( pTextBoxText ){
			if( pTextBox ){
				pTextBox->GetTextList().Remove( pTextBoxText );
			}
			pTextBoxText->FreeReference();
		}
		
		pTextBoxText = text;
		
		if( text ){
			text->AddReference();
			if( pTextBox ){
				pTextBox->GetTextList().Add( text );
			}
		}
		
		if( pTextBox ){
			pTextBox->UpdateCanvas();
		}
	}
}



bool cePlaybackActor::IsSpeechDone() const{
	return pElapsedTime >= pSpeechLength;
}



void cePlaybackActor::Reset(){
	pSpeechLength = 0.0f;
	pElapsedTime = 0.0f;
	SetTextBoxText( NULL );
}

void cePlaybackActor::Update( float elapsed ){
	pElapsedTime += elapsed;
	
	if( IsSpeechDone() ){
		SetTextBoxText( NULL );
	}
}
