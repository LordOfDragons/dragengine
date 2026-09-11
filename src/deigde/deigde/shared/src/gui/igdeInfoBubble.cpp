/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeInfoBubble.h"
#include "igdeViewRenderWindow.h"
#include "resources/igdeFont.h"
#include "../environment/igdeEnvironment.h"
#include "../engine/igdeEngineController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>


// Class igdeInfoBubble
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeInfoBubble::igdeInfoBubble(igdeViewRenderWindow &view, float order) :
pView(view),
pPadding(3, 3, 3, 3)
{
	auto &manager = *view.GetEnvironment().GetEngineController()->GetEngine()->GetCanvasManager();
	
	pCanvasBubble = manager.CreateCanvasView();
	pCanvasBubble->SetOrder(order);
	pCanvasBubble->SetVisible(false);
	view.AddCanvas(pCanvasBubble);
	
	pCanvasBorder = manager.CreateCanvasPaint();
	pCanvasBorder->SetShapeType(deCanvasPaint::estRectangle);
	pCanvasBorder->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 0.5f));
	pCanvasBorder->SetLineColor(decColor(0.0f, 0.0f, 0.5f, 0.9f));
	pCanvasBorder->SetThickness(1.0f);
	pCanvasBorder->SetOrder(0.0f);
	pCanvasBubble->AddCanvas(pCanvasBorder);
	
	pCanvasContent = manager.CreateCanvasView();
	pCanvasContent->SetOrder(1.0f);
	pCanvasBubble->AddCanvas(pCanvasContent);
	
	igdeFont::sConfiguration fc;
	view.GetEnvironment().GetApplicationFont(fc);
	pFont = view.GetEnvironment().GetSharedFont(fc);
	
	pText = manager.CreateCanvasText();
	pText->SetFont(pFont->GetEngineFont());
	pText->SetFontSize((float)pFont->GetEngineFont()->GetLineHeight());
	pText->SetColor(decColor(1.0f, 1.0f, 1.0f));
	pText->SetOrder(0.0f);
	pCanvasContent->AddCanvas(pText);
}

igdeInfoBubble::~igdeInfoBubble(){
	if(pCanvasBubble){
		if(pCanvasBubble->GetParentView()){
			pView.RemoveCanvas(pCanvasBubble);
		}
		pCanvasBubble->RemoveAllCanvas();
	}
}


// Management
///////////////

void igdeInfoBubble::SetFont(const igdeFont::Ref &font){
	DEASSERT_NOTNULL(font)
	
	if(pFont == font){
		return;
	}
	
	pFont = font;
	pText->SetFont(pFont->GetEngineFont());
	pText->SetFontSize((float)pFont->GetEngineFont()->GetLineHeight());
	pResize();
}

const decColor &igdeInfoBubble::GetTextColor() const{
	return pText->GetColor();
}

void igdeInfoBubble::SetTextColor(const decColor &color){
	pText->SetColor(color);
}

const decString &igdeInfoBubble::GetText() const{
	return pText->GetText();
}

void igdeInfoBubble::SetText(const decString &text){
	if(pText->GetText() == text){
		return;
	}
	
	pText->SetText(text);
	pResize();
}

float igdeInfoBubble::GetTransparency() const{
	return pCanvasBubble->GetTransparency();
}

void igdeInfoBubble::SetTransparency(float transparency){
	pCanvasBubble->SetTransparency(decMath::clamp(transparency, 0.0f, 1.0f));
}

void igdeInfoBubble::SetPadding(const decBoundary &padding){
	pPadding.x1 = decMath::max(padding.x1, 0);
	pPadding.y1 = decMath::max(padding.y1, 0);
	pPadding.x2 = decMath::max(padding.x2, 0);
	pPadding.y2 = decMath::max(padding.y2, 0);
	pResize();
}


void igdeInfoBubble::PositionAroundMouse(const decPoint &position, decPoint &showPosition,
Placement &placement) const{
	showPosition = position + decPoint(32, 0);
	placement = igdeInfoBubble::Placement::topRight;
	
	if(position.x + pCanvasContent->GetSize().x + 6 > pView.GetRenderAreaSize().x){
		showPosition.x = position.x - 32;
		placement = igdeInfoBubble::Placement::topLeft;
	}
	
	if(position.y - pCanvasContent->GetSize().y - 6 < 0){
		showPosition.y = position.y + 64;
		placement = placement == igdeInfoBubble::Placement::topRight
			? igdeInfoBubble::Placement::bottomRight : igdeInfoBubble::Placement::bottomLeft;
	}
}

void igdeInfoBubble::ShowAt(const decPoint &position, Placement placement){
	pResize();
	pCanvasBubble->SetPosition(pPlace(position, placement));
	pCanvasBubble->SetVisible(true);
}

void igdeInfoBubble::ShowAroundMouse(const decPoint &position){
	decPoint showPosition;
	Placement placement;
	PositionAroundMouse(position, showPosition, placement);
	ShowAt(showPosition, placement);
}

void igdeInfoBubble::ShowCentered(){
	ShowCentered(0.5f, 0.5f);
}

void igdeInfoBubble::ShowCentered(float x, float y){
	const auto viewSize = pView.GetRenderAreaSize();
	
	pResize();
	const auto &infoSize = pCanvasContent->GetSize();
	
	pCanvasBubble->SetPosition(decPoint((viewSize.x - infoSize.x) * x, (viewSize.y - infoSize.y) * y));
	pCanvasBubble->SetVisible(true);
}

void igdeInfoBubble::Hide(){
	pCanvasBubble->SetVisible(false);
}


// Protected Functions
////////////////////////

void igdeInfoBubble::pResize(){
	const decPoint textSize(pText->GetFont()->TextSize(pText->GetText()));
	pText->SetSize(textSize);
	pCanvasContent->SetSize(textSize);
	
	const decPoint size(textSize + pPadding.GetSize());
	
	pCanvasContent->SetPosition(pPadding.GetTopLeft());
	pCanvasBorder->SetSize(size);
	pCanvasBubble->SetSize(size);
}

decPoint igdeInfoBubble::pPlace(const decPoint &position, Placement placement){
	const decPoint &size = pCanvasBubble->GetSize();
	decPoint npos;
	
	switch(placement){
	case Placement::topLeft:
		npos = position - size;
		break;
		
	case Placement::topRight:
		npos.x = position.x;
		npos.y = position.y - size.y;
		break;
		
	case Placement::bottomLeft:
		npos.x = position.x - size.x;
		npos.y = position.y;
		break;
		
	case Placement::bottomRight:
		npos = position;
		break;
	}
	
	return npos;
}
