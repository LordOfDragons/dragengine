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
#include <dragengine/common/collection/decGlobalFunctions.h>
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
pLoadSaveSystem(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pMCAnimatorProperties.Init(*this);
	
	pListener = aeWindowMainListener::Ref::New(*this);
	pLoadSaveSystem = new aeLoadSaveSystem(this);
	pConfiguration = new aeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(400)));
	AddChild(splitted);
	
	pWindowProperties = igdeWPMetaContextList::Ref::New(env);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pView3D = aeView3D::Ref::New(*this);
	splitted->AddChild(pView3D, igdeContainerSplitted::eaCenter);
	
	CreateNewAnimator();
	ResetViews();
}

aeWindowMain::~aeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetAnimator(nullptr);
	
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
	if(animator == pAnimator){
		return;
	}
	
	pView3D->SetAnimator(nullptr);
	
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
		pAnimator->Dispose();
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
		
		pActionEditUndo->SetUndoSystem(animator->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(animator->GetUndoSystem());
		
		pView3D->SetAnimator(animator);
	}
	
	UpdateAllActions();
	pUpdateMetaContexts();
}

void aeWindowMain::CreateNewAnimator(){
	SetAnimator(aeAnimator::Ref::New(*this));
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
		pAnimator->NotifyBasePathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}



void aeWindowMain::CreateRule(deAnimatorRuleVisitorIdentify::eRuleTypes type, bool insert, bool intoGroup, const igdeAction &action){
	if(!pAnimator || (intoGroup && insert)){
		return;
	}
	
	auto activeRule = pAnimator->mpRuleTree.GetActive();
	int index = pAnimator->mpRules->GetCount();
	aeRuleGroup *parentGroup = nullptr;
	igdeUndo::Ref undo;
	
	if(activeRule){
		if(intoGroup){
			auto activeRuleGroup = activeRule.DynamicCast<aeRuleGroup>();
			if(!activeRuleGroup){
				return;
			}
			parentGroup = activeRuleGroup;
			index = parentGroup->mpRules->GetCount();
			
		}else{
			parentGroup = activeRule->GetParentGroup();
			if(parentGroup){
				index = parentGroup->mpRules->GetCount();
			}
			
			if(insert){
				if(parentGroup){
					index = parentGroup->mpRules->IndexOf(activeRule);
					
				}else{
					index = pAnimator->mpRules->IndexOf(activeRule);
				}
			}
		}
	}
	
	auto rule = aeRule::CreateRuleFromType(*this, type, GetEnvironment().GetTranslationManager());
	auto &storage = parentGroup ? parentGroup->mpRules : pAnimator->mpRules;
	
	auto list = storage.GetValue();
	list.Insert(rule, index);
	storage.Property().ChangePropertyValueType(storage.Context(),
		list, storage.Property().RealUndoInfo(storage.Context(), action));
	
	pAnimator->mpRuleTree.SetActive(rule);
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
		if(igdeCommonDialogs::Question(*this, igdeCommonDialogs::ebsYesNo, "@Animator.WindowMain.OpenAnimator.Title",
		"@Animator.WindowMain.OpenAnimator.Message") == igdeCommonDialogs::ebNo){
			return;
		}
	}
	
	SetAnimator(pLoadSaveSystem->LoadAnimator(filename));
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

igdeStepableTask::Ref aeWindowMain::OnGameDefinitionChanged(){
	return aeTaskSyncGameDefinition::Ref::New(*this);
}



const igdeIcon::Ref &aeWindowMain::GetRuleIcon(deAnimatorRuleVisitorIdentify::eRuleTypes type) const{
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
		DETHROW(deeInvalidParam);
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
public:
	typedef deTObjectReference<cActionBase> Ref;
	
private:
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
	
	void OnAction() override{
		if(!pWindow.GetAnimator()){
			return;
		}
		igdeUndo::Ref undo(OnAction(pWindow.GetAnimator()));
		if(undo){
			pWindow.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator) = 0;
	
	void Update() override{
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
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	
private:
	aeWindowMain &pWindow;
public:
	cActionFileNew(aeWindowMain &window) :
	igdeAction("@Animator.Action.File.New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"@Animator.Action.File.New.ToolTip", deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)),
	pWindow(window){}
	
	void OnAction() override{
		if(pWindow.GetAnimator() && pWindow.GetAnimator()->GetChanged()
		&& igdeCommonDialogs::Question(pWindow, igdeCommonDialogs::ebsYesNo, "@Animator.Dialog.NewAnimator.Title",
		"@Animator.Dialog.NewAnimator.Message") == igdeCommonDialogs::ebNo){
			return;
		}
		
		pWindow.CreateNewAnimator();
	}
};

class cActionFileOpen : public igdeAction{
public:
	typedef deTObjectReference<cActionFileOpen> Ref;
	
private:
	aeWindowMain &pWindow;
public:
	cActionFileOpen(aeWindowMain &window) : igdeAction("@Animator.Action.File.Open",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "@Animator.Action.File.Open.ToolTip",
		deInputEvent::ekcO, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcO)), pWindow(window){}
	
	void OnAction() override{
		if(pWindow.GetAnimator() && pWindow.GetAnimator()->GetChanged()){
			if(igdeCommonDialogs::Question(pWindow, igdeCommonDialogs::ebsYesNo, "@Animator.Dialog.OpenAnimator.Title",
			"@Animator.Dialog.OpenAnimator.MessageDiscardChanges") == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		decString filename(pWindow.GetAnimator() ? pWindow.GetAnimator()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData());
		if(!igdeCommonDialogs::GetFileOpen(pWindow, "@Animator.WindowMain.OpenAnimator.Title",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltAnimator ), filename ) ){
			return;
		}
		
		pWindow.SetAnimator(pWindow.GetLoadSaveSystem().LoadAnimator(filename));
		pWindow.GetRecentFiles().AddFile(filename);
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSaveAs> Ref;
	cActionFileSaveAs(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.File.SaveAs", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"@Animator.Action.File.SaveAs.ToolTip", deInputEvent::ekcA){}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator){
		decString filename(animator->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(pWindow, "@Animator.Dialog.SaveAnimator.Title",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltAnimator ), filename ) ){
			pWindow.SaveAnimator(filename);
		}
		return {};
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	typedef deTObjectReference<cActionFileSave> Ref;
	cActionFileSave(aeWindowMain &window) : cActionFileSaveAs(window){
		SetText("@Animator.Action.File.Save");
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs));
		SetDescription("@Animator.Action.File.Save.ToolTip");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
	}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		if(animator->GetSaved()){
			if(animator->GetChanged()){
				pWindow.SaveAnimator(animator->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(animator);
		}
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"@Animator.Action.Edit.Cut.ToolTip", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	igdeUndo::Ref OnAction(aeAnimator*) override{
		return {};
	}
};

class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.Action.Edit.Copy.ToolTip", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(aeAnimator*) override{
		return {};
	}
};

class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Animator.Action.Edit.Paste.ToolTip", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	igdeUndo::Ref OnAction(aeAnimator*) override{
		return {};
	}
};

class cActionEditLocoEnabled : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLocoEnabled> Ref;
	cActionEditLocoEnabled(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.LocomotionTesting", nullptr, "@Animator.Action.Edit.LocomotionTesting.ToolTip",
		deInputEvent::esmControl, deInputEvent::ekcL, deInputEvent::ekcL){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		if(animator->GetWakeboard().GetEnabled()){
			return {};
		}
		
		if(animator->GetLocomotion().GetEnabled()){
			pWindow.GetView3D().StopLocomotionTesting();
			
		}else{
			pWindow.GetView3D().StartLocomotionTesting();
		}
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetSelected(animator.GetLocomotion().GetEnabled());
	}
};

class cActionEditWBEnabled : public cActionBase{
public:
	typedef deTObjectReference<cActionEditWBEnabled> Ref;
	cActionEditWBEnabled(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.Wakeboarding", nullptr, "@Animator.Action.Edit.Wakeboarding.ToolTip",
		deInputEvent::esmControl, deInputEvent::ekcW, deInputEvent::ekcW){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		if(animator->GetLocomotion().GetEnabled()){
			return {};
		}
		
		if(animator->GetWakeboard().GetEnabled()){
			pWindow.GetView3D().StopWakeboarding();
			
		}else{
			pWindow.GetView3D().StartWakeboarding();
		}
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetSelected(animator.GetWakeboard().GetEnabled());
	}
};

class cActionEditShowBones : public cActionBase{
public:
	typedef deTObjectReference<cActionEditShowBones> Ref;
	cActionEditShowBones(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.ShowBones", nullptr, "@Animator.Action.Edit.ShowBones.ToolTip", deInputEvent::ekcB){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		animator->SetShowBones(!animator->GetShowBones());
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetSelected(animator.GetShowBones());
	}
};

class cActionEditDDBoneSize : public cActionBase{
public:
	typedef deTObjectReference<cActionEditDDBoneSize> Ref;
	cActionEditDDBoneSize(aeWindowMain &window) : cActionBase(window,
		"@Animator.Action.Edit.ShowBonesBaseSize", nullptr, "@Animator.Action.Edit.ShowBonesBaseSize.ToolTip", deInputEvent::ekcS){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		float size = animator->GetDDBoneSize();
		if(igdeCommonDialogs::GetFloat(pWindow, "@Animator.Dialog.SetBoneBaseSize.Title", "@Animator.Dialog.SetBoneBaseSize.Size", size)){
			animator->SetDDBoneSize(size);
		}
		return {};
	}
};


class cActionBaseRule : public cActionBase{
public:
	typedef deTObjectReference<cActionBaseRule> Ref;
	cActionBaseRule(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, int modifiers = deInputEvent::esmNone,
		deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic){}
	
	cActionBaseRule(aeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		return animator->GetActiveRule() ? OnActionRule(animator, animator->GetActiveRule()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionRule(aeAnimator *animator, aeRule *rule) = 0;
	
	void Update(const aeAnimator &animator) override{
		auto &rule = animator.GetActiveRule();
		if(rule){
			UpdateRule(animator, rule);
			
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
	typedef deTObjectReference<cActionRuleAdd> Ref;
	cActionRuleAdd(aeWindowMain &window, deAnimatorRuleVisitorIdentify::eRuleTypes type,
		bool insert, const char *text, igdeIcon *icon, const char *description) :
	cActionBase(window, text, icon, description), pType(type), pInsert(insert){}
	
	igdeUndo::Ref OnAction(aeAnimator*) override{
		pWindow.CreateRule(pType, pInsert, false, *this);
		return {};
	}
};

class cActionRuleAddIntoGroup : public cActionBaseRule{
	const deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	const bool pInsert;
public:
	typedef deTObjectReference<cActionRuleAddIntoGroup> Ref;
	cActionRuleAddIntoGroup(aeWindowMain &window, deAnimatorRuleVisitorIdentify::eRuleTypes type,
		bool insert, const char *text, igdeIcon *icon, const char *description) :
	cActionBaseRule(window, text, icon, description), pType(type), pInsert(insert){}
	
	virtual igdeUndo::Ref OnActionRule(aeAnimator*, aeRule *rule){
		if(rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup){
			pWindow.CreateRule(pType, pInsert, true, *this);
		}
		return {};
	}
	
	void UpdateRule(const aeAnimator & , const aeRule &rule) override{
		SetEnabled(rule.GetType() == deAnimatorRuleVisitorIdentify::ertGroup);
	}
};

}



// Private Functions
//////////////////////

void aeWindowMain::pLoadIcons(){
	pIconRuleAnimation = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_animation.png");
	pIconRuleAnimationDifference = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_animation_difference.png");
	pIconRuleAnimationSelect = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_animation.png");
	pIconRuleBoneTransformator = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_bone_transformator.png");
	pIconRuleForeignState = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_foreign_state.png");
	pIconRuleGroup = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_group.png");
	pIconRuleInverseKinematic = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_inverse_kinematic.png");
	pIconRuleLimit = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_limit.png");
	pIconRuleStateManipulator = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_state_manipulator.png");
	pIconRuleStateSnapshot = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_state_snapshot.png");
	pIconRuleSubAnimator = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_sub_animator.png");
	pIconRuleTrackTo = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_track_to.png");
	pIconRuleMirror = igdeIcon::LoadPNG(GetEditorModule(), "icons/rule_mirror.png");
}

void aeWindowMain::pCreateActions(){
	igdeEnvironment &environment = GetEnvironment();
	
	pActionFileNew = cActionFileNew::Ref::New(*this);
	pActionFileOpen = cActionFileOpen::Ref::New(*this);
	pActionFileSave = cActionFileSave::Ref::New(*this);
	pActionFileSaveAs = cActionFileSaveAs::Ref::New(*this);
	
	pActionEditUndo = igdeActionUndo::Ref::New(environment);
	pActionEditRedo = igdeActionRedo::Ref::New(environment);
	
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	
	pActionEditLocoEnabled = cActionEditLocoEnabled::Ref::New(*this);
	pActionEditWBTracking = cActionEditWBEnabled::Ref::New(*this);
	pActionEditShowBones = cActionEditShowBones::Ref::New(*this);
	pActionEditDDBoneSize = cActionEditDDBoneSize::Ref::New(*this);
	
	pActionRuleAddAnim = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimation,
		false, "@Animator.Action.Rule.Animation", pIconRuleAnimation, "@Animator.Action.Rule.Animation.ToolTip");
	pActionRuleAddAnimDiff = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimationDifference,
		false, "@Animator.Action.Rule.AnimationDifference", pIconRuleAnimationDifference, "@Animator.Action.Rule.AnimationDifference.ToolTip");
	pActionRuleAddAnimSelect = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimationSelect,
		false, "@Animator.Action.Rule.AnimationSelect", pIconRuleAnimationSelect, "@Animator.Action.Rule.AnimationSelect.ToolTip");
	pActionRuleAddBoneRot = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertBoneTransformator,
		false, "@Animator.Action.Rule.BoneTransformator", pIconRuleBoneTransformator, "@Animator.Action.Rule.BoneTransformator.ToolTip");
	pActionRuleAddInvKin = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertInverseKinematic,
		false, "@Animator.Action.Rule.InverseKinematic", pIconRuleInverseKinematic, "@Animator.Action.Rule.InverseKinematic.ToolTip");
	pActionRuleAddStateManip = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertStateManipulator,
		false, "@Animator.Action.Rule.StateManipulator", pIconRuleStateManipulator, "@Animator.Action.Rule.StateManipulator.ToolTip");
	pActionRuleAddStateSnap = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertStateSnapshot,
		false, "@Animator.Action.Rule.StateSnapshot", pIconRuleStateSnapshot, "@Animator.Action.Rule.StateSnapshot.ToolTip");
	pActionRuleAddForeignState = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertForeignState,
		false, "@Animator.Action.Rule.ForeignState", pIconRuleForeignState, "@Animator.Action.Rule.ForeignState.ToolTip");
	pActionRuleAddGroup = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertGroup,
		false, "@Animator.Action.Rule.Group", pIconRuleGroup, "@Animator.Action.Rule.Group.ToolTip");
	pActionRuleAddSubAnimator = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertSubAnimator,
		false, "@Animator.Action.Rule.SubAnimator", pIconRuleSubAnimator, "@Animator.Action.Rule.SubAnimator.ToolTip");
	pActionRuleAddTrackTo = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertTrackTo,
		false, "@Animator.Action.Rule.TrackTo", pIconRuleTrackTo, "@Animator.Action.Rule.TrackTo.ToolTip");
	pActionRuleAddLimit = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertLimit,
		false, "@Animator.Action.Rule.Limit", pIconRuleLimit, "@Animator.Action.Rule.Limit.ToolTip");
	pActionRuleAddMirror = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertMirror,
		false, "@Animator.Action.Rule.Mirror", pIconRuleMirror, "@Animator.Action.Rule.Mirror.ToolTip");
	
	pActionRuleAddIntoGroupAnim = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimation,
		false, "@Animator.Action.Rule.Animation", pIconRuleAnimation, "@Animator.Action.Rule.Animation.ToolTip");
	pActionRuleAddIntoGroupAnimDiff = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimationDifference,
		false, "@Animator.Action.Rule.AnimationDifference", pIconRuleAnimationDifference, "@Animator.Action.Rule.AnimationDifference.ToolTip");
	pActionRuleAddIntoGroupAnimSelect = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimationSelect,
		false, "@Animator.Action.Rule.AnimationSelect", pIconRuleAnimationSelect, "@Animator.Action.Rule.AnimationSelect.ToolTip");
	pActionRuleAddIntoGroupBoneRot = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertBoneTransformator,
		false, "@Animator.Action.Rule.BoneTransformator", pIconRuleBoneTransformator, "@Animator.Action.Rule.BoneTransformator.ToolTip");
	pActionRuleAddIntoGroupInvKin = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertInverseKinematic,
		false, "@Animator.Action.Rule.InverseKinematic", pIconRuleInverseKinematic, "@Animator.Action.Rule.InverseKinematic.ToolTip");
	pActionRuleAddIntoGroupStateManip = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertStateManipulator,
		false, "@Animator.Action.Rule.StateManipulator", pIconRuleStateManipulator, "@Animator.Action.Rule.StateManipulator.ToolTip");
	pActionRuleAddIntoGroupStateSnap = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertStateSnapshot,
		false, "@Animator.Action.Rule.StateSnapshot", pIconRuleStateSnapshot, "@Animator.Action.Rule.StateSnapshot.ToolTip");
	pActionRuleAddIntoGroupForeignState = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertForeignState,
		false, "@Animator.Action.Rule.ForeignState", pIconRuleForeignState, "@Animator.Action.Rule.ForeignState.ToolTip");
	pActionRuleAddIntoGroupGroup = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertGroup,
		false, "@Animator.Action.Rule.Group", pIconRuleGroup, "@Animator.Action.Rule.Group.ToolTip");
	pActionRuleAddIntoGroupSubAnimator = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertSubAnimator,
		false, "@Animator.Action.Rule.SubAnimator", pIconRuleSubAnimator, "@Animator.Action.Rule.SubAnimator.ToolTip");
	pActionRuleAddIntoGroupTrackTo = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertTrackTo,
		false, "@Animator.Action.Rule.TrackTo", pIconRuleTrackTo, "@Animator.Action.Rule.TrackTo.ToolTip");
	pActionRuleAddIntoGroupLimit = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertLimit,
		false, "@Animator.Action.Rule.Limit", pIconRuleLimit, "@Animator.Action.Rule.Limit.ToolTip");
	pActionRuleAddIntoGroupMirror = cActionRuleAddIntoGroup::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertMirror,
		false, "@Animator.Action.Rule.Mirror", pIconRuleMirror, "@Animator.Action.Rule.Mirror.ToolTip");
	
	pActionRuleInsertAnim = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimation,
		true, "@Animator.Action.Rule.Animation", pIconRuleAnimation, "@Animator.Action.Rule.Animation.Insert.ToolTip");
	pActionRuleInsertAnimDiff = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimationDifference,
		true, "@Animator.Action.Rule.AnimationDifference", pIconRuleAnimationDifference, "@Animator.Action.Rule.AnimationDifference.Insert.ToolTip");
	pActionRuleInsertAnimSelect = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertAnimationSelect,
		true, "@Animator.Action.Rule.AnimationSelect", pIconRuleAnimationSelect, "@Animator.Action.Rule.AnimationSelect.Insert.ToolTip");
	pActionRuleInsertBoneRot = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertBoneTransformator,
		true, "@Animator.Action.Rule.BoneTransformator", pIconRuleBoneTransformator, "@Animator.Action.Rule.BoneTransformator.Insert.ToolTip");
	pActionRuleInsertInvKin = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertInverseKinematic,
		true, "@Animator.Action.Rule.InverseKinematic", pIconRuleInverseKinematic, "@Animator.Action.Rule.InverseKinematic.Insert.ToolTip");
	pActionRuleInsertStateManip = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertStateManipulator,
		true, "@Animator.Action.Rule.StateManipulator", pIconRuleStateManipulator, "@Animator.Action.Rule.StateManipulator.Insert.ToolTip");
	pActionRuleInsertStateSnap = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertStateSnapshot,
		true, "@Animator.Action.Rule.StateSnapshot", pIconRuleStateSnapshot, "@Animator.Action.Rule.StateSnapshot.Insert.ToolTip");
	pActionRuleInsertForeignState = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertForeignState,
		true, "@Animator.Action.Rule.ForeignState", pIconRuleForeignState, "@Animator.Action.Rule.ForeignState.Insert.ToolTip");
	pActionRuleInsertGroup = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertGroup,
		true, "@Animator.Action.Rule.Group", pIconRuleGroup, "@Animator.Action.Rule.Group.Insert.ToolTip");
	pActionRuleInsertSubAnimator = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertSubAnimator,
		true, "@Animator.Action.Rule.SubAnimator", pIconRuleSubAnimator, "@Animator.Action.Rule.SubAnimator.Insert.ToolTip");
	pActionRuleInsertTrackTo = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertTrackTo,
		true, "@Animator.Action.Rule.TrackTo", pIconRuleTrackTo, "@Animator.Action.Rule.TrackTo.Insert.ToolTip");
	pActionRuleInsertLimit = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertLimit,
		true, "@Animator.Action.Rule.Limit", pIconRuleLimit, "@Animator.Action.Rule.Limit.Insert.ToolTip");
	pActionRuleInsertMirror = cActionRuleAdd::Ref::New(*this, deAnimatorRuleVisitorIdentify::ertMirror,
		true, "@Animator.Action.Rule.Mirror", pIconRuleMirror, "@Animator.Action.Rule.Mirror.Insert.ToolTip");
	
	
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
}

void aeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void aeWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	AddSharedToolBar(pTBEdit);
}

void aeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "@Animator.Menu.File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "@Animator.Menu.Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
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

void aeWindowMain::pUpdateMetaContexts(){
	auto list = igdeMetaContext::Data::Ref::New();
	auto &d = list->GetData();
	auto &a = pAnimator;
	d.Add(a ? a->GetMetaContext() : aeAnimator::CreateMetaContext(*this, nullptr));
	d.Add(a ? a->GetMetaContextController() : aeAnimator::CreateMetaContextController(*this, nullptr));
	d.Add(a ? a->GetMetaContextLink() : aeAnimator::CreateMetaContextLink(*this, nullptr));
	d.Add(a ? a->GetMetaContextRule() : aeAnimator::CreateMetaContextRule(*this, nullptr));
	d.Add(a ? a->GetMetaContextPlayground() : aeAnimator::CreateMetaContextPlayground(*this, nullptr));
	d.Add(a ? a->GetMetaContextView() : aeAnimator::CreateMetaContextView(*this, nullptr));
	SetMetaContexts(list);
	
	// temporary
	pWindowProperties->SetData(list);
}
