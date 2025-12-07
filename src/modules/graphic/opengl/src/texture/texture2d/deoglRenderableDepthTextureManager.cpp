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

#include "deoglRenderableDepthTexture.h"
#include "deoglRenderableDepthTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthTextureManager
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthTextureManager::deoglRenderableDepthTextureManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pTextures(NULL),
pTextureCount(0),
pTextureSize(0){
}

deoglRenderableDepthTextureManager::~deoglRenderableDepthTextureManager(){
	if(pTextures){
		while(pTextureCount > 0){
			pTextureCount--;
			delete pTextures[pTextureCount];
		}
		delete [] pTextures;
	}
}



// Management
///////////////

const deoglRenderableDepthTexture *deoglRenderableDepthTextureManager::GetTextureAt(int index) const{
	if(index < 0 || index >= pTextureCount) DETHROW(deeInvalidParam);
	
	return pTextures[index];
}

deoglRenderableDepthTexture *deoglRenderableDepthTextureManager::GetTextureWith(
int width, int height, bool withStencil, bool useFloat){
	deoglRenderableDepthTexture *texture = NULL;
	int i;
	
	// find the texture with the matching format
	for(i=0; i<pTextureCount; i++){
		if(!pTextures[i]->GetInUse() && pTextures[i]->Matches(width, height, withStencil, useFloat)){
			texture = pTextures[i];
			break;
		}
	}
	
	// if not found create a new one
	if(!texture){
		if(pTextureCount == pTextureSize){
			int newSize = pTextureSize * 3 / 2 + 1;
			deoglRenderableDepthTexture **newArray = new deoglRenderableDepthTexture*[newSize];
			if(pTextures){
				memcpy(newArray, pTextures, sizeof(deoglRenderableDepthTexture*) * pTextureSize);
				delete [] pTextures;
			}
			pTextures = newArray;
			pTextureSize = newSize;
		}
		
		texture = new deoglRenderableDepthTexture(pRenderThread, width, height, withStencil, useFloat);
		pTextures[pTextureCount] = texture;
		pTextureCount++;
	}
	
	// mark the texture in use and return it
	texture->SetInUse(true);
	return texture;
}
