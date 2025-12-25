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

#include "seWPAPanelSourceSynthesizer.h"
#include "../../seWPSource.h"
#include "../../../seWindowMain.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/controller/seController.h"
#include "../../../../synthesizer/source/seSourceSynthesizer.h"
#include "../../../../undosys/source/synthesizer/seUSourceSynthSetPathSynthesizer.h"
#include "../../../../undosys/source/synthesizer/seUSourceSynthConTargetMore.h"
#include "../../../../undosys/source/synthesizer/seUSourceSynthConTargetLess.h"
#include "../../../../undosys/source/synthesizer/seUSourceSynthSetConController.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>



// Actions
////////////

namespace{

class cPathSynthesizer : public igdeEditPathListener{
	seWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cPathSynthesizer> Ref;
	cPathSynthesizer(seWPAPanelSourceSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath * editPath){
		seSourceSynthesizer * const source = (seSourceSynthesizer*)pPanel.GetSource();
		if(!source || source->GetPathSynthesizer() == editPath->GetPath()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			seUSourceSynthSetPathSynthesizer::Ref::New(source, editPath->GetPath()));
	}
};

class cComboConTarget : public igdeComboBoxListener{
	seWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cComboConTarget> Ref;
	cComboConTarget(seWPAPanelSourceSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		seSourceSynthesizer * const source = (seSourceSynthesizer*)pPanel.GetSource();
		if(source){
			pPanel.UpdateConnection();
		}
	}
};

class cActionConTargetMore : public igdeAction {
	seWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cActionConTargetMore> Ref;
	cActionConTargetMore(seWPAPanelSourceSynthesizer &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Increase the number of targets"), pPanel(panel){}
	
	void OnAction() override{
		seSourceSynthesizer * const source = (seSourceSynthesizer*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			seUSourceSynthConTargetMore::Ref::New(source));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSource());
	}
};

class cActionConTargetLess : public igdeAction {
	seWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cActionConTargetLess> Ref;
	cActionConTargetLess(seWPAPanelSourceSynthesizer &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Decrease the number of targets"), pPanel(panel){}
	
	void OnAction() override{
		seSourceSynthesizer * const source = (seSourceSynthesizer*)pPanel.GetSource();
		if(!source || source->GetConnections().IsEmpty()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			seUSourceSynthConTargetLess::Ref::New(source));
	}
	
	void Update() override{
		const seSourceSynthesizer * const source = (seSourceSynthesizer*)pPanel.GetSource();
		SetEnabled(source && source->GetConnections().IsNotEmpty());
	}
};

class cComboController : public igdeComboBoxListener{
	seWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cComboController> Ref;
	cComboController(seWPAPanelSourceSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seSourceSynthesizer * const source = (seSourceSynthesizer*)pPanel.GetSource();
		const int target = pPanel.GetCBControllerTarget();
		if(!source || target == -1 || pPanel.GetPreventUpdate()){
			return;
		}
		
		seController * const controller = comboBox->GetSelectedItem()
			? (seController*)comboBox->GetSelectedItem()->GetData() : nullptr;
		if(source->GetConnections().GetAt(target) == controller){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			seUSourceSynthSetConController::Ref::New(source, target, controller));
	}
};

}



// Class seWPAPanelSourceSynthesizer
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSourceSynthesizer::seWPAPanelSourceSynthesizer(seWPSource &wpSource) :
seWPAPanelSource(wpSource, deSynthesizerSourceVisitorIdentify::estSynthesizer),
pPreventUpdate(false)
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, formLine;
	
	
	helper.GroupBox(*this, groupBox, "Synthesizer:");
	helper.EditPath(groupBox, "Path:", "Sets the synthesizer to use", igdeEnvironment::efpltSynthesizer,
		pEditPathSynthesizer, cPathSynthesizer::Ref::New(*this));
	
	
	helper.GroupBox(*this, groupBox, "Connections:");
	
	helper.FormLineStretchFirst(groupBox, "Target:", "Target controller to edit", formLine);
	helper.ComboBox(formLine, "Target controller", pCBConTarget, cComboConTarget::Ref::New(*this));
	helper.Button(formLine, pBtnConTargetMore, cActionConTargetMore::Ref::New(*this));
	helper.Button(formLine, pBtnConTargetLess, cActionConTargetLess::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Controller:", "Controller to take value from",
		pCBConController, cComboController::Ref::New(*this));
	
	UpdateControllerList();
}

seWPAPanelSourceSynthesizer::~seWPAPanelSourceSynthesizer(){
}



// Management
///////////////

void seWPAPanelSourceSynthesizer::OnSynthesizerPathChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		pEditPathSynthesizer->SetBasePath(synthesizer->GetDirectoryPath());
		
	}else{
		pEditPathSynthesizer->SetBasePath("");
	}
}

void seWPAPanelSourceSynthesizer::UpdateConTargetList(){
	const seSourceSynthesizer * const source = (seSourceSynthesizer*)GetSource();
	const int selection = pCBConTarget->GetSelection();
	int connectionCount = 0;
	
	pCBConTarget->RemoveAllItems();
	
	if(source){
		const decStringList &names = source->GetControllerNames();
		decString text;
		int i;
		
		connectionCount = source->GetConnections().GetCount();
		
		for(i=0; i<connectionCount; i++){
			if(i < names.GetCount()){
				text.Format("%d: %s", i, names.GetAt(i).GetString());
				
			}else{
				text.Format("%d: < invalid >", i);
			}
			
			pCBConTarget->AddItem(text);
		}
	}
	
	pCBConTarget->SetSelection(decMath::min(selection, connectionCount - 1));
	
	UpdateConnection();
}

int seWPAPanelSourceSynthesizer::GetCBControllerTarget() const{
	return pCBConTarget->GetSelection();
}

void seWPAPanelSourceSynthesizer::UpdateControllerList(){
	const seSynthesizer * const synthesizer = GetSynthesizer();
	seController * const selection = pCBConController->GetSelectedItem()
		? (seController*)pCBConController->GetSelectedItem()->GetData() : nullptr;
	
	pPreventUpdate = true; // required since changing list causes text changes
	
	try{
		pCBConController->RemoveAllItems();
		pCBConController->AddItem("< No Controller >", nullptr);
		
		if(synthesizer){
			const seController::List &list = synthesizer->GetControllers();
			const int count = list.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				seController * const controller = list.GetAt(i);
				pCBConController->AddItem(controller->GetName(), nullptr, controller);
			}
		}
		
		pCBConController->SortItems();
		
		pCBConController->SetSelectionWithData(selection);
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
	
	pPreventUpdate = false;
}

void seWPAPanelSourceSynthesizer::UpdateSource(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceSynthesizer * const source = (seSourceSynthesizer*)GetSource();
	if(source){
		pEditPathSynthesizer->SetPath(source->GetPathSynthesizer());
		
	}else{
		pEditPathSynthesizer->ClearPath();
	}
	
	const bool enabled = source;
	pEditPathSynthesizer->SetEnabled(enabled);
	pEditPathSynthesizer->SetEnabled(enabled);
	pCBConTarget->SetEnabled(enabled);
	
	pBtnConTargetMore->GetAction()->Update();
	pBtnConTargetLess->GetAction()->Update();
	
	UpdateConTargetList();
}

void seWPAPanelSourceSynthesizer::UpdateConnection(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceSynthesizer * const source = (seSourceSynthesizer*)GetSource();
	const int selection = pCBConTarget->GetSelection();
	
	if(source && selection != -1){
		pCBConController->SetSelectionWithData(source->GetConnections().GetAt(selection));
	}
	
	pCBConController->SetEnabled(source && selection != -1);
}
