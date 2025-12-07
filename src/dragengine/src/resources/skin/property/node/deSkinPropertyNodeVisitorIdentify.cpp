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

#include "deSkinPropertyNode.h"
#include "deSkinPropertyNodeGroup.h"
#include "deSkinPropertyNodeImage.h"
#include "deSkinPropertyNodeShape.h"
#include "deSkinPropertyNodeText.h"
#include "deSkinPropertyNodeVisitorIdentify.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeVisitorIdentify
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeVisitorIdentify::deSkinPropertyNodeVisitorIdentify() :
pNode(NULL),
pType(entUnknown){
}

deSkinPropertyNodeVisitorIdentify::~deSkinPropertyNodeVisitorIdentify(){
}



// Management
///////////////

deSkinPropertyNodeGroup &deSkinPropertyNodeVisitorIdentify::CastToGroup() const{
	if(pType != entGroup){
		DETHROW(deeInvalidParam);
	}
	return *((deSkinPropertyNodeGroup*)pNode);
}

deSkinPropertyNodeImage &deSkinPropertyNodeVisitorIdentify::CastToImage() const{
	if(pType != entImage){
		DETHROW(deeInvalidParam);
	}
	return *((deSkinPropertyNodeImage*)pNode);
}

deSkinPropertyNodeShape &deSkinPropertyNodeVisitorIdentify::CastToShape() const{
	if(pType != entShape){
		DETHROW(deeInvalidParam);
	}
	return *((deSkinPropertyNodeShape*)pNode);
}

deSkinPropertyNodeText &deSkinPropertyNodeVisitorIdentify::CastToText() const{
	if(pType != entText){
		DETHROW(deeInvalidParam);
	}
	return *((deSkinPropertyNodeText*)pNode);
}



void deSkinPropertyNodeVisitorIdentify::Reset(){
	pNode = NULL;
	pType = entUnknown;
}



// Visiting
/////////////

void deSkinPropertyNodeVisitorIdentify::VisitNode(deSkinPropertyNode &node){
	pNode = &node;
	pType = entUnknown;
}

void deSkinPropertyNodeVisitorIdentify::VisitGroup(deSkinPropertyNodeGroup &node){
	pNode = &node;
	pType = entGroup;
}

void deSkinPropertyNodeVisitorIdentify::VisitImage(deSkinPropertyNodeImage &node){
	pNode = &node;
	pType = entImage;
}

void deSkinPropertyNodeVisitorIdentify::VisitShape(deSkinPropertyNodeShape &node){
	pNode = &node;
	pType = entShape;
}

void deSkinPropertyNodeVisitorIdentify::VisitText(deSkinPropertyNodeText &node){
	pNode = &node;
	pType = entText;
}
