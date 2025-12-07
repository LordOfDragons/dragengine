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

#include "debnState.h"
#include "debnStateLink.h"
#include "debnStateLinkList.h"
#include "../debnConnection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/network/deNetworkState.h>



// Class debnStateLink
////////////////////////

// Constructor, destructor
////////////////////////////

debnStateLink::debnStateLink(debnState *bnState, debnConnection &connection) :
pState(bnState),
pIdentifier(-1),
pLinkState(elsDown),
pConnection(connection),
pChanged(false),
pValueChanged(NULL),
pValueCount(0),
pPreviousLink(NULL),
pNextLink(NULL)
{
	try{
		const int valueCount = bnState->GetValueCount();
		if(valueCount > 0){
			pValueChanged = new bool[valueCount];
			for(pValueCount=0; pValueCount<valueCount; pValueCount++){
				pValueChanged[pValueCount] = false;
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debnStateLink::~debnStateLink(){
	pCleanUp();
}



// Management
///////////////

void debnStateLink::DropState(){
	pState = NULL;
}

void debnStateLink::SetIdentifier(int identifier){
	if(identifier < -1){
		DETHROW(deeInvalidParam);
	}
	pIdentifier = identifier;
}

void debnStateLink::SetLinkState(int linkState){
	pLinkState = linkState;
}

void debnStateLink::SetChanged(bool changed){
	if(changed == pChanged){
		return;
	}
	
	pChanged = changed;
	
	if(changed){
		pConnection.GetModifiedStateLinks()->AddLink(this);
	}
}



bool debnStateLink::GetValueChangedAt(int index) const{
	if(index < 0 || index >= pValueCount){
		DETHROW(deeInvalidParam);
	}
	return pValueChanged[index];
}

void debnStateLink::SetValueChangedAt(int index, bool changed){
	if(index < 0 || index >= pValueCount){
		DETHROW(deeInvalidParam);
	}
	
	if(changed == pValueChanged[index]){
		return;
	}
	
	pValueChanged[index] = changed;
	
	if(changed){
		SetChanged(true);
	}
}

bool debnStateLink::HasChangedValues() const{
	int i;
	for(i=0; i<pValueCount; i++){
		if(pValueChanged[i]){
			return true;
		}
	}
	return false;
}

void debnStateLink::ResetChanged(){
	pChanged = false;
	
	int i;
	for(i=0; i<pValueCount; i++){
		pValueChanged[i] = false;
	}
}



// Linked List
////////////////

void debnStateLink::SetPreviousLink(debnStateLink *link){
	pPreviousLink = link;
}

void debnStateLink::SetNextLink(debnStateLink *link){
	pNextLink = link;
}



// Private Functions
//////////////////////

void debnStateLink::pCleanUp(){
	if(pValueChanged){
		delete [] pValueChanged;
	}
}
