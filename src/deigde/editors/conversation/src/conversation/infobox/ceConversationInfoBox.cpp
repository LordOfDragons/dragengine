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

#include "ceConversationInfoBox.h"
#include "../ceConversation.h"
#include "../playback/cePlayback.h"

#include <deigde/gui/igdeApplication.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontSize.h>
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
pTextColor(1.0f, 1.0f, 1.0f),
pBorderColor(0.0f, 0.0f, 0.0f),
pBorderThickness(0.075f),
pTextSize(18),
pPadding(10),
pWidth(0),
pHeight(0)
{
	try{
		pCanvasView = conversation.GetEngine()->GetCanvasManager()->CreateCanvasView();
		pCanvasView->SetOrder(12.0f);
		
		SetPathFont("/igde/fonts/liberation/sans/regular.ttf");
		
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

void ceConversationInfoBox::SetBorderColor(const decColor &color){
	if(color.IsEqualTo(pBorderColor)){
		return;
	}
	
	pBorderColor = color;
	UpdateCanvas();
}

void ceConversationInfoBox::SetBorderThickness(float thickness){
	thickness = decMath::max(thickness, 0.0f);
	
	if(thickness == pBorderThickness){
		return;
	}
	
	pBorderThickness = thickness;
	UpdateCanvas();
}

void ceConversationInfoBox::SetTextSize(int size){
	size = decMath::max(size, 1);
	
	if(size == pTextSize){
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
	pCanvasView->SetVisible(pLayoutTexts.IsNotEmpty());
	
	// add background color canvas
	deCanvasManager &canvasManager = *pConversation.GetEngine()->GetCanvasManager();
	deCanvasPaint::Ref canvasBackground;
	canvasBackground = canvasManager.CreateCanvasPaint();
	canvasBackground->SetShapeType(deCanvasPaint::estRectangle);
	canvasBackground->SetFillColor(pBackgroundColor);
	canvasBackground->SetLineColor(decColor(pBackgroundColor * 0.25f, pBackgroundColor.a));
	canvasBackground->SetThickness(1.0f);
	canvasBackground->SetOrder(0.0f);
	canvasBackground->SetSize(pCanvasView->GetSize());
	pCanvasView->AddCanvas(canvasBackground);
	
	// add text canvas
	if(pEngFont){
		const int fontSize = igdeApplication::app().DisplayScaled(pTextSize);
		const int padding = igdeApplication::app().DisplayScaled(pPadding);
		const int textCount = pLayoutTexts.GetCount();
		int y = padding;
		int i;
		
		for(i=0; i<textCount; i++){
			const decString &text = pLayoutTexts.GetAt(i);
			const int lineWidth = pLayoutWidths.GetAt(i);
			
			const deCanvasText::Ref canvasText(canvasManager.CreateCanvasText());
			canvasText->SetColor(pTextColor);
			canvasText->SetFont(pEngFont);
			canvasText->SetFontSize((float)fontSize);
			canvasText->SetBorderColor(pBorderColor);
			canvasText->SetBorderThickness(pBorderThickness);
			canvasText->SetText(text);
			canvasText->SetOrder((float)(1 + i));
			canvasText->SetPosition(decPoint((pWidth - lineWidth) / 2, y));
			canvasText->SetSize(decPoint(lineWidth, fontSize));
			pCanvasView->AddCanvas(canvasText);
			
			y += fontSize;
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
			pEngFont = pConversation.GetEngine()->GetFontManager()->LoadFont(pPathFont, "/");
			
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
		pEngFontSize = pEngFont->PrepareSize(igdeApplication::app().DisplayScaled(pTextSize));
		
	}catch(const deException &e){
		pConversation.GetLogger()->LogException(LOGSOURCE, e);
	}
}

void ceConversationInfoBox::pUpdateText(){
	pLayoutTexts.RemoveAll();
	pLayoutWidths.RemoveAll();
	
	const int padding = igdeApplication::app().DisplayScaled(pPadding);
	pWidth = padding * 2;
	pHeight = padding * 2;
	
	if(pText.IsEmpty() || !pEngFont){
		return;
	}
	
	const int lineHeight = igdeApplication::app().DisplayScaled(pTextSize);
	const float dilate = pBorderThickness;
	
	const auto lines = decUnicodeString::NewFromUTF8(pText).Split('\n');
	int textWidth = 0;
	
	lines.Visit([&](const decUnicodeString &line){
		const auto textSize = pEngFont->TextSize(line, pEngFontSize, lineHeight, dilate);
		
		pLayoutTexts.Add(line.ToUTF8());
		pLayoutWidths.Add(textSize.x);
		
		textWidth = decMath::max(textWidth, textSize.x);
		pHeight += lineHeight;
	});
	
	pWidth += textWidth;
}
