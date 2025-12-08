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

#include "aeWindowMain.h"
#include "aeWindowMainListener.h"
#include "aeView3D.h"
#include "aeTaskSyncGameDefinition.h"
#include "properties/aeWindowProperties.h"
#include "../animator/aeAnimator.h"
#include "../animator/locomotion/aeAnimatorLocomotion.h"
#include "../animator/controller/aeController.h"
#include "../animator/link/aeLink.h"
#include "../animator/rule/aeRuleAnimation.h"
#include "../animator/rule/aeRuleAnimationDifference.h"
#include "../animator/rule/aeRuleAnimationSelect.h"
#include "../animator/rule/aeRuleBoneTransformator.h"
#include "../animator/rule/aeRuleInverseKinematic.h"
#include "../animator/rule/aeRuleStateManipulator.h"
#include "../animator/rule/aeRuleStateSnapshot.h"
#include "../animator/rule/aeRuleForeignState.h"
#include "../animator/rule/aeRuleSubAnimator.h"
#include "../animator/rule/aeRuleTrackTo.h"
#include "../animator/rule/aeRuleLimit.h"
#include "../animator/rule/aeRuleMirror.h"
#include "../animator/rule/aeRuleGroup.h"
#include "../animator/wakeboard/aeWakeboard.h"
#include "../configuration/aeConfiguration.h"
#include "../loadsave/aeLoadSaveSystem.h"
#include "../undosys/controller/aeUAddController.h"
#include "../undosys/controller/aeURemoveController.h"
#include "../undosys/controller/aeUMoveControllerUp.h"
#include "../undosys/controller/aeUMoveControllerDown.h"
#include "../undosys/link/aeULinkAdd.h"
#include "../undosys/link/aeULinkRemove.h"
#include "../undosys/link/aeULinkRemoveUnused.h"
#include "../undosys/rule/aeUAddRule.h"
#include "../undosys/rule/aeURemoveRule.h"
#include "../undosys/rule/aeUMoveRuleUp.h"
#include "../undosys/rule/aeUMoveRuleDown.h"
#include "../undosys/rule/group/aeURuleGroupAddRule.h"
#include "../undosys/rule/group/aeURuleGroupMoveRuleUp.h"
#include "../undosys/rule/group/aeURuleGroupMoveRuleDown.h"
#include "../undosys/rule/group/aeURuleGroupRemoveRule.h"
#include "../aeIGDEModule.h"
#include "../animatoreditor.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>



// Class aeWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

aeWindowMain::aeWindowMain(aeIGDEModule &module) :
igdeEditorWindow(module),
pListener(NULL),
pLoadSaveSystem(NULL),
pView3D(NULL),
pWindowProperties(NULL),
pAnimator(NULL)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener.TakeOverWith(*this);
	pLoadSaveSystem = new aeLoadSaveSystem(this);
	pConfiguration = new aeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::NewWith(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(400)));
	AddChild(splitted);
	
	pWindowProperties.TakeOverWith(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pView3D.TakeOverWith(*this);
	splitted->AddChild(pView3D, igdeContainerSplitted::eaCenter);
	
	CreateNewAnimator();
	ResetViews();
}

aeWindowMain::~aeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetAnimator(NULL);
	
	
	pClipboard.ClearAll();
	
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
	
}



// Management
///////////////

bool aeWindowMain::QuitRequest(){
	return true;
}

void aeWindowMain::ResetViews(){
	pView3D->ResetView();
}



void aeWindowMain::SetAnimator(aeAnimator *animator){
	if(pAnimator == animator){
		return;
	}
	
	pView3D->SetAnimator(NULL);
	pWindowProperties->SetAnimator(NULL);
	
	pActionEditUndo->SetUndoSystem(NULL);
	pActionEditRedo->SetUndoSystem(NULL);
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
		pAnimator->Dispose();
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddReference();
		animator->AddNotifier(pListener);
		
		pActionEditUndo->SetUndoSystem(animator->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(animator->GetUndoSystem());
		
		pWindowProperties->SetAnimator(animator);
		pView3D->SetAnimator(animator);
	}
	
	UpdateAllActions();
}

void aeWindowMain::CreateNewAnimator(){
	const aeAnimator::Ref animator(aeAnimator::Ref::NewWith(*this));
	SetAnimator(animator);
}

void aeWindowMain::SaveAnimator(const char *filename){
	if(!pAnimator){
		return;
	}
	
	const decString basePath(pAnimator->GetDirectoryPath());
	
	pLoadSaveSystem->SaveAnimator(pAnimator, filename);
	pAnimator->SetFilePath(filename);
	pAnimator->SetChanged(false);
	pAnimator->SetSaved(true);
	
	if(pAnimator->GetDirectoryPath() != basePath){
		pWindowProperties->OnAnimatorPathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}



void aeWindowMain::CreateRule(deAnimatorRuleVisitorIdentify::eRuleTypes type, bool insert, bool intoGroup){
	if(!pAnimator || (intoGroup && insert)){
		return;
	}
	
	aeRule * const activeRule = pAnimator->GetActiveRule();
	int index = pAnimator->GetRules().GetCount();
	aeRuleGroup *parentGroup = NULL;
	igdeUndo::Ref undo;
	
	if(activeRule){
		if(intoGroup){
			if(activeRule->GetType() != deAnimatorRuleVisitorIdentify::ertGroup){
				return;
			}
			parentGroup = (aeRuleGroup*)activeRule;
			index = parentGroup->GetRules().GetCount();
			
		}else{
			parentGroup = activeRule->GetParentGroup();
			if(parentGroup){
				index = parentGroup->GetRules().GetCount();
			}
			
			if(insert){
				if(parentGroup){
					index = parentGroup->GetRules().IndexOf(activeRule);
					
				}else{
					index = pAnimator->GetRules().IndexOf(activeRule);
				}
			}
		}
	}
	
	const aeRule::Ref rule(aeRule::Ref::New(aeRule::CreateRuleFromType(type)));
	
	if(parentGroup){
		undo.TakeOverWith(parentGroup, rule, index);
		
	}else{
		undo.TakeOverWith(pAnimator, rule, index);
	}
	
	if(undo){
		pAnimator->GetUndoSystem()->Add(undo);
	}
	
	pAnimator->SetActiveRule(rule);
}



void aeWindowMain::OnBeforeEngineStart(){
}

void aeWindowMain::OnAfterEngineStart(){
	pView3D->OnAfterEngineStart();
}

void aeWindowMain::OnBeforeEngineStop(){
	pView3D->OnBeforeEngineStop();
}

void aeWindowMain::OnAfterEngineStop(){
}

void aeWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	pView3D->SetEnableRendering(true);
}

void aeWindowMain::OnDeactivate(){
	pView3D->SetEnableRendering(false);
	
	igdeEditorWindow::OnDeactivate();
}

void aeWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pView3D->OnFrameUpdate(elapsed);
}

void aeWindowMain::GetChangedDocuments(decStringList &list){
	if(pAnimator && pAnimator->GetChanged()){
		list.Add(pAnimator->GetFilePath());
	}
}

void aeWindowMain::LoadDocument(const char *filename){
	if(pAnimator && pAnimator->GetChanged()){
		if(igdeCommonDialogs::Question(this, igdeCommonDialogs::ebsYesNo, "Open Animator",
		"Open animator discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
			return;
		}
	}
	
	SetAnimator(aeAnimator::Ref::New(pLoadSaveSystem->LoadAnimator(filename)));
	GetRecentFiles().AddFile(filename);
}

bool aeWindowMain::SaveDocument(const char *filename){
	if(pAnimator && pAnimator->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void aeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void aeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewAnimator();
}

igdeStepableTask *aeWindowMain::OnGameDefinitionChanged(){
	return new aeTaskSyncGameDefinition(*this);
}



igdeIcon *aeWindowMain::GetRuleIcon(deAnimatorRuleVisitorIdentify::eRuleTypes type) const{
	switch(type){
	case deAnimatorRuleVisitorIdentify::ertAnimation:
		return pIconRuleAnimation;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationDifference:
		return pIconRuleAnimationDifference;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationSelect:
		return pIconRuleAnimationSelect;
		
	case deAnimatorRuleVisitorIdentify::ertBoneTransformator:
		return pIconRuleBoneTransformator;
		
	case deAnimatorRuleVisitorIdentify::ertInverseKinematic:
		return pIconRuleInverseKinematic;
		
	case deAnimatorRuleVisitorIdentify::ertStateManipulator:
		return pIconRuleStateManipulator;
		
	case deAnimatorRuleVisitorIdentify::ertStateSnapshot:
		return pIconRuleStateSnapshot;
		
	case deAnimatorRuleVisitorIdentify::ertForeignState:
		return pIconRuleForeignState;
		
	case deAnimatorRuleVisitorIdentify::ertGroup:
		return pIconRuleGroup;
		
	case deAnimatorRuleVisitorIdentify::ertSubAnimator:
		return pIconRuleSubAnimator;
		
	//case deAnimatorRuleVisitorIdentify::ertRetarget:
	case deAnimatorRuleVisitorIdentify::ertTrackTo:
		return pIconRuleTrackTo;
		
	case deAnimatorRuleVisitorIdentify::ertLimit:
		return pIconRuleLimit;
		
	case deAnimatorRuleVisitorIdentify::ertMirror:
		return pIconRuleMirror;
		
	default:
		return NULL;
	};
}



void aeWindowMain::SetProgressVisible(bool visible){
	GetEnvironment().SetProgressVisible(visible);
}

void aeWindowMain::SetProgress(float progress){
	GetEnvironment().SetProgress(progress);
}

void aeWindowMain::SetProgressText(const char *text){
	GetEnvironment().SetProgressText(text);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	aeWindowMain &pWindow;
	
public:
	cActionBase(aeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
	
	virtual void OnAction(){
		if(!pWindow.GetAnimator()){
			return;
		}
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(pWindow.GetAnimator())));
		if(undo){
			pWindow.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator) = 0;
	
	virtual void Update(){
		if(pWindow.GetAnimator()){
			Update(*pWindow.GetAnimator());
			
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



class cActionFileNew : public igdeAction{
	aeWindowMain &pWindow;
public:
	cActionFileNew(aeWindowMain &window) :
	igdeAction("New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new animator", deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)),
	pWindow(window){}
	
	virtual void OnAction(){
		if(pWindow.GetAnimator() && pWindow.GetAnimator()->GetChanged()
		&& igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Animator",
		"Creating a new animator discarding the current one is that ok?") == igdeCommonDialogs::ebNo){
			return;
		}
		
		pWindow.CreateNewAnimator();
	}
};

class cActionFileOpen : public igdeAction{
	aeWindowMain &pWindow;
public:
	cActionFileOpen(aeWindowMain &window) : igdeAction("Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Open animator from file",
		deInputEvent::ekcO, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcO)), pWindow(window){}
	
	virtual void OnAction(){
		if(pWindow.GetAnimator() && pWindow.GetAnimator()->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Animator",
			"Open animator discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		decString filename(pWindow.GetAnimator() ? pWindow.GetAnimator()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Animator",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltAnimator ), filename ) ){
			return;
		}
		
		pWindow.SetAnimator(aeAnimator::Ref::New(pWindow.GetLoadSaveSystem().LoadAnimator(filename)));
		pWindow.GetRecentFiles().AddFile(filename);
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	cActionFileSaveAs(aeWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Saves animator under a differen file", deInputEvent::ekcA){}
	
	virtual igdeUndo * OnAction(aeAnimator *animator){
		decString filename(animator->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Animator",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltAnimator ), filename ) ){
			pWindow.SaveAnimator(filename);
		}
		return NULL;
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	cActionFileSave(aeWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs));
		SetDescription("Saves animator to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		if(animator->GetSaved()){
			if(animator->GetChanged()){
				pWindow.SaveAnimator(animator->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(animator);
		}
		return NULL;
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	cActionEditCut(aeWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	virtual igdeUndo *OnAction(aeAnimator*){
		return NULL;
	}
};

class cActionEditCopy : public cActionBase{
public:
	cActionEditCopy(aeWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	virtual igdeUndo *OnAction(aeAnimator*){
		return NULL;
	}
};

class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste(aeWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	virtual igdeUndo *OnAction(aeAnimator*){
		return NULL;
	}
};

class cActionEditLocoEnabled : public cActionBase{
public:
	cActionEditLocoEnabled(aeWindowMain &window) : cActionBase(window,
		"Locomotion Testing", NULL, "Enables/Disabled the locomotion testing",
		deInputEvent::esmControl, deInputEvent::ekcL, deInputEvent::ekcL){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		if(animator->GetWakeboard().GetEnabled()){
			return NULL;
		}
		
		if(animator->GetLocomotion().GetEnabled()){
			pWindow.GetView3D().StopLocomotionTesting();
			
		}else{
			pWindow.GetView3D().StartLocomotionTesting();
		}
		return NULL;
	}
	
	void Update(const aeAnimator &animator) override{
		SetSelected(animator.GetLocomotion().GetEnabled());
	}
};

class cActionEditWBEnabled : public cActionBase{
public:
	   cActionEditWBEnabled(aeWindowMain &window) : cActionBase(window,
		"Wakeboarding", NULL, "Enables/Disabled wakeboarding",
		deInputEvent::esmControl, deInputEvent::ekcW, deInputEvent::ekcW){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		if(animator->GetLocomotion().GetEnabled()){
			return NULL;
		}
		
		if(animator->GetWakeboard().GetEnabled()){
			pWindow.GetView3D().StopWakeboarding();
			
		}else{
			pWindow.GetView3D().StartWakeboarding();
		}
		return NULL;
	}
	
	void Update(const aeAnimator &animator) override{
		SetSelected(animator.GetWakeboard().GetEnabled());
	}
};

class cActionEditShowBones : public cActionBase{
public:
	cActionEditShowBones(aeWindowMain &window) : cActionBase(window,
		"Show Bones", NULL, "Show bones", deInputEvent::ekcB){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		animator->SetShowBones(!animator->GetShowBones());
		return NULL;
	}
	
	void Update(const aeAnimator &animator) override{
		SetSelected(animator.GetShowBones());
	}
};

class cActionEditDDBoneSize : public cActionBase{
public:
	cActionEditDDBoneSize(aeWindowMain &window) : cActionBase(window,
		"Show Bones Base Size...", NULL, "Show bones base size", deInputEvent::ekcS){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		float size = animator->GetDDBoneSize();
		if(igdeCommonDialogs::GetFloat(&pWindow, "Set bone base size", "Size:", size)){
			animator->SetDDBoneSize(size);
		}
		return NULL;
	}
};


class cActionBaseController : public cActionBase{
public:
	cActionBaseController(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, int modifiers = deInputEvent::esmNone,
		deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic){}
	
	cActionBaseController(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		return animator->GetActiveController()
			? OnActionController(animator, animator->GetActiveController()) : NULL;
	}
	
	virtual igdeUndo *OnActionController(aeAnimator *animator, aeController *controller) = 0;
	
	void Update(const aeAnimator &animator) override{
		if(animator.GetActiveController()){
			UpdateController(animator, *animator.GetActiveController());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateController(const aeAnimator &, const aeController &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionControllerAdd : public cActionBase{
public:
	cActionControllerAdd(aeWindowMain &window) : cActionBase(window, "Add...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a controller", deInputEvent::ekcA){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		decString name("Controller");
		if(!igdeCommonDialogs::GetString(&pWindow, "Add Controller", "Name:", name)){
			return NULL;
		}
		if(animator->GetControllers().HasNamed(name)){
			igdeCommonDialogs::Error(&pWindow, "Add Controller", "Name exists already");
			return NULL;
		}
		
		const aeController::Ref controller(aeController::Ref::NewWith(name));
		return new aeUAddController(animator, controller);
	}
};

class cActionControllerDuplicate : public cActionBaseController{
public:
	cActionControllerDuplicate(aeWindowMain &window) : cActionBaseController(window, "Duplicate",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Duplicate controller", deInputEvent::ekcD){}
	
	virtual igdeUndo *OnActionController(aeAnimator *animator, aeController *controller){
		decString name(controller->GetName() + " Copy");
		if(!igdeCommonDialogs::GetString(&pWindow, "Duplicate Controller", "Name:", name)){
			return nullptr;
		}
		
		if(animator->GetControllers().HasNamed(name)){
			igdeCommonDialogs::Error(&pWindow, "Add Controller", "Name exists already");
			return nullptr;
		}
		
		const aeController::Ref duplicate(aeController::Ref::NewWith(*controller));
		duplicate->SetName(name);
		return new aeUAddController(animator, duplicate);
	}
};

class cActionControllerRemove : public cActionBaseController{
public:
	cActionControllerRemove(aeWindowMain &window) : cActionBaseController(window, "Remove",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove controller", deInputEvent::ekcR){}
	
	virtual igdeUndo *OnActionController(aeAnimator *animator, aeController *controller){
		const aeLinkList &links = animator->GetLinks();
		const int count = links.GetCount();
		int i, usageCount = 0;
		decStringList names;
		
		for(i=0; i<count; i++){
			const aeLink &link = *links.GetAt(i);
			if(link.GetController() == controller){
				names.Add(link.GetName());
				usageCount++;
			}
		}
		
		if(usageCount > 0){
			names.SortAscending();
			const decString strNames((names.GetCount() <= 5 ? names : names.Splice(0, 4)).Join(", "));
			
			if(igdeCommonDialogs::QuestionFormat(&pWindow, igdeCommonDialogs::ebsYesNo,
			"Remove Controller", "%d links are using this controller (%s). Remove controller?",
			usageCount, strNames.GetString()) != igdeCommonDialogs::ebYes){
				return NULL;
			}
		}
		
		return new aeURemoveController(animator, controller);
	}
};

class cActionControllerUp : public cActionBaseController{
public:
	cActionControllerUp(aeWindowMain &window) : cActionBaseController(window, "Move Up",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move controller up", deInputEvent::ekcU){}
	
	virtual igdeUndo *OnActionController(aeAnimator *animator, aeController *controller){
		return animator->GetControllers().IndexOf(controller) > 0
			? new aeUMoveControllerUp(animator, controller) : NULL;
	}
	
	void UpdateController(const aeAnimator &animator, const aeController &controller) override{
		SetEnabled(animator.GetControllers().IndexOf((aeController*)&controller) > 0);
	}
};

class cActionControllerDown : public cActionBaseController{
public:
	cActionControllerDown(aeWindowMain &window) : cActionBaseController(window, "Move Down",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move controller down", deInputEvent::ekcD){}
	
	virtual igdeUndo *OnActionController(aeAnimator *animator, aeController *controller){
		return animator->GetControllers().IndexOf(controller) < animator->GetControllers().GetCount() - 1
			? new aeUMoveControllerDown(animator, controller) : NULL;
	}
	
	void UpdateController(const aeAnimator &animator, const aeController &controller) override{
		SetEnabled(animator.GetControllers().IndexOf((aeController*)&controller)
			< animator.GetControllers().GetCount() - 1);
	}
};



class cActionBaseLink : public cActionBase{
public:
	cActionBaseLink(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, int modifiers = deInputEvent::esmNone,
		deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic){}
	
	cActionBaseLink(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		return animator->GetActiveLink() ? OnActionLink(animator, animator->GetActiveLink()) : NULL;
	}
	
	virtual igdeUndo *OnActionLink(aeAnimator *animator, aeLink *link) = 0;
	
	void Update(const aeAnimator &animator) override{
		if(animator.GetActiveLink()){
			UpdateLink(animator, *animator.GetActiveLink());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateLink(const aeAnimator &, const aeLink &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionLinkAdd : public cActionBase{
public:
	cActionLinkAdd(aeWindowMain &window) : cActionBase(window, "Add...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a link", deInputEvent::ekcA){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		decString name("Link");
		if(!igdeCommonDialogs::GetString(&pWindow, "Add Link", "Name:", name)){
			return NULL;
		}
		
		const aeLink::Ref link(aeLink::Ref::NewWith(name));
		return new aeULinkAdd(animator, link);
	}
};

class cActionLinkDuplicate : public cActionBaseLink{
public:
	cActionLinkDuplicate(aeWindowMain &window) : cActionBaseLink(window, "Duplicate",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Duplicate link", deInputEvent::ekcD){}
	
	virtual igdeUndo *OnActionLink(aeAnimator *animator, aeLink *link){
		decString name(link->GetName() + " Copy");
		if(!igdeCommonDialogs::GetString(&pWindow, "Duplicate Link", "Name:", name)){
			return NULL;
		}
		
		const aeLink::Ref newLink(aeLink::Ref::NewWith(*link));
		((aeLink&)(deObject&)newLink).SetName(name);
		return new aeULinkAdd(animator, newLink);
	}
};

class cActionLinkRemove : public cActionBaseLink{
public:
	cActionLinkRemove(aeWindowMain &window) : cActionBaseLink(window, "Remove",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove link", deInputEvent::ekcR){}
	
	virtual igdeUndo *OnActionLink(aeAnimator *animator, aeLink *link){
		const int usageCount = animator->CountLinkUsage(link);
		if(usageCount > 0){
			decString text;
			text.Format("The link '%s' is used by %d targets.\n"
				"If the link is removed now it is also removed from\n"
				"all the targets using it. Do you want to remove the link?",
				link->GetName().GetString(), usageCount);
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo,
			"Remove Link", text) == igdeCommonDialogs::ebNo){
				return NULL;
			}
		}
		
		return new aeULinkRemove(link);
	}
};

class cActionLinkRemoveUnused : public cActionBase{
public:
	cActionLinkRemoveUnused(aeWindowMain &window) : cActionBase(window, "Remove Unused Links",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove links not used in any rule target", deInputEvent::ekcU){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		return new aeULinkRemoveUnused(animator);
	}
};



class cActionBaseRule : public cActionBase{
public:
	cActionBaseRule(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, int modifiers = deInputEvent::esmNone,
		deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic){}
	
	cActionBaseRule(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator){
		return animator->GetActiveRule() ? OnActionRule(animator, animator->GetActiveRule()) : NULL;
	}
	
	virtual igdeUndo *OnActionRule(aeAnimator *animator, aeRule *rule) = 0;
	
	void Update(const aeAnimator &animator) override{
		if(animator.GetActiveRule()){
			UpdateRule(animator, *animator.GetActiveRule());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateRule(const aeAnimator &, const aeRule &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionRuleAdd : public cActionBase{
	const deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	const bool pInsert;
public:
	cActionRuleAdd(aeWindowMain &window, deAnimatorRuleVisitorIdentify::eRuleTypes type,
		bool insert, const char *text, igdeIcon *icon, const char *description) :
	cActionBase(window, text, icon, description), pType(type), pInsert(insert){}
	
	virtual igdeUndo *OnAction(aeAnimator*){
		pWindow.CreateRule(pType, pInsert, false);
		return NULL;
	}
};

class cActionRuleAddIntoGroup : public cActionBaseRule{
	const deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	const bool pInsert;
public:
	cActionRuleAddIntoGroup(aeWindowMain &window, deAnimatorRuleVisitorIdentify::eRuleTypes type,
		bool insert, const char *text, igdeIcon *icon, const char *description) :
	cActionBaseRule(window, text, icon, description), pType(type), pInsert(insert){}
	
	virtual igdeUndo *OnActionRule(aeAnimator*, aeRule *rule){
		if(rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup){
			pWindow.CreateRule(pType, pInsert, true);
		}
		return NULL;
	}
	
	void UpdateRule(const aeAnimator & , const aeRule &rule) override{
		SetEnabled(rule.GetType() == deAnimatorRuleVisitorIdentify::ertGroup);
	}
};

class cActionRuleRemove : public cActionBaseRule{
public:
	cActionRuleRemove(aeWindowMain &window) : cActionBaseRule(window, "Remove",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove rule", deInputEvent::ekcR){}
	
	virtual igdeUndo *OnActionRule(aeAnimator *animator, aeRule *rule){
		aeRuleGroup * const parentGroup = rule->GetParentGroup();
		
		if(parentGroup){
			return new aeURuleGroupRemoveRule(parentGroup, rule);
			
		}else{
			return new aeURemoveRule(animator, rule);
		}
	}
};

class cActionRuleUp : public cActionBaseRule{
public:
	cActionRuleUp(aeWindowMain &window) : cActionBaseRule(window, "Move Up",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move rule up", deInputEvent::ekcU){}
	
	virtual igdeUndo *OnActionRule(aeAnimator *animator, aeRule *rule){
		aeRuleGroup * const parentGroup = rule->GetParentGroup();
		
		if(parentGroup){
			const int index = parentGroup->GetRules().IndexOf(rule);
			return index > 0 ? new aeURuleGroupMoveRuleUp(parentGroup, rule) : NULL;
			
		}else{
			const int index = animator->GetRules().IndexOf(rule);
			return index > 0 ? new aeUMoveRuleUp(animator, rule) : NULL;
		}
	}
	
	void UpdateRule(const aeAnimator &animator, const aeRule &rule) override{
		const aeRuleGroup * const parentGroup = rule.GetParentGroup();
		
		if(parentGroup){
			SetEnabled(parentGroup->GetRules().IndexOf((aeRule*)&rule) > 0);
			
		}else{
			SetEnabled(animator.GetRules().IndexOf((aeRule*)&rule) > 0);
		}
	}
};

class cActionRuleDown : public cActionBaseRule{
public:
	cActionRuleDown(aeWindowMain &window) : cActionBaseRule(window, "Move Down",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move rule down", deInputEvent::ekcD){}
	
	virtual igdeUndo *OnActionRule(aeAnimator *animator, aeRule *rule){
		aeRuleGroup * const pg = rule->GetParentGroup();
		
		if(pg){
			const int index = pg->GetRules().IndexOf(rule);
			return index < pg->GetRules().GetCount() - 1 ? new aeURuleGroupMoveRuleDown(pg, rule) : NULL;
			
		}else{
			const int index = animator->GetRules().IndexOf(rule);
			return index < animator->GetRules().GetCount() - 1 ? new aeUMoveRuleDown(animator, rule) : NULL;
		}
	}
	
	void UpdateRule(const aeAnimator &animator, const aeRule &rule) override{
		const aeRuleGroup * const pg = rule.GetParentGroup();
		
		if(pg){
			SetEnabled(pg->GetRules().IndexOf((aeRule*)&rule) < pg->GetRules().GetCount() - 1);
			
		}else{
			SetEnabled(animator.GetRules().IndexOf((aeRule*)&rule) < animator.GetRules().GetCount() - 1);
		}
	}
};

}



// Private Functions
//////////////////////

void aeWindowMain::pLoadIcons(){
	pIconRuleAnimation.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_animation.png"));
	pIconRuleAnimationDifference.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_animation_difference.png"));
	pIconRuleAnimationSelect.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_animation.png"));
	pIconRuleBoneTransformator.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_bone_transformator.png"));
	pIconRuleForeignState.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_foreign_state.png"));
	pIconRuleGroup.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_group.png"));
	pIconRuleInverseKinematic.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_inverse_kinematic.png"));
	pIconRuleLimit.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_limit.png"));
	pIconRuleStateManipulator.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_state_manipulator.png"));
	pIconRuleStateSnapshot.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_state_snapshot.png"));
	pIconRuleSubAnimator.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_sub_animator.png"));
	pIconRuleTrackTo.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_track_to.png"));
	pIconRuleMirror.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_mirror.png"));
}

void aeWindowMain::pCreateActions(){
	igdeEnvironment &environment = GetEnvironment();
	
	pActionFileNew.TakeOverWith(*this);
	pActionFileOpen.TakeOverWith(*this);
	pActionFileSave.TakeOverWith(*this);
	pActionFileSaveAs.TakeOverWith(*this);
	
	pActionEditUndo.TakeOverWith(environment);
	pActionEditRedo.TakeOverWith(environment);
	
	pActionEditCut.TakeOverWith(*this);
	pActionEditCopy.TakeOverWith(*this);
	pActionEditPaste.TakeOverWith(*this);
	
	pActionEditLocoEnabled.TakeOverWith(*this);
	pActionEditWBTracking.TakeOverWith(*this);
	pActionEditShowBones.TakeOverWith(*this);
	pActionEditDDBoneSize.TakeOverWith(*this);
	
	pActionControllerAdd.TakeOverWith(*this);
	pActionControllerDuplicate.TakeOverWith(*this);
	pActionControllerRemove.TakeOverWith(*this);
	pActionControllerUp.TakeOverWith(*this);
	pActionControllerDown.TakeOverWith(*this);
	
	pActionLinkAdd.TakeOverWith(*this);
	pActionLinkDuplicate.TakeOverWith(*this);
	pActionLinkRemove.TakeOverWith(*this);
	pActionLinkRemoveUnused.TakeOverWith(*this);
	
	pActionRuleAddAnim.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertAnimation,
		false, "Animation", pIconRuleAnimation, "Add an animation rule"));
	pActionRuleAddAnimDiff.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertAnimationDifference,
		false, "Animation Difference", pIconRuleAnimationDifference, "Add an animation difference rule"));
	pActionRuleAddAnimSelect.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertAnimationSelect,
		false, "Animation Select", pIconRuleAnimationSelect, "Add an animation select rule"));
	pActionRuleAddBoneRot.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertBoneTransformator,
		false, "Bone Transformator", pIconRuleBoneTransformator, "Add a bone transformator rule"));
	pActionRuleAddInvKin.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertInverseKinematic,
		false, "Inverse Kinematic", pIconRuleInverseKinematic, "Add an inverse kinematic rule"));
	pActionRuleAddStateManip.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertStateManipulator,
		false, "State Manipulator", pIconRuleStateManipulator, "Add a state manipulator rule"));
	pActionRuleAddStateSnap.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertStateSnapshot,
		false, "State Snapshot", pIconRuleStateSnapshot, "Add a state snapshot rule"));
	pActionRuleAddForeignState.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertForeignState,
		false, "Foreign State", pIconRuleForeignState, "Add a foreign state rule"));
	pActionRuleAddGroup.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertGroup,
		false, "Group", pIconRuleGroup, "Add a group rule"));
	pActionRuleAddSubAnimator.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertSubAnimator,
		false, "Sub Animator", pIconRuleSubAnimator, "Add a sub animator rule"));
	pActionRuleAddTrackTo.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertTrackTo,
		false, "Track To", pIconRuleTrackTo, "Add a track to rule"));
	pActionRuleAddLimit.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertLimit,
		false, "Limit", pIconRuleLimit, "Add a limit rule"));
	pActionRuleAddMirror.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertMirror,
		false, "Mirror", pIconRuleMirror, "Add a mirror rule"));
	
	pActionRuleAddIntoGroupAnim.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertAnimation,
		false, "Animation", pIconRuleAnimation, "Add an animation rule"));
	pActionRuleAddIntoGroupAnimDiff.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertAnimationDifference,
		false, "Animation Difference", pIconRuleAnimationDifference, "Add an animation difference rule"));
	pActionRuleAddIntoGroupAnimSelect.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertAnimationSelect,
		false, "Animation Select", pIconRuleAnimationSelect, "Add an animation select rule"));
	pActionRuleAddIntoGroupBoneRot.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertBoneTransformator,
		false, "Bone Transformator", pIconRuleBoneTransformator, "Add a bone transformator rule"));
	pActionRuleAddIntoGroupInvKin.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertInverseKinematic,
		false, "Inverse Kinematic", pIconRuleInverseKinematic, "Add an inverse kinematic rule"));
	pActionRuleAddIntoGroupStateManip.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertStateManipulator,
		false, "State Manipulator", pIconRuleStateManipulator, "Add a state manipulator rule"));
	pActionRuleAddIntoGroupStateSnap.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertStateSnapshot,
		false, "State Snapshot", pIconRuleStateSnapshot, "Add a state snapshot rule"));
	pActionRuleAddIntoGroupForeignState.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertForeignState,
		false, "Foreign State", pIconRuleForeignState, "Add a foreign state rule"));
	pActionRuleAddIntoGroupGroup.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertGroup,
		false, "Group", pIconRuleGroup, "Add a group rule"));
	pActionRuleAddIntoGroupSubAnimator.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertSubAnimator,
		false, "Sub Animator", pIconRuleSubAnimator, "Add a sub animator rule"));
	pActionRuleAddIntoGroupTrackTo.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertTrackTo,
		false, "Track To", pIconRuleTrackTo, "Add a track to rule"));
	pActionRuleAddIntoGroupLimit.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertLimit,
		false, "Limit", pIconRuleLimit, "Add a limit rule"));
	pActionRuleAddIntoGroupMirror.TakeOver(new cActionRuleAddIntoGroup(*this, deAnimatorRuleVisitorIdentify::ertMirror,
		false, "Mirror", pIconRuleMirror, "Add a mirror rule"));
	
	pActionRuleInsertAnim.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertAnimation,
		true, "Animation", pIconRuleAnimation, "Insert an animation rule"));
	pActionRuleInsertAnimDiff.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertAnimationDifference,
		true, "Animation Difference", pIconRuleAnimationDifference, "Insert an animation difference rule"));
	pActionRuleInsertAnimSelect.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertAnimationSelect,
		true, "Animation Select", pIconRuleAnimationSelect, "Insert an animation select rule"));
	pActionRuleInsertBoneRot.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertBoneTransformator,
		true, "Bone Transformator", pIconRuleBoneTransformator, "Insert a bone transformator rule"));
	pActionRuleInsertInvKin.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertInverseKinematic,
		true, "Inverse Kinematic", pIconRuleInverseKinematic, "Insert an inverse kinematic rule"));
	pActionRuleInsertStateManip.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertStateManipulator,
		true, "State Manipulator", pIconRuleStateManipulator, "Insert a state manipulator rule"));
	pActionRuleInsertStateSnap.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertStateSnapshot,
		true, "State Snapshot", pIconRuleStateSnapshot, "Insert a state snapshot rule"));
	pActionRuleInsertForeignState.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertForeignState,
		true, "Foreign State", pIconRuleForeignState, "Insert a foreign state rule"));
	pActionRuleInsertGroup.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertGroup,
		true, "Group", pIconRuleGroup, "Insert a group rule"));
	pActionRuleInsertSubAnimator.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertSubAnimator,
		true, "Sub Animator", pIconRuleSubAnimator, "Insert a sub animator rule"));
	pActionRuleInsertTrackTo.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertTrackTo,
		true, "Track To", pIconRuleTrackTo, "Insert a track to rule"));
	pActionRuleInsertLimit.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertLimit,
		true, "Limit", pIconRuleLimit, "Insert a limit rule"));
	pActionRuleInsertMirror.TakeOver(new cActionRuleAdd(*this, deAnimatorRuleVisitorIdentify::ertMirror,
		true, "Mirror", pIconRuleMirror, "Insert a mirror rule"));
	
	pActionRuleRemove.TakeOverWith(*this);
	pActionRuleUp.TakeOverWith(*this);
	pActionRuleDown.TakeOverWith(*this);
	
	
	// register for updating
	AddUpdateAction(pActionFileNew);
	AddUpdateAction(pActionFileOpen);
	AddUpdateAction(pActionFileSave);
	AddUpdateAction(pActionFileSaveAs);
	
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
	AddUpdateAction(pActionEditLocoEnabled);
	AddUpdateAction(pActionEditWBTracking);
	AddUpdateAction(pActionEditShowBones);
	AddUpdateAction(pActionEditDDBoneSize);
	
	AddUpdateAction(pActionControllerAdd);
	AddUpdateAction(pActionControllerDuplicate);
	AddUpdateAction(pActionControllerRemove);
	AddUpdateAction(pActionControllerUp);
	AddUpdateAction(pActionControllerDown);
	
	AddUpdateAction(pActionLinkAdd);
	AddUpdateAction(pActionLinkDuplicate);
	AddUpdateAction(pActionLinkRemove);
	AddUpdateAction(pActionLinkRemoveUnused);
	
	AddUpdateAction(pActionRuleAddAnim);
	AddUpdateAction(pActionRuleAddAnimDiff);
	AddUpdateAction(pActionRuleAddAnimSelect);
	AddUpdateAction(pActionRuleAddBoneRot);
	AddUpdateAction(pActionRuleAddInvKin);
	AddUpdateAction(pActionRuleAddStateManip);
	AddUpdateAction(pActionRuleAddStateSnap);
	AddUpdateAction(pActionRuleAddForeignState);
	AddUpdateAction(pActionRuleAddGroup);
	AddUpdateAction(pActionRuleAddSubAnimator);
	AddUpdateAction(pActionRuleAddTrackTo);
	AddUpdateAction(pActionRuleAddLimit);
	AddUpdateAction(pActionRuleAddMirror);
	
	AddUpdateAction(pActionRuleInsertAnim);
	AddUpdateAction(pActionRuleInsertAnimDiff);
	AddUpdateAction(pActionRuleInsertAnimSelect);
	AddUpdateAction(pActionRuleInsertBoneRot);
	AddUpdateAction(pActionRuleInsertInvKin);
	AddUpdateAction(pActionRuleInsertStateManip);
	AddUpdateAction(pActionRuleInsertStateSnap);
	AddUpdateAction(pActionRuleInsertForeignState);
	AddUpdateAction(pActionRuleInsertGroup);
	AddUpdateAction(pActionRuleInsertSubAnimator);
	AddUpdateAction(pActionRuleInsertTrackTo);
	AddUpdateAction(pActionRuleInsertLimit);
	AddUpdateAction(pActionRuleInsertMirror);
	
	AddUpdateAction(pActionRuleRemove);
	AddUpdateAction(pActionRuleUp);
	AddUpdateAction(pActionRuleDown);
}

void aeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOverWith(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void aeWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit.TakeOverWith(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionRuleAddAnim);
	helper.ToolBarButton(pTBEdit, pActionRuleAddAnimDiff);
	helper.ToolBarButton(pTBEdit, pActionRuleAddAnimSelect);
	helper.ToolBarButton(pTBEdit, pActionRuleAddBoneRot);
	helper.ToolBarButton(pTBEdit, pActionRuleAddInvKin);
	helper.ToolBarButton(pTBEdit, pActionRuleAddStateManip);
	helper.ToolBarButton(pTBEdit, pActionRuleAddStateSnap);
	helper.ToolBarButton(pTBEdit, pActionRuleAddForeignState);
	helper.ToolBarButton(pTBEdit, pActionRuleAddGroup);
	helper.ToolBarButton(pTBEdit, pActionRuleAddSubAnimator);
	helper.ToolBarButton(pTBEdit, pActionRuleAddTrackTo);
	helper.ToolBarButton(pTBEdit, pActionRuleAddLimit);
	helper.ToolBarButton(pTBEdit, pActionRuleAddMirror);
	
	AddSharedToolBar(pTBEdit);
}

void aeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade.TakeOverWith(env, "File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Controller", deInputEvent::ekcC);
	pCreateMenuController(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Link", deInputEvent::ekcL);
	pCreateMenuLink(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Rule", deInputEvent::ekcR);
	pCreateMenuRule(cascade);
	AddSharedMenu(cascade);
}

void aeWindowMain::pCreateMenuFile(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
}

void aeWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionEditLocoEnabled);
	helper.MenuCheck(menu, pActionEditWBTracking);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionEditShowBones);
	helper.MenuCommand(menu, pActionEditDDBoneSize);
}

void aeWindowMain::pCreateMenuController(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionControllerAdd);
	helper.MenuCommand(menu, pActionControllerDuplicate);
	helper.MenuCommand(menu, pActionControllerRemove);
	helper.MenuCommand(menu, pActionControllerUp);
	helper.MenuCommand(menu, pActionControllerDown);
}

void aeWindowMain::pCreateMenuLink(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionLinkAdd);
	helper.MenuCommand(menu, pActionLinkDuplicate);
	helper.MenuCommand(menu, pActionLinkRemove);
	helper.MenuCommand(menu, pActionLinkRemoveUnused);
}

void aeWindowMain::pCreateMenuRule(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		GetEnvironment(), "Add", deInputEvent::ekcA));
	menu.AddChild(subMenu);
	helper.MenuCommand(subMenu, pActionRuleAddAnim);
	helper.MenuCommand(subMenu, pActionRuleAddAnimDiff);
	helper.MenuCommand(subMenu, pActionRuleAddAnimSelect);
	helper.MenuCommand(subMenu, pActionRuleAddBoneRot);
	helper.MenuCommand(subMenu, pActionRuleAddInvKin);
	helper.MenuCommand(subMenu, pActionRuleAddStateManip);
	helper.MenuCommand(subMenu, pActionRuleAddStateSnap);
	helper.MenuCommand(subMenu, pActionRuleAddForeignState);
	helper.MenuCommand(subMenu, pActionRuleAddGroup);
	helper.MenuCommand(subMenu, pActionRuleAddSubAnimator);
	helper.MenuCommand(subMenu, pActionRuleAddTrackTo);
	helper.MenuCommand(subMenu, pActionRuleAddLimit);
	helper.MenuCommand(subMenu, pActionRuleAddMirror);
	
	subMenu.TakeOverWith(GetEnvironment(), "Insert", deInputEvent::ekcI);
	menu.AddChild(subMenu);
	helper.MenuCommand(subMenu, pActionRuleInsertAnim);
	helper.MenuCommand(subMenu, pActionRuleInsertAnimDiff);
	helper.MenuCommand(subMenu, pActionRuleInsertAnimSelect);
	helper.MenuCommand(subMenu, pActionRuleInsertBoneRot);
	helper.MenuCommand(subMenu, pActionRuleInsertInvKin);
	helper.MenuCommand(subMenu, pActionRuleInsertStateManip);
	helper.MenuCommand(subMenu, pActionRuleInsertStateSnap);
	helper.MenuCommand(subMenu, pActionRuleInsertForeignState);
	helper.MenuCommand(subMenu, pActionRuleInsertGroup);
	helper.MenuCommand(subMenu, pActionRuleInsertSubAnimator);
	helper.MenuCommand(subMenu, pActionRuleInsertTrackTo);
	helper.MenuCommand(subMenu, pActionRuleInsertLimit);
	helper.MenuCommand(subMenu, pActionRuleInsertMirror);
	
	helper.MenuCommand(subMenu, pActionRuleRemove);
	helper.MenuCommand(subMenu, pActionRuleUp);
	helper.MenuCommand(subMenu, pActionRuleDown);
}
