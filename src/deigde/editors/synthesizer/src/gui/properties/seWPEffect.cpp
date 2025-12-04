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
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
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
	cListEffects( seWPEffect &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		seSource * const source = pPanel.GetSource();
		if( ! source ){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if( selection ){
			source->SetActiveEffect( selection ? ( seEffect* )selection->GetData() : NULL );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, pPanel.GetActionEffectCopy() );
		helper.MenuCommand( menu, pPanel.GetActionEffectCut() );
		helper.MenuCommand( menu, pPanel.GetActionEffectPasteAdd() );
		helper.MenuCommand( menu, pPanel.GetActionEffectPasteInsert() );
		
		const seWindowMain &windowMain = pPanel.GetViewSynthesizer().GetWindowMain();
		igdeMenuCascadeReference submenu;
		submenu.TakeOver( new igdeMenuCascade( menu.GetEnvironment(), "Add" ) );
		helper.MenuCommand( submenu, windowMain.GetActionEffectAddStretch() );
		menu.AddChild( submenu );
		
		submenu.TakeOver( new igdeMenuCascade( menu.GetEnvironment(), "Insert" ) );
		helper.MenuCommand( submenu, windowMain.GetActionEffectInsertStretch() );
		menu.AddChild( submenu );
		
		helper.MenuCommand( menu, windowMain.GetActionEffectRemove() );
		helper.MenuCommand( menu, windowMain.GetActionEffectUp() );
		helper.MenuCommand( menu, windowMain.GetActionEffectDown() );
	}
};


class cActionEffectCopy : public igdeAction{
	seWPEffect &pPanel;
	
public:
	cActionEffectCopy( seWPEffect &panel ) : igdeAction( "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy effect to clipboard" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		seEffect * const effect = pPanel.GetEffect();
		if( ! effect ){
			return;
		}
		
		igdeClipboardData::Ref cdata;
		cdata.TakeOver( new seClipboardDataEffect( effect ) );
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set( cdata );
	}
	
	virtual void Update(){
		SetSelected( pPanel.GetEffect() );
	}
};

class cActionEffectCut : public igdeAction{
	seWPEffect &pPanel;
	
public:
	cActionEffectCut( seWPEffect &panel ) : igdeAction( "Cut",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
		"Cut effect to clipboard" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		seEffect * const effect = pPanel.GetEffect();
		if( ! effect ){
			return;
		}
		
		igdeClipboardData::Ref cdata;
		cdata.TakeOver( new seClipboardDataEffect( effect ) );
		pPanel.GetViewSynthesizer().GetWindowMain().GetClipboard().Set( cdata );
		
		igdeUndo::Ref undo;
		undo.TakeOver( new seUSourceRemoveEffect( pPanel.GetSource(), effect ) );
		pPanel.GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetSelected( pPanel.GetEffect() );
	}
};

class cActionEffectPasteAdd : public igdeAction{
protected:
	seWPEffect &pPanel;
	
public:
	cActionEffectPasteAdd( seWPEffect &panel ) : igdeAction( "Paste",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste effect from clipboard" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetSource() ){
			return;
		}
		
		seClipboardDataEffect * const cdata = ( seClipboardDataEffect* )pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName( seClipboardDataEffect::TYPE_NAME );
		if( ! cdata ){
			return;
		}
		
		seEffect * const effect = pPanel.GetEffect();
		const seEffectList &list = pPanel.GetSource()->GetEffects();
		const int index = effect ? list.IndexOf( effect ) : list.GetCount();
		
		igdeUndo::Ref undo;
		undo.TakeOver( new seUSourcePasteEffect( pPanel.GetSource(), cdata->GetEffects(), index ) );
		pPanel.GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetSelected( pPanel.GetSource() && pPanel.GetViewSynthesizer().GetWindowMain()
			.GetClipboard().HasWithTypeName( seClipboardDataEffect::TYPE_NAME )  );
	}
};

class cActionEffectPasteInsert : public cActionEffectPasteAdd{
public:
	cActionEffectPasteInsert( seWPEffect &panel ) : cActionEffectPasteAdd( panel ){
		SetText( "Paste Append" );
		SetDescription( "Paste effect from clipboard" );
	}
	
	virtual void OnAction(){
		if( ! pPanel.GetSource() ){
			return;
		}
		
		seClipboardDataEffect * const cdata = ( seClipboardDataEffect* )pPanel.GetViewSynthesizer()
			.GetWindowMain().GetClipboard().GetWithTypeName( seClipboardDataEffect::TYPE_NAME );
		if( ! cdata ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new seUSourcePasteEffect( pPanel.GetSource(), cdata->GetEffects(),
			pPanel.GetSource()->GetEffects().GetCount() ) );
		pPanel.GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
};

}



// Class seWPEffect
/////////////////////

// Constructor, destructor
////////////////////////////

seWPEffect::seWPEffect( seViewSynthesizer &viewSynthesizer ) :
igdeContainerFlow( viewSynthesizer.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast ),
pViewSynthesizer( viewSynthesizer ),
pListener( NULL ),
pSynthesizer( NULL ),
pPanelStretch( NULL ),
pActivePanel( NULL )
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	pListener = new seWPEffectListener( *this );
	
	
	pActionEffectCopy.TakeOver( new cActionEffectCopy( *this ) );
	pActionEffectCut.TakeOver( new cActionEffectCut( *this ) );
	pActionEffectPasteAdd.TakeOver( new cActionEffectPasteAdd( *this ) );
	pActionEffectPasteInsert.TakeOver( new cActionEffectPasteInsert( *this ) );
	
	
	helper.GroupBoxFlow( *this, groupBox, "Effects:" );
	helper.ListBox( groupBox, 3, "Effects", pListEffect, new cListEffects( *this ) );
	
	
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	AddChild( pSwitcher );
	
	igdeWidget::Ref panel;
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelStretch = new seWPAPanelEffectStretch( *this ) );
	pSwitcher->AddChild( panel );
	
	pSwitcher->SetCurrent( epEmpty );
}

seWPEffect::~seWPEffect(){
	SetSynthesizer( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPEffect::SetSynthesizer( seSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	if( pSynthesizer ){
		pSynthesizer->RemoveNotifier( pListener );
		pSynthesizer->FreeReference();
	}
	
	pSynthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddNotifier( pListener );
		synthesizer->AddReference();
	}
	
	UpdateEffectList();
	UpdateEffect();
	UpdateLinkList();
	UpdateControllerList();
}

seSource *seWPEffect::GetSource() const{
	return pSynthesizer ? pSynthesizer->GetActiveSource() : NULL;
}

seEffect *seWPEffect::GetEffect() const{
	seSource * const source = GetSource();
	return source ? source->GetActiveEffect() : NULL;
}



void seWPEffect::UpdateControllerList(){
	if( pActivePanel ){
		pActivePanel->UpdateControllerList();
	}
}

void seWPEffect::UpdateLinkList(){
	if( pActivePanel ){
		pActivePanel->UpdateLinkList();
	}
}

void seWPEffect::UpdateEffectList(){
	const seSource * const source = GetSource();
	seEffect * const selection = GetEffect();
	
	pListEffect->RemoveAllItems();
	
	if( source ){
		const seEffectList &list = source->GetEffects();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			seEffect * const effect = list.GetAt( i );
			
			switch( effect->GetType() ){
			case deSynthesizerEffectVisitorIdentify::eetStretch:
				pListEffect->AddItem( "Time/Pitch Stretch", NULL, effect );
				break;
				
			default:
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	pListEffect->SetSelectionWithData( selection );
	ShowActiveEffectPanel();
	UpdateEffect();
}

void seWPEffect::SelectActiveEffect(){
	pListEffect->SetSelectionWithData( GetEffect() );
}

void seWPEffect::ShowActiveEffectPanel(){
	const seEffect * const effect = GetEffect();
	if( ! effect ){
		pSwitcher->SetCurrent( epEmpty );
		return;
	}
	
	const deSynthesizerEffectVisitorIdentify::eEffectTypes type = effect->GetType();
	
	if( type == pPanelStretch->GetRequiredType() ){
		pSwitcher->SetCurrent( epStretch );
		pActivePanel = pPanelStretch;
		
	}else{
		pSwitcher->SetCurrent( epEmpty );
		pActivePanel = NULL;
	}
	
	if( pActivePanel ){
		pActivePanel->OnActivated();
	}
}

void seWPEffect::UpdateEffect(){
	if( pActivePanel ){
		pActivePanel->UpdateEffect();
	}
}
