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

#include "gdeWPGameDefinition.h"
#include "gdeWPGameDefinitionListener.h"
#include "gdeWindowProperties.h"
#include "gdeWPPathList.h"
#include "gdeWPPropertyList.h"
#include "../gdeWindowMain.h"
#include "../../clipboard/gdeClipboardDataProperty.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/filepattern/gdeFilePattern.h"
#include "../../gamedef/property/gdeProperty.h"
#include "../../undosys/gamedef/gdeUGDSetID.h"
#include "../../undosys/gamedef/gdeUGDSetDescription.h"
#include "../../undosys/gamedef/gdeUGDSetBasePath.h"
#include "../../undosys/gamedef/gdeUGDSetScriptModule.h"
#include "../../undosys/gamedef/gdeUGDSetVFSPath.h"
#include "../../undosys/gamedef/gdeUGDSetAutoFindPathObjectClasses.h"
#include "../../undosys/gamedef/gdeUGDSetAutoFindPathSkins.h"
#include "../../undosys/gamedef/gdeUGDSetAutoFindPathSkies.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPropertyAdd.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPropertyRemove.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetName.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetDescription.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetType.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetMinValue.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetMaxValue.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetDefaultValue.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetOptions.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetPathPatternType.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetIdentifierGroup.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPToggleIdentifierUsage.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPAdd.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPRemove.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPSetName.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPSetPattern.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPSetExtension.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPropertyAdd.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPropertyRemove.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetName.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetDescription.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetType.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetMinValue.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetMaxValue.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetDefaultValue.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetOptions.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetPathPatternType.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetIdentifierGroup.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPToggleIdentifierUsage.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPAdd.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPRemove.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPSetName.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPSetPattern.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPSetExtension.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPGameDefinition &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if(!gameDefinition || textField->GetText() == gameDefinition->GetID()){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*textField, gameDefinition));
		if(undo){
			gameDefinition->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeGameDefinition *gameDefinition) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPGameDefinition &pPanel;
	
public:
	cBaseAction(gdeWPGameDefinition &panel, const char *text, igdeIcon *icon,
		const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if(!gameDefinition){
			return;
		}
		
		igdeUndo::Ref undo(OnActionGameDefinition(gameDefinition));
		if(undo){
			gameDefinition->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionGameDefinition(gdeGameDefinition *gameDefinition) = 0;
	
	virtual void Update(){
		SetEnabled(pPanel.GetGameDefinition().IsNotNull());
	}
};


class cEditId : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cEditId> Ref;
	cEditId(gdeWPGameDefinition &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeGameDefinition *gameDefinition){
		if(textField.GetText() == gameDefinition->GetID()){
			return {};
		}
		return gdeUGDSetID::Ref::New(gameDefinition, textField.GetText());
	}
};

class cEditDescription : public igdeTextAreaListener{
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cEditDescription> Ref;
	cEditDescription(gdeWPGameDefinition &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if(!gameDefinition || textArea->GetText() == gameDefinition->GetDescription()){
			return;
		}
		
		gameDefinition->GetUndoSystem()->Add(gdeUGDSetDescription::Ref::New(
			gameDefinition, textArea->GetText()));
	}
};

class cEditBasePath : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditBasePath> Ref;
	cEditBasePath(gdeWPGameDefinition &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeGameDefinition *gameDefinition) override{
		if(textField.GetText() == gameDefinition->GetBasePath()){
			return {};
		}
		return gdeUGDSetBasePath::Ref::New(gameDefinition, textField.GetText());
	}
};

class cActionBasePath : public cBaseAction{
public:
	typedef deTObjectReference<cActionBasePath> Ref;
	
private:
	igdeTextField &pTextField;
	
public:
	cActionBasePath(gdeWPGameDefinition &panel, igdeTextField &textField) :
	cBaseAction(panel, "...", nullptr, "Base path to project data files"),
	pTextField(textField){}
	
	igdeUndo::Ref OnActionGameDefinition(gdeGameDefinition *gameDefinition) override{
		decString basePath(gameDefinition->GetBasePath());
		if(igdeCommonDialogs::GetDirectory(pPanel.GetParentWindow(), "Select Project Data Directory", basePath)){
			pTextField.SetText(basePath);
			pTextField.NotifyTextChanged();
		}
		return {};
	}
};

class cEditVfsPath : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditVfsPath> Ref;
	cEditVfsPath(gdeWPGameDefinition &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeGameDefinition *gameDefinition) override{
		if(textField.GetText() == gameDefinition->GetVFSPath()){
			return {};
		}
		return gdeUGDSetVFSPath::Ref::New(gameDefinition, textField.GetText());
	}
};

class cEditScriptModule : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditScriptModule> Ref;
	cEditScriptModule(gdeWPGameDefinition &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeGameDefinition *gameDefinition) override{
		if(textField.GetText() == gameDefinition->GetScriptModule()){
			return {};
		}
		return gdeUGDSetScriptModule::Ref::New(gameDefinition, textField.GetText());
	}
};


class cEditWorldProperties : public gdeWPPropertyList {
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cEditWorldProperties> Ref;
	cEditWorldProperties(gdeWPGameDefinition &panel) :
	gdeWPPropertyList(panel.GetEnvironment()), pPanel(panel){
		SetClipboard(&panel.GetWindowProperties().GetWindowMain().GetClipboard());
	}
	
	igdeUndo::Ref UndoAdd(gdeProperty *property) override{
		return gdeUGDWPropertyAdd::Ref::New(pPanel.GetGameDefinition(), property);
	}
	
	igdeUndo::Ref UndoRemove(gdeProperty *property) override{
		return gdeUGDWPropertyRemove::Ref::New(pPanel.GetGameDefinition(), property);
	}
	
	igdeUndo::Ref UndoPaste(gdeProperty *property) override{
		const gdeUGDWPropertyAdd::Ref undo = gdeUGDWPropertyAdd::Ref::New(pPanel.GetGameDefinition(), property);
		undo->SetShortInfo("Paste property");
		return undo;
	}
	
	virtual igdeUndo::Ref UndoName(gdeProperty *property, const decString &name){
		return gdeUGDWPSetName::Ref::New(pPanel.GetGameDefinition(), property, name);
	}
	
	virtual igdeUndo::Ref UndoDescription(gdeProperty *property, const decString &description){
		return gdeUGDWPSetDescription::Ref::New(pPanel.GetGameDefinition(), property, description);
	}
	
	igdeUndo::Ref UndoType(gdeProperty *property, gdeProperty::ePropertyTypes type) override{
		return gdeUGDWPSetType::Ref::New(pPanel.GetGameDefinition(), property, type);
	}
	
	igdeUndo::Ref UndoMinimumValue(gdeProperty *property, float value) override{
		return gdeUGDWPSetMinValue::Ref::New(pPanel.GetGameDefinition(), property, value);
	}
	
	igdeUndo::Ref UndoMaximumValue(gdeProperty *property, float value) override{
		return gdeUGDWPSetMaxValue::Ref::New(pPanel.GetGameDefinition(), property, value);
	}
	
	virtual igdeUndo::Ref UndoDefaultValue(gdeProperty *property, const decString &newValue, const decString &oldValue){
		return gdeUGDWPSetDefaultValue::Ref::New(pPanel.GetGameDefinition(), property, newValue, oldValue);
	}
	
	igdeUndo::Ref UndoOptions(gdeProperty *property, const decStringList &options) override{
		return gdeUGDWPSetOptions::Ref::New(pPanel.GetGameDefinition(), property, options);
	}
	
	igdeUndo::Ref UndoPathPatternType(gdeProperty *property, gdeProperty::ePathPatternTypes type) override{
		return gdeUGDWPSetPathPatternType::Ref::New(pPanel.GetGameDefinition(), property, type);
	}
	
	virtual igdeUndo::Ref UndoIdentifierGroup(gdeProperty *property, const decString &identifier){
		return gdeUGDWPSetIdentifierGroup::Ref::New(pPanel.GetGameDefinition(), property, identifier);
	}
	
	igdeUndo::Ref UndoIdentifierUsage(gdeProperty *property) override{
		return gdeUGDWPToggleIdentifierUsage::Ref::New(pPanel.GetGameDefinition(), property);
	}
	
	igdeUndo::Ref UndoCustomFilePatternAdd(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUGDWPCFPAdd::Ref::New(pPanel.GetGameDefinition(), property, filePattern);
	}
	
	igdeUndo::Ref UndoCustomFilePatternRemove(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUGDWPCFPRemove::Ref::New(pPanel.GetGameDefinition(), property, filePattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternName(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name){
		return gdeUGDWPCFPSetName::Ref::New(pPanel.GetGameDefinition(), property, filePattern, name);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternPattern(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern){
		return gdeUGDWPCFPSetPattern::Ref::New(pPanel.GetGameDefinition(), property, filePattern, pattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternExtension(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension){
		return gdeUGDWPCFPSetExtension::Ref::New(pPanel.GetGameDefinition(), property, filePattern, extension);
	}
};


class cEditDecalProperties : public gdeWPPropertyList {
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cEditDecalProperties> Ref;
	cEditDecalProperties(gdeWPGameDefinition &panel) :
	gdeWPPropertyList(panel.GetEnvironment()), pPanel(panel){
		SetClipboard(&panel.GetWindowProperties().GetWindowMain().GetClipboard());
	}
	
	igdeUndo::Ref UndoAdd(gdeProperty *property) override{
		return gdeUGDDPropertyAdd::Ref::New(pPanel.GetGameDefinition(), property);
	}
	
	igdeUndo::Ref UndoRemove(gdeProperty *property) override{
		return gdeUGDDPropertyRemove::Ref::New(pPanel.GetGameDefinition(), property);
	}
	
	igdeUndo::Ref UndoPaste(gdeProperty *property) override{
		const gdeUGDDPropertyAdd::Ref undo = gdeUGDDPropertyAdd::Ref::New(pPanel.GetGameDefinition(), property);
		undo->SetShortInfo("Paste property");
		return undo;
	}
	
	virtual igdeUndo::Ref UndoName(gdeProperty *property, const decString &name){
		return gdeUGDDPSetName::Ref::New(pPanel.GetGameDefinition(), property, name);
	}
	
	virtual igdeUndo::Ref UndoDescription(gdeProperty *property, const decString &description){
		return gdeUGDDPSetDescription::Ref::New(pPanel.GetGameDefinition(), property, description);
	}
	
	igdeUndo::Ref UndoType(gdeProperty *property, gdeProperty::ePropertyTypes type) override{
		return gdeUGDDPSetType::Ref::New(pPanel.GetGameDefinition(), property, type);
	}
	
	igdeUndo::Ref UndoMinimumValue(gdeProperty *property, float value) override{
		return gdeUGDDPSetMinValue::Ref::New(pPanel.GetGameDefinition(), property, value);
	}
	
	igdeUndo::Ref UndoMaximumValue(gdeProperty *property, float value) override{
		return gdeUGDDPSetMaxValue::Ref::New(pPanel.GetGameDefinition(), property, value);
	}
	
	virtual igdeUndo::Ref UndoDefaultValue(gdeProperty *property, const decString &newValue, const decString &oldValue){
		return gdeUGDDPSetDefaultValue::Ref::New(pPanel.GetGameDefinition(), property, newValue, oldValue);
	}
	
	igdeUndo::Ref UndoOptions(gdeProperty *property, const decStringList &options) override{
		return gdeUGDDPSetOptions::Ref::New(pPanel.GetGameDefinition(), property, options);
	}
	
	igdeUndo::Ref UndoPathPatternType(gdeProperty *property, gdeProperty::ePathPatternTypes type) override{
		return gdeUGDDPSetPathPatternType::Ref::New(pPanel.GetGameDefinition(), property, type);
	}
	
	virtual igdeUndo::Ref UndoIdentifierGroup(gdeProperty *property, const decString &identifier){
		return gdeUGDDPSetIdentifierGroup::Ref::New(pPanel.GetGameDefinition(), property, identifier);
	}
	
	igdeUndo::Ref UndoIdentifierUsage(gdeProperty *property) override{
		return gdeUGDDPToggleIdentifierUsage::Ref::New(pPanel.GetGameDefinition(), property);
	}
	
	igdeUndo::Ref UndoCustomFilePatternAdd(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUGDDPCFPAdd::Ref::New(pPanel.GetGameDefinition(), property, filePattern);
	}
	
	igdeUndo::Ref UndoCustomFilePatternRemove(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUGDDPCFPRemove::Ref::New(pPanel.GetGameDefinition(), property, filePattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternName(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name){
		return gdeUGDDPCFPSetName::Ref::New(pPanel.GetGameDefinition(), property, filePattern, name);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternPattern(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern){
		return gdeUGDDPCFPSetPattern::Ref::New(pPanel.GetGameDefinition(), property, filePattern, pattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternExtension(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension){
		return gdeUGDDPCFPSetExtension::Ref::New(pPanel.GetGameDefinition(), property, filePattern, extension);
	}
};


class cEditAutoFindPathObjectClasses : public gdeWPPathList {
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cEditAutoFindPathObjectClasses> Ref;
	cEditAutoFindPathObjectClasses(gdeWPGameDefinition &panel) :
	gdeWPPathList(panel.GetEnvironment().GetUIHelper(), 3, "Auto Find Path Object Classes"),
	pPanel(panel){}
	
	igdeUndo::Ref UndoSet(const decStringList &paths) override{
		return gdeUGDSetAutoFindPathObjectClasses::Ref::New(pPanel.GetGameDefinition(), paths);
	}
};

class cEditAutoFindPathSkins : public gdeWPPathList {
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cEditAutoFindPathSkins> Ref;
	cEditAutoFindPathSkins(gdeWPGameDefinition &panel) :
	gdeWPPathList(panel.GetEnvironment().GetUIHelper(), 3, "Auto Find Path SkinsClasses"),
	pPanel(panel){}
	
	igdeUndo::Ref UndoSet(const decStringList &paths) override{
		return gdeUGDSetAutoFindPathSkins::Ref::New(pPanel.GetGameDefinition(), paths);
	}
};

class cEditAutoFindPathSkies : public gdeWPPathList {
	gdeWPGameDefinition &pPanel;
	
public:
	typedef deTObjectReference<cEditAutoFindPathSkies> Ref;
	cEditAutoFindPathSkies(gdeWPGameDefinition &panel) :
	gdeWPPathList(panel.GetEnvironment().GetUIHelper(), 3, "Auto Find Path Skies"),
	pPanel(panel){}
	
	igdeUndo::Ref UndoSet(const decStringList &paths) override{
		return gdeUGDSetAutoFindPathSkies::Ref::New(pPanel.GetGameDefinition(), paths);
	}
};

}



// Class gdeWPGameDefinition
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPGameDefinition::gdeWPGameDefinition(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPGameDefinitionListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// game definition
	helper.GroupBox(content, groupBox, "Game Definition:");
	helper.EditString(groupBox, "Identifier:", "Identifier", pEditID, cEditId::Ref::New(*this));
	helper.EditString(groupBox, "Description:", "Description",
		pEditDescription, 5, cEditDescription::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Base Path:", "Base path to project data files", frameLine);
	helper.EditString(frameLine, "Base path to project data files",
		pEditBasePath, cEditBasePath::Ref::New(*this));
	pActionBasePath = cActionBasePath::Ref::New(*this, pEditBasePath);
	helper.Button(frameLine, pActionBasePath);
	
	helper.EditString(groupBox, "VFS Path:", "VFS path the base path will be visible at",
		pEditVFSPath, cEditVfsPath::Ref::New(*this));
	helper.EditString(groupBox, "Script Module:", "Script module this game definition is compatible with",
		pEditScriptModule, cEditScriptModule::Ref::New(*this));
	
	// information
	helper.GroupBox(content, groupBox, "Information:");
	
	helper.EditString(groupBox, "Use Base Path:",
		"Actually used base path (can differ if this is the project game definition)",
		pEditUseBasePath, {});
	pEditUseBasePath->SetEditable(false);
	
	helper.CheckBox(groupBox, "Is Project Game Definition", "Is Project Game Definition", pChkIsProjectGameDef);
	pChkIsProjectGameDef->SetEnabled(false);
	
	// world properties
	helper.GroupBoxFlow(content, groupBox, "World Properties:", false, true);
	pEditWorldProperties = cEditWorldProperties::Ref::New(*this);
	groupBox->AddChild(pEditWorldProperties);
	
	// decal properties
	helper.GroupBoxFlow(content, groupBox, "Decal Properties:", false, true);
	pEditDecalProperties = cEditDecalProperties::Ref::New(*this);
	groupBox->AddChild(pEditDecalProperties);
	
	// auto find path
	helper.GroupBoxFlow(content, groupBox, "Auto Find Path Object Classes:", false, true);
	pEditAutoFindPathObjectClasses = cEditAutoFindPathObjectClasses::Ref::New(*this);
	groupBox->AddChild(pEditAutoFindPathObjectClasses);
	
	helper.GroupBoxFlow(content, groupBox, "Auto Find Path Skins:", false, true);
	pEditAutoFindPathSkins = cEditAutoFindPathSkins::Ref::New(*this);
	groupBox->AddChild(pEditAutoFindPathSkins);
	
	helper.GroupBoxFlow(content, groupBox, "Auto Find Path Skies:", false, true);
	pEditAutoFindPathSkies = cEditAutoFindPathSkies::Ref::New(*this);
	groupBox->AddChild(pEditAutoFindPathSkies);
}

gdeWPGameDefinition::~gdeWPGameDefinition(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPGameDefinition::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	gdeWPPropertyList &worldProperties = (gdeWPPropertyList&)(igdeWidget&)pEditWorldProperties;
	worldProperties.SetPropertyList(nullptr);
	worldProperties.SetGameDefinition(nullptr);
	
	gdeWPPropertyList &decalProperties = (gdeWPPropertyList&)(igdeWidget&)pEditDecalProperties;
	decalProperties.SetPropertyList(nullptr);
	decalProperties.SetGameDefinition(nullptr);
	
	gdeWPPathList &autoFindPathObjectClasses = (gdeWPPathList&)(igdeWidget&)pEditAutoFindPathObjectClasses;
	autoFindPathObjectClasses.SetPathList(nullptr);
	autoFindPathObjectClasses.SetUndoSystem(nullptr);
	
	gdeWPPathList &autoFindPathSkins = (gdeWPPathList&)(igdeWidget&)pEditAutoFindPathSkins;
	autoFindPathSkins.SetPathList(nullptr);
	autoFindPathSkins.SetUndoSystem(nullptr);
	
	gdeWPPathList &autoFindPathSkies = (gdeWPPathList&)(igdeWidget&)pEditAutoFindPathSkies;
	autoFindPathSkies.SetPathList(nullptr);
	autoFindPathSkies.SetUndoSystem(nullptr);
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		worldProperties.SetPropertyList(&gameDefinition->GetWorldProperties());
		worldProperties.SetGameDefinition(gameDefinition);
		
		decalProperties.SetPropertyList(&gameDefinition->GetDecalProperties());
		decalProperties.SetGameDefinition(gameDefinition);
		
		autoFindPathObjectClasses.SetPathList(&gameDefinition->GetAutoFindPathObjectClasses());
		autoFindPathObjectClasses.SetUndoSystem(gameDefinition->GetUndoSystem());
		
		autoFindPathSkins.SetPathList(&gameDefinition->GetAutoFindPathSkins());
		autoFindPathSkins.SetUndoSystem(gameDefinition->GetUndoSystem());
		
		autoFindPathSkies.SetPathList(&gameDefinition->GetAutoFindPathSkies());
		autoFindPathSkies.SetUndoSystem(gameDefinition->GetUndoSystem());
	}
	
	UpdateWorld();
}



gdeProperty *gdeWPGameDefinition::GetActiveWorldProperty() const{
	return ((gdeWPPropertyList&)(igdeWidget&)pEditWorldProperties).GetProperty();
}

gdeProperty *gdeWPGameDefinition::GetActiveDecalProperty() const{
	return ((gdeWPPropertyList&)(igdeWidget&)pEditDecalProperties).GetProperty();
}



void gdeWPGameDefinition::UpdateWorld(){
	if(pGameDefinition){
		pEditID->SetText(pGameDefinition->GetID());
		pEditDescription->SetText(pGameDefinition->GetDescription());
		pEditBasePath->SetText(pGameDefinition->GetBasePath());
		pEditVFSPath->SetText(pGameDefinition->GetVFSPath());
		pEditScriptModule->SetText(pGameDefinition->GetScriptModule());
		
		if(pGameDefinition->GetIsProjectGameDef()){
			const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
			decPath path;
			path.SetFromNative(gameProject.GetDirectoryPath());
			path.AddUnixPath(gameProject.GetPathData());
			pEditUseBasePath->SetText(path.GetPathNative());
			pChkIsProjectGameDef->SetChecked(true);
			
		}else{
			pEditUseBasePath->SetText(pGameDefinition->GetBasePath());
			pChkIsProjectGameDef->SetChecked(false);
		}
		
	}else{
		pEditID->ClearText();
		pEditDescription->ClearText();
		pEditBasePath->ClearText();
		pEditVFSPath->ClearText();
		pEditScriptModule->ClearText();
		pEditUseBasePath->ClearText();
		pChkIsProjectGameDef->SetChecked(false);
	}
	
	const bool enabled = pGameDefinition != nullptr;
	pEditID->SetEnabled(enabled);
	pEditDescription->SetEnabled(enabled);
	pEditBasePath->SetEnabled(enabled);
	pActionBasePath->Update();
	pEditVFSPath->SetEnabled(enabled);
	pEditScriptModule->SetEnabled(enabled);
	
	UpdateWorldProperties();
	UpdateDecalProperties();
	UpdateAutoFindPathObjectClasses();
	UpdateAutoFindPathSkins();
	UpdateAutoFindPathSkies();
}



void gdeWPGameDefinition::UpdateWorldProperties(){
	((gdeWPPropertyList&)(igdeWidget&)pEditWorldProperties).UpdateList();
}

void gdeWPGameDefinition::UpdateWorldProperty(){
	((gdeWPPropertyList&)(igdeWidget&)pEditWorldProperties).UpdateProperty();
}

void gdeWPGameDefinition::UpdateWorldPropertyCustomPatternList(){
	((gdeWPPropertyList&)(igdeWidget&)pEditWorldProperties).UpdateCustomPatternList();
}

void gdeWPGameDefinition::UpdateWorldPropertyCustomPattern(){
	((gdeWPPropertyList&)(igdeWidget&)pEditWorldProperties).UpdateCustomPattern();
}



void gdeWPGameDefinition::UpdateAutoFindPathObjectClasses(){
	((gdeWPPathList&)(igdeWidget&)pEditAutoFindPathObjectClasses).UpdateList();
}

void gdeWPGameDefinition::UpdateAutoFindPathSkins(){
	((gdeWPPathList&)(igdeWidget&)pEditAutoFindPathSkins).UpdateList();
}

void gdeWPGameDefinition::UpdateAutoFindPathSkies(){
	((gdeWPPathList&)(igdeWidget&)pEditAutoFindPathSkies).UpdateList();
}



void gdeWPGameDefinition::UpdateDecalProperties(){
	((gdeWPPropertyList&)(igdeWidget&)pEditDecalProperties).UpdateList();
}

void gdeWPGameDefinition::UpdateDecalProperty(){
	((gdeWPPropertyList&)(igdeWidget&)pEditDecalProperties).UpdateProperty();
}

void gdeWPGameDefinition::UpdateDecalPropertyCustomPatternList(){
	((gdeWPPropertyList&)(igdeWidget&)pEditDecalProperties).UpdateCustomPatternList();
}

void gdeWPGameDefinition::UpdateDecalPropertyCustomPattern(){
	((gdeWPPropertyList&)(igdeWidget&)pEditDecalProperties).UpdateCustomPattern();
}
