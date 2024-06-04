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

#include "gdeUOCBillboardSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCBillboardSetPropertyName
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCBillboardSetPropertyName::gdeUOCBillboardSetPropertyName( gdeObjectClass *objectClass,
gdeOCBillboard *billboard, gdeOCBillboard::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pBillboard( NULL ),
pProperty( property )
{
	if( ! objectClass || ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Billboard set property name" );
	
	pOldValue = billboard->GetPropertyName( property );
	pNewValue = newValue;
	
	pBillboard = billboard;
	billboard->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCBillboardSetPropertyName::~gdeUOCBillboardSetPropertyName(){
	if( pBillboard ){
		pBillboard->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCBillboardSetPropertyName::Undo(){
	pBillboard->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifyBillboardChanged( pBillboard );
}

void gdeUOCBillboardSetPropertyName::Redo(){
	pBillboard->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifyBillboardChanged( pBillboard );
}
