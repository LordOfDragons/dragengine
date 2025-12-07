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

#include "deoglRCanvasText.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>


// Class deoglRCanvasText
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasText::deoglRCanvasText(deoglRenderThread &renderThread) :
deoglRCanvas(renderThread),
pTextSize(1)
{
	LEAK_CHECK_CREATE(renderThread, CanvasText);
}

deoglRCanvasText::~deoglRCanvasText(){
	LEAK_CHECK_FREE(GetRenderThread(), CanvasText);
}



// Management
///////////////

void deoglRCanvasText::SetFont(deoglRFont *font){
	pFont = font;
}

void deoglRCanvasText::SetFontSize(deoglRFontSize *size){
	pFontSize = size;
}

void deoglRCanvasText::SetTextSize(float size){
	pTextSize = decMath::max(size, 0.0f);
}

void deoglRCanvasText::SetText(const char *text){
	pText = text;
}

void deoglRCanvasText::SetColor(const decColor &color){
	pColor = color;
}


void deoglRCanvasText::Render(const deoglRenderCanvasContext &context){
	if(!pFont || pText.IsEmpty()){
		return;
	}
	
	const deoglRenderCanvasContext textContext(context, *this);
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasText(textContext, *this);
}
