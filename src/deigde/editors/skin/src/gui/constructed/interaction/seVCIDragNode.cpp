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

#include <stdio.h>
#include <stdlib.h>

#include "seVCIDragNode.h"
#include "../seViewConstructedView.h"
#include "../../../skin/seSkin.h"
#include "../../../skin/texture/seTexture.h"
#include "../../../skin/property/seProperty.h"
#include "../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../undosys/gui/property/node/seUPropertyNodesDrag.h"
#include "../../../undosys/gui/property/node/seUPropertyNodesResize.h"
#include "../../../undosys/gui/property/node/seUPropertyNodesRotate.h"
#include "../../../undosys/gui/property/node/seUPropertyNodesShear.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class seVCIDragNode
/////////////////////////

// Constructor, destructor
////////////////////////////

seVCIDragNode::seVCIDragNode(seViewConstructedView &view) :
pView(view),
pDragAction(edaNone)
{
}

seVCIDragNode::~seVCIDragNode(){
}



// Management
///////////////

bool seVCIDragNode::OnDragBegin(){
	if(!pView.GetSkin() || pDragAction != edaNone || GetDragState() != edsLeft){
		return false;
	}
	
	seProperty * const property = pView.GetActiveProperty();
	if(!property){
		return 0;
	}
	
	pUndo = NULL;
	
	// check if a marker is clicked
	const decPoint &position = GetDragOrigin();
	
	if(pView.GetCanvasMarkerBorder()->GetVisible()){
		if(pInsideMarker(*pView.GetCanvasMarkerResizeTopLeft(), position)){
			pDragAction = edaResizeTopLeft;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeTopRight(), position)){
			pDragAction = edaResizeTopRight;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeBottomLeft(), position)){
			pDragAction = edaResizeBottomLeft;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeBottomRight(), position)){
			pDragAction = edaResizeBottomRight;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeTop(), position)){
			pDragAction = edaResizeTop;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeBottom(), position)){
			pDragAction = edaResizeBottom;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeLeft(), position)){
			pDragAction = edaResizeLeft;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerResizeRight(), position)){
			pDragAction = edaResizeRight;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerRotateTopLeft(), position)){
			pDragAction = edaRotateTopLeft;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerRotateTopRight(), position)){
			pDragAction = edaRotateTopRight;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerRotateBottomLeft(), position)){
			pDragAction = edaRotateBottomLeft;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerRotateBottomRight(), position)){
			pDragAction = edaRotateBottomRight;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerShearTop(), position)){
			pDragAction = edaShearTop;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerShearBottom(), position)){
			pDragAction = edaShearBottom;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerShearLeft(), position)){
			pDragAction = edaShearLeft;
			return true;
			
		}else if(pInsideMarker(*pView.GetCanvasMarkerShearRight(), position)){
			pDragAction = edaShearRight;
			return true;
		}
	}
	
	// activate node
	sePropertyNode * const node = pView.NodeAtPosition((decVector2(
		position - pView.GetCanvasContent()->GetPosition()) / pView.GetZoomScale()).Round());
	
	if(GetShiftOrigin()){
		if(node){
			if(node->GetSelected()){
				property->GetNodeSelection().Remove(node);
				
			}else{
				property->GetNodeSelection().Add(node);
				property->GetNodeSelection().SetActive(node);
			}
		}
		
	}else{
		if(node && !node->GetSelected()){
			property->GetNodeSelection().RemoveAll();
			property->GetNodeSelection().Add(node);
			
		}else if(!node){
			property->GetNodeSelection().RemoveAll();
		}
	}
	
	if(pView.GetActiveNode()){
		pDragMatrix = (pView.GetActiveNode()->GetParent()->CreateScreenTransformMatrix()
			* pView.GetZoomScaleMatrix() ).Invert().ToTexMatrix2().GetRotationMatrix();
		pDragAction = edaMoveNode;
		return true;
	}
	
	return false;
}

void seVCIDragNode::OnDragUpdate(){
	switch(pDragAction){
	case edaMoveNode:
		pMove();
		break;
		
	case edaResizeTopLeft:
	case edaResizeTopRight:
	case edaResizeBottomLeft:
	case edaResizeBottomRight:
	case edaResizeTop:
	case edaResizeBottom:
	case edaResizeLeft:
	case edaResizeRight:
		pResize();
		break;
		
	case edaRotateTopLeft:
	case edaRotateTopRight:
	case edaRotateBottomLeft:
	case edaRotateBottomRight:
		pRotate();
		break;
		
	case edaShearTop:
	case edaShearBottom:
	case edaShearLeft:
	case edaShearRight:
		pShear();
		break;
		
	default:
		break;
	}
}

void seVCIDragNode::OnDragFinish(bool){
	pUndo = NULL;
	pDragAction = edaNone;
}



// Protected Functions
////////////////////////

bool seVCIDragNode::pInsideMarker(const deCanvas &canvas, const decPoint &position) const{
	return position >= canvas.GetPosition() && position <= canvas.GetPosition() + canvas.GetSize();
}

void seVCIDragNode::pMove(){
	const decPoint diff(GetDragDistance());
	
	if(pUndo){
		pUndo->Undo();
		
	}else{
		if(diff.Absolute() < decPoint(3, 3)){
			return; // drag limit
		}
		
		seProperty * const property = pView.GetActiveProperty();
		if(!property){
			return;
		}
		
		pUndo.TakeOver(new seUPropertyNodesDrag(property->GetNodeSelection().GetSelected()));
		pView.GetSkin()->GetUndoSystem()->Add(pUndo, false);
	}
	
	((seUPropertyNodesDrag&)(igdeUndo&)pUndo).SetDistance(decPoint(pDragMatrix * decVector2(diff)));
	pUndo->Redo();
}

void seVCIDragNode::pRotate(){
	const decPoint diff(GetDragDistance());
	
	if(pUndo){
		pUndo->Undo();
		
	}else{
		if(diff.Absolute() < decPoint(3, 3)){
			return; // drag limit
		}
		
		seProperty * const property = pView.GetActiveProperty();
		if(!property){
			return;
		}
		
		const sePropertyNodeSelection &selection = property->GetNodeSelection();
		decTexMatrix2 matrix;
		decVector2 size;
		
		if(selection.GetSelected().GetCount() > 1){
			decVector2 minBounds, maxBounds;
			pView.GetSelectionBoundary(selection.GetSelected(), minBounds, maxBounds);
			
			matrix.SetTranslation(minBounds);
			size = maxBounds - minBounds;
			
		}else{
			const sePropertyNode &node = *selection.GetActive();
			matrix = node.CreateScreenTransformMatrix();
			size = decPoint(node.GetSize().x, node.GetSize().y).Absolute();
		}
		
		// TODO add a new marker to place the pivot manually
		const decVector2 origin(decVector2(GetDragOrigin()
			- pView.GetCanvasContentBackground()->GetPosition()) / pView.GetZoomScale());
		const decVector2 pivot(matrix * (size * 0.5f));
		
		pUndo.TakeOver(new seUPropertyNodesRotate(selection.GetSelected(), pivot, origin));
		pView.GetSkin()->GetUndoSystem()->Add(pUndo, false);
	}
	
	const decVector2 target(decVector2(GetDragPosition()
		- pView.GetCanvasContentBackground()->GetPosition()) / pView.GetZoomScale());
	
	((seUPropertyNodesRotate&)(igdeUndo&)pUndo).SetTarget(target);
	pUndo->Redo();
}

void seVCIDragNode::pResize(){
	const decPoint diff(GetDragDistance());
	decVector2 axis;
	
	if(pUndo){
		pUndo->Undo();
		
	}else{
		if(diff.Absolute() < decPoint(3, 3)){
			return; // drag limit
		}
	}
	
	seProperty * const property = pView.GetActiveProperty();
	if(!property){
		return;
	}
	
	const sePropertyNodeSelection &selection = property->GetNodeSelection();
	decTexMatrix2 matrix;
	decVector2 absSize;
	
	if(selection.GetSelected().GetCount() > 1){
		decVector2 minBounds, maxBounds;
		pView.GetSelectionBoundary(selection.GetSelected(), minBounds, maxBounds);
		
		matrix.SetTranslation(minBounds);
		absSize = maxBounds - minBounds;
		
	}else{
		const sePropertyNode &node = *selection.GetActive();
		matrix = node.CreateScreenTransformMatrix();
		
		const decPoint size(node.GetSize().x, node.GetSize().y);
		absSize = size.Absolute();
		
		if(size.x < 0 || size.y < 0){
			const decVector2 offset(absSize * 0.5f);
			matrix = decTexMatrix2::CreateTranslation(-offset)
				* decTexMatrix2::CreateScale( size.x < 0 ? -1.0f : 1.0f, size.y < 0 ? -1.0f : 1.0f )
				* decTexMatrix2::CreateTranslation( offset )
				* matrix;
		}
	}
	
	const decVector2 origin(decVector2(GetDragOrigin()
		- pView.GetCanvasContentBackground()->GetPosition()) / pView.GetZoomScale());
	decVector2 pivot;
	
	switch(pDragAction){
	case edaResizeTopLeft:
		pivot = matrix * absSize;
		break;
		
	case edaResizeTopRight:
		pivot = matrix * decVector2(0.0f, absSize.y);
		break;
		
	case edaResizeBottomLeft:
		pivot = matrix * decVector2(absSize.x, 0.0f);
		break;
		
	case edaResizeBottomRight:
		pivot = matrix * decVector2();
		break;
		
	case edaResizeTop:
		pivot = matrix * decVector2(absSize.x * 0.5f, absSize.y);
		break;
		
	case edaResizeBottom:
		pivot = matrix * decVector2(absSize.x * 0.5f, 0.0f);
		break;
		
	case edaResizeLeft:
		pivot = matrix * decVector2(absSize.x, absSize.y * 0.5f);
		break;
		
	case edaResizeRight:
		pivot = matrix * decVector2(0.0f, absSize.y * 0.5f);
		break;
		
	default:
		break;
	}
	
	switch(pDragAction){
	case edaResizeTop:
	case edaResizeBottom:
		axis = matrix.GetAxisY();
		if(axis.Length() < 0.5f){
			axis.Set(0.0f, 1.0f);
			
		}else{
			axis.Normalize();
		}
		break;
		
	case edaResizeLeft:
	case edaResizeRight:
		axis = matrix.GetAxisX();
		if(axis.Length() < 0.5f){
			axis.Set(1.0f, 0.0f);
			
		}else{
			axis.Normalize();
		}
		break;
		
	default:
		break;
	}
	
	if(!pUndo){
		pUndo.TakeOver(new seUPropertyNodesResize(selection.GetSelected(),
			pivot, origin, matrix.GetRotation(), matrix.GetShearing()));
		pView.GetSkin()->GetUndoSystem()->Add(pUndo, false);
	}
	
	decVector2 distance(decVector2(diff) / pView.GetZoomScale());
	
	switch(pDragAction){
	case edaResizeTop:
	case edaResizeBottom:
	case edaResizeLeft:
	case edaResizeRight:
		distance = axis * (axis * distance);
		break;
		
	default:
		break;
	}
	
	((seUPropertyNodesResize&)(igdeUndo&)pUndo).SetDistance(distance);
	pUndo->Redo();
}

void seVCIDragNode::pShear(){
	const decPoint diff(GetDragDistance());
	decVector2 axis;
	
	if(pUndo){
		pUndo->Undo();
		
	}else{
		if(diff.Absolute() < decPoint(3, 3)){
			return; // drag limit
		}
	}
	
	seProperty * const property = pView.GetActiveProperty();
	if(!property){
		return;
	}
	
	const sePropertyNodeSelection &selection = property->GetNodeSelection();
	decTexMatrix2 matrix;
	decVector2 absSize;
	
	if(selection.GetSelected().GetCount() > 1){
		decVector2 minBounds, maxBounds;
		pView.GetSelectionBoundary(selection.GetSelected(), minBounds, maxBounds);
		
		matrix.SetTranslation(minBounds);
		absSize = maxBounds - minBounds;
		
	}else{
		const sePropertyNode &node = *selection.GetActive();
		
		matrix = node.CreateScreenTransformMatrix();
		const decPoint size(node.GetSize().x, node.GetSize().y);
		absSize = size.Absolute();
		
		if(size.x < 0 || size.y < 0){
			const decVector2 offset(absSize * 0.5f);
			matrix = decTexMatrix2::CreateTranslation(-offset)
				* decTexMatrix2::CreateScale( size.x < 0 ? -1.0f : 1.0f, size.y < 0 ? -1.0f : 1.0f )
				* decTexMatrix2::CreateTranslation( offset )
				* matrix;
		}
	}
	
	const decVector2 origin(decVector2(GetDragOrigin()
		- pView.GetCanvasContentBackground()->GetPosition()) / pView.GetZoomScale());
	decVector2 pivot;
	
	switch(pDragAction){
	case edaShearTop:
		pivot = matrix * decVector2(absSize.x * 0.5f, absSize.y);
		break;
		
	case edaShearBottom:
		pivot = matrix * decVector2(absSize.x * 0.5f, 0.0f);
		break;
		
	case edaShearLeft:
		pivot = matrix * decVector2(absSize.x, absSize.y * 0.5f);
		break;
		
	case edaShearRight:
		pivot = matrix * decVector2(0.0f, absSize.y * 0.5f);
		break;
		
	default:
		break;
	}
	
	switch(pDragAction){
	case edaShearTop:
	case edaShearBottom:
		axis = matrix.GetAxisX();
		if(axis.Length() < 0.5f){
			axis.Set(1.0f, 0.0f);
			
		}else{
			axis.Normalize();
		}
		break;
		
	case edaShearLeft:
	case edaShearRight:
		axis = matrix.GetAxisY();
		if(axis.Length() < 0.5f){
			axis.Set(0.0f, 1.0f);
			
		}else{
			axis.Normalize();
		}
		break;
		
	default:
		break;
	}
	
	if(!pUndo){
		pUndo.TakeOver(new seUPropertyNodesShear(selection.GetSelected(),
				pivot, origin, matrix.GetRotation(), matrix.GetShearing()));
		pView.GetSkin()->GetUndoSystem()->Add(pUndo);
	}
	
	((seUPropertyNodesShear&)(igdeUndo&)pUndo).SetDistance(
		axis * (axis * decVector2(diff) / pView.GetZoomScale()));
	pUndo->Redo();
}
