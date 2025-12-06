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
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
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
		
		seUSourceGroupSetApplicationType::Ref undo(seUSourceGroupSetApplicationType::Ref::New( new seUSourceGroupSetApplicationType( source, type ) ));
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
	igdeContainer::Ref groupBox;
	
	
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
