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

#include "seUPNGroupNodesBottom.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupNodesBottom
////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupNodesBottom::seUPNGroupNodesBottom(sePropertyNodeGroup *node, const sePropertyNode::List &children) :
seUPNGroupMoveNodes(node, children)
{
	SetShortInfo("Move nodes to bottom");
	
	if(pChildren.IsNotEmpty() && pChildren.First()->index == 0){
		pChildren.RemoveAll(); // has no effect
	}
}

seUPNGroupNodesBottom::~seUPNGroupNodesBottom(){
}



// Management
///////////////

void seUPNGroupNodesBottom::Undo(){
	pChildren.VisitReverse([&](const cNode &child){
		pNode->MoveNode(child.node, child.index);
	});
}

void seUPNGroupNodesBottom::Redo(){
	pChildren.VisitIndexed([&](int i, const cNode &child){
		pNode->MoveNode(child.node, i);
	});
}
