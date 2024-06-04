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

#include "meWPSNavSpace.h"
#include "meWPSNavSpaceListener.h"
#include "../meWPSelection.h"
#include "../../meWindowProperties.h"
#include "../../../meWindowMain.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/navspace/meNavigationSpace.h"
#include "../../../../world/navspace/meNavigationSpaceSelection.h"
#include "../../../../undosys/properties/navspace/meUNavSpaceSetOrientation.h"
#include "../../../../undosys/properties/navspace/meUNavSpaceSetPath.h"
#include "../../../../undosys/properties/navspace/meUNavSpaceSetPosition.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cSpinNavSpace : public igdeSpinTextFieldListener{
	meWPSNavSpace &pPanel;
	
public:
	cSpinNavSpace( meWPSNavSpace &panel ) : pPanel( panel ){ }
	
	virtual void OnValueChanged( igdeSpinTextField *textField ){
		meWorld * const world = pPanel.GetWorld();
		if( ! world ){
			return;
		}
		
		meNavigationSpaceSelection &selection = world->GetSelectionNavigationSpace();
		const int active = textField->GetValue();
		if( active < 0 || active >= selection.GetSelected().GetCount() ){
			return;
		}
		
		meNavigationSpace * const navspace = selection.GetSelected().GetAt( active );
		if( navspace == selection.GetActive() ){
			return;
		}
		
		selection.SetActive( navspace );
		world->NotifyNavSpaceSelectionChanged();
	}
};

class cPathNavSpace : public igdeEditPathListener{
	meWPSNavSpace &pPanel;
	
public:
	cPathNavSpace( meWPSNavSpace &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		meNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if( ! navspace ){
			return;
		}
		
		if( editPath->GetPath() == navspace->GetFilename() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUNavSpaceSetPath( navspace, editPath->GetPath() ) );
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cEditPosition : public igdeEditDVectorListener{
	meWPSNavSpace &pPanel;
	
public:
	cEditPosition( meWPSNavSpace &panel ) : pPanel( panel ){ }
	
	virtual void OnDVectorChanged( igdeEditDVector *editDVector ){
		meNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if( ! navspace ){
			return;
		}
		
		if( editDVector->GetDVector().IsEqualTo( navspace->GetPosition() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUNavSpaceSetPosition( navspace, editDVector->GetDVector() ) );
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cEditOrientation : public igdeEditVectorListener{
	meWPSNavSpace &pPanel;
	
public:
	cEditOrientation( meWPSNavSpace &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		meNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if( ! navspace ){
			return;
		}
		
		if( editVector->GetVector().IsEqualTo( navspace->GetOrientation() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUNavSpaceSetOrientation( navspace, editVector->GetVector() ) );
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWPSNavSpace
////////////////////////

// Constructor, destructor
////////////////////////////

meWPSNavSpace::meWPSNavSpace( meWPSelection &wpselection ) :
igdeContainerScroll( wpselection.GetEnvironment(), false, true ),
pWPSelection( wpselection ),
pListener( NULL ),
pWorld( NULL )
{
	igdeEnvironment &env = wpselection.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox;
	
	pListener = new meWPSNavSpaceListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// selection
	groupBox.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 10 ) );
	content->AddChild( groupBox );
	helper.Label( groupBox, "Selection:" );
	helper.EditInteger( groupBox, "Count of selected navigation spaces", 3, pEditSelCount, NULL );
	pEditSelCount->SetEditable( false );
	helper.Label( groupBox, "Active:" );
	helper.EditSpinInteger( groupBox, "Selected navigation space", 0, 0, pSpinActive, new cSpinNavSpace( *this ) );
	
	
	// navigation space
	helper.GroupBox( content, groupBox, "Navigation Space:" );
	helper.EditPath( groupBox, "Path:", "Path to the navigation space",
		igdeEnvironment::efpltNavigationSpace, pEditPath, new cPathNavSpace( *this ) );
	helper.EditDVector( groupBox, "Position:", "Position of the navigation space.",
		pEditPositon, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Orientation:", "Orientation of the navigation space.",
		pEditOrientation, new cEditOrientation( *this ) );
	
	
	// cost information
	helper.GroupBoxFlow( content, groupBox, "Used Cost Types:" );
	helper.ListBox( groupBox, 5, "Used cost types", pListUsedCostTypes, NULL );
	pListUsedCostTypes->SetDefaultSorter();
}

meWPSNavSpace::~meWPSNavSpace(){
	SetWorld( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWPSNavSpace::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
	}
	
	UpdateSelection();
	UpdateEnabled();
	UpdateNavSpace();
	UpdateGeometry();
	UpdateUsedCostTypes();
}



meNavigationSpace *meWPSNavSpace::GetNavigationSpace() const{
	return pWorld ? pWorld->GetSelectionNavigationSpace().GetActive() : NULL;
}

void meWPSNavSpace::UpdateSelection(){
	int active = 0;
	int count = 0;
	
	if( pWorld ){
		const meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
		count = selection.GetSelected().GetCount();
		if( selection.HasActive() ){
			active = selection.GetSelected().IndexOf( selection.GetActive() );
		}
	}
	
	pEditSelCount->SetInteger( count );
	pSpinActive->SetRange( 0, count - 1 );
	pSpinActive->SetValue( active );
	pSpinActive->SetEnabled( count > 0 );
}

void meWPSNavSpace::UpdateEnabled(){
	const bool enabled = GetNavigationSpace();
	pEditPath->SetEnabled( enabled );
	pEditPositon->SetEnabled( enabled );
	pEditOrientation->SetEnabled( enabled );
	pSpinActive->SetEnabled( enabled );
}

void meWPSNavSpace::UpdateNavSpace(){
	const meNavigationSpace * const navspace = GetNavigationSpace();
	
	if( navspace ){
		pEditPath->SetPath( navspace->GetFilename() );
		
	}else{
		pEditPath->ClearPath();
	}
}

void meWPSNavSpace::UpdateGeometry(){
	const meNavigationSpace * const navspace = GetNavigationSpace();
	
	if( navspace ){
		pEditPositon->SetDVector( navspace->GetPosition() );
		pEditOrientation->SetVector( navspace->GetOrientation() );
		
	}else{
		pEditPositon->SetDVector( decDVector() );
		pEditOrientation->SetVector( decVector() );
	}
}

void meWPSNavSpace::UpdateUsedCostTypes(){
	const meNavigationSpace * const navspace = GetNavigationSpace();
	
	pListUsedCostTypes->RemoveAllItems();
	
	if( navspace ){
		const decIntList &list = navspace->GetUsedCostTypes();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			text.Format( "Type #%i", list.GetAt( i ) );
			pListUsedCostTypes->AddItem( text );
		}
	}
	
	pListUsedCostTypes->SortItems();
}
