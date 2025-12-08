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

#include "cePCBOption.h"
#include "cePlayerChoiceBox.h"
#include "../ceConversation.h"
#include "../playback/cePlayback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include "../playback/cePlaybackActionStack.h"
#include "../action/ceConversationAction.h"



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class cePlayerChoiceBox
////////////////////

// Constructor, destructor
////////////////////////////

cePlayerChoiceBox::cePlayerChoiceBox(ceConversation &conversation) :
pConversation(conversation),
pBackgroundColor(0.0f, 0.0f, 0.0f, 0.5f),
pTextColor(1.0f, 1.0f, 1.0f, 1.0f),
pSelectedBackgroundColor(0.0f, 0.0f, 0.5f, 0.5f),
pSelectedTextColor(1.0f, 0.5f, 0.5f, 1.0f),
pTextSize(18),
pPadding(10),
pPlaybackStackDepth(-1),

pSelectedOption(-1),

pCanvasView(NULL)
{
	try{
		pCanvasView.TakeOver(conversation.GetEngine()->GetCanvasManager()->CreateCanvasView());
		pCanvasView->SetOrder(11.0f);
		
		SetPathFont("/igde/fonts/sans_9_border.defont");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

cePlayerChoiceBox::~cePlayerChoiceBox(){
	pCleanUp();
}



// Management
///////////////

void cePlayerChoiceBox::SetPathFont(const char *path){
	if(pPathFont == path){
		return;
	}
	
	pPathFont = path;
	pUpdateFont();
	pUpdateFontSize();
	UpdateCanvas();
}

void cePlayerChoiceBox::SetBackgroundColor(const decColor &color){
	pBackgroundColor = color;
}

void cePlayerChoiceBox::SetTextColor(const decColor &color){
	if(color.IsEqualTo(pTextColor)){
		return;
	}
	
	pTextColor = color;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetTextSize(int size){
	size = decMath::max(size, 1);
	
	if(pTextSize == size){
		return;
	}
	
	pTextSize = size;
	pUpdateFontSize();
	UpdateCanvas();
}

void cePlayerChoiceBox::SetSelectedBackgroundColor(const decColor &color){
	if(color.IsEqualTo(pSelectedBackgroundColor)){
		return;
	}
	
	pSelectedBackgroundColor = color;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetSelectedTextColor(const decColor &color){
	if(color.IsEqualTo(pSelectedTextColor)){
		return;
	}
	
	pSelectedTextColor = color;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetPadding(int padding){
	if(padding < 0){
		padding = 0;
	}
	
	if(padding == pPadding){
		return;
	}
	
	pPadding = padding;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetPlaybackStackDepth(int depth){
	pPlaybackStackDepth = depth;
}

void cePlayerChoiceBox::SetSelectedOption(int index){
	if(index < -1 || index >= pOptions.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(pSelectedOption == index){
		return;
	}
	
	pSelectedOption = index;
	UpdateCanvas();
}

void cePlayerChoiceBox::Clear(){
	pOptions.RemoveAll();
	pSelectedOption = -1;
	UpdateCanvas();
}

int cePlayerChoiceBox::IndexOfOptionAt(int x, int y) const{
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if(!parentView){
		return -1;
	}
	
	const decPoint &boxPosition = pCanvasView->GetPosition();
	const int count = pOptions.GetCount();
	const decPoint point(x, y);
	int i;
	
	for(i=0; i<count; i++){
		const deCanvasView * const canvasOption = pOptions.GetAt(i)->GetCanvasView();
		if(!canvasOption){
			continue;
		}
		
		const decPoint position(boxPosition + canvasOption->GetPosition());
		const decPoint &size = canvasOption->GetSize();
		
		if(point >= position && point < position + size){
			return i;
		}
	}
	
	return -1;
}

void cePlayerChoiceBox::SelectOptionAt(int x, int y){
	const int index = IndexOfOptionAt(x, y);
	
	if(index != -1){
		SetSelectedOption(index);
	}
}

void cePlayerChoiceBox::UpdateCanvas(){
	// if there is no parent canvas there is no use in updating anything
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if(!parentView){
		return;
	}
	
	// remove all text canvas
	pCanvasView->RemoveAllCanvas();
	
	// make sure the selected option is in valid range
	const int count = pOptions.GetCount();
	
	if(count > 0){
		pSelectedOption = decMath::clamp(pSelectedOption, 0, count - 1);
		
	}else{
		pSelectedOption = -1;
	}
	
	// calculate required box size
	int boxHeight = pPadding * 2;
	int i;
	
	for(i=0; i<count; i++){
		cePCBOption &option = *pOptions.GetAt(i);
		
		option.Layout(*this, i == pSelectedOption);
		boxHeight += option.GetCanvasView()->GetSize().y;
	}
	
	boxHeight = decMath::max(boxHeight, pPadding * 2);
	
	// resize canvas
	const decPoint &parentSize = parentView->GetSize();
	
	pCanvasView->SetPosition(decPoint(0, (parentSize.y - boxHeight) / 2));
	pCanvasView->SetSize(decPoint(parentSize.x, boxHeight));
	
	// add option canvas
	int y = pPadding;
	
	for(i=0; i<count; i++){
		deCanvasView * const textCanvas = pOptions.GetAt(i)->GetCanvasView();
		const int height = textCanvas->GetSize().y;
		
		textCanvas->SetPosition(decPoint(0, y));
		textCanvas->SetOrder((float)(1 + i));
		pCanvasView->AddCanvas(textCanvas);
		
		y += height;
	}
}



// Private Functions
//////////////////////

void cePlayerChoiceBox::pCleanUp(){
	pOptions.RemoveAll();
	
}



void cePlayerChoiceBox::pUpdateFont(){
	pEngFontSize = nullptr;
	
	try{
		if(!pPathFont.IsEmpty()){
			pEngFont.TakeOver(pConversation.GetEngine()->GetFontManager()->LoadFont(pPathFont, "/"));
			
		}else{
			pEngFont = nullptr;
		}
		
	}catch(const deException &e){
		pConversation.GetLogger()->LogException(LOGSOURCE, e);
	}
}

void cePlayerChoiceBox::pUpdateFontSize(){
	if(!pEngFont){
		pEngFontSize = nullptr;
		return;
	}
	
	try{
		pEngFontSize = pEngFont->PrepareSize(pTextSize);
		
	}catch(const deException &e){
		pConversation.GetLogger()->LogException(LOGSOURCE, e);
	}
}
