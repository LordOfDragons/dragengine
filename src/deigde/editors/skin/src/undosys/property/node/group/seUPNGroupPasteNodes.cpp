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

#include "seUPNGroupPasteNodes.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../../clipboard/seClipboardDataPropertyNode.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupPasteNodes
///////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupPasteNodes::seUPNGroupPasteNodes(sePropertyNodeGroup *group,
int layer, const seClipboardDataPropertyNode &clipboardData) :
pGroup(NULL),
pLayer(layer)
{
	if(! group || ! group->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	const int count = clipboardData.GetCount();
	if(count == 0){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Node group paste nodes");
	
	sePropertyNode *pasteNode = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			pasteNode = clipboardData.GetAt(i).Copy();
			
			decPoint3 position(pasteNode->GetPosition());
			position.z = layer;
			pasteNode->SetPosition(position);
			
			pNodes.Add(pasteNode);
			pasteNode->FreeReference();
			pasteNode = NULL;
		}
		
		pGroup = group;
		group->AddReference();
		
	}catch(const deException &){
		if(pasteNode){
			pasteNode->FreeReference();
		}
		pNodes.RemoveAll();
		throw;
	}
}

seUPNGroupPasteNodes::~seUPNGroupPasteNodes(){
	if(pGroup){
		pGroup->FreeReference();
	}
}



// Management
///////////////

void seUPNGroupPasteNodes::Undo(){
	const int count = pNodes.GetCount();
	int i;
	
	seProperty * const property = pGroup->GetProperty();
	if(property && property->GetActiveNodeGroup() == pGroup){
		property->GetNodeSelection().RemoveAll();
	}
	
	for(i=0; i<count; i++){
		pGroup->RemoveNode((sePropertyNode*)pNodes.GetAt(i));
	}
}

void seUPNGroupPasteNodes::Redo(){
	const int count = pNodes.GetCount();
	int i;
	
	seProperty * const property = pGroup->GetProperty();
	if(property && property->GetActiveNodeGroup() == pGroup){
		property->GetNodeSelection().RemoveAll();
	}
	
	for(i=0; i<count; i++){
		pGroup->AddNode((sePropertyNode*)pNodes.GetAt(i));
	}
	
	if(property && property->GetActiveNodeGroup() == pGroup){
		property->GetNodeSelection().SetSelected(pNodes);
	}
}
