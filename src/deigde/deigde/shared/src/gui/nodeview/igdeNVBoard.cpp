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
#include <string.h>

#include "igdeNVBoard.h"
#include "igdeNVBoardListener.h"
#include "igdeNVNode.h"
#include "igdeNVSlot.h"
#include "igdeNVLink.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeUIHelper.h"
#include "../event/igdeAction.h"
#include "../menu/igdeMenuCascade.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeFont.h"
#include "../theme/igdeGuiTheme.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Actions
////////////

class igdeNVBoardActionDeleteLink : public igdeAction {
	igdeNVBoard &pBoard;
	igdeNVLink *pLink;
	
public:
	typedef deTObjectReference<igdeNVBoardActionDeleteLink> Ref;
	
	igdeNVBoardActionDeleteLink(igdeNVBoard &board, igdeNVLink *link) : igdeAction("Delete Link",
		board.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus), "Delete link"),
	pBoard(board), pLink(link){
		if(!link){
			DETHROW(deeInvalidParam);
		}
	}
	
	void OnAction() override{
		const igdeNVSlot::Ref source(pLink->GetSource());
		const igdeNVSlot::Ref target(pLink->GetTarget());
		pBoard.RemoveLink(pLink);
		pBoard.NotifyLinkRemoved(source, target);
	}
};

class igdeNVBoardActionClearLinks : public igdeAction {
	igdeNVBoard &pBoard;
	
public:
	typedef deTObjectReference<igdeNVBoardActionClearLinks> Ref;
	
	igdeNVBoardActionClearLinks(igdeNVBoard &board) : igdeAction("Clear Links",
		board.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus), "Delete all links"),
	pBoard(board){
	}
	
	void OnAction() override{
		pBoard.RemoveAllLinks();
		pBoard.NotifyAllLinksRemoved();
	}
	
	void Update() override{
		SetEnabled(pBoard.GetLinks().IsNotEmpty());
	}
};



// Class igdeNVBoard
//////////////////////

// Constructor, destructor
////////////////////////////

igdeNVBoard::igdeNVBoard(igdeEnvironment &environment) :
igdeContainer(environment),
pBgColor(115.0f / 255.0f, 115.0f / 255.0f, 115.0f / 255.0f),
pEnabled(true){
}

igdeNVBoard::~igdeNVBoard(){
	DestroyNativeWidget();
	RemoveAllLinks();
	RemoveAllNodes();
}



// Management
///////////////

void igdeNVBoard::SetBgColor(const decColor &color){
	if(color.IsEqualTo(pBgColor)){
		return;
	}
	
	pBgColor = color;
	OnColorsChanged();
}

void igdeNVBoard::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}



decPoint igdeNVBoard::GetSize() const{
	if(!GetNativeWidget()){
		return decPoint();
	}
	
	return ((igdeNativeNVBoard*)GetNativeWidget())->GetSize();
}

void igdeNVBoard::SetOffset(const decPoint &offset){
	if(offset == pOffset){
		return;
	}
	
	pOffset = offset;
	
	NotifyNodesOffsetChanged();
	OnOffsetChanged();
}



void igdeNVBoard::Clear(){
	RemoveAllLinks();
	RemoveAllNodes();
}


void igdeNVBoard::AddNode(igdeNVNode *node){
	DEASSERT_NOTNULL(node);
	DEASSERT_FALSE(pNodes.Has(node));
	DEASSERT_NULL(node->GetOwnerBoard());
	
	igdeContainer::AddChild(node);
	pNodes.Add(node);
	node->SetOwnerBoard(this);
	OnNodesChanged();
	
	SetActiveNode(node);
}

void igdeNVBoard::RemoveNode(igdeNVNode *node){
	DEASSERT_NOTNULL(node);
	DEASSERT_TRUE(pNodes.Has(node));
	
	if(GetActiveNode() == node){
		SetActiveNode(nullptr);
	}
	
	RemoveAllNodeLinks(node);
	
	igdeContainer::RemoveChild(node);
	node->SetOwnerBoard(nullptr);
	pNodes.Remove(node);
	OnNodesChanged();
}

void igdeNVBoard::RemoveAllNodes(){
	if(pNodes.IsEmpty()){
		return;
	}
	
	SetActiveNode(nullptr);
	
	RemoveAllLinks();
	
	pNodes.VisitReverse([&](igdeNVNode &node){
		node.SetOwnerBoard(nullptr);
		igdeContainer::RemoveChild(&node);
	});
	pNodes.RemoveAll();
	OnNodesChanged();
}

void igdeNVBoard::SetActiveNode(igdeNVNode *node){
	igdeNVNode * const prevNode = GetActiveNode();
	if(prevNode == node){
		return;
	}
	
	if(prevNode){
		prevNode->SetActive(false);
		prevNode->NotifyDeactivated();
	}
	
	pActiveNode = node;
	
	if(node){
		node->SetActive(true);
		node->NotifyActivated();
	}
}



igdeNVLink *igdeNVBoard::GetLinkBetween(igdeNVSlot *source, igdeNVSlot *target) const{
	DEASSERT_NOTNULL(source);
	DEASSERT_NOTNULL(target);
	
	igdeNVLink *result = nullptr;
	pLinks.HasMatching([&](igdeNVLink *l){
		if(l->GetSource() == source && l->GetTarget() == target){
			result = l;
			return true;
		}
		return false;
	});
	return result;
}

bool igdeNVBoard::HasLinkBetween(igdeNVSlot *source, igdeNVSlot *target) const{
	return GetLinkBetween(source, target) != nullptr;
}

bool igdeNVBoard::CanLink(igdeNVSlot *source, igdeNVSlot *target){
	if(!source || !target){
		DETHROW(deeInvalidParam);
	}
	
	if(source->GetIsInput() || !target->GetIsInput()
	|| source->GetOwnerNode() == target->GetOwnerNode()
	|| HasLinkBetween(source, target)
	|| !pNodes.Has(source->GetOwnerNode())
	|| !pNodes.Has(target->GetOwnerNode())){
		return false;
	}
	
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(!((igdeNVBoardListener*)pListeners.GetAt(i))->CanLink(this, source, target)){
			return false;
		}
	}
	
	return true;
}

igdeNVLink::Ref igdeNVBoard::AddLink(igdeNVSlot *source, igdeNVSlot *target){
	if(!CanLink(source, target)){
		DETHROW(deeInvalidParam);
	}
	
	const igdeNVLink::Ref link(igdeNVLink::Ref::New(source, target));
	link->SetOwnerBoard(this);
	pLinks.Add((igdeNVLink*)link);
	
	source->AddLink(link);
	target->AddLink(link);
	
	OnLinksChanged();
	
	return link;
}

void igdeNVBoard::RemoveLink(igdeNVLink *link){
	DEASSERT_NOTNULL(link);
	DEASSERT_TRUE(pLinks.Has(link));
	
	link->GetTarget()->RemoveLink(link);
	link->GetSource()->RemoveLink(link);
	
	link->SetOwnerBoard(nullptr);
	pLinks.Remove(link);
	OnLinksChanged();
}

void igdeNVBoard::RemoveAllLinks(){
	if(pLinks.GetCount() == 0){
		return;
	}
	
	pLinks.Visit([&](igdeNVLink &link){
		link.SetOwnerBoard(nullptr);
		link.GetSource()->RemoveLink(&link);
		link.GetTarget()->RemoveLink(&link);
	});
	pLinks.RemoveAll();
	OnLinksChanged();
}

void igdeNVBoard::RemoveAllNodeLinks(igdeNVNode *node){
	DEASSERT_NOTNULL(node)
	
	pLinks.RemoveIf([&](const igdeNVLink &l){
		return l.GetSource()->GetOwnerNode() == node || l.GetTarget()->GetOwnerNode() == node;
	});
}

igdeNVLink *igdeNVBoard::ClosestLinkNear(const decPoint &position, float range) const{
	if(!GetNativeWidget()){
		return nullptr;
	}
	
	return ((igdeNativeNVBoard*)GetNativeWidget())->ClosestLinkNear(position, range);
}

void igdeNVBoard::ShowContextMenu(const decPoint &position){
	if(!GetNativeWidget()){
		return;
	}
	
	const igdeNativeNVBoard &native = *((igdeNativeNVBoard*)GetNativeWidget());
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New(helper.GetEnvironment()));
	
	// link
	igdeNVLink * const link = native.GetHoverLink();
	if(link){
		helper.MenuCommand(menu, igdeNVBoardActionDeleteLink::Ref::New(*this, link));
	}
	helper.MenuCommand(menu, igdeNVBoardActionClearLinks::Ref::New(*this));
	
	// listener provided
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((igdeNVBoardListener*)pListeners.GetAt(i))->AddContextMenuEntries(this, menu, position);
	}
	
	if(menu->GetChildren().IsNotEmpty()){
		menu->Popup(*this, position);
	}
}



void igdeNVBoard::AddListener(igdeNVBoardListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeNVBoard::RemoveListener(igdeNVBoardListener *listener){
	pListeners.Remove(listener);
}
void igdeNVBoard::NotifyLinkAdded(igdeNVLink *link){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVBoardListener &l){
		l.OnLinkAdded(this, link);
	});
}

void igdeNVBoard::NotifyLinkRemoved(igdeNVSlot *source, igdeNVSlot *target){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVBoardListener &l){
		l.OnLinkRemoved(this, source, target);
	});
}

void igdeNVBoard::NotifyAllLinksRemoved(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVBoardListener &l){
		l.OnAllLinksRemoved(this);
	});
}

void igdeNVBoard::NotifyOffsetChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVBoardListener &l){
		l.OnOffsetChanged(this);
	});
}

void igdeNVBoard::NotifySizeChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVBoardListener &l){
		l.OnSizeChanged(this);
	});
}



void igdeNVBoard::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeNVBoard * const native = igdeNativeNVBoard::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
}

void igdeNVBoard::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVBoard*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeNVBoard::NotifyNodesOffsetChanged(){
	const int count = pNodes.GetCount();
	int i;
	for(i=0; i<count; i++){
		((igdeNVNode*)pNodes.GetAt(i))->OnBoardOffsetChanged();
	}
}

void igdeNVBoard::OnColorsChanged(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVBoard*)GetNativeWidget())->UpdateColors();
}

void igdeNVBoard::OnEnabledChanged(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVBoard*)GetNativeWidget())->UpdateEnabled();
}

void igdeNVBoard::OnOffsetChanged(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVBoard*)GetNativeWidget())->UpdateOffset();
}

void igdeNVBoard::OnNodesChanged(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVBoard*)GetNativeWidget())->UpdateNodes();
}


void igdeNVBoard::OnLinksChanged(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVBoard*)GetNativeWidget())->UpdateLinks();
}
