/* 
 * Drag[en]gine IGDE Synthesizer Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <stdint.h>

#include "seWPAPanelSourceGroup.h"
#include "../../seWPSource.h"
#include "../../../seWindowMain.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/source/seSourceGroup.h"
#include "../../../../undosys/source/group/seUSourceGroupSetApplicationType.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>


#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cComboApplicationType : public igdeComboBoxListener{
protected:
	seWPAPanelSourceGroup &pPanel;
	
public:
	cComboApplicationType( seWPAPanelSourceGroup &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSourceGroup * const source = ( seSourceGroup* )pPanel.GetSource();
		if( ! source || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const deSynthesizerSourceGroup::eApplicationTypes type =
			( deSynthesizerSourceGroup::eApplicationTypes )( intptr_t )
			comboBox->GetSelectedItem()->GetData();
		if( source->GetApplicationType() == type ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUSourceGroupSetApplicationType( source, type ) );
		source->GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
};

}



// Class seWPAPanelSourceGroup
////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSourceGroup::seWPAPanelSourceGroup( seWPSource &wpSource ) :
seWPAPanelSource( wpSource, deSynthesizerSourceVisitorIdentify::estGroup )
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Group:" );
	helper.ComboBox( groupBox, "Group:", "Sets how child sources are process in the the group.\n"
		"Apply all sources (all) or blend between two selected sources using the select target (select).",
		pCBApplicationType, new cComboApplicationType( *this ) );
	pCBApplicationType->AddItem( "All", NULL, ( void* )( intptr_t )deSynthesizerSourceGroup::eatAll );
	pCBApplicationType->AddItem( "Select", NULL, ( void* )( intptr_t )deSynthesizerSourceGroup::eatSelect );
	pCBApplicationType->AddItem( "Solo", NULL, ( void* )( intptr_t )deSynthesizerSourceGroup::eatSolo );
}

seWPAPanelSourceGroup::~seWPAPanelSourceGroup(){
}



// Management
///////////////

void seWPAPanelSourceGroup::UpdateSource(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceGroup * const source = ( seSourceGroup* )GetSource();
	if( source ){
		pCBApplicationType->SetSelectionWithData( ( void* )( intptr_t )source->GetApplicationType() );
		
	}else{
		pCBApplicationType->SetSelectionWithData( ( void* )( intptr_t )deSynthesizerSourceGroup::eatAll );
	}
	
	pCBApplicationType->SetEnabled( source );
}

void seWPAPanelSourceGroup::UpdateTargetList(){
	seSourceGroup * const source = ( seSourceGroup* )GetSource();
	
	seWPAPanelSource::UpdateTargetList();
	
	if( source ){
		AddTarget( "Select", &source->GetTargetSelect() );
	}
}
