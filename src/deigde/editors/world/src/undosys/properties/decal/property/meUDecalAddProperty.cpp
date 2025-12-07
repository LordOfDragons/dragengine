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

#include "meUDecalAddProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalAddProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalAddProperty::meUDecalAddProperty(meDecal *decal, const char *key, const char *value) :
pDecal(NULL)
{
	if(!decal || !key || strlen(key) == 0 || !value){
		DETHROW(deeInvalidParam);
	}
	
	meWorld * const world = decal->GetWorld();
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Add decal property");
	
	pDecal = NULL;
	
	pKey = key;
	pValue = value;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalAddProperty::~meUDecalAddProperty(){
	pCleanUp();
}



// Management
///////////////

void meUDecalAddProperty::SetValue(const char *value){
	pValue = value;
}



void meUDecalAddProperty::Undo(){
	pDecal->RemoveProperty(pKey);
}

void meUDecalAddProperty::Redo(){
	pDecal->SetProperty(pKey, pValue);
}

void meUDecalAddProperty::ProgressiveRedo(){
	Redo();
}



// Private Functions
//////////////////////

void meUDecalAddProperty::pCleanUp(){
	if(pDecal){
		pDecal->FreeReference();
	}
}
