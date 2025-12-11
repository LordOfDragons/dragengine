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

#include "deCanvasText.h"
#include "deCanvasVisitor.h"
#include "../font/deFont.h"
#include "../../common/exceptions.h"



// Class deCanvasText
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasText::deCanvasText(deCanvasManager *manager) :
deCanvas(manager),

pFontSize(10.0f){
}

deCanvasText::~deCanvasText(){
	SetPeerGraphic(nullptr);
}



// Management
///////////////

void deCanvasText::SetFont(deFont *font){
	if(font == pFont){
		return;
	}
	
	pFont = font;
	
	NotifyContentChanged();
}

void deCanvasText::SetFontSize(float size){
	if(size < 0.0f){
		size = 0.0f;
	}
	
	if(fabsf(size - pFontSize) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pFontSize = size;
	NotifyContentChanged();
}

void deCanvasText::SetText(const char *text){
	if(!text){
		DETHROW(deeInvalidParam);
	}
	
	if(pText == text){
		return;
	}
	
	pText = text;
	NotifyContentChanged();
}

void deCanvasText::SetColor(const decColor &color){
	if(pColor.IsEqualTo(color)){
		return;
	}
	
	pColor = color;
	NotifyContentChanged();
}



// Visiting
/////////////

void deCanvasText::Visit(deCanvasVisitor &visitor){
	visitor.VisitText(*this);
}
