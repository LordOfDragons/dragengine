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

#include "sePropertyNodeImage.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class sePropertyNodeImage
//////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeImage::sePropertyNodeImage(deEngine &engine) :
sePropertyNode(entImage, engine, MappedCount),
pImage(NULL),
pRepeat(1, 1){
}

sePropertyNodeImage::sePropertyNodeImage(const sePropertyNodeImage &node) :
sePropertyNode(node),
pPath(node.pPath),
pImage(NULL),
pRepeat(node.pRepeat)
{
	pImage = node.pImage;
	if(pImage){
		pImage->AddReference();
	}
}

sePropertyNodeImage::~sePropertyNodeImage(){
	if(pImage){
		pImage->FreeReference();
	}
}



// Management
///////////////

void sePropertyNodeImage::SetPath(const char *path){
	if(pPath.Equals(path)){
		return;
	}
	
	pPath = path;
	UpdateImage();
	NotifyChanged();
}

void sePropertyNodeImage::UpdateImage(){
	deImage *image = NULL;
	
	if(! pPath.IsEmpty() && GetProperty() && GetProperty()->GetTexture() && GetProperty()->GetTexture()->GetSkin()){
		const decString &basePath = GetProperty()->GetTexture()->GetSkin()->GetDirectoryPath();
		
		try{
			image = GetEngine().GetImageManager()->LoadImage(pPath, basePath);
			
		}catch(const deException &e){
			GetProperty()->GetTexture()->GetSkin()->GetLogger()->LogException("Skin Editor", e);
		}
	}
	
	if(image == pImage){
		if(image){
			image->FreeReference();
		}
		return;
	}
	
	if(pImage){
		pImage->FreeReference();
	}
	pImage = image;
}

void sePropertyNodeImage::SetRepeat(const decPoint &count){
	if(! (count >= decPoint(1, 1))){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pRepeat){
		return;
	}
	
	pRepeat = count;
	
	NotifyChanged();
}



sePropertyNode *sePropertyNodeImage::Copy() const{
	return new sePropertyNodeImage(*this);
}

void sePropertyNodeImage::UpdateResources(){
	sePropertyNode::UpdateResources();
	UpdateImage();
}
