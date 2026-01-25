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
	return unicode >= 0 && unicode < pGlyphMap.GetCount() ? *pGlyphMap.GetAt(unicode) : pUndefinedGlyph;
}

void deoglRFontGlyphs::pBuildGlyphs(const deFont &font){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects.
	//      in particular calling AddReference on render objects can lead to ugly bugs
	
	pSetGlyph(pUndefinedGlyph, font.GetUndefinedGlyph());
	
	const int glyphCount = font.GetGlyphCount();
	if(glyphCount == 0){
		return;
	}
	
	pGlyphs.SetCountDiscard(glyphCount);
	
	int maxUnicode = 0;
	pGlyphs.VisitIndexed([&](int i, sGlyph &glyph){
		const deFontGlyph &g = font.GetGlyphAt(i);
		pSetGlyph(glyph, g);
		maxUnicode = decMath::max(maxUnicode, g.GetUnicode());
	});
	
	if(maxUnicode > 0){
		pGlyphMap.SetAll(maxUnicode + 1, &pUndefinedGlyph);
		pGlyphs.VisitIndexed([&](int i, const sGlyph &glyph){
			pGlyphMap[font.GetGlyphAt(i).GetUnicode()] = &glyph;
		});
	}
}

void deoglRFontGlyphs::pBuildGlyphs(const deFontSize &size){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects.
	//      in particular calling AddReference on render objects can lead to ugly bugs
	
	pSetGlyph(pUndefinedGlyph, size.GetUndefinedGlyph());
	
	if(size.GetGlyphs().IsEmpty()){
		return;
	}
	
	pGlyphs.SetCountDiscard(size.GetGlyphs().GetCount());
	
	int maxUnicode = 0;
	size.GetGlyphs().VisitIndexed([&](int i, const deFontGlyph &g){
		pSetGlyph(pGlyphs[i], g);
		maxUnicode = decMath::max(maxUnicode, g.GetUnicode());
	});
	
	if(maxUnicode > 0){
		pGlyphMap.SetAll(maxUnicode + 1, &pUndefinedGlyph);
		pGlyphs.VisitIndexed([&](int i, const sGlyph &glyph){
			pGlyphMap[size.GetGlyphs()[i].GetUnicode()] = &glyph;
		});
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
	pGlyphs.VisitIndexed([&](int i, const sGlyph &g){
		l.LogInfoFormat("i=%d s=(%d,%d) b=(%d,%d) a=%d tc=(%f,%f,%f,%f;%d)",
			i, g.width, g.height, g.bearing, g.bearingY, g.advance, g.x1, g.y2, g.x2, g.y2, g.z);
	});
}


// Private Functions
//////////////////////

void deoglRFontGlyphs::pCleanUp(){
	SetImage(nullptr);
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
