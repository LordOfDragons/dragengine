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

#include "gdeUOCSpeakerSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSpeakerSetPropertyName
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSpeakerSetPropertyName::gdeUOCSpeakerSetPropertyName( gdeObjectClass *objectClass,
gdeOCSpeaker *speaker, gdeOCSpeaker::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pSpeaker( NULL ),
pProperty( property )
{
	if( ! objectClass || ! speaker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Speaker set property name" );
	
	pOldValue = speaker->GetPropertyName( property );
	pNewValue = newValue;
	
	pSpeaker = speaker;
	speaker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSpeakerSetPropertyName::~gdeUOCSpeakerSetPropertyName(){
	if( pSpeaker ){
		pSpeaker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSpeakerSetPropertyName::Undo(){
	pSpeaker->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifySpeakerChanged( pSpeaker );
}

void gdeUOCSpeakerSetPropertyName::Redo(){
	pSpeaker->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifySpeakerChanged( pSpeaker );
}
