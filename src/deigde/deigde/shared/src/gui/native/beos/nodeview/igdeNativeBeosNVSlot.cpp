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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosNVSlot.h"
#include "igdeNativeBeosNVSocket.h"
#include "igdeNativeBeosNVBoard.h"
#include "../../../nodeview/igdeNVSlot.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeNativeBeosNVSlot
//////////////////////////////

igdeNativeBeosNVSlot::igdeNativeBeosNVSlot(igdeNVSlot &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 100, 18), "nvSlot", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
	B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
pOwner(&owner),
pFont(NVSlotFont(owner, guitheme)),
pSocket(nullptr),
pIsDnd(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	
	pSocket = new igdeNativeBeosNVSocket(this, guitheme);
	AddChild(pSocket);
	
	if(owner.GetIsInput()){
		pSocket->MoveTo(0.0f, 1.0f);
	}else{
		pSocket->MoveTo(Bounds().Width() - pSocket->Bounds().Width(), 1.0f);
	}
	
	if(parent){
		parent->AddChild(this);
	}
	
	UpdateText();
	UpdateDescription();
	UpdateEnabled();
	UpdateColor();
	UpdateLinkedState();
}

igdeNativeBeosNVSlot::~igdeNativeBeosNVSlot() = default;



igdeNativeBeosNVSlot *igdeNativeBeosNVSlot::CreateNativeWidget(igdeNVSlot &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BView * const parent = (BView*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosNVSlot(owner, parent, *owner.GetGuiTheme());
}

void igdeNativeBeosNVSlot::PostCreateNativeWidget(){
}

void igdeNativeBeosNVSlot::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

igdeNativeBeosNVBoard *igdeNativeBeosNVSlot::GetNativeBoard() const{
	if(!pOwner->GetOwnerNode() || !pOwner->GetOwnerNode()->GetOwnerBoard()){
		return nullptr;
	}
	return (igdeNativeBeosNVBoard*)pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget();
}

void igdeNativeBeosNVSlot::UpdateText(){
	Invalidate();
}

void igdeNativeBeosNVSlot::UpdateDescription(){
	SetToolTipText(pOwner->GetDescription());
}

void igdeNativeBeosNVSlot::UpdateEnabled(){
	Invalidate();
}

void igdeNativeBeosNVSlot::UpdateColor(){
	if(pSocket){
		const decColor &c = pOwner->GetColor();
		pSocket->SetColorFillChecked({(uint8)(c.r * 255.0f), (uint8)(c.g * 255.0f),
			(uint8)(c.b * 255.0f), 255});
	}
	Invalidate();
}

void igdeNativeBeosNVSlot::UpdateLinkedState(){
	if(pSocket){
		pSocket->SetChecked(pOwner->GetLinks().IsNotEmpty());
	}
}



decPoint igdeNativeBeosNVSlot::GetCenter() const{
	if(!pSocket){
		return decPoint();
	}
	const decPoint sc(pSocket->GetCenter());
	BPoint p(sc.x, sc.y);
	pSocket->ConvertToParent(&p);
	return decPoint((int)p.x, (int)p.y);
}

static decPoint beosConvertToBoard(BView *view, const decPoint &point,
	const igdeNVNode *ownerNode, const igdeNVBoard *ownerBoard){
	if(!ownerNode || !ownerBoard){
		return point;
	}
	BView * const boardView = (BView*)ownerBoard->GetNativeContainer();
	if(!boardView){
		return point;
	}
	BPoint p((float)point.x, (float)point.y);
	view->ConvertToScreen(&p);
	boardView->ConvertFromScreen(&p);
	return decPoint((int)p.x, (int)p.y);
}

static decPoint beosConvertToNode(BView *view, const decPoint &point,
	const igdeNVNode *ownerNode){
	if(!ownerNode){
		return point;
	}
	BView * const nodeView = (BView*)ownerNode->GetNativeWidget();
	if(!nodeView){
		return point;
	}
	BPoint p((float)point.x, (float)point.y);
	view->ConvertToScreen(&p);
	nodeView->ConvertFromScreen(&p);
	return decPoint((int)p.x, (int)p.y);
}

decPoint igdeNativeBeosNVSlot::GetCenterNode() const{
	return beosConvertToNode(const_cast<igdeNativeBeosNVSlot*>(this),
		GetCenter(), pOwner->GetOwnerNode());
}

decPoint igdeNativeBeosNVSlot::GetCenterBoard() const{
	return beosConvertToBoard(const_cast<igdeNativeBeosNVSlot*>(this),
		GetCenter(), pOwner->GetOwnerNode(), 
		pOwner->GetOwnerNode() ? pOwner->GetOwnerNode()->GetOwnerBoard() : nullptr);
}

decPoint igdeNativeBeosNVSlot::GetConnector() const{
	if(!pSocket){
		return decPoint();
	}
	const decPoint connector(pOwner->GetIsInput()
		? pSocket->GetBorderLeft() : pSocket->GetBorderRight());
	BPoint p((float)connector.x, (float)connector.y);
	pSocket->ConvertToParent(&p);
	return decPoint((int)p.x, (int)p.y);
}

decPoint igdeNativeBeosNVSlot::GetConnectorNode() const{
	return beosConvertToNode(const_cast<igdeNativeBeosNVSlot*>(this),
		GetConnector(), pOwner->GetOwnerNode());
}

decPoint igdeNativeBeosNVSlot::GetConnectorBoard() const{
	return beosConvertToBoard(const_cast<igdeNativeBeosNVSlot*>(this),
		GetConnector(), pOwner->GetOwnerNode(),
		pOwner->GetOwnerNode() ? pOwner->GetOwnerNode()->GetOwnerBoard() : nullptr);
}



igdeFont *igdeNativeBeosNVSlot::NVSlotFont(const igdeNVSlot &owner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::nodeViewSlotFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::nodeViewSlotFontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::nodeViewSlotFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::nodeViewSlotFontSize, 1.0f);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return owner.GetEnvironment().GetSharedFont(configuration);
}



// BView overrides
///////////////////

void igdeNativeBeosNVSlot::Draw(BRect){
	const BRect bounds = Bounds();
	rgb_color bgColor = {200, 200, 200, 255};
	if(Parent()){
		bgColor = Parent()->ViewColor();
	}
	
	SetLowColor(bgColor);
	SetHighColor(bgColor);
	FillRect(bounds);
	
	if(!pOwner->GetEnabled()){
		SetHighColor({128, 128, 128, 255});
	}else{
		const decColor &c = pOwner->GetColor();
		SetHighColor({(uint8)(c.r * 255.0f), (uint8)(c.g * 255.0f), (uint8)(c.b * 255.0f), 255});
	}
	
	BFont font;
	if(pFont){
		font = *(BFont*)pFont->GetNativeFont();
	}
	SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	const float textY = bounds.top + (bounds.Height() + fh.ascent - fh.descent) * 0.5f;
	const float socketW = pSocket ? pSocket->Bounds().Width() + 4.0f : 0.0f;
	
	if(pOwner->GetIsInput()){
		DrawString(pOwner->GetText(), BPoint(bounds.left + socketW, textY));
	}else{
		const float textW = font.StringWidth(pOwner->GetText());
		DrawString(pOwner->GetText(), BPoint(bounds.right - socketW - textW, textY));
	}
}

void igdeNativeBeosNVSlot::MouseDown(BPoint where){
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	
	if(pSocket && pSocket->IsInsideSocket(decPoint((int)where.x, (int)where.y))){
		igdeNativeBeosNVBoard *board = GetNativeBoard();
		if(board){
			board->BeginCreateLink(this);
			pIsDnd = true;
		}
	}
}

void igdeNativeBeosNVSlot::MouseMoved(BPoint where, uint32, const BMessage*){
	if(!pIsDnd){
		return;
	}
	
	igdeNativeBeosNVBoard *board = GetNativeBoard();
	if(!board){
		return;
	}
	
	BPoint boardPos(where);
	ConvertToScreen(&boardPos);
	BView * const boardView = (BView*)pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeContainer();
	if(boardView){
		boardView->ConvertFromScreen(&boardPos);
	}
	board->SetCreateLinkPosition(decPoint((int)boardPos.x, (int)boardPos.y));
}

void igdeNativeBeosNVSlot::MouseUp(BPoint){
	if(!pIsDnd){
		return;
	}
	
	pIsDnd = false;
	igdeNativeBeosNVBoard *board = GetNativeBoard();
	if(board){
		board->FinishCreateLink();
	}
}

BSize igdeNativeBeosNVSlot::PreferredSize(){
	BFont font;
	if(pFont){
		font = *(BFont*)pFont->GetNativeFont();
	}
	const float textWidth = font.StringWidth(pOwner->GetText());
	const float socketWidth = pSocket ? pSocket->Bounds().Width() + 4.0f : 0.0f;
	
	font_height fh;
	font.GetHeight(&fh);
	return BSize(textWidth + socketWidth, fh.ascent + fh.descent + fh.leading + 4.0f);
}

#endif
