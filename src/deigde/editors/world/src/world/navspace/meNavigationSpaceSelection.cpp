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

#include "meNavigationSpace.h"
#include "meNavigationSpaceSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>



// Class meNavigationSpaceSelection
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meNavigationSpaceSelection::meNavigationSpaceSelection(){
}

meNavigationSpaceSelection::~meNavigationSpaceSelection(){
}



// Management
///////////////

void meNavigationSpaceSelection::Add(meNavigationSpace *navspaces){
	DEASSERT_NOTNULL(navspaces)
	
	navspaces->SetSelected(true);
	pSelection.Add(navspaces);
}

void meNavigationSpaceSelection::Remove(meNavigationSpace *navspaces){
	DEASSERT_NOTNULL(navspaces)
	
	navspaces->SetSelected(false);
	pSelection.Remove(navspaces);
}

void meNavigationSpaceSelection::RemoveAll(){
	pSelection.Visit([](meNavigationSpace &navspaces){
		navspaces.SetSelected(false);
	});
	
	pSelection.RemoveAll();
}



bool meNavigationSpaceSelection::HasActive() const{
	return pActive.IsNotNull();
}

void meNavigationSpaceSelection::SetActive(meNavigationSpace *navspaces){
	if(pActive){
		pActive->SetActive(false);
	}
	
	pActive = navspaces;
	
	if(navspaces){
		navspaces->SetActive(true);
	}
}

void meNavigationSpaceSelection::ActivateNext(){
	meNavigationSpace *next = nullptr;
	if(pSelection.IsNotEmpty()){
		if(pSelection.First() != pActive){
			next = pSelection.First();
			
		}else if(pSelection.GetCount() > 1){
			next = pSelection.GetAt(1);
		}
	}
	
	SetActive(next);
}

void meNavigationSpaceSelection::Reset(){
	RemoveAll();
	SetActive(nullptr);
}
