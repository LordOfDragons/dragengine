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

#include "syneWPSource.h"
#include "syneWPSourceListener.h"
#include "panels/source/syneWPAPanelSource.h"
#include "panels/source/syneWPAPanelSourceSound.h"
#include "panels/source/syneWPAPanelSourceWave.h"
#include "panels/source/syneWPAPanelSourceChain.h"
#include "panels/source/syneWPAPanelSourceGroup.h"
#include "panels/source/syneWPAPanelSourceSynthesizer.h"
#include "../syneWindowMain.h"
#include "../syneViewSynthesizer.h"
#include "../../clipboard/syneClipboardDataSource.h"
#include "../../synthesizer/syneSynthesizer.h"
#include "../../synthesizer/source/syneSource.h"
#include "../../synthesizer/source/syneSourceGroup.h"
#include "../../undosys/source/syneUPasteSource.h"
#include "../../undosys/source/syneURemoveSource.h"
#include "../../undosys/source/group/syneUSourceGroupPasteSource.h"
#include "../../undosys/source/group/syneUSourceGroupRemoveSource.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum ePanels{
	epEmpty,
	epSound,
	epWave,
	epChain,
	epGroup,
	epSynthesizer
};



// Actions
////////////

namespace{

class cTreeSources : public igdeTreeListListener{
	syneWPSource &pPanel;
	
public:
	typedef deTObjectReference<cTreeSources> Ref;
	cTreeSources(syneWPSource &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList *treeList){
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		const igdeTreeItem * const selection = treeList->GetSelection();
		if(selection && !selection->GetData()){
			// while updating the tree items do have nullptr data until they a source is properly
			// assigned to them. ignore update in this situation
			return;
		}
		
		synthesizer->SetActiveSource(selection ? (syneSource*)selection->GetData() : nullptr);
	}
	
	virtual void OnItemExpanded(igdeTreeList*, igdeTreeItem *item){
		syneSource * const source = (syneSource*)item->GetData();
		if(!source){
			return;
		}
		
		if(source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
			((syneSourceGroup*)source)->SetTreeListExpanded(true);
		}
	}
	
	virtual void OnItemCollapsed(igdeTreeList*, igdeTreeItem *item){
		syneSource * const source = (syneSource*)item->GetData();
		if(!source){
			return;
		}
		
		if(source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
			((syneSourceGroup*)source)->SetTreeListExpanded(false);
		}
	}
	
	virtual void AddContextMenuEntries(igdeTreeList*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		const syneWindowMain &windowMain = pPanel.GetViewSynthesizer().GetWindowMain();
		igdeMenuCascade::Ref submenu(igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Add"));
		helper.MenuCommand(submenu, windowMain.GetActionSourceAddWave());
		helper.MenuCommand(submenu, windowMain.GetActionSourceAddSound());
		helper.MenuCommand(submenu, windowMain.GetActionSourceAddChain());
		helper.MenuCommand(submenu, windowMain.GetActionSourceAddGroup());
		helper.MenuCommand(submenu, windowMain.GetActionSourceAddSynthesizer());
		menu.AddChild(submenu);
		
		submenu = igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Add Into Group");
		helper.MenuCommand(submenu, windowMain.GetActionSourceGroupAddWave());
		helper.MenuCommand(submenu, windowMain.GetActionSourceGroupAddSound());
		helper.MenuCommand(submenu, windowMain.GetActionSourceGroupAddChain());
		helper.MenuCommand(submenu, windowMain.GetActionSourceGroupAddGroup());
		helper.MenuCommand(submenu, windowMain.GetActionSourceGroupAddSynthesizer());
		menu.AddChild(submenu);
		
		submenu = igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Insert");
		helper.MenuCommand(submenu, windowMain.GetActionSourceInsertWave());
		helper.MenuCommand(submenu, windowMain.GetActionSourceInsertSound());
		helper.MenuCommand(submenu, windowMain.GetActionSourceInsertChain());
		helper.MenuCommand(submenu, windowMain.GetActionSourceInsertGroup());
		helper.MenuCommand(submenu, windowMain.GetActionSourceInsertSynthesizer());
		menu.AddChild(submenu);
		
		helper.MenuCommand(menu, windowMain.GetActionSourceRemove());
		helper.MenuCommand(menu, windowMain.GetActionSourceUp());
		helper.MenuCommand(menu, windowMain.GetActionSourceDown());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionSourceCopy());
		helper.MenuCommand(menu, pPanel.GetActionSourceCut());
		helper.MenuCommand(menu, pPanel.GetActionSourcePaste());
		helper.MenuCommand(menu, pPanel.GetActionSourcePasteIntoGroup());
	}
};


class cActionSourceCopy : public igdeAction{
	syneWPSource &pPanel;
	
public:
	typedef deTObjectReference<cActionSourceCopy> Ref;
	cActionSourceCopy(syneWPSource &panel) : igdeAction("Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy source to clipboard"), pPanel(panel){}
	
	void OnAction() override{
		syneSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set(
			syneClipboardDataSource::Ref::New(source));
	}
	
	void Update() override{
		SetSelected(pPanel.GetSource());
	}
};

class cActionSourceCut : public igdeAction{
	syneWPSource &pPanel;
	
public:
	typedef deTObjectReference<cActionSourceCut> Ref;
	cActionSourceCut(syneWPSource &panel) : igdeAction("Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut source to clipboard"), pPanel(panel){}
	
	void OnAction() override{
		syneSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set(
			syneClipboardDataSource::Ref::New(source));
		
		igdeUndo::Ref undo;
		if(source->GetParentGroup()){
			undo = syneUSourceGroupRemoveSource::Ref::New(source->GetParentGroup(), source);
			
		}else{
			undo = syneURemoveSource::Ref::New(pPanel.GetSynthesizer(), source);
		}
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetSelected(pPanel.GetSource());
	}
};

class cActionSourcePaste : public igdeAction{
protected:
	syneWPSource &pPanel;
	
public:
	typedef deTObjectReference<cActionSourcePaste> Ref;
	cActionSourcePaste(syneWPSource &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste source from clipboard"), pPanel(panel){}
	
	void OnAction() override{
		if(!pPanel.GetSynthesizer()){
			return;
		}
		
		syneClipboardDataSource * const cdata = (syneClipboardDataSource*)pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName(syneClipboardDataSource::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		syneSource * const source = pPanel.GetSource();
		syneSourceGroup * const group = source ? source->GetParentGroup() : nullptr;
		igdeUndo::Ref undo;
		
		if(group){
			undo = syneUSourceGroupPasteSource::Ref::New(group, cdata->GetSources(),
				group->GetSources().IndexOf(source));
			
		}else{
			const syneSource::List &list = pPanel.GetSynthesizer()->GetSources();
			const int index = source ? list.IndexOf(source) : list.GetCount();
			undo = syneUPasteSource::Ref::New(pPanel.GetSynthesizer(), cdata->GetSources(), index);
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetSelected(pPanel.GetSynthesizer() && pPanel.GetViewSynthesizer().GetWindowMain()
			.GetClipboard().HasWithTypeName(syneClipboardDataSource::TYPE_NAME));
	}
};

class cActionSourcePasteIntoGroup : public cActionSourcePaste{
public:
	typedef deTObjectReference<cActionSourcePasteIntoGroup> Ref;
	cActionSourcePasteIntoGroup(syneWPSource &panel) : cActionSourcePaste(panel){
		SetText("Paste Into Group");
		SetDescription("Paste source from clipboard into group");
	}
	
	void OnAction() override{
		syneSource * const source = pPanel.GetSource();
		if(!source || source->GetType() != deSynthesizerSourceVisitorIdentify::estGroup){
			return;
		}
		
		syneClipboardDataSource * const cdata = (syneClipboardDataSource*)pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName(syneClipboardDataSource::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		igdeUndo::Ref undo;
		syneSourceGroup * const group = (syneSourceGroup*)source;
		undo = syneUSourceGroupPasteSource::Ref::New(group, cdata->GetSources(), group->GetSources().GetCount());
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		const syneSource * const source = pPanel.GetSource();
		SetSelected(source && source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup
			&& pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().HasWithTypeName(
				syneClipboardDataSource::TYPE_NAME));
	}
};

}



// Class syneWPSource
////////////////////

// Constructor, destructor
////////////////////////////

syneWPSource::syneWPSource(syneViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer),
pPanelSound(nullptr),
pPanelWave(nullptr),
pPanelChain(nullptr),
pPanelGroup(nullptr),
pPanelSynthesizer(nullptr),
pActivePanel(nullptr)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox;
	
	pListener = syneWPSourceListener::Ref::New(*this);
	
	
	pActionSourceCopy = cActionSourceCopy::Ref::New(*this);
	pActionSourceCut = cActionSourceCut::Ref::New(*this);
	pActionSourcePaste = cActionSourcePaste::Ref::New(*this);
	pActionSourcePasteIntoGroup = cActionSourcePasteIntoGroup::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esLast);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Sources:");
	helper.TreeList(groupBox, pTreeSource, 10, "Sources", cTreeSources::Ref::New(*this));
	
	
	pSwitcher = igdeSwitcher::Ref::New(env);
	content->AddChild(pSwitcher);
	
	pSwitcher->AddChild(igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY));
	
	pPanelSound = syneWPAPanelSourceSound::Ref::New(*this);
	pSwitcher->AddChild(pPanelSound);
	
	pPanelWave = syneWPAPanelSourceWave::Ref::New(*this);
	pSwitcher->AddChild(pPanelWave);
	
	pPanelChain = syneWPAPanelSourceChain::Ref::New(*this);
	pSwitcher->AddChild(pPanelChain);
	
	pPanelGroup = syneWPAPanelSourceGroup::Ref::New(*this);
	pSwitcher->AddChild(pPanelGroup);
	
	pPanelSynthesizer = syneWPAPanelSourceSynthesizer::Ref::New(*this);
	pSwitcher->AddChild(pPanelSynthesizer);
	
	pSwitcher->SetCurrent(epEmpty);
}

syneWPSource::~syneWPSource(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

void syneWPSource::SetSynthesizer(syneSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	pPanelChain->SetSynthesizer(nullptr);
	pPanelGroup->SetSynthesizer(nullptr);
	pPanelSound->SetSynthesizer(nullptr);
	pPanelSynthesizer->SetSynthesizer(nullptr);
	pPanelWave->SetSynthesizer(nullptr);
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddNotifier(pListener);
		pPanelChain->SetSynthesizer(synthesizer);
		pPanelGroup->SetSynthesizer(synthesizer);
		pPanelSound->SetSynthesizer(synthesizer);
		pPanelSynthesizer->SetSynthesizer(synthesizer);
		pPanelWave->SetSynthesizer(synthesizer);
	}
	
	UpdateSynthesizer();
	UpdateSourceTree();
	UpdateSource();
	UpdateLinkList();
	UpdateControllerList();
	OnSynthesizerPathChanged();
}

syneSource *syneWPSource::GetSource() const{
	return pSynthesizer ? pSynthesizer->GetActiveSource() : nullptr;
}

void syneWPSource::OnSynthesizerPathChanged(){
	pPanelChain->OnSynthesizerPathChanged();
	pPanelSound->OnSynthesizerPathChanged();
	pPanelSynthesizer->OnSynthesizerPathChanged();
}



void syneWPSource::UpdateSynthesizer(){
	if(pActivePanel){
		pActivePanel->UpdateSynthesizer();
	}
}

void syneWPSource::UpdateControllerList(){
	if(pActivePanel){
		pActivePanel->UpdateControllerList();
	}
}

void syneWPSource::UpdateLinkList(){
	if(pActivePanel){
		pActivePanel->UpdateLinkList();
	}
}

void syneWPSource::UpdateSourceTree(){
	if(pSynthesizer){
		igdeTreeItem *nextItem = pTreeSource->GetFirstChild();
		
		pSynthesizer->GetSources().Visit([&](syneSource *source){
			if(!nextItem){
				const igdeTreeItem::Ref item(igdeTreeItem::Ref::New(""));
				pTreeSource->AppendItem(nullptr, item);
				nextItem = item;
			}
			
			UpdateSourceTreeItem(nextItem, source);
			
			nextItem = nextItem->GetNext();
		});
		
		while(nextItem){
			igdeTreeItem * const removeItem = nextItem;
			nextItem = nextItem->GetNext();
			pTreeSource->RemoveItem(removeItem);
		}
		
	}else{
		pTreeSource->RemoveAllItems();
	}
	
	SelectActiveSource();
	if(!pTreeSource->GetSelection() && pTreeSource->GetFirstChild()){
		pTreeSource->SetSelection(pTreeSource->GetFirstChild());
	}
}

void syneWPSource::UpdateSourceTreeItem(igdeTreeItem *item, syneSource *source){
	if(!item || !source){
		DETHROW(deeInvalidParam);
	}
	
	item->SetData(source);
	
	int order = 0;
	const igdeTreeItem *findOrderItem = item->GetPrevious();
	while(findOrderItem){
		order++;
		findOrderItem = findOrderItem->GetPrevious();
	}
	
	decString text;
	text.Format("%i: %s", order, source->GetName().GetString());
	item->SetText(text);
	item->SetIcon(pViewSynthesizer.GetWindowMain().GetSourceIcon(source->GetType()));
	pTreeSource->ItemChanged(item);
	
	if(source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
		const syneSourceGroup &sourceGroup = *((syneSourceGroup*)source);
		igdeTreeItem *nextItem = item->GetFirstChild();
		
		if(item->GetExpanded() != sourceGroup.GetTreeListExpanded()){
			item->SetExpanded(sourceGroup.GetTreeListExpanded());
			pTreeSource->ItemChanged(item);
		}
		
		sourceGroup.GetSources().Visit([&](syneSource *source2){
			if(!nextItem){
				const igdeTreeItem::Ref child(igdeTreeItem::Ref::New(""));
				pTreeSource->AppendItem(item, child);
				nextItem = child;
			}
			
			UpdateSourceTreeItem(nextItem, source2);
			
			nextItem = nextItem->GetNext();
		});
		
		while(nextItem){
			igdeTreeItem * const removeItem = nextItem;
			nextItem = nextItem->GetNext();
			pTreeSource->RemoveItem(removeItem);
		}
		
	}else{
		pTreeSource->RemoveAllItems(item);
	}
}

void syneWPSource::SelectActiveSource(){
	pTreeSource->SetSelectionWithData(GetSource());
}

void syneWPSource::ShowActiveSourcePanel(){
	const syneSource * const source = GetSource();
	if(!source){
		pSwitcher->SetCurrent(epEmpty);
		return;
	}
	
	const deSynthesizerSourceVisitorIdentify::eSourceTypes type = source->GetType();
	syneWPAPanelSource* const activePanel = pActivePanel;
	
	if(type == pPanelSound->GetRequiredType()){
		pSwitcher->SetCurrent(epSound);
		pActivePanel = pPanelSound;
		
	}else if(type == pPanelWave->GetRequiredType()){
		pSwitcher->SetCurrent(epWave);
		pActivePanel = pPanelWave;
		
	}else if(type == pPanelChain->GetRequiredType()){
		pSwitcher->SetCurrent(epChain);
		pActivePanel = pPanelChain;
		
	}else if(type == pPanelGroup->GetRequiredType()){
		pSwitcher->SetCurrent(epGroup);
		pActivePanel = pPanelGroup;
		
	}else if(type == pPanelSynthesizer->GetRequiredType()){
		pSwitcher->SetCurrent(epSynthesizer);
		pActivePanel = pPanelSynthesizer;
		
	}else{
		pSwitcher->SetCurrent(epEmpty);
		pActivePanel = nullptr;
	}
	
	if(pActivePanel != activePanel){
		if(activePanel){
			activePanel->OnDeactivated();
		}
		if(pActivePanel){
			pActivePanel->OnActivated();
		}
	}
}

void syneWPSource::UpdateSource(){
	if(!pActivePanel){
		return;
	}
	
	syneSource * const source = GetSource();
	igdeTreeItem * const item = pTreeSource->GetItemWithData(source);
	if(item){
		UpdateSourceTreeItem(item, source);
	}
	
	pActivePanel->UpdateSource();
}
