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

#include "seUPropertyConstructedFromImage.h"
#include "../../skin/property/node/sePropertyNodeImage.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class seUPropertyConstructedFromImage
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyConstructedFromImage::seUPropertyConstructedFromImage(seProperty *property) :

pOldContent(NULL)
{
	if(!property){
		DETHROW(deeInvalidParam);
	}
	
	const deImage * const image = property->GetEngineImage();
	if(!image){
		DETHROW(deeInvalidParam);
	}
	
	pOldBitCount = property->GetNodeBitCount();
	pNewBitCount = image->GetBitCount();
	
	const decPoint3 size(image->GetWidth(), image->GetHeight(), image->GetDepth());
	
	SetShortInfo("Property constructed from image");
	
	sePropertyNodeImage::Ref nodeImage = NULL;
	
	try{
		nodeImage.TakeOver(new sePropertyNodeImage(*property->GetEngine()));
		nodeImage->SetSize(size);
		nodeImage->SetPath(property->GetImagePath());
		
		pNewContent.TakeOver(new sePropertyNodeGroup(*property->GetEngine()));
		pNewContent->SetSize(size);
		pNewContent->AddNode(nodeImage);
		nodeImage = NULL;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pOldContent = property->GetNodeGroup();
	pProperty = property;
}

seUPropertyConstructedFromImage::~seUPropertyConstructedFromImage(){
	pCleanUp();
}



// Management
///////////////

void seUPropertyConstructedFromImage::Undo(){
	pProperty->SetNodeGroup(pOldContent);
	pProperty->SetNodeBitCount(pOldBitCount);
}

void seUPropertyConstructedFromImage::Redo(){
	pProperty->SetNodeGroup(pNewContent);
	pProperty->SetNodeBitCount(pNewBitCount);
}



// Private Functions
//////////////////////

void seUPropertyConstructedFromImage::pCleanUp(){
}
