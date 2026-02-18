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

#include "igdeNVNode.h"
#include "igdeNVNodeListener.h"
#include "igdeNVSlot.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeUIHelper.h"
#include "../event/igdeAction.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeFont.h"
#include "../theme/igdeGuiTheme.h"
#include "../menu/igdeMenuCascade.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeNVNode
/////////////////////

// Constructor, destructor
////////////////////////////

igdeNVNode::igdeNVNode(igdeEnvironment &environment, const char *title, const char *description) :
igdeContainer(environment),
pTitle(title),
pDescription(description),
pEnabled(true),
pActive(false),
pBgColor(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f),
pBorderColor(60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f),
pActiveTitleBgColor(165.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f),
pInactiveTitleBgColor(150.0f / 255.0f, 150.0f / 255.0f, 185.0f / 255.0f),
pOwnerBoard(nullptr),
pNativeNVNode(nullptr){
}

igdeNVNode::~igdeNVNode(){
	DestroyNativeWidget();
	RemoveAllSlots();
}



// Management
///////////////

void igdeNVNode::SetTitle(const char *title){
	if(pTitle == title){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeNVNode::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeNVNode::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeNVNode::SetActive(bool active){
	if(pActive == active){
		return;
	}
	
	pActive = active;
	OnActiveChanged();
}



void igdeNVNode::SetBgColor(const decColor &color){
	if(color.IsEqualTo(pBgColor)){
		return;
	}
	
	pBgColor = color;
	OnColorsChanged();
}

void igdeNVNode::SetBorderColor(const decColor &color){
	if(color.IsEqualTo(pBorderColor)){
		return;
	}
	
	pBorderColor = color;
	OnColorsChanged();
}

void igdeNVNode::SetActiveTitleBgColor(const decColor &color){
	if(color.IsEqualTo(pActiveTitleBgColor)){
		return;
	}
	
	pActiveTitleBgColor = color;
	OnColorsChanged();
}

void igdeNVNode::SetInactiveTitleBgColor(const decColor &color){
	if(color.IsEqualTo(pInactiveTitleBgColor)){
		return;
	}
	
	pInactiveTitleBgColor = color;
	OnColorsChanged();
}



void igdeNVNode::SetPosition(const decPoint &position){
	if(position == pPosition){
		return;
	}
	
	pPosition = position;
	OnPositionChanged();
}

decPoint igdeNVNode::GetSize() const{
	return pNativeNVNode ? pNativeNVNode->GetSize() : decPoint();
}

void igdeNVNode::SetOwnerBoard(igdeNVBoard *board){
	if(board == pOwnerBoard){
		return;
	}
	
	pOwnerBoard = board;
	
	if(board){
		OnPositionChanged();
	}
}

void igdeNVNode::OnBoardOffsetChanged(){
	OnPositionChanged();
}



void igdeNVNode::AddSlot(igdeNVSlot *slot){
	DEASSERT_NOTNULL(slot)
	DEASSERT_FALSE(pSlots.Has(slot))
	
	igdeContainer::AddChild(slot);
	pSlots.Add(slot);
	slot->SetOwnerNode(this);
	OnSlotsChanged();
}

void igdeNVNode::RemoveSlot(igdeNVSlot *slot){
	DEASSERT_NOTNULL(slot)
	DEASSERT_TRUE(pSlots.Has(slot))
	
	igdeContainer::RemoveChild(slot);
	slot->SetOwnerNode(nullptr);
	pSlots.Remove(slot);
	OnSlotsChanged();
}

void igdeNVNode::RemoveAllSlots(){
	if(pSlots.GetCount() == 0){
		return;
	}
	
	pSlots.VisitReverse([&](igdeNVSlot &slot){
		slot.SetOwnerNode(nullptr);
		igdeContainer::RemoveChild(&slot);
	});
	pSlots.RemoveAll();
	
	OnSlotsChanged();
}

void igdeNVNode::ShowContextMenu(const decPoint &position){
	if(!pNativeNVNode){
		return;
	}
	
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New(helper.GetEnvironment()));
	
	pListeners.Visit([&](igdeNVNodeListener &l){
		l.AddContextMenuEntries(this, menu);
	});
	
	if(menu->GetChildren().IsNotEmpty()){
		menu->Popup(*this, position);
	}
}



void igdeNVNode::AddListener(igdeNVNodeListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void igdeNVNode::RemoveListener(igdeNVNodeListener *listener){
	pListeners.Remove(listener);
}

void igdeNVNode::NotifyActivated(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVNodeListener &l){
		l.OnActivated(this);
	});
}

void igdeNVNode::NotifyDeactivated(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVNodeListener &l){
		l.OnDeactivated(this);
	});
}

void igdeNVNode::NotifyDragBegin(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVNodeListener &l){
		l.OnDragBegin(this);
	});
}

void igdeNVNode::NotifyDraging(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVNodeListener &l){
		l.OnDraging(this);
	});
}

void igdeNVNode::NotifyDragEnd(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeNVNodeListener &l){
		l.OnDragEnd(this);
	});
}


void igdeNVNode::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeNVNode * const native = igdeNativeNVNode::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeNVNode = native;
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->FitSizeToContent();
}

void igdeNVNode::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeNVNode*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeNVNode::DropNativeWidget(){
	pNativeNVNode = nullptr;
	igdeContainer::DropNativeWidget();
}


void igdeNVNode::OnTitleChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdateTitle();
	}
}

void igdeNVNode::OnDescriptionChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdateDescription();
	}
}

void igdeNVNode::OnEnabledChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdateEnabled();
	}
}

void igdeNVNode::OnActiveChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdateActive();
	}
}

void igdeNVNode::OnColorsChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdateColors();
	}
}

void igdeNVNode::OnSlotsChanged(){
	if(pNativeNVNode){
		pNativeNVNode->FitSizeToContent();
	}
}

void igdeNVNode::OnPositionChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdatePosition();
	}
}

void igdeNVNode::OnNativeWidgetLanguageChanged(){
	if(pNativeNVNode){
		pNativeNVNode->UpdateTitle();
		pNativeNVNode->UpdateDescription();
		pNativeNVNode->FitSizeToContent();
	}
}
