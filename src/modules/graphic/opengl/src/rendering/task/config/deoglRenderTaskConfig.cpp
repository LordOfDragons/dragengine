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

#include <string.h>

#include "deoglRenderTaskConfig.h"
#include "deoglRenderTaskConfigTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskConfig
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskConfig::deoglRenderTaskConfig() :
pTextures(NULL),
pTextureCount(0),
pTextureSize(0){
}

deoglRenderTaskConfig::~deoglRenderTaskConfig(){
	if(pTextures){
		delete [] pTextures;
	}
}



// Management
///////////////

deoglRenderTaskConfigTexture &deoglRenderTaskConfig::GetTextureAt(int index) const{
	if(index < 0 || index >= pTextureCount){
		DETHROW(deeInvalidParam);
	}
	return pTextures[index];
}

deoglRenderTaskConfigTexture &deoglRenderTaskConfig::AddTexture(){
	if(pTextureCount == pTextureSize){
		const int newSize = pTextureCount * 3 / 2 + 1;
		deoglRenderTaskConfigTexture * const newArray = new deoglRenderTaskConfigTexture[newSize];
		if(pTextures){
			memcpy(newArray, pTextures, sizeof(deoglRenderTaskConfigTexture) * pTextureCount);
			delete [] pTextures;
		}
		pTextures = newArray;
		pTextureSize = newSize;
	}
	
	deoglRenderTaskConfigTexture &texture = pTextures[pTextureCount++];
	texture.Clear();
	return texture;
}

void deoglRenderTaskConfig::RemoveAllTextures(){
	pTextureCount = 0;
}
