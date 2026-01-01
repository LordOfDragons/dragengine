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

#include "meWPSObjectShape.h"
#include "meWPSObjectShapeUpdater.h"
#include "meWPSObjectShapeListener.h"
#include "../meWPSelection.h"
#include "../../meWindowProperties.h"
#include "../../../meWindowMain.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/meObjectSelection.h"
#include "../../../../world/meWorldGuiParameters.h"
#include "../../../../world/objectshape/meObjectShape.h"
#include "../../../../undosys/properties/object/property/meUObjectSetProperty.h"
#include "../../../../undosys/gui/objectshape/meUObjectShapeReplace.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

enum eShapePanels{
	espNone,
	espSphere,
	espBox,
	espCylinder,
	espCapsule
};



// Actions
///////////

namespace{

class cSpinActive : public igdeSpinTextFieldListener{
	meWPSObjectShape &pPanel;
	
public:
	typedef deTObjectReference<cSpinActive> Ref;
	cSpinActive(meWPSObjectShape &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		if(!pPanel.IsActivePropertyShape()){
			return;
		}
		
		meObjectShapeSelection &selection = pPanel.GetWorld()->GetSelectionObjectShape();
		const meObjectShape::List &shapeList = selection.GetSelected();
		const int index = textField->GetValue();
		
		selection.SetActive(index >= 0 && index <= shapeList.GetCount() ? shapeList.GetAt(index) : nullptr);
		
		pPanel.GetWorld()->NotifyObjectShapeSelectionChanged();
	}
};

class cComboProperties : public igdeComboBoxListener{
	meWPSObjectShape &pPanel;
	bool &pPreventUpdate;
	
public:
	typedef deTObjectReference<cComboProperties> Ref;
	cComboProperties(meWPSObjectShape &panel, bool &preventUpdate) :
		pPanel(panel), pPreventUpdate(preventUpdate){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		meObject * const object = pPanel.GetActiveObject();
		if(object && !pPreventUpdate){
			object->SetActiveProperty(comboBox->GetText());
		}
	}
};

class cShapeValueVector : public igdeEditVectorListener{
	meWPSObjectShape &pPanel;
	
public:
	typedef deTObjectReference<cShapeValueVector> Ref;
	cShapeValueVector(meWPSObjectShape &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector*){
		pPanel.OnShapeChanged();
	}
};

class cShapeValueFloat : public igdeTextFieldListener{
	meWPSObjectShape &pPanel;
	
public:
	typedef deTObjectReference<cShapeValueFloat> Ref;
	cShapeValueFloat(meWPSObjectShape &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField*){
		pPanel.OnShapeChanged();
	}
};

}



// Class meWPSObjectShape
///////////////////////////

// Constructor, destructor
////////////////////////////

meWPSObjectShape::meWPSObjectShape(meWPSelection &wpselection) :
igdeContainerScroll(wpselection.GetEnvironment(), false, true),
pWPSelection(wpselection),
pPreventUpdate(false)
{
	igdeEnvironment &env = wpselection.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, form, formLine;
	
	pListener = meWPSObjectShapeListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	form = igdeContainerForm::Ref::New(env);
	content->AddChild(form);
	
	helper.EditString(form, "Class:", "Name of the active object class.", pEditObjectClass, {});
	pEditObjectClass->SetEditable(false);
	
	helper.ComboBox(form, "Property:", "Name of the active object property.",
		pCBProperties, cComboProperties::Ref::New(*this, pPreventUpdate));
	pCBProperties->SetDefaultSorter();
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 10);
	content->AddChild(formLine);
	helper.Label(formLine, "Count:");
	helper.EditInteger(formLine, "Count of shapes", 3, pEditShapeCount, {});
	pEditShapeCount->SetEditable(false);
	helper.Label(formLine, "Active:");
	helper.EditSpinInteger(formLine, "Active shape to edit", 0, 0, pSpinActive, cSpinActive::Ref::New(*this));
	
	helper.EditString(content, "Property information", pTextInfos, 6, {});
	pTextInfos->SetEditable(false);
	
	
	pCreateShapePanels();
	content->AddChild(pSwitcherShapeType);
	
	UpdateShapeNone();
}

meWPSObjectShape::~meWPSObjectShape(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPSObjectShape::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
	}
	
	UpdateListProperties(false);
	UpdateSelection();
	UpdateActiveShape();
}



meObject *meWPSObjectShape::GetActiveObject() const{
	return pWorld ? pWorld->GetSelectionObject().GetActive() : nullptr;
}

const decString &meWPSObjectShape::GetActiveProperty() const{
	meObject * const activeObject = GetActiveObject();
	static const decString noActiveProperty;
	return activeObject ? activeObject->GetActiveProperty() : noActiveProperty;
}

bool meWPSObjectShape::IsActivePropertyShape() const{
	meObject * const activeObject = GetActiveObject();
	const decString &activeProperty = GetActiveProperty();
	return activeObject && !activeProperty.IsEmpty()
		&& activeObject->IsPropertyShapeOrShapeList(activeProperty);
}



void meWPSObjectShape::UpdateListProperties(bool retainSelection){
	meObject * const activeObject = GetActiveObject();
	const decString &activeProperty = GetActiveProperty();
	const bool isPropertyShape = IsActivePropertyShape();
	decTList<int> selectionIndexList;
	int selectionIndexActive = -1;
	
	// create list of indices from the currently selected object shapes and the active shape if set
	if(retainSelection && isPropertyShape){
		const meObjectShapeSelection &selection = pWorld->GetSelectionObjectShape();
		const meObjectShape::List &shapeList = pWorld->GetObjectShapes();
		
		selection.GetSelected().Visit([&](meObjectShape *s){
			selectionIndexList.Add(shapeList.IndexOf(s));
		});
		
		if(selection.GetActive()){
			selectionIndexActive = shapeList.IndexOf(selection.GetActive());
		}
	}
	
	// update the properties list
	pPreventUpdate = true;
	try{
		pCBProperties->RemoveAllItems();
		
		if(activeObject){
			const decStringDictionary &properties = activeObject->GetProperties();
			const decStringList keys(properties.GetKeys());
			const int count = keys.GetCount();
			int i;
			
			pEditObjectClass->SetText(activeObject->GetClassName());
			
			for(i=0; i<count; i++){
				pCBProperties->AddItem(keys.GetAt(i));
			}
			
			pCBProperties->SortItems();
			
		}else{
			pEditObjectClass->ClearText();
		}
		
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
	
	pCBProperties->SetSelection(pCBProperties->IndexOfItem(activeProperty));
	
	// reset the list of selected object shapes, the selection and active state
	if(pWorld){
		pWorld->GetSelectionObjectShape().Reset();
		
		pWorld->GetObjectShapes().Visit([](meObjectShape &shape){
			shape.SetParentObject(nullptr);
			shape.SetWorld(nullptr);
		});
		
		pWorld->GetObjectShapes().RemoveAll();
	}
	
	// rebuild the shape list from the currently selected property
	if(isPropertyShape){
		igdeCodecPropertyString codec;
		decShapeList shapeList;
		int i, count;
		
		try{
			codec.DecodeShapeList(activeObject->GetProperties().GetAt(activeProperty), shapeList);
			
		}catch(const deException &){
			shapeList.RemoveAll();
		}
		
		count = shapeList.GetCount();
		
		if(activeObject->IsPropertyShape(activeProperty) && count > 1){
			count = 1;
		}
		
		for(i=0; i<count; i++){
			const meObjectShape::Ref objectShape(meObjectShape::Ref::New(
				pWorld->GetEnvironment(), *shapeList.GetAt(i)));
			objectShape->SetWorld(pWorld);
			objectShape->SetParentObject(activeObject);
			pWorld->GetObjectShapes().Add(objectShape);
		}
	}
	
	// rebuild the selected and active state using the indices stored before. drop indices outside the new list
	if(isPropertyShape){
		meObjectShapeSelection &selection = pWorld->GetSelectionObjectShape();
		const meObjectShape::List &shapeList = pWorld->GetObjectShapes();
		const int shapeCount = shapeList.GetCount();
		
		selectionIndexList.Visit([&](int index){
			if(index < shapeCount){
				selection.Add(shapeList.GetAt(index));
			}
		});
		
		if(selectionIndexActive >= 0 && selectionIndexActive < shapeCount){
			selection.SetActive(shapeList.GetAt(selectionIndexActive));
		}
		
		// the idea is the following. most of the time the property value is changed using the object shape properties
		// panel or using the 3d view object shape editor. in this case only the values of the active shape do change.
		// the selection and active does not change and the ordering of the shape list does not change. in this case
		// reproducing the selection and active using the indices of the appropriate elements results in the same
		// selection and active afterwards. in the case the user changed the property value using any other way the
		// selection and active will be incorrect but still inside valid boundaries. incorrect is not a problem since
		// changing the value using any other way is usually not happening while object shape mode is activated and
		// even if it does nobody would expect the result to be reasonable anyways. so it is okay to have a random
		// selection and active after such a modification. to avoid this situation the setPropertyValue undo action
		// can clear the shape list, selection and active. this way the selection and active is cleared in the case
		// of an uncontrolled modification of the property value. adding and removing shapes is handled differently
		// and clears selection and active at all times. this way this problem should be solved in a way that only
		// a minimal amount of code parts are touched while providing a reasonable result.
	}
}

void meWPSObjectShape::UpdateSelection(){
	int active = 0;
	int count = 0;
	
	if(IsActivePropertyShape()){
		const meObjectShapeSelection &selection = pWorld->GetSelectionObjectShape();
		count = selection.GetSelected().GetCount();
		if(selection.HasActive()){
			active = selection.GetSelected().IndexOf(selection.GetActive());
		}
	}
	
	pEditShapeCount->SetInteger(count);
	
	pSpinActive->SetRange(0, count - 1);
	pSpinActive->SetValue(active);
	pSpinActive->SetEnabled(count > 0);
}

void meWPSObjectShape::UpdateActiveShape(){
	const decShape *shape = nullptr;
	
	if(pWorld){
		meObjectShape * const objectShape = pWorld->GetSelectionObjectShape().GetActive();
		if(objectShape){
			shape = objectShape->GetShape();
		}
	}
	
	if(shape){
		meWPSObjectShapeUpdater visitor(*this);
		const_cast<decShape*>(shape)->Visit(visitor);
		
	}else{
		UpdateShapeNone();
	}
	
	pDisplayPropertyInfo();
}



void meWPSObjectShape::UpdateObjectShapeShapes(){
	const meObject * const activeObject = GetActiveObject();
	if(!activeObject || !activeObject->GetActive()){
		return;
	}
	
	pWorld->GetObjectShapes().Visit([&](meObjectShape &s){
		s.UpdateShape();
	});
}



void meWPSObjectShape::UpdateShapeNone(){
	const igdeUIHelper::EnableBoolGuard guard(pPreventUpdate);
	pSwitcherShapeType->SetCurrent(espNone);
}

void meWPSObjectShape::UpdateShapeSphere(const decShapeSphere &sphere){
	const igdeUIHelper::EnableBoolGuard guard(pPreventUpdate);
	pEditSpherePosition->SetVector(sphere.GetPosition());
	pEditSphereRadius->SetFloat(sphere.GetRadius());
	pSwitcherShapeType->SetCurrent(espSphere);
}

void meWPSObjectShape::UpdateShapeBox(const decShapeBox &box){
	const igdeUIHelper::EnableBoolGuard guard(pPreventUpdate);
	pEditBoxPosition->SetVector(box.GetPosition());
	pEditBoxRotation->SetVector(decMatrix::CreateFromQuaternion(
		box.GetOrientation()).GetEulerAngles() * RAD2DEG);
	pEditBoxExtends->SetVector(box.GetHalfExtends());
	pSwitcherShapeType->SetCurrent(espBox);
}

void meWPSObjectShape::UpdateShapeCylinder(const decShapeCylinder &cylinder){
	const igdeUIHelper::EnableBoolGuard guard(pPreventUpdate);
	pEditCylinderPosition->SetVector(cylinder.GetPosition());
	pEditCylinderRotation->SetVector(decMatrix::CreateFromQuaternion(
		cylinder.GetOrientation()).GetEulerAngles() * RAD2DEG);
	pEditCylinderHeight->SetFloat(cylinder.GetHalfHeight());
	pEditCylinderRadiusTop->SetFloat(cylinder.GetTopRadius());
	pEditCylinderRadiusBottom->SetFloat(cylinder.GetBottomRadius());
	pSwitcherShapeType->SetCurrent(espCylinder);
}

void meWPSObjectShape::UpdateShapeCapsule(const decShapeCapsule &capsule){
	const igdeUIHelper::EnableBoolGuard guard(pPreventUpdate);
	pEditCapsulePosition->SetVector(capsule.GetPosition());
	pEditCapsuleRotation->SetVector(decMatrix::CreateFromQuaternion(
			capsule.GetOrientation()).GetEulerAngles() * RAD2DEG);
	pEditCapsuleHeight->SetFloat(capsule.GetHalfHeight());
	pEditCapsuleRadiusTop->SetFloat(capsule.GetTopRadius());
	pEditCapsuleRadiusBottom->SetFloat(capsule.GetBottomRadius());
	pSwitcherShapeType->SetCurrent(espCapsule);
}

void meWPSObjectShape::OnGameDefinitionChanged(){
	UpdateListProperties(true);
}



void meWPSObjectShape::OnShapeChanged(){
	if(pPreventUpdate){
		return;
	}
	
	const eShapePanels activePanel = (eShapePanels)pSwitcherShapeType->GetCurrent();
	const decString &activeProperty = GetActiveProperty();
	meObject * const object = GetActiveObject();
	if(!object || activeProperty.IsEmpty() || activePanel == espNone){
		return;
	}
	
	const int shapeIndex = pWorld->GetObjectShapes().IndexOf(pWorld->GetSelectionObjectShape().GetActive());
	if(shapeIndex == -1){
		return;
	}
	
	igdeUndo::Ref undo;
	decShape *shape = nullptr;
	
	try{
		switch(activePanel){
		case espSphere:
			shape = new decShapeSphere(
				decMath::max(pEditSphereRadius->GetFloat(), 0.001f),
				pEditSpherePosition->GetVector());
			break;
			
		case espBox:
			shape = new decShapeBox(
				decVector(0.001f, 0.001f, 0.001f).Largest(pEditBoxExtends->GetVector()),
				pEditBoxPosition->GetVector(),
				decMatrix::CreateRotation(pEditBoxRotation->GetVector() * DEG2RAD).ToQuaternion());
			break;
			
		case espCylinder:
			shape = new decShapeCylinder(
				decMath::max(pEditCylinderHeight->GetFloat(), 0.001f),
				decMath::max(pEditCylinderRadiusTop->GetFloat(), 0.001f),
				decMath::max(pEditCylinderRadiusBottom->GetFloat(), 0.001f),
				pEditCylinderPosition->GetVector(),
				decMatrix::CreateRotation(pEditCylinderRotation->GetVector() * DEG2RAD).ToQuaternion());
			break;
			
		case espCapsule:
			shape = new decShapeCapsule(
				decMath::max(pEditCapsuleHeight->GetFloat(), 0.001f),
				decMath::max(pEditCapsuleRadiusTop->GetFloat(), 0.001f),
				decMath::max(pEditCapsuleRadiusBottom->GetFloat(), 0.001f),
				pEditCapsulePosition->GetVector(),
				decMatrix::CreateRotation(pEditCapsuleRotation->GetVector() * DEG2RAD).ToQuaternion());
			break;
			
		default:
			return;
		}
		
		undo = meUObjectShapeReplace::Ref::New(object, activeProperty, shapeIndex, *shape);
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	pWorld->GetUndoSystem()->Add(undo);
}



// Private Functions
//////////////////////

void meWPSObjectShape::pCreateShapePanels(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	pSwitcherShapeType = igdeSwitcher::Ref::New(env);
	
	
	// none
	helper.Label(pSwitcherShapeType, "No Shape Selected");
	
	
	// sphere
	helper.GroupBoxStatic(pSwitcherShapeType, groupBox, "Sphere:");
	helper.EditVector(groupBox, "Position:", "Position of the sphere.",
		pEditSpherePosition, cShapeValueVector::Ref::New(*this));
	helper.EditFloat(groupBox, "Radius:", "Radius of the sphere.",
		pEditSphereRadius, cShapeValueFloat::Ref::New(*this));
	
	
	// box
	helper.GroupBoxStatic(pSwitcherShapeType, groupBox, "Box:");
	helper.EditVector(groupBox, "Position:", "Position of the box.",
		pEditBoxPosition, cShapeValueVector::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation of the box.",
		pEditBoxRotation, cShapeValueVector::Ref::New(*this));
	helper.EditVector(groupBox, "Extends:", "Extends of the box.",
		pEditBoxExtends, cShapeValueVector::Ref::New(*this));
	
	
	// cylinder
	helper.GroupBoxStatic(pSwitcherShapeType, groupBox, "Cylinder:");
	helper.EditVector(groupBox, "Position:", "Position of the cylinder.",
		pEditCylinderPosition, cShapeValueVector::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation of the cylinder.",
		pEditCylinderRotation, cShapeValueVector::Ref::New(*this));
	helper.EditFloat(groupBox, "Height:", "Height of the cylinder.",
		pEditCylinderHeight, cShapeValueFloat::Ref::New(*this));
	helper.EditFloat(groupBox, "Radius Top:", "Top radius of the cylinder.",
		pEditCylinderRadiusTop, cShapeValueFloat::Ref::New(*this));
	helper.EditFloat(groupBox, "Radius Bottom:", "Bottom radius of the cylinder.",
		pEditCylinderRadiusBottom, cShapeValueFloat::Ref::New(*this));
	
	
	// capsule
	helper.GroupBoxStatic(pSwitcherShapeType, groupBox, "Capsule:");
	helper.EditVector(groupBox, "Position:", "Position of the capsule.",
		pEditCapsulePosition, cShapeValueVector::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation of the capsule.",
		pEditCapsuleRotation, cShapeValueVector::Ref::New(*this));
	helper.EditFloat(groupBox, "Height:", "Height of the capsule.",
		pEditCapsuleHeight, cShapeValueFloat::Ref::New(*this));
	helper.EditFloat(groupBox, "Radius Top:", "Top radius of the capsule.",
		pEditCapsuleRadiusTop, cShapeValueFloat::Ref::New(*this));
	helper.EditFloat(groupBox, "Radius Bottom:", "Bottom radius of the capsule.",
		pEditCapsuleRadiusBottom, cShapeValueFloat::Ref::New(*this));
}

void meWPSObjectShape::pDisplayPropertyInfo(){
	const meObject * const object = GetActiveObject();
	if(!object){
		pTextInfos->ClearText();
		return;
	}
	
	const decString &property = object->GetActiveProperty();
	if(property.IsEmpty()){
		pTextInfos->ClearText();
		return;
	}
	
	const igdeGDClass *gdclass = object->GetGDClass();
	igdeGDProperty *gdproperty = nullptr;
	if(gdclass){
		gdproperty = gdclass->GetPropertyNamed(property);
	}
	
	decString text;
	if(gdproperty){
		text.Format("Object Class: %s\nProperty: %s\nDefault: '%s'\n\n%s",
			object->GetClassName().GetString(), property.GetString(),
			gdproperty->GetDefaultValue().GetString(),
			gdproperty->GetDescription().GetString());
		
	}else{
		text.Format("Object Class: %s\nProperty: %s\nDefault: ''",
			object->GetClassName().GetString(), property.GetString());
	}
	
	pTextInfos->SetText(text);
}
