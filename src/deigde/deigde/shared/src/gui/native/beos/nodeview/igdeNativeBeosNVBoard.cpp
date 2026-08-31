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

#include "igdeNativeBeosNVBoard.h"
#include "igdeNativeBeosNVSlot.h"
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../nodeview/igdeNVLink.h"
#include "../../../nodeview/igdeNVSlot.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeNativeBeosNVBoard
///////////////////////////////

igdeNativeBeosNVBoard::igdeNativeBeosNVBoard(igdeNVBoard &owner, BView *parent,
	const igdeGuiTheme &) :
BView(BRect(0, 0, 300, 200), "nvBoard", B_FOLLOW_ALL,
	B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS),
pOwner(&owner),
pCreateLinkSource(nullptr),
pCreateLinkTarget(nullptr),
pIsDragBoard(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	
	if(parent){
		parent->AddChild(this);
	}
	
	UpdateColors();
}

igdeNativeBeosNVBoard::~igdeNativeBeosNVBoard() = default;



igdeNativeBeosNVBoard *igdeNativeBeosNVBoard::CreateNativeWidget(igdeNVBoard &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BView * const parent = (BView*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosNVBoard(owner, parent, *owner.GetGuiTheme());
}

void igdeNativeBeosNVBoard::PostCreateNativeWidget(){
}

void igdeNativeBeosNVBoard::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

void igdeNativeBeosNVBoard::UpdateEnabled(){
	Invalidate();
}

void igdeNativeBeosNVBoard::UpdateColors(){
	const decColor &c = pOwner->GetBgColor();
	SetViewColor((uint8)(c.r * 255.0f), (uint8)(c.g * 255.0f), (uint8)(c.b * 255.0f));
	Invalidate();
}

void igdeNativeBeosNVBoard::UpdateNodes(){
	Invalidate();
}

void igdeNativeBeosNVBoard::UpdateLinks(){
	Invalidate();
}

void igdeNativeBeosNVBoard::UpdateOffset(){
	Invalidate();
}

decPoint igdeNativeBeosNVBoard::GetSize(){
	return decPoint((int)Bounds().Width(), (int)Bounds().Height());
}

igdeNVLink *igdeNativeBeosNVBoard::ClosestLinkNear(const decPoint &position, float range) const{
	igdeNVLink *bestLink = nullptr;
	float bestDistance = 0.0f;
	
	pOwner->GetLinks().Visit([&](igdeNVLink *link){
		const decPoint p1(link->GetSource()->GetConnectorBoard());
		const decPoint p4(link->GetTarget()->GetConnectorBoard());
		const int offset = abs(p4.x - p1.x) >> 1;
		const decPoint p2(p1.x + offset, p1.y);
		const decPoint p3(p4.x - offset, p4.y);
		
		const float distance = PointBezierDistance(position, p1, p2, p3, p4);
		if(distance > range){
			return;
		}
		if(!bestLink || distance <= bestDistance){
			bestLink = link;
			bestDistance = distance;
		}
	});
	
	return bestLink;
}

const igdeNVLink::Ref &igdeNativeBeosNVBoard::GetHoverLink() const{
	return pHoverLink;
}

void igdeNativeBeosNVBoard::SetHoverLink(igdeNVLink *link){
	if(pHoverLink == link){
		return;
	}
	pHoverLink = link;
	Invalidate();
}



void igdeNativeBeosNVBoard::BeginCreateLink(igdeNativeBeosNVSlot *source){
	SetHoverLink(nullptr);
	pCreateLinkSource = source;
	pCreateLinkTarget = nullptr;
	pCreateLinkPosition = source->GetCenterBoard();
	Invalidate();
}

void igdeNativeBeosNVBoard::SetCreateLinkPosition(const decPoint &position){
	pCreateLinkPosition = position;
	Invalidate();
}

void igdeNativeBeosNVBoard::SetCreateLinkTarget(igdeNativeBeosNVSlot *target){
	pCreateLinkTarget = target;
	Invalidate();
}

void igdeNativeBeosNVBoard::FinishCreateLink(){
	if(pCreateLinkSource && pCreateLinkTarget
	&& pOwner->CanLink(&pCreateLinkSource->GetOwner(), &pCreateLinkTarget->GetOwner())){
		igdeNVLink * const link = pOwner->AddLink(
			&pCreateLinkSource->GetOwner(), &pCreateLinkTarget->GetOwner());
		pCreateLinkSource = nullptr;
		pCreateLinkTarget = nullptr;
		Invalidate();
		pOwner->NotifyLinkAdded(link);
		
	}else{
		pCreateLinkSource = nullptr;
		pCreateLinkTarget = nullptr;
		Invalidate();
	}
}



// BView overrides
///////////////////

void igdeNativeBeosNVBoard::Draw(BRect updateRect){
	const BRect bounds = Bounds();
	const decColor &bgC = pOwner->GetBgColor();
	
	SetHighColor((uint8)(bgC.r * 255.0f), (uint8)(bgC.g * 255.0f), (uint8)(bgC.b * 255.0f));
	FillRect(bounds);
	
	DrawLinks();
	DrawCreateLink();
}

void igdeNativeBeosNVBoard::MouseDown(BPoint where){
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	
	const BMessage *msg = Window() ? Window()->CurrentMessage() : nullptr;
	const bool shift = msg && (msg->FindInt32("modifiers") & B_SHIFT_KEY) != 0;
	
	if(shift){
		SetHoverLink(nullptr);
		pDragBoard.Set((int)where.x, (int)where.y);
		pIsDragBoard = true;
		be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	}else{
		const decPoint pos((int)where.x, (int)where.y);
		SetHoverLink(pOwner->ClosestLinkNear(pos));
	}
}

void igdeNativeBeosNVBoard::MouseMoved(BPoint where, uint32, const BMessage*){
	const decPoint position((int)where.x, (int)where.y);
	
	if(pIsDragBoard){
		const decPoint distance(position - pDragBoard);
		pDragBoard = position;
		pOwner->SetOffset(pOwner->GetOffset() + distance);
		pOwner->NotifyOffsetChanged();
		
	}else{
		SetHoverLink(pOwner->ClosestLinkNear(position));
	}
}

void igdeNativeBeosNVBoard::MouseUp(BPoint){
	if(pIsDragBoard){
		pIsDragBoard = false;
	}
}

void igdeNativeBeosNVBoard::FrameResized(float, float){
	pOwner->NotifyNodesOffsetChanged();
	pOwner->NotifySizeChanged();
	Invalidate();
}



// Drawing helpers
///////////////////

void igdeNativeBeosNVBoard::DrawLinks() const{
	const rgb_color colorLink = {0, 0, 128, 255};
	const rgb_color colorHover = {128, 128, 255, 255};
	
	pOwner->GetLinks().Visit([this, &colorLink, &colorHover](igdeNVLink *link){
		const decPoint p1(link->GetSource()->GetConnectorBoard());
		const decPoint p4(link->GetTarget()->GetConnectorBoard());
		const int offset = abs(p4.x - p1.x) >> 1;
		const decPoint p2(p1.x + offset, p1.y);
		const decPoint p3(p4.x - offset, p4.y);
		DrawBezier(pHoverLink == link ? colorHover : colorLink, p1, p2, p3, p4);
	});
}

void igdeNativeBeosNVBoard::DrawCreateLink() const{
	if(!pCreateLinkSource){
		return;
	}
	
	decPoint positionTarget;
	rgb_color colorLink;
	
	if(pCreateLinkTarget){
		positionTarget = pCreateLinkTarget->GetConnectorBoard();
		if(pOwner->CanLink(&pCreateLinkSource->GetOwner(), &pCreateLinkTarget->GetOwner())){
			colorLink = {0, 192, 0, 255};
		}else{
			colorLink = {255, 0, 0, 255};
		}
	}else{
		positionTarget = pCreateLinkPosition;
		colorLink = {192, 192, 0, 255};
	}
	
	const decPoint p1(pCreateLinkSource->GetConnectorBoard());
	const decPoint p4(positionTarget);
	const int offset = abs(p4.x - p1.x) >> 1;
	const decPoint p2(p1.x + offset, p1.y);
	const decPoint p3(p4.x - offset, p4.y);
	DrawBezier(colorLink, p1, p2, p3, p4);
}

void igdeNativeBeosNVBoard::DrawBezier(rgb_color color, const decPoint &p1,
const decPoint &p2, const decPoint &p3, const decPoint &p4) const{
	const rgb_color blended = {(uint8)((color.red + 255) / 2),
		(uint8)((color.green + 255) / 2), (uint8)((color.blue + 255) / 2), 255};
	
	SetHighColor(blended);
	DrawBezierLine(p1, p2, p3, p4);
	
	SetHighColor(color);
	DrawBezierLine(p1 + decPoint(0, -1), p2 + decPoint(0, -1), p3 + decPoint(0, -1), p4 + decPoint(0, -1));
	DrawBezierLine(p1 + decPoint(0, 1), p2 + decPoint(0, 1), p3 + decPoint(0, 1), p4 + decPoint(0, 1));
}

void igdeNativeBeosNVBoard::DrawBezierLine(const decPoint &p1, const decPoint &p2,
const decPoint &p3, const decPoint &p4) const{
	const decVector2 bp1((float)p1.x, (float)p1.y);
	const decVector2 bp2((float)p2.x, (float)p2.y);
	const decVector2 bp3((float)p3.x, (float)p3.y);
	const decVector2 bp4((float)p4.x, (float)p4.y);
	InternalDrawBezier(bp1, bp2, bp3, bp4);
}

void igdeNativeBeosNVBoard::InternalDrawBezier(const decVector2 &p1, const decVector2 &p2,
const decVector2 &p3, const decVector2 &p4) const{
	decVector2 diff(p2 - p1);
	float length = diff * diff;
	diff = p3 - p2;
	length += diff * diff;
	diff = p4 - p3;
	length += diff * diff;
	
	if(length < 60.0f){
		StrokeLine(BPoint(p1.x, p1.y), BPoint(p2.x, p2.y));
		StrokeLine(BPoint(p2.x, p2.y), BPoint(p3.x, p3.y));
		StrokeLine(BPoint(p3.x, p3.y), BPoint(p4.x, p4.y));
		return;
	}
	
	const decVector2 np1((p1 + p2) * 0.5f);
	const decVector2 np2((p2 + p3) * 0.5f);
	const decVector2 np3((p3 + p4) * 0.5f);
	const decVector2 np4((np1 + np2) * 0.5f);
	const decVector2 np5((np2 + np3) * 0.5f);
	const decVector2 np6((np4 + np5) * 0.5f);
	
	InternalDrawBezier(p1, np1, np4, np6);
	InternalDrawBezier(np6, np5, np3, p4);
}

static float fPointLineDist(const decVector2 &bp, const decVector2 &bp1, const decVector2 &bp2){
	decVector2 normal(bp2 - bp1);
	const float length = normal.Length();
	if(length <= 1.0f){
		return (bp - bp1).Length();
	}
	normal /= length;
	const float distance = normal * (bp - bp1);
	if(distance < -0.5f || distance > length + 0.5f){
		return INFINITY;
	}
	return (bp - bp1 - normal * distance).Length();
}

float igdeNativeBeosNVBoard::PointBezierDistance(const decPoint &p, const decPoint &bp1,
const decPoint &bp2, const decPoint &bp3, const decPoint &bp4) const{
	return PointBezierDistance(decVector2(p), decVector2(bp1), decVector2(bp2),
		decVector2(bp3), decVector2(bp4));
}

float igdeNativeBeosNVBoard::PointBezierDistance(const decVector2 &bp, const decVector2 &bp1,
const decVector2 &bp2, const decVector2 &bp3, const decVector2 &bp4) const{
	decVector2 diff(bp2 - bp1);
	float length = diff * diff;
	diff = bp3 - bp2;
	length += diff * diff;
	diff = bp4 - bp3;
	length += diff * diff;
	
	if(length < 60.0f){
		return decMath::min(fPointLineDist(bp, bp1, bp2),
			fPointLineDist(bp, bp2, bp3), fPointLineDist(bp, bp3, bp4));
	}
	
	const decVector2 nbp1((bp1 + bp2) * 0.5f);
	const decVector2 nbp2((bp2 + bp3) * 0.5f);
	const decVector2 nbp3((bp3 + bp4) * 0.5f);
	const decVector2 nbp4((nbp1 + nbp2) * 0.5f);
	const decVector2 nbp5((nbp2 + nbp3) * 0.5f);
	const decVector2 nbp6((nbp4 + nbp5) * 0.5f);
	
	return decMath::min(PointBezierDistance(bp, bp1, nbp1, nbp4, nbp6),
		PointBezierDistance(bp, nbp6, nbp5, nbp3, bp4));
}

#endif
