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
		
		igdeUndo::Ref undo(OnChanged(textField, decal));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, meDecal *decal) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPSDecal &pPanel;
	
public:
	cBaseAction(meWPSDecal &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(decal));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(meDecal *decal) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), decal));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, meDecal *decal) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(editDVector->GetDVector(), decal));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decDVector &vector, meDecal *decal) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(editVector2->GetVector2(), decal));
		if(undo){
			decal->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector2 &vector, meDecal *decal) = 0;
};



class cSpinActive : public igdeSpinTextFieldListener{
	meWPSDecal &pPanel;
	
public:
	typedef deTObjectReference<cSpinActive> Ref;
	cSpinActive(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		if(!pPanel.GetWorld()){
			return;
		}
		
		meDecalSelection &selection = pPanel.GetWorld()->GetSelectionDecal();
		const int index = textField->GetValue();
		
		selection.SetActive(index >= 0 && index <= selection.GetSelected().GetCount()
			? selection.GetSelected().GetAt(index) : nullptr);
		
		pPanel.GetWorld()->NotifyDecalSelectionChanged();
	}
};

class cEditPosition : public cBaseEditDVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(meWPSDecal &panel) : cBaseEditDVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decDVector &vector, meDecal *decal) override{
		return !decal->GetPosition().IsEqualTo(vector) ? meUDecalPosition::Ref::New(decal, vector) : meUDecalPosition::Ref();
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(meWPSDecal &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, meDecal *decal) override{
		return !decal->GetRotation().IsEqualTo(vector) ? meUDecalRotation::Ref::New(decal, vector) : meUDecalRotation::Ref();
	}
};

class cEditSize : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditSize> Ref;
	cEditSize(meWPSDecal &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, meDecal *decal) override{
		return !decal->GetSize().IsEqualTo(vector) ? meUDecalSize::Ref::New(decal, vector) : meUDecalSize::Ref();
	}
};

class cActionVisible : public cBaseAction{
public:
	typedef deTObjectReference<cActionVisible> Ref;
	cActionVisible(meWPSDecal &panel) : cBaseAction(panel, "Visible", nullptr, "Decal is initially visible"){}
	
	igdeUndo::Ref OnAction(meDecal *decal) override{
		return meUDecalVisible::Ref::New(decal);
	}
};

class cEditOrder : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditOrder> Ref;
	cEditOrder(meWPSDecal &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meDecal *decal) override{
		// TODO
		return {}; //decal->GetOrder() != textField->GetInteger() ? meUDecalOrder::Ref::New(decal, textField->GetInteger()) : meUDecalOrder::Ref();
	}
};

class cEditSkin : public igdeEditPathListener{
	meWPSDecal &pPanel;
	
public:
	typedef deTObjectReference<cEditSkin> Ref;
	cEditSkin(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal || decal->GetSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(
			meUDecalSkin::Ref::New(decal, editPath->GetPath()));
	}
};

class cEditTexCoordOffset : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditTexCoordOffset> Ref;
	cEditTexCoordOffset(meWPSDecal &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector2 &vector, meDecal *decal) override{
		return !decal->GetTexCoordOffset().IsEqualTo(vector)
			? meUDecalTCOffset::Ref::New(decal, vector) : igdeUndo::Ref();
	}
};

class cEditTexCoordScaling : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditTexCoordScaling> Ref;
	cEditTexCoordScaling(meWPSDecal &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector2 &vector, meDecal *decal) override{
		return !decal->GetTexCoordScaling().IsEqualTo(vector)
			? meUDecalTCScaling::Ref::New(decal, vector) : igdeUndo::Ref();
	}
};

class cEditTexCoordRotation : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditTexCoordRotation> Ref;
	cEditTexCoordRotation(meWPSDecal &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meDecal *decal) override{
		const float value = textField->GetFloat();
		return fabsf(decal->GetTexCoordRotation() - value) > FLOAT_SAFE_EPSILON
			? meUDecalTCRotation::Ref::New(decal, value) : igdeUndo::Ref();
	}
};

class cEditColorTint : public igdeColorBoxListener{
	meWPSDecal &pPanel;
	
public:
	typedef deTObjectReference<cEditColorTint> Ref;
	cEditColorTint(meWPSDecal &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		meDecal * const decal = pPanel.GetActiveDecal();
		if(!decal || decal->GetColorTint().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(
			meUDecalColorTint::Ref::New(decal, colorBox->GetColor()));
	}
};


class cEditProperties : public meWPPropertyList {
	meWPSDecal &pPanel;
	
public:
	typedef deTObjectReference<cEditProperties> Ref;
	cEditProperties(meWPSDecal &panel) : meWPPropertyList(panel.GetEnvironment()), pPanel(panel){}
	
	virtual decString GetGDDefaultValue(const char *key) const{
		const igdeGDProperty * const gdProperty = GetGDProperty(key);
		return gdProperty ? gdProperty->GetDefaultValue() : vEmptyString;
	}
	
	const igdeGDProperty *GetGDProperty(const char *key) const override{
		return pPanel.GetWorld() ? pPanel.GetWorld()->GetGameDefinition()->
			GetListDecalProperties().FindOrDefault([&](const igdeGDProperty &p){
				return p.GetName() == key;
			}) : nullptr;
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		decStringSet keys;
		if(pPanel.GetWorld()){
			pPanel.GetWorld()->GetGameDefinition()->GetListDecalProperties().Visit([&](igdeGDProperty::Ref &prop){
				keys.Add(prop->GetName());
			});
		}
		return keys;
	}
	
	void AddContextMenuEntries(igdeUIHelper &helper, igdeMenuCascade &menu) override{
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionPropCopyToSel());
		helper.MenuCommand(menu, pPanel.GetActionPropCloneToSel());
		helper.MenuCommand(menu, pPanel.GetActionPropRemoveFromSel());
	}
	
	virtual igdeUndo::Ref UndoAddProperty(const decString &key, const decString &value){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? meUDecalAddProperty::Ref::New(decal, key, value) : meUDecalAddProperty::Ref();
	}
	
	virtual igdeUndo::Ref UndoRemoveProperty(const decString &key){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? meUDecalRemoveProperty::Ref::New(decal, key, decal->GetProperties().GetAt(key)) : meUDecalRemoveProperty::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperty(const decString &key, const decString &oldValue, const decString &newValue){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? meUDecalSetProperty::Ref::New(decal, key, oldValue, newValue) : meUDecalSetProperty::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperties(const decStringDictionary &properties){
		meDecal * const decal = pPanel.GetActiveDecal();
		return decal ? meUDecalSetProperties::Ref::New(decal, properties) : meUDecalSetProperties::Ref();
	}
};

class cActionPropCopyToSel : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropCopyToSel> Ref;
	cActionPropCopyToSel(meWPSDecal &panel) : cBaseAction(panel, "Copy To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Copy property from active decal to all selected decals"){}
	
	igdeUndo::Ref OnAction(meDecal *decal) override{
		meDecal::List list(pPanel.GetWorld()->GetSelectionDecal().GetSelected());
		const decString &property = pPanel.GetActiveProperty();
		list.Remove(decal);
		return !property.IsEmpty() && list.IsNotEmpty() ? meUDecalPropertyCopyToSelected::Ref::New(
			list, property, decal->GetProperties().GetAt(property)) : igdeUndo::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveDecal() && !pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionDecal().GetSelected().GetCount() > 1);
	}
};

class cActionPropRemoveFromSel : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropRemoveFromSel> Ref;
	cActionPropRemoveFromSel(meWPSDecal &panel) : cBaseAction(panel, "Remove From Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Remove property from all selected decals"){}
	
	igdeUndo::Ref OnAction(meDecal*) override{
		const meDecal::List &list = pPanel.GetWorld()->GetSelectionDecal().GetSelected();
		const decString &property = pPanel.GetActiveProperty();
		return !property.IsEmpty() && list.IsNotEmpty() ? meUDecalPropertyRemoveFromSelected::Ref::New(list, property) : igdeUndo::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveDecal() && !pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionDecal().GetSelected().IsNotEmpty());
	}
};

class cActionPropCloneToSel : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropCloneToSel> Ref;
	cActionPropCloneToSel(meWPSDecal &panel) : cBaseAction(panel, "Clone To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Clone all properties from active decal to all selected decals"){}
	
	igdeUndo::Ref OnAction(meDecal *decal) override{
		meDecal::List list(pPanel.GetWorld()->GetSelectionDecal().GetSelected());
		list.Remove(decal);
		return list.IsNotEmpty() ? meUDecalClonePropertiesToSelected::Ref::New(list, decal->GetProperties()) : meUDecalClonePropertiesToSelected::Ref();
	}
	
	void Update() override{
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
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = meWPSDecalListener::Ref::New(*this);
	
	pActionPropCopyToSel = cActionPropCopyToSel::Ref::New(*this);
	pActionPropRemoveFromSel = cActionPropRemoveFromSel::Ref::New(*this);
	pActionPropCloneToSel = cActionPropCloneToSel::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	groupBox = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 10);
	content->AddChild(groupBox);
	helper.Label(groupBox, "Selected:");
	helper.EditInteger(groupBox, "Number of selected decals", 3, pEditSelCount, {});
	pEditSelCount->SetEditable(false);
	helper.Label(groupBox, "Active:");
	helper.EditSpinInteger(groupBox, "Active decal to edit", 0, 0, pSpinActive, cSpinActive::Ref::New(*this));
	
	
	// geometry
	helper.GroupBox(content, groupBox, "Geometry:");
	
	helper.EditString(groupBox, "ID:", "Unique decal ID", pEditID, {});
	pEditID->SetEditable(false);
	
	helper.EditDVector(groupBox, "Position:", "Position of decal.", pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation of decal.", pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditVector(groupBox, "Size:", "Size of decal.", pEditSize, cEditSize::Ref::New(*this));
	helper.EditInteger(groupBox, "Order:", "Drawing order of decal.", pEditOrder, cEditOrder::Ref::New(*this));
	helper.CheckBox(groupBox, pChkVisible, cActionVisible::Ref::New(*this));
	
	
	// texture
	helper.GroupBox(content, groupBox, "Texture:");
	
	helper.EditPath(groupBox, "Skin:", "Skin to use for decal",
		igdeEnvironment::efpltSkin, pEditSkin, cEditSkin::Ref::New(*this));
	
	
	// texture transform
	helper.GroupBox(content, groupBox, "Texture Transformation:", true);
	
	helper.EditVector2(groupBox, "Translation:", "Texture coordinates translation.",
		pEditTexCoordOffset, cEditTexCoordOffset::Ref::New(*this));
	helper.EditVector2(groupBox, "Scaling:", "Texture coordinates scaling.",
		pEditTexCoordScaling, cEditTexCoordScaling::Ref::New(*this));
	helper.EditFloat(groupBox, "Rotation:", "Texture coordinates rotation.",
		pEditTexCoordRotation, cEditTexCoordRotation::Ref::New(*this));
	helper.ColorBox(groupBox, "Tint:", "Color tint.", pClrColorTint, cEditColorTint::Ref::New(*this));
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "Properties:");
	
	pEditProperties = cEditProperties::Ref::New(*this);
	groupBox->AddChild(pEditProperties);
}

meWPSDecal::~meWPSDecal(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPSDecal::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pEditProperties->SetClipboard(nullptr);
		pEditProperties->SetUndoSystem(nullptr);
		pEditProperties->SetTriggerTargetList(nullptr);
		pEditProperties->SetIdentifiers(decStringSet());
		
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		pEditProperties->SetClipboard(&pWPSelection.GetWindowProperties().GetWindowMain().GetClipboard());
		pEditProperties->SetUndoSystem(world->GetUndoSystem());
		pEditProperties->SetTriggerTargetList(&world->GetTriggerTable());
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
	return pWorld ? pWorld->GetSelectionDecal().GetActive() : nullptr;
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
	const bool enabled = GetActiveDecal() != nullptr;
	
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
		pEditProperties->SelectProperty(GetActiveProperty());
	}
}

void meWPSDecal::UpdatePropertyKeys(){
	pEditProperties->UpdateKeys();
}

void meWPSDecal::UpdateProperties(){
	pEditProperties->SetProperties(GetActiveDecal() ? GetActiveDecal()->GetProperties() : decStringDictionary());
	SelectActiveProperty();
}

void meWPSDecal::UpdateIdentifierLists(){
	const decString &property = GetActiveProperty();
	decStringSet identifiers;
	
	if(pWorld && !property.IsEmpty()){
		const igdeGDProperty * const gdProperty = pEditProperties->GetGDProperty(property);
		if(gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().FindOrDefault([&](const meIDGroup &group){
				return group.GetName() == gdProperty->GetIdentifierGroup();
			});
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
	pEditProperties->SetIdentifiers(identifiers);
}


void meWPSDecal::OnGameDefinitionChanged(){
	UpdateIdentifierLists();
	UpdatePropertyKeys();
	UpdateProperties();
}
