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

#include "gdeUOCComponentSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCComponentSetPropertyName
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCComponentSetPropertyName::gdeUOCComponentSetPropertyName( gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponent::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pComponent( NULL ),
pProperty( property )
{
	if( ! objectClass || ! component ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Component set property name" );
	
	pOldValue = component->GetPropertyName( property );
	pNewValue = newValue;
	
	pComponent = component;
	component->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCComponentSetPropertyName::~gdeUOCComponentSetPropertyName(){
	if( pComponent ){
		pComponent->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCComponentSetPropertyName::Undo(){
	pComponent->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifyComponentChanged( pComponent );
}

void gdeUOCComponentSetPropertyName::Redo(){
	pComponent->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifyComponentChanged( pComponent );
}
