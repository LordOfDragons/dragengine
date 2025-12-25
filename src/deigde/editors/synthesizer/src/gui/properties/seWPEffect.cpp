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

#include "seWPEffect.h"
#include "seWPEffectListener.h"
#include "panels/effect/seWPAPanelEffect.h"
#include "panels/effect/seWPAPanelEffectStretch.h"
#include "../seWindowMain.h"
#include "../seViewSynthesizer.h"
#include "../../clipboard/seClipboardDataEffect.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/effect/seEffect.h"
#include "../../synthesizer/effect/seEffectStretch.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../undosys/source/effect/seUSourcePasteEffect.h"
#include "../../undosys/source/effect/seUSourceRemoveEffect.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum ePanels{
	epEmpty,
	epStretch,
	EP_COUNT
};



// Actions
////////////

namespace{

class cListEffects : public igdeListBoxListener{
	seWPEffect &pPanel;
	
public:
	typedef deTObjectReference<cListEffects> Ref;
	cListEffects(seWPEffect &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if(selection){
			source->SetActiveEffect(selection ? (seEffect*)selection->GetData() : nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, pPanel.GetActionEffectCopy());
		helper.MenuCommand(menu, pPanel.GetActionEffectCut());
		helper.MenuCommand(menu, pPanel.GetActionEffectPasteAdd());
		helper.MenuCommand(menu, pPanel.GetActionEffectPasteInsert());
		
		const seWindowMain &windowMain = pPanel.GetViewSynthesizer().GetWindowMain();
		igdeMenuCascade::Ref submenu(igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Add"));
		helper.MenuCommand(submenu, windowMain.GetActionEffectAddStretch());
		menu.AddChild(submenu);
		
		submenu = igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Insert");
		helper.MenuCommand(submenu, windowMain.GetActionEffectInsertStretch());
		menu.AddChild(submenu);
		
		helper.MenuCommand(menu, windowMain.GetActionEffectRemove());
		helper.MenuCommand(menu, windowMain.GetActionEffectUp());
		helper.MenuCommand(menu, windowMain.GetActionEffectDown());
	}
};


class cActionEffectCopy : public igdeAction{
	seWPEffect &pPanel;

public:
	typedef deTObjectReference<cActionEffectCopy> Ref;
	cActionEffectCopy(seWPEffect &panel) : igdeAction("Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy effect to clipboard"), pPanel(panel){}
	
	void OnAction() override{
		seEffect * const effect = pPanel.GetEffect();
		if(!effect){
			return;
		}
		
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set(
			seClipboardDataEffect::Ref::New(effect));
	}
	
	void Update() override{
		SetSelected(pPanel.GetEffect());
	}
};

class cActionEffectCut : public igdeAction{
	seWPEffect &pPanel;
	
public:
	typedef deTObjectReference<cActionEffectCut> Ref;
	cActionEffectCut(seWPEffect &panel) : igdeAction("Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut effect to clipboard"), pPanel(panel){}
	
	void OnAction() override{
		seEffect * const effect = pPanel.GetEffect();
		if(!effect){
			return;
		}
		
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set(
			seClipboardDataEffect::Ref::New(effect));
		
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(
			seUSourceRemoveEffect::Ref::New(pPanel.GetSource(), effect));
	}
	
	void Update() override{
		SetSelected(pPanel.GetEffect());
	}
};

class cActionEffectPasteAdd : public igdeAction{
protected:
	seWPEffect &pPanel;
	
public:
	typedef deTObjectReference<cActionEffectPasteAdd> Ref;
	cActionEffectPasteAdd(seWPEffect &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste effect from clipboard"), pPanel(panel){}
	
	void OnAction() override{
		if(!pPanel.GetSource()){
			return;
		}
		
		seClipboardDataEffect * const cdata = (seClipboardDataEffect*)pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName(seClipboardDataEffect::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		seEffect * const effect = pPanel.GetEffect();
		const seEffect::List &list = pPanel.GetSource()->GetEffects();
		const int index = effect ? list.IndexOf(effect) : list.GetCount();
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(seUSourcePasteEffect::Ref::New(
			pPanel.GetSource(), cdata->GetEffects(), index));
	}
	
	void Update() override{
		SetSelected(pPanel.GetSource() && pPanel.GetViewSynthesizer().GetWindowMain()
			.GetClipboard().HasWithTypeName(seClipboardDataEffect::TYPE_NAME));
	}
};

class cActionEffectPasteInsert : public cActionEffectPasteAdd{
public:
	typedef deTObjectReference<cActionEffectPasteInsert> Ref;
	cActionEffectPasteInsert(seWPEffect &panel) : cActionEffectPasteAdd(panel){
		SetText("Paste Append");
		SetDescription("Paste effect from clipboard");
	}
	
	void OnAction() override{
		if(!pPanel.GetSource()){
			return;
		}
		
		seClipboardDataEffect * const cdata = (seClipboardDataEffect*)pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName(seClipboardDataEffect::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(seUSourcePasteEffect::Ref::New(
			pPanel.GetSource(), cdata->GetEffects(), pPanel.GetSource()->GetEffects().GetCount()));
	}
};

}



// Class seWPEffect
/////////////////////

// Constructor, destructor
////////////////////////////

seWPEffect::seWPEffect(seViewSynthesizer &viewSynthesizer) :
igdeContainerFlow(viewSynthesizer.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast),
pViewSynthesizer(viewSynthesizer),
pPanelStretch(nullptr),
pActivePanel(nullptr)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	pListener = seWPEffectListener::Ref::New(*this);
	
	
	pActionEffectCopy = cActionEffectCopy::Ref::New(*this);
	pActionEffectCut = cActionEffectCut::Ref::New(*this);
	pActionEffectPasteAdd = cActionEffectPasteAdd::Ref::New(*this);
	pActionEffectPasteInsert = cActionEffectPasteInsert::Ref::New(*this);
	
	
	helper.GroupBoxFlow(*this, groupBox, "Effects:");
	helper.ListBox(groupBox, 3, "Effects", pListEffect, cListEffects::Ref::New(*this));
	
	
	pSwitcher = igdeSwitcher::Ref::New(env);
	AddChild(pSwitcher);
	
	pSwitcher->AddChild(igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY));
	
	pPanelStretch = seWPAPanelEffectStretch::Ref::New(*this);
	pSwitcher->AddChild(pPanelStretch);
	
	pSwitcher->SetCurrent(epEmpty);
}

seWPEffect::~seWPEffect(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

void seWPEffect::SetSynthesizer(seSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddNotifier(pListener);
	}
	
	UpdateEffectList();
	UpdateEffect();
	UpdateLinkList();
	UpdateControllerList();
}

seSource *seWPEffect::GetSource() const{
	return pSynthesizer ? pSynthesizer->GetActiveSource() : nullptr;
}

seEffect *seWPEffect::GetEffect() const{
	seSource * const source = GetSource();
	return source ? source->GetActiveEffect() : nullptr;
}



void seWPEffect::UpdateControllerList(){
	if(pActivePanel){
		pActivePanel->UpdateControllerList();
	}
}

void seWPEffect::UpdateLinkList(){
	if(pActivePanel){
		pActivePanel->UpdateLinkList();
	}
}

void seWPEffect::UpdateEffectList(){
	const seSource * const source = GetSource();
	seEffect * const selection = GetEffect();
	
	pListEffect->RemoveAllItems();
	
	if(source){
		source->GetEffects().Visit([&](seEffect *effect){
			switch(effect->GetType()){
			case deSynthesizerEffectVisitorIdentify::eetStretch:
				pListEffect->AddItem("Time/Pitch Stretch", nullptr, effect);
				break;
				
			default:
				DETHROW(deeInvalidParam);
			}
		});
	}
	
	pListEffect->SetSelectionWithData(selection);
	ShowActiveEffectPanel();
	UpdateEffect();
}

void seWPEffect::SelectActiveEffect(){
	pListEffect->SetSelectionWithData(GetEffect());
}

void seWPEffect::ShowActiveEffectPanel(){
	const seEffect * const effect = GetEffect();
	if(!effect){
		pSwitcher->SetCurrent(epEmpty);
		return;
	}
	
	const deSynthesizerEffectVisitorIdentify::eEffectTypes type = effect->GetType();
	
	if(type == pPanelStretch->GetRequiredType()){
		pSwitcher->SetCurrent(epStretch);
		pActivePanel = pPanelStretch;
		
	}else{
		pSwitcher->SetCurrent(epEmpty);
		pActivePanel = nullptr;
	}
	
	if(pActivePanel){
		pActivePanel->OnActivated();
	}
}

void seWPEffect::UpdateEffect(){
	if(pActivePanel){
		pActivePanel->UpdateEffect();
	}
}
