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

#include "deoglSCConstructedDynamic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>


// Class deoglSCConstructedDynamic
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSCConstructedDynamic::deoglSCConstructedDynamic() :
pDynamic(false){
}

deoglSCConstructedDynamic::~deoglSCConstructedDynamic(){
}



// Visiting
/////////////

bool deoglSCConstructedDynamic::IsDynamic(deSkinPropertyNode &node){
	deoglSCConstructedDynamic visitor;
	node.Visit(visitor);
	return visitor.pDynamic;
}

void deoglSCConstructedDynamic::VisitNode(deSkinPropertyNode &node){
	if(pDynamic){
		return;
	}
	
	int i;
	for(i=0; i<deSkinPropertyNode::MappedCount; i++){
		if(node.GetMappedFor((deSkinPropertyNode::eMapped)i) != -1){
			pDynamic = true;
			return;
		}
	}
}

void deoglSCConstructedDynamic::VisitGroup(deSkinPropertyNodeGroup &node){
	deSkinPropertyNodeVisitor::VisitGroup(node);
	
	const int count = node.GetNodeCount();
	int i;
	for(i=0; !pDynamic && i<count; i++){
		node.GetNodeAt(i)->Visit(*this);
	}
}

void deoglSCConstructedDynamic::VisitImage(deSkinPropertyNodeImage &node){
	deSkinPropertyNodeVisitor::VisitImage(node);
}

void deoglSCConstructedDynamic::VisitShape(deSkinPropertyNodeShape &node){
	deSkinPropertyNodeVisitor::VisitShape(node);
	
	if(pDynamic){
		return;
	}
	
	int i;
	for(i=0; i<deSkinPropertyNodeShape::ShapeMappedCount; i++){
		if(node.GetShapeMappedFor((deSkinPropertyNodeShape::eShapeMapped)i) != -1){
			pDynamic = true;
			return;
		}
	}
}

void deoglSCConstructedDynamic::VisitText(deSkinPropertyNodeText &node){
	deSkinPropertyNodeVisitor::VisitText(node);
	
	if(pDynamic){
		return;
	}
	
	int i;
	for(i=0; i<deSkinPropertyNodeText::TextMappedCount; i++){
		if(node.GetTextMappedFor((deSkinPropertyNodeText::eTextMapped)i) != -1){
			pDynamic = true;
			return;
		}
	}
}
