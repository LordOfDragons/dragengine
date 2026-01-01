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

#include "meUAddDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUAddDecal
//////////////////////

// Constructor, destructor
////////////////////////////

meUAddDecal::meUAddDecal(meWorld *world, meDecal *decal){
	if(!world || !decal) DETHROW(deeInvalidParam);
	
	pWorld = nullptr;
	pDecal = nullptr;
	
	SetShortInfo("Add Decal");
	SetLongInfo("");
	
	pDecal = decal;
	pWorld = world;
}

meUAddDecal::~meUAddDecal(){
}



// Management
///////////////

void meUAddDecal::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	selection.Remove(pDecal);
	if(pDecal->GetActive()){
		selection.ActivateNext();
	}
	
	pWorld->RemoveDecal(pDecal);
	
	pWorld->NotifyDecalCountChanged();
	pWorld->NotifyDecalSelectionChanged();
}

void meUAddDecal::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	pWorld->AddDecal(pDecal);
	
	selection.Reset();
	selection.Add(pDecal);
	selection.SetActive(pDecal);
	
	pWorld->NotifyDecalCountChanged();
	pWorld->NotifyDecalSelectionChanged();
}
