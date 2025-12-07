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

#include "deoglRenderableColorArrayTexture.h"
#include "deoglRenderableColorArrayTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorArrayTextureManager
//////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorArrayTextureManager::deoglRenderableColorArrayTextureManager(
	deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pArrayTextures(NULL),
pArrayTextureCount(0),
pArrayTextureSize(0){
}

deoglRenderableColorArrayTextureManager::~deoglRenderableColorArrayTextureManager(){
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

const deoglRenderableColorArrayTexture *deoglRenderableColorArrayTextureManager::GetAt(int index) const{
	if(index < 0 || index >= pArrayTextureCount){
		DETHROW(deeInvalidParam);
	}
	
	return pArrayTextures[index];
}

deoglRenderableColorArrayTexture *deoglRenderableColorArrayTextureManager::GetWith(
int width, int height, int layerCount, int componentCount, bool isFloat){
	deoglRenderableColorArrayTexture *texture = NULL;
	int i;
	
	// find the texture with the matching format
	for(i=0; i<pArrayTextureCount; i++){
		if(! pArrayTextures[i]->GetInUse() && pArrayTextures[i]->Matches(
		width, height, layerCount, componentCount, isFloat)){
			texture = pArrayTextures[i];
			break;
		}
	}
	
	// if not found create a new one
	if(! texture){
		if(pArrayTextureCount == pArrayTextureSize){
			int newSize = pArrayTextureSize * 3 / 2 + 1;
			deoglRenderableColorArrayTexture **newArray = new deoglRenderableColorArrayTexture*[newSize];
			
			if(pArrayTextures){
				memcpy(newArray, pArrayTextures, sizeof(deoglRenderableColorArrayTexture*) * pArrayTextureSize);
				delete [] pArrayTextures;
			}
			
			pArrayTextures = newArray;
			pArrayTextureSize = newSize;
		}
		
		texture = new deoglRenderableColorArrayTexture(pRenderThread,
			width, height, layerCount, componentCount, isFloat);
		
		pArrayTextures[pArrayTextureCount] = texture;
		pArrayTextureCount++;
	}
	
	// mark the texture in use and return it
	texture->SetInUse(true);
	return texture;
}
