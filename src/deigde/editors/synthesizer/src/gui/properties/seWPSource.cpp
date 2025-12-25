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

#include "seWPSource.h"
#include "seWPSourceListener.h"
#include "panels/source/seWPAPanelSource.h"
#include "panels/source/seWPAPanelSourceSound.h"
#include "panels/source/seWPAPanelSourceWave.h"
#include "panels/source/seWPAPanelSourceChain.h"
#include "panels/source/seWPAPanelSourceGroup.h"
#include "panels/source/seWPAPanelSourceSynthesizer.h"
#include "../seWindowMain.h"
#include "../seViewSynthesizer.h"
#include "../../clipboard/seClipboardDataSource.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/source/seSourceGroup.h"
#include "../../undosys/source/seUPasteSource.h"
#include "../../undosys/source/seURemoveSource.h"
#include "../../undosys/source/group/seUSourceGroupPasteSource.h"
#include "../../undosys/source/group/seUSourceGroupRemoveSource.h"

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
	seWPSource &pPanel;
	
public:
	typedef deTObjectReference<cTreeSources> Ref;
	cTreeSources(seWPSource &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList *treeList){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		const igdeTreeItem * const selection = treeList->GetSelection();
		if(selection && !selection->GetData()){
			// while updating the tree items do have nullptr data until they a source is properly
			// assigned to them. ignore update in this situation
			return;
		}
		
		synthesizer->SetActiveSource(selection ? (seSource*)selection->GetData() : nullptr);
	}
	
	virtual void OnItemExpanded(igdeTreeList*, igdeTreeItem *item){
		seSource * const source = (seSource*)item->GetData();
		if(!source){
			return;
		}
		
		if(source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
			((seSourceGroup*)source)->SetTreeListExpanded(true);
		}
	}
	
	virtual void OnItemCollapsed(igdeTreeList*, igdeTreeItem *item){
		seSource * const source = (seSource*)item->GetData();
		if(!source){
			return;
		}
		
		if(source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
			((seSourceGroup*)source)->SetTreeListExpanded(false);
		}
	}
	
	virtual void AddContextMenuEntries(igdeTreeList*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		const seWindowMain &windowMain = pPanel.GetViewSynthesizer().GetWindowMain();
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
	seWPSource &pPanel;
	
public:
	typedef deTObjectReference<cActionSourceCopy> Ref;
	cActionSourceCopy(seWPSource &panel) : igdeAction("Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy source to clipboard"), pPanel(panel){}
	
	void OnAction() override{
		seSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set(
			seClipboardDataSource::Ref::New(source));
	}
	
	void Update() override{
		SetSelected(pPanel.GetSource());
	}
};

class cActionSourceCut : public igdeAction{
	seWPSource &pPanel;
	
public:
	typedef deTObjectReference<cActionSourceCut> Ref;
	cActionSourceCut(seWPSource &panel) : igdeAction("Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut source to clipboard"), pPanel(panel){}
	
	void OnAction() override{
		seSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set(
			seClipboardDataSource::Ref::New(source));
		
		igdeUndo::Ref undo;
		if(source->GetParentGroup()){
			undo = seUSourceGroupRemoveSource::Ref::New(source->GetParentGroup(), source);
			
		}else{
			undo = seURemoveSource::Ref::New(pPanel.GetSynthesizer(), source);
		}
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetSelected(pPanel.GetSource());
	}
};

class cActionSourcePaste : public igdeAction{
protected:
	seWPSource &pPanel;
	
public:
	typedef deTObjectReference<cActionSourcePaste> Ref;
	cActionSourcePaste(seWPSource &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste source from clipboard"), pPanel(panel){}
	
	void OnAction() override{
		if(!pPanel.GetSynthesizer()){
			return;
		}
		
		seClipboardDataSource * const cdata = (seClipboardDataSource*)pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName(seClipboardDataSource::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		seSource * const source = pPanel.GetSource();
		seSourceGroup * const group = source ? source->GetParentGroup() : nullptr;
		igdeUndo::Ref undo;
		
		if(group){
			undo = seUSourceGroupPasteSource::Ref::New(group, cdata->GetSources(),
				group->GetSources().IndexOf(source));
			
		}else{
			const seSource::List &list = pPanel.GetSynthesizer()->GetSources();
			const int index = source ? list.IndexOf(source) : list.GetCount();
			undo = seUPasteSource::Ref::New(pPanel.GetSynthesizer(), cdata->GetSources(), index);
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetSelected(pPanel.GetSynthesizer() && pPanel.GetViewSynthesizer().GetWindowMain()
			.GetClipboard().HasWithTypeName(seClipboardDataSource::TYPE_NAME));
	}
};

class cActionSourcePasteIntoGroup : public cActionSourcePaste{
public:
	typedef deTObjectReference<cActionSourcePasteIntoGroup> Ref;
	cActionSourcePasteIntoGroup(seWPSource &panel) : cActionSourcePaste(panel){
		SetText("Paste Into Group");
		SetDescription("Paste source from clipboard into group");
	}
	
	void OnAction() override{
		seSource * const source = pPanel.GetSource();
		if(!source || source->GetType() != deSynthesizerSourceVisitorIdentify::estGroup){
			return;
		}
		
		seClipboardDataSource * const cdata = (seClipboardDataSource*)pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName(seClipboardDataSource::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		igdeUndo::Ref undo;
		seSourceGroup * const group = (seSourceGroup*)source;
		undo = seUSourceGroupPasteSource::Ref::New(group, cdata->GetSources(), group->GetSources().GetCount());
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		const seSource * const source = pPanel.GetSource();
		SetSelected(source && source->GetType() == deSynthesizerSourceVisitorIdentify::estGroup
			&& pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().HasWithTypeName(
				seClipboardDataSource::TYPE_NAME));
	}
};

}



// Class seWPSource
////////////////////

// Constructor, destructor
////////////////////////////

seWPSource::seWPSource(seViewSynthesizer &viewSynthesizer) :
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
	
	pListener = seWPSourceListener::Ref::New(*this);
	
	
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
	
	pPanelSound = seWPAPanelSourceSound::Ref::New(*this);
	pSwitcher->AddChild(pPanelSound);
	
	pPanelWave = seWPAPanelSourceWave::Ref::New(*this);
	pSwitcher->AddChild(pPanelWave);
	
	pPanelChain = seWPAPanelSourceChain::Ref::New(*this);
	pSwitcher->AddChild(pPanelChain);
	
	pPanelGroup = seWPAPanelSourceGroup::Ref::New(*this);
	pSwitcher->AddChild(pPanelGroup);
	
	pPanelSynthesizer = seWPAPanelSourceSynthesizer::Ref::New(*this);
	pSwitcher->AddChild(pPanelSynthesizer);
	
	pSwitcher->SetCurrent(epEmpty);
}

seWPSource::~seWPSource(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

void seWPSource::SetSynthesizer(seSynthesizer *synthesizer){
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

seSource *seWPSource::GetSource() const{
	return pSynthesizer ? pSynthesizer->GetActiveSource() : nullptr;
}

void seWPSource::OnSynthesizerPathChanged(){
	pPanelChain->OnSynthesizerPathChanged();
	pPanelSound->OnSynthesizerPathChanged();
	pPanelSynthesizer->OnSynthesizerPathChanged();
}



void seWPSource::UpdateSynthesizer(){
	if(pActivePanel){
		pActivePanel->UpdateSynthesizer();
	}
}

void seWPSource::UpdateControllerList(){
	if(pActivePanel){
		pActivePanel->UpdateControllerList();
	}
}

void seWPSource::UpdateLinkList(){
	if(pActivePanel){
		pActivePanel->UpdateLinkList();
	}
}

void seWPSource::UpdateSourceTree(){
	int i;
	
	if(pSynthesizer){
		igdeTreeItem *nextItem = pTreeSource->GetFirstChild();
		
		pSynthesizer->GetSources().Visit([&](seSource *source){
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

void seWPSource::UpdateSourceTreeItem(igdeTreeItem *item, seSource *source){
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
		const seSourceGroup &sourceGroup = *((seSourceGroup*)source);
		igdeTreeItem *nextItem = item->GetFirstChild();
		
		if(item->GetExpanded() != sourceGroup.GetTreeListExpanded()){
			item->SetExpanded(sourceGroup.GetTreeListExpanded());
			pTreeSource->ItemChanged(item);
		}
		
		sourceGroup.GetSources().Visit([&](seSource *source2){
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

void seWPSource::SelectActiveSource(){
	pTreeSource->SetSelectionWithData(GetSource());
}

void seWPSource::ShowActiveSourcePanel(){
	const seSource * const source = GetSource();
	if(!source){
		pSwitcher->SetCurrent(epEmpty);
		return;
	}
	
	const deSynthesizerSourceVisitorIdentify::eSourceTypes type = source->GetType();
	seWPAPanelSource* const activePanel = pActivePanel;
	
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

void seWPSource::UpdateSource(){
	if(!pActivePanel){
		return;
	}
	
	seSource * const source = GetSource();
	igdeTreeItem * const item = pTreeSource->GetItemWithData(source);
	if(item){
		UpdateSourceTreeItem(item, source);
	}
	
	pActivePanel->UpdateSource();
}
