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

#include "seWPTexture.h"
#include "seWPTextureListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"
#include "../../undosys/texture/seUTextureSetName.h"
#include "../../undosys/property/seUPropertySetValueType.h"
#include "../../undosys/property/seUPropertySetValue.h"
#include "../../undosys/property/seUPropertySetColor.h"
#include "../../undosys/property/seUPropertySetImagePath.h"
#include "../../undosys/property/seUPropertySetVideoPath.h"
#include "../../undosys/property/seUPropertySetMappedComponent.h"
#include "../../undosys/property/seUPropertyToggleVideoSharedTime.h"
#include "../../undosys/property/seUPropertySetConstructedColor.h"
#include "../../undosys/property/seUPropertyConstructedToggleTileX.h"
#include "../../undosys/property/seUPropertyConstructedToggleTileY.h"
#include "../../undosys/property/seUPropertyConstructedSetBitCount.h"
#include "../../undosys/property/seUPropertySetRenderableName.h"
#include "../../undosys/property/seUPropertySetBoneName.h"
#include "../../undosys/property/seUPropertySetConstructedSize.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPointListener.h>
#include <deigde/gui/composed/igdeEditPoint3.h>
#include <deigde/gui/composed/igdeEditPoint3Listener.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	seWPTexture &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(seWPTexture &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		if(!skin || !texture){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(skin, texture));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSkin *skin, seTexture *texture) = 0;
	
	void Update() override{
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		if(skin && texture){
			Update(*skin, *texture);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const seSkin &, const seTexture &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseActionProperty : public cBaseAction{
public:
	typedef deTObjectReference<cBaseActionProperty> Ref;
	
public:
	cBaseActionProperty(seWPTexture &panel, const char *text, igdeIcon *icon,
		const char *description) : cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(seSkin *skin, seTexture *texture) override{
		return pPanel.GetProperty() ? OnActionProperty(skin, texture, pPanel.GetProperty()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionProperty(seSkin *skin, seTexture *texture, seProperty *property) = 0;
	
	
	void Update(const seSkin &skin, const seTexture &texture) override{
		seProperty * const property = pPanel.GetProperty();
		if(property){
			UpdateProperty(skin, texture, *property);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateProperty(const seSkin &, const seTexture &, const seProperty &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPTexture &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if(!skin || !texture || !property){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*textField, skin, texture, property));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin *skin,
		seTexture *texture, seProperty *property) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	seWPTexture &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditPathListener> Ref;
	cBaseEditPathListener(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if(!skin || !texture || !property){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*editPath, skin, texture, property));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditPath &editPath, seSkin *skin,
		seTexture *texture, seProperty *property) = 0;
};

class cBaseColorBoxListener : public igdeColorBoxListener{
protected:
	seWPTexture &pPanel;
	
public:
	typedef deTObjectReference<cBaseColorBoxListener> Ref;
	cBaseColorBoxListener(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if(!skin || !texture || !property){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*colorBox, skin, texture, property));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeColorBox &colorBox, seSkin *skin,
		seTexture *texture, seProperty *property) = 0;
};


class cListTextures : public igdeListBoxListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cListTextures> Ref;
	cListTextures(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			skin->SetActiveTexture(listBox->GetSelectedItem()
				? (seTexture*)listBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		seWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeEnvironment &env = menu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand(menu, windowMain.GetActionTextureAdd());
		helper.MenuCommand(menu, windowMain.GetActionTextureRemove());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, windowMain.GetActionTextureImportFromFile());
		helper.MenuCommand(menu, windowMain.GetActionTextureImportFromGDef());
	}
};

class cTextTextureName : public igdeTextFieldListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cTextTextureName> Ref;
	cTextTextureName(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seTexture * const texture = pPanel.GetTexture();
		const decString &value = textField->GetText();
		if(!texture || texture->GetName() == value){
			return;
		}
		
		if(pPanel.GetSkin()->GetTextures().HasMatching([&value, texture](const seTexture &t){
			return t.GetName() == value;
		})){
			igdeCommonDialogs::Error(&pPanel, "Set Texture Name", "Texture name exists already");
			textField->SetText(texture->GetName());
			return;
		}
		
		pPanel.GetSkin()->GetUndoSystem()->Add(seUTextureSetName::Ref::New(texture, value));
	}
};

class cEditPreviewEditTCOffset : public igdeEditPointListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cEditPreviewEditTCOffset> Ref;
	cEditPreviewEditTCOffset(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnPointChanged(igdeEditPoint *editPoint){
		seTexture * const texture = pPanel.GetTexture();
		if(texture){
			texture->SetTexCoordOffset(editPoint->GetPoint());
		}
	}
};

class cEditPreviewEditTCScale : public igdeEditPointListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cEditPreviewEditTCScale> Ref;
	cEditPreviewEditTCScale(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnPointChanged(igdeEditPoint *editPoint){
		seTexture * const texture = pPanel.GetTexture();
		if(texture){
			texture->SetTexCoordScaling(editPoint->GetPoint());
		}
	}
};

class cEditPreviewEditTCRotation : public igdeTextFieldListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cEditPreviewEditTCRotation> Ref;
	cEditPreviewEditTCRotation(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seTexture * const texture = pPanel.GetTexture();
		if(texture){
			texture->SetTexCoordRotation(textField->GetFloat());
		}
	}
};


class cListProperties : public igdeListBoxListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cListProperties> Ref;
	cListProperties(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seTexture * const texture = pPanel.GetTexture();
		if(texture){
			texture->SetActiveProperty(listBox->GetSelectedItem()
				? (seProperty*)listBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		seWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeEnvironment &env = menu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand(menu, windowMain.GetActionTexturePropertyAdd());
		helper.MenuCommand(menu, windowMain.GetActionTexturePropertyRemove());
	}
};

class cComboPropertyValueType : public igdeComboBoxListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cComboPropertyValueType> Ref;
	cComboPropertyValueType(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seProperty * const property = pPanel.GetProperty();
		if(!property || !comboBox->GetSelectedItem()){
			return;
		}
		
		const seProperty::eValueTypes value = (seProperty::eValueTypes)(intptr_t)
			comboBox->GetSelectedItem()->GetData();
		if(value == property->GetValueType()){
			return;
		}
		
		pPanel.GetSkin()->GetUndoSystem()->Add(seUPropertySetValueType::Ref::New(property, value));
	}
};

class cTextPropertyRenderable : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPropertyRenderable> Ref;
	cTextPropertyRenderable(seWPTexture &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seTexture*, seProperty *property) override{
		return property->GetRenderableName() != textField.GetText()
			? seUPropertySetRenderableName::Ref::New(property, textField.GetText()) : igdeUndo::Ref();
	}
};

class cTextPropertyBone : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPropertyBone> Ref;
	cTextPropertyBone(seWPTexture &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seTexture*, seProperty *property) override{
		return property->GetBoneName() != textField.GetText()
			? seUPropertySetBoneName::Ref::New(property, textField.GetText()) : igdeUndo::Ref();
	}
};


class cTextPropertyValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPropertyValue> Ref;
	cTextPropertyValue(seWPTexture &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seTexture*, seProperty *property) override{
		const float value = textField.GetFloat();
		return fabsf(property->GetValue() - value) > FLOAT_SAFE_EPSILON
			? seUPropertySetValue::Ref::New(property, value) : igdeUndo::Ref();
	}
};


class cColorPropertyColor : public cBaseColorBoxListener{
public:
	typedef deTObjectReference<cColorPropertyColor> Ref;
	cColorPropertyColor(seWPTexture &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeColorBox &colorBox, seSkin*, seTexture*, seProperty *property) override{
		return !property->GetColor().IsEqualTo(colorBox.GetColor())
			? seUPropertySetColor::Ref::New(property, colorBox.GetColor()) : igdeUndo::Ref();
	}
};


class cPathPropertyImage : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathPropertyImage> Ref;
	cPathPropertyImage(seWPTexture &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, seSkin*, seTexture*, seProperty *property) override{
		return property->GetImagePath() != editPath.GetPath()
			? seUPropertySetImagePath::Ref::New(property, editPath.GetPath()) : igdeUndo::Ref();
	}
};


class cPathPropertyVideo : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathPropertyVideo> Ref;
	cPathPropertyVideo(seWPTexture &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath &editPath, seSkin*, seTexture*, seProperty *property) override{
		return property->GetVideoPath() != editPath.GetPath()
			? seUPropertySetVideoPath::Ref::New(property, editPath.GetPath()) : igdeUndo::Ref();
	}
};

class cActionPropertyVideoUseShared : public cBaseActionProperty{
public:
	typedef deTObjectReference<cActionPropertyVideoUseShared> Ref;
	cActionPropertyVideoUseShared(seWPTexture &panel) : cBaseActionProperty(panel,
		"Use shared time", nullptr, "Playback time is shared across instances"){}
	
	virtual igdeUndo::Ref OnActionProperty(seSkin*, seTexture* , seProperty *property){
		return seUPropertyToggleVideoSharedTime::Ref::New(property);
	}
	
	void UpdateProperty(const seSkin &, const seTexture &, const seProperty &property) override{
		SetEnabled(true);
		SetSelected(property.GetVideoSharedTime());
	}
};


class cSpinPropertyMappedComponent : public igdeSpinTextFieldListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cSpinPropertyMappedComponent> Ref;
	cSpinPropertyMappedComponent(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField*){
		pPanel.UpdatePropertyMappedComponent();
	}
};

class cComboPropertyMappedTarget : public igdeComboBoxListener{
	seWPTexture &pPanel;
	bool &pPreventUpdate;
public:
	typedef deTObjectReference<cComboPropertyMappedTarget> Ref;
	cComboPropertyMappedTarget(seWPTexture &panel, bool &preventUpdate) :
	pPanel(panel), pPreventUpdate(preventUpdate){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(pPreventUpdate){
			return;
		}
		
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if(!skin || !texture || !property || !comboBox->GetSelectedItem()){
			return;
		}
		
		const int index = pPanel.GetPropertyMappedComponentIndex();
		seMapped * const curMapped = property->GetMappedComponent(index);
		seMapped * const newMapped = (seMapped*)comboBox->GetSelectedItem()->GetData();
		if(newMapped == curMapped){
			return;
		}
		
		skin->GetUndoSystem()->Add(seUPropertySetMappedComponent::Ref::New(
			property, index, newMapped));
	}
};


class cColorPropertyConstructed : public cBaseColorBoxListener{
public:
	typedef deTObjectReference<cColorPropertyConstructed> Ref;
	cColorPropertyConstructed(seWPTexture &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeColorBox &colorBox, seSkin*, seTexture*, seProperty *property) override{
		return !property->GetNodeColor().IsEqualTo(colorBox.GetColor())
			? seUPropertySetConstructedColor::Ref::New(property, colorBox.GetColor()) : igdeUndo::Ref();
	}
};

class cEditPropertyConstructedSize : public igdeEditPoint3Listener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cEditPropertyConstructedSize> Ref;
	cEditPropertyConstructedSize(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnPoint3Changed(igdeEditPoint3 *editPoint3){
		seProperty * const property = pPanel.GetProperty();
		if(!property || property->GetNodeGroup()->GetSize() == editPoint3->GetPoint3()){
			return;
		}
		
		pPanel.GetSkin()->GetUndoSystem()->Add(seUPropertySetConstructedSize::Ref::New(
			property, editPoint3->GetPoint3()));
	}
};

class cActionConstructedChkTileX : public cBaseActionProperty{
public:
	typedef deTObjectReference<cActionConstructedChkTileX> Ref;
	cActionConstructedChkTileX(seWPTexture &panel) : cBaseActionProperty(panel,
		"X-Axis", nullptr, "Tile nodes along X axis"){}
	
	virtual igdeUndo::Ref OnActionProperty(seSkin*, seTexture* , seProperty *property){
		return seUPropertyConstructedToggleTileX::Ref::New(property);
	}
	
	void UpdateProperty(const seSkin &, const seTexture &, const seProperty &property) override{
		SetEnabled(true);
		SetSelected(property.GetNodeTileX());
	}
};

class cActionConstructedChkTileY : public cBaseActionProperty{
public:
	typedef deTObjectReference<cActionConstructedChkTileY> Ref;
	cActionConstructedChkTileY(seWPTexture &panel) : cBaseActionProperty(panel,
		"Y-Axis", nullptr, "Tile nodes along Y axis"){}
	
	virtual igdeUndo::Ref OnActionProperty(seSkin*, seTexture* , seProperty *property){
		return seUPropertyConstructedToggleTileY::Ref::New(property);
	}
	
	void UpdateProperty(const seSkin &, const seTexture &, const seProperty &property) override{
		SetEnabled(true);
		SetSelected(property.GetNodeTileY());
	}
};

class cComboConstructedBitCount : public igdeComboBoxListener{
	seWPTexture &pPanel;
public:
	typedef deTObjectReference<cComboConstructedBitCount> Ref;
	cComboConstructedBitCount(seWPTexture &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seSkin * const skin = pPanel.GetSkin();
		seProperty * const property = pPanel.GetProperty();
		if(!skin || !pPanel.GetTexture() || !property || !comboBox->GetSelectedItem()){
			return;
		}
		
		const int bitCount = (int)(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(bitCount == property->GetNodeBitCount()){
			return;
		}
		
		skin->GetUndoSystem()->Add(seUPropertyConstructedSetBitCount::Ref::New(property, bitCount));
	}
};

}



// Class seWPTexture
//////////////////////

// Constructor, destructor
////////////////////////////

seWPTexture::seWPTexture(seWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pRequiresUpdate(false),
pPreventUpdateMappedTarget(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = seWPTextureListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// texture
	helper.GroupBoxFlow(content, groupBox, "Texture:");
	helper.ListBox(groupBox, 8, "Textures", pListTexture, cListTextures::Ref::New(*this));
	pListTexture->SetDefaultSorter();
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	helper.EditString(form, "Name:", "Name of texture", pEditTexName, cTextTextureName::Ref::New(*this));
	
	
	// preview
	helper.GroupBox(content, groupBox, "Texture Preview:", true);
	helper.EditPoint(groupBox, "Translation:", "Texture coordinates translation.",
		pPreviewEditTCOffset, cEditPreviewEditTCOffset::Ref::New(*this));
	helper.EditPoint(groupBox, "Scaling:", "Texture coordinates scaling.",
		pPreviewEditTCScaling, cEditPreviewEditTCScale::Ref::New(*this));
	helper.EditFloat(groupBox, "Rotation:", "Texture coordinates rotation.",
		pPreviewEditTCRotation, cEditPreviewEditTCRotation::Ref::New(*this));
	
	
	// property
	helper.GroupBoxFlow(content, groupBox, "Property:");
	helper.ListBox(groupBox, 8, "Property", pListProperty, cListProperties::Ref::New(*this));
	pListProperty->SetDefaultSorter();
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.EditString(form, "Name:", "Name of property", pEditPropName, {});
	pEditPropName->SetEditable(false);
	
	helper.EditString(form, "Renderable:", "Name of renderable to use or empty string to use none",
		pEditPropRenderable, cTextPropertyRenderable::Ref::New(*this));
	
	helper.EditString(form, "Bone:", "Name of bone to use or empty string. "
		"Used by properties using vertex position instead of texture coordinates.",
		pEditPropBone, cTextPropertyBone::Ref::New(*this));
	
	helper.ComboBox(form, "Value Type:", "Type of value to use for the property",
		pCBPropertyType, cComboPropertyValueType::Ref::New(*this));
	pCBPropertyType->AddItem("Value", nullptr, (void*)(intptr_t)seProperty::evtValue);
	pCBPropertyType->AddItem("Color", nullptr, (void*)(intptr_t)seProperty::evtColor);
	pCBPropertyType->AddItem("Image", nullptr, (void*)(intptr_t)seProperty::evtImage);
	pCBPropertyType->AddItem("Video", nullptr, (void*)(intptr_t)seProperty::evtVideo);
	pCBPropertyType->AddItem("Constructed", nullptr, (void*)(intptr_t)seProperty::evtConstructed);
	pCBPropertyType->AddItem("Mapped", nullptr, (void*)(intptr_t)seProperty::evtMapped);
	
	
	// property panel switcher
	pSwitcher = igdeSwitcher::Ref::New(env);
	content->AddChild(pSwitcher);
	
	
	// empty
	helper.Label(pSwitcher, "", "");
	
	
	// static value
	panel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	pSwitcher->AddChild(panel);
	helper.GroupBox(panel, groupBox, "Value:");
	helper.EditFloat(groupBox, "Value:", "Value to use", pEditPvtValue, cTextPropertyValue::Ref::New(*this));
	
	
	// color value
	panel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	pSwitcher->AddChild(panel);
	helper.GroupBox(panel, groupBox, "Color:");
	helper.ColorBox(groupBox, "Color:", "Color to use", pClrPvtColor, cColorPropertyColor::Ref::New(*this));
	
	
	// image
	panel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	pSwitcher->AddChild(panel);
	helper.GroupBox(panel, groupBox, "Image:");
	helper.EditPath(groupBox, "Path:", "Path to the image to use", igdeEnvironment::efpltImage,
		pEditPvtImagePath, cPathPropertyImage::Ref::New(*this));
	
	helper.EditString(groupBox, "", "Image information", pLabPvtImageInfo, {});
	pLabPvtImageInfo->SetEditable(false);
	
	
	// video
	panel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	pSwitcher->AddChild(panel);
	helper.GroupBox(panel, groupBox, "Video:");
	helper.EditPath(groupBox, "Path:", "Path to the video to use", igdeEnvironment::efpltVideo,
		pEditPvtVideoPath, cPathPropertyVideo::Ref::New(*this));
	
	helper.EditString(groupBox, "", "Video information", pLabPvtVideoInfo, {});
	pLabPvtVideoInfo->SetEditable(false);
	
	helper.CheckBox(groupBox, pChkPvtVideoSharedTime, cActionPropertyVideoUseShared::Ref::New(*this));
	
	
	// constructed
	panel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	pSwitcher->AddChild(panel);
	helper.GroupBox(panel, groupBox, "Constructed:");
	helper.ColorBox(groupBox, "Color:", "Color to use",
		pConstructedClrColor, cColorPropertyConstructed::Ref::New(*this));
	helper.EditPoint3(groupBox, "Size:", "Size of constructed image",
		pConstructedEditSize, cEditPropertyConstructedSize::Ref::New(*this));
	
	helper.FormLine(groupBox, "Tile:", "Tile nodes", formLine);
	helper.CheckBox(formLine, pConstructedChkTileX, cActionConstructedChkTileX::Ref::New(*this));
	helper.CheckBox(formLine, pConstructedChkTileY, cActionConstructedChkTileY::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Bit Count:", "Bit count of constructed image",
		pConstructedCBBitCount, cComboConstructedBitCount::Ref::New(*this));
	pConstructedCBBitCount->AddItem("8-Bit", nullptr, (void*)(intptr_t)8);
	pConstructedCBBitCount->AddItem("16-Bit", nullptr, (void*)(intptr_t)16);
	pConstructedCBBitCount->AddItem("32-Bit", nullptr, (void*)(intptr_t)32);
	
	
	// mapped
	panel = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	pSwitcher->AddChild(panel);
	helper.GroupBoxFlow(panel, groupBox, "Mapped:");
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.EditSpinInteger(form, "Component:", "Component to edit (0=red, 1=green, 2=blue, 3=alpha)",
		0, 3, pSpinPvtMappedComponent, cSpinPropertyMappedComponent::Ref::New(*this));
	
	helper.ComboBox(form, "Target:", "Target mapped to use for this component",
		pCBPvtMappedTarget, cComboPropertyMappedTarget::Ref::New(*this, pPreventUpdateMappedTarget));
	
	pCBPvtMappedTarget->SetDefaultSorter();
	UpdatePropertyMappedTargetList();
}

seWPTexture::~seWPTexture(){
	SetSkin(nullptr);
}



// Management
///////////////

void seWPTexture::SetSkin(seSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	if(pSkin){
		pSkin->RemoveListener(pListener);
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddListener(pListener);
	}
	
	UpdatePropertyMappedTargetList();
	UpdateTextureList();
	OnSkinPathChanged();
}

void seWPTexture::OnSkinPathChanged(){
	if(pSkin){
		pEditPvtImagePath->SetBasePath(pSkin->GetDirectoryPath());
		pEditPvtVideoPath->SetBasePath(pSkin->GetDirectoryPath());
		
	}else{
		pEditPvtImagePath->SetBasePath("");
		pEditPvtVideoPath->SetBasePath("");
	}
}

seTexture *seWPTexture::GetTexture() const{
	return pSkin ? pSkin->GetActiveTexture() : nullptr;
}

seProperty *seWPTexture::GetProperty() const{
	seTexture * const texture = GetTexture();
	return texture ? texture->GetActiveProperty() : nullptr;
}



void seWPTexture::UpdateTextureList(){
	seTexture * const selection = GetTexture();
	
	pListTexture->RemoveAllItems();
	
	if(pSkin){
		pSkin->GetTextures().Visit([&](seTexture *texture){
			pListTexture->AddItem(texture->GetName(), nullptr, texture);
		});
		pListTexture->SortItems();
	}
	
	pListTexture->SetSelectionWithData(selection);
	UpdateTexture();
}

void seWPTexture::SelectActiveTexture(){
	pListTexture->SetSelectionWithData(GetTexture());
}

void seWPTexture::UpdateTexture(){
	const seTexture * const texture = GetTexture();
	
	if(texture){
		pEditTexName->SetText(texture->GetName());
		
	}else{
		pEditTexName->ClearText();
	}
	
	pEditTexName->SetEnabled(texture);
	
	UpdatePropertyList();
	UpdatePreviewParameters();
}

void seWPTexture::UpdatePreviewParameters(){
	const seTexture * const texture = GetTexture();
	
	if(texture){
		pPreviewEditTCOffset->SetPoint(texture->GetTexCoordOffset());
		pPreviewEditTCScaling->SetPoint(texture->GetTexCoordScaling());
		pPreviewEditTCRotation->SetFloat(texture->GetTexCoordRotation());
		
	}else{
		pPreviewEditTCOffset->SetPoint(decPoint());
		pPreviewEditTCScaling->SetPoint(decPoint());
		pPreviewEditTCRotation->ClearText();
	}
	
	const bool enabled = texture;
	pPreviewEditTCOffset->SetEnabled(enabled);
	pPreviewEditTCScaling->SetEnabled(enabled);
	pPreviewEditTCRotation->SetEnabled(enabled);
}

void seWPTexture::UpdatePropertyList(){
	const seTexture * const texture = GetTexture();
	seProperty * const selection = GetProperty();
	
	pListProperty->RemoveAllItems();
	
	if(texture){
		texture->GetProperties().Visit([&](seProperty *property){
			pListProperty->AddItem(property->GetName(), 0, property);
		});
		
		pListProperty->SortItems();
	}
	
	pListProperty->SetSelectionWithData(selection);
	ShowPropertyPanel();
	UpdateProperty();
}

void seWPTexture::SelectActiveProperty(){
	pListProperty->SetSelectionWithData(GetProperty());
	
	// catches certain cases
	ShowPropertyPanel();
	UpdateProperty();
}

void seWPTexture::ShowPropertyPanel(){
	const seProperty * const property = GetProperty();
	
	if(property){
		switch(property->GetValueType()){
		case seProperty::evtValue:
			pSwitcher->SetCurrent(1);
			break;
			
		case seProperty::evtColor:
			pSwitcher->SetCurrent(2);
			break;
			
		case seProperty::evtImage:
			pSwitcher->SetCurrent(3);
			break;
			
		case seProperty::evtVideo:
			pSwitcher->SetCurrent(4);
			break;
			
		case seProperty::evtConstructed:
			pSwitcher->SetCurrent(5);
			break;
			
		case seProperty::evtMapped:
			pSwitcher->SetCurrent(6);
			break;
			
		default:
			pSwitcher->SetCurrent(0); // empty
		}
		
	}else{
		pSwitcher->SetCurrent(0); // empty
	}
}

void seWPTexture::UpdateProperty(){
	const seProperty * const property = GetProperty();
	
	if(property){
		pEditPropName->SetText(property->GetName());
		pEditPropRenderable->SetText(property->GetRenderableName());
		pEditPropBone->SetText(property->GetBoneName());
		pCBPropertyType->SetSelectionWithData((void*)(intptr_t)property->GetValueType());
		
		pEditPvtValue->SetFloat(property->GetValue());
		
		pClrPvtColor->SetColor(property->GetColor());
		
		pEditPvtImagePath->SetPath(property->GetImagePath());
		
		if(property->GetEngineImage()){
			const deImage &image = *property->GetEngineImage();
			const char *type = "?";
			decString text;
			
			if(image.GetComponentCount() == 1){
				type = "Gray";
				
			}else if(image.GetComponentCount() == 2){
				type = "Gray-Alpha";
				
			}else if(image.GetComponentCount() == 3){
				type = "RGB";
				
			}else if(image.GetComponentCount() == 4){
				type ="RGBA";
			}
			
			text.Format("%dx%dx%d, %s, %d-Bit", image.GetWidth(), image.GetHeight(),
				image.GetDepth(), type, image.GetBitCount());
			pLabPvtImageInfo->SetText(text);
			
		}else{
			pLabPvtImageInfo->ClearText();
		}
		
		pEditPvtVideoPath->SetPath(property->GetVideoPath());
		if(property->GetEngineVideo()){
			const deVideo &video = *property->GetEngineVideo();
			const char *type = "?";
			decString text;
			
			switch(video.GetComponentCount()){
			case 1:
				type = "Grayscale";
				break;
				
			case 2:
				type = "GrayscaleAlpha";
				break;
				
			case 3:
				type = "RGB";
				break;
				
			case 4:
				type = "RGBA";
				break;
			}
			
			text.Format("%dx%d %s, FPS %.1f, Frames %d, Playtime %.1fs", video.GetWidth(),
				video.GetHeight(), type, video.GetFrameRate(), video.GetFrameCount(),
				video.GetPlayTime());
			pLabPvtVideoInfo->SetText(text);
			
		}else{
			pLabPvtVideoInfo->ClearText();
		}
		
		pConstructedClrColor->SetColor(property->GetNodeColor());
		pConstructedEditSize->SetPoint3(property->GetNodeGroup()->GetSize());
		pConstructedCBBitCount->SetSelectionWithData((void*)(intptr_t)property->GetNodeBitCount());
		
	}else{
		pEditPropName->ClearText();
		pEditPropRenderable->ClearText();
		pEditPropBone->ClearText();
		pCBPropertyType->SetSelectionWithData((void*)(intptr_t)seProperty::evtValue);
		
		pEditPvtValue->ClearText();
		
		pClrPvtColor->SetColor(decColor(1.0f, 1.0f, 1.0f));
		
		pEditPvtImagePath->ClearPath();
		pLabPvtImageInfo->ClearText();
		
		pEditPvtVideoPath->ClearPath();
		pLabPvtVideoInfo->ClearText();
		
		pConstructedClrColor->SetColor(decColor(1.0f, 1.0f, 1.0f));
		pConstructedEditSize->SetPoint3(decPoint3());
		pConstructedCBBitCount->SetSelectionWithData((void*)(intptr_t)8);
	}
	
	const bool enabled = property;
	pSpinPvtMappedComponent->SetEnabled(enabled);
	pEditPropName->SetEnabled(enabled);
	pEditPropRenderable->SetEnabled(enabled);
	pEditPropBone->SetEnabled(enabled);
	
	pEditPvtValue->SetEnabled(enabled);
	
	pClrPvtColor->SetEnabled(enabled);
	
	pEditPvtImagePath->SetEnabled(enabled);
	
	pEditPvtVideoPath->SetEnabled(enabled);
	pChkPvtVideoSharedTime->GetAction()->Update();
	
	pConstructedClrColor->SetEnabled(enabled);
	pConstructedEditSize->SetEnabled(enabled);
	pConstructedChkTileX->GetAction()->Update();
	pConstructedChkTileY->GetAction()->Update();
	pConstructedCBBitCount->SetEnabled(enabled);
	
	UpdatePropertyMappedComponent();
}

int seWPTexture::GetPropertyMappedComponentIndex() const{
	return pSpinPvtMappedComponent->GetValue();
}

void seWPTexture::UpdatePropertyMappedComponent(){
	const seProperty * const property = GetProperty();
	const int index = GetPropertyMappedComponentIndex();
	
	pPreventUpdateMappedTarget = true;
	
	if(property){
		pCBPvtMappedTarget->SetSelectionWithData(property->GetMappedComponent(index));
		
	}else{
		pCBPvtMappedTarget->SetSelectionWithData(nullptr);
	}
	
	pPreventUpdateMappedTarget = false;
	
	const bool enabled = property;
	pCBPvtMappedTarget->SetEnabled(enabled);
}

void seWPTexture::UpdatePropertyMappedTargetList(){
	seMapped * const selection = pCBPvtMappedTarget->GetSelectedItem() ?
		(seMapped*)pCBPvtMappedTarget->GetSelectedItem()->GetData() : nullptr;
	
	pPreventUpdateMappedTarget = true;
	pCBPvtMappedTarget->RemoveAllItems();
	
	if(pSkin){
		pSkin->GetMapped().Visit([&](seMapped *mapped){
			pCBPvtMappedTarget->AddItem(mapped->GetName(), nullptr, mapped);
		});
		pCBPvtMappedTarget->SortItems();
	}
	
	pCBPvtMappedTarget->InsertItem(0, "< None >", nullptr, nullptr);
	pCBPvtMappedTarget->SetSelectionWithData(selection);
	pPreventUpdateMappedTarget = false;
}
