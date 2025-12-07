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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debnState.h"
#include "debnStateLink.h"
#include "debnStateLinkList.h"
#include "../debnConnection.h"
#include "../deNetworkBasic.h"
#include "../values/debnValue.h"
#include "../visitors/debnVisitorValueCreate.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/network/deNetworkState.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValue.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingNetworkState.h>



// Class debnState
////////////////////

// Constructor, destructor
////////////////////////////

debnState::debnState(deNetworkState &state) :
pState(state),
pValues(NULL),
pValueCount(0),
pValueSize(0),
pLinks(NULL),
pParentWorld(NULL)
{
	pLinks = new debnStateLinkList;
}

debnState::~debnState(){
	if(pLinks){
		const int count = pLinks->GetLinkCount();
		int i;
		for(i=0; i<count; i++){
			pLinks->GetLinkAt(i)->DropState();
		}
		delete pLinks;
	}
	
	if(pValues){
		while(pValueCount > 0){
			pValueCount--;
			delete pValues[pValueCount];
		}
		delete [] pValues;
	}
}



// Management
///////////////

void debnState::Update(){
}

void debnState::LinkReadValues(decBaseFileReader &reader, debnStateLink &link){
	deBaseScriptingNetworkState * const scrState = pState.GetPeerScripting();
	const int count = reader.ReadByte();
	int i;
	
	for(i=0; i<count; i++){
		const int index = reader.ReadUShort();
		if(index < 0 || index >= pValueCount){
			DETHROW(deeInvalidParam);
		}
		
		pValues[index]->ReadValue(reader);
		InvalidateValueExcept(index, link);
		
		if(scrState){
			scrState->StateValueChanged(index);
		}
	}
	
	link.SetChanged(link.HasChangedValues());
}

void debnState::LinkReadAllValues(decBaseFileReader &reader, debnStateLink &link){
	deBaseScriptingNetworkState * const scrState = pState.GetPeerScripting();
	const int count = pState.GetValueCount();
	int i;
	
	for(i=0; i<count; i++){
		pValues[i]->ReadValue(reader);
		InvalidateValue(i);
		
		if(scrState){
			scrState->StateValueChanged(i);
		}
	}
	
	if(!link.HasChangedValues()){
		link.SetChanged(false);
	}
}

bool debnState::LinkReadAndVerifyAllValues(decBaseFileReader &reader){
	deBaseScriptingNetworkState *scrState = pState.GetPeerScripting();
	int i;
	
	if(reader.ReadUShort() != pValueCount){
		return false;
	}
	
	for(i=0; i<pValueCount; i++){
		const eValueTypes type = (eValueTypes)reader.ReadByte();
		if(type != pValues[i]->GetDataType()){
			return false;
		}
		
		pValues[i]->ReadValue(reader);
		InvalidateValue(i);
		
		if(scrState){
			scrState->StateValueChanged(i);
		}
	}
	
	return i == pValueCount;
}

void debnState::LinkWriteValues(decBaseFileWriter &writer){
	int i;
	for(i=0; i<pValueCount; i++){
		pValues[i]->WriteValue(writer);
	}
}

void debnState::LinkWriteValuesWithVerify(decBaseFileWriter &writer){
	writer.WriteUShort(pValueCount);
	
	int i;
	for(i=0; i<pValueCount; i++){
		writer.WriteByte((uint8_t)pValues[i]->GetDataType());
		pValues[i]->WriteValue(writer);
	}
}

void debnState::LinkWriteValues(decBaseFileWriter &writer, debnStateLink &link){
	int i, changedCount = 0;
	for(i=0; i<pValueCount; i++){
		if(link.GetValueChangedAt(i)){
			changedCount++;
		}
	}
	if(changedCount > 255){
		changedCount = 255;
	}
	
	writer.WriteByte((uint8_t)changedCount);
	
	for(i=0; i<pValueCount; i++){
		if(!link.GetValueChangedAt(i)){
			continue;
		}
		
		writer.WriteUShort((uint16_t)i);
		pValues[i]->WriteValue(writer);
		
		link.SetValueChangedAt(i, false);
		
		changedCount--;
		if(changedCount == 0){
			break;
		}
	}
	
	link.SetChanged(link.HasChangedValues());
}

void debnState::InvalidateValue(int index){
	const int count = pLinks->GetLinkCount();
	int i;
	for(i=0; i<count; i++){
		pLinks->GetLinkAt(i)->SetValueChangedAt(index, true);
	}
}

void debnState::InvalidateValueExcept(int index, debnStateLink &link){
	const int count = pLinks->GetLinkCount();
	int i;
	for(i=0; i<count; i++){
		debnStateLink * const updateLink = pLinks->GetLinkAt(i);
		updateLink->SetValueChangedAt(index, updateLink != &link);
	}
}



void debnState::SetParentWorld(debnWorld *world){
	pParentWorld = world;
}



// Notifications
//////////////////

void debnState::ValueAdded(int index, deNetworkValue *value){
	if(!value) DETHROW(deeInvalidParam);
	debnVisitorValueCreate visitorCreate;
	
	if(pValueCount == pValueSize){
		int newSize = pValueSize * 3 / 2 + 1;
		debnValue **newArray = new debnValue*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pValues){
			memcpy(newArray, pValues, sizeof(debnValue*) * pValueSize);
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = newSize;
	}
	
	value->Visit(visitorCreate);
	if(!visitorCreate.GetValue()) DETHROW(deeInvalidParam);
	
	pValues[pValueCount] = visitorCreate.GetValue();
	pValueCount++;
}

void debnState::ValueChanged(int index, deNetworkValue*){
//	printf( "value %i at state %p changed\n", index, pState );
	if(!pValues[index]->UpdateValue(false)){
		// value did not change enough to require synchronize links
		return;
	}
	
	const int count = pLinks->GetLinkCount();
	int i;
	
	for(i=0; i<count; i++){
		pLinks->GetLinkAt(i)->SetValueChangedAt(index, true);
	}
}
