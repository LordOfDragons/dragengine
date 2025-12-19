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

#include "deoglRFontGlyphs.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/deoglImage.h"
#include "../texture/deoglRImage.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>
#include <dragengine/resources/font/deFontGlyph.h>


// Class deoglRFontGlyphs
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRFontGlyphs::deoglRFontGlyphs(deoglRenderThread &renderThread, const deFont &font) :
pRenderThread(renderThread),
pGlyphs(nullptr),
pGlyphMap(nullptr),
pGlyphCount(0),
pGlyphMapCount(0),
pLineHeight(font.GetLineHeight()),
pDelayedImage(nullptr)
{
	try{
		pBuildGlyphs(font);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglRFontGlyphs::deoglRFontGlyphs(deoglRenderThread &renderThread, const deFontSize &size) :
pRenderThread(renderThread),
pGlyphs(nullptr),
pGlyphMap(nullptr),
pGlyphCount(0),
pGlyphMapCount(0),
pLineHeight(size.GetLineHeight()),
pDelayedImage(nullptr)
{
	try{
		pBuildGlyphs(size);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglRFontGlyphs::~deoglRFontGlyphs(){
	pCleanUp();
}


// Management
///////////////

void deoglRFontGlyphs::FinalizeAsyncResLoading(){
	// FinalizeAsyncResLoading is called from main thread
	if(!pDelayedImage){
		return;
	}
	
	// ensure image texture is created. this requires SyncToRender() called from the main
	// thread and PrepareForRender() called in the render thread using delayed operations.
	pDelayedImage->SyncToRender();
	
	pImage = pDelayedImage->GetRImage();
	pRenderThread.GetDelayedOperations().AddInitImage(pImage);
}

const deoglRFontGlyphs::sGlyph &deoglRFontGlyphs::GetGlyphFor(int unicode) const{
	return unicode >= 0 && unicode < pGlyphMapCount ? *pGlyphMap[unicode] : pUndefinedGlyph;
}

void deoglRFontGlyphs::pBuildGlyphs(const deFont &font){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects.
	//      in particular calling AddReference on render objects can lead to ugly bugs
	
	pSetGlyph(pUndefinedGlyph, font.GetUndefinedGlyph());
	
	const int glyphCount = font.GetGlyphCount();
	if(glyphCount == 0){
		return;
	}
	
	pGlyphs = new sGlyph[glyphCount];
	
	int i, maxUnicode = 0;
	for(i=0; i<glyphCount; i++){
		const deFontGlyph &g = font.GetGlyphAt(i);
		pSetGlyph(pGlyphs[i], g);
		maxUnicode = decMath::max(maxUnicode, g.GetUnicode());
	}
	pGlyphCount = glyphCount;
	
	if(maxUnicode > 0){
		pGlyphMap = new const sGlyph*[maxUnicode + 1];
		for(i=0; i<=maxUnicode; i++){
			pGlyphMap[i] = &pUndefinedGlyph;
		}
		for(i=0; i<glyphCount; i++){
			pGlyphMap[font.GetGlyphAt(i).GetUnicode()] = pGlyphs + i;
		}
		pGlyphMapCount = maxUnicode + 1;
	}
}

void deoglRFontGlyphs::pBuildGlyphs(const deFontSize &size){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects.
	//      in particular calling AddReference on render objects can lead to ugly bugs
	
	pSetGlyph(pUndefinedGlyph, size.GetUndefinedGlyph());
	
	const int glyphCount = size.GetGlyphCount();
	if(glyphCount == 0){
		return;
	}
	
	pGlyphs = new sGlyph[glyphCount];
	
	int i, maxUnicode = 0;
	for(i=0; i<glyphCount; i++){
		const deFontGlyph &g = size.GetGlyphAt(i);
		pSetGlyph(pGlyphs[i], g);
		maxUnicode = decMath::max(maxUnicode, g.GetUnicode());
	}
	pGlyphCount = glyphCount;
	
	if(maxUnicode > 0){
		pGlyphMap = new const sGlyph*[maxUnicode + 1];
		for(i=0; i<=maxUnicode; i++){
			pGlyphMap[i] = &pUndefinedGlyph;
		}
		for(i=0; i<glyphCount; i++){
			pGlyphMap[size.GetGlyphAt(i).GetUnicode()] = pGlyphs + i;
		}
		pGlyphMapCount = maxUnicode + 1;
	}
}

void deoglRFontGlyphs::SetImage(const deImage *image){
	if(pImage){
		pRenderThread.GetDelayedOperations().RemoveInitImage(pImage);
		pImage = nullptr;
	}
	
	if(image){
		pDelayedImage = (deoglImage*)image->GetPeerGraphic();
		
	}else{
		pDelayedImage = nullptr;
	}
}

void deoglRFontGlyphs::DebugPrint() const{
	deoglRTLogger &l = pRenderThread.GetLogger();
	int i;
	for(i=0; i<pGlyphCount; i++){
		const sGlyph &g = pGlyphs[i];
		l.LogInfoFormat("i=%d s=(%d,%d) b=(%d,%d) a=%d tc=(%f,%f,%f,%f;%d)",
			i, g.width, g.height, g.bearing, g.bearingY, g.advance, g.x1, g.y2, g.x2, g.y2, g.z);
	}
}


// Private Functions
//////////////////////

void deoglRFontGlyphs::pCleanUp(){
	SetImage(nullptr);
	if(pGlyphMap){
		delete [] pGlyphMap;
	}
	if(pGlyphs){
		delete [] pGlyphs;
	}
}

void deoglRFontGlyphs::pSetGlyph(sGlyph &target, const deFontGlyph &source){
	target.x1 = (float)source.GetX();
	target.y1 = (float)source.GetY();
	target.x2 = (float)(source.GetX() + source.GetWidth());
	target.y2 = (float)(source.GetY() + source.GetHeight());
	target.z = source.GetZ();
	target.bearing = source.GetBearing();
	target.bearingY = source.GetBearingY();
	target.width = source.GetWidth();
	target.height = source.GetHeight();
	target.advance = source.GetAdvance();
}
