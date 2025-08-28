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

#include "deoglRFont.h"
#include "deoglRFontSize.h"
#include "../renderthread/deoglRenderThread.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/resources/font/deFontSize.h>


// Class deoglRFontSize
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRFontSize::deoglRFontSize(deoglRenderThread &renderThread,
	deoglRFont &font, const deFontSize &size) :
pFont(font),
pSize(size),
pGlyphs(renderThread, size)
{
	pGlyphs.SetImage(size.GetImage());
	
	// register for delayed async res initialize and delayed opengl object creation
	renderThread.GetDelayedOperations().AddAsyncResInitFontSize(this);
	
	LEAK_CHECK_CREATE(renderThread, FontSize);
}

deoglRFontSize::~deoglRFontSize(){
	LEAK_CHECK_FREE(pRenderThread, FontSize);
	
	pGlyphs.GetRenderThread().GetDelayedOperations().RemoveAsyncResInitFontSize(this);
}


// Management
///////////////

void deoglRFontSize::FinalizeAsyncResLoading(){
	pGlyphs.FinalizeAsyncResLoading();
}
