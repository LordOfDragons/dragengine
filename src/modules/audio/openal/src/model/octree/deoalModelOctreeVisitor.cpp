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

#include "deoalModelOctree.h"
#include "deoalModelOctreeVisitor.h"
#include "../deoalModelFace.h"
#include "../../utils/collision/deoalCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoalModelOctreeVisitor
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalModelOctreeVisitor::deoalModelOctreeVisitor(){
}

deoalModelOctreeVisitor::~deoalModelOctreeVisitor(){
}



// Visiting
/////////////

void deoalModelOctreeVisitor::VisitNode(deoalModelOctree &node){
	VisitNode(&node, deoalCollisionDetection::eirPartial);
}

void deoalModelOctreeVisitor::VisitNode(deoalOctree *node, int /*intersection*/){
	const deoalModelOctree &oalNode = *((deoalModelOctree*)node);
	const int count = oalNode.GetFaceCount();
	int i;
	
	for(i=0; i<count; i++){
		VisitFace(oalNode.GetFaceAt(i));
	}
}

void deoalModelOctreeVisitor::VisitFace(deoalModelFace*){
}
