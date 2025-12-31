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

#include "syneWPAPanelSourceSynthesizer.h"
#include "../../syneWPSource.h"
#include "../../../syneWindowMain.h"
#include "../../../../synthesizer/syneSynthesizer.h"
#include "../../../../synthesizer/controller/syneController.h"
#include "../../../../synthesizer/source/syneSourceSynthesizer.h"
#include "../../../../undosys/source/synthesizer/syneUSourceSynthSetPathSynthesizer.h"
#include "../../../../undosys/source/synthesizer/syneUSourceSynthConTargetMore.h"
#include "../../../../undosys/source/synthesizer/syneUSourceSynthConTargetLess.h"
#include "../../../../undosys/source/synthesizer/syneUSourceSynthSetConController.h"

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
	syneWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cPathSynthesizer> Ref;
	cPathSynthesizer(syneWPAPanelSourceSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath * editPath){
		syneSourceSynthesizer * const source = (syneSourceSynthesizer*)pPanel.GetSource();
		if(!source || source->GetPathSynthesizer() == editPath->GetPath()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			syneUSourceSynthSetPathSynthesizer::Ref::New(source, editPath->GetPath()));
	}
};

class cComboConTarget : public igdeComboBoxListener{
	syneWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cComboConTarget> Ref;
	cComboConTarget(syneWPAPanelSourceSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		syneSourceSynthesizer * const source = (syneSourceSynthesizer*)pPanel.GetSource();
		if(source){
			pPanel.UpdateConnection();
		}
	}
};

class cActionConTargetMore : public igdeAction {
	syneWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cActionConTargetMore> Ref;
	cActionConTargetMore(syneWPAPanelSourceSynthesizer &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Increase the number of targets"), pPanel(panel){}
	
	void OnAction() override{
		syneSourceSynthesizer * const source = (syneSourceSynthesizer*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			syneUSourceSynthConTargetMore::Ref::New(source));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSource());
	}
};

class cActionConTargetLess : public igdeAction {
	syneWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cActionConTargetLess> Ref;
	cActionConTargetLess(syneWPAPanelSourceSynthesizer &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Decrease the number of targets"), pPanel(panel){}
	
	void OnAction() override{
		syneSourceSynthesizer * const source = (syneSourceSynthesizer*)pPanel.GetSource();
		if(!source || source->GetConnections().IsEmpty()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			syneUSourceSynthConTargetLess::Ref::New(source));
	}
	
	void Update() override{
		const syneSourceSynthesizer * const source = (syneSourceSynthesizer*)pPanel.GetSource();
		SetEnabled(source && source->GetConnections().IsNotEmpty());
	}
};

class cComboController : public igdeComboBoxListener{
	syneWPAPanelSourceSynthesizer &pPanel;
	
public:
	typedef deTObjectReference<cComboController> Ref;
	cComboController(syneWPAPanelSourceSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		syneSourceSynthesizer * const source = (syneSourceSynthesizer*)pPanel.GetSource();
		const int target = pPanel.GetCBControllerTarget();
		if(!source || target == -1 || pPanel.GetPreventUpdate()){
			return;
		}
		
		syneController * const controller = comboBox->GetSelectedItem()
			? (syneController*)comboBox->GetSelectedItem()->GetData() : nullptr;
		if(source->GetConnections().GetAt(target) == controller){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			syneUSourceSynthSetConController::Ref::New(source, target, controller));
	}
};

}



// Class syneWPAPanelSourceSynthesizer
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

syneWPAPanelSourceSynthesizer::syneWPAPanelSourceSynthesizer(syneWPSource &wpSource) :
syneWPAPanelSource(wpSource, deSynthesizerSourceVisitorIdentify::estSynthesizer),
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

syneWPAPanelSourceSynthesizer::~syneWPAPanelSourceSynthesizer(){
}



// Management
///////////////

void syneWPAPanelSourceSynthesizer::OnSynthesizerPathChanged(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		pEditPathSynthesizer->SetBasePath(synthesizer->GetDirectoryPath());
		
	}else{
		pEditPathSynthesizer->SetBasePath("");
	}
}

void syneWPAPanelSourceSynthesizer::UpdateConTargetList(){
	const syneSourceSynthesizer * const source = (syneSourceSynthesizer*)GetSource();
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

int syneWPAPanelSourceSynthesizer::GetCBControllerTarget() const{
	return pCBConTarget->GetSelection();
}

void syneWPAPanelSourceSynthesizer::UpdateControllerList(){
	const syneSynthesizer * const synthesizer = GetSynthesizer();
	syneController * const selection = pCBConController->GetSelectedItem()
		? (syneController*)pCBConController->GetSelectedItem()->GetData() : nullptr;
	
	pPreventUpdate = true; // required since changing list causes text changes
	
	try{
		pCBConController->RemoveAllItems();
		pCBConController->AddItem("< No Controller >", nullptr);
		
		if(synthesizer){
			const syneController::List &list = synthesizer->GetControllers();
			const int count = list.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				syneController * const controller = list.GetAt(i);
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

void syneWPAPanelSourceSynthesizer::UpdateSource(){
	syneWPAPanelSource::UpdateSource();
	
	const syneSourceSynthesizer * const source = (syneSourceSynthesizer*)GetSource();
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

void syneWPAPanelSourceSynthesizer::UpdateConnection(){
	syneWPAPanelSource::UpdateSource();
	
	const syneSourceSynthesizer * const source = (syneSourceSynthesizer*)GetSource();
	const int selection = pCBConTarget->GetSelection();
	
	if(source && selection != -1){
		pCBConController->SetSelectionWithData(source->GetConnections().GetAt(selection));
	}
	
	pCBConController->SetEnabled(source && selection != -1);
}
