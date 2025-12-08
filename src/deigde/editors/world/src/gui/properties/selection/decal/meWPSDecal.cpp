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

#include "meWPSDecal.h"
#include "meWPSDecalListener.h"
#include "../meWPSelection.h"
#include "../../meWindowProperties.h"
#include "../../meWPPropertyList.h"
#include "../../../meWindowMain.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/idgroup/meIDGroup.h"
#include "../../../../world/decal/meDecal.h"
#include "../../../../world/decal/meDecalSelection.h"
#include "../../../../undosys/properties/decal/meUDecalPosition.h"
#include "../../../../undosys/properties/decal/meUDecalRotation.h"
#include "../../../../undosys/properties/decal/meUDecalSize.h"
#include "../../../../undosys/properties/decal/meUDecalSkin.h"
#include "../../../../undosys/properties/decal/meUDecalTCOffset.h"
#include "../../../../undosys/properties/decal/meUDecalTCScaling.h"
#include "../../../../undosys/properties/decal/meUDecalTCRotation.h"
#include "../../../../undosys/properties/decal/meUDecalColorTint.h"
#include "../../../../undosys/properties/decal/meUDecalVisible.h"
#include "../../../../undosys/properties/decal/property/meUDecalAddProperty.h"
#include "../../../../undosys/properties/decal/property/meUDecalSetProperty.h"
#include "../../../../undosys/properties/decal/property/meUDecalRemoveProperty.h"
#include "../../../../undosys/properties/decal/property/meUDecalPropertyCopyToSelected.h"
#include "../../../../undosys/properties/decal/property/meUDecalPropertyRemoveFromSelected.h"
#include "../../../../undosys/properties/decal/property/meUDecalClonePropertiesToSelected.h"
#include "../../../../undosys/properties/decal/property/meUDecalSetProperties.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>



// Actions
///////////

static const decString vEmptyString;

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	meWPSDecal &pPanel;
	
public:
	cBaseTextFieldListener(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, decal)));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meDecal *decal) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPSDecal &pPanel;
	
public:
	cBaseAction(meWPSDecal &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(decal)));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(meDecal *decal) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	meWPSDecal &pPanel;
	
public:
	cBaseEditVectorListener(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector->GetVector(), decal)));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, meDecal *decal) = 0;
};

class cBaseEditDVectorListener : public igdeEditDVectorListener{
protected:
	meWPSDecal &pPanel;
	
public:
	cBaseEditDVectorListener(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnDVectorChanged(igdeEditDVector *editDVector){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editDVector->GetDVector(), decal)));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decDVector &vector, meDecal *decal) = 0;
};

class cBaseEditVector2Listener : public igdeEditVector2Listener{
protected:
	meWPSDecal &pPanel;
	
public:
	cBaseEditVector2Listener(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector2->GetVector2(), decal)));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, meDecal *decal) = 0;
};



class cSpinActive : public igdeSpinTextFieldListener{
	meWPSDecal &pPanel;
	
public:
	cSpinActive(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		if(!pPanel.GetWorld()){
			return;
		}
		
		meDecalSelection &selection = pPanel.GetWorld()->GetSelectionDecal();
		const int index = textField->GetValue();
		
		selection.SetActive(index >= 0 && index <= selection.GetSelected().GetCount()
			? selection.GetSelected().GetAt(index) : NULL);
		
		pPanel.GetWorld()->NotifyDecalSelectionChanged();
	}
};

class cEditPosition : public cBaseEditDVectorListener{
public:
	cEditPosition(meWPSDecal &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decDVector &vector, meDecal *decal){
		return !decal->GetPosition().IsEqualTo(vector) ? new meUDecalPosition(decal, vector) : NULL;
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	cEditRotation(meWPSDecal &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, meDecal *decal){
		return !decal->GetRotation().IsEqualTo(vector) ? new meUDecalRotation(decal, vector) : NULL;
	}
};

class cEditSize : public cBaseEditVectorListener{
public:
	cEditSize(meWPSDecal &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, meDecal *decal){
		return !decal->GetSize().IsEqualTo(vector) ? new meUDecalSize(decal, vector) : NULL;
	}
};

class cActionVisible : public cBaseAction{
public:
	cActionVisible(meWPSDecal &panel) : cBaseAction(panel, "Visible", NULL, "Decal is initially visible"){}
	
	igdeUndo *OnAction(meDecal *decal) override{
		return new meUDecalVisible(decal);
	}
};

class cEditOrder : public cBaseTextFieldListener{
public:
	cEditOrder(meWPSDecal &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meDecal *decal){
		// TODO
		return NULL; //decal->GetOrder() != textField->GetInteger() ? new meUDecalOrder(decal, textField->GetInteger()) : NULL;
	}
};

class cEditSkin : public igdeEditPathListener{
	meWPSDecal &pPanel;
	
public:
	cEditSkin(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal || decal->GetSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(
			meUDecalSkin::Ref::NewWith(decal, editPath->GetPath()));
	}
};

class cEditTexCoordOffset : public cBaseEditVector2Listener{
public:
	cEditTexCoordOffset(meWPSDecal &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, meDecal *decal){
		return !decal->GetTexCoordOffset().IsEqualTo(vector)
			? new meUDecalTCOffset(decal, vector) : NULL;
	}
};

class cEditTexCoordScaling : public cBaseEditVector2Listener{
public:
	cEditTexCoordScaling(meWPSDecal &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, meDecal *decal){
		return !decal->GetTexCoordScaling().IsEqualTo(vector)
			? new meUDecalTCScaling(decal, vector) : NULL;
	}
};

class cEditTexCoordRotation : public cBaseTextFieldListener{
public:
	cEditTexCoordRotation(meWPSDecal &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meDecal *decal){
		const float value = textField->GetFloat();
		return fabsf(decal->GetTexCoordRotation() - value) > FLOAT_SAFE_EPSILON
			? new meUDecalTCRotation(decal, value) : NULL;
	}
};

class cEditColorTint : public igdeColorBoxListener{
	meWPSDecal &pPanel;
	
public:
	cEditColorTint(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal || decal->GetColorTint().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(
			meUDecalColorTint::Ref::NewWith(decal, colorBox->GetColor()));
	}
};


class cEditProperties : public meWPPropertyList {
	meWPSDecal &pPanel;
	
public:
	cEditProperties(meWPSDecal &panel) : meWPPropertyList(panel.GetEnvironment()), pPanel(panel){}
	
	virtual decString GetGDDefaultValue(const char *key) const{
		const igdeGDProperty * const gdProperty = GetGDProperty(key);
		return gdProperty ? gdProperty->GetDefaultValue() : vEmptyString;
	}
	
	const igdeGDProperty *GetGDProperty(const char *key) const override{
		return pPanel.GetWorld() ? pPanel.GetWorld()->GetGameDefinition()->GetListDecalProperties().GetNamed(key) : NULL;
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		decStringSet keys;
		if(pPanel.GetWorld()){
			const igdeGDPropertyList &list = pPanel.GetWorld()->GetGameDefinition()->GetListDecalProperties();
			const int count = list.GetCount();
			int i;
			for(i=0; i<count; i++){
				keys.Add(list.GetAt(i)->GetName());
			}
		}
		return keys;
	}
	
	void AddContextMenuEntries(igdeUIHelper &helper, igdeMenuCascade &menu) override{
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionPropCopyToSel());
		helper.MenuCommand(menu, pPanel.GetActionPropCloneToSel());
		helper.MenuCommand(menu, pPanel.GetActionPropRemoveFromSel());
	}
	
	virtual igdeUndo *UndoAddProperty(const decString &key, const decString &value){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? new meUDecalAddProperty(decal, key, value) : NULL;
	}
	
	virtual igdeUndo *UndoRemoveProperty(const decString &key){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? new meUDecalRemoveProperty(decal, key, decal->GetProperties().GetAt(key)) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperty(const decString &key, const decString &oldValue, const decString &newValue){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? new meUDecalSetProperty(decal, key, oldValue, newValue) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperties(const decStringDictionary &properties){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? new meUDecalSetProperties(decal, properties) : NULL;
	}
};

class cActionPropCopyToSel : public cBaseAction{
public:
	cActionPropCopyToSel(meWPSDecal &panel) : cBaseAction(panel, "Copy To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Copy property from active decal to all selected decals"){}
	
	igdeUndo *OnAction(meDecal *decal) override{
		meDecalList list(pPanel.GetWorld()->GetSelectionDecal().GetSelected());
		const decString &property = pPanel.GetActiveProperty();
		list.RemoveIfPresent(decal);
		return !property.IsEmpty() && list.GetCount() > 0 ? new meUDecalPropertyCopyToSelected(
			list, property, decal->GetProperties().GetAt(property)) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActiveDecal() && !pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionDecal().GetSelected().GetCount() > 1);
	}
};

class cActionPropRemoveFromSel : public cBaseAction{
public:
	cActionPropRemoveFromSel(meWPSDecal &panel) : cBaseAction(panel, "Remove From Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Remove property from all selected decals"){}
	
	igdeUndo *OnAction(meDecal*) override{
		const meDecalList &list = pPanel.GetWorld()->GetSelectionDecal().GetSelected();
		const decString &property = pPanel.GetActiveProperty();
		return !property.IsEmpty() && list.GetCount() > 0 ?
			new meUDecalPropertyRemoveFromSelected(list, property) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActiveDecal() && !pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionDecal().GetSelected().GetCount() > 0);
	}
};

class cActionPropCloneToSel : public cBaseAction{
public:
	cActionPropCloneToSel(meWPSDecal &panel) : cBaseAction(panel, "Clone To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Clone all properties from active decal to all selected decals"){}
	
	igdeUndo *OnAction(meDecal *decal) override{
		meDecalList list(pPanel.GetWorld()->GetSelectionDecal().GetSelected());
		list.RemoveIfPresent(decal);
		return list.GetCount() > 0 ? new meUDecalClonePropertiesToSelected(list, decal->GetProperties()) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActiveDecal() && pPanel.GetWorld()->GetSelectionDecal().GetSelected().GetCount() > 1);
	}
};

}



// Class meWPSDecal
/////////////////////

// Constructor, destructor
////////////////////////////

meWPSDecal::meWPSDecal(meWPSelection &wpselection) :
igdeContainerScroll(wpselection.GetEnvironment(), false, true),
pWPSelection(wpselection)
{
	igdeEnvironment &env = wpselection.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer *content, groupBox, formLine;
	
	pListener.TakeOver(new meWPSDecalListener(*this));
	
	pActionPropCopyToSel.TakeOver(new cActionPropCopyToSel(*this));
	pActionPropRemoveFromSel.TakeOver(new cActionPropRemoveFromSel(*this));
	pActionPropCloneToSel.TakeOver(new cActionPropCloneToSel(*this));
	
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	groupBox.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 10));
	content->AddChild(groupBox);
	helper.Label(groupBox, "Selected:");
	helper.EditInteger(groupBox, "Number of selected decals", 3, pEditSelCount, NULL);
	pEditSelCount->SetEditable(false);
	helper.Label(groupBox, "Active:");
	helper.EditSpinInteger(groupBox, "Active decal to edit", 0, 0, pSpinActive, new cSpinActive(*this));
	
	
	// geometry
	helper.GroupBox(content, groupBox, "Geometry:");
	
	helper.EditString(groupBox, "ID:", "Unique decal ID", pEditID, NULL);
	pEditID->SetEditable(false);
	
	helper.EditDVector(groupBox, "Position:", "Position of decal.", pEditPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation of decal.", pEditRotation, new cEditRotation(*this));
	helper.EditVector(groupBox, "Size:", "Size of decal.", pEditSize, new cEditSize(*this));
	helper.EditInteger(groupBox, "Order:", "Drawing order of decal.", pEditOrder, new cEditOrder(*this));
	helper.CheckBox(groupBox, pChkVisible, new cActionVisible(*this), true);
	
	
	// texture
	helper.GroupBox(content, groupBox, "Texture:");
	
	helper.EditPath(groupBox, "Skin:", "Skin to use for decal",
		igdeEnvironment::efpltSkin, pEditSkin, new cEditSkin(*this));
	
	
	// texture transform
	helper.GroupBox(content, groupBox, "Texture Transformation:", true);
	
	helper.EditVector2(groupBox, "Translation:", "Texture coordinates translation.",
		pEditTexCoordOffset, new cEditTexCoordOffset(*this));
	helper.EditVector2(groupBox, "Scaling:", "Texture coordinates scaling.",
		pEditTexCoordScaling, new cEditTexCoordScaling(*this));
	helper.EditFloat(groupBox, "Rotation:", "Texture coordinates rotation.",
		pEditTexCoordRotation, new cEditTexCoordRotation(*this));
	helper.ColorBox(groupBox, "Tint:", "Color tint.", pClrColorTint, new cEditColorTint(*this));
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "Properties:");
	
	pEditProperties.TakeOver(new cEditProperties(*this));
	groupBox->AddChild(pEditProperties);
}

meWPSDecal::~meWPSDecal(){
	SetWorld(NULL);
}



// Management
///////////////

void meWPSDecal::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	meWPPropertyList &editProperties = (meWPPropertyList&)(igdeWidget&)pEditProperties;
	
	if(pWorld){
		editProperties.SetClipboard(NULL);
		editProperties.SetUndoSystem(NULL);
		editProperties.SetTriggerTargetList(NULL);
		editProperties.SetIdentifiers(decStringSet());
		
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		editProperties.SetClipboard(&pWPSelection.GetWindowProperties().GetWindowMain().GetClipboard());
		editProperties.SetUndoSystem(world->GetUndoSystem());
		editProperties.SetTriggerTargetList(&world->GetTriggerTable());
	}
	
	UpdateSelection();
	UpdateEnabled();
	UpdateDecal();
	UpdateGeometry();
	UpdateTexture();
	UpdatePropertyKeys();
	UpdateProperties();
}



meDecal *meWPSDecal::GetActiveDecal() const{
	return pWorld ? pWorld->GetSelectionDecal().GetActive() : NULL;
}

const decString &meWPSDecal::GetActiveProperty() const{
	const meDecal * const decal = GetActiveDecal();
	return decal ? decal->GetActiveProperty() : vEmptyString;
}



void meWPSDecal::UpdateSelection(){
	int active = 0;
	int count = 0;
	
	if(pWorld){
		const meDecalSelection &selection = pWorld->GetSelectionDecal();
		count = selection.GetSelected().GetCount();
		if(selection.HasActive()){
			active = selection.GetSelected().IndexOf(selection.GetActive());
		}
	}
	
	pEditSelCount->SetInteger(count);
	pSpinActive->SetRange(0, count - 1);
	pSpinActive->SetValue(active);
	pSpinActive->SetEnabled(count > 0);
}

void meWPSDecal::UpdateEnabled(){
	const bool enabled = GetActiveDecal() != NULL;
	
	pEditPosition->SetEnabled(enabled);
	pEditSize->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditOrder->SetEnabled(enabled);
	pChkVisible->SetEnabled(enabled);
		
	pEditSkin->SetEnabled(enabled);
	pEditTexCoordOffset->SetEnabled(enabled);
	pEditTexCoordScaling->SetEnabled(enabled);
	pEditTexCoordRotation->SetEnabled(enabled);
	pClrColorTint->SetEnabled(enabled);
}

void meWPSDecal::UpdateDecal(){
	const meDecal * const decal = GetActiveDecal();
	
	if(decal){
		//pEditOrder->SetInteger( decal->GetOrder() );
		pEditOrder->SetInteger(0);
		pChkVisible->SetChecked(decal->GetVisible());
		
	}else{
		pEditOrder->ClearText();
		pChkVisible->SetChecked(false);
	}
}

void meWPSDecal::UpdateGeometry(){
	const meDecal * const decal = GetActiveDecal();
	
	if(decal){
		//pEditID->SetText( decal->GetID() ); // not created
		
		pEditPosition->SetDVector(decal->GetPosition());
		pEditSize->SetVector(decal->GetSize());
		pEditRotation->SetVector(decal->GetRotation());
		
	}else{
		pEditPosition->SetDVector(decDVector());
		pEditSize->SetVector(decVector());
		pEditRotation->SetVector(decVector());
	}
}

void meWPSDecal::UpdateTexture(){
	const meDecal * const decal = GetActiveDecal();
	
	if(decal){
		pEditSkin->SetPath(decal->GetSkinPath());
		pEditTexCoordOffset->SetVector2(decal->GetTexCoordOffset());
		pEditTexCoordScaling->SetVector2(decal->GetTexCoordScaling());
		pEditTexCoordRotation->SetFloat(decal->GetTexCoordRotation());
		pClrColorTint->SetColor(decal->GetColorTint());
		
	}else{
		pEditSkin->ClearPath();
		pEditTexCoordOffset->SetVector2(decVector2());
		pEditTexCoordScaling->SetVector2(decVector2(1.0f, 1.0f));
		pEditTexCoordRotation->ClearText();
		pClrColorTint->SetColor(decColor(1.0f, 1.0f, 1.0f));
	}
}

void meWPSDecal::SelectActiveProperty(){
	if(!GetActiveProperty().IsEmpty()){
		((meWPPropertyList&)(igdeWidget&)pEditProperties).SelectProperty(GetActiveProperty());
	}
}

void meWPSDecal::UpdatePropertyKeys(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).UpdateKeys();
}

void meWPSDecal::UpdateProperties(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).SetProperties(
		GetActiveDecal() ? GetActiveDecal()->GetProperties() : decStringDictionary());
	SelectActiveProperty();
}

void meWPSDecal::UpdateIdentifierLists(){
	meWPPropertyList &editProperties = (meWPPropertyList&)(igdeWidget&)pEditProperties;
	const decString &property = GetActiveProperty();
	decStringSet identifiers;
	
	if(pWorld && !property.IsEmpty()){
		const igdeGDProperty * const gdProperty = editProperties.GetGDProperty(property);
		if(gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().GetNamed(gdProperty->GetIdentifierGroup());
			if(idgroup){
				const decStringList &list = idgroup->GetIDList();
				const int count = list.GetCount();
				int i;
				
				for(i=0; i<count; i++){
					identifiers.Add(list.GetAt(i));
				}
			}
		}
	}
	editProperties.SetIdentifiers(identifiers);
}


void meWPSDecal::OnGameDefinitionChanged(){
	UpdateIdentifierLists();
	UpdatePropertyKeys();
	UpdateProperties();
}
