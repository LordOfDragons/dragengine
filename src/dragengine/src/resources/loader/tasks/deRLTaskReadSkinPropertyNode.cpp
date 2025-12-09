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

#include "deRLTaskReadSkinPropertyNode.h"
#include "deRLTaskReadSkinInternal.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../font/deFont.h"
#include "../../font/deFontManager.h"
#include "../../skin/property/node/deSkinPropertyNodeGroup.h"
#include "../../skin/property/node/deSkinPropertyNodeImage.h"
#include "../../skin/property/node/deSkinPropertyNodeText.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"



// Class deRLTaskReadSkinPropertyNode
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkinPropertyNode::deRLTaskReadSkinPropertyNode(deResourceLoader &resourceLoader,
deRLTaskReadSkinInternal &task, deEngine &engine, deVirtualFileSystem *vfs, const char *basePath) :
pResourceLoader(resourceLoader),
pTask(task),
pEngine(engine),
pVirtualFileSystem(vfs),
pBasePath(basePath)
{
	(void)pEngine; // silence compiler moaning
}

deRLTaskReadSkinPropertyNode::~deRLTaskReadSkinPropertyNode(){
}



// Visiting
/////////////

void deRLTaskReadSkinPropertyNode::VisitNode(deSkinPropertyNode &node){
	if(node.GetMask()){
		node.GetMask()->Visit(*this);
	}
}

void deRLTaskReadSkinPropertyNode::VisitGroup(deSkinPropertyNodeGroup &node){
	deSkinPropertyNodeVisitor::VisitGroup(node);
	
	const int count = node.GetNodeCount();
	int i;
	
	for(i=0; i<count; i++){
		node.GetNodeAt(i)->Visit(*this);
	}
}

void deRLTaskReadSkinPropertyNode::VisitImage(deSkinPropertyNodeImage &node){
	deSkinPropertyNodeVisitor::VisitImage(node);
	
	if(node.GetImage()){
		return;
	}
	
	decString path(node.GetPath());
	if(path.IsEmpty()){
		return;
	}
	
	if(!decPath::IsUnixPathAbsolute(path)){
		decPath resourcePath;
		resourcePath.SetFromUnix(pBasePath);
		resourcePath.AddUnixPath(path);
		path = resourcePath.GetPathUnix();
	}
	
	pTask.AddInternalTask(deRLTaskReadSkinInternal::cInternalTask::Ref::NewWith(&node,
		pResourceLoader.AddLoadRequest(pVirtualFileSystem, path, deResourceLoader::ertImage)));
}

void deRLTaskReadSkinPropertyNode::VisitText(deSkinPropertyNodeText &node){
	deSkinPropertyNodeVisitor::VisitText(node);
	
	if(node.GetFont()){
		return;
	}
	
	decString path(node.GetPath());
	if(path.IsEmpty()){
		return;
	}
	
	if(!decPath::IsUnixPathAbsolute(path)){
		decPath resourcePath(decPath::CreatePathUnix(pBasePath));
		resourcePath.AddUnixPath(path);
		path = resourcePath.GetPathUnix();
	}
	
	pTask.AddInternalTask(deRLTaskReadSkinInternal::cInternalTask::Ref::NewWith(&node, 
		pResourceLoader.AddLoadRequest(pVirtualFileSystem, path, deResourceLoader::ertFont)));
}
