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

#include "deoglRenderableColorTexture.h"
#include "deoglRenderableColorTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorTextureManager
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorTextureManager::deoglRenderableColorTextureManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}

deoglRenderableColorTextureManager::~deoglRenderableColorTextureManager(){
}



// Management
///////////////

const deoglRenderableColorTexture *deoglRenderableColorTextureManager::GetTextureAt(int index) const{
	return pTextures.GetAt(index);
}

deoglRenderableColorTexture *deoglRenderableColorTextureManager::GetTextureWith(int width, int height, int componentCount, bool isFloat){
	// find the texture with the matching format
	auto found = pTextures.FindOrNull([&](const deoglRenderableColorTexture &t){
		return !t.GetInUse() && t.Matches(width, height, componentCount, isFloat);
	});
	if(found){
		found->SetInUse(true);
		return found;
	}
	
	// if not found create a new one
	auto texture = deTUniqueReference<deoglRenderableColorTexture>::New(
		pRenderThread, width, height, componentCount, isFloat);
	texture->SetInUse(true);
	pTextures.Add(std::move(texture));
	return pTextures.Last();
}
