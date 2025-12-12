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

#include "deRLTaskReadSkinProperty.h"
#include "deRLTaskReadSkinInternal.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../skin/property/deSkinPropertyConstructed.h"
#include "../../skin/property/deSkinPropertyImage.h"
#include "../../skin/property/deSkinPropertyVideo.h"
#include "../../skin/property/node/deSkinPropertyNodeGroup.h"
#include "../../video/deVideo.h"
#include "../../video/deVideoManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"



// Class deRLTaskReadSkinProperty
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkinProperty::deRLTaskReadSkinProperty(deResourceLoader &resourceLoader,
deRLTaskReadSkinInternal &task, deEngine &engine, deVirtualFileSystem *vfs, const char *basePath) :
pResourceLoader(resourceLoader),
pTask(task),
pEngine(engine),
pVirtualFileSystem(vfs),
pBasePath(basePath),
pLoadNode(resourceLoader, task, engine, vfs, basePath){
}

deRLTaskReadSkinProperty::~deRLTaskReadSkinProperty(){
}



// Visiting
/////////////

void deRLTaskReadSkinProperty::VisitImage(deSkinPropertyImage &property){
	if(property.GetImage()){
		return;
	}
	
	decString path(property.GetPath());
	
	if(path.IsEmpty()){
		// TODO has to be done by the graphic module not here
		try{
			property.SetImage(pEngine.GetImageManager()->LoadDefault());
			
		}catch(const deException &){
		}
		return;
	}
	
	if(!decPath::IsUnixPathAbsolute(path)){
		decPath resourcePath;
		resourcePath.SetFromUnix(pBasePath);
		resourcePath.AddUnixPath(path);
		path = resourcePath.GetPathUnix();
	}
	
	pTask.AddInternalTask(deRLTaskReadSkinInternal::cInternalTask::Ref::New(&property,
		pResourceLoader.AddLoadRequest(pVirtualFileSystem, path, deResourceLoader::ertImage)));
}

void deRLTaskReadSkinProperty::VisitVideo(deSkinPropertyVideo &property){
	if(property.GetVideo()){
		return;
	}
	
	decString path(property.GetPath());
	if(path.IsEmpty()){
		return;
	}
	
	if(!decPath::IsUnixPathAbsolute(path)){
		decPath resourcePath;
		resourcePath.SetFromUnix(pBasePath);
		resourcePath.AddUnixPath(path);
		path = resourcePath.GetPathUnix();
	}
	
	// loaded directly since video uses decoders the responsible module create later on
	try{
		property.SetVideo(pEngine.GetVideoManager()->LoadVideo(pVirtualFileSystem, path, pBasePath, false));
		
	}catch(const deException &){
	}
}

void deRLTaskReadSkinProperty::VisitConstructed(deSkinPropertyConstructed &property){
	property.GetContent().Visit(pLoadNode);
}
