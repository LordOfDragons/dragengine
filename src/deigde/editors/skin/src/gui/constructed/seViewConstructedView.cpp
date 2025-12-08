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
#include <stdlib.h>
#include <string.h>

#include "seViewConstructedView.h"
#include "seViewConstructedViewListener.h"
#include "interaction/seVCIKeyHandling.h"
#include "interaction/seVCIDragNode.h"
#include "../seWindowMain.h"
#include "../../clipboard/seClipboardDataPropertyNode.h"
#include "../../configuration/seConfiguration.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeImage.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"
#include "../../skin/property/node/sePropertyNodeShape.h"
#include "../../skin/property/node/sePropertyNodeText.h"
#include "../../undosys/property/seUPropertyConstructedFromImage.h"
#include "../../undosys/property/node/seUPropertyNodeSetMask.h"
#include "../../undosys/property/node/seUPropertyNodeRemoveMask.h"
#include "../../undosys/property/node/image/seUPropertyNodeImageSizeFromImage.h"
#include "../../undosys/property/node/group/seUPNGroupAddNode.h"
#include "../../undosys/property/node/group/seUPNGroupRemoveNodes.h"
#include "../../undosys/property/node/group/seUPNGroupPasteNodes.h"
#include "../../undosys/property/node/group/seUPNGroupNodesBottom.h"
#include "../../undosys/property/node/group/seUPNUngroupNodes.h"
#include "../../undosys/property/node/group/seUPNGroupNodesDown.h"
#include "../../undosys/property/node/group/seUPNGroupNodesTop.h"
#include "../../undosys/property/node/group/seUPNGroupNodesUp.h"
#include "../../undosys/property/node/group/seUPNGroupNodes.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/module/igdeEditorModule.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>



// Actions
////////////

namespace {

class cBaseAction : public igdeAction{
protected:
	seViewConstructedView &pView;
	
public:
	cBaseAction(seViewConstructedView &view, const char *text, igdeIcon *icon,
		const char *description) : igdeAction(text, icon, description), pView(view){}
	
	virtual void OnAction(){
		seSkin * const skin = pView.GetSkin();
		seProperty * const property = pView.GetActiveProperty();
		if(!skin || !property){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(skin, property)));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(seSkin *skin, seProperty *property) = 0;
	
	virtual void Update(){
		seSkin * const skin = pView.GetSkin();
		seProperty * const property = pView.GetActiveProperty();
		if(skin && property){
			Update(*skin, *property);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const seSkin &, const seProperty &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseActionNode : public cBaseAction{
public:
	cBaseActionNode(seViewConstructedView &view, const char *text, igdeIcon *icon,
		const char *description) : cBaseAction(view, text, icon, description){}
	
	virtual igdeUndo *OnAction(seSkin *skin, seProperty *property){
		sePropertyNode * const node = pView.GetActiveNode();
		return node ? OnActionNode(skin, property, node) : NULL;
	}
	
	virtual igdeUndo *OnActionNode(seSkin *skin, seProperty *property, sePropertyNode *node) = 0;
	
	void Update(const seSkin &skin, const seProperty &property) override{
		sePropertyNode * const node = pView.GetActiveNode();
		if(node && node->GetParent()){
			UpdateNode(skin, property, *node);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateNode(const seSkin &, const seProperty &, const sePropertyNode &node){
		SetEnabled(node.GetParent());
		SetSelected(false);
	}
};


class cActionConstructedFromImage : public cBaseAction{
public:
	cActionConstructedFromImage(seViewConstructedView &view) : cBaseAction(view,
		"Constructed from Image", NULL, "Create constructed from image"){}
	
	virtual igdeUndo *OnAction(seSkin*, seProperty *property){
		return property->GetEngineImage() ? new seUPropertyConstructedFromImage(property) : NULL;
	}
};


class cBaseActionAddNode : public cBaseAction{
public:
	cBaseActionAddNode(seViewConstructedView &view, const char *text, igdeIcon *icon,
		const char *description) : cBaseAction(view, text, icon, description){}
	
	virtual igdeUndo *OnAction(seSkin *skin, seProperty *property){
		const sePropertyNode::Ref node(sePropertyNode::Ref::New(CreateNode(*skin, *property)));
		if(!node){
			return nullptr;
		}
		
		node->SetPosition(decPoint3(0, 0, property->GetActiveNodeLayer()));
		
		return new seUPNGroupAddNode(pView.GetActiveNodeGroup()
			? pView.GetActiveNodeGroup() : property->GetNodeGroup(), node);
	}
	
	virtual sePropertyNode *CreateNode(seSkin &skin, seProperty &property) = 0;
};

class cActionAddShape : public cBaseActionAddNode{
public:
	cActionAddShape(seViewConstructedView &view) : cBaseActionAddNode(view, "Add Shape",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add shape node"){}
	
	sePropertyNode *CreateNode(seSkin &, seProperty &) override{
		return new sePropertyNodeShape(*pView.GetEngine());
	}
};

class cActionAddImage : public cBaseActionAddNode{
public:
	cActionAddImage(seViewConstructedView &view) : cBaseActionAddNode(view, "Add Image ...",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add image node"){}
	
	sePropertyNode *CreateNode(seSkin &skin, seProperty &) override{
		igdeEnvironment &env = pView.GetEnvironment();
		decString path(skin.GetDirectoryPath());
		if(!igdeCommonDialogs::GetFileOpen(&pView, "Select Image", *env.GetFileSystemGame(),
		*env.GetOpenFilePatternList(igdeEnvironment::efpltImage), path)){
			return nullptr;
		}
		
		deImage::Ref image(deImage::Ref::New(
			pView.GetEngine()->GetImageManager()->LoadImage(path, "/")));
		
		const sePropertyNodeImage::Ref node(sePropertyNodeImage::Ref::New(
			new sePropertyNodeImage(*pView.GetEngine())));
		
		if(!skin.GetDirectoryPath().IsEmpty() && decPath::IsUnixPathAbsolute(path)){
			path = decPath::RelativePathUnix(path, skin.GetDirectoryPath(), true).GetPathUnix();
		}
		
		node->SetPath(path);
		node->SetSize(decPoint3(image->GetWidth(), image->GetHeight(), image->GetDepth()));
		node->AddReference(); // because we need to hand over a reference
		return node;
	}
};

class cActionAddText : public cBaseActionAddNode{
public:
	cActionAddText(seViewConstructedView &view) : cBaseActionAddNode(view, "Add Text",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add text node"){}
	
	sePropertyNode *CreateNode(seSkin &, seProperty &) override{
		const sePropertyNodeText::Ref node(sePropertyNodeText::Ref::NewWith(*pView.GetEngine()));
		node->SetPath("/igde/fonts/regular_67px.defont");
		node->SetTextSize(67.0f);
		node->SetSize(decPoint3(256, 67, 1));
		node->SetText("Text");
		node->AddReference(); // because we need to hand over a reference
		return node;
	}
};

class cActionRemoveNode : public cBaseActionNode{
public:
	cActionRemoveNode(seViewConstructedView &view) : cBaseActionNode(view, "Remove Nodes",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove nodes"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty *property, sePropertyNode *node){
		return new seUPNGroupRemoveNodes(node->GetParent(), property->GetNodeSelection().GetSelected());
	}
};

class cActionCopyNode : public cBaseActionNode{
public:
	cActionCopyNode(seViewConstructedView &view) : cBaseActionNode(view, "Copy Nodes",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy nodes"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty *property, sePropertyNode*){
		pView.GetWindowMain().GetClipboard().Set(seClipboardDataPropertyNode::Ref::NewWith(
			property->GetNodeSelection().GetSelected()));
		return NULL;
	}
};

class cActionCutNode : public cBaseActionNode{
public:
	cActionCutNode(seViewConstructedView &view) : cBaseActionNode(view, "Cut Nodes",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut), "Cut nodes"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty *property, sePropertyNode *node){
		pView.GetWindowMain().GetClipboard().Set(seClipboardDataPropertyNode::Ref::NewWith(
			property->GetNodeSelection().GetSelected()));
		
		return new seUPNGroupRemoveNodes(node->GetParent(), property->GetNodeSelection().GetSelected());
	}
};

class cActionPasteNode : public cBaseAction{
public:
	cActionPasteNode(seViewConstructedView &view) : cBaseAction(view, "Paste Nodes",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste), "Paste nodes"){}
	
	virtual igdeUndo *OnAction(seSkin*, seProperty *property){
		const seClipboardDataPropertyNode * const data = (seClipboardDataPropertyNode*)
			pView.GetWindowMain().GetClipboard().GetWithTypeName(seClipboardDataPropertyNode::TYPE_NAME);
		return data ? new seUPNGroupPasteNodes(
			pView.GetActiveNodeGroup() ? pView.GetActiveNodeGroup() : property->GetNodeGroup(),
			property->GetActiveNodeLayer(), *data) : NULL;
	}
	
	void Update(const seSkin &, const seProperty &) override{
		SetEnabled(pView.GetWindowMain().GetClipboard().HasWithTypeName(seClipboardDataPropertyNode::TYPE_NAME));
	}
};

class cActionEnterGroup : public cBaseActionNode{
public:
	cActionEnterGroup(seViewConstructedView &view) : cBaseActionNode(view, "Enter Group",
		NULL, "Enter group"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty *property, sePropertyNode *node){
		if(node->GetNodeType() == sePropertyNode::entGroup){
			property->GetNodeSelection().RemoveAll();
			property->SetActiveNodeGroup((sePropertyNodeGroup*)node);
		}
		return NULL;
	}
	
	void UpdateNode(const seSkin &, const seProperty &, const sePropertyNode &node) override{
		SetEnabled(node.GetNodeType() == sePropertyNode::entGroup);
	}
};

class cActionExitGroup : public cBaseAction{
public:
	cActionExitGroup(seViewConstructedView &view) : cBaseAction(view, "Exit Group",
		NULL, "Exit group"){}
	
	virtual igdeUndo *OnAction(seSkin*, seProperty *property){
		sePropertyNodeGroup * const node = property->GetActiveNodeGroup();
		if(!node){
			return NULL;
		}
		
		if(node->GetParent() == property->GetNodeGroup()){
			property->GetNodeSelection().RemoveAll();
			property->SetActiveNodeGroup(NULL);
			property->GetNodeSelection().Add(node);
			
		}else{
			property->SetActiveNodeGroup(node->GetParent());
		}
		return NULL;
	}
	
	void Update(const seSkin &, const seProperty &property) override{
		SetEnabled(property.GetActiveNodeGroup());
	}
};

class cActionGroupNodes : public cBaseAction{
public:
	cActionGroupNodes(seViewConstructedView &view) : cBaseAction(view, "Group Nodes",
		NULL, "Group Nodes"){}
	
	virtual igdeUndo *OnAction(seSkin*, seProperty *property){
		return property->GetNodeSelection().GetSelected().GetCount() > 1
			? new seUPNGroupNodes(property->GetNodeSelection().GetSelected()) : NULL;
	}
	
	void Update(const seSkin &, const seProperty &property) override{
		SetEnabled(property.GetNodeSelection().GetSelected().GetCount() > 1);
	}
};

class cActionUngroupNodes : public cBaseActionNode{
public:
	cActionUngroupNodes(seViewConstructedView &view) : cBaseActionNode(view, "Ungroup Nodes",
		NULL, "Ungroup Nodes"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty*, sePropertyNode *node){
		return node->GetNodeType() == sePropertyNode::entGroup
			? new seUPNUngroupNodes((sePropertyNodeGroup*)node) : NULL;
	}
	
	void UpdateNode(const seSkin &, const seProperty &, const sePropertyNode &node) override{
		SetEnabled(node.GetNodeType() == sePropertyNode::entGroup);
	}
};

class cBaseMoveNodes : public cBaseActionNode{
public:
	cBaseMoveNodes(seViewConstructedView &view, const char *text, igdeIcon *icon,
		const char *description) : cBaseActionNode(view, text, icon, description){}
	
	virtual igdeUndo *OnActionNode(seSkin *skin, seProperty *property, sePropertyNode *node){
		seUPNGroupMoveNodes::Ref undo = NULL;
		if(node){
			undo = CreateUndo(skin, property, node);
			if(!undo->HasAnyEffect()){
				undo = NULL;
			}
		}
		return undo;
	}
	
	virtual seUPNGroupMoveNodes *CreateUndo(seSkin *skin, seProperty *property, sePropertyNode *node) = 0;
};

class cActionMoveNodesTop : public cBaseMoveNodes{
public:
	cActionMoveNodesTop(seViewConstructedView &view) : cBaseMoveNodes(view, "Move Node Top",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongUp), "Move node to top"){}
	
	seUPNGroupMoveNodes *CreateUndo(seSkin*, seProperty *property, sePropertyNode *node) override{
		return new seUPNGroupNodesTop(node->GetParent(), property->GetNodeSelection().GetSelected());
	}
};

class cActionMoveNodesUp : public cBaseMoveNodes{
public:
	cActionMoveNodesUp(seViewConstructedView &view) : cBaseMoveNodes(view, "Move Node Up",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move node up"){}
	
	seUPNGroupMoveNodes *CreateUndo(seSkin*, seProperty *property, sePropertyNode *node) override{
		return new seUPNGroupNodesUp(node->GetParent(), property->GetNodeSelection().GetSelected());
	}
};

class cActionMoveNodesDown : public cBaseMoveNodes{
public:
	cActionMoveNodesDown(seViewConstructedView &view) : cBaseMoveNodes(view, "Move Node Down",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown), "Move node down"){}
	
	seUPNGroupNodesDown *CreateUndo(seSkin*, seProperty *property, sePropertyNode *node) override{
		return new seUPNGroupNodesDown(node->GetParent(), property->GetNodeSelection().GetSelected());
	}
};

class cActionMoveNodesBottom : public cBaseMoveNodes{
public:
	cActionMoveNodesBottom(seViewConstructedView &view) : cBaseMoveNodes(view, "Move Node Bottom",
		view.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown), "Move node to bottom"){}
	
	seUPNGroupMoveNodes *CreateUndo(seSkin*, seProperty *property, sePropertyNode *node) override{
		return new seUPNGroupNodesBottom(node->GetParent(), property->GetNodeSelection().GetSelected());
	}
};

class cActionSetMask : public cBaseActionNode{
public:
	cActionSetMask(seViewConstructedView &view) : cBaseActionNode(view, "Set Mask",
		NULL, "Set Mask"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty *property, sePropertyNode *node){
		const sePropertyNodeList &selection = property->GetNodeSelection().GetSelected();
		if(node->GetMask() || selection.GetCount() != 2){
			return NULL;
		}
		if(selection.GetAt(0) == node){
			return new seUPropertyNodeSetMask(node, selection.GetAt(1));
			
		}else{
			return new seUPropertyNodeSetMask(node, selection.GetAt(0));
		}
	}
	
	void UpdateNode(const seSkin &, const seProperty &property, const sePropertyNode &node) override{
		SetEnabled(!node.GetMask() && property.GetNodeSelection().GetSelected().GetCount() == 2);
	}
};

class cActionRemoveMask : public cBaseActionNode{
public:
	cActionRemoveMask(seViewConstructedView &view) : cBaseActionNode(view, "Remove Mask",
		NULL, "Remove Mask"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty*, sePropertyNode *node){
		return node->GetMask() ? new seUPropertyNodeRemoveMask(node) : NULL;
	}
	
	void UpdateNode(const seSkin &, const seProperty &, const sePropertyNode &node) override{
		SetEnabled(node.GetMask());
	}
};

class cActionSizeFromImage : public cBaseActionNode{
public:
	cActionSizeFromImage(seViewConstructedView &view) : cBaseActionNode(view,
		"Size from image size", NULL, "Set image size to constructed size"){}
	
	virtual igdeUndo *OnActionNode(seSkin*, seProperty*, sePropertyNode *node){
		return node->GetNodeType() == sePropertyNode::entImage
			&& ((sePropertyNodeImage*)node)->GetImage()
			? new seUPropertyNodeImageSizeFromImage((sePropertyNodeImage*)node) : NULL;
	}
	
	void UpdateNode(const seSkin &, const seProperty &, const sePropertyNode &node) override{
		SetEnabled(node.GetNodeType() == sePropertyNode::entImage && ((sePropertyNodeImage&)node).GetImage());
	}
};

}



// Class seViewConstructedView
////////////////////////////////

// Constructor, destructor
////////////////////////////

seViewConstructedView::seViewConstructedView(seWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pListener(NULL),

pZoom(100),
pZoomScale(1.0f),
pBorderSize(5, 5)
{
	pListener = new seViewConstructedViewListener(*this);
	
	pActionConstructedFromImage.TakeOver(new cActionConstructedFromImage(*this));
	pActionAddShape.TakeOver(new cActionAddShape(*this));
	pActionAddImage.TakeOver(new cActionAddImage(*this));
	pActionAddText.TakeOver(new cActionAddText(*this));
	pActionRemoveNode.TakeOver(new cActionRemoveNode(*this));
	pActionCopyNode.TakeOver(new cActionCopyNode(*this));
	pActionCutNode.TakeOver(new cActionCutNode(*this));
	pActionPasteNode.TakeOver(new cActionPasteNode(*this));
	pActionEnterGroup.TakeOver(new cActionEnterGroup(*this));
	pActionExitGroup.TakeOver(new cActionExitGroup(*this));
	pActionGroupNodes.TakeOver(new cActionGroupNodes(*this));
	pActionUngroupNodes.TakeOver(new cActionUngroupNodes(*this));
	pActionMoveNodeTop.TakeOver(new cActionMoveNodesTop(*this));
	pActionMoveNodeUp.TakeOver(new cActionMoveNodesUp(*this));
	pActionMoveNodeDown.TakeOver(new cActionMoveNodesDown(*this));
	pActionMoveNodeBottom.TakeOver(new cActionMoveNodesBottom(*this));
	pActionSetMask.TakeOver(new cActionSetMask(*this));
	pActionRemoveMask.TakeOver(new cActionRemoveMask(*this));
	pActionSizeFromImage.TakeOver(new cActionSizeFromImage(*this));
	
	pKeyHandling.TakeOver(new seVCIKeyHandling(*this));
	AddListener(pKeyHandling);
	
	pDragNode.TakeOver(new seVCIDragNode(*this));
	AddListener(pDragNode);
}

seViewConstructedView::~seViewConstructedView(){
	SetSkin(NULL);
	
	if(pListener){
		delete pListener;
	}
}



// Management
///////////////

void seViewConstructedView::ResetView(){
}

void seViewConstructedView::SetSkin(seSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	pDragNode->Cancel();
	
	if(pSkin){
		pSkin->RemoveListener(pListener);
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddListener(pListener);
		OnResize();
	}
	
	UpdateConstructedCanvas();
	RecreateContentCanvas();
	UpdateMarkers();
}

void seViewConstructedView::SetZoom(int zoom){
	zoom = decMath::clamp(zoom, 25, 800);
	if(zoom == pZoom){
		return;
	}
	
	pZoom = zoom;
	pZoomScale = (float)pZoom * 0.01f;
	pZoomScaleMatrix.SetScale(pZoomScale, pZoomScale);
	
	OnResize();
}

void seViewConstructedView::SetOffset(const decPoint &offset){
	if(offset == pOffset){
		return;
	}
	
	pOffset = offset;
	OnResize();
}

decPoint seViewConstructedView::GetContentSize() const{
	return pCanvasContentBackground ? pCanvasContentBackground->GetSize() + pBorderSize * 2 : decPoint();
}



seProperty *seViewConstructedView::GetActiveProperty() const{
	return pSkin && pSkin->GetActiveTexture() ? pSkin->GetActiveTexture()->GetActiveProperty() : NULL;
}

sePropertyNode *seViewConstructedView::GetActiveNode() const{
	seProperty * const property = GetActiveProperty();
	if(!property){
		return NULL;
	}
	
	sePropertyNode * const node = property->GetNodeSelection().GetActive();
	if(!node){
		return NULL;
	}
	
	const int activeLayer = property->GetActiveNodeLayer();
	if(activeLayer < node->GetPosition().z || activeLayer >= node->GetPosition().z + node->GetSize().z){
		return NULL;
	}
	
	return node;
}

sePropertyNodeGroup *seViewConstructedView::GetActiveNodeGroup() const{
	seProperty * const property = GetActiveProperty();
	if(!property){
		return NULL;
	}
	
	sePropertyNodeGroup * const nodeGroup = property->GetActiveNodeGroup();
	if(!nodeGroup){
		return NULL;
	}
	
	const int activeLayer = property->GetActiveNodeLayer();
	if(activeLayer < nodeGroup->GetPosition().z || activeLayer >= nodeGroup->GetPosition().z + nodeGroup->GetSize().z){
		return NULL;
	}
	
	return nodeGroup;
}

sePropertyNode *seViewConstructedView::NodeAtPosition(const decPoint &position) const{
	seProperty * const property = GetActiveProperty();
	if(!property){
		return NULL;
	}
	
	sePropertyNodeGroup *nodeGroup = property->GetActiveNodeGroup();
	decPoint groupPosition(position);
	
	if(nodeGroup){
		groupPosition = nodeGroup->CreateScreenTransformMatrix().Invert() * groupPosition;
		
	}else{
		nodeGroup = property->GetNodeGroup();
	}
	
	const int activeLayer = property->GetActiveNodeLayer();
	const int count = nodeGroup->GetNodeCount();
	int i;
	
	for(i=count-1; i>=0; i--){
		sePropertyNode * const node = nodeGroup->GetNodeAt(i);
		if(activeLayer < node->GetPosition().z || activeLayer >= node->GetPosition().z + node->GetSize().z) {
			continue;
		}
		
		const decVector2 npos(node->CreateParentTransformMatrix().Invert().ToTexMatrix2() * decVector2(groupPosition));
		const decVector2 nsize(fabsf((float)node->GetSize().x), fabsf((float)node->GetSize().y));
		
		if(npos >= decVector2() && npos < nsize){
			return node;
		}
	}
	
	return NULL;
}



void seViewConstructedView::UpdateConstructedCanvas(){
	if(!pCanvasContent){
		return;
	}
	
	const seProperty * const property = GetActiveProperty();
	if(!property){
		pCanvasContentBackground->SetVisible(false);
		return;
	}
	
	const float zoom = (float)pZoom * 0.01f;
	sePropertyNodeGroup &content = *property->GetNodeGroup();
	const decPoint panelSize(GetRenderAreaSize());
	const decPoint constructedSize(
		(int)(content.GetSize().x * zoom),
		(int)(content.GetSize().y * zoom));
	const decPoint constructedPosition((panelSize - constructedSize) / 2 + pOffset);
	
	pCanvasContentBackground->SetSize(constructedSize);
	pCanvasContentBackground->SetPosition(constructedPosition);
	pCanvasContentBackground->SetVisible(true);
	pCanvasContentBackground->SetFillColor(property->GetNodeColor());
	
	pCanvasContent->SetSize(decPoint(content.GetSize().x, content.GetSize().y));
	pCanvasContent->SetPosition(constructedPosition);
	pCanvasContent->SetVisible(true);
	pCanvasContent->SetTransform(decTexMatrix2::CreateScale(zoom, zoom));
}

void seViewConstructedView::RecreateContentCanvas(){
	if(!pCanvasContent){
		return;
	}
	
	pCanvasContent->RemoveAllCanvas();
	
	const seProperty * const property = GetActiveProperty();
	if(!property){
		return;
	}
	
	pRecreateContentCanvas(*property->GetNodeGroup(), pCanvasContent);
	pUpdateContentCanvasParams(*property->GetNodeGroup(), pCanvasContent);
}

void seViewConstructedView::UpdateContentCanvasParams(){
	const seProperty * const property = GetActiveProperty();
	if(!property){
		return;
	}
	
	pUpdateContentCanvasParams(*property->GetNodeGroup(), pCanvasContent);
}

void seViewConstructedView::UpdateMarkers(){
	if(!pCanvasContent){
		return;
	}
	
	seProperty * const activeProperty = GetActiveProperty();
	sePropertyNode * const activeNode = GetActiveNode();
	
	if(!activeProperty || !activeNode){
		pCanvasMarkerBorder->SetVisible(false);
		pCanvasMarkerResizeTopLeft->SetVisible(false);
		pCanvasMarkerResizeTop->SetVisible(false);
		pCanvasMarkerResizeTopRight->SetVisible(false);
		pCanvasMarkerResizeLeft->SetVisible(false);
		pCanvasMarkerResizeRight->SetVisible(false);
		pCanvasMarkerResizeBottomLeft->SetVisible(false);
		pCanvasMarkerResizeBottomRight->SetVisible(false);
		pCanvasMarkerResizeBottom->SetVisible(false);
		pCanvasMarkerRotateTopLeft->SetVisible(false);
		pCanvasMarkerRotateTopRight->SetVisible(false);
		pCanvasMarkerRotateBottomLeft->SetVisible(false);
		pCanvasMarkerRotateBottomRight->SetVisible(false);
		pCanvasMarkerShearTop->SetVisible(false);
		pCanvasMarkerShearBottom->SetVisible(false);
		pCanvasMarkerShearLeft->SetVisible(false);
		pCanvasMarkerShearRight->SetVisible(false);
		return;
	}
	
	// transformation
	const sePropertyNodeSelection &selection = activeProperty->GetNodeSelection();
	const decVector2 offsetScreen(pCanvasContentBackground->GetPosition());
	decTexMatrix2 transform;
	decPoint3 nodeSize;
	
	if(selection.GetSelected().GetCount() > 1){
		decVector2 minBounds, maxBounds;
		GetSelectionBoundary(selection.GetSelected(), minBounds, maxBounds);
		
		transform = decTexMatrix2::CreateTranslation(minBounds)
			* pZoomScaleMatrix * decTexMatrix2::CreateTranslation( offsetScreen );
		
		const decPoint rounded((maxBounds - minBounds).Round());
		nodeSize.Set(rounded.x, rounded.y, 1);
		
	}else{
		transform = activeNode->CreateScreenTransformMatrix()
			* pZoomScaleMatrix * decTexMatrix2::CreateTranslation( offsetScreen );
		
		nodeSize = activeNode->GetSize().Absolute();
		
		if(activeNode->GetSize().x < 0 || activeNode->GetSize().y < 0){
			const decVector2 offset((float)nodeSize.x * 0.5f, (float)nodeSize.y * 0.5f);
			transform =
				decTexMatrix2::CreateTranslation(-offset)
				* decTexMatrix2::CreateScale(
					activeNode->GetSize().x < 0 ? -1.0f : 1.0f,
					activeNode->GetSize().y < 0 ? -1.0f : 1.0f)
				* decTexMatrix2::CreateTranslation( offset )
				* transform;
		}
	}
	
	const decPoint cornerTopLeft(transform.GetPosition().Round());
	const decPoint cornerTopRight((transform * decVector2((float)nodeSize.x, 0.0f)).Round());
	const decPoint cornerBottomLeft((transform * decVector2(0.0f, (float)nodeSize.y)).Round());
	const decPoint cornerBottomRight((transform * decVector2((float)nodeSize.x, (float)nodeSize.y)).Round());
	
	const decPoint centerTop((cornerTopLeft + cornerTopRight) / 2);
	const decPoint centerBottom((cornerBottomLeft + cornerBottomRight) / 2);
	const decPoint centerLeft((cornerTopLeft + cornerBottomLeft) / 2);
	const decPoint centerRight((cornerTopRight + cornerBottomRight) / 2);
	
	const decVector2 sizeMarker(pCanvasMarkerResizeTopLeft->GetSize()); // for all the same
	
	const float markerRotation = transform.GetRotation();
	const decTexMatrix2 markerTransform(
		decTexMatrix2::CreateTranslation(sizeMarker * -0.5f)
		* decTexMatrix2::CreateRotation( markerRotation )
		* decTexMatrix2::CreateTranslation( sizeMarker * 0.5f ) );
		// this transform looks strange since it is based on the screen transform. in this transform the
		// position is also rotate around the canvas center in contrary to regular canvas where the
		// top-left corner stays top-left and only the transform rotates. for this reason no half-size
		// offset translation is done before and after the shearing/rotation part
	
	const decPoint offsetMarkerPosition(-sizeMarker * 0.5f);
	
	const decPoint offsetInnerX((markerTransform.GetAxisX().Normalized() * (sizeMarker.x * 0.5f)).Round());
	const decPoint offsetOuterX((markerTransform.GetAxisX().Normalized() * (sizeMarker.x * 1.5f)).Round());
	const decPoint offsetInnerY((markerTransform.GetAxisY().Normalized() * (sizeMarker.y * 0.5f)).Round());
	const decPoint offsetOuterY((markerTransform.GetAxisY().Normalized() * (sizeMarker.y * 1.5f)).Round());
	
	// center
	pCanvasMarkerBorder->SetPosition(transform.GetPosition().Round());
	
	const decVector2 scaling(transform.GetScaling());
	pCanvasMarkerBorder->SetSize(decVector2(scaling.x * nodeSize.x, scaling.y * nodeSize.y).Round());
	
	pCanvasMarkerBorder->SetTransform(
		decTexMatrix2::CreateShear(transform.GetShearing(), 0.0f)
		* decTexMatrix2::CreateRotation( markerRotation ) );
		// this transform looks strange since it is based on the screen transform. in this transform the
		// position is also rotate around the canvas center in contrary to regular canvas where the
		// top-left corner stays top-left and only the transform rotates. for this reason no half-size
		// offset translation is done before and after the shearing/rotation part
	pCanvasMarkerBorder->SetVisible(true);
	
	// inner controls
	pCanvasMarkerResizeTopLeft->SetPosition(cornerTopLeft - offsetInnerX - offsetInnerY + offsetMarkerPosition);
	pCanvasMarkerResizeTopLeft->SetTransform(markerTransform);
	pCanvasMarkerResizeTopLeft->SetVisible(true);
	
	pCanvasMarkerResizeTopRight->SetPosition(cornerTopRight + offsetInnerX - offsetInnerY + offsetMarkerPosition);
	pCanvasMarkerResizeTopRight->SetTransform(markerTransform);
	pCanvasMarkerResizeTopRight->SetVisible(true);
	
	pCanvasMarkerResizeBottomLeft->SetPosition(cornerBottomLeft - offsetInnerX + offsetInnerY + offsetMarkerPosition);
	pCanvasMarkerResizeBottomLeft->SetTransform(markerTransform);
	pCanvasMarkerResizeBottomLeft->SetVisible(true);
	
	pCanvasMarkerResizeBottomRight->SetPosition(cornerBottomRight + offsetInnerX + offsetInnerY + offsetMarkerPosition);
	pCanvasMarkerResizeBottomRight->SetTransform(markerTransform);
	pCanvasMarkerResizeBottomRight->SetVisible(true);
	
	pCanvasMarkerResizeTop->SetPosition(centerTop - offsetInnerY + offsetMarkerPosition);
	pCanvasMarkerResizeTop->SetTransform(markerTransform);
	pCanvasMarkerResizeTop->SetVisible(true);
	
	pCanvasMarkerResizeBottom->SetPosition(centerBottom + offsetInnerY + offsetMarkerPosition);
	pCanvasMarkerResizeBottom->SetTransform(markerTransform);
	pCanvasMarkerResizeBottom->SetVisible(true);
	
	pCanvasMarkerResizeLeft->SetPosition(centerLeft - offsetInnerX + offsetMarkerPosition);
	pCanvasMarkerResizeLeft->SetTransform(markerTransform);
	pCanvasMarkerResizeLeft->SetVisible(true);
	
	pCanvasMarkerResizeRight->SetPosition(centerRight + offsetInnerX + offsetMarkerPosition);
	pCanvasMarkerResizeRight->SetTransform(markerTransform);
	pCanvasMarkerResizeRight->SetVisible(true);
	
	// outer controls
	pCanvasMarkerRotateTopLeft->SetPosition(cornerTopLeft - offsetOuterX - offsetOuterY + offsetMarkerPosition);
	pCanvasMarkerRotateTopLeft->SetTransform(markerTransform);
	pCanvasMarkerRotateTopLeft->SetVisible(true);
	
	pCanvasMarkerRotateTopRight->SetPosition(cornerTopRight + offsetOuterX - offsetOuterY + offsetMarkerPosition);
	pCanvasMarkerRotateTopRight->SetTransform(markerTransform);
	pCanvasMarkerRotateTopRight->SetVisible(true);
	
	pCanvasMarkerRotateBottomLeft->SetPosition(cornerBottomLeft - offsetOuterX + offsetOuterY + offsetMarkerPosition);
	pCanvasMarkerRotateBottomLeft->SetTransform(markerTransform);
	pCanvasMarkerRotateBottomLeft->SetVisible(true);
	
	pCanvasMarkerRotateBottomRight->SetPosition(cornerBottomRight + offsetOuterX + offsetOuterY + offsetMarkerPosition);
	pCanvasMarkerRotateBottomRight->SetTransform(markerTransform);
	pCanvasMarkerRotateBottomRight->SetVisible(true);
	
	pCanvasMarkerShearTop->SetPosition(centerTop - offsetOuterY + offsetMarkerPosition);
	pCanvasMarkerShearTop->SetTransform(markerTransform);
	pCanvasMarkerShearTop->SetVisible(true);
	
	pCanvasMarkerShearBottom->SetPosition(centerBottom + offsetOuterY + offsetMarkerPosition);
	pCanvasMarkerShearBottom->SetTransform(markerTransform);
	pCanvasMarkerShearBottom->SetVisible(true);
	
	pCanvasMarkerShearLeft->SetPosition(centerLeft - offsetOuterX + offsetMarkerPosition);
	pCanvasMarkerShearLeft->SetTransform(markerTransform);
	pCanvasMarkerShearLeft->SetVisible(true);
	
	pCanvasMarkerShearRight->SetPosition(centerRight + offsetOuterX + offsetMarkerPosition);
	pCanvasMarkerShearRight->SetTransform(markerTransform);
	pCanvasMarkerShearRight->SetVisible(true);
}


void seViewConstructedView::UpdateGroupDarkening(){
	/*seProperty * const property = GetActiveProperty();
	sePropertyNodeGroup *nodeGroup = NULL;
	
	if(property){
		nodeGroup = property->GetActiveNodeGroup();
	}
	
	if(!nodeGroup){*/
		pCanvasGroupDarkeningTop->SetVisible(false);
		pCanvasGroupDarkeningBottom->SetVisible(false);
		pCanvasGroupDarkeningLeft->SetVisible(false);
		pCanvasGroupDarkeningRight->SetVisible(false);
		/*return;
	}
	
	decPoint topLeft, bottomRight;
	GetNodeScreenBoundingBox(*nodeGroup, topLeft, bottomRight);
	
	const int width = getWidth();
	const int height = getHeight();
	
	pCanvasGroupDarkeningTop->SetPosition(decPoint());
	pCanvasGroupDarkeningTop->SetSize(decPoint(width, topLeft.y));
	pCanvasGroupDarkeningTop->SetVisible(true);
	
	pCanvasGroupDarkeningBottom->SetPosition(decPoint(0, bottomRight.y));
	pCanvasGroupDarkeningBottom->SetSize(decPoint(width, height - bottomRight.y));
	pCanvasGroupDarkeningBottom->SetVisible(true);
	
	pCanvasGroupDarkeningLeft->SetPosition(decPoint(0, topLeft.y));
	pCanvasGroupDarkeningLeft->SetSize(decPoint(topLeft.x, bottomRight.y - topLeft.y));
	pCanvasGroupDarkeningLeft->SetVisible(true);
	
	pCanvasGroupDarkeningRight->SetPosition(decPoint(bottomRight.x, topLeft.y));
	pCanvasGroupDarkeningRight->SetSize(decPoint(width - bottomRight.x, bottomRight.y - topLeft.y));
	pCanvasGroupDarkeningRight->SetVisible(true);*/
}



void seViewConstructedView::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pCanvasContent){
		return;
	}
	
	// NOTE for deSkinPropertyNode only these properties to store make sense:
	//      - Point3 position
	//      - Point3 size
	//      - float rotation
	//      - float shearing
	//      rotation and slant apply to XY-coordinates only. rotation rotates the node
	//      counter-clock-wise around the node center (position + size / 2). shearing
	//      shears along the X-axis defined as angle between the sheared X-axis and
	//      Y-axis relative to the orthogonal case. positive shearing increases the
	//      angle from 90 degrees onwards and thus shears to the left.
	//      the left. anything more complex is useless since the texture matrix for
	//      transformation works like this:
	//      [ Xaxis.x | Yaxis.x | offset.x ]
	//      [ Xaxis.y | Yaxis.y | offset.y ]
	//      Xaxis and Yaxis can only carry the following information:
	//      - length(Xaxis): scaling in X direction
	//      - length(Yaxis): scaling in Y direction
	//      - angle between Yaxis and Canvas-Yaxis: rotation
	//      - angle between Xaxis and Yaxis: 90-degree + shearing
	//      Important to note is that shearing in Y direction is nothing else but
	//      shearing in X direction combined with an appropriate rotation. Thus rotation
	//      and shearing covers all possible scenarios. The offset is already included
	//      in the position and the scaling is already included in the size. Thus only
	//      rotation and shearing are required to be defined in addition. The folliwng
	//      rules allows to convert between TexMatrix2 and node parameters:
	//      - position.x = texMatrix.position.x
	//      - position.y = texMatrix.position.y
	//      - size.x = length(texMatrix.axisX)
	//      - size.y = length(texMatrix.axisY)
	//      - rotation = atan2(texMatrix.axisX.y, texMatrix.axisX.x)
	//      - shearing = 90 - dot(normalized(texMatrix.axisX), normalized(texMatrix.axisY))
	//      
	//      for editing purpose it could be useful to use a TexMatrix2 with two new methods
	//        void texMatrix2NodeParam(const TexMatrix, positon, size, rotation, shearing)
	//      and
	//        void nodeParam2TexMatrix(const positon, const size, const rotation, const shearing, TexMatrix2)
	//      for simple transformation of a single node this is not required but for transforming
	//      multiple nodes at the same this this would be the most simple solution.
	//      nodeParam2TexMatrix can be done by combining TexMatrix2 creation methods.
	//      for shearing the CreateShear() method has to be used with 0 for y-shearing and
	//      tan(shearing) as x-shearing.
	
	// TODO show wrapping texture by using 8 CanvasCanvasView placed aruond content canvas
	//      showing content canvas with a transparency value to show more or less strong
	
	deCanvasManager &canvasManager = *GetEngineController().GetEngine()->GetCanvasManager();
	const decColor colorBackground(0.65f, 0.65f, 0.65f);
	
	// constructed image background
	pCanvasContentBackground.TakeOver(canvasManager.CreateCanvasPaint());
	pCanvasContentBackground->SetOrder(1.0f);
	pCanvasContentBackground->SetShapeType(deCanvasPaint::estRectangle);
	pCanvasContentBackground->SetThickness(0.0f);
	AddCanvas(pCanvasContentBackground);
	
	// constructed image content
	pCanvasContent.TakeOver(canvasManager.CreateCanvasView());
	pCanvasContent->SetOrder(2.0f);
	AddCanvas(pCanvasContent);
	
	// group darkening
	pCreateDarkeningCanvas(pCanvasGroupDarkeningTop, 3.0f);
	AddCanvas(pCanvasGroupDarkeningTop);
	
	pCreateDarkeningCanvas(pCanvasGroupDarkeningBottom, 3.1f);
	AddCanvas(pCanvasGroupDarkeningBottom);
	
	pCreateDarkeningCanvas(pCanvasGroupDarkeningLeft, 3.2f);
	AddCanvas(pCanvasGroupDarkeningLeft);
	
	pCreateDarkeningCanvas(pCanvasGroupDarkeningRight, 3.3f);
	AddCanvas(pCanvasGroupDarkeningRight);
	
	// markers
	pCanvasMarkerBorder.TakeOver(canvasManager.CreateCanvasPaint());
	pCanvasMarkerBorder->SetOrder(4.0f);
	pCanvasMarkerBorder->SetShapeType(deCanvasPaint::estRectangle);
	pCanvasMarkerBorder->SetLineColor(decColor(0.5f, 0.5f, 0.5f, 0.5f));
	pCanvasMarkerBorder->SetFillColor(decColor(0.5f, 0.5f, 0.5f, 0.1f));
	pCanvasMarkerBorder->SetVisible(false);
	AddCanvas(pCanvasMarkerBorder);
	
	pCreateMarkerCanvas(pCanvasMarkerResizeTopLeft, "marker/resize_top_left.png", 4.05f);
	AddCanvas(pCanvasMarkerResizeTopLeft);
	pCreateMarkerCanvas(pCanvasMarkerResizeTopRight, "marker/resize_top_right.png", 4.1f);
	AddCanvas(pCanvasMarkerResizeTopRight);
	pCreateMarkerCanvas(pCanvasMarkerResizeBottomLeft, "marker/resize_top_right.png", 4.15f);
	AddCanvas(pCanvasMarkerResizeBottomLeft);
	pCreateMarkerCanvas(pCanvasMarkerResizeBottomRight, "marker/resize_top_left.png", 4.2f);
	AddCanvas(pCanvasMarkerResizeBottomRight);
	pCreateMarkerCanvas(pCanvasMarkerResizeTop, "marker/resize_y.png", 4.25f);
	AddCanvas(pCanvasMarkerResizeTop);
	pCreateMarkerCanvas(pCanvasMarkerResizeBottom, "marker/resize_y.png", 4.3f);
	AddCanvas(pCanvasMarkerResizeBottom);
	pCreateMarkerCanvas(pCanvasMarkerResizeLeft, "marker/resize_x.png", 4.35f);
	AddCanvas(pCanvasMarkerResizeLeft);
	pCreateMarkerCanvas(pCanvasMarkerResizeRight, "marker/resize_x.png", 4.4f);
	AddCanvas(pCanvasMarkerResizeRight);
	pCreateMarkerCanvas(pCanvasMarkerRotateTopLeft, "marker/rotate_top_left.png", 4.45f);
	AddCanvas(pCanvasMarkerRotateTopLeft);
	pCreateMarkerCanvas(pCanvasMarkerRotateTopRight, "marker/rotate_top_right.png", 4.5f);
	AddCanvas(pCanvasMarkerRotateTopRight);
	pCreateMarkerCanvas(pCanvasMarkerRotateBottomLeft, "marker/rotate_bottom_left.png", 4.55f);
	AddCanvas(pCanvasMarkerRotateBottomLeft);
	pCreateMarkerCanvas(pCanvasMarkerRotateBottomRight, "marker/rotate_bottom_right.png", 4.6f);
	AddCanvas(pCanvasMarkerRotateBottomRight);
	pCreateMarkerCanvas(pCanvasMarkerShearTop, "marker/shear_x.png", 4.65f);
	AddCanvas(pCanvasMarkerShearTop);
	pCreateMarkerCanvas(pCanvasMarkerShearBottom, "marker/shear_x.png", 4.7f);
	AddCanvas(pCanvasMarkerShearBottom);
	pCreateMarkerCanvas(pCanvasMarkerShearLeft, "marker/shear_y.png", 4.75f);
	AddCanvas(pCanvasMarkerShearLeft);
	pCreateMarkerCanvas(pCanvasMarkerShearRight, "marker/shear_y.png", 4.8f);
	AddCanvas(pCanvasMarkerShearRight);
	
	// resize
	OnResize();
}

void seViewConstructedView::OnResize(){
	UpdateConstructedCanvas();
	UpdateMarkers();
}

void seViewConstructedView::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	// TODO?
}

void seViewConstructedView::GetSelectionBoundary(const sePropertyNodeList &list,
decVector2 &minBounds, decVector2 &maxBounds){
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const sePropertyNode &childNode = *list.GetAt(i);
		const decTexMatrix2 childTransform(childNode.CreateScreenTransformMatrix());
		const decPoint3 &childSize = childNode.GetSize();
		const decVector2 absSize((float)abs(childSize.x), (float)abs(childSize.y));
		
		const decVector2 p1(childTransform.GetPosition());
		const decVector2 p2(childTransform * decVector2(absSize.x, 0.0f));
		const decVector2 p3(childTransform * decVector2(0.0f, absSize.y));
		const decVector2 p4(childTransform * absSize);
		
		const decVector2 smallest(p1.Smallest(p2).Smallest(p3).Smallest(p4));
		const decVector2 largest(p1.Largest(p2).Largest(p3).Largest(p4));
		
		if(i == 0){
			minBounds = smallest;
			maxBounds = largest;
			
		}else{
			minBounds.SetSmallest(smallest);
			maxBounds.SetLargest(largest);
		}
	}
}



// Private Functions
//////////////////////

void seViewConstructedView::pCreateMarkerCanvas(deCanvasImage::Ref &canvas,
const char *pathImage, float order) const{
	decPath path;
	path.SetFromUnix("/data/modules");
	path.AddComponent(pWindowMain.GetEditorModule().GetEditorDirectory());
	path.AddComponent("images");
	path.AddUnixPath(pathImage);
	
	deImage::Ref image(deImage::Ref::New(pWindowMain.GetEngine()->GetImageManager()->LoadImage(
		pWindowMain.GetEnvironment().GetFileSystemIGDE(), path.GetPathUnix(), "/")));
	
	canvas.TakeOver(pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasImage());
	canvas->SetOrder(order);
	canvas->SetVisible(false);
	canvas->SetSize(decPoint(image->GetWidth(), image->GetHeight()));
	canvas->SetImage(image);
}

void seViewConstructedView::pCreateDarkeningCanvas(deCanvasPaint::Ref &canvas, float order) const{
	const decColor darkeningColor(0.0f, 0.0f, 0.0f);
	const float darkeningAlpha = 0.5f; //0.25f
	
	canvas.TakeOver(pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasPaint());
	canvas->SetOrder(order);
	canvas->SetShapeType(deCanvasPaint::estRectangle);
	canvas->SetThickness(0.0f);
	canvas->SetFillColor(darkeningColor);
	canvas->SetTransparency(darkeningAlpha);
	canvas->SetVisible(false);
}



void seViewConstructedView::pRecreateContentCanvas(const sePropertyNodeGroup &nodeGroup, deCanvasView &canvasView){
	deCanvasManager &canvasManager = *pWindowMain.GetEngine()->GetCanvasManager();
	const int activeLayer = GetActiveProperty()->GetActiveNodeLayer();
	const int count = nodeGroup.GetNodeCount();
	deCanvas::Ref canvas;
	int i;
	
	for(i=0; i<count; i++){
		const sePropertyNode &node = *nodeGroup.GetNodeAt(i);
		if(activeLayer < node.GetPosition().z || activeLayer >= node.GetPosition().z + node.GetSize().z) {
			continue;
		}
		
		switch(node.GetNodeType()){
		case sePropertyNodeImage::entImage:
			canvas.TakeOver(canvasManager.CreateCanvasImage());
			break;
			
		case sePropertyNodeImage::entShape:
			canvas.TakeOver(canvasManager.CreateCanvasPaint());
			break;
			
		case sePropertyNodeImage::entText:
			canvas.TakeOver(canvasManager.CreateCanvasText());
			break;
			
		case sePropertyNodeImage::entGroup:
			canvas.TakeOver(canvasManager.CreateCanvasView());
			pRecreateContentCanvas((const sePropertyNodeGroup &)node, (deCanvasView&)(deCanvas&)canvas);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
		canvasView.AddCanvas(canvas);
	}
}

void seViewConstructedView::pUpdateContentCanvasParams(const sePropertyNodeGroup &nodeGroup, deCanvasView &canvasView){
	const int count = nodeGroup.GetNodeCount();
	if(count == 0){
		return;
	}
	
	const int activeLayer = GetActiveProperty()->GetActiveNodeLayer();
	deCanvas *canvas = canvasView.GetRootCanvas();
	deCanvasVisitorIdentify identify;
	int i;
	
	for(i=0; i<count; i++){
		const sePropertyNode &node = *nodeGroup.GetNodeAt(i);
		if(activeLayer < node.GetPosition().z || activeLayer >= node.GetPosition().z + node.GetSize().z) {
			continue;
		}
		
		if(!canvas){
			DETHROW(deeInvalidParam);
		}
		
		identify.Reset();
		canvas->Visit(identify);
		
		canvas->SetPosition(decPoint(node.GetPosition().x, node.GetPosition().y));
		canvas->SetSize(decPoint(node.GetSize().x, node.GetSize().y));
		canvas->SetTransparency(node.GetTransparency());
		
		canvas->SetTransform(node.CreateCanvasTransformMatrix());
		canvas->SetColorTransform(node.CreateCanvasColorTransformMatrix());
		//node.GetGamma();
		
		//node.GetMask();
		
		switch(node.GetNodeType()){
		case sePropertyNodeImage::entImage:{
			deCanvasImage &canvasImage = identify.CastToImage();
			const sePropertyNodeImage &nodeImage = (const sePropertyNodeImage &)node;
			
			canvasImage.SetImage(nodeImage.GetImage());
			canvasImage.SetRepeatX(nodeImage.GetRepeat().x);
			canvasImage.SetRepeatY(nodeImage.GetRepeat().y);
			}break;
			
		case sePropertyNodeImage::entShape:{
			deCanvasPaint &canvasPaint = identify.CastToPaint();
			const sePropertyNodeShape &nodeShape = (const sePropertyNodeShape &)node;
			
			switch(nodeShape.GetShapeType()){
			case deSkinPropertyNodeShape::estRectangle:
				canvasPaint.SetShapeType(deCanvasPaint::estRectangle);
				break;
				
			case deSkinPropertyNodeShape::estEllipse:
				canvasPaint.SetShapeType(deCanvasPaint::estEllipse);
				break;
			}
			
			canvasPaint.SetFillColor(nodeShape.GetFillColor());
			canvasPaint.SetLineColor(nodeShape.GetLineColor());
			canvasPaint.SetThickness(nodeShape.GetThickness());
			}break;
			
		case sePropertyNodeImage::entText:{
			deCanvasText &canvasText = identify.CastToText();
			const sePropertyNodeText &nodeText = (const sePropertyNodeText &)node;
			
			canvasText.SetColor(nodeText.GetColor());
			canvasText.SetFont(nodeText.GetFont());
			canvasText.SetFontSize(nodeText.GetTextSize());
			canvasText.SetText(nodeText.GetText());
			}break;
			
		case sePropertyNodeImage::entGroup:
			pUpdateContentCanvasParams((const sePropertyNodeGroup &)node, identify.CastToView());
			break;
		}
		
		canvas = canvas->GetLLViewNext();
	}
}
