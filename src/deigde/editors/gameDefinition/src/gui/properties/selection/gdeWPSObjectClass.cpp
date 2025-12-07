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

class cBaseComboBoxListener : public igdeComboBoxListener {
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	cBaseComboBoxListener(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(! objectClass){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(*comboBox, objectClass)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass) = 0;
};

class cBaseAction : public igdeAction {
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	cBaseAction(gdeWPSObjectClass &panel, const char *text, igdeIcon *icon, const char *description) : 
	igdeAction(text, icon, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(! objectClass){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnActionObjectClass(objectClass)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetObjectClass());
	}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener {
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	cBaseTextFieldListener(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(! objectClass){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(*textField, objectClass)));
		if(undo) {
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass) = 0;
};


class cTextName : public cBaseTextFieldListener {
public:
	cTextName(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass){
		if(objectClass->GetName() == textField.GetText()){
			return NULL;
		}
		return new gdeUOCSetName(objectClass, textField.GetText());
	}
};

class cTextDescription : public igdeTextAreaListener {
	gdeWPSObjectClass &pPanel;
	
public:
	cTextDescription(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(! objectClass || objectClass->GetDescription() == textArea->GetDescription()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCSetDescription::Ref::NewWith(
			objectClass, textArea->GetText()));
	}
};

class cTextDefaultInheritPropertyPrefix : public cBaseTextFieldListener {
public:
	cTextDefaultInheritPropertyPrefix(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass){
		if(objectClass->GetDefaultInheritPropertyPrefix() == textField.GetText()){
			return NULL;
		}
		return new gdeUOCSetDefaultInheritPropertyPrefix(objectClass, textField.GetText());
	}
};

class cComboScaleMode : public cBaseComboBoxListener {
public:
	cComboScaleMode(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass){
		const gdeObjectClass::eScaleModes scaleMode = (gdeObjectClass::eScaleModes)
			(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(objectClass->GetScaleMode() == scaleMode){
			return NULL;
		}
		return new gdeUOCSetScaleMode(objectClass, scaleMode);
	}
};

class cActionIsGhost : public cBaseAction {
public:
	cActionIsGhost(gdeWPSObjectClass &panel) : cBaseAction(panel, "Ghost", NULL,
		"Object is not touched by other objects during placements"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		return new gdeUOCToggleIsGhost(objectClass);
	}
	
	virtual void Update(){/* empty on purpose! */}
};

class cActionCanInstantiate : public cBaseAction {
public:
	cActionCanInstantiate(gdeWPSObjectClass &panel) : cBaseAction(panel, "Can Instantiate", NULL,
		"Object can be instantiated (visible in browser and usable). Disable for classes only inherited from."){ }
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		return new gdeUOCToggleCanInstantiate(objectClass);
	}
	
	virtual void Update(){/* empty on purpose! */}
};

class cActionIsAttachableBehavior : public cBaseAction {
public:
	cActionIsAttachableBehavior(gdeWPSObjectClass &panel) : cBaseAction(panel,
		"Attachable Behavior", nullptr, "Object is an attachable behavior"){}
	
	igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass) override{
		return new gdeUOCToggleIsAttachableBehavior(objectClass);
	}
	
	void Update() override{/* empty on purpose! */}
};

class cActionInheritSubObjects : public cBaseAction {
	const int pMask;
public:
	cActionInheritSubObjects(gdeWPSObjectClass &panel, int mask, const char *text,
	const char *description) : cBaseAction(panel, text, nullptr, description), pMask(mask){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		int filter = objectClass->GetInheritSubObjects();
		if((filter & pMask) == pMask){
			filter &= ~pMask;
			
		}else{
			filter |= pMask;
		}
		return new gdeUOCSetInheritSubObjects(objectClass, filter);
	}
	
	virtual void Update(){/* empty on purpose! */}
};


class cEditProperties : public gdeWPPropertyList {
	gdeWPSObjectClass &pPanel;
	
public:
	cEditProperties(gdeWPSObjectClass &panel) :
	gdeWPPropertyList(panel.GetEnvironment()), pPanel(panel){
		SetClipboard(&panel.GetWindowProperties().GetWindowMain().GetClipboard());
	}
	
	virtual igdeUndo *UndoAdd(gdeProperty *property){
		return new gdeUOCPropertyAdd(pPanel.GetObjectClass(), property);
	}
	
	virtual igdeUndo *UndoRemove(gdeProperty *property){
		return new gdeUOCPropertyRemove(pPanel.GetObjectClass(), property);
	}
	
	virtual igdeUndo *UndoPaste(gdeProperty *property){
		gdeUOCPropertyAdd * const undo = new gdeUOCPropertyAdd(pPanel.GetObjectClass(), property);
		undo->SetShortInfo("Paste property");
		return undo;
	}
	
	virtual igdeUndo *UndoName(gdeProperty *property, const decString &name){
		return new gdeUOCPSetName(pPanel.GetObjectClass(), property, name);
	}
	
	virtual igdeUndo *UndoDescription(gdeProperty *property, const decString &description){
		return new gdeUOCPSetDescription(pPanel.GetObjectClass(), property, description);
	}
	
	virtual igdeUndo *UndoType(gdeProperty *property, gdeProperty::ePropertyTypes type){
		return new gdeUOCPSetType(pPanel.GetObjectClass(), property, type);
	}
	
	virtual igdeUndo *UndoMinimumValue(gdeProperty *property, float value){
		return new gdeUOCPSetMinValue(pPanel.GetObjectClass(), property, value);
	}
	
	virtual igdeUndo *UndoMaximumValue(gdeProperty *property, float value){
		return new gdeUOCPSetMaxValue(pPanel.GetObjectClass(), property, value);
	}
	
	virtual igdeUndo *UndoDefaultValue(gdeProperty *property, const decString &newValue, const decString &oldValue){
		return new gdeUOCPSetDefaultValue(pPanel.GetObjectClass(), property, newValue, oldValue);
	}
	
	virtual igdeUndo *UndoOptions(gdeProperty *property, const decStringList &options){
		return new gdeUOCPSetOptions(pPanel.GetObjectClass(), property, options);
	}
	
	virtual igdeUndo *UndoPathPatternType(gdeProperty *property, gdeProperty::ePathPatternTypes type){
		return new gdeUOCPSetPathPatternType(pPanel.GetObjectClass(), property, type);
	}
	
	virtual igdeUndo *UndoIdentifierGroup(gdeProperty *property, const decString &identifier){
		return new gdeUOCPSetIdentifierGroup(pPanel.GetObjectClass(), property, identifier);
	}
	
	virtual igdeUndo *UndoIdentifierUsage(gdeProperty *property){
		return new gdeUOCPToggleIdentifierUsage(pPanel.GetObjectClass(), property);
	}
	
	virtual igdeUndo *UndoCustomFilePatternAdd(gdeProperty *property, gdeFilePattern *filePattern){
		return new gdeUOCPCFPAdd(pPanel.GetObjectClass(), property, filePattern);
	}
	
	virtual igdeUndo *UndoCustomFilePatternRemove(gdeProperty *property, gdeFilePattern *filePattern){
		return new gdeUOCPCFPRemove(pPanel.GetObjectClass(), property, filePattern);
	}
	
	virtual igdeUndo *UndoCustomFilePatternName(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name){
		return new gdeUOCPCFPSetName(pPanel.GetObjectClass(), property, filePattern, name);
	}
	
	virtual igdeUndo *UndoCustomFilePatternPattern(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern){
		return new gdeUOCPCFPSetPattern(pPanel.GetObjectClass(), property, filePattern, pattern);
	}
	
	virtual igdeUndo *UndoCustomFilePatternExtension(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension){
		return new gdeUOCPCFPSetExtension(pPanel.GetObjectClass(), property, filePattern, extension);
	}
};

class cEditTextureProperties : public gdeWPPropertyList {
	gdeWPSObjectClass &pPanel;
	
public:
	cEditTextureProperties(gdeWPSObjectClass &panel) :
	gdeWPPropertyList(panel.GetEnvironment()), pPanel(panel){
		SetClipboard(&panel.GetWindowProperties().GetWindowMain().GetClipboard());
	}
	
	virtual igdeUndo *UndoAdd(gdeProperty *property){
		return new gdeUOCTexPropertyAdd(pPanel.GetObjectClass(), property);
	}
	
	virtual igdeUndo *UndoRemove(gdeProperty *property){
		return new gdeUOCTexPropertyRemove(pPanel.GetObjectClass(), property);
	}
	
	virtual igdeUndo *UndoPaste(gdeProperty *property){
		gdeUOCTexPropertyAdd * const undo = new gdeUOCTexPropertyAdd(pPanel.GetObjectClass(), property);
		undo->SetShortInfo("Paste property");
		return undo;
	}
	
	virtual igdeUndo *UndoName(gdeProperty *property, const decString &name){
		return new gdeUOCTPSetName(pPanel.GetObjectClass(), property, name);
	}
	
	virtual igdeUndo *UndoDescription(gdeProperty *property, const decString &description){
		return new gdeUOCTPSetDescription(pPanel.GetObjectClass(), property, description);
	}
	
	virtual igdeUndo *UndoType(gdeProperty *property, gdeProperty::ePropertyTypes type){
		return new gdeUOCTPSetType(pPanel.GetObjectClass(), property, type);
	}
	
	virtual igdeUndo *UndoMinimumValue(gdeProperty *property, float value){
		return new gdeUOCTPSetMinValue(pPanel.GetObjectClass(), property, value);
	}
	
	virtual igdeUndo *UndoMaximumValue(gdeProperty *property, float value){
		return new gdeUOCTPSetMaxValue(pPanel.GetObjectClass(), property, value);
	}
	
	virtual igdeUndo *UndoDefaultValue(gdeProperty *property, const decString &newValue, const decString &oldValue){
		return new gdeUOCTPSetDefaultValue(pPanel.GetObjectClass(), property, newValue, oldValue);
	}
	
	virtual igdeUndo *UndoOptions(gdeProperty *property, const decStringList &options){
		return new gdeUOCTPSetOptions(pPanel.GetObjectClass(), property, options);
	}
	
	virtual igdeUndo *UndoPathPatternType(gdeProperty *property, gdeProperty::ePathPatternTypes type){
		return new gdeUOCTPSetPathPatternType(pPanel.GetObjectClass(), property, type);
	}
	
	virtual igdeUndo *UndoIdentifierGroup(gdeProperty *property, const decString &identifier){
		return new gdeUOCTPSetIdentifierGroup(pPanel.GetObjectClass(), property, identifier);
	}
	
	virtual igdeUndo *UndoIdentifierUsage(gdeProperty *property){
		return new gdeUOCTPToggleIdentifierUsage(pPanel.GetObjectClass(), property);
	}
	
	virtual igdeUndo *UndoCustomFilePatternAdd(gdeProperty *property, gdeFilePattern *filePattern){
		return new gdeUOCTPCFPAdd(pPanel.GetObjectClass(), property, filePattern);
	}
	
	virtual igdeUndo *UndoCustomFilePatternRemove(gdeProperty *property, gdeFilePattern *filePattern){
		return new gdeUOCTPCFPRemove(pPanel.GetObjectClass(), property, filePattern);
	}
	
	virtual igdeUndo *UndoCustomFilePatternName(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name){
		return new gdeUOCTPCFPSetName(pPanel.GetObjectClass(), property, filePattern, name);
	}
	
	virtual igdeUndo *UndoCustomFilePatternPattern(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern){
		return new gdeUOCTPCFPSetPattern(pPanel.GetObjectClass(), property, filePattern, pattern);
	}
	
	virtual igdeUndo *UndoCustomFilePatternExtension(gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension){
		return new gdeUOCTPCFPSetExtension(pPanel.GetObjectClass(), property, filePattern, extension);
	}
};



class cListInherits : public igdeListBoxListener{
	gdeWPSObjectClass &pPanel;
	
public:
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

class cActionInheritAdd : public cBaseAction {
public:
	cActionInheritAdd(gdeWPSObjectClass &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add inherit"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		const decStringSet &classNames = pPanel.GetGameDefinition()->GetClassNameList();
		const int count = classNames.GetCount();
		decStringList proposals;
		int i;
		for(i=0; i<count; i++){
			proposals.Add(classNames.GetAt(i));
		}
		proposals.SortAscending();
		
		decString name("Name");
		if(! igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Inherit", "Name:", name, proposals)){
			return nullptr;
		}
		const gdeObjectClass * const inheritOC = pPanel.GetGameDefinition()->FindObjectClass(name);
		if(inheritOC){
			if(objectClass == inheritOC){
				igdeCommonDialogs::Error(&pPanel, "Add Inherit", "Can not inherit from yourself");
				return nullptr;
			}
			if(inheritOC->InheritsFrom(objectClass)){
				igdeCommonDialogs::Error(&pPanel, "Add Inherit", "Inheritance loop");
				return nullptr;
			}
		}
		
		const gdeOCInherit::Ref objRef(gdeOCInherit::Ref::NewWith(name));
		gdeOCInherit &inherit = (gdeOCInherit&)(deObject&)objRef;
		
		const gdeObjectClass * const ioc = pPanel.GetGameDefinition()->FindObjectClass(name);
		if(ioc){
			inherit.SetPropertyPrefix(ioc->GetDefaultInheritPropertyPrefix());
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCAddInherit::Ref::NewWith(objectClass, &inherit));
		
		pPanel.SelectInherit(&inherit);
		pPanel.UpdateInherit();
		return NULL;
	}
};

class cActionInheritRemove : public cBaseAction {
public:
	cActionInheritRemove(gdeWPSObjectClass &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove inherit"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		gdeOCInherit * const inherit = pPanel.GetInherit();
		return inherit ? new gdeUOCRemoveInherit(objectClass, inherit) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetInherit() != NULL);
	}
};

class cActionInheritRemoveAll : public cBaseAction {
public:
	cActionInheritRemoveAll(gdeWPSObjectClass &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all inherits"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		return objectClass->GetInherits().GetCount() > 0 ? new gdeUOCRemoveAllInherits(objectClass) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetInherit() != NULL);
	}
};

class cComboInheritName : public cBaseComboBoxListener {
public:
	cComboInheritName(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass){
		gdeOCInherit * const inherit = pPanel.GetInherit();
		if(! inherit || inherit->GetName() == comboBox.GetText()){
			return nullptr;
		}
		const gdeObjectClass * const inheritOC = pPanel.GetGameDefinition()->FindObjectClass(comboBox.GetText());
		if(inheritOC){
			if(objectClass == inheritOC){
				igdeCommonDialogs::Error(&pPanel, "Set Inherit Name", "Can not inherit from yourself");
				comboBox.SetText(inherit->GetName());
				return nullptr;
			}
			if(inheritOC->InheritsFrom(objectClass)){
				igdeCommonDialogs::Error(&pPanel, "Set Inherit Name", "Inheritance loop");
				comboBox.SetText(inherit->GetName());
				return nullptr;
			}
		}
		return new gdeUOCInheritSetName(objectClass, inherit, comboBox.GetText());
	}
};

class cTextInheritPropertyPrefix : public cBaseTextFieldListener {
public:
	cTextInheritPropertyPrefix(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass){
		gdeOCInherit * const inherit = pPanel.GetInherit();
		if(! inherit || inherit->GetPropertyPrefix() == textField.GetText()){
			return NULL;
		}
		return new gdeUOCInheritSetPropertyPrefix(objectClass, inherit, textField.GetText());
	}
};

class cActionInheritResetPropertyPrefix : public cBaseAction {
public:
	cActionInheritResetPropertyPrefix(gdeWPSObjectClass &panel) : cBaseAction(panel, "R", NULL,
		"Reset property prefix to default property prefix of inherited object class"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		gdeOCInherit * const inherit = pPanel.GetInherit();
		if(inherit){
			const gdeObjectClass * const ioc = pPanel.GetGameDefinition()->FindObjectClass(inherit->GetName());
			if(ioc){
				if(inherit->GetPropertyPrefix() != ioc->GetDefaultInheritPropertyPrefix()){
					return new gdeUOCInheritSetPropertyPrefix(objectClass, inherit, ioc->GetDefaultInheritPropertyPrefix());
				}
			}
		}
		return NULL;
	}
};

class cActionInheritJumpToClass : public cBaseAction {
public:
	cActionInheritJumpToClass(gdeWPSObjectClass &panel) : cBaseAction(panel, "",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Class"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass*){
		const gdeOCInherit * const inherit = pPanel.GetInherit();
		if(! inherit){
			return NULL;
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeObjectClass * const ioc = gameDefinition.GetObjectClasses().GetNamed(inherit->GetName());
		if(ioc){
			gameDefinition.SetActiveObjectClass(ioc);
			gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotObjectClass);
		}
		return NULL;
	}
};


class cComboCategory : public cBaseComboBoxListener {
public:
	cComboCategory(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass){
		if(objectClass->GetCategory() == comboBox.GetText()){
			return NULL;
		}
		return new gdeUOCSetCategory(objectClass, comboBox.GetText());
	}
};

class cActionJumpToCategory : public cBaseAction {
public:
	cActionJumpToCategory(gdeWPSObjectClass &panel) : cBaseAction(panel, "",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Category"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeCategory * const category = gameDefinition.GetCategoriesObjectClass()
			.GetWithPath(objectClass->GetCategory());
		if(category){
			gameDefinition.SetActiveCategory(category);
			gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategoryObjectClass);
		}
		return NULL;
	}
};



class cListHideTags : public gdeWPTagList {
	gdeWPSObjectClass &pPanel;
	
public:
	cListHideTags(gdeWPSObjectClass &panel, igdeUIHelper &helper) :
	gdeWPTagList(helper, 5, "Hide Tags"), pPanel(panel){}
	
	virtual igdeUndo *UndoSet(const decStringSet &tags){
		return new gdeUOCSetHideTags(pPanel.GetObjectClass(), tags);
	}
};

class cListPartialHideTags : public gdeWPTagList {
	gdeWPSObjectClass &pPanel;
	
public:
	cListPartialHideTags(gdeWPSObjectClass &panel, igdeUIHelper &helper) :
	gdeWPTagList(helper, 5, "Partial Hide Tags"), pPanel(panel){}
	
	virtual igdeUndo *UndoSet(const decStringSet &tags){
		return new gdeUOCSetPartialHideTags(pPanel.GetObjectClass(), tags);
	}
};


class cActionPropertyValueSet : public cBaseAction {
public:
	cActionPropertyValueSet(gdeWPSObjectClass &panel) : cBaseAction(panel, "Set",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Set property value"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		const decString &key = pPanel.GetPropertyKey();
		if(! objectClass || key.IsEmpty()){
			return NULL;
		}
		
		decString value;
		objectClass->NamedPropertyDefaultValue(key, value);
		if(! igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Set Property Value", "Value:", value)){
			return NULL;
		}
		
		decStringDictionary values(objectClass->GetPropertyValues());
		values.SetAt(key, value);
		return new gdeUOCSetPropertyValues(objectClass, values);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetObjectClass() && ! pPanel.GetPropertyKey().IsEmpty());
	}
};

class cActionPropertyValueRemove : public cBaseAction {
public:
	cActionPropertyValueRemove(gdeWPSObjectClass &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove property value"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		const char * const key = pPanel.GetPropertyValue();
		if(! key || ! objectClass->GetPropertyValues().Has(key)){
			return NULL;
		}
		
		decStringDictionary values(objectClass->GetPropertyValues());
		values.Remove(key);
		return new gdeUOCSetPropertyValues(objectClass, values);
	}
	
	virtual void Update(){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		const char * const key = pPanel.GetPropertyValue();
		SetEnabled(objectClass && key && objectClass->GetPropertyValues().Has(key));
	}
};

class cActionPropertyValueClear : public cBaseAction {
public:
	cActionPropertyValueClear(gdeWPSObjectClass &panel) : cBaseAction(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all property values"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		if(objectClass->GetPropertyValues().GetCount() == 0){
			return NULL;
		}
		return new gdeUOCSetPropertyValues(objectClass, decStringDictionary());
	}
	
	virtual void Update(){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		SetEnabled(objectClass && objectClass->GetPropertyValues().GetCount() > 0);
	}
};

class cActionPropertyValuesFromSubObjects : public cBaseAction {
public:
	cActionPropertyValuesFromSubObjects(gdeWPSObjectClass &panel) : cBaseAction(panel,
		"Set from sub objects", NULL, "Set property values from sub objects links"){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		return gdeUOCPropertyValuesFromSubObjects::Create(objectClass);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetObjectClass());
	}
};

class cListPropertyValues : public igdeIconListBoxListener {
	gdeWPSObjectClass &pPanel;
	
public:
	cListPropertyValues(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeIconListBox *listBox, int index){
		gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		if(! objectClass){
			return;
		}
		
		const decString &key = listBox->GetItemAt(index)->GetText();
		decString value(objectClass->GetPropertyValues().GetAt(key, ""));
		if(! igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Edit Property Value", "Value:", value)){
			return;
		}
		
		decStringDictionary values(objectClass->GetPropertyValues());
		values.SetAt(key, value);
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCSetPropertyValues::Ref::NewWith(objectClass, values));
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
	cBaseComboBoxTextureListener(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture(comboBox, objectClass, texture) : NULL;
	}
	
	virtual igdeUndo *OnChangedTexture(igdeComboBox &comboBox, gdeObjectClass *objectClass,
		gdeOCComponentTexture *texture) = 0;
};

class cBaseActionTexture : public cBaseAction{
public:
	cBaseActionTexture(gdeWPSObjectClass &panel, const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnActionTexture(objectClass, texture) : NULL;
	}
	
	virtual igdeUndo *OnActionTexture(gdeObjectClass *objectClass, gdeOCComponentTexture *texture) = 0;
	
	virtual void Update(){
		SetEnabled(pPanel.GetTexture());
	}
};

class cBaseTextFieldTextureListener : public cBaseTextFieldListener{
public:
	cBaseTextFieldTextureListener(gdeWPSObjectClass &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture(textField, objectClass, texture) : NULL;
	}
	
	virtual igdeUndo *OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass,
		gdeOCComponentTexture *texture) = 0;
};

class cBaseEditVector2TextureListener : public igdeEditVector2Listener{
protected:
	gdeWPSObjectClass &pPanel;
	
public:
	cBaseEditVector2TextureListener(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(! texture){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChangedTexture(editVector2->GetVector2(), pPanel.GetObjectClass(), texture)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass,
		gdeOCComponentTexture *texture) = 0;
};


class cComboTextures : public cBaseComboBoxListener{
public:
	cComboTextures(gdeWPSObjectClass &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass){
		const igdeListItem * const selection = comboBox.GetSelectedItem();
		objectClass->SetActiveTexture(selection ? (gdeOCComponentTexture*)selection->GetData() : NULL);
		pPanel.GetGameDefinition()->NotifyOCActiveTextureChanged(objectClass);
		return NULL;
	}
};

class cActionTextureAdd : public cBaseAction{
	const decString pTextureName;
	
public:
	cActionTextureAdd(gdeWPSObjectClass &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add texture"){}
	
	cActionTextureAdd(gdeWPSObjectClass &panel, const decString &textureName) :
		cBaseAction(panel, textureName, NULL, "Add texture"), pTextureName(textureName){}
	
	virtual igdeUndo *OnActionObjectClass(gdeObjectClass *objectClass){
		decString name(pTextureName);
		
		if(name.IsEmpty()){
			name = "Texture";
			
			while(true){
				if(! igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Texture", "Name:", name)){
					return NULL;
				}
				
				if(objectClass->GetTextures().HasNamed(name)){
					igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Add Texture", "A texture with this name exists already.");
					
				}else{
					break;
				}
			}
		}
		
		const gdeOCComponentTexture::Ref texture(gdeOCComponentTexture::Ref::NewWith(name));
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCAddTexture::Ref::NewWith(objectClass, texture));
		
		objectClass->SetActiveTexture(texture);
		pPanel.GetGameDefinition()->NotifyOCActiveTextureChanged(objectClass);
		return NULL;
	}
	
	virtual void Update(){
		const gdeObjectClass * const objectClass = pPanel.GetObjectClass();
		SetEnabled(objectClass && (pTextureName.IsEmpty() || ! objectClass->GetTextures().HasNamed(pTextureName)));
	}
};

class cActionTextureRemove : public cBaseActionTexture{
public:
	cActionTextureRemove(gdeWPSObjectClass &panel) : cBaseActionTexture(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove selected texture"){}
	
	virtual igdeUndo *OnActionTexture(gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
		return new gdeUOCRemoveTexture(objectClass, texture);
	}
};

class cActionTexturesMenu : public igdeActionContextMenu{
	gdeWPSObjectClass &pPanel;
	
public:
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
	cTextTextureEditName(gdeWPSObjectClass &panel) : cBaseTextFieldTextureListener(panel){}
	
	virtual igdeUndo *OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
		if(texture->GetName() == textField.GetText()){
			return NULL;
		}
		
		if(objectClass->GetTextures().HasNamed(textField.GetText())){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Rename texture", "A texture with this name exists already.");
			textField.SetText(texture->GetName());
			return NULL;
		}
		
		return new gdeUOCTextureSetName(objectClass, texture, textField.GetText());
	}
};

class cEditTextureEditPathSkin : public igdeEditPathListener{
	gdeWPSObjectClass &pPanel;
	
public:
	cEditTextureEditPathSkin(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(! texture || texture->GetPathSkin() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCTextureSetPathSkin::Ref::NewWith(
			pPanel.GetObjectClass(), texture, editPath->GetPath()));
	}
};

class cEditTextureEditOffset : public cBaseEditVector2TextureListener{
public:
	cEditTextureEditOffset(gdeWPSObjectClass &panel) : cBaseEditVector2TextureListener(panel){}
	
	virtual igdeUndo *OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
		if(texture->GetOffset().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCTextureSetOffset(objectClass, texture, vector);
	}
};

class cTextTextureEditRotation : public cBaseTextFieldTextureListener{
public:
	cTextTextureEditRotation(gdeWPSObjectClass &panel) : cBaseTextFieldTextureListener(panel){}
	
	virtual igdeUndo *OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
		const float value = textField.GetFloat();
		if(fabsf(texture->GetRotation() - value) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new gdeUOCTextureSetRotation(objectClass, texture, value);
	}
};

class cEditTextureEditScale : public cBaseEditVector2TextureListener{
public:
	cEditTextureEditScale(gdeWPSObjectClass &panel) : cBaseEditVector2TextureListener(panel){}
	
	virtual igdeUndo *OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
		if(texture->GetScale().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCTextureSetScale(objectClass, texture, vector);
	}
};

class cColorTextureTint : public igdeColorBoxListener{
	gdeWPSObjectClass &pPanel;
	
public:
	cColorTextureTint(gdeWPSObjectClass &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(! texture || texture->GetColorTint().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCTextureSetColorTint::Ref::NewWith(
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
pWindowProperties(windowProperties),
pListener(NULL),
pGameDefinition(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, form, frameLine;
	
	pListener = new gdeWPSObjectClassListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// actions
	pActionInheritAdd.TakeOver(new cActionInheritAdd(*this));
	pActionInheritRemove.TakeOver(new cActionInheritRemove(*this));
	pActionInheritRemoveAll.TakeOver(new cActionInheritRemoveAll(*this));
	
	pActionPropertyValueSet.TakeOver(new cActionPropertyValueSet(*this));
	pActionPropertyValueRemove.TakeOver(new cActionPropertyValueRemove(*this));
	pActionPropertyValueClear.TakeOver(new cActionPropertyValueClear(*this));
	pActionPropertyValuesFromSubObjects.TakeOver(new cActionPropertyValuesFromSubObjects(*this));
	
	pActionTexturesMenu.TakeOver(new cActionTexturesMenu(*this));
	pActionTextureAdd.TakeOver(new cActionTextureAdd(*this));
	pActionTextureRemove.TakeOver(new cActionTextureRemove(*this));
	
	
	// object class
	helper.GroupBox(content, groupBox, "Object Class:");
	helper.EditString(groupBox, "Name:", "Object class name", pEditName, new cTextName(*this));
	helper.EditString(groupBox, "Description:", "Object class description",
		pEditDescription, 15, 5, new cTextDescription(*this));
	
	helper.ComboBox(groupBox, "Scale Mode:", "Object scale mode", pCBScaleMode,
		new cComboScaleMode(*this));
	pCBScaleMode->AddItem("Fixed", NULL, (void*)(intptr_t)gdeObjectClass::esmFixed);
	pCBScaleMode->AddItem("Uniform", NULL, (void*)(intptr_t)gdeObjectClass::esmUniform);
	pCBScaleMode->AddItem("Free", NULL, (void*)(intptr_t)gdeObjectClass::esmFree);
	
	helper.EditString(groupBox, "Def Inherit Prop Pref:",
		"Default property prefix to use if this object class is inherited from",
		pEditDefaultInheritPropertyPrefix, new cTextDefaultInheritPropertyPrefix(*this));
	
	helper.CheckBox(groupBox, pChkIsGhost, new cActionIsGhost(*this), true);
	helper.CheckBox(groupBox, pChkCanInstantiate, new cActionCanInstantiate(*this), true);
	helper.CheckBox(groupBox, pChkIsAttachableBehavior, new cActionIsAttachableBehavior(*this), true);
	
	helper.FormLineStretchFirst(groupBox, "Category: ", "Category", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Category", pCBCategory, new cComboCategory(*this));
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToCategory, new cActionJumpToCategory(*this), true);
	
	
	// inherits
	helper.GroupBoxFlow(content, groupBox, "Inherits:");
	
	helper.ListBox(groupBox, 5, "Inherited object classes", pListInherits, new cListInherits(*this));
	pListInherits->SetDefaultSorter();
	
	form.TakeOver(new igdeContainerForm(env));
	groupBox->AddChild(form);
	
	helper.FormLineStretchFirst(form, "Name:", "Name of class to inherit", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Name of class to inherit",
		pInheritCBClass, new cComboInheritName(*this));
	pInheritCBClass->SetDefaultSorter();
	pInheritCBClass->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToInheritClass, new cActionInheritJumpToClass(*this), true);
	
	helper.FormLineStretchFirst(form, "Property Prefix:",
		"Prefix to apply to inherited property names", frameLine);
	helper.EditString(frameLine, "Prefix to apply to inherited property names",
		pInheritEditPropertyPrefix, new cTextInheritPropertyPrefix(*this));
	helper.Button(frameLine, pBtnInheritPropertyPrefixReset, new cActionInheritResetPropertyPrefix(*this), true);
	
	
	// inherit sub objects
	helper.GroupBoxFlow(content, groupBox, "Inherit Sub-Objects:", false, true);
	helper.CheckBox(groupBox, pChkInheritSOBillboards, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoBillboards, "Billboards", "Inherit billboards"), true);
	helper.CheckBox(groupBox, pChkInheritSOComponents, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoComponents, "Components", "Inherit components"), true);
	helper.CheckBox(groupBox, pChkInheritSOLights, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoLights, "Lights", "Inherit lights"), true);
	helper.CheckBox(groupBox, pChkInheritSOSnapPoints, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoSnapPoints, "Snap Points", "Inherit snap points"), true);
	helper.CheckBox(groupBox, pChkInheritSOParticleEmitters, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoParticleEmitters, "Particle Emitters", "Inherit particle emitters"), true);
	helper.CheckBox(groupBox, pChkInheritSOForceFields, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoForceFields, "Force Fields", "Inherit force fields"), true);
	helper.CheckBox(groupBox, pChkInheritSOEnvMapProbes, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoEnvMapProbes, "Environment Map Probes", "Inherit environment map probes"), true);
	helper.CheckBox(groupBox, pChkInheritSOSpeakers, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoSpeakers, "Speakers", "Inherit speakers"), true);
	helper.CheckBox(groupBox, pChkInheritSONavigationSpaces, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoNavigationSpaces, "Navigation Spaces", "Inherit navigation spaces"), true);
	helper.CheckBox(groupBox, pChkInheritSONavigationBlockers, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoNavigationBlockers, "Navigation Blockers", "Inherit navigation blockers"), true);
	helper.CheckBox(groupBox, pChkInheritSOWorlds, new cActionInheritSubObjects(*this,
		igdeGDClass::efsoWorlds, "Worlds", "Inherit worlds"), true);
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "Properties:");
	pEditProperties.TakeOver(new cEditProperties(*this));
	groupBox->AddChild(pEditProperties);
	
	
	// property values
	helper.GroupBoxFlow(content, groupBox, "Property Values:", true);
	
	frameLine.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	helper.ComboBox(frameLine, true, "Property value to set", pCBPropertyValuesKeys, NULL);
	pCBPropertyValuesKeys->SetDefaultSorter();
	helper.Button(frameLine, pBtnPropertyValueSet, pActionPropertyValueSet);
	groupBox->AddChild(frameLine);
	
	const igdeUIHelper::sColumnHeader headersPropertyValues[2] = {
		igdeUIHelper::sColumnHeader("Key", nullptr, igdeApplication::app().DisplayScaled(150)),
		igdeUIHelper::sColumnHeader("Value", nullptr, igdeApplication::app().DisplayScaled(200))
	};
	helper.IconListBox(groupBox, pListPropertyValues,
		igdeApplication::app().DisplayScaled(decPoint(100, 120)),
		headersPropertyValues, 2, "Property values", new cListPropertyValues(*this));
	pListPropertyValues->SetDefaultSorter();
	
	
	// texture properties
	helper.GroupBoxFlow(content, groupBox, "Texture Properties:", false, true);
	pEditTextureProperties.TakeOver(new cEditTextureProperties(*this));
	groupBox->AddChild(pEditTextureProperties);
	
	
	// tagging
	helper.GroupBoxFlow(content, groupBox, "Hide Tags:", false, true);
	pListHideTags.TakeOver(new cListHideTags(*this, helper));
	groupBox->AddChild(pListHideTags);
	
	helper.GroupBoxFlow(content, groupBox, "Partial Hide Tags:", false, true);
	pListPartialHideTags.TakeOver(new cListPartialHideTags(*this, helper));
	groupBox->AddChild(pListPartialHideTags);
	
	
	// textures
	helper.GroupBox(content, groupBox, "Textures:");
	
	helper.FormLineStretchFirst(groupBox, "Texture:", "Texture to edit", frameLine);
	helper.ComboBox(frameLine, "Texture to edit", pCBTextures, new cComboTextures(*this));
	pCBTextures->SetDefaultSorter();
	helper.Button(frameLine, pBtnTextures, pActionTexturesMenu);
	pActionTexturesMenu->SetWidget(pBtnTextures);
	
	helper.EditString(groupBox, "Name:", "Name of texture", pTextureEditName, new cTextTextureEditName(*this));
	helper.EditPath(groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pTextureEditPathSkin, new cEditTextureEditPathSkin(*this));
	helper.EditVector2(groupBox, "Offset:", "Texture coordinate offset",
		pTextureEditOffset, new cEditTextureEditOffset(*this));
	helper.EditFloat(groupBox, "Rotation:",
		"Texture coordinate rotation around texture center in degrees",
		pTextureEditRotation, new cTextTextureEditRotation(*this));
	helper.EditVector2(groupBox, "Scale:", "Texture coordinate scaling",
		pTextureEditScale, new cEditTextureEditScale(*this));
	helper.ColorBox(groupBox, "Tint:", "Texture tint color",
		pTextureClrTint, new cColorTextureTint(*this));
}

gdeWPSObjectClass::~gdeWPSObjectClass(){
	SetGameDefinition(NULL);
	
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSObjectClass::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	gdeWPTagList &listHideTags = (gdeWPTagList&)(igdeWidget&)pListHideTags;
	listHideTags.SetUndoSystem(NULL);
	listHideTags.SetTagList(NULL);
	
	gdeWPTagList &listPartialHideTags = (gdeWPTagList&)(igdeWidget&)pListPartialHideTags;
	listPartialHideTags.SetUndoSystem(NULL);
	listPartialHideTags.SetTagList(NULL);
	
	gdeWPPropertyList &properties = (gdeWPPropertyList&)(igdeWidget&)pEditProperties;
	properties.SetPropertyList(NULL);
	properties.SetGameDefinition(NULL);
	
	gdeWPPropertyList &textureProperties = (gdeWPPropertyList&)(igdeWidget&)pEditTextureProperties;
	textureProperties.SetPropertyList(NULL);
	textureProperties.SetGameDefinition(NULL);
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		gameDefinition->AddReference();
		
		listHideTags.SetUndoSystem(gameDefinition->GetUndoSystem());
		listPartialHideTags.SetUndoSystem(gameDefinition->GetUndoSystem());
		properties.SetGameDefinition(gameDefinition);
		textureProperties.SetGameDefinition(gameDefinition);
	}
	
	UpdateCategoryList();
	UpdateClassLists();
	
	UpdateObjectClass();
	UpdateIdentifierLists();
}



gdeObjectClass *gdeWPSObjectClass::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeProperty *gdeWPSObjectClass::GetProperty() const{
	return ((gdeWPPropertyList&)(igdeWidget&)pEditProperties).GetProperty();
}

gdeProperty *gdeWPSObjectClass::GetTextureProperty() const{
	return ((gdeWPPropertyList&)(igdeWidget&)pEditTextureProperties).GetProperty();
}

gdeOCInherit *gdeWPSObjectClass::GetInherit() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	if(! objectClass){
		return NULL;
	}
	
	const igdeListItem * const selection = pListInherits->GetSelectedItem();
	return selection ? (gdeOCInherit*)selection->GetData() : NULL;
}

const decString &gdeWPSObjectClass::GetPropertyKey() const{
	return pCBPropertyValuesKeys->GetText();
}

const char *gdeWPSObjectClass::GetPropertyValue() const{
	const igdeListItem * const selection = pListPropertyValues->GetSelectedItem();
	return selection ? selection->GetText().GetString() : NULL;
}

gdeOCComponentTexture *gdeWPSObjectClass::GetTexture() const{
	gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? objectClass->GetActiveTexture().Pointer() : nullptr;
}



void gdeWPSObjectClass::UpdateUsedTagsList(){
	gdeWPTagList &hideTags = (gdeWPTagList&)(igdeWidget&)pListHideTags;
	gdeWPTagList &partialHideTags = (gdeWPTagList&)(igdeWidget&)pListPartialHideTags;
	
	if(pGameDefinition){
		hideTags.UpdateUsedTagList(pGameDefinition->GetUsedTagsObjectClass());
		partialHideTags.UpdateUsedTagList(pGameDefinition->GetUsedTagsObjectClass());
		
	}else{
		hideTags.UpdateUsedTagList(decStringSet());
		partialHideTags.UpdateUsedTagList(decStringSet());
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
	pCBCategory->SetInvalidValue(! pCBCategory->GetText().IsEmpty() && ! pCBCategory->GetSelectedItem());
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
	pInheritCBClass->SetInvalidValue(! pInheritCBClass->GetText().IsEmpty()
		&& ! pInheritCBClass->GetSelectedItem());
}

void gdeWPSObjectClass::UpdateIdentifierLists(){
	((gdeWPPropertyList&)(igdeWidget&)pEditProperties).UpdatePropertyIdentifierList();
	((gdeWPPropertyList&)(igdeWidget&)pEditTextureProperties).UpdatePropertyIdentifierList();
}



void gdeWPSObjectClass::UpdateObjectClass(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	gdeWPPropertyList &properties = (gdeWPPropertyList&)(igdeWidget&)pEditProperties;
	gdeWPPropertyList &textureProperties = (gdeWPPropertyList&)(igdeWidget&)pEditTextureProperties;
	gdeWPTagList &hideTags = (gdeWPTagList&)(igdeWidget&)pListHideTags;
	gdeWPTagList &partialHideTags = (gdeWPTagList&)(igdeWidget&)pListPartialHideTags;
	
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
		pCBCategory->SetInvalidValue(! pCBCategory->GetText().IsEmpty()
			&& ! pCBCategory->GetSelectedItem());
		properties.SetPropertyList(&objectClass->GetProperties());
		textureProperties.SetPropertyList(&objectClass->GetTextureProperties());
		hideTags.SetTagList(&objectClass->GetHideTags());
		partialHideTags.SetTagList(&objectClass->GetPartialHideTags());
		
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
		properties.SetPropertyList(NULL);
		textureProperties.SetPropertyList(NULL);
		hideTags.SetTagList(NULL);
		partialHideTags.SetTagList(NULL);
	}
	
	const bool enabled = objectClass != NULL;
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
	((gdeWPPropertyList&)(igdeWidget&)pEditProperties).UpdateList();
}

void gdeWPSObjectClass::UpdateProperty(){
	((gdeWPPropertyList&)(igdeWidget&)pEditProperties).UpdateProperty();
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
	((gdeWPPropertyList&)(igdeWidget&)pEditTextureProperties).UpdateList();
}

void gdeWPSObjectClass::UpdateTextureProperty(){
	((gdeWPPropertyList&)(igdeWidget&)pEditTextureProperties).UpdateProperty();
}

void gdeWPSObjectClass::UpdateInherits(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	gdeOCInherit * const activeInherit = GetInherit();
	
	pListInherits->RemoveAllItems();
	
	if(objectClass){
		const gdeOCInheritList &list = objectClass->GetInherits();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			gdeOCInherit * const inherit = list.GetAt(i);
			text.Format("%s: '%s'", inherit->GetName().GetString(), inherit->GetPropertyPrefix().GetString());
			pListInherits->AddItem(text, NULL, inherit);
		}
		pListInherits->SortItems();
	}
	
	pListInherits->SetSelectionWithData(activeInherit);
	if(! pListInherits->GetSelectedItem() && pListInherits->GetItemCount() > 0){
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
	
	pInheritCBClass->SetInvalidValue(! pInheritCBClass->GetText().IsEmpty()
		&& ! pInheritCBClass->GetSelectedItem());
	
	pInheritCBClass->SetEnabled(inherit != NULL);
	pInheritEditPropertyPrefix->SetEnabled(inherit != NULL);
}

void gdeWPSObjectClass::SelectInherit(gdeOCInherit *inherit){
	pListInherits->SetSelectionWithData(inherit);
}

void gdeWPSObjectClass::UpdateHideTags(){
	((gdeWPTagList&)(igdeWidget&)pListHideTags).UpdateList();
}

void gdeWPSObjectClass::UpdatePartialHideTags(){
	((gdeWPTagList&)(igdeWidget&)pListPartialHideTags).UpdateList();
}

void gdeWPSObjectClass::UpdateTextureList(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	
	pCBTextures->RemoveAllItems();
	
	if(objectClass){
		const gdeOCComponentTextureList &textures = objectClass->GetTextures();
		const int count = textures.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			gdeOCComponentTexture * const texture = textures.GetAt(i);
			pCBTextures->AddItem(texture->GetName(), NULL, texture);
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
