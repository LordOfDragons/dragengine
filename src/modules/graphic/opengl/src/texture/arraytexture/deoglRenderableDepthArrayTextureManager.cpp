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

#include "deoglRenderableDepthArrayTexture.h"
#include "deoglRenderableDepthArrayTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthArrayTextureManager
//////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthArrayTextureManager::deoglRenderableDepthArrayTextureManager(
	deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pArrayTextures(NULL),
pArrayTextureCount(0),
pArrayTextureSize(0){
}

deoglRenderableDepthArrayTextureManager::~deoglRenderableDepthArrayTextureManager(){
	if(pArrayTextures){
		while(pArrayTextureCount > 0){
			pArrayTextureCount--;
			delete pArrayTextures[pArrayTextureCount];
		}
		delete [] pArrayTextures;
	}
}



// Management
///////////////

const deoglRenderableDepthArrayTexture *deoglRenderableDepthArrayTextureManager::GetAt(int index) const{
	if(index < 0 || index >= pArrayTextureCount){
		DETHROW(deeInvalidParam);
	}
	
	return pArrayTextures[index];
}

deoglRenderableDepthArrayTexture *deoglRenderableDepthArrayTextureManager::GetWith(
int width, int height, int layerCount, bool withStencil, bool useFloat){
	deoglRenderableDepthArrayTexture *texture = NULL;
	int i;
	
	// find the texture with the matching format
	for(i=0; i<pArrayTextureCount; i++){
		if(!pArrayTextures[i]->GetInUse() && pArrayTextures[i]->Matches(
		width, height, layerCount, withStencil, useFloat)){
			texture = pArrayTextures[i];
			break;
		}
	}
	
	// if not found create a new one
	if(!texture){
		if(pArrayTextureCount == pArrayTextureSize){
			int newSize = pArrayTextureSize * 3 / 2 + 1;
			deoglRenderableDepthArrayTexture **newArray = new deoglRenderableDepthArrayTexture*[newSize];
			if(pArrayTextures){
				memcpy(newArray, pArrayTextures, sizeof(deoglRenderableDepthArrayTexture*) * pArrayTextureSize);
				delete [] pArrayTextures;
			}
			pArrayTextures = newArray;
			pArrayTextureSize = newSize;
		}
		
		texture = new deoglRenderableDepthArrayTexture(pRenderThread,
			width, height, layerCount, withStencil, useFloat);
		pArrayTextures[pArrayTextureCount] = texture;
		pArrayTextureCount++;
	}
	
	// mark the texture in use and return it
	texture->SetInUse(true);
	return texture;
}
