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
#include <stdlib.h>

#include "seClipboardDataPropertyNode.h"
#include "../skin/property/node/sePropertyNode.h"
#include "../skin/property/node/sePropertyNodeList.h"

#include <dragengine/common/exceptions.h>



// Class seClipboardDataPropertyNode
//////////////////////////////////////

const char * const seClipboardDataPropertyNode::TYPE_NAME = "property-node";

// Constructor, destructor
////////////////////////////

seClipboardDataPropertyNode::seClipboardDataPropertyNode(const sePropertyNodeList &nodes) :
igdeClipboardData(TYPE_NAME)
{
	const int count = nodes.GetCount();
	if(count == 0){
		DETHROW(deeInvalidParam);
	}
	
	sePropertyNode *node = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			node = nodes.GetAt(i)->Copy();
			pNodes.Add(node);
			node->FreeReference();
			node = NULL;
		}
		
	}catch(const deException &){
		pNodes.RemoveAll();
		throw;
	}
}

seClipboardDataPropertyNode::~seClipboardDataPropertyNode(){
}



// Management
///////////////

int seClipboardDataPropertyNode::GetCount() const{
	return pNodes.GetCount();
}

const sePropertyNode &seClipboardDataPropertyNode::GetAt(int index) const{
	return *((const sePropertyNode *)pNodes.GetAt(index));
}
