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

#include "syneWPAPanelSourceGroup.h"
#include "../../syneWPSource.h"
#include "../../../syneWindowMain.h"
#include "../../../../synthesizer/syneSynthesizer.h"
#include "../../../../synthesizer/source/syneSourceGroup.h"
#include "../../../../undosys/source/group/syneUSourceGroupSetApplicationType.h"

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
	syneWPAPanelSourceGroup &pPanel;
	
public:
	using Ref = deTObjectReference<cComboApplicationType>;
	cComboApplicationType(syneWPAPanelSourceGroup &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		syneSourceGroup * const source = (syneSourceGroup*)pPanel.GetSource();
		if(!source || !comboBox->GetSelectedItem()){
			return;
		}
		
		const deSynthesizerSourceGroup::eApplicationTypes type =
			(deSynthesizerSourceGroup::eApplicationTypes)(intptr_t)
			comboBox->GetSelectedItem()->GetData();
		if(source->GetApplicationType() == type){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			syneUSourceGroupSetApplicationType::Ref::New(source, type));
	}
};

}



// Class syneWPAPanelSourceGroup
////////////////////////////////

// Constructor, destructor
////////////////////////////

syneWPAPanelSourceGroup::syneWPAPanelSourceGroup(syneWPSource &wpSource) :
syneWPAPanelSource(wpSource, deSynthesizerSourceVisitorIdentify::estGroup)
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "@Synthesizer.WPAPanelSourceGroup.GroupGroup");
	helper.ComboBox(groupBox, "@Synthesizer.WPAPanelSourceGroup.FieldApplicationType",
		"@Synthesizer.WPAPanelSourceGroup.FieldApplicationType.ToolTip",
		pCBApplicationType, cComboApplicationType::Ref::New(*this));
	pCBApplicationType->SetAutoTranslateItems(true);
	pCBApplicationType->AddItem("@Synthesizer.WPAPanelSourceGroup.ApplicationType.All", nullptr, (void*)(intptr_t)deSynthesizerSourceGroup::eatAll);
	pCBApplicationType->AddItem("@Synthesizer.WPAPanelSourceGroup.ApplicationType.Select", nullptr, (void*)(intptr_t)deSynthesizerSourceGroup::eatSelect);
	pCBApplicationType->AddItem("@Synthesizer.WPAPanelSourceGroup.ApplicationType.Solo", nullptr, (void*)(intptr_t)deSynthesizerSourceGroup::eatSolo);
}

syneWPAPanelSourceGroup::~syneWPAPanelSourceGroup(){
}



// Management
///////////////

void syneWPAPanelSourceGroup::UpdateSource(){
	syneWPAPanelSource::UpdateSource();
	
	const syneSourceGroup * const source = (syneSourceGroup*)GetSource();
	if(source){
		pCBApplicationType->SetSelectionWithData((void*)(intptr_t)source->GetApplicationType());
		
	}else{
		pCBApplicationType->SetSelectionWithData((void*)(intptr_t)deSynthesizerSourceGroup::eatAll);
	}
	
	pCBApplicationType->SetEnabled(source);
}

void syneWPAPanelSourceGroup::UpdateTargetList(){
	syneSourceGroup * const source = (syneSourceGroup*)GetSource();
	
	syneWPAPanelSource::UpdateTargetList();
	
	if(source){
		AddTarget("@Synthesizer.WPAPanelSourceGroup.Target.Select", source->GetTargetSelect());
	}
}
