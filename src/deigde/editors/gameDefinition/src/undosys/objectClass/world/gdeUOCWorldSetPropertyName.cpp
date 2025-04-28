/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "gdeUOCWorldSetPropertyName.h"

#include <dragengine/common/exceptions.h>


// Class gdeUOCWorldSetPropertyName
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCWorldSetPropertyName::gdeUOCWorldSetPropertyName(gdeObjectClass *objectClass,
	gdeOCWorld *world, gdeOCWorld::eProperties property, const char *newValue) :
pObjectClass(objectClass),
pWorld(world),
pProperty(property)
{
	DEASSERT_NOTNULL(objectClass)
	DEASSERT_NOTNULL(world)
	
	SetShortInfo("World set property name");
	
	pOldValue = world->GetPropertyName(property);
	pNewValue = newValue;
}


// Management
///////////////

void gdeUOCWorldSetPropertyName::Undo(){
	pWorld->SetPropertyName(pProperty, pOldValue);
	pObjectClass->NotifyWorldChanged(pWorld);
}

void gdeUOCWorldSetPropertyName::Redo(){
	pWorld->SetPropertyName(pProperty, pNewValue);
	pObjectClass->NotifyWorldChanged(pWorld);
}
