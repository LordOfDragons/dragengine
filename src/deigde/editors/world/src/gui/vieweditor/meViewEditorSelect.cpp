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

#include "meViewEditorSelect.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../collisions/meCLSelect.h"
#include "../../collisions/meCLClosestElement.h"
#include "../../collisions/meCLCollect.h"
#include "../../collisions/meCLHitListEntry.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class meViewEditorSelect
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorSelect::meViewEditorSelect(meView3D &view) :
meViewEditorNavigation(view),
pCLSelect(nullptr),
pCLBubbleInfo(nullptr)
{
	pColVol = view.GetEngine()->GetColliderManager()->CreateColliderVolume();
	
	pCanvasSelect = view.GetEngine()->GetCanvasManager()->CreateCanvasPaint();
	pCanvasSelect->SetShapeType(deCanvasPaint::estRectangle);
	pCanvasSelect->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.1f));
	pCanvasSelect->SetLineColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
	pCanvasSelect->SetThickness(1.0f);
	pCanvasSelect->SetOrder(10.0f);
	pCanvasSelect->SetVisible(false);
	view.AddCanvas(pCanvasSelect);
	
	pInfoBubble = meInfoBubble::Ref::New(view);
	
	igdeFont::sConfiguration fc;
	view.GetEnvironment().GetApplicationFont(fc);
	pFont = view.GetEnvironment().GetSharedFont(fc);
	
	pInfoBubbleText = view.GetEngine()->GetCanvasManager()->CreateCanvasText();
	pInfoBubbleText->SetFont(pFont->GetEngineFont());
	pInfoBubbleText->SetFontSize((float)pFont->GetEngineFont()->GetLineHeight());
	pInfoBubbleText->SetColor(decColor(1.0f, 1.0f, 1.0f));
	pInfoBubbleText->SetOrder(0.0f);
	pInfoBubble->GetCanvasContent()->AddCanvas(pInfoBubbleText);
	
	try{
		pCLSelect = new meCLSelect(view.GetWorld());
		pCLBubbleInfo = new meCLSelect(view.GetWorld());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meViewEditorSelect::~meViewEditorSelect(){
	pCleanUp();
}



// Management
///////////////

void meViewEditorSelect::UpdateRectSelection(){
	decLayerMask collisionCategory;
	collisionCategory.SetBit(meWorld::eclmEditing);
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit(meWorld::eclmDecals);
	collisionFilter.SetBit(meWorld::eclmObjects);
	collisionFilter.SetBit(meWorld::eclmObjectShape);
	collisionFilter.SetBit(meWorld::eclmAI);
	
	// determine the boundaries of the box
	const decPoint &dragCurrent = GetDragCurrent();
	const decPoint &dragOrigin = GetDragOrigin();
	decBoundary box;
	box.x1 = dragOrigin.x;
	box.y1 = dragOrigin.y;
	box.x2 = dragCurrent.x;
	box.y2 = dragCurrent.y;
	box.OrientateRect();
	
	const meCamera &camera = GetActiveCamera();
	const int height = GetViewHeight();
	const int width = GetViewWidth();
	
	decVector corner1(camera.GetDirectionFor(width, height, box.x1, box.y1));
	decVector corner2(camera.GetDirectionFor(width, height, box.x2, box.y1));
	decVector corner3(camera.GetDirectionFor(width, height, box.x2, box.y2));
	decVector corner4(camera.GetDirectionFor(width, height, box.x1, box.y2));
	
	const decVector view(GetMatrixView().TransformView().ToVector());
	const float rectSelDist = GetWorldGuiParameters().GetRectSelDistance();
	corner1 *= rectSelDist / (view * corner1);
	corner2 *= rectSelDist / (view * corner2);
	corner3 *= rectSelDist / (view * corner3);
	corner4 *= rectSelDist / (view * corner4);
	
	pColVol->SetPosition(GetMatrixView().GetPosition());
	pColVol->SetOrientation(decQuaternion());
	pColVol->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
	
	decShapeHull * const shapeHull = new decShapeHull;
	shapeHull->SetPointCount(5);
	shapeHull->SetPointAt(0, decVector());
	shapeHull->SetPointAt(1, corner1);
	shapeHull->SetPointAt(2, corner2);
	shapeHull->SetPointAt(3, corner3);
	shapeHull->SetPointAt(4, corner4);
	
	decShapeList shapeList;
	shapeList.Add(shapeHull);
	((deColliderVolume&)(deCollider&)pColVol).SetShapes(shapeList);
	
	// test with this frustum for selection
	pCLSelect->Prepare();
	ColliderTestCollision(pCLSelect, pColVol);
	pCLSelect->RunAction();
	
	// update canvas
	OnResize();
}



// Callbacks
//////////////

bool meViewEditorSelect::OnKeyPress(deInputEvent::eKeyCodes key, bool shift, bool control){
	if(key == deInputEvent::ekcControl){
		OnMouseMove(pLastMousePosition.x, pLastMousePosition.y, shift, true);
	}
	
	return meViewEditorNavigation::OnKeyPress(key, shift, control);
}

bool meViewEditorSelect::OnKeyRelease(deInputEvent::eKeyCodes key, bool shift, bool control){
	if(key == deInputEvent::ekcControl){
		OnMouseMove(pLastMousePosition.x, pLastMousePosition.y, shift, false);
	}
	
	return meViewEditorNavigation::OnKeyRelease(key, shift, control);
}

void meViewEditorSelect::OnResize(){
	if(!GetDragLeftMouseButton() || !pCLSelect || !pCLSelect->GetRectSelect()){
		pCanvasSelect->SetVisible(false);
		return;
	}
	
	const decPoint &dragOrigin = GetDragOrigin();
	const decPoint &dragCurrent = GetDragCurrent();
	decBoundary box;
	
	box.x1 = dragOrigin.x;
	box.y1 = dragOrigin.y;
	box.x2 = dragCurrent.x;
	box.y2 = dragCurrent.y;
	box.OrientateRect();
	
	pCanvasSelect->SetPosition(decPoint(box.x1, box.y1));
	pCanvasSelect->SetSize(decPoint(box.x2 - box.x1, box.y2 - box.y1));
	pCanvasSelect->SetVisible(true);
}

void meViewEditorSelect::OnLeftMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonPress(x, y, shift, control);
	pLastMousePosition.Set(x, y);
	
	if(GetDragLeftMouseButton() && pCLSelect){
		const int elementMode = GetElementMode();
		
		pCLSelect->SetSingleSelect(!shift);
		pCLSelect->SetRectSelect(false);
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmDecals);
		collisionFilter.SetBit(meWorld::eclmObjects);
		collisionFilter.SetBit(meWorld::eclmObjectShape);
		collisionFilter.SetBit(meWorld::eclmAI);
		
		pCLSelect->SetCanSelectObjects(elementMode == meWorldGuiParameters::eemObject);
		pCLSelect->SetCanSelectObjectShapes(elementMode == meWorldGuiParameters::eemObjectShape);
		pCLSelect->SetCanSelectDecals(elementMode == meWorldGuiParameters::eemDecal);
		pCLSelect->SetCanSelectNavSpaces(elementMode == meWorldGuiParameters::eemNavSpace);
		
		pCLSelect->SetCanHitObjects(elementMode == meWorldGuiParameters::eemObject);
		pCLSelect->SetCanHitObjectShapes(elementMode == meWorldGuiParameters::eemObjectShape);
		pCLSelect->SetCanHitDecals(elementMode == meWorldGuiParameters::eemDecal);
		pCLSelect->SetCanHitNavSpaces(elementMode == meWorldGuiParameters::eemNavSpace);
		
		pCLSelect->Reset();
		meWorldGuiParameters &guiparams = GetWorldGuiParameters();
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = GetActiveCamera().GetDirectionFor(
			GetViewWidth(), GetViewHeight(), x, y) * guiparams.GetRectSelDistance();
		pCLSelect->SetRay(rayPosition, rayDirection);
		
		pInfoBubble->Hide();
		OnResize();
	}
}

class BuildSelectionListVisitor : public decTVisitor<meCLHitListEntry>{
public:
	meWorldGuiParameters::eElementModes elementMode;
	decStringList selectionList;
	meCLHitListEntry::List selectionHitEntries;
	float rayDistance;
	BuildSelectionListVisitor() = default;
	
	void operator() (meCLHitListEntry &entry) override{
		decString text;
		
		switch(elementMode){
		case meWorldGuiParameters::eemObject:
			if(entry.GetObject()){
				const meObject &object = *entry.GetObject();
				text.Format("%s%s (%s): %.1f m",
					object.GetActive() ? "[A] " : object.GetSelected() ? "[S] " : "",
					object.GetClassName().GetString(),
					object.GetID().ToHexString().GetString(),
					entry.GetDistance() * rayDistance);
				selectionList.Add(text);
				selectionHitEntries.Add(&entry);
			}
			break;
			
		case meWorldGuiParameters::eemObjectShape:
			if(entry.GetObjectShape()){
				const meObjectShape &shape = *entry.GetObjectShape();
				text.Format("%sShape: %.1f m",
					shape.GetActive() ? "[A] " : shape.GetSelected() ? "[S] " : "",
					entry.GetDistance() * rayDistance);
				selectionList.Add(text);
				selectionHitEntries.Add(&entry);
			}
			break;
			
		case meWorldGuiParameters::eemDecal:
			if(entry.GetDecal()){
				const meDecal &decal = *entry.GetDecal();
				
				decString path(decal.GetSkinPath());
				if(path.GetLength() > 50){
					path = decString("...") + path.GetRight(50);
				}
				
				text.Format("%s%s",
					decal.GetActive() ? "[A] " : decal.GetSelected() ? "[S] " : "",
					path.GetString());
				selectionList.Add(text);
				selectionHitEntries.Add(&entry);
			}
			break;
			
		case meWorldGuiParameters::eemNavSpace:
			if(entry.GetNavigationSpace()){
				const meNavigationSpace &navspace = *entry.GetNavigationSpace();
				text.Format("%sNavSpace: %.1f m",
					navspace.GetActive() ? "[A] " : navspace.GetSelected() ? "[S] " : "",
					entry.GetDistance() * rayDistance);
				selectionList.Add(text);
				selectionHitEntries.Add(&entry);
			}
			break;
		}
	}
};

void meViewEditorSelect::OnLeftMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonRelease(x, y, shift, control);
	pLastMousePosition.Set(x, y);
	
	if(pCLSelect){
		pCLSelect->Reset();
		
		if(!pCLSelect->GetRectSelect()){
			decLayerMask collisionCategory;
			collisionCategory.SetBit(meWorld::eclmEditing);
			
			decLayerMask collisionFilter;
			collisionFilter.SetBit(meWorld::eclmDecals);
			collisionFilter.SetBit(meWorld::eclmObjects);
			collisionFilter.SetBit(meWorld::eclmObjectShape);
			collisionFilter.SetBit(meWorld::eclmAI);
			
			RayTestCollision(pCLSelect, pCLSelect->GetRayOrigin(), pCLSelect->GetRayDirection(),
				decCollisionFilter(collisionCategory, collisionFilter));
			
			meCLHitListEntry::List &hitList = pCLSelect->GetHitList();
			if(hitList.IsNotEmpty()){
				hitList.SortAscending();
				
				if(hitList.GetCount() == 1 || !control){
					pCLSelect->RunAction(0);
					
				}else{
					hitList.RemoveIf([&](meCLHitListEntry &a){
						return hitList.HasMatching([&](const meCLHitListEntry &b){
							return &a != &b && a.IsSame(b);
						});
					});
					
					BuildSelectionListVisitor visitor;
					visitor.elementMode = GetWorldGuiParameters().GetElementMode();
					visitor.rayDistance = (float)pCLSelect->GetRayDirection().Length();
					hitList.Visit(visitor);
					
					int selectionIndex = 0;
					if(igdeCommonDialogs::SelectString(&GetView(), "Choose Element",
					"Choose element to select/deselect:", visitor.selectionList, selectionIndex)){
						pCLSelect->RunAction(selectionIndex);
					}
				}
			}
		}
		
		OnResize();
		pUpdateInfoBubble(x, y, !control);
	}
}

void meViewEditorSelect::OnRightMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnRightMouseButtonPress(x, y, shift, control);
	pLastMousePosition.Set(x, y);
	
	pInfoBubble->Hide();
}

void meViewEditorSelect::OnRightMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnRightMouseButtonRelease(x, y, shift, control);
	pLastMousePosition.Set(x, y);
	
	pUpdateInfoBubble(x, y, !control);
}

void meViewEditorSelect::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnMouseMove(x, y, shift, control);
	pLastMousePosition.Set(x, y);
	
	if(GetDragLeftMouseButton() && pCLSelect){
		if(!pCLSelect->GetRectSelect()){
			const int threshold = GetWorldGuiParameters().GetRectSelDragThreshold();
			const decPoint &dragDist = GetDragDistanceStart();
			
			if(abs(dragDist.x) < threshold && abs(dragDist.y) < threshold){
				return;
				
			}else{
				pCLSelect->SetRectSelect(true);
				pCLSelect->StoreRectSelect();
			}
		}
		
		UpdateRectSelection();
		
	}else{
		pUpdateInfoBubble(x, y, !control);
	}
}

void meViewEditorSelect::OnMouseWheel(int steps, bool shift, bool control){
	meViewEditorNavigation::OnMouseWheel(steps, shift, control);
	
	if(GetDragLeftMouseButton() && pCLSelect && pCLSelect->GetRectSelect()){
		meWorldGuiParameters &guiparams = GetWorldGuiParameters();
		
		if(steps > 0){
			guiparams.SetRectSelDistance(guiparams.GetRectSelDistance() + guiparams.GetRectSelDistanceStep());
			
		}else if(steps < 0){
			guiparams.SetRectSelDistance(guiparams.GetRectSelDistance() - guiparams.GetRectSelDistanceStep());
		}
		
		GetWorld().NotifyEditingChanged();
		
		UpdateRectSelection();
	}
}

void meViewEditorSelect::OnMousLeave(){
	meViewEditorNavigation::OnMousLeave();
	
	pInfoBubble->Hide();
}



// Private Functions
//////////////////////

void meViewEditorSelect::pCleanUp(){
	if(pCanvasSelect && pCanvasSelect->GetParentView()){
		GetView().RemoveCanvas(pCanvasSelect);
	}
	
	if(pCLBubbleInfo){
		delete pCLBubbleInfo;
	}
	if(pCLSelect){
		delete pCLSelect;
	}
}

class BuildBubbleTextVisitor : public decTVisitor<meCLHitListEntry>{
public:
	meWorldGuiParameters::eElementModes elementMode;
	decStringList lines;
	float rayDistance;
	BuildBubbleTextVisitor() = default;
	
	void operator() (meCLHitListEntry &entry) override{
		decString text;
		
		switch(elementMode){
		case meWorldGuiParameters::eemObject:
			if(entry.GetObject()){
				const meObject * const object = entry.GetObject();
				text.Format("%s%s (%s): %.1f m",
					object->GetActive() ? "[A] " : object->GetSelected() ? "[S] " : "",
					object->GetClassName().GetString(),
					object->GetID().ToHexString().GetString(),
					entry.GetDistance() * rayDistance);
				lines.Insert(text, 0);
			}
			break;
			
		case meWorldGuiParameters::eemObjectShape:
			if(entry.GetObjectShape()){
				const meObjectShape &shape = *entry.GetObjectShape();
				text.Format("%sShape: %.1f m",
					shape.GetActive() ? "[A] " : shape.GetSelected() ? "[S] " : "",
					entry.GetDistance() * rayDistance);
				lines.Insert(text, 0);
			}
			break;
			
		case meWorldGuiParameters::eemDecal:
			if(entry.GetDecal()){
				const meDecal &decal = *entry.GetDecal();
				
				decString path(decal.GetSkinPath());
				if(path.GetLength() > 50){
					path = decString("...") + path.GetRight(50);
				}
				
				text.Format("%s%s",
					decal.GetActive() ? "[A] " : decal.GetSelected() ? "[S] " : "",
					path.GetString());
				lines.Insert(text, 0);
			}
			break;
			
		case meWorldGuiParameters::eemNavSpace:
			if(entry.GetNavigationSpace()){
				const meNavigationSpace &navspace = *entry.GetNavigationSpace();
				text.Format("%sNavSpace: %.1f m",
					navspace.GetActive() ? "[A] " : navspace.GetSelected() ? "[S] " : "",
					entry.GetDistance() * rayDistance);
				lines.Insert(text, 0);
			}
			break;
		}
	}
	
	decString GetText() const{
		return DEJoin(lines, "\n");
	}
	
	void ReverseText(){
		lines.Reverse();
	}
};

void meViewEditorSelect::pUpdateInfoBubble(int x, int y, bool singleElement){
	if(GetDragLeftMouseButton() || GetDragRightMouseButton() || !pCLBubbleInfo){
		return;
	}
	
	const meWorldGuiParameters::eElementModes elementMode = GetElementMode();
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit(meWorld::eclmEditing);
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit(meWorld::eclmDecals);
	collisionFilter.SetBit(meWorld::eclmObjects);
	collisionFilter.SetBit(meWorld::eclmObjectShape);
	collisionFilter.SetBit(meWorld::eclmAI);
	
	pCLBubbleInfo->SetCanSelectObjects(elementMode == meWorldGuiParameters::eemObject);
	pCLBubbleInfo->SetCanSelectObjectShapes(elementMode == meWorldGuiParameters::eemObjectShape);
	pCLBubbleInfo->SetCanSelectDecals(elementMode == meWorldGuiParameters::eemDecal);
	pCLBubbleInfo->SetCanSelectNavSpaces(elementMode == meWorldGuiParameters::eemNavSpace);
	
	pCLBubbleInfo->SetCanHitObjects(elementMode == meWorldGuiParameters::eemObject);
	pCLBubbleInfo->SetCanHitObjectShapes(elementMode == meWorldGuiParameters::eemObjectShape);
	pCLBubbleInfo->SetCanHitDecals(elementMode == meWorldGuiParameters::eemDecal);
	pCLBubbleInfo->SetCanHitNavSpaces(elementMode == meWorldGuiParameters::eemNavSpace);
	
	UpdateMatrices();
	
	pCLBubbleInfo->Reset();
	
	meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	const decDVector rayPosition = GetMatrixView().GetPosition();
	const decPoint size(GetViewWidth(), GetViewHeight());
	const decVector rayDirection(GetActiveCamera().GetDirectionFor(size.x, size.y, x, y)
		* guiparams.GetRectSelDistance());
	RayTestCollision(pCLBubbleInfo, rayPosition, rayDirection,
		decCollisionFilter(collisionCategory, collisionFilter));
	
	meCLHitListEntry::List &collected = pCLBubbleInfo->GetHitList();
	if(collected.IsEmpty()){
		pInfoBubble->Hide();
		return;
	}
	
	collected.SortAscending();
	
	BuildBubbleTextVisitor visitor;
	visitor.rayDistance = guiparams.GetRectSelDistance();
	visitor.elementMode = elementMode;
	visitor.rayDistance = (float)guiparams.GetRectSelDistance();
	
	if(singleElement){
		visitor.operator()(collected.First());
		
	}else{
		const int limit = 5;
		collected.RemoveIf([&](meCLHitListEntry &a){
			return collected.HasMatching([&](const meCLHitListEntry &b){
				return &a != &b && a.IsSame(b);
			});
		});
		
		if(collected.GetCount() <= limit){
			collected.Visit(visitor);
			
		}else{
			collected.Visit(visitor, 0, limit);
			
			decString text;
			text.Format("... and %d more", collected.GetCount() - limit);
			visitor.lines.Insert(text, 0);
		}
	}
	
	pInfoBubbleText->SetText(visitor.GetText());
	const decPoint textSize(pInfoBubbleText->GetFont()->TextSize(pInfoBubbleText->GetText()));
	
	pInfoBubbleText->SetSize(textSize);
	pInfoBubble->GetCanvasContent()->SetSize(textSize);
	
	decPoint position = decPoint(x + 32, y);
	meInfoBubble::ePlacement placement = meInfoBubble::epTopRight;
	
	if(position.x + textSize.x + 6 > size.x){
		position.x = x - 32;
		placement = meInfoBubble::epTopLeft;
	}
	
	if(position.y - textSize.y - 6 < 0){
		position.y = y + 64;
		placement = placement == meInfoBubble::epTopRight
			? meInfoBubble::epBottomRight : meInfoBubble::epBottomLeft;
	}
	
	if(placement == meInfoBubble::epBottomLeft || placement == meInfoBubble::epBottomRight){
		visitor.ReverseText();
		pInfoBubbleText->SetText(visitor.GetText());
	}
	
	pInfoBubble->ShowAt(position, placement);
}
