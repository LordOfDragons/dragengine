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

#include "seWPLink.h"
#include "seWPLinkListener.h"
#include "seWindowProperties.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"
#include "../../sky/link/seLink.h"
#include "../../sky/layer/seLayer.h"
#include "../../undosys/link/seULinkAdd.h"
#include "../../undosys/link/seULinkRemove.h"
#include "../../undosys/link/seULinkSetName.h"
#include "../../undosys/link/seULinkSetCurve.h"
#include "../../undosys/link/seULinkSetController.h"
#include "../../undosys/link/seULinkSetRepeat.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/layout/igdeContainerBorderReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPLink &pPanel;
	
public:
	cBaseTextFieldListener( seWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSky * const sky = pPanel.GetSky();
		seLink * const link = pPanel.GetLink();
		if( ! sky || ! link ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, sky, link ) );
		if( undo ){
			sky->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSky *sky, seLink *link ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPLink &pPanel;
	
public:
	cBaseComboBoxListener( seWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSky * const sky = pPanel.GetSky();
		seLink * const link = pPanel.GetLink();
		if( ! sky || ! link ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, sky, link ) );
		if( undo ){
			sky->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, seSky *sky, seLink *link ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPLink &pPanel;
	
public:
	cBaseAction( seWPLink &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	cBaseAction( seWPLink &panel, igdeIcon *icon, const char *description ) :
	igdeAction( "", icon, description ),
	pPanel( panel ){ }
	
	cBaseAction( seWPLink &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		seSky * const sky = pPanel.GetSky();
		if( ! sky ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( sky ) );
		if( undo ){
			sky->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( seSky *sky ) = 0;
};

class cBaseActionLink : public cBaseAction{
public:
	cBaseActionLink( seWPLink &panel, const char *text, const char *description ) :
	cBaseAction( panel, text, description ){ }
	
	cBaseActionLink( seWPLink &panel, igdeIcon *icon, const char *description ) :
	cBaseAction( panel, icon, description ){ }
	
	cBaseActionLink( seWPLink &panel, const char *text, igdeIcon *icon, const char *description ) :
	cBaseAction( panel, text, icon, description ){ }
	
	virtual igdeUndo *OnAction( seSky *sky ){
		seLink * const link = pPanel.GetLink();
		if( link ){
			return OnActionLink( sky, link );
			
		}else{
			return NULL;
		}
	}
	
	virtual igdeUndo *OnActionLink( seSky *sky, seLink *link ) = 0;
};



class cListLinks : public igdeListBoxListener{
	seWPLink &pPanel;
public:
	cListLinks( seWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		seSky * const sky = pPanel.GetSky();
		if( ! sky ){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		
		if( selection ){
			sky->SetActiveLink( ( seLink* )selection->GetData() );
			
		}else{
			sky->SetActiveLink( NULL );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand( menu, pPanel.GetActionLinkAdd() );
		helper.MenuCommand( menu, pPanel.GetActionLinkRemove() );
	}
};

class cActionLinkAdd : public cBaseAction{
public:
	cActionLinkAdd( seWPLink &panel ) : cBaseAction( panel, "Add",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Add a link to the end of the list." ){ }
	
	virtual igdeUndo *OnAction( seSky *sky ){
		deObjectReference link;
		link.TakeOver( new seLink );
		return new seULinkAdd( sky, ( seLink* )( deObject* )link );
	}
};

class cActionLinkRemove : public cBaseActionLink{
public:
	cActionLinkRemove( seWPLink &panel ) : cBaseActionLink( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove the selected link." ){ }
	
	virtual igdeUndo *OnActionLink( seSky *sky, seLink *link ){
		const int usageCount = sky->CountLinkUsage( link );
		
		if( usageCount > 0 && igdeCommonDialogs::QuestionFormat(
			&pPanel, igdeCommonDialogs::ebsYesNo, "Remove Link",
			"The link '%s' is used by %i targets.\n"
			"If the link is removed now it is also removed from\n"
			"all the targets using it. Do you want to remove the link?",
			link->GetName().GetString(), usageCount ) != igdeCommonDialogs::ebYes ){
				return NULL;
		}
		
		return new seULinkRemove( link );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetLink() != NULL );
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	cTextName( seWPLink &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSky*, seLink *link ){
		const decString &name = textField->GetText();
		if( name == link->GetName() ){
			return NULL;
		}
		return new seULinkSetName( link, name );
	}
};

class cComboController : public cBaseComboBoxListener{
public:
	cComboController( seWPLink &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, seSky*, seLink *link ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		seController *controller = NULL;
		if( selection ){
			controller = ( seController* )selection->GetData();
		}
		
		if( controller == link->GetController() ){
			return NULL;
		}
		
		return new seULinkSetController( link, controller );
	}
};

class cTextRepeat : public cBaseTextFieldListener{
public:
	cTextRepeat( seWPLink &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSky*, seLink *link ){
		const int repeat = textField->GetInteger();
		if( repeat == link->GetRepeat() ){
			return NULL;
		}
		return new seULinkSetRepeat( link, repeat );
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
protected:
	seWPLink &pPanel;
	igdeUndoReference pUndo;
	
public:
	cEditCurve( seWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnCurveChanged( igdeViewCurveBezier *viewCurveBezier ){
		if( pUndo ){
			( ( seULinkSetCurve& )( igdeUndo& )pUndo ).SetNewCurve( viewCurveBezier->GetCurve() );
			
		}else if( ! pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve() ){
			return;
			
		}else{
			pUndo.TakeOver( new seULinkSetCurve( pPanel.GetLink(), viewCurveBezier->GetCurve() ) );
		}
		
		pPanel.GetSky()->GetUndoSystem()->Add( pUndo );
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging( igdeViewCurveBezier *viewCurveBezier ){
		if( pUndo ){
			( ( seULinkSetCurve& )( igdeUndo& )pUndo ).SetNewCurve( viewCurveBezier->GetCurve() );
			pUndo->Redo();
			
		}else if( pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve() ){
			pUndo.TakeOver( new seULinkSetCurve( pPanel.GetLink(), viewCurveBezier->GetCurve() ) );
		}
	}
};

}



// Class seWPLink
///////////////////

// Constructor, destructor
////////////////////////////

seWPLink::seWPLink( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pSky( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPLinkListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// link list
	helper.GroupBoxFlow( content, groupBox, "Links:" );
	
	helper.ListBox( groupBox, 8, "Links", pListLinks, new cListLinks( *this ) );
	
	pActionLinkAdd.TakeOver( new cActionLinkAdd( *this ) );
	pActionLinkRemove.TakeOver( new cActionLinkRemove( *this ) );
	
	// link settings
	helper.GroupBoxFlow( content, groupBox, "Link Settings:" );
	
	frameLine.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( frameLine );
	
	helper.EditString( frameLine, "Name:", "Name of the link", pEditName, new cTextName( *this ) );
	
	helper.ComboBox( frameLine, "Link:", true, "Name of the link if used", pCBController,
		new cComboController( *this ) );
	pCBController->SetDefaultSorter();
	
	helper.EditInteger( frameLine, "Repeat:", "Repeat link value", pEditRepeat, new cTextRepeat( *this ) );
	
	helper.ViewCurveBezier( groupBox, pEditCurve, new cEditCurve( *this ) );
}

seWPLink::~seWPLink(){
	SetSky( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPLink::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	if( pSky ){
		pSky->RemoveListener( pListener );
		pSky->FreeReference();
	}
	
	pSky = sky;
	
	if( sky ){
		sky->AddListener( pListener );
		sky->AddReference();
	}
	
	UpdateControllerList();
	UpdateLinkList();
}

seLink *seWPLink::GetLink() const{
	if( pSky ){
		return pSky->GetActiveLink();
	}
	
	return NULL;
}



void seWPLink::UpdateLinkList(){
	pListLinks->RemoveAllItems();
	
	if( pSky ){
		const seLinkList &links = pSky->GetLinks();
		const int linkCount = links.GetCount();
		int i;
		
		for( i=0; i<linkCount; i++ ){
			seLink * const link = links.GetAt( i );
			pListLinks->AddItem( link->GetName().GetString(), NULL, link );
		}
		pListLinks->SortItems();
	}
	
	SelectActiveLink();
}

void seWPLink::SelectActiveLink(){
	const int selection = pListLinks->IndexOfItemWithData( GetLink() );
	
	pListLinks->SetSelection( selection );
	if( selection != -1 ){
		pListLinks->MakeItemVisible( selection );
	}
	
	UpdateLink();
}

void seWPLink::UpdateLink(){
	seLink * const link = GetLink();
	
	if( link ){
		pEditName->SetText( link->GetName() );
		pCBController->SetSelectionWithData(link->GetController() );
		pEditRepeat->SetInteger( link->GetRepeat() );
		pEditCurve->SetCurve( link->GetCurve() );
		
	}else{
		pEditName->ClearText();
		pCBController->SetSelectionWithData( NULL );
		pEditRepeat->ClearText();
		pEditCurve->SetDefaultBezier();
	}
	
	const bool enabled = link != NULL;
	pEditName->SetEnabled( enabled );
	pCBController->SetEnabled( enabled );
	pEditRepeat->SetEnabled( enabled );
	pEditCurve->SetEnabled( enabled );
}

void seWPLink::UpdateControllerList(){
	pCBController->RemoveAllItems();
	pCBController->AddItem( "< No Controller >", NULL );
	
	if( pSky ){
		const seControllerList &controllers = pSky->GetControllers();
		const int controllerCount = controllers.GetCount();
		int i;
		
		for( i=0; i<controllerCount; i++ ){
			seController * const controller = controllers.GetAt( i );
			pCBController->AddItem( controller->GetName().GetString(), NULL, controller );
		}
		pCBController->SortItems();
	}
	
	seLink * const link = GetLink();
	if( link ){
		pCBController->SetSelectionWithData( link->GetController() );
		
	}else{
		pCBController->SetSelectionWithData( NULL );
	}
}
