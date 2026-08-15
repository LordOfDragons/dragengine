/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deAvif3DImageInfo.h"


// Class deAvif3DImageInfo
////////////////////////////

// Constructor, destructor
////////////////////////////

deAvif3DImageInfo::deAvif3DImageInfo(const decString &filename) :
pFilename(filename),
pWidth(0),
pHeight(0),
pDepth(1),
pComponentCount(3),
pBitCount(8),
pHasAlpha(false),
pIsGrayscale(false){
}

deAvif3DImageInfo::~deAvif3DImageInfo(){
}



// Management
///////////////

int deAvif3DImageInfo::GetWidth(){
	return pWidth;
}

int deAvif3DImageInfo::GetHeight(){
	return pHeight;
}

int deAvif3DImageInfo::GetDepth(){
	return pDepth;
}

int deAvif3DImageInfo::GetComponentCount(){
	if(pIsGrayscale && pHasAlpha){
		return 2;
		
	}else if(pIsGrayscale){
		return 1;
		
	}else if(pHasAlpha){
		return 4;
		
	}else{
		return 3;
	}
}

int deAvif3DImageInfo::GetBitCount(){
	return pBitCount;
}
