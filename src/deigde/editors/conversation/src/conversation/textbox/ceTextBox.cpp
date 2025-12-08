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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceTextBox.h"
#include "ceTextBoxText.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceTextBox
////////////////////

// Constructor, destructor
////////////////////////////

ceTextBox::ceTextBox(deEngine &engine, deLogger &logger) :
pEngine(engine),
pLogger(logger),
pBackgroundColor(0.0f, 0.0f, 0.0f, 0.5f),
pTextColor(1.0f, 1.0f, 1.0f, 1.0f),
pTextSize(18),
pPadding(10),
pTextOffset(200),
pCanvasView(NULL)
{
	try{
		pCanvasView.TakeOver(engine.GetCanvasManager()->CreateCanvasView());
		pCanvasView->SetOrder(10.0f);
		
		SetPathFont("/igde/fonts/sans_9_border.defont");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

ceTextBox::~ceTextBox(){
	pCleanUp();
}



// Management
///////////////

void ceTextBox::SetPathFont(const char *path){
	if(pPathFont == path){
		return;
	}
	
	pPathFont = path;
	pUpdateFont();
	pUpdateFontSize();
	UpdateCanvas();
}

void ceTextBox::SetBackgroundColor(const decColor &color){
	if(color.IsEqualTo(pBackgroundColor)){
		return;
	}
	
	pBackgroundColor = color;
	UpdateCanvas();
}

void ceTextBox::SetTextColor(const decColor &color){
	if(color.IsEqualTo(pTextColor)){
		return;
	}
	
	pTextColor = color;
	UpdateCanvas();
}

void ceTextBox::SetTextSize(int size){
	size = decMath::max(size, 1);
	
	if(pTextSize == size){
		return;
	}
	
	pTextSize = size;
	pUpdateFontSize();
	UpdateCanvas();
}

void ceTextBox::SetPadding(int padding){
	if(padding < 0){
		padding = 0;
	}
	
	if(padding == pPadding){
		return;
	}
	
	pPadding = padding;
	UpdateCanvas();
}

void ceTextBox::SetTextOffset(int offset){
	if(offset < 0){
		offset = 0;
	}
	
	if(pTextOffset == offset){
		return;
	}
	
	pTextOffset = offset;
	UpdateCanvas();
}



void ceTextBox::UpdateCanvas(){
	// if there is no parent canvas there is no use in updating anything
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if(!parentView){
		return;
	}
	
	// remove all text canvas
	pCanvasView->RemoveAllCanvas();
	
	// calculate required box size
	const int count = pTexts.GetCount();
	int boxHeight = pPadding * 2;
	int i;
	
	for(i=0; i<count; i++){
		ceTextBoxText &text = *pTexts.GetAt(i);
		
		text.Layout(*this);
		boxHeight += text.GetCanvasView()->GetSize().y;
	}
	
	// keep box at a height of at least 20 pixel
	if(pEngFont){
		boxHeight = decMath::max(boxHeight, pPadding * 2 + pTextSize);
	}
	
	// resize canvas
	const decPoint &parentSize = parentView->GetSize();
	
	pCanvasView->SetSize(decPoint(parentSize.x, boxHeight));
	pCanvasView->SetPosition(decPoint(0, parentSize.y - boxHeight));
	
	// add background color canvas
	deCanvasPaint *canvasBackground = NULL;
	try{
		canvasBackground = pEngine.GetCanvasManager()->CreateCanvasPaint();
		canvasBackground->SetShapeType(deCanvasPaint::estRectangle);
		canvasBackground->SetFillColor(pBackgroundColor);
		canvasBackground->SetLineColor(decColor(pBackgroundColor, 0.0f));
		canvasBackground->SetThickness(0.0f);
		canvasBackground->SetOrder(0.0f);
		canvasBackground->SetSize(pCanvasView->GetSize());
		pCanvasView->AddCanvas(canvasBackground);
		canvasBackground->FreeReference();
		
	}catch(const deException &){
		throw;
	}
	
	// add text canvas
	int y = pPadding;
	
	for(i=0; i<count; i++){
		deCanvasView * const textCanvas = pTexts.GetAt(i)->GetCanvasView();
		const int height = textCanvas->GetSize().y;
		
		textCanvas->SetPosition(decPoint(0, y));
		textCanvas->SetOrder((float)(1 + i));
		pCanvasView->AddCanvas(textCanvas);
		
		y += height;
	}
}



// Private Functions
//////////////////////

void ceTextBox::pCleanUp(){
	pTexts.RemoveAll();
	
}



void ceTextBox::pUpdateFont(){
	try{
		if(!pPathFont.IsEmpty()){
			pEngFont.TakeOver(pEngine.GetFontManager()->LoadFont(pPathFont, "/"));
			
		}else{
			pEngFont = nullptr;
		}
		
	}catch(const deException &e){
		pLogger.LogException(LOGSOURCE, e);
	}
}

void ceTextBox::pUpdateFontSize(){
	if(!pEngFont){
		pEngFontSize = nullptr;
		return;
	}
	
	try{
		pEngFontSize = pEngFont->PrepareSize(pTextSize);
		
	}catch(const deException &e){
		pLogger.LogException(LOGSOURCE, e);
	}
}
