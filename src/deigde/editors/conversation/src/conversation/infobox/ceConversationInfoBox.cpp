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

#include "ceConversationInfoBox.h"
#include "../ceConversation.h"
#include "../playback/cePlayback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceConversationInfoBox
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationInfoBox::ceConversationInfoBox(ceConversation &conversation) :
pConversation(conversation),
pBackgroundColor(1.0f, 1.0f, 0.0f, 0.5f),
pTextColor(1.0f, 1.0f, 1.0f, 1.0f),
pTextSize(18),
pPadding(10),
pWidth(0),
pHeight(0),

pCanvasView(NULL)
{
	try{
		pCanvasView.TakeOver(conversation.GetEngine()->GetCanvasManager()->CreateCanvasView());
		pCanvasView->SetOrder(12.0f);
		
		SetPathFont("/igde/fonts/sans_9_border.defont");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

ceConversationInfoBox::~ceConversationInfoBox(){
	pCleanUp();
}



// Management
///////////////

void ceConversationInfoBox::SetPathFont(const char *path){
	if(pPathFont == path){
		return;
	}
	
	pPathFont = path;
	pUpdateFont();
	pUpdateFontSize();
	UpdateCanvas();
}

void ceConversationInfoBox::SetBackgroundColor(const decColor &color){
	if(color.IsEqualTo(pBackgroundColor)){
		return;
	}
	
	pBackgroundColor = color;
	UpdateCanvas();
}

void ceConversationInfoBox::SetTextColor(const decColor &color){
	if(color.IsEqualTo(pTextColor)){
		return;
	}
	
	pTextColor = color;
	UpdateCanvas();
}

void ceConversationInfoBox::SetTextSize(int size){
	size = decMath::max(size, 1);
	
	if(pTextSize == size){
		return;
	}
	
	pTextSize = size;
	pUpdateFontSize();
	UpdateCanvas();
}

void ceConversationInfoBox::SetPadding(int padding){
	if(padding < 0){
		padding = 0;
	}
	
	if(padding == pPadding){
		return;
	}
	
	pPadding = padding;
	UpdateCanvas();
}



void ceConversationInfoBox::SetText(const char *text){
	if(!text){
		DETHROW(deeInvalidParam);
	}
	
	if(pText.Equals(text)){
		return;
	}
	
	pText = text;
	UpdateCanvas();
}



void ceConversationInfoBox::Clear(){
	SetText("");
}



void ceConversationInfoBox::UpdateCanvas(){
	// if there is no parent canvas there is no use in updating anything
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if(!parentView){
		return;
	}
	
	// update text to get the lines and required size
	pUpdateText();
	
	// remove all text canvas
	pCanvasView->RemoveAllCanvas();
	
	// resize canvas
	pCanvasView->SetSize(decPoint(pWidth, pHeight));
	pCanvasView->SetPosition(decPoint((parentView->GetSize().x - pWidth) / 2, 10));
	
	// make canvas visible if any text is inside
	pCanvasView->SetVisible(pLayoutTexts.GetCount() > 0);
	
	// add background color canvas
	deCanvasManager &canvasManager = *pConversation.GetEngine()->GetCanvasManager();
	deCanvasPaint *canvasBackground = NULL;
	try{
		canvasBackground = canvasManager.CreateCanvasPaint();
		canvasBackground->SetShapeType(deCanvasPaint::estRectangle);
		canvasBackground->SetFillColor(pBackgroundColor);
		canvasBackground->SetLineColor(decColor(pBackgroundColor * 0.25f, pBackgroundColor.a));
		canvasBackground->SetThickness(1.0f);
		canvasBackground->SetOrder(0.0f);
		canvasBackground->SetSize(pCanvasView->GetSize());
		pCanvasView->AddCanvas(canvasBackground);
		canvasBackground->FreeReference();
		
	}catch(const deException &){
		throw;
	}
	
	// add text canvas
	if(pEngFont){
		const int textCount = pLayoutTexts.GetCount();
		deCanvasText *canvasText = NULL;
		int y = pPadding;
		int i;
		
		try{
			for(i=0; i<textCount; i++){
				const decString &text = pLayoutTexts.GetAt(i);
				const int lineWidth = pLayoutWidths.GetAt(i);
				
				canvasText = canvasManager.CreateCanvasText();
				canvasText->SetColor(pTextColor);
				canvasText->SetFont(pEngFont);
				canvasText->SetFontSize((float)pTextSize);
				canvasText->SetText(text);
				canvasText->SetOrder((float)(1 + i));
				canvasText->SetPosition(decPoint((pWidth - lineWidth) / 2, y));
				canvasText->SetSize(decPoint(lineWidth, pTextSize));
				pCanvasView->AddCanvas(canvasText);
				canvasText->FreeReference();
				canvasText = NULL;
				
				y += pTextSize;
			}
			
		}catch(const deException &){
			throw;
		}
	}
}



// Private Functions
//////////////////////

void ceConversationInfoBox::pCleanUp(){
}



void ceConversationInfoBox::pUpdateFont(){
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

void ceConversationInfoBox::pUpdateFontSize(){
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

void ceConversationInfoBox::pUpdateText(){
	pLayoutTexts.RemoveAll();
	pLayoutWidths.RemoveAll();
	
	pWidth = pPadding * 2;
	pHeight = pPadding * 2;
	
	if(pText.IsEmpty() || !pEngFont){
		return;
	}
	
	decUTF8Decoder utf8Decoder;
	utf8Decoder.SetString(pText);
	
	const float scale = (float)pTextSize / (float)(pEngFontSize
		? pEngFontSize->GetLineHeight() : pEngFont->GetLineHeight());
	const int textLength = utf8Decoder.GetLength();
	int textHeight = 0;
	int textWidth = 0;
	int lineWidth = 0;
	int lineStart = 0;
	
	while(utf8Decoder.GetPosition() < textLength){
		const int position = utf8Decoder.GetPosition();
		const int character = utf8Decoder.DecodeNextCharacter();
		if(character < 0){
			continue; // invalid unicode character
		}
		
		if(character == '\n'){
			pLayoutTexts.Add(pText.GetMiddle(lineStart, position));
			pLayoutWidths.Add(lineWidth);
			textHeight += pTextSize;
			lineWidth = 0;
			lineStart = utf8Decoder.GetPosition();
			
		}else{
			lineWidth += (int)((float)pEngFont->GetGlyph(character, pEngFontSize).GetAdvance() * scale);
			if(lineWidth > textWidth){
				textWidth = lineWidth;
			}
		}
	}
	
	if(lineStart < textLength){
		pLayoutTexts.Add(pText.GetMiddle(lineStart, textLength));
		pLayoutWidths.Add(lineWidth);
		textHeight += pTextSize;
	}
	
	pWidth += textWidth;
	pHeight += textHeight;
}
