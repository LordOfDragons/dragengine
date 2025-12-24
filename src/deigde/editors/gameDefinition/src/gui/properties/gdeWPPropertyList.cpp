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

#include "gdeWPPropertyList.h"
#include "gdeDefaultPropertyValue.h"
#include "../../clipboard/gdeClipboardDataProperty.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/filepattern/gdeFilePattern.h"
#include "../../gamedef/property/gdeProperty.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gui/composed/igdeEditPropertyValueListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cComboProperty : public igdeComboBoxListener{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cComboProperty> Ref;
	cComboProperty(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateProperty();
	}
};


class cActionPropertyMenu : public igdeActionContextMenu{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionPropertyMenu> Ref;
	cActionPropertyMenu(gdeWPPropertyList &panel) :
	igdeActionContextMenu("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Show Properties Menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionPropertyAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionPropertyRemove());
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, pPanel.GetActionPropertyCopy());
		helper.MenuCommand(contextMenu, pPanel.GetActionPropertyCut());
		helper.MenuCommand(contextMenu, pPanel.GetActionPropertyPaste());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetPropertyList() != nullptr);
	}
};

class cActionPropertyAdd : public igdeAction{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionPropertyAdd> Ref;
	cActionPropertyAdd(gdeWPPropertyList &panel) :
	igdeAction("Add...", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add property"),
	pPanel(panel){}
	
	void OnAction() override{
		if(!pPanel.GetPropertyList() || !pPanel.GetUndoSystem()){
			return;
		}
		
		const gdeProperty::List &list = *pPanel.GetPropertyList();
		decString name("Property");
		
		while(igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Property", "Name:", name)){
			if(list.HasMatching([&](const gdeProperty *property){
				return property->GetName() == name;
			})){
				igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Add Property", "Name exists already.");
				continue;
			}
			
			const gdeProperty::Ref property(gdeProperty::Ref::New(name));
			
			igdeUndo::Ref undo(pPanel.UndoAdd(property));
			if(undo){
				pPanel.GetUndoSystem()->Add(undo);
				pPanel.SelectProperty(property);
			}
			return;
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetPropertyList() != nullptr);
	}
};

class cActionPropertyRemove : public igdeAction{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionPropertyRemove> Ref;
	cActionPropertyRemove(gdeWPPropertyList &panel) :
	igdeAction("Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected property"),
	pPanel(panel){}
	
	void OnAction() override{
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		igdeUndo::Ref undo(pPanel.UndoRemove(property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetProperty() != nullptr);
	}
};

class cActionPropertyCopy : public igdeAction{
protected:
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionPropertyCopy> Ref;
	cActionPropertyCopy(gdeWPPropertyList &panel) :
	igdeAction("Copy", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy selected property"),
	pPanel(panel){}
	
	void OnAction() override{
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetClipboard()){
			return;
		}
		
		const gdeProperty::Ref clipProperty(gdeProperty::Ref::New(*property));
		
		pPanel.GetClipboard()->Set(gdeClipboardDataProperty::Ref::New(clipProperty));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetProperty() != nullptr);
	}
};

class cActionPropertyCut : public cActionPropertyCopy {
public:
	cActionPropertyCut(gdeWPPropertyList &panel) : cActionPropertyCopy(panel){
		SetText("Cut");
		SetIcon(panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut));
		SetDescription("Cut selected property");
	}
	
	void OnAction() override{
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem() || !pPanel.GetClipboard()){
			return;
		}
		
		cActionPropertyCopy::OnAction();
		
		igdeUndo::Ref undo(pPanel.UndoRemove(property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
		}
	}
};

class cActionPropertyPaste : public igdeAction{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionPropertyPaste> Ref;
	cActionPropertyPaste(gdeWPPropertyList &panel) :
	igdeAction("Paste", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste property"),
	pPanel(panel){}
	
	void OnAction() override{
		if(!pPanel.GetClipboard()){
			return;
		}
		
		const gdeClipboardDataProperty * const clip = (const gdeClipboardDataProperty *)
			pPanel.GetClipboard()->GetWithTypeName(gdeClipboardDataProperty::TYPE_NAME);
		if(!clip){
			return;
		}
		
		const gdeProperty::List &list = *pPanel.GetPropertyList();
		decString name(clip->GetProperty()->GetName());
		
		while(list.HasMatching([&](const gdeProperty *property){
			return property->GetName() == name;
		})){
			igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Paste Property", "Name exists already.");
			if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Paste Property", "Name:", name)){
				return;
			}
		}
		
		const gdeProperty::Ref refProperty(gdeProperty::Ref::New(*clip->GetProperty()));
		gdeProperty * const property = refProperty;
		property->SetName(name);
		
		igdeUndo::Ref undo(pPanel.UndoPaste(property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
			pPanel.SelectProperty(property);
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetPropertyList() && pPanel.GetClipboard()
			&& pPanel.GetClipboard()->HasWithTypeName(gdeClipboardDataProperty::TYPE_NAME));
	}
};


class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, gdeProperty *property) = 0;
};

class cBaseTextAreaListener : public igdeTextAreaListener{
protected:
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextAreaListener> Ref;
	cBaseTextAreaListener(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textArea, property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextArea *textArea, gdeProperty *property) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, gdeProperty *property) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPPropertyList &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description), pPanel(panel){}
	
	void OnAction() override{
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		igdeUndo::Ref undo(OnActionUndo(property));
		if(undo){
			pPanel.GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionUndo(gdeProperty *property) = 0;
	
	void Update() override{
		SetEnabled(pPanel.GetProperty() != nullptr);
	}
};


class cEditName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditName> Ref;
	cEditName(gdeWPPropertyList &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, gdeProperty *property) override{
		const decString &name = textField->GetText();
		if(name == property->GetName()){
			return {};
		}
		
		if(pPanel.GetPropertyList()->HasMatching([&](const gdeProperty *other){
			return other->GetName() == name;
		})){
			igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Rename property", "Name exists already.");
			textField->SetText(property->GetName());
			return {};
		}
		
		return pPanel.UndoName(property, name);
	}
};

class cEditDescription : public cBaseTextAreaListener{
public:
	typedef deTObjectReference<cEditDescription> Ref;
	cEditDescription(gdeWPPropertyList &panel) : cBaseTextAreaListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextArea *textArea, gdeProperty *property) override{
		if(textArea->GetText() == property->GetDescription()){
			return {};
		}
		return pPanel.UndoDescription(property, textArea->GetText());
	}
};

class cComboType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboType> Ref;
	cComboType(gdeWPPropertyList &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, gdeProperty *property) override{
		const gdeProperty::ePropertyTypes type =
			(gdeProperty::ePropertyTypes)(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(type == property->GetType()){
			return {};
		}
		
		igdeUndo::Ref undo(pPanel.UndoType(property, type));
		pPanel.GetUndoSystem()->Add(undo);
		
		pPanel.SetDefaultValueFromType();
		return {};
	}
};

class cEditDefault : public igdeEditPropertyValueListener{
	gdeWPPropertyList &pPanel;
	igdeUndo::Ref pUndo;
	decString pOldValue;
	
public:
	typedef deTObjectReference<cEditDefault> Ref;
	cEditDefault(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnPropertyValueChanged(igdeEditPropertyValue *editPropertyValue){
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		if(!pUndo){
			pOldValue = property->GetDefaultValue();
		}
		pUndo = pPanel.UndoDefaultValue(property, editPropertyValue->GetValue(), pOldValue);
		if(pUndo){
			pPanel.GetUndoSystem()->Add(pUndo);
			pUndo = NULL;
		}
	}
	
	virtual void OnPropertyValueChanging(igdeEditPropertyValue *editPropertyValue){
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		if(!pUndo){
			pOldValue = property->GetDefaultValue();
		}
		pUndo = pPanel.UndoDefaultValue(property, editPropertyValue->GetValue(), pOldValue);
		if(pUndo){
			pUndo->Redo();
		}
	}
};

class cEditMinimum : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cEditMinimum> Ref;
	cEditMinimum(gdeWPPropertyList &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, gdeProperty *property){
		const float value = textField->GetFloat();
		if(fabsf(value - property->GetMinimumValue()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return pPanel.UndoMinimumValue(property, value);
	}
};

class cEditMaximum : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cEditMaximum> Ref;
	cEditMaximum(gdeWPPropertyList &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, gdeProperty *property) override{
		const float value = textField->GetFloat();
		if(fabsf(value - property->GetMaximumValue()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return pPanel.UndoMaximumValue(property, value);
	}
};


class cListOptions : public igdeListBoxListener{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cListOptions> Ref;
	cListOptions(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox*){
	}
	
	virtual void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu){
		igdeUIHelper &helper = listBox->GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionOptionAdd());
		helper.MenuCommand(menu, pPanel.GetActionOptionRemove());
	}
};

class cActionOptionAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionOptionAdd> Ref;
	cActionOptionAdd(gdeWPPropertyList &panel) : cBaseAction(panel, "Add Option...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add option"){}
	
	igdeUndo::Ref OnActionUndo(gdeProperty *property) override{
		decString option("Option");
		
		while(igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Option", "Option:", option)){
			if(property->GetOptions().Has(option)){
				igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Add Option", "Option exists already.");
				continue;
			}
			
			decStringList options(property->GetOptions());
			options.Add(option);
			return pPanel.UndoOptions(property, options);
		}
		
		return {};
	}
};

class cActionOptionRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionOptionRemove> Ref;
	cActionOptionRemove(gdeWPPropertyList &panel) : cBaseAction(panel, "Remove Option",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove option"){}
	
	igdeUndo::Ref OnActionUndo(gdeProperty *property) override{
		const decString option(pPanel.GetOption());
		if(option.IsEmpty()){
			return {};
		}
		
		decStringList options(property->GetOptions());
		options.RemoveFrom(options.IndexOf(option));
		return pPanel.UndoOptions(property, options);
	}
};


class cComboPathPatternType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPathPatternType> Ref;
	cComboPathPatternType(gdeWPPropertyList &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, gdeProperty *property) override{
		const gdeProperty::ePathPatternTypes type =
			(gdeProperty::ePathPatternTypes)(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(type == property->GetPathPatternType()){
			return {};
		}
		return pPanel.UndoPathPatternType(property, type);
	}
};

class cComboCustomPattern : public igdeComboBoxListener{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cComboCustomPattern> Ref;
	cComboCustomPattern(gdeWPPropertyList &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateCustomPattern();
	}
};


class cActionCustomPatternMenu : public igdeActionContextMenu{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionCustomPatternMenu> Ref;
	cActionCustomPatternMenu(gdeWPPropertyList &panel) :
	igdeActionContextMenu("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Show Custom Pattern Menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionCustomPatternAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionCustomPatternRemove());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetPropertyList() != nullptr);
	}
};

class cActionCustomPatternAdd : public igdeAction{
	gdeWPPropertyList &pPanel;
	
public:
	typedef deTObjectReference<cActionCustomPatternAdd> Ref;
	cActionCustomPatternAdd(gdeWPPropertyList &panel) : igdeAction("Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add custom pattern"),
		pPanel(panel){}
	
	void OnAction() override{
		gdeProperty * const property = pPanel.GetProperty();
		if(!property || !pPanel.GetUndoSystem()){
			return;
		}
		
		decString name("File Pattern");
		if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add File Pattern", "Name:", name)){
			return;
		}
		
		const gdeFilePattern::Ref refFilePattern(gdeFilePattern::Ref::New(name, "*.ext", "*.ext"));
		gdeFilePattern * const filePattern = refFilePattern;
		
		pPanel.GetUndoSystem()->Add(
			 pPanel.UndoCustomFilePatternAdd(property, filePattern));
		
		pPanel.SelectCustomPattern(filePattern);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetProperty() != nullptr);
	}
};

class cActionCustomPatternRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionCustomPatternRemove> Ref;
	cActionCustomPatternRemove(gdeWPPropertyList &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove custom pattern"){}
	
	igdeUndo::Ref OnActionUndo(gdeProperty *property) override{
		gdeFilePattern * const filePattern = pPanel.GetCustomPattern();
		if(!filePattern){
			return {};
		}
		return pPanel.UndoCustomFilePatternRemove(property, filePattern);
	}
};


class cBaseTextFieldListenerCustomPattern : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cBaseTextFieldListenerCustomPattern> Ref;
	cBaseTextFieldListenerCustomPattern(gdeWPPropertyList &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, gdeProperty *property) override{
		gdeFilePattern * const filePattern = pPanel.GetCustomPattern();
		return filePattern ? OnChangedPattern(textField, property, filePattern) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnChangedPattern(igdeTextField *textField,
		gdeProperty *property, gdeFilePattern *filePattern) = 0;
};


class cEditCustomPatternName : public cBaseTextFieldListenerCustomPattern {
public:
	typedef deTObjectReference<cEditCustomPatternName> Ref;
	cEditCustomPatternName(gdeWPPropertyList &panel) : cBaseTextFieldListenerCustomPattern(panel){}
	
	igdeUndo::Ref OnChangedPattern(igdeTextField *textField, gdeProperty *property, gdeFilePattern *filePattern) override{
		if(textField->GetText() == filePattern->GetName()){
			return {};
		}
		return pPanel.UndoCustomFilePatternName(property, filePattern, textField->GetText());
	}
};

class cEditCustomPatternPattern : public cBaseTextFieldListenerCustomPattern {
public:
	typedef deTObjectReference<cEditCustomPatternPattern> Ref;
	cEditCustomPatternPattern(gdeWPPropertyList &panel) : cBaseTextFieldListenerCustomPattern(panel){}
	
	igdeUndo::Ref OnChangedPattern(igdeTextField *textField, gdeProperty *property, gdeFilePattern *filePattern) override{
		if(textField->GetText() == filePattern->GetPattern()){
			return {};
		}
		return pPanel.UndoCustomFilePatternPattern(property, filePattern, textField->GetText());
	}
};

class cEditCustomPatternExtension : public cBaseTextFieldListenerCustomPattern {
public:
	typedef deTObjectReference<cEditCustomPatternExtension> Ref;
	cEditCustomPatternExtension(gdeWPPropertyList &panel) : cBaseTextFieldListenerCustomPattern(panel){}
	
	igdeUndo::Ref OnChangedPattern(igdeTextField *textField, gdeProperty *property, gdeFilePattern *filePattern) override{
		if(textField->GetText() == filePattern->GetDefaultExtension()){
			return {};
		}
		return pPanel.UndoCustomFilePatternExtension(property, filePattern, textField->GetText());
	}
};


class cComboIdentifierGroup : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboIdentifierGroup> Ref;
	cComboIdentifierGroup(gdeWPPropertyList &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, gdeProperty *property) override{
		if(comboBox->GetText() == property->GetIdentifierGroup()){
			return {};
		}
		return pPanel.UndoIdentifierGroup(property, comboBox->GetText());
	}
};

class cActionIdentifierUsage : public cBaseAction{
public:
	typedef deTObjectReference<cActionIdentifierUsage> Ref;
	cActionIdentifierUsage(gdeWPPropertyList &panel) : cBaseAction(panel,
		"Defines Identifier", nullptr, "Property defines identifier"){}
	
	igdeUndo::Ref OnActionUndo(gdeProperty *property) override{
		return pPanel.UndoIdentifierUsage(property);
	}
	
	void Update() override{/* empty on purpose!*/}
};

}



// Class gdeWPPropertyList
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPPropertyList::gdeWPPropertyList(igdeEnvironment &environment) :
igdeContainerFlow(environment, igdeContainerFlow::eaY),
pPropertyList(nullptr),
pGameDefinition(nullptr),
pClipboard(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref form, frameLine, group;
	
	
	pIconIdentifierUsage = env.GetStockIcon(igdeEnvironment::esiSmallPlus);
	pIconIdentifierNoUsage = env.GetStockIcon(igdeEnvironment::esiSmallMinus);
	
	
	pActionPropertiesMenu = cActionPropertyMenu::Ref::New(*this);
	pActionPropertyAdd = cActionPropertyAdd::Ref::New(*this);
	pActionPropertyRemove = cActionPropertyRemove::Ref::New(*this);
	pActionPropertyCopy = cActionPropertyCopy::Ref::New(*this);
	pActionPropertyCut = cActionPropertyCut::Ref::New(*this);
	pActionPropertyPaste = cActionPropertyPaste::Ref::New(*this);
	
	pActionOptionAdd = cActionOptionAdd::Ref::New(*this);
	pActionOptionRemove = cActionOptionRemove::Ref::New(*this);
	
	pActionCustomPatternAdd = cActionCustomPatternAdd::Ref::New(*this);
	pActionCustomPatternRemove = cActionCustomPatternRemove::Ref::New(*this);
	
	pActionCustomPatternMenu = cActionCustomPatternMenu::Ref::New(*this);
	
	form = igdeContainerForm::Ref::New(env);
	AddChild(form);
	
	
	helper.FormLineStretchFirst(form, "Property:", "Property to edit", frameLine);
	helper.ComboBox(frameLine, "Property to edit", pCBProperties, cComboProperty::Ref::New(*this));
	pCBProperties->SetDefaultSorter();
	helper.Button(frameLine, pBtnMenuProperties, pActionPropertiesMenu);
	pActionPropertiesMenu->SetWidget(pBtnMenuProperties);
	
	helper.EditString(form, "Name:", "Property name", pEditName, cEditName::Ref::New(*this));
	helper.EditString(form, "Description:", "Property description", pEditDescription,
		15, 3, cEditDescription::Ref::New(*this));
	
	helper.ComboBox(form, "Type:", "Property type", pCBType, cComboType::Ref::New(*this));
	pCBType->AddItem("String", nullptr, (void*)(intptr_t)gdeProperty::eptString);
	pCBType->AddItem("Integer", nullptr, (void*)(intptr_t)gdeProperty::eptInteger);
	pCBType->AddItem("Point2", nullptr, (void*)(intptr_t)gdeProperty::eptPoint2);
	pCBType->AddItem("Point3", nullptr, (void*)(intptr_t)gdeProperty::eptPoint3);
	pCBType->AddItem("Float", nullptr, (void*)(intptr_t)gdeProperty::eptFloat);
	pCBType->AddItem("Vector2", nullptr, (void*)(intptr_t)gdeProperty::eptVector2);
	pCBType->AddItem("Vector3", nullptr, (void*)(intptr_t)gdeProperty::eptVector3);
	pCBType->AddItem("Color", nullptr, (void*)(intptr_t)gdeProperty::eptColor);
	pCBType->AddItem("Boolean", nullptr, (void*)(intptr_t)gdeProperty::eptBoolean);
	pCBType->AddItem("Path", nullptr, (void*)(intptr_t)gdeProperty::eptPath);
	pCBType->AddItem("Range", nullptr, (void*)(intptr_t)gdeProperty::eptRange);
	pCBType->AddItem("Select", nullptr, (void*)(intptr_t)gdeProperty::eptSelect);
	pCBType->AddItem("List", nullptr, (void*)(intptr_t)gdeProperty::eptList);
	pCBType->AddItem("Trigger expression", nullptr, (void*)(intptr_t)gdeProperty::eptTriggerExpression);
	pCBType->AddItem("Trigger target", nullptr, (void*)(intptr_t)gdeProperty::eptTriggerTarget);
	pCBType->AddItem("Shape", nullptr, (void*)(intptr_t)gdeProperty::eptShape);
	pCBType->AddItem("Shape list", nullptr, (void*)(intptr_t)gdeProperty::eptShapeList);
	pCBType->AddItem("Identifier", nullptr, (void*)(intptr_t)gdeProperty::eptIdentifier);
	
	gdeDefaultPropertyValue::CreateAndAdd(form, helper, "Default:", "Default property value",
		pEditDefault, cEditDefault::Ref::New(*this));
	
	
	// type specific parameters
	helper.GroupBoxFlow(*this, group, "Parameters:", false, true);
	pSwiParameters = igdeSwitcher::Ref::New(env);
	group->AddChild(pSwiParameters);
	
	
	// no additional parameters
	helper.Label(pSwiParameters, "< No Parameters >");
	
	
	// range
	form = igdeContainerForm::Ref::New(env);
	pSwiParameters->AddChild(form);
	
	helper.EditFloat(form, "Minimum:", "Minimum value for range type property",
		pEditMinimum, cEditMinimum::Ref::New(*this));
	helper.EditFloat(form, "Maximum:", "Maximum value for range type property",
		pEditMaximum, cEditMaximum::Ref::New(*this));
	
	
	// selection
	helper.ListBox(pSwiParameters, 6, "Allowed options", pListOptions, cListOptions::Ref::New(*this));
	
	
	// path
	form = igdeContainerForm::Ref::New(env);
	pSwiParameters->AddChild(form);
	
	helper.ComboBox(form, "Pattern type:", "Path pattern type",
		pCBPathPatternType, cComboPathPatternType::Ref::New(*this));
	pCBPathPatternType->AddItem("All files", nullptr, (void*)(intptr_t)gdeProperty::epptAll);
	pCBPathPatternType->AddItem("Model resources", nullptr, (void*)(intptr_t)gdeProperty::epptModel);
	pCBPathPatternType->AddItem("Skin resources", nullptr, (void*)(intptr_t)gdeProperty::epptSkin);
	pCBPathPatternType->AddItem("Rig resources", nullptr, (void*)(intptr_t)gdeProperty::epptRig);
	pCBPathPatternType->AddItem("Animation resources", nullptr, (void*)(intptr_t)gdeProperty::epptAnimation);
	pCBPathPatternType->AddItem("Animator resources", nullptr, (void*)(intptr_t)gdeProperty::epptAnimator);
	pCBPathPatternType->AddItem("Image resources", nullptr, (void*)(intptr_t)gdeProperty::epptImage);
	pCBPathPatternType->AddItem("Occlusion mesh resources", nullptr, (void*)(intptr_t)gdeProperty::epptOcclusionMesh);
	pCBPathPatternType->AddItem("Navigation space resources", nullptr, (void*)(intptr_t)gdeProperty::epptNavigationSpace);
	pCBPathPatternType->AddItem("Particle emitter resources", nullptr, (void*)(intptr_t)gdeProperty::epptParticleEmitter);
	pCBPathPatternType->AddItem("Sound resources", nullptr, (void*)(intptr_t)gdeProperty::epptSound);
	pCBPathPatternType->AddItem("Synthesizer resources", nullptr, (void*)(intptr_t)gdeProperty::epptSynthesizer);
	pCBPathPatternType->AddItem("Video resources", nullptr, (void*)(intptr_t)gdeProperty::epptVideo);
	pCBPathPatternType->AddItem("Font resources", nullptr, (void*)(intptr_t)gdeProperty::epptFont);
	pCBPathPatternType->AddItem("Sky resources", nullptr, (void*)(intptr_t)gdeProperty::epptSky);
	pCBPathPatternType->AddItem("Camera resources", nullptr, (void*)(intptr_t)gdeProperty::epptCamera);
	pCBPathPatternType->AddItem("World resources", nullptr, (void*)(intptr_t)gdeProperty::epptWorld);
	pCBPathPatternType->AddItem("Custom file pattern", nullptr, (void*)(intptr_t)gdeProperty::epptCustom);
	
	helper.FormLineStretchFirst(form, "Custom pattern:", "Custom pattern to edit.", frameLine);
	helper.ComboBox(frameLine, "Custom pattern to edit.", pCBCustomPattern, cComboCustomPattern::Ref::New(*this));
	pCBCustomPattern->SetDefaultSorter();
	helper.Button(frameLine, pBtnCustomPatternMenu, pActionCustomPatternMenu);
	pActionCustomPatternMenu->SetWidget(pBtnCustomPatternMenu);
	
	helper.EditString(form, "Name:", "Name of pattern in file dialogs",
		pCustomPatternEditName, cEditCustomPatternName::Ref::New(*this));
	helper.EditString(form, "Pattern:", "Pattern",
		pCustomPatternEditPattern, cEditCustomPatternPattern::Ref::New(*this));
	helper.EditString(form, "Extension:", "Extension to use for saving files",
		pCustomPatternEditExtension, cEditCustomPatternExtension::Ref::New(*this));
	
	
	// identifier
	form = igdeContainerForm::Ref::New(env);
	pSwiParameters->AddChild(form);
	
	helper.ComboBox(form, "Group:", true, "Identifier group name", pCBIdentifierGroup, cComboIdentifierGroup::Ref::New(*this));
	pCBIdentifierGroup->SetDefaultSorter();
	
	helper.CheckBox(form, pChkIdentifierUsage, cActionIdentifierUsage::Ref::New(*this));
}

gdeWPPropertyList::~gdeWPPropertyList(){
}



// Management
///////////////

void gdeWPPropertyList::SetPropertyList(const gdeProperty::List *propertyList){
	if(propertyList == pPropertyList){
		return;
	}
	
	pPropertyList = propertyList;
	
	UpdateList();
}

void gdeWPPropertyList::SetGameDefinition(gdeGameDefinition *gameDefinition){
	pGameDefinition = gameDefinition;
}

igdeUndoSystem *gdeWPPropertyList::GetUndoSystem() const{
	return pGameDefinition ? pGameDefinition->GetUndoSystem() : nullptr;
}

void gdeWPPropertyList::SetClipboard(igdeClipboard *clipboard){
	pClipboard = clipboard;
}



gdeProperty *gdeWPPropertyList::GetProperty() const{
	if(!pPropertyList){
		return {};
	}
	
	const igdeListItem * const selection = pCBProperties->GetSelectedItem();
	return selection ? (gdeProperty*)selection->GetData() : nullptr;
}

decString gdeWPPropertyList::GetOption() const{
	const gdeProperty * const property = GetProperty();
	return property && pListOptions->GetSelectedItem()
		? pListOptions->GetSelectedItem()->GetText() : decString();
}

int gdeWPPropertyList::GetCustomPatternIndex() const{
	const gdeProperty * const property = GetProperty();
	return property ? pCBCustomPattern->GetSelection() : -1;
}

gdeFilePattern *gdeWPPropertyList::GetCustomPattern() const{
	const gdeProperty * const property = GetProperty();
	return property && pCBCustomPattern->GetSelectedItem() ?
		(gdeFilePattern*)pCBCustomPattern->GetSelectedItem()->GetData() : nullptr;
}



void gdeWPPropertyList::UpdateList(){
	gdeProperty * const property = GetProperty();
	
	pCBProperties->RemoveAllItems();
	
	if(pPropertyList){
		const int count = pPropertyList->GetCount();
		int i;
		
		for(i=0; i<count; i++){
			gdeProperty * const property2 = pPropertyList->GetAt(i);
			pCBProperties->AddItem(property2->GetName(), nullptr, property2);
		}
		
		pCBProperties->SortItems();
	}
	
	pCBProperties->SetSelectionWithData(property);
	if(pCBProperties->GetSelection() == -1 && pCBProperties->GetItems().IsNotEmpty()){
		pCBProperties->SetSelection(0);
	}
	
	UpdateProperty();
}

void gdeWPPropertyList::UpdateProperty(){
	const gdeProperty * const property = GetProperty();
	
	if(property){
		pEditName->SetText(property->GetName());
		pEditDescription->SetText(property->GetDescription());
		pCBType->SetSelectionWithData((void*)(intptr_t)property->GetType());
		pEditMinimum->SetFloat(property->GetMinimumValue());
		pEditMaximum->SetFloat(property->GetMaximumValue());
		pEditDefault->SetValue(property->GetDefaultValue(), *property);
		
		const decString selectedOption(pListOptions->GetSelectedItem()
			? pListOptions->GetSelectedItem()->GetText() : decString());
		const decStringList &options = property->GetOptions();
		const int optionCount = options.GetCount();
		int i;
		pListOptions->RemoveAllItems();
		for(i=0; i<optionCount; i++){
			pListOptions->AddItem(options.GetAt(i));
		}
		pListOptions->SetSelection(pListOptions->IndexOfItem(selectedOption));
		
		pCBPathPatternType->SetSelectionWithData((void*)(intptr_t)property->GetPathPatternType());
		
		UpdatePropertyIdentifierList();
		pCBIdentifierGroup->SetText(property->GetIdentifierGroup());
		
		pChkIdentifierUsage->SetChecked(property->GetIdentifierUsage());
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pCBType->SetSelectionWithData((void*)(intptr_t)gdeProperty::eptString);
		pEditMinimum->ClearText();
		pEditMaximum->ClearText();
		pEditDefault->ClearValue();
		pListOptions->RemoveAllItems();
		pCBPathPatternType->SetSelectionWithData((void*)(intptr_t)gdeProperty::epptAll);
		pCBIdentifierGroup->RemoveAllItems();
		pCBIdentifierGroup->ClearText();
		pChkIdentifierUsage->SetChecked(false);
	}
	
	UpdateCustomPatternList();
	UpdateEnabled();
}

void gdeWPPropertyList::UpdatePropertyIdentifierList(){
	pCBIdentifierGroup->RemoveAllItems();
	
	if(pGameDefinition){
		const decStringSet &definedIDs = pGameDefinition->GetDefinedIDs();
		int i, count = definedIDs.GetCount();
		for(i=0; i<count; i++){
			pCBIdentifierGroup->AddItem(definedIDs.GetAt(i), pIconIdentifierNoUsage);
		}
		
		const decStringSet &usedIDs = pGameDefinition->GetUsedIDs();
		count = usedIDs.GetCount();
		for(i=0; i<count; i++){
			const decString &id = usedIDs.GetAt(i);
			const int index = pCBIdentifierGroup->IndexOfItem(id);
			if(index == -1){
				pCBIdentifierGroup->AddItem(id, pIconIdentifierUsage);
				
			}else{
				igdeListItem &item = pCBIdentifierGroup->GetItems().GetAt(index);
				if(item.GetIcon() == pIconIdentifierNoUsage){
					item.SetIcon(pIconIdentifierUsage);
					pCBIdentifierGroup->ItemChangedAt(index);
				}
			}
		}
	}
	
	if(pPropertyList){
		const int propertyCount = pPropertyList->GetCount();
		int i;
		
		for(i=0; i<propertyCount; i++){
			const gdeProperty &property = *pPropertyList->GetAt(i);
			if(property.GetType() != gdeProperty::eptIdentifier){
				continue;
			}
			
			const decString &group = property.GetIdentifierGroup();
			if(group.IsEmpty()){
				continue;
			}
			
			const int index = pCBIdentifierGroup->IndexOfItem(group);
			if(index == -1){
				pCBIdentifierGroup->AddItem(group, property.GetIdentifierUsage()
					? pIconIdentifierUsage : pIconIdentifierNoUsage);
				
			}else if(property.GetIdentifierUsage()){
				igdeListItem &item = pCBIdentifierGroup->GetItems().GetAt(index);
				if(item.GetIcon() == pIconIdentifierNoUsage){
					item.SetIcon(pIconIdentifierUsage);
					pCBIdentifierGroup->ItemChangedAt(index);
				}
			}
		}
	}
}

void gdeWPPropertyList::UpdateCustomPatternList(){
	const gdeProperty * const property = GetProperty();
	
	if(property){
		gdeFilePattern * const selection = pCBCustomPattern->GetSelectedItem()
			? (gdeFilePattern*)pCBCustomPattern->GetSelectedItem()->GetData() : nullptr;
		
		const gdeFilePattern::List &list = property->GetCustomPathPattern();
		const int count = list.GetCount();
		int i;
		
		pCBCustomPattern->RemoveAllItems();
		for(i=0; i<count; i++){
			gdeFilePattern * const pattern = list.GetAt(i);
			pCBCustomPattern->AddItem(pattern->GetName(), nullptr, pattern);
		}
		pCBCustomPattern->SortItems();
		pCBCustomPattern->SetSelectionWithData(selection);
		
	}else{
		pCBCustomPattern->RemoveAllItems();
	}
	
	UpdateCustomPattern();
}

void gdeWPPropertyList::UpdateCustomPattern(){
	const gdeFilePattern * const filePattern = GetCustomPattern();
	
	if(filePattern){
		pCustomPatternEditName->SetText(filePattern->GetName());
		pCustomPatternEditPattern->SetText(filePattern->GetPattern());
		pCustomPatternEditExtension->SetText(filePattern->GetDefaultExtension());
		
	}else{
		pCustomPatternEditName->ClearText();
		pCustomPatternEditPattern->ClearText();
		pCustomPatternEditExtension->ClearText();
	}
}

void gdeWPPropertyList::UpdateEnabled(){
	const gdeProperty * const property = GetProperty();
	
	const bool hasList = pPropertyList;
	const bool hasProperty = property;
	
	pCBProperties->SetEnabled(hasList);
	pBtnMenuProperties->SetEnabled(hasList);
	pEditName->SetEnabled(hasProperty);
	pEditDescription->SetEnabled(hasProperty);
	pCBType->SetEnabled(hasProperty);
	
	pEditDefault->SetEnabled(hasProperty);
	
	pEditMinimum->SetEnabled(hasProperty);
	pEditMaximum->SetEnabled(hasProperty);
	
	pListOptions->SetEnabled(hasProperty);
	
	const bool hasCustomPattern = property && property->GetPathPatternType() == gdeProperty::epptCustom;
	pCBPathPatternType->SetEnabled(hasProperty);
	pCBCustomPattern->SetEnabled(hasCustomPattern);
	pBtnCustomPatternMenu->SetEnabled(hasCustomPattern);
	pCustomPatternEditName->SetEnabled(hasCustomPattern);
	pCustomPatternEditPattern->SetEnabled(hasCustomPattern);
	pCustomPatternEditExtension->SetEnabled(hasCustomPattern);
	
	pCBIdentifierGroup->SetEnabled(hasProperty);
	pChkIdentifierUsage->SetEnabled(hasProperty);
	
	if(property){
		switch(property->GetType()){
		case gdeProperty::eptRange:
			pSwiParameters->SetCurrent(1);  // range
			break;
			
		case gdeProperty::eptSelect:
			pSwiParameters->SetCurrent(2);  // select
			break;
			
		case gdeProperty::eptPath:
			pSwiParameters->SetCurrent(3);  // path
			break;
			
		case gdeProperty::eptIdentifier:
			pSwiParameters->SetCurrent(4);  // identifier
			break;
			
		default:
			pSwiParameters->SetCurrent(0);  // nothing
		}
		
	}else{
		pSwiParameters->SetCurrent(0);  // nothing
	}
}

void gdeWPPropertyList::SelectProperty(gdeProperty *property){
	pCBProperties->SetSelectionWithData(property);
	UpdateProperty();
}

void gdeWPPropertyList::SelectCustomPattern(gdeFilePattern *filePattern){
	pCBCustomPattern->SetSelectionWithData(filePattern);
	UpdateCustomPattern();
}

void gdeWPPropertyList::SetDefaultValueFromType(){
	gdeProperty * const property = GetProperty();
	if(!pGameDefinition || !property){
		return;
	}
	
	decString value;
	
	switch((gdeProperty::ePropertyTypes)(intptr_t)pCBType->GetSelectedItem()->GetData()){
	case gdeProperty::eptInteger:
	case gdeProperty::eptFloat:
	case gdeProperty::eptBoolean:
		value = "0";
		break;
		
	case gdeProperty::eptPoint2:
	case gdeProperty::eptVector2:
		value = "0 0";
		break;
		
	case gdeProperty::eptPoint3:
	case gdeProperty::eptVector3:
	case gdeProperty::eptColor:
		value = "0 0 0";
		break;
		
	case gdeProperty::eptRange:
		pEditMinimum->GetText();
		break;
		
	case gdeProperty::eptSelect:
		if(pListOptions->GetItems().IsNotEmpty()){
			value = pListOptions->GetItems().First()->GetText();
		}
		break;
		
	default:
		break;
	}
	
	if(value == pEditDefault->GetValue()){
		return;
	}
	
	pGameDefinition->GetUndoSystem()->Add(
		 UndoDefaultValue(property, value, pEditDefault->GetValue()));
}
