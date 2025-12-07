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

#include "seUPNGroupRemoveNodes.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupRemoveNodes
////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupRemoveNodes::seUPNGroupRemoveNodes(
sePropertyNodeGroup *node, const sePropertyNodeList &children) :
pNode(NULL),
pChildren(NULL),
pCount(0)
{
	if(!node || !node->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	const int count = children.GetCount();
	if(count == 0){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Node group remove nodes");
	
	try{
		pChildren = new sNode[count];
		
		for(pCount=0; pCount<count; pCount++){
			pChildren[pCount].index = -1;
			pChildren[pCount].node = children.GetAt(pCount);
			pChildren[pCount].node->AddReference();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pNode = node;
	node->AddReference();
}

seUPNGroupRemoveNodes::~seUPNGroupRemoveNodes(){
	pCleanUp();
}



// Management
///////////////

void seUPNGroupRemoveNodes::Undo(){
	int i;
	for(i=pCount-1; i>=0; i--){
		pNode->InsertNode(pChildren[i].index, pChildren[i].node);
		pChildren[i].index = -1;
	}
}

void seUPNGroupRemoveNodes::Redo(){
	int i;
	for(i=0; i<pCount; i++){
		pChildren[i].index = pNode->IndexOfNode(pChildren[i].node);
		pNode->RemoveNode(pChildren[i].node);
	}
}



// Private Functions
//////////////////////

void seUPNGroupRemoveNodes::pCleanUp(){
	if(pChildren){
		int i;
		for(i=0; i<pCount; i++){
			pChildren[i].node->FreeReference();
		}
		delete [] pChildren;
	}
	if(pNode){
		pNode->FreeReference();
	}
}
