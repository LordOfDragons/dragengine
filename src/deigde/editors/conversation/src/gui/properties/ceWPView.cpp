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

#include "ceWPView.h"
#include "ceWPViewListener.h"
#include "ceWindowProperties.h"
#include "../ceWindowMain.h"
#include "../../configuration/ceConfiguration.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/actor/ceConversationActor.h"
#include "../../conversation/actor/controller/ceActorController.h"
#include "../../conversation/actor/pose/ceActorPose.h"
#include "../../conversation/actor/gesture/ceActorGesture.h"
#include "../../conversation/coordsystem/ceCoordSystem.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/playback/cePlayback.h"
#include "../../conversation/playback/command/cePlaybackCommand.h"
#include "../../conversation/prop/ceProp.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/browse/igdeDialogBrowserObjectClass.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPTriggerTable.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decGlobalFunctions.h>



// Actions
////////////

class cVariableKey : public deObject{
public:
	typedef deTObjectReference<cVariableKey> Ref;
	decString name;
	cVariableKey(const decString &n) : name(n){}
};

namespace{

class cBaseAction : public igdeAction{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(ceWPView &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description), pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetConversation();
		if(!conversation){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(conversation));
		if(undo){
			conversation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(ceConversation *conversation) = 0;
	
	void Update() override{
		const ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			Update(*conversation);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const ceConversation &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseActionContextMenu : public igdeActionContextMenu{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseActionContextMenu> Ref;
	cBaseActionContextMenu(ceWPView &panel, const char *description) :
	igdeActionContextMenu("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
	description), pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			AddContextMenuEntries(contextMenu, conversation);
		}
	}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation *conversation) = 0;
	
	void Update() override{
		const ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			Update(*conversation);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const ceConversation &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			igdeUndo::Ref undo(OnChanged(*comboBox, conversation));
			if(undo){
				conversation->GetUndoSystem()->Add(undo);
			}
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			igdeUndo::Ref undo(OnChanged(*textField, conversation));
			if(undo){
				conversation->GetUndoSystem()->Add(undo);
			}
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation *conversation) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			igdeUndo::Ref undo(OnChanged(*editVector, conversation));
			if(undo){
				conversation->GetUndoSystem()->Add(undo);
			}
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation *conversation) = 0;
};

class cBaseSpinTextFieldListener : public igdeSpinTextFieldListener{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseSpinTextFieldListener> Ref;
	cBaseSpinTextFieldListener(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			igdeUndo::Ref undo(OnChanged(*textField, conversation));
			if(undo){
				conversation->GetUndoSystem()->Add(undo);
			}
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeSpinTextField &textField, ceConversation *conversation) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	ceWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditPathListener> Ref;
	cBaseEditPathListener(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			igdeUndo::Ref undo(OnChanged(*editPath, conversation));
			if(undo){
				conversation->GetUndoSystem()->Add(undo);
			}
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation *conversation) = 0;
};



class cActionWPCamera : public cBaseAction{
public:
	typedef deTObjectReference<cActionWPCamera> Ref;
	
public:
	cActionWPCamera(ceWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->NotifyCameraChanged();
		return {};
	}
};

class cActionWPSky : public cBaseAction{
public:
	typedef deTObjectReference<cActionWPSky> Ref;
	
public:
	cActionWPSky(ceWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->NotifySkyChanged();
		return {};
	}
};

class cActionWPEnvObject : public cBaseAction{
public:
	typedef deTObjectReference<cActionWPEnvObject> Ref;
	
public:
	cActionWPEnvObject(ceWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->NotifyEnvObjectChanged();
		return {};
	}
};



class cComboProp : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboProp> Ref;
	cComboProp(ceWPView &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) override{
		conversation->SetActiveProp(comboBox.GetSelectedItem()
			? (ceProp*)comboBox.GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};

class cActionPropAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropAdd> Ref;
	
public:
	cActionPropAdd(ceWPView &panel) : cBaseAction(panel, "Add",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Prop"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		const ceProp::Ref prop(ceProp::Ref::New());
		conversation->AddProp(prop);
		conversation->SetActiveProp(prop);
		return {};
	}
};

class cActionPropRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropRemove> Ref;
	
public:
	cActionPropRemove(ceWPView &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Prop"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetProp()){
			conversation->RemoveProp(pPanel.GetProp());
			if(conversation->GetProps().IsNotEmpty()){
				conversation->SetActiveProp(conversation->GetProps().First());
			}
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetProp());
	}
};

class cActionPropMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionPropMenu> Ref;
	cActionPropMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "Prop menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionPropAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionPropRemove::Ref::New(pPanel));
	}
};

class cTextPropName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPropName> Ref;
	cTextPropName(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetProp()){
			pPanel.GetProp()->SetName(textField.GetText());
		}
		return {};
	}
};

class cTextPropClass : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPropClass> Ref;
	cTextPropClass(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetProp()){
			pPanel.GetProp()->SetObjectClass(textField.GetText());
		}
		return {};
	}
};

class cActionPropClass : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropClass> Ref;
	
private:
	igdeTextField::Ref &pTextField;
	
public:
	cActionPropClass(ceWPView &panel, igdeTextField::Ref &textField) : cBaseAction(panel, "",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Brings up a dialog to select the prop class"), pTextField(textField){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		if(!pPanel.GetProp() || !pPanel.GetGameDefinition()){
			return {};
		}
		
		igdeGDClass *gdClass = pPanel.GetGameDefinition()->GetClassManager()->GetNamed(pPanel.GetProp()->GetObjectClass());
		if(igdeDialogBrowserObjectClass::SelectObjectClass(&pPanel, gdClass)){
			pPanel.GetProp()->SetObjectClass(gdClass->GetName());
		}
		return {};
	}
};

class cActionPropVisible : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropVisible> Ref;
	
public:
	cActionPropVisible(ceWPView &panel) : cBaseAction(panel, "Visible", nullptr, "Determines if the prop is visible"){ }
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		if(pPanel.GetProp()){
			pPanel.GetProp()->SetVisible(!pPanel.GetProp()->GetVisible());
		}
		return {};
	}
};

class cVectorPropPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorPropPosition> Ref;
	cVectorPropPosition(ceWPView &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		if(pPanel.GetProp()){
			pPanel.GetProp()->SetPosition(editVector.GetVector());
		}
		return {};
	}
};

class cVectorPropRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorPropRotation> Ref;
	cVectorPropRotation(ceWPView &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		if(pPanel.GetProp()){
			pPanel.GetProp()->SetOrientation(editVector.GetVector());
		}
		return {};
	}
};



class cSpinActor : public cBaseSpinTextFieldListener{
public:
	typedef deTObjectReference<cSpinActor> Ref;
	cSpinActor(ceWPView &panel) : cBaseSpinTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeSpinTextField &textField, ceConversation *conversation) override{
		conversation->SetActiveActor(conversation->GetActorList().GetAt(textField.GetValue()));
		return {};
	}
};

class cActionActorAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorAdd> Ref;
	
public:
	cActionActorAdd(ceWPView &panel) : cBaseAction(panel, "Add",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Actor"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		const ceConversationActor::Ref actor(ceConversationActor::Ref::New(pPanel.GetEnvironment()));
		conversation->AddActor(actor);
		conversation->SetActiveActor(actor);
		return {};
	}
};

class cActionActorRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorRemove> Ref;
	
public:
	cActionActorRemove(ceWPView &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Actor"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActor()){
			conversation->RemoveActor(pPanel.GetActor());
			if(conversation->GetActorList().GetCount() > 0){
				conversation->SetActiveActor(conversation->GetActorList().GetAt(0));
			}
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActor());
	}
};

class cActionActorMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionActorMenu> Ref;
	cActionActorMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "Actor menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionActorAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorRemove::Ref::New(pPanel));
	}
};

class cTextActorID : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextActorID> Ref;
	cTextActorID(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetID(textField.GetText());
		}
		return {};
	}
};

class cTextActorAliasID : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextActorAliasID> Ref;
	cTextActorAliasID(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetAliasID(textField.GetText());
		}
		return {};
	}
};

class cVectorActorPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorActorPosition> Ref;
	cVectorActorPosition(ceWPView &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPosition(editVector.GetVector());
		}
		return {};
	}
};

class cVectorActorRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorActorRotation> Ref;
	cVectorActorRotation(ceWPView &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetOrientation(editVector.GetVector());
		}
		return {};
	}
};

class cPathActorModel : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorModel> Ref;
	cPathActorModel(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPathModel(editPath.GetPath());
		}
		return {};
	}
};

class cPathActorSkin : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorSkin> Ref;
	cPathActorSkin(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPathSkin(editPath.GetPath());
		}
		return {};
	}
};

class cPathActorRig : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorRig> Ref;
	cPathActorRig(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPathRig(editPath.GetPath());
		}
		return {};
	}
};

class cPathActorSpeechAnimation : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorSpeechAnimation> Ref;
	cPathActorSpeechAnimation(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPathSpeechAnimation(editPath.GetPath());
		}
		return {};
	}
};

class cPathActorEyeAnimator : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorEyeAnimator> Ref;
	cPathActorEyeAnimator(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPathEyesAnimator(editPath.GetPath());
		}
		return {};
	}
};

class cPathActorFacePoseAnimator : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorFacePoseAnimator> Ref;
	cPathActorFacePoseAnimator(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetPathFacePoseAnimator(editPath.GetPath());
		}
		return {};
	}
};

class cActionActorWaiting : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorWaiting> Ref;
	
public:
	cActionActorWaiting(ceWPView &panel) : cBaseAction(panel, "Waiting", nullptr,
	"Simulates the actor game scripts answer to the question if conversation should be waiting"){ }
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetWaiting(!pPanel.GetActor()->GetWaiting());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActor());
		SetSelected(pPanel.GetActor() && pPanel.GetActor()->GetWaiting());
	}
};

class cTextActorBoneHeadRotator : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextActorBoneHeadRotator> Ref;
	cTextActorBoneHeadRotator(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetActor()){
			pPanel.GetActor()->SetBoneHeadRotator(textField.GetText());
		}
		return {};
	}
};


class cComboActorPose : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboActorPose> Ref;
	cComboActorPose(ceWPView &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation*) override{
		if(pPanel.GetActor() && comboBox.GetSelectedItem()){
			pPanel.GetActor()->SetActivePose((ceActorPose*)comboBox.GetSelectedItem()->GetData());
		}
		return {};
	}
};

class cActionActorPoseAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseAdd> Ref;
	
public:
	cActionActorPoseAdd(ceWPView &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Actor Pose"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceConversationActor * const actor = pPanel.GetActor();
		if(!actor){
			return {};
		}
		
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Pose", "Pose:", name)){
			return {};
		}
		
		if(actor->GetPoses().HasMatching([&](const ceActorPose::Ref &p){ return p->GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Pose", "A pose with this name exists already.");
			return {};
		}
		
		const ceActorPose::Ref pose(ceActorPose::Ref::New(pPanel.GetEnvironment(), name));
		actor->GetPoses().Add(pose);
		conversation->NotifyActorPosesChanged(actor);
		actor->SetActivePose(pose);
		conversation->NotifyActorActivePoseChanged(actor);
		return {};
	}
};

class cActionActorPoseRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseRemove> Ref;
	
public:
	cActionActorPoseRemove(ceWPView &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Actor Pose"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActorPose()){
			pPanel.GetActor()->GetPoses().Remove(pPanel.GetActorPose());
			conversation->NotifyActorPosesChanged(pPanel.GetActor());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorPose());
	}
};

class cActionActorPoseRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseRename> Ref;
	
public:
	cActionActorPoseRename(ceWPView &panel) : cBaseAction(panel, "Rename...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Rename Actor Pose"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceConversationActor * const actor = pPanel.GetActor();
		ceActorPose * const pose = pPanel.GetActorPose();
		if(!pose || !actor){
			return {};
		}
		
		decString name(pose->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Pose", "Pose:", name) || name == pose->GetName()){
			return {};
		}
		
		if(actor->GetPoses().HasMatching([&](const ceActorPose::Ref &p){ return p->GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Pose", "A pose with this name exists already.");
			
		}else{
			pose->SetName(name);
			actor->NotifyPosesChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorPose());
	}
};

class cActionActorPoseMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionActorPoseMenu> Ref;
	cActionActorPoseMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "ActorPose menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionActorPoseAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorPoseRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorPoseRename::Ref::New(pPanel));
	}
};

class cPathActorPoseAnimator : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorPoseAnimator> Ref;
	cPathActorPoseAnimator(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActorPose()){
			pPanel.GetActorPose()->SetPathAnimator(editPath.GetPath());
		}
		return {};
	}
};


class cComboActorPoseController : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboActorPoseController> Ref;
	cComboActorPoseController(ceWPView &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox&, ceConversation*) override{
		pPanel.UpdateActorController();
		return {};
	}
};

class cActionActorPoseControllerAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseControllerAdd> Ref;
	
public:
	cActionActorPoseControllerAdd(ceWPView &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add actor pose controller"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceActorPose * const pose = pPanel.GetActorPose();
		if(!pose){
			return {};
		}
		
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Controller", "Controller:", name, pose->GetControllerNames())){
			return {};
		}
		
		if(pose->GetControllers().HasMatching([&](const ceActorController::Ref &c){ return c->GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Controller", "A controller with this name exists already.");
			return {};
		}
		
		const ceActorController::Ref controller(ceActorController::Ref::New());
		controller->SetName(name);
		pose->GetControllers().Add(controller);
		conversation->NotifyActorPosesChanged(pPanel.GetActor());
		pPanel.SelectActorPoseController(controller);
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorPose());
	}
};

class cActionActorPoseControllerRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseControllerRemove> Ref;
	
public:
	cActionActorPoseControllerRemove(ceWPView &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove actor pose controller"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActorPoseController()){
			pPanel.GetActorPose()->GetControllers().Remove(pPanel.GetActorPoseController());
			conversation->NotifyActorPosesChanged(pPanel.GetActor());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorPoseController());
	}
};

class cActionActorPoseControllerRemoveAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseControllerRemoveAll> Ref;
	
public:
	cActionActorPoseControllerRemoveAll(ceWPView &panel) : cBaseAction(panel, "Remove All",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all actor pose controllers"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActorPose() && pPanel.GetActorPose()->GetControllers().IsNotEmpty()){
			pPanel.GetActorPose()->GetControllers().RemoveAll();
			conversation->NotifyActorPosesChanged(pPanel.GetActor());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorPose() && pPanel.GetActorPose()->GetControllers().IsNotEmpty());
	}
};

class cActionActorPoseControllerRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorPoseControllerRename> Ref;
	
public:
	cActionActorPoseControllerRename(ceWPView &panel) : cBaseAction(panel, "Rename...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Rename Actor Pose Controller"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceActorController * const controller = pPanel.GetActorPoseController();
		if(!controller){
			return {};
		}
		
		decString name(controller->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Controller", "Controller:", name)
		|| name == controller->GetName()){
			return {};
		}
		
		if(pPanel.GetActorPose()->GetControllers().HasMatching([&](const ceActorController::Ref &c){ return c->GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Controller", "A controller with this name exists already.");
			
		}else{
			controller->SetName(name);
			pPanel.GetActor()->NotifyPosesChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorPoseController());
	}
};

class cActionActorPoseControllerMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionActorPoseControllerMenu> Ref;
	cActionActorPoseControllerMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "ActorPoseController menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionActorPoseControllerAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorPoseControllerRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorPoseControllerRemoveAll::Ref::New(pPanel));
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionActorPoseControllerRename::Ref::New(pPanel));
	}
};

class cComboActorPoseControllerUpdateType : public cBaseComboBoxListener{
	igdeComboBox::Ref &pCBController;
	
public:
	typedef deTObjectReference<cComboActorPoseControllerUpdateType> Ref;
	cComboActorPoseControllerUpdateType(ceWPView &panel, igdeComboBox::Ref &cbController) :
	cBaseComboBoxListener(panel), pCBController(cbController){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation*) override{
		ceActorController * const controller = pCBController->GetSelectedItem()
			? (ceActorController*)pCBController->GetSelectedItem()->GetData() : nullptr;
		
		if(controller && comboBox.GetSelectedItem()){
			controller->SetUpdateType((ceActorController::eUpdateTypes)(intptr_t)comboBox.GetSelectedItem()->GetData());
		}
		return {};
	}
};

class cTextActorPoseControllerValue : public cBaseTextFieldListener{
	igdeComboBox::Ref &pCBController;
	
public:
	typedef deTObjectReference<cTextActorPoseControllerValue> Ref;
	cTextActorPoseControllerValue(ceWPView &panel, igdeComboBox::Ref &cbController) :
	cBaseTextFieldListener(panel), pCBController(cbController){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceActorController * const controller = pCBController->GetSelectedItem()
			? (ceActorController*)pCBController->GetSelectedItem()->GetData() : nullptr;
		
		if(controller){
			controller->SetValue(textField.GetFloat());
		}
		return {};
	}
};

class cTextActorPoseControllerVector : public cBaseEditVectorListener{
	igdeComboBox::Ref &pCBController;
	
public:
	typedef deTObjectReference<cTextActorPoseControllerVector> Ref;
	cTextActorPoseControllerVector(ceWPView &panel, igdeComboBox::Ref &cbController) :
	cBaseEditVectorListener(panel), pCBController(cbController){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceActorController * const controller = pCBController->GetSelectedItem()
			? (ceActorController*)pCBController->GetSelectedItem()->GetData() : nullptr;
		
		if(controller){
			controller->SetVector(editVector.GetVector());
		}
		return {};
	}
};


class cComboActorGesture : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboActorGesture> Ref;
	cComboActorGesture(ceWPView &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation*) override{
		pPanel.SelectActorPoseGesture(comboBox.GetSelectedItem()
			? (ceActorGesture*)comboBox.GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};

class cActionActorGestureAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorGestureAdd> Ref;
	
public:
	cActionActorGestureAdd(ceWPView &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Actor Gesture"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceActorPose * const pose = pPanel.GetActorPose();
		if(!pose){
			return {};
		}
		
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Gesture", "Gesture:", name)){
			return {};
		}
		
		if(pose->GetGestures().HasMatching([&name](const ceActorGesture &g){ return g.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Gesture", "A gesture with this name exists already.");
			return {};
		}
		
		const ceActorGesture::Ref g(ceActorGesture::Ref::New(pPanel.GetEnvironment(), name));
		pose->GetGestures().Add(g);
		
		pPanel.GetActor()->NotifyPoseGesturesChanged(pose);
		pPanel.UpdateActorGestures();
		pPanel.SelectActorPoseGesture(g);
		return {};
	}
};

class cActionActorGestureRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorGestureRemove> Ref;
	
public:
	cActionActorGestureRemove(ceWPView &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Actor Gesture"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		if(pPanel.GetActorGesture()){
			pPanel.GetActorPose()->GetGestures().Remove(pPanel.GetActorGesture());
			pPanel.GetActor()->NotifyPoseGesturesChanged(pPanel.GetActorPose());
			pPanel.UpdateActorGestures();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorGesture());
	}
};

class cActionActorGestureRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorGestureRename> Ref;
	
public:
	cActionActorGestureRename(ceWPView &panel) : cBaseAction(panel, "Rename...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Rename Actor Gesture"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceActorPose * const pose = pPanel.GetActorPose();
		ceActorGesture * const gesture = pPanel.GetActorGesture();
		if(!gesture || !pose){
			return {};
		}
		
		decString name(gesture->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Gesture", "Gesture:", name) || name == gesture->GetName()){
			return {};
		}
		
		if(pose->GetGestures().HasMatching([&name](const ceActorGesture &g){ return g.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Gesture", "A gesture with this name exists already.");
			
		}else{
			gesture->SetName(name);
			pPanel.GetActor()->NotifyPoseGesturesChanged(pose);
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActorGesture());
	}
};

class cActionActorGestureMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionActorGestureMenu> Ref;
	cActionActorGestureMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "ActorGesture menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionActorGestureAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorGestureRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionActorGestureRename::Ref::New(pPanel));
	}
};

class cPathActorGestureAnimator : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathActorGestureAnimator> Ref;
	cPathActorGestureAnimator(ceWPView &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, ceConversation*) override{
		if(pPanel.GetActorGesture() && pPanel.GetActorGesture()->GetPathAnimator() != editPath.GetPath()){
			pPanel.GetActorGesture()->SetPathAnimator(editPath.GetPath());
			pPanel.GetActor()->NotifyPoseGesturesChanged(pPanel.GetActorPose());
		}
		return {};
	}
};


class cActionActorCommandAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorCommandAdd> Ref;
	
public:
	cActionActorCommandAdd(ceWPView &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Actor Command"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceConversationActor * const actor = pPanel.GetActor();
		if(!actor){
			return {};
		}
		
		decString command;
		if(!igdeCommonDialogs::GetMultilineString(&pPanel.GetWindowProperties().GetWindowMain(),
			"Add Command", "Command:", command)
		|| actor->GetCommands().HasMatching([&](const cePlaybackCommand::Ref &c){ return c->GetCommand() == command; })){
			return {};
		}
		
		const cePlaybackCommand::Ref pcommand(cePlaybackCommand::Ref::New(command, false));
		actor->GetCommands().Add(pcommand);
		conversation->NotifyActorCommandsChanged(actor);
		return {};
	}
};

class cActionActorCommandRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorCommandRemove> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionActorCommandRemove(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove Actor Command"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActor() && pListBox.GetSelectedItem()){
			pPanel.GetActor()->GetCommands().Remove((cePlaybackCommand*)pListBox.GetSelectedItem()->GetData());
			conversation->NotifyActorCommandsChanged(pPanel.GetActor());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActor() && pListBox.GetSelectedItem());
	}
};

class cActionActorCommandClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorCommandClear> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionActorCommandClear(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove All", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove All Actor Commands"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActor() && pListBox.GetItems().IsNotEmpty()){
			pPanel.GetActor()->GetCommands().RemoveAll();
			conversation->NotifyActorCommandsChanged(pPanel.GetActor());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActor() && pListBox.GetItems().IsNotEmpty());
	}
};

class cListActorCommands: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	typedef deTObjectReference<cListActorCommands> Ref;
	cListActorCommands(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeListBox *listBox, int index){
		if(pPanel.GetActor()){
			cePlaybackCommand &pcommand = *((cePlaybackCommand*)listBox->GetItems().GetAt(index)->GetData());
			pcommand.SetValue(!pcommand.GetValue());
			pPanel.GetConversation()->NotifyActorCommandsChanged(pPanel.GetActor());
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu){
		if(pPanel.GetActor()){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand(menu, cActionActorCommandAdd::Ref::New(pPanel));
			helper.MenuCommand(menu, cActionActorCommandRemove::Ref::New(pPanel, *listBox));
			helper.MenuCommand(menu, cActionActorCommandClear::Ref::New(pPanel, *listBox));
		}
	}
};


class cActionActorParameterSet : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorParameterSet> Ref;
	
public:
	typedef deTObjectReference<cVariableKey> cVariableKeyRef;
	cActionActorParameterSet(ceWPView &panel) : cBaseAction(panel, "Set...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Set Actor Parameter"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceConversationActor * const actor = pPanel.GetActor();
		if(!actor){
			return {};
		}
		
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Set Parameter", "Parameter:", name)){
			return {};
		}
		
		int value = actor->GetParameters().GetAtOrDefault(name, 0);
		if(!igdeCommonDialogs::GetInteger(&pPanel, "Set Parameter", "Value:", value)){
			return {};
		}
		
		actor->GetParameters().SetAt(name, value);
		conversation->NotifyActorParametersChanged(actor);
		return {};
	}
};

class cActionActorParameterRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorParameterRemove> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionActorParameterRemove(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove Actor Parameter"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActor() && pListBox.GetSelectedItem()){
			const cVariableKey::Ref key(pListBox.GetSelectedItemRefData().DynamicCast<cVariableKey>());
			if(key){
				pPanel.GetActor()->GetParameters().RemoveIfPresent(key->name);
				conversation->NotifyActorParametersChanged(pPanel.GetActor());
			}
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActor() && pListBox.GetSelectedItem());
	}
};

class cActionActorParameterClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionActorParameterClear> Ref;
	
public:
	cActionActorParameterClear(ceWPView &panel) : cBaseAction(panel, "Remove All",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove All Actor Parameters"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetActor() && pPanel.GetActor()->GetParameters().IsNotEmpty()){
			pPanel.GetActor()->GetParameters().RemoveAll();
			conversation->NotifyActorParametersChanged(pPanel.GetActor());
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetActor() && pPanel.GetActor()->GetParameters().IsNotEmpty());
	}
};

class cListActorParameters: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	typedef deTObjectReference<cListActorParameters> Ref;
	cListActorParameters(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeListBox *listBox, int){
		if(!pPanel.GetActor()){
			return;
		}
		
		const cVariableKey::Ref parameter(listBox->GetSelectedItemRefData().DynamicCast<cVariableKey>());
		int value = pPanel.GetActor()->GetParameters().GetAtOrDefault(parameter->name, 0);
		if(igdeCommonDialogs::GetInteger(&pPanel, "Set Parameter Value", "Value:", value)){
			pPanel.GetActor()->GetParameters().SetAt(parameter->name, value);
			pPanel.GetConversation()->NotifyActorParametersChanged(pPanel.GetActor());
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu){
		if(pPanel.GetActor()){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand(menu, cActionActorParameterSet::Ref::New(pPanel));
			helper.MenuCommand(menu, cActionActorParameterRemove::Ref::New(pPanel, *listBox));
			helper.MenuCommand(menu, cActionActorParameterClear::Ref::New(pPanel));
		}
	}
};


class cSpinCoordSys : public cBaseSpinTextFieldListener{
public:
	typedef deTObjectReference<cSpinCoordSys> Ref;
	cSpinCoordSys(ceWPView &panel) : cBaseSpinTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeSpinTextField &textField, ceConversation *conversation) override{
		conversation->SetActiveCoordSystem(conversation->GetCoordSystemList().GetAt(textField.GetValue()));
		return {};
	}
};

class cActionCoordSysAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionCoordSysAdd> Ref;
	
public:
	cActionCoordSysAdd(ceWPView &panel) : cBaseAction(panel, "Add",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Coord-System"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		const ceCoordSystem::Ref actor(ceCoordSystem::Ref::New());
		conversation->AddCoordSystem(actor);
		conversation->SetActiveCoordSystem(actor);
		return {};
	}
};

class cActionCoordSysRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionCoordSysRemove> Ref;
	
public:
	cActionCoordSysRemove(ceWPView &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Coord-System"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pPanel.GetCoordSys()){
			conversation->RemoveCoordSystem(pPanel.GetCoordSys());
			if(conversation->GetCoordSystemList().GetCount() > 0){
				conversation->SetActiveCoordSystem(conversation->GetCoordSystemList().GetAt(0));
			}
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetCoordSys());
	}
};

class cActionCoordSysMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionCoordSysMenu> Ref;
	cActionCoordSysMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "Coord-System menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionCoordSysAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionCoordSysRemove::Ref::New(pPanel));
	}
};

class cTextCoordSysID : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCoordSysID> Ref;
	cTextCoordSysID(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetCoordSys()){
			pPanel.GetCoordSys()->SetID(textField.GetText());
		}
		return {};
	}
};

class cTextCoordSysAliasID : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCoordSysAliasID> Ref;
	cTextCoordSysAliasID(ceWPView &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		if(pPanel.GetCoordSys()){
			pPanel.GetCoordSys()->SetAliasID(textField.GetText());
		}
		return {};
	}
};

class cVectorCoordSysPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCoordSysPosition> Ref;
	cVectorCoordSysPosition(ceWPView &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		if(pPanel.GetCoordSys()){
			pPanel.GetCoordSys()->SetPosition(editVector.GetVector());
		}
		return {};
	}
};

class cVectorCoordSysRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCoordSysRotation> Ref;
	cVectorCoordSysRotation(ceWPView &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		if(pPanel.GetCoordSys()){
			pPanel.GetCoordSys()->SetOrientation(editVector.GetVector());
		}
		return {};
	}
};



class cComboPlaybackFile : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPlaybackFile> Ref;
	cComboPlaybackFile(ceWPView &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox&, ceConversation*) override{
		pPanel.UpdatePlaybackTopicList();
		return {};
	}
};

class cActionPlaybackSelectTopic : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackSelectTopic> Ref;
	
public:
	cActionPlaybackSelectTopic(ceWPView &panel) : cBaseAction(panel, "Set",
	nullptr, "Set this topic as the one to play back"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		cePlayback &playback = *conversation->GetPlayback();
		playback.SetTopic(pPanel.GetSelectedPlaybackTopic());
		playback.Rewind();
		playback.SetRunning(false);
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetSelectedPlaybackTopic());
	}
};

class cActionPlaybackRewind : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackRewind> Ref;
	
public:
	cActionPlaybackRewind(ceWPView &panel) : cBaseAction(panel, "Rewind",
	nullptr, "Rewind the playback to the beginning of the selected topic"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->GetPlayback()->Rewind();
		return {};
	}
};

class cActionPlaybackRunning : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackRunning> Ref;
	
public:
	cActionPlaybackRunning(ceWPView &panel) : cBaseAction(panel, "Running",
	nullptr, "Determines if the topic is currently played back"){ }
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->GetPlayback()->SetRunning(!conversation->GetPlayback()->GetRunning());
		return {};
	}
	
	void Update(const ceConversation &conversation) override{
		SetEnabled(conversation.GetPlayback()->GetTopic());
		SetSelected(conversation.GetPlayback()->GetRunning());
	}
};

class cActionPlaybackPaused : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackPaused> Ref;
	
public:
	cActionPlaybackPaused(ceWPView &panel) : cBaseAction(panel, "Paused",
	nullptr, "Determines if the played back is paused"){ }
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->GetPlayback()->SetPaused(!conversation->GetPlayback()->GetPaused());
		return {};
	}
	
	void Update(const ceConversation &conversation) override{
		SetEnabled(conversation.GetPlayback()->GetTopic());
		SetSelected(conversation.GetPlayback()->GetPaused());
	}
};

class cActionPlaybackAutoAdvanceCommands : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackAutoAdvanceCommands> Ref;
	
public:
	cActionPlaybackAutoAdvanceCommands(ceWPView &panel) : cBaseAction(panel, "Auto Advance",
	nullptr, "Auto advance certain commands (game/actor commands, trigger, add/remove actor/coordsystem)"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->GetPlayback()->SetAutoAdvanceCommands(!conversation->GetPlayback()->GetAutoAdvanceCommands());
		return {};
	}
	
	void Update(const ceConversation &conversation) override{
		SetEnabled(true);
		SetSelected(conversation.GetPlayback()->GetAutoAdvanceCommands());
	}
};

class cComboPlaybackCameraHandling : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPlaybackCameraHandling> Ref;
	cComboPlaybackCameraHandling(ceWPView &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) override{
		if(comboBox.GetSelectedItem()){
			conversation->GetPlayback()->SetCameraHandling(
				(cePlayback::eCameraHandling)(intptr_t)comboBox.GetSelectedItem()->GetData());
		}
		return {};
	}
};


class cActionPlaybackCommandAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackCommandAdd> Ref;
	
public:
	cActionPlaybackCommandAdd(ceWPView &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Playback Command"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		decString command;
		if(!igdeCommonDialogs::GetMultilineString(&pPanel.GetWindowProperties().GetWindowMain(),
			"Add Command", "Command:", command)
		|| conversation->GetPlayback()->GetCommands().HasMatching([&](const cePlaybackCommand::Ref &c){ return c->GetCommand() == command; })){
			return {};
		}
		
		const cePlaybackCommand::Ref pcommand(cePlaybackCommand::Ref::New(command, false));
		conversation->GetPlayback()->GetCommands().Add(pcommand);
		conversation->NotifyPlaybackCommandListChanged();
		return {};
	}
};

class cActionPlaybackCommandRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackCommandRemove> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionPlaybackCommandRemove(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove Playback Command"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pListBox.GetSelectedItem()){
			conversation->GetPlayback()->GetCommands().Remove(
				(cePlaybackCommand*)pListBox.GetSelectedItem()->GetData());
			conversation->NotifyPlaybackCommandListChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pListBox.GetSelectedItem());
	}
};

class cActionPlaybackCommandClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackCommandClear> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionPlaybackCommandClear(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove All", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove All Playback Commands"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pListBox.GetItems().IsNotEmpty()){
			conversation->GetPlayback()->GetCommands().RemoveAll();
			conversation->NotifyPlaybackCommandListChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pListBox.GetItems().IsNotEmpty());
	}
};

class cListPlaybackCommands: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	typedef deTObjectReference<cListPlaybackCommands> Ref;
	cListPlaybackCommands(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeListBox *listBox, int index){
		if(pPanel.GetConversation()){
			cePlaybackCommand &pcommand = *((cePlaybackCommand*)listBox->GetItems().GetAt(index)->GetData());
			pcommand.SetValue(!pcommand.GetValue());
			pPanel.GetConversation()->NotifyPlaybackCommandListChanged();
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu){
		if(pPanel.GetConversation()){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand(menu, cActionPlaybackCommandAdd::Ref::New(pPanel));
			helper.MenuCommand(menu, cActionPlaybackCommandRemove::Ref::New(pPanel, *listBox));
			helper.MenuCommand(menu, cActionPlaybackCommandClear::Ref::New(pPanel, *listBox));
		}
	}
};


class cActionPlaybackVariableAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackVariableAdd> Ref;
	
public:
	cActionPlaybackVariableAdd(ceWPView &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Playback Variable"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		decString variable;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Variable", "Variable:", variable)
		|| conversation->GetPlayback()->GetVariables().Has(variable)){
			return {};
		}
		
		conversation->GetPlayback()->GetVariables().SetAt(variable, 0);
		conversation->NotifyPlaybackVarListChanged();
		return {};
	}
};

class cActionPlaybackVariableSet : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackVariableSet> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionPlaybackVariableSet(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Set...", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"Set Playback Variable"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(!pListBox.GetSelectedItem()){
			return {};
		}
		
		const decString name(pListBox.GetSelectedItemRefData().DynamicCast<cVariableKey>()->name);
		cePlayback::VariableMap &variables = conversation->GetPlayback()->GetVariables();
		const int curValue = variables.GetAtOrDefault(name, 0);
		int value = curValue;
		if(igdeCommonDialogs::GetInteger(&pPanel, "Set Variable", "Value:", value) && value != curValue){
			variables.SetAt(name, value);
			conversation->NotifyPlaybackVarListChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pListBox.GetSelectedItem());
	}
};

class cActionPlaybackVariableRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackVariableRemove> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionPlaybackVariableRemove(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove Playback Variable"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pListBox.GetSelectedItem()){
			conversation->GetPlayback()->GetVariables().Remove(
				pListBox.GetSelectedItemRefData().DynamicCast<cVariableKey>()->name);
			conversation->NotifyPlaybackVarListChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pListBox.GetSelectedItem());
	}
};

class cActionPlaybackVariableClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackVariableClear> Ref;
	
private:
	igdeListBox &pListBox;
	
public:
	cActionPlaybackVariableClear(ceWPView &panel, igdeListBox &listBox) :
	cBaseAction(panel, "Remove All", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove All Playback Variables"), pListBox(listBox){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		if(pListBox.GetItems().IsNotEmpty()){
			conversation->GetPlayback()->GetVariables().RemoveAll();
			conversation->NotifyPlaybackVarListChanged();
		}
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pListBox.GetItems().IsNotEmpty());
	}
};

class cListPlaybackVariables: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	typedef deTObjectReference<cListPlaybackVariables> Ref;
	cListPlaybackVariables(ceWPView &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeListBox *listBox, int){
		if(!pPanel.GetConversation() || !listBox->GetSelectedItem()){
			return;
		}
		
		const decString name(listBox->GetSelectedItemRefData().DynamicCast<cVariableKey>()->name);
		cePlayback::VariableMap &variables = pPanel.GetConversation()->GetPlayback()->GetVariables();
		const int curValue = variables.GetAtOrDefault(name, 0);
		int value = curValue;
		if(igdeCommonDialogs::GetInteger(&pPanel, "Set Variable", "Value:", value) && value != curValue){
			variables.SetAt(name, value);
			pPanel.GetConversation()->NotifyPlaybackVarListChanged();
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu){
		if(pPanel.GetConversation()){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand(menu, cActionPlaybackVariableAdd::Ref::New(pPanel));
			helper.MenuCommand(menu, cActionPlaybackVariableSet::Ref::New(pPanel, *listBox));
			helper.MenuCommand(menu, cActionPlaybackVariableRemove::Ref::New(pPanel, *listBox));
			helper.MenuCommand(menu, cActionPlaybackVariableClear::Ref::New(pPanel, *listBox));
		}
	}
};



class cActionPlaybackTriggerTable : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaybackTriggerTable> Ref;
	
public:
	cActionPlaybackTriggerTable(ceWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->NotifyPlaybackTriggerTableChanged();
		return {};
	}
};



class cActionMissingWordsCopy : public cBaseAction{
public:
	typedef deTObjectReference<cActionMissingWordsCopy> Ref;
	
public:
	cActionMissingWordsCopy(ceWPView &panel) : cBaseAction(panel, "Copy To Clipboard",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy missing words found during playback to clipboard"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		const decStringSet &list = conversation->GetPlayback()->GetMissingWords();
		const int count = list.GetCount();
		decStringList ordered;
		int i;
		for(i=0; i<count; i++){
			ordered.Add(list.GetAt(i));
		}
		ordered.SortAscending();
		
		decString text(DEJoin(ordered, "\n"));
		igdeCommonDialogs::GetMultilineString(&pPanel.GetWindowProperties().GetWindowMain(),
			"Copy To Clipboard", "Text for you to copy to the clipboard", text);
		return {};
	}
};

class cActionMissingWordsClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionMissingWordsClear> Ref;
	
public:
	cActionMissingWordsClear(ceWPView &panel) : cBaseAction(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
		"Clear list of missing words found during playback"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		conversation->GetPlayback()->GetMissingWords().RemoveAll();
		conversation->NotifyPlaybackMissingWordsChanged();
		return {};
	}
};

class cActionPlaybackMissingWordsMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionPlaybackMissingWordsMenu> Ref;
	cActionPlaybackMissingWordsMenu(ceWPView &panel) : cBaseActionContextMenu(panel, "Missing Words menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionMissingWordsCopy::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionMissingWordsClear::Ref::New(pPanel));
	}
};

}



// Class ceWPView
///////////////////

// Constructor, destructor
////////////////////////////

ceWPView::ceWPView(ceWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, groupBox2, form, formLine;
	igdeActionContextMenu::Ref actionContextMenu;
	
	pListener = ceWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// wrapper properties
	helper.WPCamera(content, pWPCamera, cActionWPCamera::Ref::New(*this), "Camera:", false, true);
	helper.WPCamera(content, pWPCameraFree, cActionWPCamera::Ref::New(*this), "Camera Free:", false, true);
	helper.WPSky(content, pWPSky, cActionWPSky::Ref::New(*this), "Sky:", false, true);
	helper.WPWObject(content, pWPEnvObject, cActionWPEnvObject::Ref::New(*this), "Environment Object:", false, true);
	
	
	// props
	helper.GroupBox(content, groupBox, "Props:", true);
	
	helper.FormLineStretchFirst(groupBox, "Prop:", "Prop to edit", formLine);
	helper.ComboBox(formLine, "Prop to edit", pCBProps, cComboProp::Ref::New(*this));
	pCBProps->SetDefaultSorter();
	actionContextMenu = cActionPropMenu::Ref::New(*this);
	helper.Button(formLine, pBtnPropMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnPropMenu);
	
	helper.EditString(groupBox, "Name:", "Sets the name of the prop", pEditPropName, cTextPropName::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Object:", "Class of the prop", formLine);
	helper.EditString(formLine, "Class of the prop", pEditPropClass, cTextPropClass::Ref::New(*this));
	helper.Button(formLine, pBtnPropMenu, cActionPropClass::Ref::New(*this, pEditPropClass));
	
	helper.CheckBox(groupBox, pChkPropVisible, cActionPropVisible::Ref::New(*this));
	helper.EditVector(groupBox, "Position:", "Prop position", pEditPropPos, cVectorPropPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Prop rotation", pEditPropRot, cVectorPropRotation::Ref::New(*this));
	
	
	// props
	helper.GroupBoxFlow(content, groupBox, "Actors:", false);
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.FormLineStretchFirst(form, "Actor:", "Actor to edit", formLine);
	helper.EditSpinInteger(formLine, "Actor to edit", 0, 0, pSpinActor, cSpinActor::Ref::New(*this));
	actionContextMenu = cActionActorMenu::Ref::New(*this);
	helper.Button(formLine, pBtnActorMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnActorMenu);
	
	helper.EditString(form, "ID:", "Unique actor ID in the conversation",
		pEditActorID, cTextActorID::Ref::New(*this));
	helper.EditString(form, "Alias ID:", "Optional unique actor alias ID",
		pEditActorAliasID, cTextActorAliasID::Ref::New(*this));
	helper.EditVector(form, "Position:", "Actor position", pEditActorPos, cVectorActorPosition::Ref::New(*this));
	helper.EditVector(form, "Rotation:", "Actor rotation", pEditActorOri, cVectorActorRotation::Ref::New(*this));
	helper.EditPath(form, "Model:", "Actor model to use", igdeEnvironment::efpltModel,
		pEditActorPathModel, cPathActorModel::Ref::New(*this));
	helper.EditPath(form, "Skin:", "Actor skin to use", igdeEnvironment::efpltSkin,
		pEditActorPathSkin, cPathActorSkin::Ref::New(*this));
	helper.EditPath(form, "Rig:", "Actor rig to use", igdeEnvironment::efpltRig,
		pEditActorPathRig, cPathActorRig::Ref::New(*this));
	helper.EditPath(form, "Speech Animation:", "Speech animation to use",
		igdeEnvironment::efpltSpeechAnimation, pEditActorPathSpeechAnim, cPathActorSpeechAnimation::Ref::New(*this));
	helper.EditPath(form, "Eye Animator:", "Eye animator to use",
		igdeEnvironment::efpltAnimator, pEditActorPathEyeAnimator, cPathActorEyeAnimator::Ref::New(*this));
	helper.EditPath(form, "Face Pose Animator:", "Face pose animator to use",
		igdeEnvironment::efpltAnimator, pEditActorPathFaceAnimator, cPathActorFacePoseAnimator::Ref::New(*this));
	helper.CheckBox(form, pChkActorWaiting, cActionActorWaiting::Ref::New(*this));
	helper.EditString(form, "Head Rotator:", "Name of head rotator bone or empty string if not used",
		pEditActorBoneHeadRotator, cTextActorBoneHeadRotator::Ref::New(*this));
	
	helper.FormLineStretchFirst(form, "Actor Pose:", "Actor pose to edit", formLine);
	helper.ComboBox(formLine, "Actor pose to edit", pCBActorPose, cComboActorPose::Ref::New(*this));
	actionContextMenu = cActionActorPoseMenu::Ref::New(*this);
	helper.Button(formLine, pBtnActorPoseMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnActorPoseMenu);
	
	helper.EditPath(form, "Animator:", "Animator to use",
		igdeEnvironment::efpltAnimator, pEditActorPosePathAnimator, cPathActorPoseAnimator::Ref::New(*this));
	
	
	helper.GroupBox(groupBox, groupBox2, "Animator Controllers:", true);
	
	helper.FormLineStretchFirst(groupBox2, "Controller:", "Controller to edit", formLine);
	helper.ComboBox(formLine, "Controller to edit", pCBActorPoseController, cComboActorPoseController::Ref::New(*this));
	actionContextMenu = cActionActorPoseControllerMenu::Ref::New(*this);
	helper.Button(formLine, pBtnActorPoseControllerMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnActorPoseControllerMenu);
	
	helper.ComboBox(groupBox2, "Update Type:", "How to update the controller",
		pCBActorPoseControllerUpdateType, cComboActorPoseControllerUpdateType::Ref::New(*this, pCBActorPoseController));
	pCBActorPoseControllerUpdateType->AddItem("Constant Value", nullptr,
		(void*)(intptr_t)ceActorController::eutConstant);
	pCBActorPoseControllerUpdateType->AddItem("Elapsed Time", nullptr,
		(void*)(intptr_t)ceActorController::eutElapsedTime);
	pCBActorPoseControllerUpdateType->AddItem("Head Left-Right", nullptr,
		(void*)(intptr_t)ceActorController::eutHeadLeftRight);
	pCBActorPoseControllerUpdateType->AddItem("Head Up-Down", nullptr,
		(void*)(intptr_t)ceActorController::eutHeadUpDown);
	pCBActorPoseControllerUpdateType->AddItem("Eyes Left-Right", nullptr,
		(void*)(intptr_t)ceActorController::eutEyesLeftRight);
	pCBActorPoseControllerUpdateType->AddItem("Eyes Up-Down", nullptr,
		(void*)(intptr_t)ceActorController::eutEyesUpDown);
	
	helper.EditFloat(groupBox2, "Value:", "Constant value to use if the update type is constant",
		pEditActorPoseControllerValue, cTextActorPoseControllerValue::Ref::New(*this, pCBActorPoseController));
	helper.EditVector(groupBox2, "Vector:", "Constant vector to use if the update type is constant",
		pEditActorPoseControllerVector, cTextActorPoseControllerVector::Ref::New(*this, pCBActorPoseController));
	
	
	helper.GroupBox(groupBox, groupBox2, "Actor Gestures:", true);
	
	helper.FormLineStretchFirst(groupBox2, "Gesture:", "Actor gesture to edit", formLine);
	helper.ComboBox(formLine, "Actor gesture to edit", pCBActorGesture, cComboActorGesture::Ref::New(*this));
	actionContextMenu = cActionActorGestureMenu::Ref::New(*this);
	helper.Button(formLine, pBtnActorGestureMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnActorGestureMenu);
	
	helper.EditPath(groupBox2, "Animator:", "Animator to use", igdeEnvironment::efpltAnimator,
		pEditActorGesturePathAnimator, cPathActorGestureAnimator::Ref::New(*this));
	
	
	helper.GroupBoxFlow(groupBox, groupBox2, "Actor Command Simulation:", false, true);
	helper.ListBox(groupBox2, 5, "Actor Commands", pListActorCommands, cListActorCommands::Ref::New(*this));
	pListActorCommands->SetDefaultSorter();
	
	helper.GroupBoxFlow(groupBox, groupBox2, "Actor Parameters:", false, true);
	helper.ListBox(groupBox2, 5, "Actor Parameters", pListActorParameters, cListActorParameters::Ref::New(*this));
	pListActorParameters->SetDefaultSorter();
	
	
	// coordinate system
	helper.GroupBox(content, groupBox, "Coordinate Systems:", true);
	
	helper.FormLineStretchFirst(groupBox, "Coord-System:", "Coordinate system to edit", formLine);
	helper.EditSpinInteger(formLine, "Coordinate system to edit", 0, 0, pSpinCoordSys, cSpinCoordSys::Ref::New(*this));
	actionContextMenu = cActionCoordSysMenu::Ref::New(*this);
	helper.Button(formLine, pBtnCoordSysMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnCoordSysMenu);
	
	helper.EditString(groupBox, "ID:", "Unique coordinate system ID in the conversation",
		pEditCoordSysID, cTextCoordSysID::Ref::New(*this));
	helper.EditString(groupBox, "Alias ID:", "Optional unique coordinate system alias ID",
		pEditCoordSysAliasID, cTextCoordSysAliasID::Ref::New(*this));
	helper.EditVector(groupBox, "Position:", "Position of the coordinate system",
		pEditCoordSysPosition, cVectorCoordSysPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Orientation of the coordinate system",
		pEditCoordSysRotation, cVectorCoordSysRotation::Ref::New(*this));
	
	
	// playback
	helper.GroupBoxFlow(content, groupBox, "Playback:", false);
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.ComboBoxFilter(form, "File:", "File to show topics for",
		pCBPlaybackFile, cComboPlaybackFile::Ref::New(*this));
	pCBPlaybackFile->SetDefaultSorter();
	
	helper.ComboBoxFilter(form, "Topic:", "Topic to play back", pCBPlaybackTopic, {});
	pCBPlaybackTopic->SetDefaultSorter();
	
	helper.FormLine(form, "", "", formLine);
	helper.Button(formLine, pBtnPlaybackSelectTopic, cActionPlaybackSelectTopic::Ref::New(*this));
	helper.Button(formLine, pBtnPlaybackRewind, cActionPlaybackRewind::Ref::New(*this));
	
	helper.FormLine(form, "", "", formLine);
	helper.CheckBoxOnly(formLine, pChkPlaybackRunning, cActionPlaybackRunning::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkPlaybackPaused, cActionPlaybackPaused::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkPlaybackAutoAdvanceCommands, cActionPlaybackAutoAdvanceCommands::Ref::New(*this));
	
	helper.ComboBox(form, "Camera Handling:", "How camera is handled",
		pCBPlaybackCameraHandling, cComboPlaybackCameraHandling::Ref::New(*this));
	pCBPlaybackCameraHandling->AddItem("Conversation", nullptr, (void*)(intptr_t)cePlayback::echConversation);
	pCBPlaybackCameraHandling->AddItem("Camera Shot", nullptr, (void*)(intptr_t)cePlayback::echCameraShot);
	pCBPlaybackCameraHandling->AddItem("Free", nullptr, (void*)(intptr_t)cePlayback::echFree);
	pCBPlaybackCameraHandling->SetSelectionWithData((void*)(intptr_t)cePlayback::echFree);
	
	
	helper.GroupBoxFlow(groupBox, groupBox2, "Game Command:", false, true);
	helper.ListBox(groupBox2, 5, "Game Commands", pListPlaybackCommands, cListPlaybackCommands::Ref::New(*this));
	pListPlaybackCommands->SetDefaultSorter();
	
	helper.GroupBoxFlow(groupBox, groupBox2, "Variables:", false, true);
	helper.ListBox(groupBox2, 5, "Variables", pListPlaybackVars, cListPlaybackVariables::Ref::New(*this));
	pListPlaybackVars->SetDefaultSorter();
	
	
	helper.WPTriggerTable(groupBox, pWPPlaybackTriggerTable,
		cActionPlaybackTriggerTable::Ref::New(*this), "Trigger Table:", false, true);
	
	
	helper.GroupBoxFlow(groupBox, groupBox2, "Debug:", false, false);
	
	helper.FormLineStretchFirst(groupBox2, "Missing Words:", "Missing words found during playback", formLine);
	helper.ComboBox(formLine, "Missing words found during playback", pCBPlaybackMissingWords, {});
	pCBPlaybackMissingWords->SetDefaultSorter();
	actionContextMenu = cActionPlaybackMissingWordsMenu::Ref::New(*this);
	helper.Button(formLine, pBtnPlaybackMissingWordsMenu, actionContextMenu);
	actionContextMenu->SetWidget(pBtnPlaybackMissingWordsMenu);
}

ceWPView::~ceWPView(){
	SetConversation(nullptr);
}



// Management
///////////////

void ceWPView::SetConversation(ceConversation *conversation){
	if(conversation == pConversation){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPSky->SetSky(nullptr);
	pWPCamera->SetCamera(nullptr);
	pWPCameraFree->SetCamera(nullptr);
	pWPPlaybackTriggerTable->SetTriggerTargetList(nullptr);
	
	if(pConversation){
		pConversation->RemoveListener(pListener);
	}
	
	pConversation = conversation;
	
	if(conversation){
		conversation->AddListener(pListener);
		pWPSky->SetSky(conversation->GetSky());
		pWPEnvObject->SetObject(conversation->GetEnvObject());
		pWPCamera->SetCamera(conversation->GetCamera());
		pWPCameraFree->SetCamera(conversation->GetCameraFree());
		pWPPlaybackTriggerTable->SetTriggerTargetList(&conversation->GetPlayback()->GetTriggerTable());
	}
	
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdateCamera();
	UpdatePropList();
	UpdateActors();
	UpdateCoordSystems();
	UpdatePlayback();
	UpdatePlaybackFileList();
	UpdatePlaybackTopicList();
	UpdatePlaybackCommands();
	UpdatePlaybackVariables();
	UpdatePlaybackTriggerTable();
}

void ceWPView::UpdateView(){
}

void ceWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void ceWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void ceWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
	pWPCameraFree->UpdateCamera();
	pWPCameraFree->UpdateViewDirection();
}



ceConversationActor *ceWPView::GetActor() const{
	return pConversation ? pConversation->GetActiveActor() : nullptr;
}

ceActorPose *ceWPView::GetActorPose() const{
	const ceConversationActor * const actor = GetActor();
	return actor ? actor->GetActivePose() : nullptr;
}

ceActorController *ceWPView::GetActorPoseController() const{
	return pCBActorPoseController->GetSelectedItem()
		? (ceActorController*)pCBActorPoseController->GetSelectedItem()->GetData() : nullptr;
}

ceActorGesture *ceWPView::GetActorGesture() const{
	return GetActorPose() && pCBActorGesture->GetSelectedItem()
		? (ceActorGesture*)pCBActorGesture->GetSelectedItem()->GetData() : nullptr;
}

ceProp *ceWPView::GetProp() const{
	return pConversation ? pConversation->GetActiveProp() : nullptr;
}

void ceWPView::UpdatePropList(){
	pCBProps->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetProps().Visit([&](ceProp *prop){
			pCBProps->AddItem(prop->GetName(), nullptr, prop);
		});
	}
	
	pCBProps->SortItems();
	pCBProps->SetSelectionWithData(GetProp());
	UpdateProp();
}

void ceWPView::UpdateProp(){
	const ceProp * const prop = GetProp();
	
	if(prop){
		pEditPropName->SetText(prop->GetName());
		pEditPropClass->SetText(prop->GetObjectClass());
		pEditPropPos->SetVector(prop->GetPosition());
		pEditPropRot->SetVector(prop->GetOrientation());
		
	}else{
		pEditPropName->ClearText();
		pEditPropClass->ClearText();
		pEditPropPos->SetVector(decVector());
		pEditPropRot->SetVector(decVector());
	}
	
	const bool enabled = prop;
	pEditPropName->SetEnabled(enabled);
	pEditPropClass->SetEnabled(enabled);
	pEditPropPos->SetEnabled(enabled);
	pEditPropRot->SetEnabled(enabled);
	
	pChkPropVisible->GetAction()->Update();
}

void ceWPView::UpdateActors(){
	const ceConversationActor * const actor = GetActor();
	
	// actor list
	if(pConversation){
		const ceConversationActorList &list = pConversation->GetActorList();
		pSpinActor->SetRange(0, decMath::max(list.GetCount() - 1, 0));
		pSpinActor->SetValue(actor ? list.IndexOf((ceConversationActor*)actor) : 0);
		
	}else{
		pSpinActor->SetRange(0, 0);
	}
	
	// actor
	if(actor){
		pEditActorID->SetText(actor->GetID());
		pEditActorAliasID->SetText(actor->GetAliasID());
		pEditActorPos->SetVector(actor->GetPosition());
		pEditActorOri->SetVector(actor->GetOrientation());
		pEditActorPathModel->SetPath(actor->GetPathModel());
		pEditActorPathSkin->SetPath(actor->GetPathSkin());
		pEditActorPathRig->SetPath(actor->GetPathRig());
		pEditActorPathSpeechAnim->SetPath(actor->GetPathSpeechAnimation());
		pEditActorPathEyeAnimator->SetPath(actor->GetPathEyesAnimator());
		pEditActorPathFaceAnimator->SetPath(actor->GetPathFacePoseAnimator());
		pEditActorBoneHeadRotator->SetText(actor->GetBoneHeadRotator());
		
	}else{
		pEditActorID->ClearText();
		pEditActorAliasID->ClearText();
		pEditActorPos->SetVector(decVector());
		pEditActorOri->SetVector(decVector());
		pEditActorPathModel->ClearPath();
		pEditActorPathSkin->ClearPath();
		pEditActorPathRig->ClearPath();
		pEditActorPathSpeechAnim->ClearPath();
		pEditActorPathEyeAnimator->ClearPath();
		pEditActorPathFaceAnimator->ClearPath();
		pEditActorBoneHeadRotator->ClearText();
	}
	
	const bool enabled = actor;
	pEditActorID->SetEnabled(enabled);
	pEditActorAliasID->SetEnabled(enabled);
	pEditActorPos->SetEnabled(enabled);
	pEditActorOri->SetEnabled(enabled);
	pEditActorPathModel->SetEnabled(enabled);
	pEditActorPathSkin->SetEnabled(enabled);
	pEditActorPathRig->SetEnabled(enabled);
	pEditActorPathSpeechAnim->SetEnabled(enabled);
	pEditActorPathEyeAnimator->SetEnabled(enabled);
	pEditActorPathFaceAnimator->SetEnabled(enabled);
	pEditActorBoneHeadRotator->SetEnabled(enabled);
	
	pChkActorWaiting->GetAction()->Update();
	
	UpdateActorPoses();
	UpdateActorCommands();
	UpdateActorParameters();
}

void ceWPView::UpdateActorPoses(){
	ceConversationActor * const actor = GetActor();
	ceActorPose *activePose = actor ? actor->GetActivePose() : nullptr;
	
	pCBActorPose->RemoveAllItems();
	
	if(actor){
		actor->GetPoses().Visit([&](ceActorPose *p){
			pCBActorPose->AddItem(p->GetName(), nullptr, p);
		});
		
		if(!activePose && actor->GetPoses().GetCount() > 0){
			activePose = actor->GetPoses().First();
		}
		actor->SetActivePose(activePose);
	}
	
	SelectActiveActorPose();
}

void ceWPView::SelectActiveActorPose(){
	pCBActorPose->SetSelectionWithData(GetActorPose());
	UpdateActorPose();
}

void ceWPView::UpdateActorPose(){
	const ceActorPose * const pose = GetActorPose();
	
	if(pose){
		pEditActorPosePathAnimator->SetPath(pose->GetPathAnimator());
		
	}else{
		pEditActorPosePathAnimator->ClearPath();
	}
	
	const bool enabled = pose;
	pEditActorPosePathAnimator->SetEnabled(enabled);
	
	UpdateActorControllers();
	UpdateActorGestures();
}

void ceWPView::UpdateActorGestures(){
	const ceActorPose * const pose = GetActorPose();
	ceActorGesture *gesture = GetActorGesture();
	pCBActorGesture->RemoveAllItems();
	
	if(pose){
		pose->GetGestures().Visit([&](ceActorGesture *g){
			pCBActorGesture->AddItem(g->GetName(), nullptr, g);
		});
		pCBActorGesture->SortItems();
	}
	
	pCBActorGesture->SetSelectionWithData(gesture);
	if(!pCBActorGesture->GetSelectedItem() && pCBActorGesture->GetItems().IsNotEmpty()){
		pCBActorGesture->SetSelection(0);
	}
	gesture = GetActorGesture();
	
	if(gesture){
		pEditActorGesturePathAnimator->SetPath(gesture->GetPathAnimator());
		
	}else{
		pEditActorGesturePathAnimator->ClearPath();
	}
	
	const bool enabled = gesture;
	pEditActorGesturePathAnimator->SetEnabled(enabled);
}

void ceWPView::SelectActorPoseGesture(ceActorGesture *gesture){
	pCBActorGesture->SetSelectionWithData(gesture);
	UpdateActorGesture();
}

void ceWPView::UpdateActorGesture(){
	const ceActorGesture * const gesture = GetActorGesture();
	
	if(gesture){
		pEditActorGesturePathAnimator->SetPath(gesture->GetPathAnimator());
		
	}else{
		pEditActorGesturePathAnimator->ClearPath();
	}
	
	pEditActorGesturePathAnimator->SetEnabled(gesture);
}

void ceWPView::UpdateActorControllers(){
	const ceActorPose * const pose = GetActorPose();
	ceActorController * const selection = pCBActorPoseController->GetSelectedItem()
		? (ceActorController*)pCBActorPoseController->GetSelectedItem()->GetData() : nullptr;
	
	pCBActorPoseController->RemoveAllItems();
	
	if(pose){
		pose->GetControllers().Visit([&](ceActorController *c){
			pCBActorPoseController->AddItem(c->GetName(), nullptr, c);
		});
		pCBActorPoseController->SetSelectionWithData(selection);
	}
	
	pCBActorPoseController->SetEnabled(pose);
	
	UpdateActorController();
}

void ceWPView::UpdateActorController(){
	const ceActorPose * const pose = GetActorPose();
	const ceActorController * const controller = pCBActorPoseController->GetSelectedItem()
		? (ceActorController*)pCBActorPoseController->GetSelectedItem()->GetData() : nullptr;
	
	if(pose && controller){
		pCBActorPoseControllerUpdateType->SetSelectionWithData(
			(void*)(intptr_t)controller->GetUpdateType());
		pEditActorPoseControllerValue->SetFloat(controller->GetValue());
		pEditActorPoseControllerVector->SetVector(controller->GetVector());
		
	}else{
		pCBActorPoseControllerUpdateType->SetSelectionWithData(
			(void*)(intptr_t)ceActorController::eutElapsedTime);
		pEditActorPoseControllerValue->ClearText();
		pEditActorPoseControllerVector->SetVector(decVector());
	}
	
	const bool enabled = pose && controller;
	pCBActorPoseControllerUpdateType->SetEnabled(enabled);
	pEditActorPoseControllerValue->SetEnabled(enabled);
	pEditActorPoseControllerVector->SetEnabled(enabled);
}

void ceWPView::SelectActorPoseController(ceActorController *controller){
	pCBActorPoseController->SetSelectionWithData(controller);
}

void ceWPView::UpdateActorCommands(){
	const ceConversationActor * const actor = GetActor();
	cePlaybackCommand * const selection = pListActorCommands->GetSelectedItem()
		? (cePlaybackCommand*)pListActorCommands->GetSelectedItem()->GetData() : nullptr;
	
	pListActorCommands->RemoveAllItems();
	
	if(actor){
		igdeIcon * const iconTrue = GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus);
		igdeIcon * const iconFalse = GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus);
		actor->GetCommands().Visit([&](cePlaybackCommand *c){
			pListActorCommands->AddItem(c->GetCommand(), c->GetValue() ? iconTrue : iconFalse, c);
		});
	}
	
	pListActorCommands->SortItems();
	
	pListActorCommands->SetSelectionWithData(selection);
	if(!pListActorCommands->GetSelectedItem() && pListActorCommands->GetItems().IsNotEmpty()){
		pListActorCommands->SetSelection(0);
	}
}

void ceWPView::UpdateActorParameters(){
	const cVariableKey::Ref selection(pListActorParameters->GetSelectedItemRefData().DynamicCast<cVariableKey>());
	
	pListActorParameters->RemoveAllItems();
	
	const ceConversationActor * const actor = GetActor();
	if(actor){
		actor->GetParameter().Visit([&](const decString &name, int value){
			decString text;
			text.Format("%s = %d", name.GetString(), value);
			pListActorParameters->AddItemRef(text, nullptr, cVariableKey::Ref::New(name));
		});
	}
	
	pListActorParameters->SortItems();
	
	if(selection){
		const int count = pListActorParameters->GetItems().GetCount();
		int i;
		for(i=0; i<count; i++){
			const cVariableKey::Ref varKey = pListActorParameters->GetItems().GetAt(i)->
				GetRefData().DynamicCast<cVariableKey>();
			if(varKey->name == selection->name){
				pListActorParameters->SetSelection(i);
				break;
			}
		}
	}
	
	if(!pListActorParameters->GetSelectedItem() && pListActorParameters->GetItems().IsNotEmpty()){
		pListActorParameters->SetSelection(0);
	}
}



ceCoordSystem *ceWPView::GetCoordSys() const{
	return pConversation ? pConversation->GetActiveCoordSystem() : nullptr;
}

void ceWPView::UpdateCoordSystems(){
	if(pConversation){
		const ceCoordSystemList &coordSystemList = pConversation->GetCoordSystemList();
		pSpinCoordSys->SetRange(0, decMath::max(coordSystemList.GetCount() - 1, 0));
		
		ceCoordSystem * const coordsys = pConversation->GetActiveCoordSystem();
		if(coordsys){
			pSpinCoordSys->SetValue(coordSystemList.IndexOf(coordsys));
			pEditCoordSysID->SetText(coordsys->GetID());
			pEditCoordSysAliasID->SetText(coordsys->GetAliasID());
			pEditCoordSysPosition->SetVector(coordsys->GetPosition());
			pEditCoordSysRotation->SetVector(coordsys->GetOrientation());
			
		}else{
			pEditCoordSysID->ClearText();
			pEditCoordSysAliasID->ClearText();
			pEditCoordSysPosition->SetVector(decVector());
			pEditCoordSysRotation->SetVector(decVector());
		}
		
	}else{
		pEditCoordSysID->ClearText();
		pEditCoordSysAliasID->ClearText();
		pEditCoordSysPosition->SetVector(decVector());
		pEditCoordSysRotation->SetVector(decVector());
	}
	
	const bool enabled = GetCoordSys();
	pEditCoordSysID->SetEnabled(enabled);
	pEditCoordSysAliasID->SetEnabled(enabled);
	pEditCoordSysPosition->SetEnabled(enabled);
	pEditCoordSysRotation->SetEnabled(enabled);
}



void ceWPView::UpdatePlaybackFileList(){
	ceConversationFile * const selectedFile = GetSelectedPlaybackFile();
	pCBPlaybackFile->RemoveAllItems();
	
	if(pConversation){
		pConversation->AllFiles().Visit([this](ceConversationFile *f){
			pCBPlaybackFile->AddItem(f->GetID(), nullptr, f);
		});
		
		pCBPlaybackFile->SortItems();
		pCBPlaybackFile->StoreFilterItems();
	}
	
	pCBPlaybackFile->SetSelectionWithData(selectedFile);
}

ceConversationFile *ceWPView::GetSelectedPlaybackFile() const{
	return pCBPlaybackFile->GetSelectedItem()
		? (ceConversationFile*)pCBPlaybackFile->GetSelectedItem()->GetData() : nullptr;
}

void ceWPView::UpdatePlaybackTopicList(){
	ceConversationFile * const selectedFile = GetSelectedPlaybackFile();
	ceConversationTopic * const selectedTopic = GetSelectedPlaybackTopic();
	
	pCBPlaybackTopic->RemoveAllItems();
	
	if(selectedFile){
		pConversation->AllTopics(selectedFile->GetID()).Visit([&](ceConversationTopic *topic){
			pCBPlaybackTopic->AddItem(topic->GetID(), nullptr, topic);
		});
		pCBPlaybackTopic->SortItems();
		pCBPlaybackTopic->StoreFilterItems();
	}
	
	pCBPlaybackTopic->SetSelectionWithData(selectedTopic);
}

ceConversationTopic *ceWPView::GetSelectedPlaybackTopic() const{
	return pCBPlaybackTopic->GetSelectedItem()
		? (ceConversationTopic*)pCBPlaybackTopic->GetSelectedItem()->GetData() : nullptr;
}

void ceWPView::UpdatePlayback(){
	if(pConversation){
		pCBPlaybackCameraHandling->SetSelectionWithData(
			(void*)(intptr_t)pConversation->GetPlayback()->GetCameraHandling());
		
	}else{
		pCBPlaybackCameraHandling->SetSelectionWithData((void*)(intptr_t)cePlayback::echFree);
	}
	
	pCBPlaybackCameraHandling->SetEnabled(pConversation);
	
	pChkPlaybackRunning->GetAction()->Update();
	pChkPlaybackPaused->GetAction()->Update();
	pChkPlaybackAutoAdvanceCommands->GetAction()->Update();
}

void ceWPView::UpdatePlaybackCommands(){
	cePlaybackCommand * const selectedEntry = pListPlaybackCommands->GetSelectedItem()
		? (cePlaybackCommand*)pListPlaybackCommands->GetSelectedItem()->GetData() : nullptr;
	
	pListPlaybackCommands->RemoveAllItems();
	
	if(pConversation){
		igdeIcon * const iconTrue = GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus);
		igdeIcon * const iconFalse = GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus);
		pConversation->GetPlayback()->GetCommands().Visit([&](cePlaybackCommand *c){
			pListPlaybackCommands->AddItem(c->GetCommand(), c->GetValue() ? iconTrue : iconFalse, c);
		});
	}
	
	pListPlaybackCommands->SortItems();
	
	pListPlaybackCommands->SetSelectionWithData(selectedEntry);
	if(!pListPlaybackCommands->GetSelectedItem() && pListPlaybackCommands->GetItems().IsNotEmpty()){
		pListPlaybackCommands->SetSelection(0);
	}
}

void ceWPView::UpdatePlaybackVariables(){
	const cVariableKey::Ref selection(pListPlaybackVars->GetSelectedItemRefData().DynamicCast<cVariableKey>());
	
	pListPlaybackVars->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetPlayback()->GetVariables().Visit([&](const decString &n, int v){
			decString text;
			text.Format("%s = %d", n.GetString(), v);
			pListPlaybackVars->AddItemRef(text, nullptr, cVariableKey::Ref::New(n));
		});
	}
	
	pListPlaybackVars->SortItems();
	
	if(selection){
		const int count = pListPlaybackVars->GetItems().GetCount();
		int i;
		for(i=0; i<count; i++){
			const cVariableKey::Ref varKey = pListPlaybackVars->GetItems().GetAt(i)->
				GetRefData().DynamicCast<cVariableKey>();
			if(varKey->name == selection->name){
				pListPlaybackVars->SetSelection(i);
				break;
			}
		}
	}
	
	if(!pListPlaybackVars->GetSelectedItem() && pListPlaybackVars->GetItems().IsNotEmpty()){
		pListPlaybackVars->SetSelection(0);
	}
}

void ceWPView::UpdatePlaybackTriggerTable(){
	pWPPlaybackTriggerTable->UpdateTable();
}

void ceWPView::UpdatePlaybackMissingWords(){
	const decString selectedWord(pCBPlaybackMissingWords->GetText());
	
	pCBPlaybackMissingWords->RemoveAllItems();
	
	if(pConversation){
		const decStringSet &list = pConversation->GetPlayback()->GetMissingWords();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pCBPlaybackMissingWords->AddItem(list.GetAt(i));
		}
	}
	
	pCBPlaybackMissingWords->SortItems();
	
	pCBPlaybackMissingWords->SetText(selectedWord);
	if(!pCBPlaybackMissingWords->GetSelectedItem() && pCBPlaybackMissingWords->GetItems().IsNotEmpty()){
		pCBPlaybackMissingWords->SetSelection(0);
	}
}
