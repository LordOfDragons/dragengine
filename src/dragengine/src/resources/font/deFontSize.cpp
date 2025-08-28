/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deFontSize.h"
#include "../../common/exceptions.h"


// Class deFontSize
/////////////////////

// Constructors, destructor
/////////////////////////////

deFontSize::deFontSize(int lineHeight, int glyphCount) :
pUndefinedGlyph{},
pGlyphs(nullptr),
pLineHeight(lineHeight),
pGlyphCount(0),
pFontWidth(0),
pBaseLine(lineHeight * 3 / 4)
{
	DEASSERT_TRUE(lineHeight > 0)
	DEASSERT_TRUE(glyphCount >= 0)
	DEASSERT_TRUE(glyphCount < 0xffff)
	
	if(glyphCount > 0){
		pGlyphs = new deFontGlyph[glyphCount];
		pGlyphCount = glyphCount;
	}
}

deFontSize::~deFontSize(){
	if(pGlyphs){
		delete [] pGlyphs;
		pGlyphs = nullptr;
		pGlyphCount = 0;
	}
}


// Management
///////////////

void deFontSize::SetBaseLine(int baseLine){
	DEASSERT_TRUE(baseLine >= 0)
	
	pBaseLine = baseLine;
}

void deFontSize::UpdateGlyphs(){
	pFontWidth = 0;
	
	int i;
	for(i=0; i<pGlyphCount; i++){
		if(pGlyphs[i].GetWidth() > pFontWidth){
			pFontWidth = pGlyphs[i].GetWidth();
		}
	}
}

deFontGlyph &deFontSize::GetGlyphAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pGlyphCount)
	
	return pGlyphs[index];
}

const deFontGlyph &deFontSize::GetGlyphAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pGlyphCount)
	
	return pGlyphs[index];
}

void deFontSize::SetImagePath(const char *path){
	pImagePath = path;
}

void deFontSize::SetImage(deImage *image){
	pImage = image;
}

void deFontSize::SetTaskLoad(deParallelTask *task){
	pTaskLoad = task;
}
