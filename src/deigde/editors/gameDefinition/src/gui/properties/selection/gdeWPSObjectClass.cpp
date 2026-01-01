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

#include "gdeWPSObjectClass.h"
#include "gdeWPSObjectClassListener.h"
#include "../gdeWindowProperties.h"
#include "../gdeWPPropertyList.h"
#include "../gdeWPTagList.h"
#include "../../gdeWindowMain.h"
#include "../../../clipboard/gdeClipboardDataProperty.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/component/gdeOCComponentTexture.h"
#include "../../../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/gdeUOCSetName.h"
#include "../../../undosys/objectClass/gdeUOCSetDescription.h"
#include "../../../undosys/objectClass/gdeUOCSetDefaultInheritPropertyPrefix.h"
#include "../../../undosys/objectClass/gdeUOCSetScaleMode.h"
#include "../../../undosys/objectClass/gdeUOCSetHideTags.h"
#include "../../../undosys/objectClass/gdeUOCSetPartialHideTags.h"
#include "../../../undosys/objectClass/gdeUOCSetCategory.h"
#include "../../../undosys/objectClass/gdeUOCSetPropertyValues.h"
#include "../../../undosys/objectClass/gdeUOCPropertyValuesFromSubObjects.h"
#include "../../../undosys/objectClass/gdeUOCToggleIsGhost.h"
#include "../../../undosys/objectClass/gdeUOCToggleCanInstantiate.h"
#include "../../../undosys/objectClass/gdeUOCToggleIsAttachableBehavior.h"
#include "../../../undosys/objectClass/gdeUOCSetInheritSubObjects.h"
#include "../../../undosys/objectClass/property/gdeUOCPropertyAdd.h"
#include "../../../undosys/objectClass/property/gdeUOCPropertyRemove.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetName.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetDescription.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetType.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetMinValue.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetMaxValue.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetDefaultValue.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetOptions.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetPathPatternType.h"
#include "../../../undosys/objectClass/property/gdeUOCPSetIdentifierGroup.h"
#include "../../../undosys/objectClass/property/gdeUOCPToggleIdentifierUsage.h"
#include "../../../undosys/objectClass/property/gdeUOCPCFPAdd.h"
#include "../../../undosys/objectClass/property/gdeUOCPCFPRemove.h"
#include "../../../undosys/objectClass/property/gdeUOCPCFPSetName.h"
#include "../../../undosys/objectClass/property/gdeUOCPCFPSetPattern.h"
#include "../../../undosys/objectClass/property/gdeUOCPCFPSetExtension.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTexPropertyAdd.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTexPropertyRemove.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetName.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetDescription.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetType.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetMinValue.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetMaxValue.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetDefaultValue.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetOptions.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetPathPatternType.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPSetIdentifierGroup.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPToggleIdentifierUsage.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPCFPAdd.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPCFPRemove.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPCFPSetName.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPCFPSetPattern.h"
#include "../../../undosys/objectClass/texproperty/gdeUOCTPCFPSetExtension.h"
#include "../../../undosys/objectClass/inherit/gdeUOCAddInherit.h"
#include "../../../undosys/objectClass/inherit/gdeUOCRemoveInherit.h"
#include "../../../undosys/objectClass/inherit/gdeUOCRemoveAllInherits.h"
#include "../../../undosys/objectClass/inherit/gdeUOCInheritSetName.h"
#include "../../../undosys/objectClass/inherit/gdeUOCInheritSetPropertyPrefix.h"
#include "../../../undosys/objectClass/texture/gdeUOCAddTexture.h"
#include "../../../undosys/objectClass/texture/gdeUOCRemoveTexture.h"
#include "../../../undosys/objectClass/texture/gdeUOCTextureSetColorTint.h"
#include "../../../undosys/objectClass/texture/gdeUOCTextureSetName.h"
#include "../../../undosys/objectClass/texture/gdeUOCTextureSetOffset.h"
#include "../../../undosys/objectClass/texture/gdeUOCTextureSetPathSkin.h"
#include "../../../undosys/objectClass/texture/gdeUOCTextureSetRotation.h"
#include "../../../undosys/objectClass/texture/gdeUOCTextureSetScale.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

namespace{

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(!objectClass){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*comboBox, objectClass));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSObjectClass &panel, const char *text, igdeIcon *icon, const char *description) : 
	igdeAction(text, icon, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(!objectClass){
			return;
		}
		
		igdeUndo::Ref undo(OnActionObjectClass(objectClass));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetObjectClass());
	}
	
	virtual igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(!objectClass){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*textField, objectClass));
		if(undo) {
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass) = 0;
};


class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass){
		if(objectClass->GetName() == textField.GetText()){
			return {};
		}
		return gdeUOCSetName::Ref::New(objectClass, textField.GetText());
	}
};

class cTextDescription : public igdeTextAreaListener{
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cTextDescription> Ref;
	cTextDescription(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(!objectClass || objectClass->GetDescription() == textArea->GetDescription()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCSetDescription::Ref::New(
			objectClass, textArea->GetText()));
	}
};

class cTextDefaultInheritPropertyPrefix : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextDefaultInheritPropertyPrefix> Ref;
	cTextDefaultInheritPropertyPrefix(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass) override{
		if(objectClass->GetDefaultInheritPropertyPrefix() == textField.GetText()){
			return {};
		}
		return gdeUOCSetDefaultInheritPropertyPrefix::Ref::New(objectClass, textField.GetText());
	}
};

class cComboScaleMode : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboScaleMode> Ref;
	cComboScaleMode(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass) override{
		const gdeObjectClass::eScaleModes scaleMode = (gdeObjectClass::eScaleModes)
			(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(objectClass->GetScaleMode() == scaleMode){
			return {};
		}
		return gdeUOCSetScaleMode::Ref::New(objectClass, scaleMode);
	}
};

class cActionIsGhost : public cBaseAction{
public:
	typedef deTObjectReference<cActionIsGhost> Ref;
	cActionIsGhost(gdeWPSObjectClass &panel) : cBaseAction(panel, "Ghost", nullptr,
		"Object is not touched by other objects during placements"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		return gdeUOCToggleIsGhost::Ref::New(objectClass);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionCanInstantiate : public cBaseAction{
public:
	typedef deTObjectReference<cActionCanInstantiate> Ref;
	cActionCanInstantiate(gdeWPSObjectClass &panel) : cBaseAction(panel, "Can Instantiate", nullptr,
		"Object can be instantiated (visible in browser and usable). Disable for classes only inherited from."){ }
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		return gdeUOCToggleCanInstantiate::Ref::New(objectClass);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionIsAttachableBehavior : public cBaseAction{
public:
	typedef deTObjectReference<cActionIsAttachableBehavior> Ref;
	cActionIsAttachableBehavior(gdeWPSObjectClass &panel) : cBaseAction(panel,
		"Attachable Behavior", nullptr, "Object is an attachable behavior"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		return gdeUOCToggleIsAttachableBehavior::Ref::New(objectClass);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionInheritSubObjects : public cBaseAction{
	const int pMask;
public:
	typedef deTObjectReference<cActionInheritSubObjects> Ref;
	cActionInheritSubObjects(gdeWPSObjectClass &panel, int mask, const char *text,
	const char *description) : cBaseAction(panel, text, nullptr, description), pMask(mask){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		int filter = objectClass->GetInheritSubObjects();
		if((filter & pMask) == pMask){
			filter &= ~pMask;
			
		}else{
			filter |= pMask;
		}
		return gdeUOCSetInheritSubObjects::Ref::New(objectClass, filter);
	}
	
	void Update() override{/* empty on purpose!*/}
};


class cEditProperties : public gdeWPPropertyList {
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cEditProperties> Ref;
	cEditProperties(gdeWPSObjectClass &panel) :
	gdeWPPropertyList(panel.GetEnvironment()), pPanel(panel){
		SetClipboard(&panel.GetWindowProperties().GetWindowMain().GetClipboard());
	}
	
	igdeUndo::Ref UndoAdd(gdeProperty *property) override{
		return gdeUOCPropertyAdd::Ref::New(pPanel.GetObjectClass(), property);
	}
	
	igdeUndo::Ref UndoRemove(gdeProperty *property) override{
		return gdeUOCPropertyRemove::Ref::New(pPanel.GetObjectClass(), property);
	}
	
	igdeUndo::Ref UndoPaste(gdeProperty *property) override{
		const gdeUOCPropertyAdd::Ref undo = gdeUOCPropertyAdd::Ref::New(pPanel.GetObjectClass(), property);
		undo->SetShortInfo("Paste property");
		return undo;
	}
	
	virtual igdeUndo::Ref UndoName(gdeProperty *property, const decString &name){
		return gdeUOCPSetName::Ref::New(pPanel.GetObjectClass(), property, name);
	}
	
	virtual igdeUndo::Ref UndoDescription(gdeProperty *property, const decString &description){
		return gdeUOCPSetDescription::Ref::New(pPanel.GetObjectClass(), property, description);
	}
	
	igdeUndo::Ref UndoType(gdeProperty *property, gdeProperty::ePropertyTypes type) override{
		return gdeUOCPSetType::Ref::New(pPanel.GetObjectClass(), property, type);
	}
	
	igdeUndo::Ref UndoMinimumValue(gdeProperty *property, float value) override{
		return gdeUOCPSetMinValue::Ref::New(pPanel.GetObjectClass(), property, value);
	}
	
	igdeUndo::Ref UndoMaximumValue(gdeProperty *property, float value) override{
		return gdeUOCPSetMaxValue::Ref::New(pPanel.GetObjectClass(), property, value);
	}
	
	virtual igdeUndo::Ref UndoDefaultValue(gdeProperty *property, const decString &newValue, const decString &oldValue){
		return gdeUOCPSetDefaultValue::Ref::New(pPanel.GetObjectClass(), property, newValue, oldValue);
	}
	
	virtual igdeUndo::Ref UndoOptions(gdeProperty *property, const decStringList &options){
		return gdeUOCPSetOptions::Ref::New(pPanel.GetObjectClass(), property, options);
	}
	
	igdeUndo::Ref UndoPathPatternType(gdeProperty *property, gdeProperty::ePathPatternTypes type) override{
		return gdeUOCPSetPathPatternType::Ref::New(pPanel.GetObjectClass(), property, type);
	}
	
	virtual igdeUndo::Ref UndoIdentifierGroup(gdeProperty *property, const decString &identifier){
		return gdeUOCPSetIdentifierGroup::Ref::New(pPanel.GetObjectClass(), property, identifier);
	}
	
	igdeUndo::Ref UndoIdentifierUsage(gdeProperty *property) override{
		return gdeUOCPToggleIdentifierUsage::Ref::New(pPanel.GetObjectClass(), property);
	}
	
	igdeUndo::Ref UndoCustomFilePatternAdd(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUOCPCFPAdd::Ref::New(pPanel.GetObjectClass(), property, filePattern);
	}
	
	igdeUndo::Ref UndoCustomFilePatternRemove(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUOCPCFPRemove::Ref::New(pPanel.GetObjectClass(), property, filePattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternName(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name){
		return gdeUOCPCFPSetName::Ref::New(pPanel.GetObjectClass(), property, filePattern, name);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternPattern(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern){
		return gdeUOCPCFPSetPattern::Ref::New(pPanel.GetObjectClass(), property, filePattern, pattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternExtension(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension){
		return gdeUOCPCFPSetExtension::Ref::New(pPanel.GetObjectClass(), property, filePattern, extension);
	}
};

class cEditTextureProperties : public gdeWPPropertyList {
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cEditTextureProperties> Ref;
	cEditTextureProperties(gdeWPSObjectClass &panel) :
	gdeWPPropertyList(panel.GetEnvironment()), pPanel(panel){
		SetClipboard(&panel.GetWindowProperties().GetWindowMain().GetClipboard());
	}
	
	igdeUndo::Ref UndoAdd(gdeProperty *property) override{
		return gdeUOCTexPropertyAdd::Ref::New(pPanel.GetObjectClass(), property);
	}
	
	igdeUndo::Ref UndoRemove(gdeProperty *property) override{
		return gdeUOCTexPropertyRemove::Ref::New(pPanel.GetObjectClass(), property);
	}
	
	igdeUndo::Ref UndoPaste(gdeProperty *property) override{
		const gdeUOCTexPropertyAdd::Ref undo = gdeUOCTexPropertyAdd::Ref::New(pPanel.GetObjectClass(), property);
		undo->SetShortInfo("Paste property");
		return undo;
	}
	
	virtual igdeUndo::Ref UndoName(gdeProperty *property, const decString &name){
		return gdeUOCTPSetName::Ref::New(pPanel.GetObjectClass(), property, name);
	}
	
	virtual igdeUndo::Ref UndoDescription(gdeProperty *property, const decString &description){
		return gdeUOCTPSetDescription::Ref::New(pPanel.GetObjectClass(), property, description);
	}
	
	igdeUndo::Ref UndoType(gdeProperty *property, gdeProperty::ePropertyTypes type) override{
		return gdeUOCTPSetType::Ref::New(pPanel.GetObjectClass(), property, type);
	}
	
	igdeUndo::Ref UndoMinimumValue(gdeProperty *property, float value) override{
		return gdeUOCTPSetMinValue::Ref::New(pPanel.GetObjectClass(), property, value);
	}
	
	igdeUndo::Ref UndoMaximumValue(gdeProperty *property, float value) override{
		return gdeUOCTPSetMaxValue::Ref::New(pPanel.GetObjectClass(), property, value);
	}
	
	virtual igdeUndo::Ref UndoDefaultValue(gdeProperty *property, const decString &newValue, const decString &oldValue){
		return gdeUOCTPSetDefaultValue::Ref::New(pPanel.GetObjectClass(), property, newValue, oldValue);
	}
	
	virtual igdeUndo::Ref UndoOptions(gdeProperty *property, const decStringList &options){
		return gdeUOCTPSetOptions::Ref::New(pPanel.GetObjectClass(), property, options);
	}
	
	igdeUndo::Ref UndoPathPatternType(gdeProperty *property, gdeProperty::ePathPatternTypes type) override{
		return gdeUOCTPSetPathPatternType::Ref::New(pPanel.GetObjectClass(), property, type);
	}
	
	virtual igdeUndo::Ref UndoIdentifierGroup(gdeProperty *property, const decString &identifier){
		return gdeUOCTPSetIdentifierGroup::Ref::New(pPanel.GetObjectClass(), property, identifier);
	}
	
	igdeUndo::Ref UndoIdentifierUsage(gdeProperty *property) override{
		return gdeUOCTPToggleIdentifierUsage::Ref::New(pPanel.GetObjectClass(), property);
	}
	
	igdeUndo::Ref UndoCustomFilePatternAdd(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUOCTPCFPAdd::Ref::New(pPanel.GetObjectClass(), property, filePattern);
	}
	
	igdeUndo::Ref UndoCustomFilePatternRemove(gdeProperty *property, gdeFilePattern *filePattern) override{
		return gdeUOCTPCFPRemove::Ref::New(pPanel.GetObjectClass(), property, filePattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternName(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name){
		return gdeUOCTPCFPSetName::Ref::New(pPanel.GetObjectClass(), property, filePattern, name);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternPattern(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern){
		return gdeUOCTPCFPSetPattern::Ref::New(pPanel.GetObjectClass(), property, filePattern, pattern);
	}
	
	virtual igdeUndo::Ref UndoCustomFilePatternExtension(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension){
		return gdeUOCTPCFPSetExtension::Ref::New(pPanel.GetObjectClass(), property, filePattern, extension);
	}
};



class cListInherits : public igdeListBoxListener{
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cListInherits> Ref;
	cListInherits(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox*){
		pPanel.UpdateInherit();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionInheritAdd());
		helper.MenuCommand(menu, pPanel.GetActionInheritRemove());
		helper.MenuCommand(menu, pPanel.GetActionInheritRemoveAll());
	}
};

class cActionInheritAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionInheritAdd> Ref;
	cActionInheritAdd(gdeWPSObjectClass &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add inherit"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		const decStringSet &classNames = pPanel.GetGameDefinition()->GetClassNameList();
		const int count = classNames.GetCount();
		decStringList proposals;
		int i;
		for(i=0; i<count; i++){
			proposals.Add(classNames.GetAt(i));
		}
		proposals.SortAscending();
		
		decString name("Name");
		if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Inherit", "Name:", name, proposals)){
			return {};
		}
		const gdeObjectClass * const inheritOC = pPanel.GetGameDefinition()->FindObjectClass(name);
		if(inheritOC){
			if(objectClass == inheritOC){
				igdeCommonDialogs::Error(&pPanel, "Add Inherit", "Can not inherit from yourself");
				return {};
			}
			if(inheritOC->InheritsFrom(objectClass)){
				igdeCommonDialogs::Error(&pPanel, "Add Inherit", "Inheritance loop");
				return {};
			}
		}
		
		const gdeOCInherit::Ref objRef(gdeOCInherit::Ref::New(name));
		gdeOCInherit &inherit = (gdeOCInherit&)(deObject&)objRef;
		
		const gdeObjectClass * const ioc = pPanel.GetGameDefinition()->FindObjectClass(name);
		if(ioc){
			inherit.SetPropertyPrefix(ioc->GetDefaultInheritPropertyPrefix());
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCAddInherit::Ref::New(objectClass, &inherit));
		
		pPanel.SelectInherit(&inherit);
		pPanel.UpdateInherit();
		return {};
	}
};

class cActionInheritRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionInheritRemove> Ref;
	cActionInheritRemove(gdeWPSObjectClass &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove inherit"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		gdeOCInherit * const inherit = pPanel.GetInherit();
		return inherit ? gdeUOCRemoveInherit::Ref::New(objectClass, inherit) : gdeUOCRemoveInherit::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetInherit() != nullptr);
	}
};

class cActionInheritRemoveAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionInheritRemoveAll> Ref;
	cActionInheritRemoveAll(gdeWPSObjectClass &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all inherits"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		return objectClass->GetInherits().GetCount() > 0 ? gdeUOCRemoveAllInherits::Ref::New(objectClass) : gdeUOCRemoveAllInherits::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetInherit() != nullptr);
	}
};

class cComboInheritName : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboInheritName> Ref;
	cComboInheritName(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass) override{
		gdeOCInherit * const inherit = pPanel.GetInherit();
		if(!inherit || inherit->GetName() == comboBox.GetText()){
			return {};
		}
		const gdeObjectClass * const inheritOC = pPanel.GetGameDefinition()->FindObjectClass(comboBox.GetText());
		if(inheritOC){
			if(objectClass == inheritOC){
				igdeCommonDialogs::Error(&pPanel, "Set Inherit Name", "Can not inherit from yourself");
				comboBox.SetText(inherit->GetName());
				return {};
			}
			if(inheritOC->InheritsFrom(objectClass)){
				igdeCommonDialogs::Error(&pPanel, "Set Inherit Name", "Inheritance loop");
				comboBox.SetText(inherit->GetName());
				return {};
			}
		}
		return gdeUOCInheritSetName::Ref::New(objectClass, inherit, comboBox.GetText());
	}
};

class cTextInheritPropertyPrefix : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextInheritPropertyPrefix> Ref;
	cTextInheritPropertyPrefix(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass) override{
		gdeOCInherit * const inherit = pPanel.GetInherit();
		if(!inherit || inherit->GetPropertyPrefix() == textField.GetText()){
			return {};
		}
		return gdeUOCInheritSetPropertyPrefix::Ref::New(objectClass, inherit, textField.GetText());
	}
};

class cActionInheritResetPropertyPrefix : public cBaseAction{
public:
	typedef deTObjectReference<cActionInheritResetPropertyPrefix> Ref;
	cActionInheritResetPropertyPrefix(gdeWPSObjectClass &panel) : cBaseAction(panel, "R", nullptr,
		"Reset property prefix to default property prefix of inherited object class"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		gdeOCInherit * const inherit = pPanel.GetInherit();
		if(inherit){
			const gdeObjectClass * const ioc = pPanel.GetGameDefinition()->FindObjectClass(inherit->GetName());
			if(ioc){
				if(inherit->GetPropertyPrefix() != ioc->GetDefaultInheritPropertyPrefix()){
					return gdeUOCInheritSetPropertyPrefix::Ref::New(objectClass, inherit, ioc->GetDefaultInheritPropertyPrefix());
				}
			}
		}
		return {};
	}
};

class cActionInheritJumpToClass : public cBaseAction{
public:
	typedef deTObjectReference<cActionInheritJumpToClass> Ref;
	cActionInheritJumpToClass(gdeWPSObjectClass &panel) : cBaseAction(panel, "",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Class"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass*) override{
		const gdeOCInherit * const inherit = pPanel.GetInherit();
		if(!inherit){
			return {};
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeObjectClass * const ioc = gameDefinition.GetObjectClasses().FindOrDefault([&](gdeObjectClass &oc){
			return oc.GetName() == inherit->GetName();
		});
		if(ioc){
			gameDefinition.SetActiveObjectClass(ioc);
			gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotObjectClass);
		}
		return {};
	}
};


class cComboCategory : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCategory> Ref;
	cComboCategory(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass) override{
		if(objectClass->GetCategory() == comboBox.GetText()){
			return {};
		}
		return gdeUOCSetCategory::Ref::New(objectClass, comboBox.GetText());
	}
};

class cActionJumpToCategory : public cBaseAction{
public:
	typedef deTObjectReference<cActionJumpToCategory> Ref;
	cActionJumpToCategory(gdeWPSObjectClass &panel) : cBaseAction(panel, "",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Category"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeCategory * const category = gameDefinition.GetCategoriesObjectClass()
			.GetWithPath(objectClass->GetCategory());
		if(category){
			gameDefinition.SetActiveCategory(category);
			gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategoryObjectClass);
		}
		return {};
	}
};



class cListHideTags : public gdeWPTagList {
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cListHideTags> Ref;
	cListHideTags(gdeWPSObjectClass &panel, igdeUIHelper &helper) :
	gdeWPTagList(helper, 5, "Hide Tags"), pPanel(panel){}
	
	igdeUndo::Ref UndoSet(const decStringSet &tags) override{
		return gdeUOCSetHideTags::Ref::New(pPanel.GetObjectClass(), tags);
	}
};

class cListPartialHideTags : public gdeWPTagList {
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cListPartialHideTags> Ref;
	cListPartialHideTags(gdeWPSObjectClass &panel, igdeUIHelper &helper) :
	gdeWPTagList(helper, 5, "Partial Hide Tags"), pPanel(panel){}
	
	igdeUndo::Ref UndoSet(const decStringSet &tags) override{
		return gdeUOCSetPartialHideTags::Ref::New(pPanel.GetObjectClass(), tags);
	}
};


class cActionPropertyValueSet : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropertyValueSet> Ref;
	cActionPropertyValueSet(gdeWPSObjectClass &panel) : cBaseAction(panel, "Set",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Set property value"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		const decString &key = pPanel.GetPropertyKey();
		if(!objectClass || key.IsEmpty()){
			return {};
		}
		
		decString value;
		objectClass->NamedPropertyDefaultValue(key, value);
		if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Set Property Value", "Value:", value)){
			return {};
		}
		
		decStringDictionary values(objectClass->GetPropertyValues());
		values.SetAt(key, value);
		return gdeUOCSetPropertyValues::Ref::New(objectClass, values);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetObjectClass() && !pPanel.GetPropertyKey().IsEmpty());
	}
};

class cActionPropertyValueRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropertyValueRemove> Ref;
	cActionPropertyValueRemove(gdeWPSObjectClass &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove property value"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		const char * const key = pPanel.GetPropertyValue();
		if(!key || !objectClass->GetPropertyValues().Has(key)){
			return {};
		}
		
		decStringDictionary values(objectClass->GetPropertyValues());
		values.Remove(key);
		return gdeUOCSetPropertyValues::Ref::New(objectClass, values);
	}
	
	void Update() override{
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		const char * const key = pPanel.GetPropertyValue();
		SetEnabled(objectClass && key && objectClass->GetPropertyValues().Has(key));
	}
};

class cActionPropertyValueClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropertyValueClear> Ref;
	cActionPropertyValueClear(gdeWPSObjectClass &panel) : cBaseAction(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all property values"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		if(objectClass->GetPropertyValues().GetCount() == 0){
			return {};
		}
		return gdeUOCSetPropertyValues::Ref::New(objectClass, decStringDictionary());
	}
	
	void Update() override{
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		SetEnabled(objectClass && objectClass->GetPropertyValues().GetCount() > 0);
	}
};

class cActionPropertyValuesFromSubObjects : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropertyValuesFromSubObjects> Ref;
	cActionPropertyValuesFromSubObjects(gdeWPSObjectClass &panel) : cBaseAction(panel,
		"Set from sub objects", nullptr, "Set property values from sub objects links"){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		return gdeUOCPropertyValuesFromSubObjects::Create(objectClass);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetObjectClass());
	}
};

class cListPropertyValues : public igdeIconListBoxListener{
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cListPropertyValues> Ref;
	cListPropertyValues(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeIconListBox *listBox, int index){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(!objectClass){
			return;
		}
		
		const decString &key = listBox->GetItems().GetAt(index)->GetText();
		decString value(objectClass->GetPropertyValues().GetAtOrDefault(key));
		if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Edit Property Value", "Value:", value)){
			return;
		}
		
		decStringDictionary values(objectClass->GetPropertyValues());
		values.SetAt(key, value);
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCSetPropertyValues::Ref::New(objectClass, values));
	}
	
	virtual void AddContextMenuEntries(igdeIconListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionPropertyValueSet());
		helper.MenuCommand(menu, pPanel.GetActionPropertyValueRemove());
		helper.MenuCommand(menu, pPanel.GetActionPropertyValueClear());
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionPropertyValuesFromSubObjects());
	}
};


// textures

class cBaseComboBoxTextureListener : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cBaseComboBoxTextureListener> Ref;
	cBaseComboBoxTextureListener(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture(comboBox, objectClass, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnChangedTexture(igdeComboBox &comboBox, gdeObjectClass *objectClass,
		gdeOCComponentTexture *texture) = 0;
};

class cBaseActionTexture : public cBaseAction{
public:
	typedef deTObjectReference<cBaseActionTexture> Ref;
	
public:
	cBaseActionTexture(gdeWPSObjectClass &panel, const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnActionTexture(objectClass, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass, gdeOCComponentTexture *texture) = 0;
	
	void Update() override{
		SetEnabled(pPanel.GetTexture());
	}
};

class cBaseTextFieldTextureListener : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cBaseTextFieldTextureListener> Ref;
	cBaseTextFieldTextureListener(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass) override{
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture(textField, objectClass, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass,
		gdeOCComponentTexture *texture) = 0;
};

class cBaseEditVector2TextureListener : public igdeEditVector2Listener{
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVector2TextureListener> Ref;
	cBaseEditVector2TextureListener(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture){
			return;
		}
		
		igdeUndo::Ref undo(OnChangedTexture(editVector2->GetVector2(), pPanel.GetObjectClass(), texture));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass,
		gdeOCComponentTexture *texture) = 0;
};


class cComboTextures : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTextures> Ref;
	cComboTextures(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass) override{
		const igdeListItem * const selection = comboBox.GetSelectedItem();
		objectClass->SetActiveTexture(selection ? (gdeOCComponentTexture*)selection->GetData() : nullptr);
		pPanel.GetGameDefinition()->NotifyOCActiveTextureChanged(objectClass);
		return {};
	}
};

class cActionTextureAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureAdd> Ref;
	
private:
	const decString pTextureName;
	
public:
	cActionTextureAdd(gdeWPSObjectClass &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add texture"){}
	
	cActionTextureAdd(gdeWPSObjectClass &panel, const decString &textureName) :
		cBaseAction(panel, textureName, nullptr, "Add texture"), pTextureName(textureName){}
	
	igdeUndo::Ref OnActionObjectClass(gdeObjectClass *objectClass) override{
		decString name(pTextureName);
		
		if(name.IsEmpty()){
			name = "Texture";
			
			while(true){
				if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Texture", "Name:", name)){
					return {};
				}
				
				if(objectClass->GetTextures().HasMatching([&](const gdeOCComponentTexture &tex){
					return tex.GetName() == name;
				})){
					igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Add Texture", "A texture with this name exists already.");
					
				}else{
					break;
				}
			}
		}
		
		const gdeOCComponentTexture::Ref texture(gdeOCComponentTexture::Ref::New(name));
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCAddTexture::Ref::New(objectClass, texture));
		
		objectClass->SetActiveTexture(texture);
		pPanel.GetGameDefinition()->NotifyOCActiveTextureChanged(objectClass);
		return {};
	}
	
	void Update() override{
		const gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		SetEnabled(objectClass && (pTextureName.IsEmpty() || !objectClass->GetTextures().HasMatching([&](const gdeOCComponentTexture &t){
			return t.GetName() == pTextureName;
		})));
	}
};

class cActionTextureRemove : public cBaseActionTexture{
public:
	typedef deTObjectReference<cActionTextureRemove> Ref;
	cActionTextureRemove(gdeWPSObjectClass &panel) : cBaseActionTexture(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove selected texture"){}
	
	igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override{
		return gdeUOCRemoveTexture::Ref::New(objectClass, texture);
	}
};

class cActionTexturesMenu : public igdeActionContextMenu{
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cActionTexturesMenu> Ref;
	cActionTexturesMenu(gdeWPSObjectClass &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Textures menu"), pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeEnvironment &env = contextMenu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureRemove());
	}
};

class cTextTextureEditName : public cBaseTextFieldTextureListener{
public:
	typedef deTObjectReference<cTextTextureEditName> Ref;
	cTextTextureEditName(gdeWPSObjectClass &panel) : cBaseTextFieldTextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override{
		if(texture->GetName() == textField.GetText()){
			return {};
		}
		
		if(objectClass->GetTextures().HasMatching([&](const gdeOCComponentTexture &t){
			return t.GetName() == textField.GetText();
		})){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Rename texture", "A texture with this name exists already.");
			textField.SetText(texture->GetName());
			return {};
		}
		
		return gdeUOCTextureSetName::Ref::New(objectClass, texture, textField.GetText());
	}
};

class cEditTextureEditPathSkin : public igdeEditPathListener{
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cEditTextureEditPathSkin> Ref;
	cEditTextureEditPathSkin(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture || texture->GetPathSkin() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCTextureSetPathSkin::Ref::New(
			pPanel.GetObjectClass(), texture, editPath->GetPath()));
	}
};

class cEditTextureEditOffset : public cBaseEditVector2TextureListener{
public:
	typedef deTObjectReference<cEditTextureEditOffset> Ref;
	cEditTextureEditOffset(gdeWPSObjectClass &panel) : cBaseEditVector2TextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override{
		if(texture->GetOffset().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCTextureSetOffset::Ref::New(objectClass, texture, vector);
	}
};

class cTextTextureEditRotation : public cBaseTextFieldTextureListener{
public:
	typedef deTObjectReference<cTextTextureEditRotation> Ref;
	cTextTextureEditRotation(gdeWPSObjectClass &panel) : cBaseTextFieldTextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override{
		const float value = textField.GetFloat();
		if(fabsf(texture->GetRotation() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCTextureSetRotation::Ref::New(objectClass, texture, value);
	}
};

class cEditTextureEditScale : public cBaseEditVector2TextureListener{
public:
	typedef deTObjectReference<cEditTextureEditScale> Ref;
	cEditTextureEditScale(gdeWPSObjectClass &panel) : cBaseEditVector2TextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override{
		if(texture->GetScale().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCTextureSetScale::Ref::New(objectClass, texture, vector);
	}
};

class cColorTextureTint : public igdeColorBoxListener{
	gdeWPSObjectClass &pPanel;
	
public:
	typedef deTObjectReference<cColorTextureTint> Ref;
	cColorTextureTint(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture || texture->GetColorTint().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCTextureSetColorTint::Ref::New(
			pPanel.GetObjectClass(), texture, colorBox->GetColor()));
	}
};

}



// Class gdeWPSObjectClass
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSObjectClass::gdeWPSObjectClass(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, form, frameLine;
	
	pListener = gdeWPSObjectClassListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// actions
	pActionInheritAdd = cActionInheritAdd::Ref::New(*this);
	pActionInheritRemove = cActionInheritRemove::Ref::New(*this);
	pActionInheritRemoveAll = cActionInheritRemoveAll::Ref::New(*this);
	
	pActionPropertyValueSet = cActionPropertyValueSet::Ref::New(*this);
	pActionPropertyValueRemove = cActionPropertyValueRemove::Ref::New(*this);
	pActionPropertyValueClear = cActionPropertyValueClear::Ref::New(*this);
	pActionPropertyValuesFromSubObjects = cActionPropertyValuesFromSubObjects::Ref::New(*this);
	
	pActionTexturesMenu = cActionTexturesMenu::Ref::New(*this);
	pActionTextureAdd = cActionTextureAdd::Ref::New(*this);
	pActionTextureRemove = cActionTextureRemove::Ref::New(*this);
	
	
	// object class
	helper.GroupBox(content, groupBox, "Object Class:");
	helper.EditString(groupBox, "Name:", "Object class name", pEditName, cTextName::Ref::New(*this));
	helper.EditString(groupBox, "Description:", "Object class description",
		pEditDescription, 15, 5, cTextDescription::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Scale Mode:", "Object scale mode", pCBScaleMode, cComboScaleMode::Ref::New(*this));
	pCBScaleMode->AddItem("Fixed", nullptr, (void*)(intptr_t)gdeObjectClass::esmFixed);
	pCBScaleMode->AddItem("Uniform", nullptr, (void*)(intptr_t)gdeObjectClass::esmUniform);
	pCBScaleMode->AddItem("Free", nullptr, (void*)(intptr_t)gdeObjectClass::esmFree);
	
	helper.EditString(groupBox, "Def Inherit Prop Pref:",
		"Default property prefix to use if this object class is inherited from",
		pEditDefaultInheritPropertyPrefix, cTextDefaultInheritPropertyPrefix::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkIsGhost, cActionIsGhost::Ref::New(*this));
	helper.CheckBox(groupBox, pChkCanInstantiate, cActionCanInstantiate::Ref::New(*this));
	helper.CheckBox(groupBox, pChkIsAttachableBehavior, cActionIsAttachableBehavior::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Category: ", "Category", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Category", pCBCategory, cComboCategory::Ref::New(*this));
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToCategory, cActionJumpToCategory::Ref::New(*this));
	
	
	// inherits
	helper.GroupBoxFlow(content, groupBox, "Inherits:");
	
	helper.ListBox(groupBox, 5, "Inherited object classes", pListInherits, cListInherits::Ref::New(*this));
	pListInherits->SetDefaultSorter();
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.FormLineStretchFirst(form, "Name:", "Name of class to inherit", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Name of class to inherit",
		pInheritCBClass, cComboInheritName::Ref::New(*this));
	pInheritCBClass->SetDefaultSorter();
	pInheritCBClass->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToInheritClass, cActionInheritJumpToClass::Ref::New(*this));
	
	helper.FormLineStretchFirst(form, "Property Prefix:",
		"Prefix to apply to inherited property names", frameLine);
	helper.EditString(frameLine, "Prefix to apply to inherited property names",
		pInheritEditPropertyPrefix, cTextInheritPropertyPrefix::Ref::New(*this));
	helper.Button(frameLine, pBtnInheritPropertyPrefixReset, cActionInheritResetPropertyPrefix::Ref::New(*this));
	
	
	// inherit sub objects
	helper.GroupBoxFlow(content, groupBox, "Inherit Sub-Objects:", false, true);
	helper.CheckBox(groupBox, pChkInheritSOBillboards, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoBillboards, "Billboards", "Inherit billboards"));
	helper.CheckBox(groupBox, pChkInheritSOComponents, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoComponents, "Components", "Inherit components"));
	helper.CheckBox(groupBox, pChkInheritSOLights, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoLights, "Lights", "Inherit lights"));
	helper.CheckBox(groupBox, pChkInheritSOSnapPoints, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoSnapPoints, "Snap Points", "Inherit snap points"));
	helper.CheckBox(groupBox, pChkInheritSOParticleEmitters, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoParticleEmitters, "Particle Emitters", "Inherit particle emitters"));
	helper.CheckBox(groupBox, pChkInheritSOForceFields, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoForceFields, "Force Fields", "Inherit force fields"));
	helper.CheckBox(groupBox, pChkInheritSOEnvMapProbes, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoEnvMapProbes, "Environment Map Probes", "Inherit environment map probes"));
	helper.CheckBox(groupBox, pChkInheritSOSpeakers, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoSpeakers, "Speakers", "Inherit speakers"));
	helper.CheckBox(groupBox, pChkInheritSONavigationSpaces, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoNavigationSpaces, "Navigation Spaces", "Inherit navigation spaces"));
	helper.CheckBox(groupBox, pChkInheritSONavigationBlockers, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoNavigationBlockers, "Navigation Blockers", "Inherit navigation blockers"));
	helper.CheckBox(groupBox, pChkInheritSOWorlds, cActionInheritSubObjects::Ref::New(*this,
		igdeGDClass::efsoWorlds, "Worlds", "Inherit worlds"));
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "Properties:");
	pEditProperties = cEditProperties::Ref::New(*this);
	groupBox->AddChild(pEditProperties);
	
	
	// property values
	helper.GroupBoxFlow(content, groupBox, "Property Values:", true);
	
	frameLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	helper.ComboBox(frameLine, true, "Property value to set", pCBPropertyValuesKeys, {});
	pCBPropertyValuesKeys->SetDefaultSorter();
	helper.Button(frameLine, pBtnPropertyValueSet, pActionPropertyValueSet);
	groupBox->AddChild(frameLine);
	
	const igdeUIHelper::sColumnHeader headersPropertyValues[2] = {
		igdeUIHelper::sColumnHeader("Key", nullptr, igdeApplication::app().DisplayScaled(150)),
		igdeUIHelper::sColumnHeader("Value", nullptr, igdeApplication::app().DisplayScaled(200))
	};
	helper.IconListBox(groupBox, pListPropertyValues,
		igdeApplication::app().DisplayScaled(decPoint(100, 120)),
		headersPropertyValues, 2, "Property values", cListPropertyValues::Ref::New(*this));
	pListPropertyValues->SetDefaultSorter();
	
	
	// texture properties
	helper.GroupBoxFlow(content, groupBox, "Texture Properties:", false, true);
	pEditTextureProperties = cEditTextureProperties::Ref::New(*this);
	groupBox->AddChild(pEditTextureProperties);
	
	
	// tagging
	helper.GroupBoxFlow(content, groupBox, "Hide Tags:", false, true);
	pListHideTags = cListHideTags::Ref::New(*this, helper);
	groupBox->AddChild(pListHideTags);
	
	helper.GroupBoxFlow(content, groupBox, "Partial Hide Tags:", false, true);
	pListPartialHideTags = cListPartialHideTags::Ref::New(*this, helper);
	groupBox->AddChild(pListPartialHideTags);
	
	
	// textures
	helper.GroupBox(content, groupBox, "Textures:");
	
	helper.FormLineStretchFirst(groupBox, "Texture:", "Texture to edit", frameLine);
	helper.ComboBox(frameLine, "Texture to edit", pCBTextures, cComboTextures::Ref::New(*this));
	pCBTextures->SetDefaultSorter();
	helper.Button(frameLine, pBtnTextures, pActionTexturesMenu);
	pActionTexturesMenu->SetWidget(pBtnTextures);
	
	helper.EditString(groupBox, "Name:", "Name of texture", pTextureEditName, cTextTextureEditName::Ref::New(*this));
	helper.EditPath(groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pTextureEditPathSkin, cEditTextureEditPathSkin::Ref::New(*this));
	helper.EditVector2(groupBox, "Offset:", "Texture coordinate offset",
		pTextureEditOffset, cEditTextureEditOffset::Ref::New(*this));
	helper.EditFloat(groupBox, "Rotation:",
		"Texture coordinate rotation around texture center in degrees",
		pTextureEditRotation, cTextTextureEditRotation::Ref::New(*this));
	helper.EditVector2(groupBox, "Scale:", "Texture coordinate scaling",
		pTextureEditScale, cEditTextureEditScale::Ref::New(*this));
	helper.ColorBox(groupBox, "Tint:", "Texture tint color",
		pTextureClrTint, cColorTextureTint::Ref::New(*this));
}

gdeWPSObjectClass::~gdeWPSObjectClass(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSObjectClass::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	pListHideTags->SetUndoSystem(nullptr);
	pListHideTags->SetTagList(nullptr);
	
	pListPartialHideTags->SetUndoSystem(nullptr);
	pListPartialHideTags->SetTagList(nullptr);
	
	pEditProperties->SetPropertyList(nullptr);
	pEditProperties->SetGameDefinition(nullptr);
	
	pEditTextureProperties->SetPropertyList(nullptr);
	pEditTextureProperties->SetGameDefinition(nullptr);
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		pListHideTags->SetUndoSystem(gameDefinition->GetUndoSystem());
		pListPartialHideTags->SetUndoSystem(gameDefinition->GetUndoSystem());
		pEditProperties->SetGameDefinition(gameDefinition);
		pEditTextureProperties->SetGameDefinition(gameDefinition);
	}
	
	UpdateCategoryList();
	UpdateClassLists();
	UpdateObjectClass();
	UpdateIdentifierLists();
	UpdateHideTags();
	UpdateUsedTagsList();
	UpdatePartialHideTags();
}



gdeObjectClass *gdeWPSObjectClass::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeProperty *gdeWPSObjectClass::GetProperty() const{
	return pEditProperties->GetProperty();
}

gdeProperty *gdeWPSObjectClass::GetTextureProperty() const{
	return pEditTextureProperties->GetProperty();
}

gdeOCInherit *gdeWPSObjectClass::GetInherit() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	if(!objectClass){
		return nullptr;
	}
	
	const igdeListItem * const selection = pListInherits->GetSelectedItem();
	return selection ? (gdeOCInherit*)selection->GetData() : nullptr;
}

const decString &gdeWPSObjectClass::GetPropertyKey() const{
	return pCBPropertyValuesKeys->GetText();
}

const char *gdeWPSObjectClass::GetPropertyValue() const{
	const igdeListItem * const selection = pListPropertyValues->GetSelectedItem();
	return selection ? selection->GetText().GetString() : nullptr;
}

gdeOCComponentTexture *gdeWPSObjectClass::GetTexture() const{
	gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? objectClass->GetActiveTexture().Pointer() : nullptr;
}



void gdeWPSObjectClass::UpdateUsedTagsList(){
	if(pGameDefinition){
		pListHideTags->UpdateUsedTagList(pGameDefinition->GetUsedTagsObjectClass());
		pListPartialHideTags->UpdateUsedTagList(pGameDefinition->GetUsedTagsObjectClass());
		
	}else{
		pListHideTags->UpdateUsedTagList(decStringSet());
		pListPartialHideTags->UpdateUsedTagList(decStringSet());
	}
}

void gdeWPSObjectClass::UpdateCategoryList(){
	const decString selection(pCBCategory->GetText());
	
	pCBCategory->RemoveAllItems();
	
	if(pGameDefinition){
		const gdeCategoryList &categories = pGameDefinition->GetCategoriesObjectClass();
		if(categories.GetCount() > 0){
			pUpdateCategoryList(categories, "");
		}
		
		pCBCategory->SortItems();
		pCBCategory->StoreFilterItems();
	}
	
	pCBCategory->SetText(selection);
	pCBCategory->SetInvalidValue(!pCBCategory->GetText().IsEmpty() && !pCBCategory->GetSelectedItem());
}

void gdeWPSObjectClass::UpdateClassLists(){
	const decString selectedInheritClass(pInheritCBClass->GetText());
	
	pInheritCBClass->RemoveAllItems();
	
	if(pGameDefinition){
		const decStringSet &set = pGameDefinition->GetClassNameList();
		const int count = set.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			const decString &name = set.GetAt(i);
			pInheritCBClass->AddItem(name);
		}
		
		pInheritCBClass->SortItems();
		pInheritCBClass->StoreFilterItems();
	}
	
	pInheritCBClass->SetText(selectedInheritClass);
	pInheritCBClass->SetInvalidValue(!pInheritCBClass->GetText().IsEmpty()
		&& !pInheritCBClass->GetSelectedItem());
}

void gdeWPSObjectClass::UpdateIdentifierLists(){
	pEditProperties->UpdatePropertyIdentifierList();
	pEditTextureProperties->UpdatePropertyIdentifierList();
}



void gdeWPSObjectClass::UpdateObjectClass(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	if(objectClass){
		const int iso = objectClass->GetInheritSubObjects();
		
		pEditName->SetText(objectClass->GetName());
		pEditDescription->SetText(objectClass->GetDescription());
		pEditDefaultInheritPropertyPrefix->SetText(objectClass->GetDefaultInheritPropertyPrefix());
		pCBScaleMode->SetSelectionWithData((void*)(intptr_t)objectClass->GetScaleMode());
		pChkIsGhost->SetChecked(objectClass->GetIsGhost());
		pChkCanInstantiate->SetChecked(objectClass->GetCanInstantiate());
		pChkIsAttachableBehavior->SetChecked(objectClass->GetIsAttachableBehavior());
		pChkInheritSOBillboards->SetChecked((iso & igdeGDClass::efsoBillboards) != 0);
		pChkInheritSOComponents->SetChecked((iso & igdeGDClass::efsoComponents) != 0);
		pChkInheritSOLights->SetChecked((iso & igdeGDClass::efsoLights) != 0);
		pChkInheritSOSnapPoints->SetChecked((iso & igdeGDClass::efsoSnapPoints) != 0);
		pChkInheritSOParticleEmitters->SetChecked((iso & igdeGDClass::efsoParticleEmitters) != 0);
		pChkInheritSOForceFields->SetChecked((iso & igdeGDClass::efsoForceFields) != 0);
		pChkInheritSOEnvMapProbes->SetChecked((iso & igdeGDClass::efsoEnvMapProbes) != 0);
		pChkInheritSOSpeakers->SetChecked((iso & igdeGDClass::efsoSpeakers) != 0);
		pChkInheritSONavigationSpaces->SetChecked((iso & igdeGDClass::efsoNavigationSpaces) != 0);
		pChkInheritSONavigationBlockers->SetChecked((iso & igdeGDClass::efsoNavigationBlockers) != 0);
		pChkInheritSOWorlds->SetChecked((iso & igdeGDClass::efsoWorlds) != 0);
		pCBCategory->SetText(objectClass->GetCategory());
		pCBCategory->SetInvalidValue(!pCBCategory->GetText().IsEmpty()
			&& !pCBCategory->GetSelectedItem());
		pEditProperties->SetPropertyList(&objectClass->GetProperties());
		pEditTextureProperties->SetPropertyList(&objectClass->GetTextureProperties());
		pListHideTags->SetTagList(&objectClass->GetHideTags());
		pListPartialHideTags->SetTagList(&objectClass->GetPartialHideTags());
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pEditDefaultInheritPropertyPrefix->ClearText();
		pCBScaleMode->SetSelectionWithData((void*)(intptr_t)gdeObjectClass::esmFixed);
		pChkIsGhost->SetChecked(false);
		pChkCanInstantiate->SetChecked(true);
		pChkIsAttachableBehavior->SetChecked(false);
		pChkInheritSOBillboards->SetChecked(true);
		pChkInheritSOComponents->SetChecked(true);
		pChkInheritSOLights->SetChecked(true);
		pChkInheritSOSnapPoints->SetChecked(true);
		pChkInheritSOParticleEmitters->SetChecked(true);
		pChkInheritSOForceFields->SetChecked(true);
		pChkInheritSOEnvMapProbes->SetChecked(true);
		pChkInheritSOSpeakers->SetChecked(true);
		pChkInheritSONavigationSpaces->SetChecked(true);
		pChkInheritSONavigationBlockers->SetChecked(true);
		pChkInheritSOWorlds->SetChecked(true);
		pCBCategory->ClearText();
		pCBCategory->SetInvalidValue(false);
		pEditProperties->SetPropertyList(nullptr);
		pEditTextureProperties->SetPropertyList(nullptr);
		pListHideTags->SetTagList(nullptr);
		pListPartialHideTags->SetTagList(nullptr);
	}
	
	const bool enabled = objectClass != nullptr;
	pEditName->SetEnabled(enabled);
	pEditDescription->SetEnabled(enabled);
	pEditDefaultInheritPropertyPrefix->SetEnabled(enabled);
	pCBScaleMode->SetEnabled(enabled);
	pChkIsGhost->SetEnabled(enabled);
	pChkCanInstantiate->SetEnabled(enabled);
	pChkIsAttachableBehavior->SetEnabled(enabled);
	pChkInheritSOBillboards->SetEnabled(enabled);
	pChkInheritSOComponents->SetEnabled(enabled);
	pChkInheritSOLights->SetEnabled(enabled);
	pChkInheritSOSnapPoints->SetEnabled(enabled);
	pChkInheritSOParticleEmitters->SetEnabled(enabled);
	pChkInheritSOForceFields->SetEnabled(enabled);
	pChkInheritSOEnvMapProbes->SetEnabled(enabled);
	pChkInheritSOSpeakers->SetEnabled(enabled);
	pChkInheritSONavigationSpaces->SetEnabled(enabled);
	pChkInheritSONavigationBlockers->SetEnabled(enabled);
	pChkInheritSOWorlds->SetEnabled(enabled);
	pCBCategory->SetEnabled(enabled);
	pActionPropertyValueSet->Update();
	
	UpdateProperties();
	UpdatePropertyValueKeys();
	UpdatePropertyValues();
	UpdateTextureProperties();
	UpdateInherits();
	UpdateHideTags();
	UpdatePartialHideTags();
	UpdateTextureList();
}

void gdeWPSObjectClass::UpdateProperties(){
	pEditProperties->UpdateList();
}

void gdeWPSObjectClass::UpdateProperty(){
	pEditProperties->UpdateProperty();
}

void gdeWPSObjectClass::UpdatePropertyValues(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	decString selection(pListPropertyValues->GetSelectedItem()
		? pListPropertyValues->GetSelectedItem()->GetText() : decString());
	
	pListPropertyValues->RemoveAllItems();
	
	if(objectClass){
		const decStringList keys(objectClass->GetPropertyValues().GetKeys());
		const int count = keys.GetCount();
		decStringList details;
		int i;
		
		for(i=0; i<count; i++){
			const decString &key = keys.GetAt(i);
			details.RemoveAll();
			details.Add(objectClass->GetPropertyValues().GetAt(key));
			pListPropertyValues->AddItem(key, details);
		}
		
		pListPropertyValues->SortItems();
	}
	
	pListPropertyValues->SetSelection(pListPropertyValues->IndexOfItem(selection));
}

void gdeWPSObjectClass::UpdatePropertyValueKeys(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if(objectClass){
		objectClass->AddPropertyNamesTo(properties, true);
	}
	
	const decString selection(pCBPropertyValuesKeys->GetText());
	pCBPropertyValuesKeys->RemoveAllItems();
	
	const int count = properties.GetCount();
	for(i=0; i<count; i++){
		pCBPropertyValuesKeys->AddItem(properties.GetAt(i));
	}
	
	pCBPropertyValuesKeys->SortItems();
	pCBPropertyValuesKeys->SetText(selection);
}

void gdeWPSObjectClass::UpdateTextureProperties(){
	pEditTextureProperties->UpdateList();
}

void gdeWPSObjectClass::UpdateTextureProperty(){
	pEditTextureProperties->UpdateProperty();
}

void gdeWPSObjectClass::UpdateInherits(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	gdeOCInherit * const activeInherit = GetInherit();
	
	pListInherits->RemoveAllItems();
	
	if(objectClass){
		const gdeOCInherit::List &list = objectClass->GetInherits();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			gdeOCInherit * const inherit = list.GetAt(i);
			text.Format("%s: '%s'", inherit->GetName().GetString(), inherit->GetPropertyPrefix().GetString());
			pListInherits->AddItem(text, nullptr, inherit);
		}
		pListInherits->SortItems();
	}
	
	pListInherits->SetSelectionWithData(activeInherit);
	if(!pListInherits->GetSelectedItem() && pListInherits->GetItems().IsNotEmpty()){
		pListInherits->SetSelection(0);
	}
	
	UpdateInherit();
}

void gdeWPSObjectClass::UpdateInherit(){
	const gdeOCInherit * const inherit = GetInherit();
	
	if(inherit){
		pInheritCBClass->SetText(inherit->GetName());
		pInheritEditPropertyPrefix->SetText(inherit->GetPropertyPrefix());
		
	}else{
		pInheritCBClass->ClearText();
		pInheritEditPropertyPrefix->ClearText();
	}
	
	pInheritCBClass->SetInvalidValue(!pInheritCBClass->GetText().IsEmpty()
		&& !pInheritCBClass->GetSelectedItem());
	
	pInheritCBClass->SetEnabled(inherit != nullptr);
	pInheritEditPropertyPrefix->SetEnabled(inherit != nullptr);
}

void gdeWPSObjectClass::SelectInherit(gdeOCInherit *inherit){
	pListInherits->SetSelectionWithData(inherit);
}

void gdeWPSObjectClass::UpdateHideTags(){
	pListHideTags->UpdateList();
}

void gdeWPSObjectClass::UpdatePartialHideTags(){
	pListPartialHideTags->UpdateList();
}

void gdeWPSObjectClass::UpdateTextureList(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	
	pCBTextures->RemoveAllItems();
	
	if(objectClass){
		const gdeOCComponentTexture::List &textures = objectClass->GetTextures();
		const int count = textures.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			gdeOCComponentTexture * const texture = textures.GetAt(i);
			pCBTextures->AddItem(texture->GetName(), nullptr, texture);
		}
	}
	
	pCBTextures->SortItems();
	
	SelectActiveTexture();
	UpdateTexture();
}

void gdeWPSObjectClass::SelectActiveTexture(){
	pCBTextures->SetSelectionWithData(GetTexture());
}

void gdeWPSObjectClass::UpdateTexture(){
	const gdeOCComponentTexture * const texture = GetTexture();
	
	if(texture){
		pTextureEditName->SetText(texture->GetName());
		pTextureEditPathSkin->SetPath(texture->GetPathSkin());
		pTextureEditOffset->SetVector2(texture->GetOffset());
		pTextureEditRotation->SetFloat(texture->GetRotation());
		pTextureEditScale->SetVector2(texture->GetScale());
		pTextureClrTint->SetColor(texture->GetColorTint());
		
	}else{
		pTextureEditName->ClearText();
		pTextureEditPathSkin->ClearPath();
		pTextureEditOffset->SetVector2(decVector2());
		pTextureEditRotation->ClearText();
		pTextureEditScale->SetVector2(decVector2());
		pTextureClrTint->SetColor(decColor(1.0f, 1.0f, 1.0f));
	}
	
	const bool enabled = texture;
	
	pTextureEditName->SetEnabled(enabled);
	pTextureEditPathSkin->SetEnabled(enabled);
	pTextureEditOffset->SetEnabled(enabled);
	pTextureEditRotation->SetEnabled(enabled);
	pTextureEditScale->SetEnabled(enabled);
	pTextureClrTint->SetEnabled(enabled);
}



// Private Functions
//////////////////////

void gdeWPSObjectClass::pUpdateCategoryList(const gdeCategoryList &list, const char *prefix){
	const int count = list.GetCount();
	decString text;
	int i;
	
	for(i=0; i<count; i++){
		const gdeCategory &category = *list.GetAt(i);
		text.Format("%s%s", prefix, category.GetName().GetString());
		pCBCategory->AddItem(text);
		if(category.GetCategories().GetCount() > 0){
			text.AppendCharacter('/');
			pUpdateCategoryList(category.GetCategories(), text);
		}
	}
}
