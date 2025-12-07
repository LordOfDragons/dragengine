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

#include "gdeUOCNavBlockerSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCNavBlockerSetPropertyName
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCNavBlockerSetPropertyName::gdeUOCNavBlockerSetPropertyName(gdeObjectClass *objectClass,
gdeOCNavigationBlocker *navblocker, gdeOCNavigationBlocker::eProperties property, const char *newValue) :
pObjectClass(NULL),
pNavBlocker(NULL),
pProperty(property)
{
	if(!objectClass || !navblocker){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("NavBlocker set property name");
	
	pOldValue = navblocker->GetPropertyName(property);
	pNewValue = newValue;
	
	pNavBlocker = navblocker;
	navblocker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCNavBlockerSetPropertyName::~gdeUOCNavBlockerSetPropertyName(){
	if(pNavBlocker){
		pNavBlocker->FreeReference();
	}
	if(pObjectClass){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCNavBlockerSetPropertyName::Undo(){
	pNavBlocker->SetPropertyName(pProperty, pOldValue);
	pObjectClass->NotifyNavigationBlockerChanged(pNavBlocker);
}

void gdeUOCNavBlockerSetPropertyName::Redo(){
	pNavBlocker->SetPropertyName(pProperty, pNewValue);
	pObjectClass->NotifyNavigationBlockerChanged(pNavBlocker);
}
