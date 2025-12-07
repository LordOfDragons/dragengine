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

#include "deoglRenderableDepthCubeMap.h"
#include "deoglRenderableDepthCubeMapManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthCubeMapManager
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthCubeMapManager::deoglRenderableDepthCubeMapManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pCubeMaps(NULL),
pCubeMapCount(0),
pCubeMapSize(0){
}

deoglRenderableDepthCubeMapManager::~deoglRenderableDepthCubeMapManager(){
	if(pCubeMaps){
		while(pCubeMapCount > 0){
			pCubeMapCount--;
			delete pCubeMaps[pCubeMapCount];
		}
		delete [] pCubeMaps;
	}
}



// Management
///////////////

const deoglRenderableDepthCubeMap *deoglRenderableDepthCubeMapManager::GetCubeMapAt(int index) const{
	if(index < 0 || index >= pCubeMapCount) DETHROW(deeInvalidParam);
	
	return pCubeMaps[index];
}

deoglRenderableDepthCubeMap *deoglRenderableDepthCubeMapManager::GetCubeMapWith(int size, bool useFloat){
	deoglRenderableDepthCubeMap *cubemap = NULL;
	int i;
	
	// find the cubemap with the matching format
	for(i=0; i<pCubeMapCount; i++){
		if(! pCubeMaps[i]->GetInUse() && pCubeMaps[i]->Matches(size, useFloat)){
			cubemap = pCubeMaps[i];
			break;
		}
	}
	
	// if not found create a new one
	if(! cubemap){
		if(pCubeMapCount == pCubeMapSize){
			int newSize = pCubeMapSize * 3 / 2 + 1;
			deoglRenderableDepthCubeMap **newArray = new deoglRenderableDepthCubeMap*[newSize];
			if(! newArray) DETHROW(deeOutOfMemory);
			if(pCubeMaps){
				memcpy(newArray, pCubeMaps, sizeof(deoglRenderableDepthCubeMap*) * pCubeMapSize);
				delete [] pCubeMaps;
			}
			pCubeMaps = newArray;
			pCubeMapSize = newSize;
		}
		
		cubemap = new deoglRenderableDepthCubeMap(pRenderThread, size, useFloat);
		
		pCubeMaps[pCubeMapCount] = cubemap;
		pCubeMapCount++;
	}
	
	// mark the cubemap in use and return it
	cubemap->SetInUse(true);
	return cubemap;
}
