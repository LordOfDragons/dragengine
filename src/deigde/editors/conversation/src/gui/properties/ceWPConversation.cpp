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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPConversation.h"
#include "ceWPConversationListener.h"
#include "ceWindowProperties.h"
#include "../ceWindowMain.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/actor/ceConversationActor.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/facepose/ceFacePose.h"
#include "../../conversation/gesture/ceGesture.h"
#include "../../conversation/target/ceTarget.h"
#include "../../configuration/ceConfiguration.h"
#include "../../loadsave/ceLoadSaveSystem.h"
#include "../../undosys/ceUConvoSetImportConvoPath.h"
#include "../../undosys/cameraShot/ceUCCShotAdd.h"
#include "../../undosys/cameraShot/ceUCCShotRemove.h"
#include "../../undosys/cameraShot/ceUCCShotSetActorCount.h"
#include "../../undosys/cameraShot/ceUCCShotSetCameraTarget.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamDistFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamDistTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffCamFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffCamTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamOrbitFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamOrbitTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetFovFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetFovTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetLookAtTarget.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffLookAtFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffLookAtTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetName.h"
#include "../../undosys/cameraShot/ceUCCShotSetPosFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetPosTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetRotFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetRotTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetTiltFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetTiltTo.h"
#include "../../undosys/cameraShot/ceUCCShotToggleLockUpAxis.h"
#include "../../undosys/cameraShot/ceUCCShotToggleAlignTargets.h"
#include "../../undosys/cameraShot/ceUCCShotToggleRelativeToLookAt.h"
#include "../../undosys/cameraShot/ceUCCShotToggleLockCameraTarget.h"
#include "../../undosys/cameraShot/ceUCCShotToggleLockLookAtTarget.h"
#include "../../undosys/facePose/ceUCFacePoseAdd.h"
#include "../../undosys/facePose/ceUCFacePoseRemove.h"
#include "../../undosys/facePose/ceUCFacePoseSetName.h"
#include "../../undosys/facePose/controller/ceUCFPControllerAdd.h"
#include "../../undosys/facePose/controller/ceUCFPControllerRemove.h"
#include "../../undosys/facePose/controller/ceUCFPControllerSetValue.h"
#include "../../undosys/gesture/ceUCGestureAdd.h"
#include "../../undosys/gesture/ceUCGestureRemove.h"
#include "../../undosys/gesture/ceUCGestureSetName.h"
#include "../../undosys/gesture/ceUCGestureSetAnimator.h"
#include "../../undosys/gesture/ceUCGestureToggleHold.h"
#include "../../undosys/gesture/ceUCGestureSetDuration.h"
#include "../../undosys/target/ceUCTargetAdd.h"
#include "../../undosys/target/ceUCTargetRemove.h"
#include "../../undosys/target/ceUCTargetSetName.h"
#include "../../undosys/target/ceUCTargetSetActor.h"
#include "../../undosys/target/ceUCTargetSetEntityID.h"
#include "../../undosys/target/ceUCTargetSetPosition.h"
#include "../../undosys/target/ceUCTargetSetOrientation.h"
#include "../../undosys/target/ceUCTargetSetBone.h"
#include "../../utils/ceControllerValue.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	ceWPConversation &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(ceWPConversation &panel, const char *text, igdeIcon *icon, const char *description) :
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
	ceWPConversation &pPanel;
	
public:
	typedef deTObjectReference<cBaseActionContextMenu> Ref;
	cBaseActionContextMenu(ceWPConversation &panel, const char *description) :
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
	ceWPConversation &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(ceWPConversation &panel) : pPanel(panel){}
	
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
	ceWPConversation &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(ceWPConversation &panel) : pPanel(panel){}
	
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
	ceWPConversation &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(ceWPConversation &panel) : pPanel(panel){}
	
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



class cActionImportConvoAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportConvoAdd> Ref;
	
public:
	cActionImportConvoAdd(ceWPConversation &panel) : cBaseAction(panel, "",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Import Conversation"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		const decString &path = pPanel.GetPathImportConvo();
		if(path.IsEmpty() || path == conversation->GetFilePath()
		|| conversation->GetImportConversationPath().Has(path)){
			return {};
		}
		
		decStringList list(pPanel.GetConversation()->GetImportConversationPath());
		list.Add(path);
		return ceUConvoSetImportConvoPath::Ref::New(pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list);
	}
};

class cActionImportConvoRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportConvoRemove> Ref;
	
public:
	cActionImportConvoRemove(ceWPConversation &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Import"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		const decString path(pPanel.GetImportConvo());
		if(path.IsEmpty()){
			return {};
		}
		
		decStringList list(pPanel.GetConversation()->GetImportConversationPath());
		const int index = list.IndexOf(path);
		if(index == -1){
			return {};
		}
		
		list.RemoveFrom(index);
		return ceUConvoSetImportConvoPath::Ref::New(pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list);
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(!pPanel.GetImportConvo().IsEmpty());
	}
};

class cActionImportConvoMoveUp : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportConvoMoveUp> Ref;
	
public:
	cActionImportConvoMoveUp(ceWPConversation &panel) : cBaseAction(panel, "Move Up",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move Up"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		const decString path(pPanel.GetImportConvo());
		if(path.IsEmpty()){
			return {};
		}
		
		decStringList list(pPanel.GetConversation()->GetImportConversationPath());
		const int index = list.IndexOf(path);
		if(index < 1){
			return {};
		}
		
		list.Move(index, index - 1);
		return ceUConvoSetImportConvoPath::Ref::New(pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list);
	}
	
	void Update(const ceConversation &conversation) override{
		const int index = conversation.GetImportConversationPath().IndexOf(pPanel.GetImportConvo());
		SetEnabled(!pPanel.GetImportConvo().IsEmpty() && index > 0);
	}
};

class cActionImportConvoMoveDown : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportConvoMoveDown> Ref;
	
public:
	cActionImportConvoMoveDown(ceWPConversation &panel) : cBaseAction(panel, "Move Down",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move Down"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		const decString path(pPanel.GetImportConvo());
		if(path.IsEmpty()){
			return {};
		}
		
		decStringList list(pPanel.GetConversation()->GetImportConversationPath());
		const int index = list.IndexOf(path);
		if(index == -1 || index < list.GetCount() - 1){
			return {};
		}
		
		list.Move(index, index + 1);
		return ceUConvoSetImportConvoPath::Ref::New(pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list);
	}
	
	void Update(const ceConversation &conversation) override{
		const decStringList &list = conversation.GetImportConversationPath();
		const int index = list.IndexOf(pPanel.GetImportConvo());
		SetEnabled(!pPanel.GetImportConvo().IsEmpty() && index != -1 && index < list.GetCount() - 1);
	}
};

class cActionImportConvoClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportConvoClear> Ref;
	
public:
	cActionImportConvoClear(ceWPConversation &panel) : cBaseAction(panel, "Remove All",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove All Imports"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		return pPanel.GetConversation() && pPanel.GetConversation()->GetImportConversationPath().GetCount() > 0
			? ceUConvoSetImportConvoPath::Ref::New(pPanel.GetWindowProperties().GetWindowMain()
				.GetLoadSaveSystem(), pPanel.GetConversation(), decStringList()) : igdeUndo::Ref();
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetConversation() && pPanel.GetConversation()->GetImportConversationPath().GetCount() > 0);
	}
};

class cListImportConvo : public igdeListBoxListener{
	ceWPConversation &pPanel;
public:
	typedef deTObjectReference<cListImportConvo> Ref;
	cListImportConvo(ceWPConversation &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, cActionImportConvoMoveUp::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportConvoMoveDown::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportConvoRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportConvoClear::Ref::New(pPanel));
	}
};



class cComboTarget : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) override{
		conversation->SetActiveTarget(comboBox.GetSelectedItem()
			? (ceTarget*)comboBox.GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};

class cActionTargetAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionTargetAdd> Ref;
	
public:
	cActionTargetAdd(ceWPConversation &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Target"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		decString name("Target");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Target", "Name:", name)){
			return {};
		}
		if(conversation->GetTargets().HasMatching([&](const ceTarget &t){ return t.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Target", "Duplicate name");
			return {};
		}
		
		const ceTarget::Ref target(ceTarget::Ref::New(name));
		return ceUCTargetAdd::Ref::New(conversation, target);
	}
};

class cActionTargetRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionTargetRemove> Ref;
	
public:
	cActionTargetRemove(ceWPConversation &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Target"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		return pPanel.GetTarget() ? ceUCTargetRemove::Ref::New(pPanel.GetTarget()) : ceUCTargetRemove::Ref();
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetTarget());
	}
};

class cActionTargetRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionTargetRename> Ref;
	
public:
	cActionTargetRename(ceWPConversation &panel) : cBaseAction(panel,
	"Rename...", nullptr, "Rename Target"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceTarget * const target = pPanel.GetTarget();
		if(!target){
			return {};
		}
		
		decString name(target->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Target", "Name:", name) 
		|| name == target->GetName()){
			return {};
		}
		if(conversation->GetTargets().HasMatching([&](const ceTarget &t){ return t.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Target", "Duplicate name");
			return {};
		}
		
		return ceUCTargetSetName::Ref::New(target, name);
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetTarget());
	}
};

class cActionTargetMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionTargetMenu> Ref;
	cActionTargetMenu(ceWPConversation &panel) : cBaseActionContextMenu(panel, "Target menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionTargetAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTargetRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTargetRename::Ref::New(pPanel));
	}
};

class cComboTargetActorID : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTargetActorID> Ref;
	cComboTargetActorID(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation*) override{
		ceTarget * const target = pPanel.GetTarget();
		return target && comboBox.GetText() != target->GetActor()
			? ceUCTargetSetActor::Ref::New(target, comboBox.GetText()) : igdeUndo::Ref();
	}
};

class cTextTargetEntityID : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextTargetEntityID> Ref;
	cTextTargetEntityID(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceTarget * const target = pPanel.GetTarget();
		return target && textField.GetText() != target->GetCoordSystem()
			? ceUCTargetSetEntityID::Ref::New(target, textField.GetText()) : igdeUndo::Ref();
	}
};

class cTextTargetBone : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextTargetBone> Ref;
	cTextTargetBone(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceTarget * const target = pPanel.GetTarget();
		return target && textField.GetText() != target->GetBone()
			? ceUCTargetSetBone::Ref::New(target, textField.GetText()) : igdeUndo::Ref();
	}
};

class cVectorTargetPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorTargetPosition> Ref;
	cVectorTargetPosition(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceTarget * const target = pPanel.GetTarget();
		return target && !editVector.GetVector().IsEqualTo(target->GetPosition())
			? ceUCTargetSetPosition::Ref::New(target, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorTargetOrientation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorTargetOrientation> Ref;
	cVectorTargetOrientation(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceTarget * const target = pPanel.GetTarget();
		return target && !editVector.GetVector().IsEqualTo(target->GetOrientation())
			? ceUCTargetSetOrientation::Ref::New(target, editVector.GetVector()) : igdeUndo::Ref();
	}
};



class cComboCameraShot : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCameraShot> Ref;
	cComboCameraShot(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) override{
		conversation->SetActiveCameraShot(comboBox.GetSelectedItem()
			? (ceCameraShot*)comboBox.GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};

class cActionCameraShotAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraShotAdd> Ref;
	
public:
	cActionCameraShotAdd(ceWPConversation &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Camera Shot"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		decString name("Camera Shot");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Camera Shot", "Name:", name)){
			return {};
		}
		if(conversation->GetCameraShotList().HasMatching([&](const ceCameraShot &c){ return c.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Camera Shot", "Duplicate name");
			return {};
		}
		
		return ceUCCShotAdd::Ref::New(conversation, ceCameraShot::Ref::New(name));
	}
};

class cActionCameraShotRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraShotRemove> Ref;
	
public:
	cActionCameraShotRemove(ceWPConversation &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Camera Shot"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		return pPanel.GetCameraShot() ? ceUCCShotRemove::Ref::New(pPanel.GetCameraShot()) : ceUCCShotRemove::Ref();
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetCameraShot());
	}
};

class cActionCameraShotRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraShotRename> Ref;
	
public:
	cActionCameraShotRename(ceWPConversation &panel) : cBaseAction(panel,
	"Rename...", nullptr, "Rename Camera Shot"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		if(!cameraShot){
			return {};
		}
		
		decString name(cameraShot->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Camera Shot", "Name:", name) 
		|| name == cameraShot->GetName()){
			return {};
		}
		if(conversation->GetCameraShotList().HasMatching([&](const ceCameraShot &c){ return c.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Camera Shot", "Duplicate name");
			return {};
		}
		
		return ceUCCShotSetName::Ref::New(cameraShot, name);
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetCameraShot());
	}
};

class cActionCameraDuplicate : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraDuplicate> Ref;
	
public:
	cActionCameraDuplicate(ceWPConversation &panel) : cBaseAction(panel,
	"Duplicate...", nullptr, "Duplicate Camera Shot"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		if(!cameraShot){
			return {};
		}
		
		decString name(cameraShot->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Duplicate Camera Shot", "Name:", name)){
			return {};
		}
		if(conversation->GetCameraShotList().HasMatching([&](const ceCameraShot &c){ return c.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Duplicate Camera Shot", "Duplicate name");
			return {};
		}
		
		const ceCameraShot::Ref duplicate(ceCameraShot::Ref::New(*cameraShot));
		((ceCameraShot&)(deObject&)duplicate).SetName(name);
		return ceUCCShotAdd::Ref::New(conversation, duplicate);
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetCameraShot());
	}
};

class cActionCameraShotMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionCameraShotMenu> Ref;
	cActionCameraShotMenu(ceWPConversation &panel) : cBaseActionContextMenu(panel, "Camera Shot menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionCameraShotAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionCameraShotRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionCameraShotRename::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionCameraDuplicate::Ref::New(pPanel));
	}
};

class cTextCShotActorCount : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotActorCount> Ref;
	cTextCShotActorCount(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const int value = decMath::max(textField.GetInteger(), 0);
		return cameraShot && value != cameraShot->GetActorCount()
			? ceUCCShotSetActorCount::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};


class cComboCameraShotCameraTarget : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCameraShotCameraTarget> Ref;
	cComboCameraShotCameraTarget(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && comboBox.GetText() != cameraShot->GetCameraTarget()
			? ceUCCShotSetCameraTarget::Ref::New(cameraShot, comboBox.GetText()) : igdeUndo::Ref();
	}
};

class cVectorCShotOffCamFrom : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotOffCamFrom> Ref;
	cVectorCShotOffCamFrom(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetOffsetCameraFrom())
			? ceUCCShotSetOffCamFrom::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotOffCamTo : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotOffCamTo> Ref;
	cVectorCShotOffCamTo(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetOffsetCameraTo())
			? ceUCCShotSetOffCamTo::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};


class cComboCameraShotCameraLookAt : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCameraShotCameraLookAt> Ref;
	cComboCameraShotCameraLookAt(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && comboBox.GetText() != cameraShot->GetLookAtTarget()
			? ceUCCShotSetLookAtTarget::Ref::New(cameraShot, comboBox.GetText()) : igdeUndo::Ref();
	}
};

class cVectorCShotOffLookAtFrom : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotOffLookAtFrom> Ref;
	cVectorCShotOffLookAtFrom(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetOffsetLookAtFrom())
			? ceUCCShotSetOffLookAtFrom::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotOffLookAtTo : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotOffLookAtTo> Ref;
	cVectorCShotOffLookAtTo(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetOffsetLookAtTo())
			? ceUCCShotSetOffLookAtTo::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotCamOrbitFrom : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotCamOrbitFrom> Ref;
	cVectorCShotCamOrbitFrom(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetCameraOrbitFrom())
			? ceUCCShotSetCamOrbitFrom::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotCamOrbitTo : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotCamOrbitTo> Ref;
	cVectorCShotCamOrbitTo(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetCameraOrbitTo())
			? ceUCCShotSetCamOrbitTo::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cTextCShotCamDistanceFrom : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotCamDistanceFrom> Ref;
	cTextCShotCamDistanceFrom(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf(value - cameraShot->GetCameraDistanceFrom()) > FLOAT_SAFE_EPSILON
			? ceUCCShotSetCamDistFrom::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};

class cTextCShotCamDistanceTo : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotCamDistanceTo> Ref;
	cTextCShotCamDistanceTo(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf(value - cameraShot->GetCameraDistanceTo()) > FLOAT_SAFE_EPSILON
			? ceUCCShotSetCamDistTo::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};


class cVectorCShotPositionFrom : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotPositionFrom> Ref;
	cVectorCShotPositionFrom(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetPositionFrom())
			? ceUCCShotSetPosFrom::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotPositionTo : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotPositionTo> Ref;
	cVectorCShotPositionTo(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetPositionTo())
			? ceUCCShotSetPosTo::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotRotationFrom : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotRotationFrom> Ref;
	cVectorCShotRotationFrom(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetRotationFrom())
			? ceUCCShotSetRotFrom::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cVectorCShotRotationTo : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cVectorCShotRotationTo> Ref;
	cVectorCShotRotationTo(ceWPConversation &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector &editVector, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && !editVector.GetVector().IsEqualTo(cameraShot->GetRotationTo())
			? ceUCCShotSetRotTo::Ref::New(cameraShot, editVector.GetVector()) : igdeUndo::Ref();
	}
};

class cTextCShotTiltFrom : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotTiltFrom> Ref;
	cTextCShotTiltFrom(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf(value - cameraShot->GetTiltFrom()) > FLOAT_SAFE_EPSILON
			? ceUCCShotSetTiltFrom::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};

class cTextCShotTiltTo : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotTiltTo> Ref;
	cTextCShotTiltTo(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf(value - cameraShot->GetTiltTo()) > FLOAT_SAFE_EPSILON
			? ceUCCShotSetTiltTo::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};

class cTextCShotFovFrom : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotFovFrom> Ref;
	cTextCShotFovFrom(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf(value - cameraShot->GetFovFrom()) > FLOAT_SAFE_EPSILON
			? ceUCCShotSetFovFrom::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};

class cTextCShotFovTo : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCShotFovTo> Ref;
	cTextCShotFovTo(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf(value - cameraShot->GetFovTo()) > FLOAT_SAFE_EPSILON
			? ceUCCShotSetFovTo::Ref::New(cameraShot, value) : igdeUndo::Ref();
	}
};

class cActionCShotAlignTargets : public cBaseAction{
public:
	typedef deTObjectReference<cActionCShotAlignTargets> Ref;
	
public:
	cActionCShotAlignTargets(ceWPConversation &panel) : cBaseAction(panel,
		"Align Targets", nullptr, "Targets are aligned to face each other"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? ceUCCShotToggleAlignTargets::Ref::New(cameraShot) : ceUCCShotToggleAlignTargets::Ref();
	}
	
	void Update(const ceConversation &) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled(cameraShot);
		SetSelected(cameraShot && cameraShot->GetAlignTargets());
	}
};

class cActionCShotLockUpAxis : public cBaseAction{
public:
	typedef deTObjectReference<cActionCShotLockUpAxis> Ref;
	
public:
	cActionCShotLockUpAxis(ceWPConversation &panel) : cBaseAction(panel,
		"Align Target Lock Up-Axis", nullptr, "Target up axes are locked during aligning"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? ceUCCShotToggleLockUpAxis::Ref::New(cameraShot) : ceUCCShotToggleLockUpAxis::Ref();
	}
	
	void Update(const ceConversation &) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled(cameraShot);
		SetSelected(cameraShot && cameraShot->GetLockUpAxis());
	}
};

class cActionCShotRelativeToLookAt : public cBaseAction{
public:
	typedef deTObjectReference<cActionCShotRelativeToLookAt> Ref;
	
public:
	cActionCShotRelativeToLookAt(ceWPConversation &panel) : cBaseAction(panel,
		"Anchor Relative to Look-At", nullptr, "Camera is relative to the camera target or the look-at target"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? ceUCCShotToggleRelativeToLookAt::Ref::New(cameraShot) : ceUCCShotToggleRelativeToLookAt::Ref();
	}
	
	void Update(const ceConversation &) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled(cameraShot);
		SetSelected(cameraShot && cameraShot->GetRelativeToLookAt());
	}
};

class cActionCShotLockCameraTarget : public cBaseAction{
public:
	typedef deTObjectReference<cActionCShotLockCameraTarget> Ref;
	
public:
	cActionCShotLockCameraTarget(ceWPConversation &panel) : cBaseAction(panel,
		"Lock Target Camera", nullptr, "Camera target position is locked during the entire camera shot"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? ceUCCShotToggleLockCameraTarget::Ref::New(cameraShot) : ceUCCShotToggleLockCameraTarget::Ref();
	}
	
	void Update(const ceConversation &) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled(cameraShot);
		SetSelected(cameraShot && cameraShot->GetLockCameraTarget());
	}
};

class cActionCShotLockLookAtTarget : public cBaseAction{
public:
	typedef deTObjectReference<cActionCShotLockLookAtTarget> Ref;
	
public:
	cActionCShotLockLookAtTarget(ceWPConversation &panel) : cBaseAction(panel,
		"Lock Target Look-At", nullptr, "Look-at target position is locked during the entire camera shot"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? ceUCCShotToggleLockLookAtTarget::Ref::New(cameraShot) : ceUCCShotToggleLockLookAtTarget::Ref();
	}
	
	void Update(const ceConversation &) override{
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled(cameraShot);
		SetSelected(cameraShot && cameraShot->GetLockLookAtTarget());
	}
};



class cComboGesture : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboGesture> Ref;
	cComboGesture(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) override{
		conversation->SetActiveGesture(comboBox.GetSelectedItem()
			? (ceGesture*)comboBox.GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};

class cActionGestureAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionGestureAdd> Ref;
	
public:
	cActionGestureAdd(ceWPConversation &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Gesture"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		decString name("Gesture");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Gesture", "Name:", name)){
			return {};
		}
		if(conversation->GetGestures().HasMatching([&](const ceGesture &g){ return g.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Gesture", "Duplicate name");
			return {};
		}
		
		const ceGesture::Ref gesture(ceGesture::Ref::New(name));
		return ceUCGestureAdd::Ref::New(conversation, gesture);
	}
};

class cActionGestureRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionGestureRemove> Ref;
	
public:
	cActionGestureRemove(ceWPConversation &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Gesture"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		return pPanel.GetGesture() ? ceUCGestureRemove::Ref::New(pPanel.GetGesture()) : ceUCGestureRemove::Ref();
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetGesture());
	}
};

class cActionGestureRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionGestureRename> Ref;
	
public:
	cActionGestureRename(ceWPConversation &panel) : cBaseAction(panel,
	"Rename...", nullptr, "Rename Gesture"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceGesture * const gesture = pPanel.GetGesture();
		if(!gesture){
			return {};
		}
		
		decString name(gesture->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Gesture", "Name:", name) 
		|| name == gesture->GetName()){
			return {};
		}
		if(conversation->GetGestures().HasMatching([&](const ceGesture &g){ return g.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Gesture", "Duplicate name");
			return {};
		}
		
		return ceUCGestureSetName::Ref::New(gesture, name);
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetGesture());
	}
};

class cActionGestureMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionGestureMenu> Ref;
	cActionGestureMenu(ceWPConversation &panel) : cBaseActionContextMenu(panel, "Gesture menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionGestureAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionGestureRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionGestureRename::Ref::New(pPanel));
	}
};

class cTextGestureAnimator : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextGestureAnimator> Ref;
	cTextGestureAnimator(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceGesture * const gesture = pPanel.GetGesture();
		return gesture && textField.GetText() != gesture->GetAnimator()
			? ceUCGestureSetAnimator::Ref::New(gesture, textField.GetText()) : igdeUndo::Ref();
	}
};

class cActionGestureHold : public cBaseAction{
public:
	typedef deTObjectReference<cActionGestureHold> Ref;
	
public:
	cActionGestureHold(ceWPConversation &panel) : cBaseAction(panel,
		"Hold Gesture", nullptr, "Hold gesture after gesture finished"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceGesture * const gesture = pPanel.GetGesture();
		return gesture ? ceUCGestureToggleHold::Ref::New(gesture) : ceUCGestureToggleHold::Ref();
	}
	
	void Update(const ceConversation &) override{
		ceGesture * const gesture = pPanel.GetGesture();
		SetEnabled(gesture);
		SetSelected(gesture && gesture->GetHold());
	}
};

class cTextGestureDuration : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextGestureDuration> Ref;
	cTextGestureDuration(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceGesture * const gesture = pPanel.GetGesture();
		const float value = textField.GetFloat();
		return gesture && fabsf(value - gesture->GetDuration()) > FLOAT_SAFE_EPSILON
			? ceUCGestureSetDuration::Ref::New(gesture, value) : igdeUndo::Ref();
	}
};



class cComboFacePose : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboFacePose> Ref;
	cComboFacePose(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, ceConversation *conversation) override{
		conversation->SetActiveFacePose(comboBox.GetSelectedItem()
			? (ceFacePose*)comboBox.GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};

class cActionFacePoseAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionFacePoseAdd> Ref;
	
public:
	cActionFacePoseAdd(ceWPConversation &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Face Pose"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		decString name("FacePose");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Face Pose", "Name:", name)){
			return {};
		}
		if(conversation->GetFacePoseList().HasMatching([&](const ceFacePose &f){ return f.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Face Pose", "Duplicate name");
			return {};
		}
		
		const ceFacePose::Ref facePose(ceFacePose::Ref::New(name));
		return ceUCFacePoseAdd::Ref::New(conversation, facePose);
	}
};

class cActionFacePoseRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionFacePoseRemove> Ref;
	
public:
	cActionFacePoseRemove(ceWPConversation &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Face Pose"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		return pPanel.GetFacePose() ? ceUCFacePoseRemove::Ref::New(pPanel.GetFacePose()) : ceUCFacePoseRemove::Ref();
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetFacePose());
	}
};

class cActionFacePoseRename : public cBaseAction{
public:
	typedef deTObjectReference<cActionFacePoseRename> Ref;
	
public:
	cActionFacePoseRename(ceWPConversation &panel) : cBaseAction(panel,
	"Rename...", nullptr, "Rename Face Pose"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceFacePose * const facePose = pPanel.GetFacePose();
		if(!facePose){
			return {};
		}
		
		decString name(facePose->GetName());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Face Pose", "Name:", name) 
		|| name == facePose->GetName()){
			return {};
		}
		if(conversation->GetFacePoseList().HasMatching([&](const ceFacePose &f){ return f.GetName() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Face Pose", "Duplicate name");
			return {};
		}
		
		return ceUCFacePoseSetName::Ref::New(facePose, name);
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetFacePose());
	}
};

class cActionFacePoseMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionFacePoseMenu> Ref;
	cActionFacePoseMenu(ceWPConversation &panel) : cBaseActionContextMenu(panel, "Face Pose menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionFacePoseAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionFacePoseRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionFacePoseRename::Ref::New(pPanel));
	}
};

class cComboFacePoseController : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboFacePoseController> Ref;
	cComboFacePoseController(ceWPConversation &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox&, ceConversation*) override{
		ceFacePose * const facePose = pPanel.GetFacePose();
		if(facePose){
			pPanel.UpdateFPController();
		}
		return {};
	}
};

class cActionFacePoseControllerAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionFacePoseControllerAdd> Ref;
	
public:
	cActionFacePoseControllerAdd(ceWPConversation &panel) : cBaseAction(panel, "Add...",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Face Pose Controller"){}
	
	igdeUndo::Ref OnAction(ceConversation *conversation) override{
		ceFacePose * const facePose = pPanel.GetFacePose();
		if(!facePose || conversation->GetFacePoseControllerNameList().GetCount() == 0){
			return {};
		}
		
		decStringList names(conversation->GetFacePoseControllerNameList());
		names.SortAscending();
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Face Pose Controller", "Controller to add", name, names)){
			return {};
		}
		
		if(facePose->GetControllers().HasMatching([&](const ceControllerValue &c){ return c.GetController() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Face Pose Controller", "Duplicate controller");
			return {};
		}
		
		const ceControllerValue::Ref controller(ceControllerValue::Ref::New(name, 1.0f));
		ceUCFPControllerAdd::Ref undo(ceUCFPControllerAdd::Ref::New(facePose, controller));
		conversation->GetUndoSystem()->Add(undo);
		
		pPanel.SelectFacePoseController(controller);
		return {};
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetFacePose());
	}
};

class cActionFacePoseControllerRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionFacePoseControllerRemove> Ref;
	
public:
	cActionFacePoseControllerRemove(ceWPConversation &panel) : cBaseAction(panel, "Remove",
	panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Face Pose Controller"){}
	
	igdeUndo::Ref OnAction(ceConversation*) override{
		ceFacePose * const facePose = pPanel.GetFacePose();
		ceControllerValue * const controller = pPanel.GetFacePoseController();
		return facePose && controller ? ceUCFPControllerRemove::Ref::New(facePose, controller) : ceUCFPControllerRemove::Ref();
	}
	
	void Update(const ceConversation &) override{
		SetEnabled(pPanel.GetFacePoseController());
	}
};

class cActionFacePoseControllerMenu : public cBaseActionContextMenu{
public:
	typedef deTObjectReference<cActionFacePoseControllerMenu> Ref;
	cActionFacePoseControllerMenu(ceWPConversation &panel) :
	cBaseActionContextMenu(panel, "Face Pose Controller menu"){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu, ceConversation*){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionFacePoseControllerAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionFacePoseControllerRemove::Ref::New(pPanel));
	}
};

class cTextFPControllerValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextFPControllerValue> Ref;
	cTextFPControllerValue(ceWPConversation &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, ceConversation*) override{
		ceControllerValue * const controller = pPanel.GetFacePoseController();
		const float value = textField.GetFloat();
		return controller && fabsf(value - controller->GetValue()) > FLOAT_SAFE_EPSILON
			? ceUCFPControllerSetValue::Ref::New(pPanel.GetFacePose(), controller, value) : igdeUndo::Ref();
	}
};

}



// Class ceWPConversation
///////////////////////////

// Constructor, destructor
////////////////////////////

ceWPConversation::ceWPConversation(ceWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, groupBox2, form, formLine;
	igdeActionContextMenu::Ref actionContextMenu;
	
	pListener = ceWPConversationListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// import conversation
	helper.GroupBoxFlow(content, groupBox, "Import Conversation:", true, true);
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	helper.EditPath(formLine, "Path to add to import list", *windowProperties.GetWindowMain()
		.GetLoadSaveSystem()->GetConversationFilePatterns(), pPathImportConvo, {});
	helper.Button(formLine, cActionImportConvoAdd::Ref::New(*this));
	groupBox->AddChild(formLine);
	helper.ListBox(groupBox, 3, "Imported conversations", pListImportConvoPath, cListImportConvo::Ref::New(*this));
	
	
	// targets
	helper.GroupBox(content, groupBox, "Targets:", true);
	
	helper.FormLineStretchFirst(groupBox, "Target:", "Target to edit", formLine);
	helper.ComboBoxFilter(formLine, "Target to edit", pCBTarget, cComboTarget::Ref::New(*this));
	pCBTarget->SetDefaultSorter();
	actionContextMenu = cActionTargetMenu::Ref::New(*this);
	helper.Button(formLine, pBtnTarget, actionContextMenu);
	actionContextMenu->SetWidget(pBtnTarget);
	
	helper.ComboBoxFilter(groupBox, "Actor:", true, "ID of the actor or empty to not use an actor by ID",
		pCBTargetActorID, cComboTargetActorID::Ref::New(*this));
	pCBTargetActorID->SetDefaultSorter();
	
	helper.EditString(groupBox, "Coord-System:", "ID of coordinate system or empty to not use a coordinate system",
		pEditTargetEntityID, cTextTargetEntityID::Ref::New(*this));
	helper.EditString(groupBox, "Bone:",
		"Name of the bone to use as coordinate frame or an empty string to use the origin",
		pEditTargetBone, cTextTargetBone::Ref::New(*this));
	helper.EditVector(groupBox, "Position:", "Position relative to the target object",
		pEditTargetPosition, cVectorTargetPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Orientation:", "Orientation relative to the target object",
		pEditTargetOrientation, cVectorTargetOrientation::Ref::New(*this));
	
	
	// camera shots
	helper.GroupBoxFlow(content, groupBox, "Camera Shots:", false, false);
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	helper.FormLineStretchFirst(form, "Camera Shot:", "Camera Shot to edit", formLine);
	helper.ComboBoxFilter(formLine, "Camera Shot to edit", pCBCameraShot, cComboCameraShot::Ref::New(*this));
	pCBCameraShot->SetDefaultSorter();
	actionContextMenu = cActionCameraShotMenu::Ref::New(*this);
	helper.Button(formLine, pBtnCameraShot, actionContextMenu);
	actionContextMenu->SetWidget(pBtnCameraShot);
	
	helper.EditInteger(form, "Actor Count:", "Number of actors required for this camera action",
		pEditCShotActorCount, cTextCShotActorCount::Ref::New(*this));
	
	// camera shot camera target
	helper.GroupBox(groupBox, groupBox2, "Camera Target:", false);
	
	helper.ComboBoxFilter(groupBox2, "Target:", true,
		"Target the camera is attached to or empty string for static position",
		pCBCameraShotCameraTarget, cComboCameraShotCameraTarget::Ref::New(*this));
	pCBCameraShotCameraTarget->SetDefaultSorter();
	
	helper.EditVector(groupBox2, "Offset From:", "Start offset relative to the target",
		pEditCShotOffCamFrom, cVectorCShotOffCamFrom::Ref::New(*this));
	helper.EditVector(groupBox2, "Offset To:", "End offset relative to the target",
		pEditCShotOffCamTo, cVectorCShotOffCamTo::Ref::New(*this));
	
	// camera shot look at target
	helper.GroupBox(groupBox, groupBox2, "Camera Look-At:", false);
	
	helper.ComboBoxFilter(groupBox2, "Look-At Target:", true,
		"Target to point the camera at or empty string for static position",
		pCBCameraShotLookAtTarget, cComboCameraShotCameraLookAt::Ref::New(*this));
	pCBCameraShotLookAtTarget->SetDefaultSorter();
	
	helper.EditVector(groupBox2, "Offset From:", "Start offset relative to the look-at target",
		pEditCShotOffLookAtFrom, cVectorCShotOffLookAtFrom::Ref::New(*this));
	helper.EditVector(groupBox2, "Offset To:", "End offset relative to the look-at target",
		pEditCShotOffLookAtTo, cVectorCShotOffLookAtTo::Ref::New(*this));
	
	// camera shot camera target
	helper.GroupBox(groupBox, groupBox2, "Orbiting:", false);
	
	helper.EditVector(groupBox2, "Orbit From:", "Start rotation of the camera orbit",
		pEditCShotCamOrbitFrom, cVectorCShotCamOrbitFrom::Ref::New(*this));
	helper.EditVector(groupBox2, "Orbit To:", "End rotation of the camera orbit",
		pEditCShotCamOrbitTo, cVectorCShotCamOrbitTo::Ref::New(*this));
	
	helper.EditFloat(groupBox2, "Distance From:", "Start distance of the camera to the orbit position",
		pEditCShotCamDistFrom, cTextCShotCamDistanceFrom::Ref::New(*this));
	helper.EditFloat(groupBox2, "Distance To:", "End distance of the camera to the orbit position",
		pEditCShotCamDistTo, cTextCShotCamDistanceTo::Ref::New(*this));
	
	// camera shot camera target
	helper.GroupBox(groupBox, groupBox2, "Parameters:", false);
	
	helper.EditVector(groupBox2, "Position From:", "Start position",
		pEditCShotPosFrom, cVectorCShotPositionFrom::Ref::New(*this));
	helper.EditVector(groupBox2, "Position To:", "End position",
		pEditCShotPosTo, cVectorCShotPositionTo::Ref::New(*this));
	
	helper.EditVector(groupBox2, "Rotation From:", "Start rotation",
		pEditCShotRotFrom, cVectorCShotRotationFrom::Ref::New(*this));
	helper.EditVector(groupBox2, "Rotation To:", "End rotation",
		pEditCShotRotTo, cVectorCShotRotationTo::Ref::New(*this));
	
	helper.EditFloat(groupBox2, "Tilt From:", "Start tilt of the camera along the view axis",
		pEditCShotTiltFrom, cTextCShotTiltFrom::Ref::New(*this));
	helper.EditFloat(groupBox2, "Tilt To:", "End tilt of the camera along the view axis",
		pEditCShotTiltTo, cTextCShotTiltTo::Ref::New(*this));
	
	helper.EditFloat(groupBox2, "Fov From:", "Start fov of the camera along the view axis",
		pEditCShotFovFrom, cTextCShotFovFrom::Ref::New(*this));
	helper.EditFloat(groupBox2, "Fov To:", "End fov of the camera along the view axis",
		pEditCShotFovTo, cTextCShotFovTo::Ref::New(*this));
	
	helper.CheckBox(groupBox2, pChkCShotAlignTargets, cActionCShotAlignTargets::Ref::New(*this));
	helper.CheckBox(groupBox2, pChkCShotLockUpAxis, cActionCShotLockUpAxis::Ref::New(*this));
	helper.CheckBox(groupBox2, pChkCShotRelativeToLookAt, cActionCShotRelativeToLookAt::Ref::New(*this));
	helper.CheckBox(groupBox2, pChkCShotLockCameraTarget, cActionCShotLockCameraTarget::Ref::New(*this));
	helper.CheckBox(groupBox2, pChkCShotLockLookAtTarget, cActionCShotLockLookAtTarget::Ref::New(*this));
	
	
	// gestures
	helper.GroupBox(content, groupBox, "Gestures:", true);
	
	helper.FormLineStretchFirst(groupBox, "Gesture:", "Gesture to edit", formLine);
	helper.ComboBoxFilter(formLine, "Gesture to edit", pCBGesture, cComboGesture::Ref::New(*this));
	pCBGesture->SetDefaultSorter();
	actionContextMenu = cActionGestureMenu::Ref::New(*this);
	helper.Button(formLine, pBtnGesture, actionContextMenu);
	actionContextMenu->SetWidget(pBtnGesture);
	
	helper.EditString(groupBox, "Animator:", "Name of the animator to use for this gesture",
		pEditGestureAnimator, cTextGestureAnimator::Ref::New(*this));
	
	helper.EditFloat(groupBox, "Duration:", "Duration of gesture. Used as default value in strips",
		pEditGestureDuration, cTextGestureDuration::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkGestureHold, cActionGestureHold::Ref::New(*this));
	
	
	// facePoses
	helper.GroupBox(content, groupBox, "Face Poses:", true);
	
	helper.FormLineStretchFirst(groupBox, "Face Pose:", "Face Pose to edit", formLine);
	helper.ComboBoxFilter(formLine, "Face Pose to edit", pCBFacePose, cComboFacePose::Ref::New(*this));
	pCBFacePose->SetDefaultSorter();
	actionContextMenu = cActionFacePoseMenu::Ref::New(*this);
	helper.Button(formLine, pBtnFacePose, actionContextMenu);
	actionContextMenu->SetWidget(pBtnFacePose);
	
	helper.FormLineStretchFirst(groupBox, "Controller:", "Face pose controller to edit", formLine);
	helper.ComboBox(formLine, "Face pose controller to edit", pCBFPController, cComboFacePoseController::Ref::New(*this));
	pCBFPController->SetDefaultSorter();
	actionContextMenu = cActionFacePoseControllerMenu::Ref::New(*this);
	helper.Button(formLine, pBtnFPController, actionContextMenu);
	actionContextMenu->SetWidget(pBtnFPController);
	
	helper.EditFloat(groupBox, "Value:", "Value of face pose controller",
		pEditFPControllerValue, cTextFPControllerValue::Ref::New(*this));
}

ceWPConversation::~ceWPConversation(){
	SetConversation(nullptr);
}



// Management
///////////////

void ceWPConversation::SetConversation(ceConversation *conversation){
	if(conversation == pConversation){
		return;
	}
	
	if(pConversation){
		pConversation->RemoveListener(pListener);
	}
	
	pConversation = conversation;
	
	if(conversation){
		conversation->AddListener(pListener);
	}
	
	const bool enabled = conversation;
	pCBGesture->SetEnabled(enabled);
	pCBFacePose->SetEnabled(enabled);
	
	pBtnGesture->GetAction()->Update();
	pBtnFacePose->GetAction()->Update();
	
	if(!enabled){
		pCBGesture->ClearText();
		pCBFacePose->ClearText();
	}
	
	UpdateTargetList();
	UpdateCameraShotList();
	UpdateConversation();
	UpdateGestureList();
	UpdateFacePoseList();
	
	UpdateActorIDLists();
	OnConversationPathChanged();
}



void ceWPConversation::UpdateConversation(){
	UpdateImportConvoPathList();
}

void ceWPConversation::OnConversationPathChanged(){
	if(pConversation){
		pPathImportConvo->SetBasePath(pConversation->GetDirectoryPath());
		
	}else{
		pPathImportConvo->SetBasePath("");
	}
}


void ceWPConversation::UpdateImportConvoPathList(){
	const decString selection(GetImportConvo());
	
	pListImportConvoPath->RemoveAllItems();
	
	if(pConversation){
		const decStringList &list = pConversation->GetImportConversationPath();
		const int count = list.GetCount();
		int i;
		for(i=0; i<count; i++){
			pListImportConvoPath->AddItem(list.GetAt(i));
		}
	}
	
	if(!selection.IsEmpty()){
		pListImportConvoPath->SetSelection(pListImportConvoPath->IndexOfItem(selection));
	}
	if(pListImportConvoPath->GetSelection() == -1 && pListImportConvoPath->GetItems().IsNotEmpty()){
		pListImportConvoPath->SetSelection(0);
	}
}

const decString & ceWPConversation::GetPathImportConvo() const{
	return pPathImportConvo->GetPath();
}

decString ceWPConversation::GetImportConvo() const{
	return pListImportConvoPath->GetSelectedItem()
		? pListImportConvoPath->GetSelectedItem()->GetText() : decString();
}



ceGesture *ceWPConversation::GetGesture() const{
	return pConversation ? pConversation->GetActiveGesture() : nullptr;
}

void ceWPConversation::UpdateGestureList(){
	ceGesture * const selection = GetGesture();
	
	pCBGesture->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetGestures().Visit([&](ceGesture *g){
			pCBGesture->AddItem(g->GetName(), nullptr, g);
		});
		
		pCBGesture->SortItems();
		pCBGesture->StoreFilterItems();
	}
	
	if(pConversation){
		pConversation->SetActiveGesture(selection);
	}
}

void ceWPConversation::SelectActiveGesture(){
	pCBGesture->SetSelectionWithData(GetGesture());
	
	const bool enabled = GetGesture();
	pEditGestureAnimator->SetEnabled(enabled);
	
	if(!enabled){
		pEditGestureAnimator->ClearText();
	}
	
	UpdateGesture();
}

void ceWPConversation::UpdateGesture(){
	ceGesture * const gesture = GetGesture();
	if(gesture){
		pEditGestureAnimator->SetText(gesture->GetAnimator());
		pEditGestureDuration->SetFloat(gesture->GetDuration());
	}
	
	pChkGestureHold->GetAction()->Update();
}



ceFacePose *ceWPConversation::GetFacePose() const{
	return pConversation ? pConversation->GetActiveFacePose() : nullptr;
}

void ceWPConversation::UpdateFacePoseList(){
	ceFacePose * const selection = GetFacePose();
	
	pCBFacePose->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetFacePoseList().Visit([&](ceFacePose *f){
			pCBFacePose->AddItem(f->GetName(), nullptr, f);
		});
		
		pCBFacePose->SortItems();
		pCBFacePose->StoreFilterItems();
	}
	
	if(pConversation){
		pConversation->SetActiveFacePose(selection);
	}
}

void ceWPConversation::SelectActiveFacePose(){
	pCBFacePose->SetSelectionWithData(GetFacePose());
	
	const bool enabled = GetFacePose();
	pCBFPController->SetEnabled(enabled);
	
	if(!enabled){
		pCBFPController->ClearText();
	}
	
	UpdateFacePose();
}

void ceWPConversation::UpdateFacePose(){
	UpdateFPControllerList();
}

ceControllerValue *ceWPConversation::GetFacePoseController() const{
	ceFacePose * const facePose = GetFacePose();
	return facePose && pCBFPController->GetSelectedItem()
		? (ceControllerValue*)pCBFPController->GetSelectedItem()->GetData() : nullptr;
}

void ceWPConversation::UpdateFPControllerList(){
	ceFacePose * const facePose = GetFacePose();
	ceControllerValue * const controller = GetFacePoseController();
	
	pCBFPController->RemoveAllItems();
	
	if(facePose){
		const decStringList &controllerNames = pConversation->GetFacePoseControllerNameList();
		facePose->GetControllers().Visit([&](ceControllerValue *c){
			decString text;
			if(c->GetControllerIndex() == -1){
				if(controllerNames.Has(c->GetController())){
					text = c->GetController();
					
				}else{
					text.Format("%s (missing)", c->GetController().GetString());
				}
				
			}else{ // deprecated
				text.Format("%d (deprecated)", c->GetControllerIndex());
			}
			
			pCBFPController->AddItem(text, nullptr, c);
		});
	}
	
	pCBFPController->SetSelectionWithData(controller);
	if(pCBFPController->GetSelection() == -1 && pCBFPController->GetItems().IsNotEmpty()){
		pCBFPController->SetSelection(0);
	}
	UpdateFPController();
}

void ceWPConversation::SelectFacePoseController(ceControllerValue *controller){
	pCBFPController->SetSelectionWithData(controller);
}

void ceWPConversation::UpdateFPController(){
	ceControllerValue * const controller = GetFacePoseController();
	
	if(controller){
		pEditFPControllerValue->SetFloat(controller->GetValue());
		
	}else{
		pEditFPControllerValue->ClearText();
	}
	
	pEditFPControllerValue->SetEnabled(controller);
}



ceCameraShot *ceWPConversation::GetCameraShot() const{
	return pConversation ? pConversation->GetActiveCameraShot() : nullptr;
}

void ceWPConversation::UpdateCameraShotList(){
	ceCameraShot * const selection = GetCameraShot();
	
	pCBCameraShot->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetCameraShotList().Visit([&](ceCameraShot *c){
			pCBCameraShot->AddItem(c->GetName(), nullptr, c);
		});
		
		pCBCameraShot->SortItems();
		pCBCameraShot->StoreFilterItems();
	}
	
	if(pConversation){
		pConversation->SetActiveCameraShot(selection);
	}
}

void ceWPConversation::SelectActiveCameraShot(){
	ceCameraShot * const cameraShot = GetCameraShot();
	
	pCBCameraShot->SetSelectionWithData(cameraShot);
	
	const bool enabled = cameraShot;
	pEditCShotActorCount->SetEnabled(enabled);
	pCBCameraShotCameraTarget->SetEnabled(enabled);
	pEditCShotOffCamFrom->SetEnabled(enabled);
	pEditCShotOffCamTo->SetEnabled(enabled);
	pEditCShotCamOrbitFrom->SetEnabled(enabled);
	pEditCShotCamOrbitTo->SetEnabled(enabled);
	pEditCShotCamDistFrom->SetEnabled(enabled);
	pEditCShotCamDistTo->SetEnabled(enabled);
	pCBCameraShotLookAtTarget->SetEnabled(enabled);
	pEditCShotOffLookAtFrom->SetEnabled(enabled);
	pEditCShotOffLookAtTo->SetEnabled(enabled);
	pEditCShotPosFrom->SetEnabled(enabled);
	pEditCShotPosTo->SetEnabled(enabled);
	pEditCShotRotFrom->SetEnabled(enabled);
	pEditCShotRotTo->SetEnabled(enabled);
	pEditCShotTiltFrom->SetEnabled(enabled);
	pEditCShotTiltTo->SetEnabled(enabled);
	pEditCShotFovFrom->SetEnabled(enabled);
	pEditCShotFovTo->SetEnabled(enabled);
	pChkCShotLockUpAxis->SetEnabled(enabled);
	pChkCShotAlignTargets->SetEnabled(enabled);
	pChkCShotRelativeToLookAt->SetEnabled(enabled);
	pChkCShotLockCameraTarget->SetEnabled(enabled);
	pChkCShotLockLookAtTarget->SetEnabled(enabled);
	
	if(!enabled){
		pEditCShotActorCount->ClearText();
		pEditCShotOffCamFrom->SetVector(decVector());
		pEditCShotOffCamTo->SetVector(decVector());
		pEditCShotCamOrbitFrom->SetVector(decVector());
		pEditCShotCamOrbitTo->SetVector(decVector());
		pEditCShotCamDistFrom->ClearText();
		pEditCShotCamDistTo->ClearText();
		pEditCShotOffLookAtFrom->SetVector(decVector());
		pEditCShotOffLookAtTo->SetVector(decVector());
		pEditCShotPosFrom->SetVector(decVector());
		pEditCShotPosTo->SetVector(decVector());
		pEditCShotRotFrom->SetVector(decVector());
		pEditCShotRotTo->SetVector(decVector());
		pEditCShotTiltFrom->ClearText();
		pEditCShotTiltTo->ClearText();
		pEditCShotFovFrom->ClearText();
		pEditCShotFovTo->ClearText();
	}
	
	UpdateCameraShot();
}

void ceWPConversation::UpdateCameraShot(){
	const ceCameraShot * const cameraShot = GetCameraShot();
	
	if(cameraShot){
		pEditCShotActorCount->SetInteger(cameraShot->GetActorCount());
		pCBCameraShotCameraTarget->SetText(cameraShot->GetCameraTarget());
		pEditCShotOffCamFrom->SetVector(cameraShot->GetOffsetCameraFrom());
		pEditCShotOffCamTo->SetVector(cameraShot->GetOffsetCameraTo());
		pEditCShotCamOrbitFrom->SetVector(cameraShot->GetCameraOrbitFrom());
		pEditCShotCamOrbitTo->SetVector(cameraShot->GetCameraOrbitTo());
		pEditCShotCamDistFrom->SetFloat(cameraShot->GetCameraDistanceFrom());
		pEditCShotCamDistTo->SetFloat(cameraShot->GetCameraDistanceTo());
		pCBCameraShotLookAtTarget->SetText(cameraShot->GetLookAtTarget());
		pEditCShotOffLookAtFrom->SetVector(cameraShot->GetOffsetLookAtFrom());
		pEditCShotOffLookAtTo->SetVector(cameraShot->GetOffsetLookAtTo());
		pEditCShotPosFrom->SetVector(cameraShot->GetPositionFrom());
		pEditCShotPosTo->SetVector(cameraShot->GetPositionTo());
		pEditCShotRotFrom->SetVector(cameraShot->GetRotationFrom());
		pEditCShotRotTo->SetVector(cameraShot->GetRotationTo());
		pEditCShotTiltFrom->SetFloat(cameraShot->GetTiltFrom());
		pEditCShotTiltTo->SetFloat(cameraShot->GetTiltTo());
		pEditCShotFovFrom->SetFloat(cameraShot->GetFovFrom());
		pEditCShotFovTo->SetFloat(cameraShot->GetFovTo());
	}
	
	pChkCShotLockUpAxis->GetAction()->Update();
	pChkCShotAlignTargets->GetAction()->Update();
	pChkCShotRelativeToLookAt->GetAction()->Update();
	pChkCShotLockCameraTarget->GetAction()->Update();
	pChkCShotLockLookAtTarget->GetAction()->Update();
}



ceTarget *ceWPConversation::GetTarget() const{
	return pConversation ? pConversation->GetActiveTarget() : nullptr;
}

void ceWPConversation::UpdateTargetList(){
	ceTarget * const selection = GetTarget();
	
	pCBTarget->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetTargets().Visit([&](ceTarget *t){
			pCBTarget->AddItem(t->GetName(), nullptr, t);
		});
		pCBTarget->SortItems();
		pCBTarget->StoreFilterItems();
	}
	
	if(pConversation){
		pConversation->SetActiveTarget(selection);
	}
	
	// update lists containing targets
	const decString selCShotCamera(pCBCameraShotCameraTarget->GetText());
	const decString selCShotLookAt(pCBCameraShotLookAtTarget->GetText());
	
	pCBCameraShotCameraTarget->RemoveAllItems();
	pCBCameraShotLookAtTarget->RemoveAllItems();
	
	if(pConversation){
		pConversation->AllTargets().Visit([&](const ceTarget &t){
			if(!t.GetName().IsEmpty()){
				pCBCameraShotCameraTarget->AddItem(t.GetName());
				pCBCameraShotLookAtTarget->AddItem(t.GetName());
			}
		});
		pCBCameraShotCameraTarget->SortItems();
		pCBCameraShotLookAtTarget->SortItems();
	}
	
	pCBCameraShotCameraTarget->StoreFilterItems();
	pCBCameraShotLookAtTarget->StoreFilterItems();
	
	pCBCameraShotCameraTarget->SetText(selCShotCamera);
	pCBCameraShotLookAtTarget->SetText(selCShotLookAt);
}

void ceWPConversation::SelectActiveTarget(){
	pCBTarget->SetSelectionWithData(GetTarget());
	
	const bool enabled = GetTarget();
	pCBTargetActorID->SetEnabled(enabled);
	pEditTargetEntityID->SetEnabled(enabled);
	pEditTargetBone->SetEnabled(enabled);
	pEditTargetPosition->SetEnabled(enabled);
	pEditTargetOrientation->SetEnabled(enabled);
	
	if(!enabled){
		pEditTargetEntityID->ClearText();
		pEditTargetBone->ClearText();
		pEditTargetPosition->SetVector(decVector());
		pEditTargetOrientation->SetVector(decVector());
	}
	
	UpdateTarget();
}

void ceWPConversation::UpdateTarget(){
	const ceTarget * const target = GetTarget();
	
	if(target){
		pCBTargetActorID->SetText(target->GetActor());
		pEditTargetEntityID->SetText(target->GetCoordSystem());
		pEditTargetBone->SetText(target->GetBone());
		pEditTargetPosition->SetVector(target->GetPosition());
		pEditTargetOrientation->SetVector(target->GetOrientation());
	}
}



void ceWPConversation::UpdateActorIDLists(){
	const decString selection(pCBTargetActorID->GetText());
	
	pCBTargetActorID->RemoveAllItems();
	
	if(pConversation){
		const ceConversationActorList &list = pConversation->GetActorList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			if(list.GetAt(i)->GetID().IsEmpty()){
				continue;
			}
			
			const decString &id = list.GetAt(i)->GetID();
			if(!pCBTargetActorID->HasItem(id)){
				pCBTargetActorID->AddItem(id);
			}
			
			const decString &aliasID = list.GetAt(i)->GetAliasID();
			if(!aliasID.IsEmpty() && !pCBTargetActorID->HasItem(aliasID)){
				pCBTargetActorID->AddItem(aliasID);
			}
		}
		
		pCBTargetActorID->SortItems();
		pCBTargetActorID->StoreFilterItems();
	}
	
	pCBTargetActorID->SetText(selection);
}
