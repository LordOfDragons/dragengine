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

#include "gdeUOCForceFieldSetFluctuationDirection.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/forceField/gdeOCForceField.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCForceFieldSetFluctuationDirection
//////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCForceFieldSetFluctuationDirection::gdeUOCForceFieldSetFluctuationDirection(
gdeObjectClass::Ref objectClass, gdeOCForceField::Ref forceField, float newValue) :

pForceField(NULL)
{
	if(!objectClass || !forceField){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Force field set fluctuation direction");
	
	pOldValue = forceField->GetFluctuationDirection();
	pNewValue = newValue;
	
	pForceField = forceField;
	pObjectClass = objectClass;
}

gdeUOCForceFieldSetFluctuationDirection::~gdeUOCForceFieldSetFluctuationDirection(){
}



// Management
///////////////

void gdeUOCForceFieldSetFluctuationDirection::Undo(){
	pForceField->SetFluctuationDirection(pOldValue);
	pObjectClass->NotifyForceFieldChanged(pForceField);
}

void gdeUOCForceFieldSetFluctuationDirection::Redo(){
	pForceField->SetFluctuationDirection(pNewValue);
	pObjectClass->NotifyForceFieldChanged(pForceField);
}
