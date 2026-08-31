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

#include "igdeNativeBeosViewCurveBezier.h"
#include "../../../curveedit/igdeViewCurveBezier.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <cmath>


// Class igdeNativeBeosViewCurveBezier
///////////////////////////////////////

igdeNativeBeosViewCurveBezier::igdeNativeBeosViewCurveBezier(igdeViewCurveBezier &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 200, 100), "viewCurveBezier", B_FOLLOW_ALL,
	B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS),
pOwner(&owner),
pGridCenter(0.5f, 0.5f),
pGridZoom(1.0f, 1.0f),
pDotSize(5),
pHitThreshold(8),
pDragMode(edmNone)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	
	if(parent){
		parent->AddChild(this);
	}
	
	UpdateParameters();
}

igdeNativeBeosViewCurveBezier::~igdeNativeBeosViewCurveBezier() = default;



igdeNativeBeosViewCurveBezier *igdeNativeBeosViewCurveBezier::CreateNativeWidget(igdeViewCurveBezier &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BView * const parent = (BView*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosViewCurveBezier(owner, parent, *owner.GetGuiTheme());
}

void igdeNativeBeosViewCurveBezier::PostCreateNativeWidget(){
}

void igdeNativeBeosViewCurveBezier::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

void igdeNativeBeosViewCurveBezier::UpdateEnabled(){
	Invalidate();
}

void igdeNativeBeosViewCurveBezier::UpdateDefaultSize(){
	const decPoint &defSize = pOwner->GetDefaultSize();
	ResizeTo((float)defSize.x, (float)defSize.y);
}

void igdeNativeBeosViewCurveBezier::UpdateCurve(){
	Invalidate();
}

void igdeNativeBeosViewCurveBezier::UpdateSelectedPoint(){
	Invalidate();
}

void igdeNativeBeosViewCurveBezier::UpdateClamp(){
	Invalidate();
}

void igdeNativeBeosViewCurveBezier::ResetView(){
	pGridCenter.Set(0.5f, 0.5f);
	pGridZoom.Set(1.0f, 1.0f);
	UpdateParameters();
	Invalidate();
}

void igdeNativeBeosViewCurveBezier::FitViewToCurve(){
	const decCurveBezier &curve = pOwner->GetCurve();
	const int count = curve.GetPointCount();
	
	if(count == 0){
		ResetView();
		return;
	}
	
	decVector2 minPt(curve.GetPointAt(0).GetPoint());
	decVector2 maxPt(minPt);
	
	for(int i = 1; i < count; i++){
		const decVector2 &pt = curve.GetPointAt(i).GetPoint();
		minPt.x = decMath::min(minPt.x, pt.x);
		minPt.y = decMath::min(minPt.y, pt.y);
		maxPt.x = decMath::max(maxPt.x, pt.x);
		maxPt.y = decMath::max(maxPt.y, pt.y);
	}
	
	const decVector2 range(maxPt - minPt);
	pGridCenter = (minPt + maxPt) * 0.5f;
	
	const BRect bounds = Bounds();
	if(range.x > 0.001f && range.y > 0.001f){
		pGridZoom.x = bounds.Width() * 0.8f / range.x;
		pGridZoom.y = bounds.Height() * 0.8f / range.y;
	}
	
	UpdateParameters();
	Invalidate();
}



int igdeNativeBeosViewCurveBezier::IndexOfPointAt(int x, int y, eSelectionModes mode) const{
	const decCurveBezier &curve = pOwner->GetCurve();
	const int count = curve.GetPointCount();
	
	for(int i = 0; i < count; i++){
		const decCurveBezierPoint &pt = curve.GetPointAt(i);
		
		if(mode == esmPoint || mode == esmHandle1 || mode == esmHandle2){
			const int px = CurveXToWindow(pt.GetPoint().x);
			const int py = CurveYToWindow(pt.GetPoint().y);
			if(abs(x - px) <= pHitThreshold && abs(y - py) <= pHitThreshold){
				return i;
			}
		}
		
		if(mode == esmHandle1){
			const int px = CurveXToWindow(pt.GetHandle1().x);
			const int py = CurveYToWindow(pt.GetHandle1().y);
			if(abs(x - px) <= pHitThreshold && abs(y - py) <= pHitThreshold){
				return i;
			}
		}
		
		if(mode == esmHandle2){
			const int px = CurveXToWindow(pt.GetHandle2().x);
			const int py = CurveYToWindow(pt.GetHandle2().y);
			if(abs(x - px) <= pHitThreshold && abs(y - py) <= pHitThreshold){
				return i;
			}
		}
	}
	
	return -1;
}

void igdeNativeBeosViewCurveBezier::UpdateParameters(){
	const BRect bounds = Bounds();
	pWindowCenter.Set((int)(bounds.Width() * 0.5f), (int)(bounds.Height() * 0.5f));
	pGridScale.Set(bounds.Width() * pGridZoom.x, bounds.Height() * pGridZoom.y);
}

int igdeNativeBeosViewCurveBezier::CurveXToWindow(float curveCoord) const{
	return pWindowCenter.x + (int)((curveCoord - pGridCenter.x) * pGridScale.x);
}

int igdeNativeBeosViewCurveBezier::CurveYToWindow(float curveCoord) const{
	return pWindowCenter.y - (int)((curveCoord - pGridCenter.y) * pGridScale.y);
}

float igdeNativeBeosViewCurveBezier::WindowXToCurve(int windowCoord) const{
	return (float)(windowCoord - pWindowCenter.x) / pGridScale.x + pGridCenter.x;
}

float igdeNativeBeosViewCurveBezier::WindowYToCurve(int windowCoord) const{
	return (float)(pWindowCenter.y - windowCoord) / pGridScale.y + pGridCenter.y;
}



void igdeNativeBeosViewCurveBezier::DrawGrid(){
	const BRect bounds = Bounds();
	
	// Dark background
	SetHighColor({40, 40, 40, 255});
	FillRect(bounds);
	
	// Grid lines
	SetHighColor({70, 70, 70, 255});
	
	// Vertical lines
	const float gridStepX = 0.1f;
	for(float gx = floorf(WindowXToCurve(0) / gridStepX) * gridStepX; ; gx += gridStepX){
		const int px = CurveXToWindow(gx);
		if(px > (int)bounds.Width()){
			break;
		}
		if(px >= 0){
			StrokeLine(BPoint((float)px, bounds.top), BPoint((float)px, bounds.bottom));
		}
	}
	
	// Horizontal lines
	const float gridStepY = 0.1f;
	for(float gy = floorf(WindowYToCurve((int)bounds.Height()) / gridStepY) * gridStepY; ; gy += gridStepY){
		const int py = CurveYToWindow(gy);
		if(py < 0){
			break;
		}
		if(py <= (int)bounds.Height()){
			StrokeLine(BPoint(bounds.left, (float)py), BPoint(bounds.right, (float)py));
		}
	}
	
	// Axis lines
	SetHighColor({100, 100, 100, 255});
	const int axisX = CurveXToWindow(0.0f);
	const int axisY = CurveYToWindow(0.0f);
	StrokeLine(BPoint((float)axisX, bounds.top), BPoint((float)axisX, bounds.bottom));
	StrokeLine(BPoint(bounds.left, (float)axisY), BPoint(bounds.right, (float)axisY));
}

void igdeNativeBeosViewCurveBezier::DrawCurve(){
	const decCurveBezier &curve = pOwner->GetCurve();
	const int count = curve.GetPointCount();
	if(count < 2){
		return;
	}
	
	SetHighColor({255, 255, 0, 255});
	
	for(int i = 0; i < count - 1; i++){
		const decCurveBezierPoint &p1 = curve.GetPointAt(i);
		const decCurveBezierPoint &p2 = curve.GetPointAt(i + 1);
		
		const decVector2 bp1(p1.GetPoint());
		const decVector2 bp2(p1.GetHandle2());
		const decVector2 bp3(p2.GetHandle1());
		const decVector2 bp4(p2.GetPoint());
		
		InternalDrawCurve(bp1, bp2, bp3, bp4);
	}
}

void igdeNativeBeosViewCurveBezier::DrawCurveHandles(){
	const decCurveBezier &curve = pOwner->GetCurve();
	const int count = curve.GetPointCount();
	const int selectedIndex = pOwner->GetSelectedPoint();
	
	for(int i = 0; i < count; i++){
		const decCurveBezierPoint &pt = curve.GetPointAt(i);
		const bool isSelected = (i == selectedIndex);
		
		const int px = CurveXToWindow(pt.GetPoint().x);
		const int py = CurveYToWindow(pt.GetPoint().y);
		
		// Handle lines
		if(i > 0 || curve.GetInterpolationMode() == decCurveBezier::eimBezier){
			const int hx1 = CurveXToWindow(pt.GetHandle1().x);
			const int hy1 = CurveYToWindow(pt.GetHandle1().y);
			SetHighColor({128, 128, 128, 255});
			StrokeLine(BPoint((float)px, (float)py), BPoint((float)hx1, (float)hy1));
			SetHighColor(isSelected ? rgb_color{255, 128, 0, 255} : rgb_color{128, 128, 255, 255});
			FillEllipse(BPoint((float)hx1, (float)hy1), (float)(pDotSize - 2), (float)(pDotSize - 2));
		}
		
		if(i < count - 1 || curve.GetInterpolationMode() == decCurveBezier::eimBezier){
			const int hx2 = CurveXToWindow(pt.GetHandle2().x);
			const int hy2 = CurveYToWindow(pt.GetHandle2().y);
			SetHighColor({128, 128, 128, 255});
			StrokeLine(BPoint((float)px, (float)py), BPoint((float)hx2, (float)hy2));
			SetHighColor(isSelected ? rgb_color{255, 128, 0, 255} : rgb_color{128, 128, 255, 255});
			FillEllipse(BPoint((float)hx2, (float)hy2), (float)(pDotSize - 2), (float)(pDotSize - 2));
		}
		
		// Control point
		if(isSelected){
			SetHighColor({255, 255, 0, 255});
		}else{
			SetHighColor({200, 200, 200, 255});
		}
		FillEllipse(BPoint((float)px, (float)py), (float)pDotSize, (float)pDotSize);
		SetHighColor({0, 0, 0, 255});
		StrokeEllipse(BPoint((float)px, (float)py), (float)pDotSize, (float)pDotSize);
	}
}

void igdeNativeBeosViewCurveBezier::InternalDrawCurve(const decVector2 &p1,
const decVector2 &p2, const decVector2 &p3, const decVector2 &p4){
	const decVector2 d1(p2 - p1);
	const decVector2 d2(p3 - p2);
	const decVector2 d3(p4 - p3);
	const float len = d1 * d1 + d2 * d2 + d3 * d3;
	
	if(len < 4.0f){
		const int x1 = CurveXToWindow(p1.x), y1 = CurveYToWindow(p1.y);
		const int x4 = CurveXToWindow(p4.x), y4 = CurveYToWindow(p4.y);
		StrokeLine(BPoint((float)x1, (float)y1), BPoint((float)x4, (float)y4));
		return;
	}
	
	const decVector2 np1((p1 + p2) * 0.5f);
	const decVector2 np2((p2 + p3) * 0.5f);
	const decVector2 np3((p3 + p4) * 0.5f);
	const decVector2 np4((np1 + np2) * 0.5f);
	const decVector2 np5((np2 + np3) * 0.5f);
	const decVector2 np6((np4 + np5) * 0.5f);
	
	InternalDrawCurve(p1, np1, np4, np6);
	InternalDrawCurve(np6, np5, np3, p4);
}



// BView overrides
///////////////////

void igdeNativeBeosViewCurveBezier::Draw(BRect){
	DrawGrid();
	DrawCurve();
	DrawCurveHandles();
}

void igdeNativeBeosViewCurveBezier::MouseDown(BPoint where){
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	
	const int x = (int)where.x;
	const int y = (int)where.y;
	
	// Check for clicking on a control point or handle
	const int pointIndex = IndexOfPointAt(x, y, esmPoint);
	if(pointIndex != -1){
		pOwner->SetSelectedPoint(pointIndex);
		pDragMode = edmDragCurvePoint;
		pDragOrg.Set(x, y);
		pDragOrgPoint = pOwner->GetCurve().GetPointAt(pointIndex);
		return;
	}
	
	// Check for right mouse button - context menu
	const BMessage *msg = Window() ? Window()->CurrentMessage() : nullptr;
	const int32 buttons = msg ? msg->FindInt32("buttons") : 0;
	if(buttons & B_SECONDARY_MOUSE_BUTTON){
		pOwner->ShowContextMenu(decPoint(x, y));
		return;
	}
	
	// Start view drag
	pDragMode = edmDragView;
	pDragOrg.Set(x, y);
}

void igdeNativeBeosViewCurveBezier::MouseMoved(BPoint where, uint32, const BMessage*){
	const int x = (int)where.x;
	const int y = (int)where.y;
	
	if(pDragMode == edmDragCurvePoint){
		const int selected = pOwner->GetSelectedPoint();
		if(selected == -1){
			pDragMode = edmNone;
			return;
		}
		
		const float newX = WindowXToCurve(x);
		const float newY = WindowYToCurve(y);
		
		const decVector2 oldPt(pDragOrgPoint.GetPoint());
		const decVector2 delta(newX - oldPt.x, newY - oldPt.y);
		
		decCurveBezierPoint pt(pDragOrgPoint);
		pt.SetPoint(decVector2(newX, newY));
		pt.SetHandle1(pDragOrgPoint.GetHandle1() + delta);
		pt.SetHandle2(pDragOrgPoint.GetHandle2() + delta);
		
		pOwner->GetCurve().SetPointAt(selected, pt);
		pOwner->NotifyCurveChanging();
		Invalidate();
		
	}else if(pDragMode == edmDragView){
		const decPoint delta(x - pDragOrg.x, y - pDragOrg.y);
		pDragOrg.Set(x, y);
		
		pGridCenter.x -= (float)delta.x / pGridScale.x;
		pGridCenter.y += (float)delta.y / pGridScale.y;
		
		UpdateParameters();
		Invalidate();
	}
}

void igdeNativeBeosViewCurveBezier::MouseUp(BPoint){
	if(pDragMode == edmDragCurvePoint){
		pOwner->NotifyCurveChanged();
	}
	pDragMode = edmNone;
}

void igdeNativeBeosViewCurveBezier::FrameResized(float, float){
	UpdateParameters();
	Invalidate();
}

#endif
