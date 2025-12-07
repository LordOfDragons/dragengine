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

#include "deoglCapsTextureFormat.h"
#include "deoglCapsTextureFormatList.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapsTextureFormatList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapsTextureFormatList::deoglCapsTextureFormatList() :
pFormats(NULL),
pFormatCount(0),
pFormatSize(0){
}

deoglCapsTextureFormatList::~deoglCapsTextureFormatList(){
	if(pFormats){
		while(pFormatCount > 0){
			pFormatCount--;
			delete pFormats[pFormatCount];
		}
		delete [] pFormats;
	}
}



// Management
///////////////

const deoglCapsTextureFormat *deoglCapsTextureFormatList::GetFormatAt(int index) const{
	if(index < 0 || index >= pFormatCount){
		DETHROW(deeInvalidParam);
	}
	return pFormats[index];
}

const deoglCapsTextureFormat *deoglCapsTextureFormatList::GetFormatWith(GLint format) const{
	int i;
	
	for(i=0; i<pFormatCount; i++){
		if(pFormats[i]->GetFormat() == format){
			return pFormats[i];
		}
	}
	
	return NULL;
}

void deoglCapsTextureFormatList::AddFormat(GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, bool isDepth, bool isDepthFloat, bool isStencil, bool isCompressed, const char *name){
	if(pFormatCount == pFormatSize){
		const int newSize = pFormatSize * 3 / 2 + 1;
		deoglCapsTextureFormat ** const newArray = new deoglCapsTextureFormat*[newSize];
		
		if(pFormats){
			memcpy(newArray, pFormats, sizeof(deoglCapsTextureFormat*) * pFormatSize);
			delete [] pFormats;
		}
		
		pFormats = newArray;
		pFormatSize = newSize;
	}
	
	pFormats[pFormatCount] = new deoglCapsTextureFormat(format, pixelFormat, pixelType,
		bitsPerPixel, isDepth, isDepthFloat, isStencil, isCompressed, name);
	pFormatCount++;
}
