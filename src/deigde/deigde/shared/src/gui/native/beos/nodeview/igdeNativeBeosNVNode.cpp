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

#include "igdeNativeBeosNVNode.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>

static const int pTitleBarHeight = 20;


// Class igdeNativeBeosNVNode
//////////////////////////////

igdeNativeBeosNVNode::igdeNativeBeosNVNode(igdeNVNode &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 100, 50), "nvNode", B_FOLLOW_NONE, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
pOwner(&owner),
pFont(NVNodeFont(owner, guitheme)),
pTitleIsDragging(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	
	if(parent){
		parent->AddChild(this);
	}
	
	UpdateTitle();
	UpdateDescription();
	UpdateEnabled();
	UpdateColors();
	UpdatePosition();
}

igdeNativeBeosNVNode::~igdeNativeBeosNVNode() = default;



igdeNativeBeosNVNode *igdeNativeBeosNVNode::CreateNativeWidget(igdeNVNode &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BView * const parent = (BView*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosNVNode(owner, parent, *owner.GetGuiTheme());
}

void igdeNativeBeosNVNode::PostCreateNativeWidget(){
}

void igdeNativeBeosNVNode::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

void igdeNativeBeosNVNode::UpdateTitle(){
	Invalidate(BRect(0, 0, Bounds().Width(), pTitleBarHeight));
}

void igdeNativeBeosNVNode::UpdateDescription(){
	SetToolTipText(pOwner->GetDescription());
}

void igdeNativeBeosNVNode::UpdateEnabled(){
	Invalidate();
}

void igdeNativeBeosNVNode::UpdateActive(){
	UpdateColors();
}

void igdeNativeBeosNVNode::UpdateColors(){
	Invalidate();
}

void igdeNativeBeosNVNode::UpdatePosition(){
	decPoint position(pOwner->GetPosition());
	if(pOwner->GetOwnerBoard()){
		position += pOwner->GetOwnerBoard()->GetSize() / 2 + pOwner->GetOwnerBoard()->GetOffset();
	}
	MoveTo((float)position.x, (float)position.y);
	
	if(Parent()){
		Parent()->Invalidate();
	}
}

void igdeNativeBeosNVNode::FitSizeToContent(){
	float totalHeight = (float)pTitleBarHeight;
	const int slotCount = pOwner->GetSlotCount();
	for(int i = 0; i < slotCount; i++){
		totalHeight += 20.0f;
	}
	
	BFont font;
	if(pFont){
		font = *(BFont*)pFont->GetNativeFont();
	}
	const float titleWidth = font.StringWidth(pOwner->GetTitle()) + 20.0f;
	const float minWidth = decMath::max(100.0f, titleWidth);
	
	ResizeTo(minWidth, totalHeight);
}

decPoint igdeNativeBeosNVNode::GetSize(){
	return decPoint((int)Bounds().Width(), (int)Bounds().Height());
}



igdeFont *igdeNativeBeosNVNode::NVNodeFont(const igdeNVNode &owner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::nodeViewNodeFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::nodeViewNodeFontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::nodeViewNodeFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::nodeViewNodeFontSize, 1.0f);
		
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

void igdeNativeBeosNVNode::Draw(BRect){
	const BRect bounds = Bounds();
	const bool isActive = pOwner->GetActive();
	
	const decColor &bgC = pOwner->GetBgColor();
	const rgb_color bgColor = {(uint8)(bgC.r * 255.0f), (uint8)(bgC.g * 255.0f),
		(uint8)(bgC.b * 255.0f), 255};
	
	const decColor &titleC = isActive ? pOwner->GetActiveTitleBgColor() : pOwner->GetInactiveTitleBgColor();
	const rgb_color titleColor = {(uint8)(titleC.r * 255.0f), (uint8)(titleC.g * 255.0f),
		(uint8)(titleC.b * 255.0f), 255};
	
	const decColor &borderC = pOwner->GetBorderColor();
	const rgb_color borderColor = {(uint8)(borderC.r * 255.0f), (uint8)(borderC.g * 255.0f),
		(uint8)(borderC.b * 255.0f), 255};
	
	// Title bar
	SetHighColor(titleColor);
	FillRect(BRect(0, 0, bounds.Width(), (float)pTitleBarHeight));
	
	// Body background
	SetHighColor(bgColor);
	FillRect(BRect(0, (float)pTitleBarHeight, bounds.Width(), bounds.Height()));
	
	// Border
	SetHighColor(borderColor);
	StrokeRect(bounds);
	SetHighColor(borderColor);
	StrokeLine(BPoint(0, (float)pTitleBarHeight), BPoint(bounds.Width(), (float)pTitleBarHeight));
	
	// Title text
	BFont font;
	if(pFont){
		font = *(BFont*)pFont->GetNativeFont();
	}
	SetFont(&font);
	SetHighColor({0, 0, 0, 255});
	
	font_height fh;
	font.GetHeight(&fh);
	const float textY = (float)pTitleBarHeight * 0.5f + (fh.ascent - fh.descent) * 0.5f;
	const float textW = font.StringWidth(pOwner->GetTitle());
	DrawString(pOwner->GetTitle(), BPoint((bounds.Width() - textW) * 0.5f, textY));
}

void igdeNativeBeosNVNode::MouseDown(BPoint where){
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	
	if(where.y <= (float)pTitleBarHeight){
		if(pOwner->GetOwnerBoard()){
			pOwner->GetOwnerBoard()->SetActiveNode(pOwner);
		}
		pTitleDragOffset.Set((int)where.x, (int)where.y);
		pTitleIsDragging = true;
		pOwner->NotifyDragBegin();
	}
}

void igdeNativeBeosNVNode::MouseMoved(BPoint where, uint32, const BMessage*){
	if(!pTitleIsDragging){
		return;
	}
	
	const BPoint pos = Frame().LeftTop();
	decPoint position(
		(int)(pos.x + where.x - pTitleDragOffset.x),
		(int)(pos.y + where.y - pTitleDragOffset.y));
	
	if(pOwner->GetOwnerBoard()){
		position -= pOwner->GetOwnerBoard()->GetSize() / 2 + pOwner->GetOwnerBoard()->GetOffset();
	}
	pOwner->SetPosition(position);
	pOwner->NotifyDraging();
	
	if(pOwner->GetOwnerBoard() && pOwner->GetOwnerBoard()->GetNativeWidget()){
		((BView*)pOwner->GetOwnerBoard()->GetNativeWidget())->Invalidate();
	}
}

void igdeNativeBeosNVNode::MouseUp(BPoint){
	if(!pTitleIsDragging){
		return;
	}
	
	pTitleIsDragging = false;
	pOwner->NotifyDragEnd();
}

void igdeNativeBeosNVNode::FrameResized(float, float){
	Invalidate();
}

#endif
