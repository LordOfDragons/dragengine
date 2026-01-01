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

#include "deoglDelayedSaveImage.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/modules/image/deBaseImageModule.h>
#include <dragengine/systems/deModuleSystem.h>



// Class deoglDelayedSaveImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDelayedSaveImage::deoglDelayedSaveImage(const decPath &path, int width,
int height, int depth, int componentCount, int bitCount, char *data) :
pPath(path),
pWidth(width),
pHeight(height),
pDepth(depth),
pComponentCount(componentCount),
pBitCount(bitCount),
pData(data){
	if(!data){
		DETHROW(deeInvalidParam);
	}
}

deoglDelayedSaveImage::~deoglDelayedSaveImage(){
	if(pData){
		delete [] pData;
	}
}



// Management
///////////////

void deoglDelayedSaveImage::SaveImage(deGraphicOpenGl &ogl, deVirtualFileSystem &vfs){
	// NOTE this call is used only for debug images created by deoglDebugSaveTexture.
	//      for this reason no optimization with parallel tasks is used
	deBaseImageModule * const module = (deBaseImageModule*)ogl.GetGameEngine()->
		GetModuleSystem()->GetModuleAbleToLoad(deModuleSystem::emtImage, pPath.GetPathUnix());
	DEASSERT_NOTNULL(module)
	
	const deImage::Ref image(ogl.GetGameEngine()->GetImageManager()->CreateImage(
		pWidth, pHeight, pDepth, pComponentCount, pBitCount));
	memcpy(image->GetData(), pData, pWidth * pHeight * pDepth * pComponentCount * (pBitCount >> 3));
	module->SaveImage(vfs.OpenFileForWriting(pPath), *image);
}
