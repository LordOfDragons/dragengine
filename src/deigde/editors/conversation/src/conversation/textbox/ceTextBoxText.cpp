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

#include "ceTextBox.h"
#include "ceTextBoxText.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasText.h>



// Class ceTextBoxText
////////////////////////

// Constructor, destructor
////////////////////////////

ceTextBoxText::ceTextBoxText() :
pCanvasView(NULL){
}

ceTextBoxText::~ceTextBoxText(){
}



// Management
///////////////

void ceTextBoxText::SetName(const decUnicodeString &name){
	pName = name;
}

void ceTextBoxText::SetText(const decUnicodeString &text){
	pText = text;
}

void ceTextBoxText::Layout(const ceTextBox &textBox){
	// create canvas if not existing
	deCanvasManager &canvasManager = *textBox.GetEngine().GetCanvasManager();
	if(!pCanvasView){
		pCanvasView = canvasManager.CreateCanvasView();
	}
	
	// clear canvas
	pCanvasView->RemoveAllCanvas();
	pCanvasView->SetSize(decPoint(10, 1));
	
	// get font. if not existing exit since we can not render anything this way
	deFont * const font = textBox.GetFont();
	if(!font){
		return;
	}
	
	// determine required height
	deCanvasView &parentVide = *textBox.GetCanvasView();
	const decPoint &parentSize = parentVide.GetSize();
	deCanvasText::Ref canvasText = NULL;
	
	const decString name(pName.ToUTF8());
	const decString text(pText.ToUTF8());
	const int padding = textBox.GetPadding();
	const int offset = textBox.GetTextOffset();
	
	const int textHeight = textBox.GetTextSize();
	const int top = 0;
	const int left = padding;
	const int right = parentSize.x - padding;
	
	pCanvasView->SetSize(decPoint(parentSize.x, textHeight));
	
	try{
		// create name canvas
		canvasText = canvasManager.CreateCanvasText();
		canvasText->SetColor(textBox.GetTextColor());
		canvasText->SetFont(font);
		canvasText->SetFontSize((float)textBox.GetTextSize());
		canvasText->SetText(name);
		canvasText->SetOrder(0.0f);
		canvasText->SetPosition(decPoint(left, top));
		canvasText->SetSize(decPoint(offset, textHeight));
		pCanvasView->AddCanvas(canvasText);
		canvasText = NULL;
		
		// create text canvas
		canvasText = canvasManager.CreateCanvasText();
		canvasText->SetColor(textBox.GetTextColor());
		canvasText->SetFont(font);
		canvasText->SetFontSize((float)textBox.GetTextSize());
		canvasText->SetText(text);
		canvasText->SetOrder(1.0f);
		canvasText->SetPosition(decPoint(left + offset, top));
		canvasText->SetSize(decPoint(right - offset - left, textHeight));
		pCanvasView->AddCanvas(canvasText);
		canvasText = NULL;
		
	}catch(const deException &){
		throw;
	}
}
