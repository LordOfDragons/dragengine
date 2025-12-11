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

#include "deoglRFont.h"
#include "../renderthread/deoglRenderThread.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/resources/font/deFont.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoglRFont
/////////////////////

// Constructor, destructor
////////////////////////////

deoglRFont::deoglRFont(deoglRenderThread &renderThread, const deFont &font) :
pFilename(font.GetFilename()),
pIsColorFont(font.GetIsColorFont()),
pGlyphs(renderThread, font)
{
	pGlyphs.SetImage(font.GetImage());
	
	// register for delayed async res initialize and delayed opengl object creation
	renderThread.GetDelayedOperations().AddAsyncResInitFont(this);
	
	LEAK_CHECK_CREATE(renderThread, Font);
}

deoglRFont::~deoglRFont(){
	LEAK_CHECK_FREE(pRenderThread, Font);
	
	pGlyphs.GetRenderThread().GetDelayedOperations().RemoveAsyncResInitFont(this);
}


// Management
///////////////

void deoglRFont::FinalizeAsyncResLoading(){
	pGlyphs.FinalizeAsyncResLoading();
}


deoglRFontSize *deoglRFont::GetFontSizeFor(deFont &font, int lineHeight){
	const deFontSize * const size = font.EnsureSizePrepared(lineHeight);
	if(!size){
		return nullptr;
	}
	
	lineHeight = size->GetLineHeight();
	
	const deMutexGuard guard(pMutex);
	const int count = pSizes.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoglRFontSize * const rsize = (deoglRFontSize*)pSizes.GetAt(i);
		if(rsize->GetGlyphs().GetLineHeight() == lineHeight){
			return rsize;
		}
	}
	
	const deoglRFontSize::Ref rsize(deoglRFontSize::Ref::NewWith(pGlyphs.GetRenderThread(), *this, *size));
	pSizes.Add(rsize);
	return rsize;
}
