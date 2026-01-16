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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeWPView.h"
#include "aeWPViewListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeCamera.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/attachment/aeAttachment.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/wakeboard/aeWakeboard.h"
#include "../../animator/aeSubAnimator.h"
#include "../../configuration/aeConfiguration.h"
#include "../../loadsave/aeLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPCamera.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPView &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPView &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnAction(animator);
		}
	}
	
	virtual void OnAction(aeAnimator *animator) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			Update(*animator);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextField : public igdeTextFieldListener{
protected:
	aeWPView &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseTextField>;
	cBaseTextField(aeWPView &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(textField, animator);
		}
	}
	
	virtual void OnChanged(igdeTextField *textField, aeAnimator *animator) = 0;
};

class cBaseComboBox : public igdeComboBoxListener{
protected:
	aeWPView &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBox>;
	cBaseComboBox(aeWPView &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(comboBox, animator);
		}
	}
	
	virtual void OnChanged(igdeComboBox *comboBox, aeAnimator *animator) = 0;
};

class cBaseEditPath : public igdeEditPathListener{
protected:
	aeWPView &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseEditPath>;
	cBaseEditPath(aeWPView &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(editPath, animator);
		}
	}
	
	virtual void OnChanged(igdeEditPath *editPath, aeAnimator *animator) = 0;
};



class cPathDisplayModel : public cBaseEditPath{
public:
	using Ref = deTObjectReference<cPathDisplayModel>;
	cPathDisplayModel(aeWPView &panel) : cBaseEditPath(panel){}
	
	void OnChanged(igdeEditPath *editPath, aeAnimator *animator) override{
		animator->SetDisplayModelPath(editPath->GetPath());
	}
};

class cPathDisplaySkin : public cBaseEditPath{
public:
	using Ref = deTObjectReference<cPathDisplaySkin>;
	cPathDisplaySkin(aeWPView &panel) : cBaseEditPath(panel){}
	
	void OnChanged(igdeEditPath *editPath, aeAnimator *animator) override{
		animator->SetDisplaySkinPath(editPath->GetPath());
	}
};

class cPathDisplayRig : public cBaseEditPath{
public:
	using Ref = deTObjectReference<cPathDisplayRig>;
	cPathDisplayRig(aeWPView &panel) : cBaseEditPath(panel){}
	
	void OnChanged(igdeEditPath *editPath, aeAnimator *animator) override{
		animator->SetDisplayRigPath(editPath->GetPath());
	}
};


class cPathTestingAnimator : public cBaseEditPath{
public:
	using Ref = deTObjectReference<cPathTestingAnimator>;
	cPathTestingAnimator(aeWPView &panel) : cBaseEditPath(panel){}
	
	void OnChanged(igdeEditPath *editPath, aeAnimator *animator) override{
		if(animator->GetTestingSubAnimator()->GetPathAnimator().Equals(editPath->GetPath())){
			return;
		}
		
		animator->GetTestingSubAnimator()->SetPathAnimator(editPath->GetPath());
		animator->GetTestingSubAnimator()->LoadAnimator(
			pPanel.GetWindowProperties().GetWindowMain().GetLoadSaveSystem());
		animator->NotifyViewChanged();
	}
};

class cActionResetState : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionResetState>;
	
public:
	cActionResetState(aeWPView &panel) : cBaseAction(panel, "Reset Animation State", nullptr,
		"Animation state is reset before each frame update for testing relative animators"){ }
	
	void OnAction(aeAnimator *animator) override{
		animator->SetResetState(!animator->GetResetState());
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(true);
		SetSelected(animator.GetResetState());
	}
};


class cEditSky : public cBaseAction{
public:
	using Ref = deTObjectReference<cEditSky>;
	
public:
	cEditSky(aeWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(aeAnimator *animator) override{
		animator->NotifySkyChanged();
	}
};

class cEditEnvObject : public cBaseAction{
public:
	using Ref = deTObjectReference<cEditEnvObject>;
	
public:
	cEditEnvObject(aeWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(aeAnimator *animator) override{
		animator->NotifyEnvObjectChanged();
	}
};

class cEditCamera : public cBaseAction{
public:
	using Ref = deTObjectReference<cEditCamera>;
	
public:
	cEditCamera(aeWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(aeAnimator *animator) override{
		animator->NotifyCameraChanged();
	}
};


class cActionCamAttach : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionCamAttach>;
	
public:
	cActionCamAttach(aeWPView &panel) : cBaseAction(panel, "Attach camera to a bone", nullptr,
		"Attaches the camera to a bone instead of roaming around freely"){}
	
	void OnAction(aeAnimator *animator) override{
		animator->GetCamera()->SetAttachToBone(!animator->GetCamera()->GetAttachToBone());
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(true);
		SetSelected(animator.GetCamera()->GetAttachToBone());
	}
};

class cComboCamAttachBone : public cBaseComboBox{
public:
	using Ref = deTObjectReference<cComboCamAttachBone>;
	cComboCamAttachBone(aeWPView &panel) : cBaseComboBox(panel){}
	
	void OnChanged(igdeComboBox *comboBox, aeAnimator *animator) override{
		animator->GetCamera()->SetBone(comboBox->GetText());
	}
};

class cEditCamRelPos : public igdeEditVectorListener{
	aeWPView &pPanel;
public:
	using Ref = deTObjectReference<cEditCamRelPos>;
	cEditCamRelPos(aeWPView &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			animator->GetCamera()->SetRelativePosition(editVector->GetVector());
		}
	}
};

class cEditCamRelRot : public igdeEditVectorListener{
	aeWPView &pPanel;
public:
	using Ref = deTObjectReference<cEditCamRelRot>;
	cEditCamRelRot(aeWPView &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			animator->GetCamera()->SetRelativeOrientation(editVector->GetVector());
		}
	}
};


class cTextPlaySpeed : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextPlaySpeed>;
	cTextPlaySpeed(aeWPView &panel) : cBaseTextField(panel){}
	
	void OnChanged(igdeTextField *textField, aeAnimator *animator) override{
		animator->SetPlaySpeed(textField->GetFloat());
	}
};

class cTextTimeStep : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextTimeStep>;
	cTextTimeStep(aeWPView &panel) : cBaseTextField(panel){}
	
	void OnChanged(igdeTextField *textField, aeAnimator *animator) override{
		animator->SetTimeStep(textField->GetFloat());
	}
};

class cActionFrameStep : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionFrameStep>;
	
private:
	const float pFactor;
public:
	cActionFrameStep(aeWPView &panel, igdeIcon *icon, const char *description, float factor) :
		cBaseAction(panel, "", icon, description), pFactor(factor){}
	
	void OnAction(aeAnimator *animator) override{
		const float timeStep = animator->GetTimeStep() * pFactor;
		animator->GetControllers().Visit([&](aeController &controller){
			controller.UpdateValue(timeStep);
		});
	}
};

class cActionFramePrev : public cActionFrameStep{
public:
	using Ref = deTObjectReference<cActionFramePrev>;
	cActionFramePrev(aeWPView &panel) : cActionFrameStep(panel,
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiLeft),
		"Step animation backward by the given time step", -1.0f){}
};

class cActionFrameNext : public cActionFrameStep{
public:
	using Ref = deTObjectReference<cActionFrameNext>;
	cActionFrameNext(aeWPView &panel) : cActionFrameStep(panel,
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiRight),
		"Step animation forward by the given time step", 1.0f){}
};

class cActionPaused : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionPaused>;
	
public:
	cActionPaused(aeWPView &panel) : cBaseAction(panel, "Pause", nullptr, "Paus animation playback"){}
	
	void OnAction(aeAnimator *animator) override{
		animator->SetPaused(!animator->GetPaused());
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(true);
		SetSelected(animator.GetPaused());
	}
};

class cActionReset : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionReset>;
	
public:
	cActionReset(aeWPView &panel) : cBaseAction(panel, "Reset", nullptr, "Reset animation"){}
	
	void OnAction(aeAnimator *animator) override{
		animator->GetControllers().Visit([](aeController &controller){
			controller.ResetValue();
		});
	}
};


class cComboAttachment : public cBaseComboBox{
public:
	using Ref = deTObjectReference<cComboAttachment>;
	cComboAttachment(aeWPView &panel) : cBaseComboBox(panel){}
	
	void OnChanged(igdeComboBox *comboBox, aeAnimator *animator) override{
		animator->SetActiveAttachment(comboBox->GetSelectedItem()
			? (aeAttachment*)comboBox->GetSelectedItem()->GetData() : nullptr);
	}
};

class cActionAttachmentAdd : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionAttachmentAdd>;
	
public:
	cActionAttachmentAdd(aeWPView &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add attachment"){}
	
	void OnAction(aeAnimator *animator) override{
		const decString baseName("Attachment");
		decString name(baseName);
		int number = 1;
		while(animator->GetAttachmentNamed(name)){
			name.Format("%s #%d", baseName.GetString(), number);
		}
		
		const aeAttachment::Ref attachment(aeAttachment::Ref::New(&pPanel.GetEnvironment(), name));
		animator->AddAttachment(attachment);
		animator->SetActiveAttachment(attachment);
	}
};

class cActionAttachmentRemove : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionAttachmentRemove>;
	
public:
	cActionAttachmentRemove(aeWPView &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove attachment"){}
	
	void OnAction(aeAnimator *animator) override{
		aeAttachment * const attachment = pPanel.GetAttachment();
		if(!attachment){
			return;
		}
		
		animator->RemoveAttachment(attachment);
		if(animator->GetAttachments().IsNotEmpty()){
			animator->SetActiveAttachment(animator->GetAttachments().First());
		}
	}
	
	void Update(const aeAnimator &) override{
		SetEnabled(pPanel.GetAttachment());
	}
};

class cActionAttachmentClear : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionAttachmentClear>;
	
public:
	cActionAttachmentClear(aeWPView &panel) : cBaseAction(panel, "Clear",
		nullptr, "Remove all attachment"){}
	
	void OnAction(aeAnimator *animator) override{
		animator->RemoveAllAttachments();
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetAttachments().IsNotEmpty());
	}
};

class cActionAttachmentLoadConfig : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionAttachmentLoadConfig>;
	
public:
	cActionAttachmentLoadConfig(aeWPView &panel) : cBaseAction(panel, "Load configuration...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Load configuration"){}
	
	void OnAction(aeAnimator *animator) override{
		aeLoadSaveSystem &lssys = pPanel.GetWindowProperties().GetWindowMain().GetLoadSaveSystem();
		decString filename(animator->GetPathAttachmentConfig());
		if(!igdeCommonDialogs::GetFileOpen(&pPanel, "Open Attachment Configuration",
		*pPanel.GetEnvironment().GetFileSystemGame(), lssys.GetAttConfigFilePatterns(), filename ) ){
			return;
		}
		
		animator->SetPathAttachmentConfig(filename);
		lssys.LoadAttConfig(filename, *animator);
	}
};

class cActionAttachmentSaveConfig : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionAttachmentSaveConfig>;
	
public:
	cActionAttachmentSaveConfig(aeWPView &panel) : cBaseAction(panel, "Save configuration...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Save configuration"){}
	
	void OnAction(aeAnimator *animator) override{
		aeLoadSaveSystem &lssys = pPanel.GetWindowProperties().GetWindowMain().GetLoadSaveSystem();
		decString filename(animator->GetPathAttachmentConfig());
		if(!igdeCommonDialogs::GetFileSave(&pPanel, "Save Attachment Configuration",
		*pPanel.GetEnvironment().GetFileSystemGame(), lssys.GetAttConfigFilePatterns(), filename ) ){
			return;
		}
		
		animator->SetPathAttachmentConfig(filename);
		lssys.SaveAttConfig(filename, *animator);
	}
};

class cActionMenuAttach : public igdeActionContextMenu{
	aeWPView &pPanel;
public:
	using Ref = deTObjectReference<cActionMenuAttach>;
	
	cActionMenuAttach(aeWPView &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Reset animation"),
	pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(contextMenu, cActionAttachmentAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionAttachmentRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionAttachmentClear::Ref::New(pPanel));
		
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionAttachmentLoadConfig::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionAttachmentSaveConfig::Ref::New(pPanel));
	}
};

class cTextAttachmentName : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextAttachmentName>;
	cTextAttachmentName(aeWPView &panel) : cBaseTextField(panel){}
	
	void OnChanged(igdeTextField *textField, aeAnimator *animator) override{
		aeAttachment * const attachment = pPanel.GetAttachment();
		if(!attachment || textField->GetText() == attachment->GetName()){
			return;
		}
		
		if(animator->GetAttachmentNamed(textField->GetText())){
			textField->SetText(attachment->GetName());
			
		}else{
			attachment->SetName(textField->GetText());
		}
	}
};

class cComboAttachmentBone : public cBaseComboBox{
public:
	using Ref = deTObjectReference<cComboAttachmentBone>;
	cComboAttachmentBone(aeWPView &panel) : cBaseComboBox(panel){}
	
	void OnChanged(igdeComboBox *comboBox, aeAnimator*) override{
		aeAttachment * const attachment = pPanel.GetAttachment();
		if(attachment){
			attachment->SetBoneName(comboBox->GetText());
		}
	}
};

class cComboAttachmentType : public cBaseComboBox{
public:
	using Ref = deTObjectReference<cComboAttachmentType>;
	cComboAttachmentType(aeWPView &panel) : cBaseComboBox(panel){}
	
	void OnChanged(igdeComboBox *comboBox, aeAnimator*) override{
		aeAttachment * const attachment = pPanel.GetAttachment();
		if(attachment && comboBox->GetSelectedItem()){
			attachment->SetAttachType((aeAttachment::eAttachTypes)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cEditAttachmentObject : public cBaseAction{
public:
	using Ref = deTObjectReference<cEditAttachmentObject>;
	
public:
	cEditAttachmentObject(aeWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(aeAnimator *animator) override{
		if(pPanel.GetAttachment()){
			animator->NotifyAttachmentChanged(pPanel.GetAttachment());
		}
	}
};

}



// Class aeWPView
///////////////////

// Constructor, destructor
////////////////////////////

aeWPView::aeWPView(aeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = aeWPViewListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// controllers
	helper.GroupBox(content, groupBox, "Display File Path:");
	
	helper.EditPath(groupBox, "Model:", "Sets the model to use", igdeEnvironment::efpltModel,
		pEditDisplayModelPath, cPathDisplayModel::Ref::New(*this));
	helper.EditPath(groupBox, "Skin:", "Sets the skin to use", igdeEnvironment::efpltSkin,
		pEditDisplaySkinPath, cPathDisplaySkin::Ref::New(*this));
	helper.EditPath(groupBox, "Rig:", "Sets the rig to use", igdeEnvironment::efpltRig,
		pEditDisplayRigPath, cPathDisplayRig::Ref::New(*this));
	
	
	// testing animator
	helper.GroupBox(content, groupBox, "Testing Base Animator:", true);
	
	helper.EditPath(groupBox, "Animator:",
		"Path to a testing animator to use as the base state for the editing animator",
		igdeEnvironment::efpltAnimator, pEditTestingAnimatorPath, cPathTestingAnimator::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkResetState, cActionResetState::Ref::New(*this));
	
	
	// sky, environment object, camera
	helper.WPSky(content, pWPSky, cEditSky::Ref::New(*this), "Sky:", false, true);
	helper.WPWObject(content, pWPEnvObject, cEditEnvObject::Ref::New(*this), "Environment Object:", false, true);
	helper.WPCamera(content, pWPCamera, cEditCamera::Ref::New(*this), "Camera:", false, true);
	
	
	// camera attaching
	helper.GroupBox(content, groupBox, "Camera Attaching:");
	
	helper.CheckBox(groupBox, pChkCamAttach, cActionCamAttach::Ref::New(*this));
	helper.ComboBoxFilter(groupBox, "Bone:", true, "Bone the camera is attached to",
		pCBCamBone, cComboCamAttachBone::Ref::New(*this));
	pCBCamBone->SetDefaultSorter();
	
	helper.EditVector(groupBox, "Position:",
		"Camera position relative to the attachment bone coordinate system",
		pEditCamRelPos, cEditCamRelPos::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:",
		"Camera rotation relative to the attachment bone coordinate system",
		pEditCamRelRot, cEditCamRelRot::Ref::New(*this));
	
	
	// playback
	helper.GroupBox(content, groupBox, "Playback:");
	
	helper.EditFloat(groupBox, "Play Speed:", "Sets the playback speed",
		pEditPlaySpeed, cTextPlaySpeed::Ref::New(*this));
	helper.EditFloat(groupBox, "Time Step:", "Provides fine grained animation stepping",
		pEditTimeStep, cTextTimeStep::Ref::New(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnFramePrev, cActionFramePrev::Ref::New(*this));
	helper.Button(formLine, pBtnPauseAnimation, cActionPaused::Ref::New(*this));
	helper.Button(formLine, pBtnFrameNext, cActionFrameNext::Ref::New(*this));
	helper.Button(formLine, pBtnResetAnimation, cActionReset::Ref::New(*this));
	
	
	// attachments
	helper.GroupBox(content, groupBox, "Attachments:", true);
	
	helper.FormLineStretchFirst(groupBox, "Attachment:", "Attachment to edit", formLine);
	helper.ComboBox(formLine, "Attachment to edit", pCBAttachments, cComboAttachment::Ref::New(*this));
	
	cActionMenuAttach::Ref actionMenuAttachment(cActionMenuAttach::Ref::New(*this));
	helper.Button(formLine, pBtnAttMenu, actionMenuAttachment);
	actionMenuAttachment->SetWidget(pBtnAttMenu);
	
	helper.EditString(groupBox, "Name:", "Sets the name of the attachment",
		pEditAttName, cTextAttachmentName::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "Attach Bone:", true, "Bone to attach to instead of the entire rig",
		pCBAttBoneName, cComboAttachmentBone::Ref::New(*this));
	pCBAttBoneName->SetDefaultSorter();
	
	helper.ComboBox(groupBox, "Attach:", "Attach type", pCBAttAttachType, cComboAttachmentType::Ref::New(*this));
	pCBAttAttachType->AddItem("None", nullptr, (void*)(intptr_t)aeAttachment::eatNone);
	pCBAttAttachType->AddItem("Bone", nullptr, (void*)(intptr_t)aeAttachment::eatBone);
	pCBAttAttachType->AddItem("Rig", nullptr, (void*)(intptr_t)aeAttachment::eatRig);
	
	helper.WPWObject(content, pWPAttachment, cEditAttachmentObject::Ref::New(*this),
		"Attachment Object:", false, true);
}

aeWPView::~aeWPView(){
	SetAnimator(nullptr);
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
		pAnimator = nullptr;
	}
}



// Management
///////////////

void aeWPView::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	pWPSky->SetSky(nullptr);
	pWPEnvObject->SetObject(nullptr);
	pWPCamera->SetCamera(nullptr);
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
		pWPSky->SetSky(animator->GetSky());
		pWPEnvObject->SetObject(animator->GetEnvObject());
		pWPCamera->SetCamera(animator->GetCamera());
	}
	
	UpdateBoneList();
	UpdatePath();
	UpdateCamera();
	UpdateCameraView();
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdatePlayback();
	UpdateAttachmentList();
	OnAnimatorPathChanged();
}

aeAttachment *aeWPView::GetAttachment() const{
	return pAnimator ? pAnimator->GetActiveAttachment().Pointer() : nullptr;
}

void aeWPView::SelectActiveAttachment(){
	pCBAttachments->SetSelectionWithData(GetAttachment());
}

void aeWPView::UpdateBoneList(){
	const decString cameraBone(pCBCamBone->GetText());
	const decString attachBone(pCBAttBoneName->GetText());
	
	pCBCamBone->RemoveAllItems();
	pCBAttBoneName->RemoveAllItems();
	
	if(pAnimator){
		const deRig * const engRig = pAnimator->GetEngineComponent()
			? pAnimator->GetEngineComponent()->GetRig().Pointer() : nullptr;
		if(engRig){
			const int count = engRig->GetBoneCount();
			int i;
			
			for(i=0; i<count; i++){
				const decString &name = engRig->GetBoneAt(i)->GetName();
				pCBCamBone->AddItem(name);
				pCBAttBoneName->AddItem(name);
			}
		}
		
		pCBCamBone->SortItems();
		pCBAttBoneName->SortItems();
	}
	
	pCBCamBone->StoreFilterItems();
	pCBAttBoneName->StoreFilterItems();
	
	pCBCamBone->SetText(cameraBone);
	pCBAttBoneName->SetText(attachBone);
}

void aeWPView::UpdatePath(){
	if(pAnimator){
		pEditDisplayModelPath->SetPath(pAnimator->GetDisplayModelPath());
		pEditDisplaySkinPath->SetPath(pAnimator->GetDisplaySkinPath());
		pEditDisplayRigPath->SetPath(pAnimator->GetDisplayRigPath());
		
	}else{
		pEditDisplayModelPath->ClearPath();
		pEditDisplaySkinPath->ClearPath();
		pEditDisplayRigPath->ClearPath();
	}
	
	const bool enabled = pAnimator;
	pEditDisplayModelPath->SetEnabled(enabled);
	pEditDisplaySkinPath->SetEnabled(enabled);
	pEditDisplayRigPath->SetEnabled(enabled);
}

void aeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void aeWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void aeWPView::UpdateCamera(){
	const aeCamera * const camera = pAnimator ? pAnimator->GetCamera() : nullptr;
	
	pWPCamera->UpdateCamera();
	
	if(camera){
		pCBCamBone->SetText(camera->GetBone());
		pEditCamRelPos->SetVector(camera->GetRelativePosition());
		pEditCamRelRot->SetVector(camera->GetRelativeOrientation());
		
	}else{
		pCBCamBone->ClearText();
		pEditCamRelPos->SetVector(decVector());
		pEditCamRelRot->SetVector(decVector());
	}
	
	const bool enabled = camera;
	pCBCamBone->SetEnabled(enabled);
	pEditCamRelPos->SetEnabled(enabled);
	pEditCamRelRot->SetEnabled(enabled);
	
	pChkCamAttach->GetAction()->Update();
}

void aeWPView::UpdateCameraView(){
	pWPCamera->UpdateViewDirection();
}

void aeWPView::UpdateView(){
	if(pAnimator){
		pEditTestingAnimatorPath->SetPath(pAnimator->GetTestingSubAnimator()->GetPathAnimator());
		
	}else{
		pEditTestingAnimatorPath->ClearPath();
	}
	
	const bool enabled = pAnimator;
	pEditTestingAnimatorPath->SetEnabled(enabled);
	
	pChkResetState->GetAction()->Update();
}

void aeWPView::UpdatePlayback(){
	if(pAnimator){
		pEditPlaySpeed->SetFloat(pAnimator->GetPlaySpeed());
		pEditTimeStep->SetFloat(pAnimator->GetTimeStep());
		
	}else{
		pEditPlaySpeed->ClearText();
		pEditTimeStep->ClearText();
	}
	
	const bool enabled = pAnimator;
	pEditPlaySpeed->SetEnabled(enabled);
	pEditTimeStep->SetEnabled(enabled);
}

void aeWPView::UpdateAttachmentList(){
	pCBAttachments->UpdateRestoreSelection([&](){
		pCBAttachments->RemoveAllItems();
		
		if(pAnimator){
			pAnimator->GetAttachments().Visit([&](aeAttachment *attachment){
				pCBAttachments->AddItem(attachment->GetName(), nullptr, attachment);
			});
			
			pCBAttachments->SortItems();
		}
	}, 0);
	
	UpdateAttachment();
}

void aeWPView::UpdateAttachment(){
	const aeAttachment * const attachment = GetAttachment();
	
	if(attachment){
		pEditAttName->SetText(attachment->GetName());
		pWPAttachment->SetObject(attachment->GetObjectWrapper());
		pCBAttAttachType->SetSelectionWithData((void*)(intptr_t)attachment->GetAttachType());
		pCBAttBoneName->SetText(attachment->GetBoneName());
		
	}else{
		pEditAttName->ClearText();
		pWPAttachment->SetObject(nullptr);
		pCBAttAttachType->SetSelectionWithData((void*)(intptr_t)aeAttachment::eatNone);
		pCBAttBoneName->ClearText();
	}
	
	const bool enabled = attachment;
	pEditAttName->SetEnabled(enabled);
	pCBAttAttachType->SetEnabled(enabled);
	pCBAttBoneName->SetEnabled(enabled);
}

void aeWPView::OnAnimatorPathChanged(){
	if(pAnimator){
		pEditDisplayModelPath->SetBasePath(pAnimator->GetDirectoryPath());
		pEditDisplaySkinPath->SetBasePath(pAnimator->GetDirectoryPath());
		pEditDisplayRigPath->SetBasePath(pAnimator->GetDirectoryPath());
		
	}else{
		pEditDisplayModelPath->SetBasePath("");
		pEditDisplaySkinPath->SetBasePath("");
		pEditDisplayRigPath->SetBasePath("");
	}
}
