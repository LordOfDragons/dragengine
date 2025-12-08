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

#include "deSkinPropertyNodeGroup.h"
#include "deSkinPropertyNodeImage.h"
#include "deSkinPropertyNodeText.h"
#include "deSkinPropertyNodeVisitorLoad.h"
#include "../../../image/deImage.h"
#include "../../../image/deImageManager.h"
#include "../../../font/deFont.h"
#include "../../../font/deFontManager.h"
#include "../../../../deEngine.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeVisitorLoad
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeVisitorLoad::deSkinPropertyNodeVisitorLoad(
deEngine &engine, deVirtualFileSystem *vfs, const char *basePath) :
pEngine(engine),
pVirtualFileSystem(vfs),
pBasePath(basePath){
}

deSkinPropertyNodeVisitorLoad::~deSkinPropertyNodeVisitorLoad(){
}



// Visiting
/////////////

void deSkinPropertyNodeVisitorLoad::VisitNode(deSkinPropertyNode &node){
	if(node.GetMask()){
		node.GetMask()->Visit(*this);
	}
}

void deSkinPropertyNodeVisitorLoad::VisitGroup(deSkinPropertyNodeGroup &node){
	deSkinPropertyNodeVisitor::VisitGroup(node);
	
	const int count = node.GetNodeCount();
	int i;
	
	for(i=0; i<count; i++){
		node.GetNodeAt(i)->Visit(*this);
	}
}

void deSkinPropertyNodeVisitorLoad::VisitImage(deSkinPropertyNodeImage &node){
	deSkinPropertyNodeVisitor::VisitImage(node);
	
	if(node.GetImage()){
		return;
	}
	
	const decString &path = node.GetPath();
	deImage *image = NULL;
	
	if(!path.IsEmpty()){
		try{
			image = pEngine.GetImageManager()->LoadImage(pVirtualFileSystem, path, pBasePath);
			node.SetImage(image);
			image->FreeReference();
			image = NULL;
			
		}catch(const deException &){
			if(image){
				image->FreeReference();
				image = NULL;
			}
		}
	}
}

void deSkinPropertyNodeVisitorLoad::VisitText(deSkinPropertyNodeText &node){
	deSkinPropertyNodeVisitor::VisitText(node);
	
	if(node.GetFont()){
		return;
	}
	
	const decString &path = node.GetPath();
	deFont *font = NULL;
	
	if(!path.IsEmpty()){
		try{
			font = pEngine.GetFontManager()->LoadFont(pVirtualFileSystem, path, pBasePath);
			node.SetFont(font);
			font->FreeReference();
			font = NULL;
			
		}catch(const deException &){
			if(font){
				font->FreeReference();
				font = NULL;
			}
		}
	}
}
