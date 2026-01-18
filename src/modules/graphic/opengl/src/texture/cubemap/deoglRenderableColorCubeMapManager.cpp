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

#include "deoglRenderableColorCubeMap.h"
#include "deoglRenderableColorCubeMapManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorCubeMapManager
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorCubeMapManager::deoglRenderableColorCubeMapManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}

deoglRenderableColorCubeMapManager::~deoglRenderableColorCubeMapManager(){
}



// Management
///////////////

const deoglRenderableColorCubeMap *deoglRenderableColorCubeMapManager::GetCubeMapAt(int index) const{
	return pCubeMaps.GetAt(index);
}

deoglRenderableColorCubeMap *deoglRenderableColorCubeMapManager::GetCubeMapWith(int size, int componentCount, bool isFloat){
	// find the cubemap with the matching format
	deoglRenderableColorCubeMap *found = pCubeMaps.FindOrNull([&](const deoglRenderableColorCubeMap &c){
		return !c.GetInUse() && c.Matches(size, componentCount, isFloat);
	});
	if(found){
		found->SetInUse(true);
		return found;
	}
	
	// if not found create a new one
	auto cubemap = deTUniqueReference<deoglRenderableColorCubeMap>::New(pRenderThread, size, componentCount, isFloat);
	cubemap->SetInUse(true);
	pCubeMaps.Add(std::move(cubemap));
	return pCubeMaps.Last();
}
