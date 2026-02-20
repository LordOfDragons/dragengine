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

#include "deoglCanvasText.h"
#include "render/deoglRCanvasText.h"
#include "../deGraphicOpenGl.h"
#include "../font/deoglFont.h"
#include "../font/deoglRFont.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/font/deFont.h>



// Class deoglCanvasText
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasText::deoglCanvasText(deGraphicOpenGl &ogl, deCanvasText &canvas) :
deoglCanvas(ogl, canvas),
pCanvasText(canvas){
}

deoglCanvasText::~deoglCanvasText(){
}



// Management
///////////////

void deoglCanvasText::DropRCanvas(){
	pRCanvasText = nullptr;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasText::SyncContentToRender(){
	const deFont * const font = pCanvasText.GetFont();
	deoglRFont *rfont = nullptr;
	deoglRFontSize *rfontSize = nullptr;
	
	if(font){
		deoglFont &oglFont = *((deoglFont*)font->GetPeerGraphic());
		rfont = oglFont.GetRFont();
		if(rfont){
			rfontSize = oglFont.GetFontSizeFor((int)(pCanvasText.GetFontSize() + 0.5f));
		}
	}
	
	pRCanvasText->SetFont(rfont);
	pRCanvasText->SetFontSize(rfontSize);
	pRCanvasText->SetTextSize(pCanvasText.GetFontSize());
	pRCanvasText->SetColor(pCanvasText.GetColor());
	pRCanvasText->SetText(pCanvasText.GetText());
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasText::CreateRCanvas(){
	pRCanvasText = deoglRCanvasText::Ref::New(GetOgl().GetRenderThread());
	return pRCanvasText;
}
