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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "deglDialogGameProblems.h"
#include "deglDGPPanelSystem.h"
#include "../../deglWindowMain.h"
#include "../../deglGuiBuilder.h"
#include "../../../deglLauncher.h"

#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/delEngineModuleList.h>
#include <delauncher/game/fileformat/delFileFormat.h>

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP(deglDGPPanelSystem) deglDGPPanelSystemMap[]={
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_GRA, deglDGPPanelSystem::onCBModGraChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_GRA, deglDGPPanelSystem::onCBModGraChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_INP, deglDGPPanelSystem::onCBModInpChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_INP, deglDGPPanelSystem::onCBModInpChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_PHY, deglDGPPanelSystem::onCBModPhyChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_PHY, deglDGPPanelSystem::onCBModPhyChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_AMR, deglDGPPanelSystem::onCBModAmrChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_AMR, deglDGPPanelSystem::onCBModAmrChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_AI, deglDGPPanelSystem::onCBModAIChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_AI, deglDGPPanelSystem::onCBModAIChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_CR, deglDGPPanelSystem::onCBModCRChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_CR, deglDGPPanelSystem::onCBModCRChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_AUD, deglDGPPanelSystem::onCBModAudChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_AUD, deglDGPPanelSystem::onCBModAudChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_NET, deglDGPPanelSystem::onCBModNetChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_NET, deglDGPPanelSystem::onCBModNetChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_SYN, deglDGPPanelSystem::onCBModSynChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_SYN, deglDGPPanelSystem::onCBModSynChanged),
	FXMAPFUNC(SEL_COMMAND, deglDGPPanelSystem::ID_CB_MOD_VR, deglDGPPanelSystem::onCBModVRChanged),
	FXMAPFUNC(SEL_CHANGED, deglDGPPanelSystem::ID_CB_MOD_VR, deglDGPPanelSystem::onCBModVRChanged),
};



// Class deglDGPPanelSystem
/////////////////////////////////

FXIMPLEMENT(deglDGPPanelSystem, FXVerticalFrame, deglDGPPanelSystemMap, ARRAYNUMBER(deglDGPPanelSystemMap))

// Constructor, destructor
////////////////////////////

deglDGPPanelSystem::deglDGPPanelSystem(){}

deglDGPPanelSystem::deglDGPPanelSystem(deglDialogGameProblems *parentDialog, FXComposite *container) :
FXVerticalFrame(container, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0),
pParentDialog(parentDialog),
pStatusWorking(false)
{
	if(!parentDialog) DETHROW(deeInvalidParam);
	
	FXScrollWindow *scrollWindow;
	FXVerticalFrame *frameContent;
	
	pSysAI.type = deModuleSystem::emtAI;
	pSysAnimator.type = deModuleSystem::emtAnimator;
	pSysAudio.type = deModuleSystem::emtAudio;
	pSysCrashRecovery.type = deModuleSystem::emtCrashRecovery;
	pSysGraphic.type = deModuleSystem::emtGraphic;
	pSysInput.type = deModuleSystem::emtInput;
	pSysNetwork.type = deModuleSystem::emtNetwork;
	pSysSynthesizer.type = deModuleSystem::emtSynthesizer;
	pSysPhysics.type = deModuleSystem::emtPhysics;
	pSysVR.type = deModuleSystem::emtVR;
	
	// create content
	scrollWindow = new FXScrollWindow(this, LAYOUT_FILL_X | LAYOUT_FILL_Y | SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK);
	frameContent = new FXVerticalFrame(scrollWindow, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3);
	
	pCreateSystem(pSysGraphic, "Graphic Module:", "Select the Graphic Module to use", ID_CB_MOD_GRA, frameContent);
	pCreateSystem(pSysInput, "Input Module:", "Select the Input Module to use", ID_CB_MOD_INP, frameContent);
	pCreateSystem(pSysPhysics, "Physics Module:", "Select the Physics Module to use", ID_CB_MOD_PHY, frameContent);
	pCreateSystem(pSysAnimator, "Animator Module:", "Select the Animator Module to use", ID_CB_MOD_AMR, frameContent);
	pCreateSystem(pSysAI, "AI Module:", "Select the AI Module to use", ID_CB_MOD_AI, frameContent);
	pCreateSystem(pSysCrashRecovery, "Crash Recovery Module:", "Select the Crash Recovery Module to use", ID_CB_MOD_CR, frameContent);
	pCreateSystem(pSysAudio, "Audio Module:", "Select the Audio Module to use", ID_CB_MOD_AUD, frameContent);
	pCreateSystem(pSysSynthesizer, "Synthesizer Module:", "Select Synthesizer Module to use", ID_CB_MOD_SYN, frameContent);
	pCreateSystem(pSysNetwork, "Network Module:", "Select the Network Module to use", ID_CB_MOD_NET, frameContent);
	pCreateSystem(pSysVR, "VR Module:", "Select the VR Module to use", ID_CB_MOD_VR, frameContent);
}

deglDGPPanelSystem::~deglDGPPanelSystem(){
}



// Management
///////////////

void deglDGPPanelSystem::UpdateSystemModuleLists(){
	const delEngineModuleList &moduleList = pParentDialog->GetWindowMain()->GetLauncher()->GetEngine().GetModules();
	int i, count = moduleList.GetCount();
	
	pSysGraphic.combobox->clearItems();
	pSysInput.combobox->clearItems();
	pSysPhysics.combobox->clearItems();
	pSysAnimator.combobox->clearItems();
	pSysAI.combobox->clearItems();
	pSysCrashRecovery.combobox->clearItems();
	pSysAudio.combobox->clearItems();
	pSysSynthesizer.combobox->clearItems();
	pSysNetwork.combobox->clearItems();
	pSysVR.combobox->clearItems();
	
	for(i=0; i<count; i++){
		const delEngineModule &module = *moduleList.GetAt(i);
		
		switch(module.GetType()){
		case deModuleSystem::emtGraphic:
			pSysGraphic.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtInput:
			pSysInput.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtPhysics:
			pSysPhysics.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtAnimator:
			pSysAnimator.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtAI:
			pSysAI.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtCrashRecovery:
			pSysCrashRecovery.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtAudio:
			pSysAudio.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtSynthesizer:
			pSysSynthesizer.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtNetwork:
			pSysNetwork.combobox->appendItem(module.GetName().GetString());
			break;
			
		case deModuleSystem::emtVR:
			pSysVR.combobox->appendItem(module.GetName().GetString());
			break;
			
		default:
			break;
		}
	}
	
	pSysGraphic.combobox->sortItems();
	pSysInput.combobox->sortItems();
	pSysPhysics.combobox->sortItems();
	pSysAnimator.combobox->sortItems();
	pSysAI.combobox->sortItems();
	pSysCrashRecovery.combobox->sortItems();
	pSysAudio.combobox->sortItems();
	pSysSynthesizer.combobox->sortItems();
	pSysNetwork.combobox->sortItems();
	pSysVR.combobox->sortItems();
}

void deglDGPPanelSystem::UpdatePanel(){
	pStatusWorking = true;
	
	UpdateSystem(pSysGraphic, pParentDialog->GetWorkingProfile()->GetModuleGraphic().GetString());
	UpdateSystem(pSysInput, pParentDialog->GetWorkingProfile()->GetModuleInput().GetString());
	UpdateSystem(pSysPhysics, pParentDialog->GetWorkingProfile()->GetModulePhysics().GetString());
	UpdateSystem(pSysAnimator, pParentDialog->GetWorkingProfile()->GetModuleAnimator().GetString());
	UpdateSystem(pSysAI, pParentDialog->GetWorkingProfile()->GetModuleAI().GetString());
	UpdateSystem(pSysCrashRecovery, pParentDialog->GetWorkingProfile()->GetModuleCrashRecovery().GetString());
	UpdateSystem(pSysAudio, pParentDialog->GetWorkingProfile()->GetModuleAudio().GetString());
	UpdateSystem(pSysSynthesizer, pParentDialog->GetWorkingProfile()->GetModuleSynthesizer().GetString());
	UpdateSystem(pSysNetwork, pParentDialog->GetWorkingProfile()->GetModuleNetwork().GetString());
	UpdateSystem(pSysVR, pParentDialog->GetWorkingProfile()->GetModuleVR().GetString());
}

void deglDGPPanelSystem::UpdateSystem(sSystem &system, const char *moduleName){
	const delEngineModuleList &modules = pParentDialog->GetWindowMain()->GetLauncher()->GetEngine().GetModules();
	FXColor colorBack = pParentDialog->GetBackColorValid();
	FXColor colorText = pParentDialog->GetTextColorValid();
	delEngineModule *module;
	bool working = true;
	
	system.combobox->setText(moduleName);
	module = modules.GetNamed(moduleName);
	
	if(!module){
		system.problem->setText("No module exists with the given name.");
		working = false;
		
	}else if(module->GetType() != system.type){
		system.problem->setText("Module is not of the correct type.");
		working = false;
		
	}else if(module->GetStatus() != delEngineModule::emsReady){
		system.problem->setText("Module has problems and can not be used.");
		working = false;
	}
	
	if(working){
		system.problem->setText("");
		system.problem->hide();
		
	}else{
		colorBack = pParentDialog->GetBackColorProblem();
		colorText = pParentDialog->GetTextColorProblem();
		system.problem->show();
	}
	
	system.box->setBackColor(colorBack);
	system.label->setBackColor(colorBack);
	system.label->setTextColor(colorText);
	system.problem->setBackColor(colorBack);
	system.problem->setTextColor(colorText);
	
	pStatusWorking &= working;
}



// Events
///////////

long deglDGPPanelSystem::onCBModGraChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleGraphic(pSysGraphic.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModInpChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleInput(pSysInput.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModPhyChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModulePhysics(pSysPhysics.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModAmrChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleAnimator(pSysAnimator.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModAIChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleAI(pSysAI.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModCRChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleCrashRecovery(pSysCrashRecovery.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModAudChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleAudio(pSysAudio.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModNetChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleNetwork(pSysNetwork.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModSynChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleSynthesizer(pSysSynthesizer.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}

long deglDGPPanelSystem::onCBModVRChanged(FXObject*, FXSelector, void*){
	pParentDialog->GetWorkingProfile()->SetModuleVR(pSysVR.combobox->getText().text());
	pParentDialog->UpdatePanels();
	return 1;
}



// Private Functions
//////////////////////

void deglDGPPanelSystem::pCreateSystem(sSystem &system, const char *textLabel,
const char *toolText, int comboBoxSelector, FXComposite *container){
	const deglGuiBuilder &guiBuilder = *pParentDialog->GetWindowMain()->GetGuiBuilder();
	FXHorizontalFrame *frameLine;
	int fixTextWidth = 180;
	
	system.box = new FXVerticalFrame(container, FRAME_GROOVE | LAYOUT_FILL_X,
		0, 0, 0, 0, 1, 1, 1, 1, 0, 0);
	frameLine = new FXHorizontalFrame(system.box, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	system.label = new FXLabel(frameLine, textLabel, nullptr,
		LABEL_NORMAL | LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y, 0, 0, fixTextWidth);
	system.label->setJustify(JUSTIFY_LEFT | JUSTIFY_CENTER_Y);
	system.label->setTipText(toolText);
	
	system.combobox = guiBuilder.CreateComboBox(frameLine, this,
		comboBoxSelector, toolText, true, 20, 5, false);
	system.combobox->setSortFunc(deglGuiBuilder::SortListItemByName);
	
	system.problem = new FXLabel(system.box, "This label contains a short problem description.",
		nullptr, LABEL_NORMAL | LAYOUT_FILL_X);
	system.problem->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
}
