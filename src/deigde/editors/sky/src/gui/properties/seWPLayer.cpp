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

#include "seWPLayer.h"
#include "seWPLayerListener.h"
#include "seWindowProperties.h"
#include "../../sky/seSky.h"
#include "../../sky/body/seBody.h"
#include "../../sky/controller/seControllerTarget.h"
#include "../../sky/layer/seLayer.h"
#include "../../sky/link/seLink.h"
#include "../../undosys/layer/seULayerAdd.h"
#include "../../undosys/body/seUBodyAdd.h"
#include "../../undosys/body/seUBodyRemove.h"
#include "../../undosys/body/seUBodyMoveUp.h"
#include "../../undosys/body/seUBodyMoveDown.h"
#include "../../undosys/body/seUBodySetSkin.h"
#include "../../undosys/body/seUBodySetOrientation.h"
#include "../../undosys/body/seUBodySetSize.h"
#include "../../undosys/body/seUBodySetColor.h"
#include "../../undosys/layer/seULayerRemove.h"
#include "../../undosys/layer/seULayerMoveUp.h"
#include "../../undosys/layer/seULayerMoveDown.h"
#include "../../undosys/layer/seULayerSetName.h"
#include "../../undosys/layer/seULayerSetSkin.h"
#include "../../undosys/layer/seULayerSetOffset.h"
#include "../../undosys/layer/seULayerSetOrientation.h"
#include "../../undosys/layer/seULayerSetColor.h"
#include "../../undosys/layer/seULayerSetIntensity.h"
#include "../../undosys/layer/seULayerSetTransparency.h"
#include "../../undosys/layer/seULayerToggleMulBySkyLight.h"
#include "../../undosys/layer/seULayerToggleMulBySkyColor.h"
#include "../../undosys/layer/seULayerSetLightColor.h"
#include "../../undosys/layer/seULayerSetLightIntensity.h"
#include "../../undosys/layer/seULayerSetAmbientIntensity.h"
#include "../../undosys/layer/seULayerSetLightOrientation.h"
#include "../../undosys/target/seUTargetAddLink.h"
#include "../../undosys/target/seUTargetRemoveLink.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPLayer &pPanel;
	
public:
	cBaseTextFieldListener(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSky * const sky = pPanel.GetSky();
		seLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSky *sky, seLayer *layer) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	seWPLayer &pPanel;
	
public:
	cBaseEditVectorListener(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		seSky * const sky = pPanel.GetSky();
		seLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, seSky *sky, seLayer *layer) = 0;
};

class cBaseEditVector2Listener : public igdeEditVector2Listener{
protected:
	seWPLayer &pPanel;
	
public:
	cBaseEditVector2Listener(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		seSky * const sky = pPanel.GetSky();
		seLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector2->GetVector2(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector2 &vector, seSky *sky, seLayer *layer) = 0;
};

class cBaseColorBoxListener : public igdeColorBoxListener{
protected:
	seWPLayer &pPanel;
	
public:
	cBaseColorBoxListener(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		seSky * const sky = pPanel.GetSky();
		seLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(colorBox->GetColor(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decColor &color, seSky *sky, seLayer *layer) = 0;
};

class cBasePathListener : public igdeEditPathListener{
protected:
	seWPLayer &pPanel;
	
public:
	cBasePathListener(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		seSky * const sky = pPanel.GetSky();
		seLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editPath->GetPath(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decString &path, seSky *sky, seLayer *layer) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPLayer &pPanel;
	
public:
	cBaseAction(seWPLayer &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	cBaseAction(seWPLayer &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description),
	pPanel(panel){}
	
	cBaseAction(seWPLayer &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(sky));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSky *sky) = 0;
	
	virtual void Update(){
		seSky * const sky = pPanel.GetSky();
		if(sky){
			UpdateSky(*sky);
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateSky(const seSky &){
		SetEnabled(true);
	}
};

class cBaseActionLayer : public cBaseAction{
public:
	cBaseActionLayer(seWPLayer &panel, const char *text, const char *description) :
	cBaseAction(panel, text, description){}
	
	cBaseActionLayer(seWPLayer &panel, igdeIcon *icon, const char *description) :
	cBaseAction(panel, icon, description){}
	
	cBaseActionLayer(seWPLayer &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	virtual igdeUndo::Ref OnAction(seSky *sky){
		seLayer * const layer = pPanel.GetLayer();
		if(layer){
			return OnActionLayer(sky, layer);
			
		}else{
			return {};
		}
	}
	
	virtual igdeUndo::Ref OnActionLayer(seSky *sky, seLayer *layer) = 0;
	
	void UpdateSky (const seSky &sky) override{
		seLayer * const layer = pPanel.GetLayer();
		if(layer){
			UpdateLayer(sky, *layer);
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateLayer(const seSky &, const seLayer &){
		SetEnabled(true);
	}
};



class cListLayers : public igdeListBoxListener{
	seWPLayer &pPanel;
public:
	typedef deTObjectReference<cListLayers> Ref;
	cListLayers(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		sky->SetActiveLayer(selection ? (seLayer*)selection->GetData() : nullptr);
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand(menu, pPanel.GetActionLayerAdd());
		helper.MenuCommand(menu, pPanel.GetActionLayerRemove());
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionLayerUp());
		helper.MenuCommand(menu, pPanel.GetActionLayerDown());
	}
};

class cActionLayerAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionLayerAdd> Ref;
	cActionLayerAdd(seWPLayer &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a layer to the end of the list."){}
	
	igdeUndo::Ref OnAction(seSky *sky) override{
		const seLayer::Ref layer(seLayer::Ref::New(pPanel.GetEnvironment()));
		return seULayerAdd::Ref::New(sky, layer);
	}
};

class cActionLayerRemove : public cBaseActionLayer{
public:
	typedef deTObjectReference<cActionLayerRemove> Ref;
	cActionLayerRemove(seWPLayer &panel) : cBaseActionLayer(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove the selected layer."){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		return seULayerRemove::Ref::New(layer);
	}
};

class cActionLayerUp : public cBaseActionLayer{
	igdeListBox &pListBox;
public:
	typedef deTObjectReference<cActionLayerUp> Ref;
	cActionLayerUp(seWPLayer &panel, igdeListBox &listBox) : cBaseActionLayer(panel,
		"Move Up", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move layer up in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		return seULayerMoveUp::Ref::New(layer);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pListBox.GetSelection() > 0);
	}
};

class cActionLayerDown : public cBaseActionLayer{
	igdeListBox &pListBox;
public:
	typedef deTObjectReference<cActionLayerDown> Ref;
	cActionLayerDown(seWPLayer &panel, igdeListBox &listBox) : cBaseActionLayer(panel,
		"Move Down", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move layer down in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		return seULayerMoveDown::Ref::New(layer);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pListBox.GetSelection() >= 0 && pListBox.GetSelection() < pListBox.GetItems().GetCount() - 1);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seLayer *layer) override{
		if(textField->GetText() == layer->GetName()){
			return {};
		}
		return seULayerSetName::Ref::New(layer, textField->GetText());
	}
};

class cPathSkin : public cBasePathListener{
public:
	typedef deTObjectReference<cPathSkin> Ref;
	cPathSkin(seWPLayer &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, seSky*, seLayer *layer) override{
		if(layer->GetSkinPath() == path){
			return {};
		}
		return seULayerSetSkin::Ref::New(layer, path);
	}
};

class cEditOffset : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditOffset> Ref;
	cEditOffset(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, seSky*, seLayer *layer) override{
		if(layer->GetOffset().IsEqualTo(vector)){
			return {};
		}
		return seULayerSetOffset::Ref::New(layer, vector);
	}
};

class cEditOrientation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditOrientation> Ref;
	cEditOrientation(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, seSky*, seLayer *layer) override{
		if(layer->GetOrientation().IsEqualTo(vector)){
			return {};
		}
		return seULayerSetOrientation::Ref::New(layer, vector);
	}
};

class cEditColor : public cBaseColorBoxListener{
public:
	typedef deTObjectReference<cEditColor> Ref;
	cEditColor(seWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(const decColor &color, seSky*, seLayer *layer) override{
		if(layer->GetColor().IsEqualTo(color)){
			return {};
		}
		return seULayerSetColor::Ref::New(layer, color);
	}
};

class cTextIntensity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextIntensity> Ref;
	cTextIntensity(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seLayer *layer) override{
		const float value = textField->GetFloat();
		if(fabsf(layer->GetIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return seULayerSetIntensity::Ref::New(layer, value);
	}
};

class cSliderTransparency : public igdeEditSliderTextListener{
	seWPLayer &pPanel;
	igdeUndo::Ref pUndo;
public:
	typedef deTObjectReference<cSliderTransparency> Ref;
	cSliderTransparency(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		seSky * const sky = pPanel.GetSky();
		seLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		if(pUndo){
			((seULayerSetTransparency&)(igdeUndo&)pUndo).SetNewTransparency(sliderText->GetValue());
			pUndo->Redo();
			
		}else{
			pUndo = seULayerSetTransparency::Ref::New(layer, sliderText->GetValue());
			sky->GetUndoSystem()->Add(pUndo);
		}
	}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		if(!pUndo){
			return;
		}
		
		igdeUndo::Ref undo(pUndo);
		pUndo = nullptr;
		
		((seULayerSetTransparency&)(igdeUndo&)undo).SetNewTransparency(sliderText->GetValue());
		undo->Redo();
	}
};

class cActionMulBySkyLight : public cBaseActionLayer{
public:
	typedef deTObjectReference<cActionMulBySkyLight> Ref;
	cActionMulBySkyLight(seWPLayer &panel) : cBaseActionLayer(panel, "Multiply By Sky Light",
		"Determines if the layer intensity is multiplied by the sky light intensity"){ }
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		return seULayerToggleMulBySkyLight::Ref::New(layer);
	}
};

class cActionMulBySkyColor : public cBaseActionLayer{
public:
	typedef deTObjectReference<cActionMulBySkyColor> Ref;
	cActionMulBySkyColor(seWPLayer &panel) : cBaseActionLayer(panel, "Multiply By Sky Color",
		"Determines if the layer color is multiplied by the sky light color"){ }
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		return seULayerToggleMulBySkyColor::Ref::New(layer);
	}
};

class cEditLightColor : public cBaseColorBoxListener{
public:
	typedef deTObjectReference<cEditLightColor> Ref;
	cEditLightColor(seWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(const decColor &color, seSky*, seLayer *layer) override{
		if(layer->GetLightColor().IsEqualTo(color)){
			return {};
		}
		return seULayerSetLightColor::Ref::New(layer, color);
	}
};

class cTextLightIntensity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLightIntensity> Ref;
	cTextLightIntensity(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seLayer *layer) override{
		const float value = textField->GetFloat();
		if(fabsf(layer->GetLightIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return seULayerSetLightIntensity::Ref::New(layer, value);
	}
};

class cTextAmbientIntensity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextAmbientIntensity> Ref;
	cTextAmbientIntensity(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seLayer *layer) override{
		const float value = textField->GetFloat();
		if(fabsf(layer->GetAmbientIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return seULayerSetAmbientIntensity::Ref::New(layer, value);
	}
};

class cEditLightOrientation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLightOrientation> Ref;
	cEditLightOrientation(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, seSky*, seLayer *layer) override{
		if(layer->GetLightOrientation().IsEqualTo(vector)){
			return {};
		}
		return seULayerSetLightOrientation::Ref::New(layer, vector);
	}
};


class cSpinBody : public igdeSpinTextFieldListener{
	seWPLayer &pPanel;
public:
	typedef deTObjectReference<cSpinBody> Ref;
	cSpinBody(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		seLayer * const layer = pPanel.GetLayer();
		if(!layer){
			return;
		}
		
		const int index = textField->GetValue();
		
		if(index == -1){
			layer->SetActiveBody(nullptr);
			
		}else{
			layer->SetActiveBody(layer->GetBodies().GetAt(index));
		}
	}
};

class cActionBody : public cBaseActionLayer{
	igdeButton::Ref &pButton;
public:
	typedef deTObjectReference<cActionBody> Ref;
	cActionBody(seWPLayer &panel, igdeButton::Ref &button) :
	cBaseActionLayer(panel, "...", "Body menu"), pButton(button){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer*){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New(pPanel.GetEnvironment()));
		helper.MenuCommand(menu, pPanel.GetActionBodyAdd());
		helper.MenuCommand(menu, pPanel.GetActionBodyRemove());
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionBodyUp());
		helper.MenuCommand(menu, pPanel.GetActionBodyDown());
		menu->PopupBottom(pButton);
		return {};
	}
};

class cActionBodyAdd : public cBaseActionLayer{
public:
	typedef deTObjectReference<cActionBodyAdd> Ref;
	cActionBodyAdd(seWPLayer &panel) : cBaseActionLayer(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add body to end of list"){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		const seBody::Ref refBody(seBody::Ref::New(pPanel.GetEngine()));
		return seUBodyAdd::Ref::New(layer, (seBody*)refBody.operator->());
	}
};

class cActionBodyRemove : public cBaseActionLayer{
public:
	typedef deTObjectReference<cActionBodyRemove> Ref;
	cActionBodyRemove(seWPLayer &panel) : cBaseActionLayer(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected body from list"){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		if(!body){
			return {};
		}
		return seUBodyRemove::Ref::New(body);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pPanel.GetBody());
	}
};

class cActionBodyUp : public cBaseActionLayer{
	igdeSpinTextField &pSpinTextField;
public:
	typedef deTObjectReference<cActionBodyUp> Ref;
	cActionBodyUp(seWPLayer &panel, igdeSpinTextField &spinTextField) :
	cBaseActionLayer(panel, "Move Up",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move body up in the list"),
	pSpinTextField(spinTextField){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		const int index = pSpinTextField.GetValue();
		if(body && index == pSpinTextField.GetLower()){
			return {};
		}
		return seUBodyMoveUp::Ref::New(body);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pSpinTextField.GetValue() > pSpinTextField.GetLower());
	}
};

class cActionBodyDown : public cBaseActionLayer{
	igdeSpinTextField &pSpinTextField;
public:
	typedef deTObjectReference<cActionBodyDown> Ref;
	cActionBodyDown(seWPLayer &panel, igdeSpinTextField &spinTextField) :
	cBaseActionLayer(panel, "Move Down",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move body down in the list"),
	pSpinTextField(spinTextField){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		const int index = pSpinTextField.GetValue();
		if(body && index == pSpinTextField.GetUpper()){
			return {};
		}
		return seUBodyMoveDown::Ref::New(body);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pSpinTextField.GetValue() < pSpinTextField.GetUpper());
	}
};

class cPathBodySkin : public cBasePathListener{
public:
	typedef deTObjectReference<cPathBodySkin> Ref;
	cPathBodySkin(seWPLayer &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, seSky*, seLayer*) override{
		seBody * const body = pPanel.GetBody();
		if(!body || body->GetSkinPath() == path){
			return {};
		}
		return seUBodySetSkin::Ref::New(body, path);
	}
};

class cEditBodyOrientation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditBodyOrientation> Ref;
	cEditBodyOrientation(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, seSky*, seLayer*) override{
		seBody * const body = pPanel.GetBody();
		if(!body || body->GetOrientation().IsEqualTo(vector)){
			return {};
		}
		return seUBodySetOrientation::Ref::New(body, vector);
	}
};

class cEditBodySize : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditBodySize> Ref;
	cEditBodySize(seWPLayer &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector2 &vector, seSky*, seLayer*) override{
		seBody * const body = pPanel.GetBody();
		if(!body || body->GetSize().IsEqualTo(vector)){
			return {};
		}
		return seUBodySetSize::Ref::New(body, vector);
	}
};

class cEditBodyColor : public cBaseColorBoxListener{
public:
	typedef deTObjectReference<cEditBodyColor> Ref;
	cEditBodyColor(seWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(const decColor &color, seSky*, seLayer *layer) override{
		seBody * const body = pPanel.GetBody();
		if(layer->GetColor().IsEqualTo(color)){
			return {};
		}
		return seUBodySetColor::Ref::New(body, color);
	}
};


class cComboTarget : public igdeComboBoxListener{
	seWPLayer &pPanel;
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(pPanel.GetLayer() && comboBox->GetSelectedItem()){
			pPanel.GetLayer()->SetActiveTarget((deSkyLayer::eTargets)(intptr_t)
				comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cListLinks : public igdeListBoxListener{
	seWPLayer &pPanel;
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(seWPLayer &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseActionLayer{
	igdeComboBox &pComboLinks;
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	cActionLinkAdd(seWPLayer &panel, igdeComboBox &comboLinks) : cBaseActionLayer(panel,
		"", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"Add link to target if not present"),
	pComboLinks(comboLinks){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		const igdeListItem * const selection = pComboLinks.GetSelectedItem();
		if(!selection){
			return {};
		}
		
		const deSkyLayer::eTargets target = layer->GetActiveTarget();
		seLink * const link = (seLink*)selection->GetData();
		if(layer->GetTarget(target).GetLinks().Has(link)){
			return {};
		}
		
		return seUTargetAddLink::Ref::New(layer, target, link);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pComboLinks.GetSelectedItem());
	}
};

class cActionLinkRemove : public cBaseActionLayer{
	igdeListBox &pListLinks;
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	cActionLinkRemove(seWPLayer &panel, igdeListBox &listLinks) : cBaseActionLayer(panel,
		"Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected link from target"),
	pListLinks(listLinks){}
	
	virtual igdeUndo::Ref OnActionLayer(seSky*, seLayer *layer){
		const igdeListItem * const selection = pListLinks.GetSelectedItem();
		if(!selection){
			return {};
		}
		
		const deSkyLayer::eTargets target = layer->GetActiveTarget();
		seLink * const link = (seLink*)selection->GetData();
		if(!layer->GetTarget(target).GetLinks().Has(link)){
			return {};
		}
		
		return seUTargetRemoveLink::Ref::New(layer, target, link);
	}
	
	void UpdateLayer(const seSky &, const seLayer &layer) override{
		SetEnabled(layer.GetTarget(layer.GetActiveTarget()).GetLinks().GetCount() > 0);
	}
};

}



// Class seWPLayer
////////////////////

// Constructor, destructor
////////////////////////////

seWPLayer::seWPLayer(seWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine, frameLine2;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = seWPLayerListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// layer list
	helper.GroupBoxFlow(content, groupBox, "Layers:");
	helper.ListBox(groupBox, 8, "Layers", pListLayer, cListLayers::Ref::New(*this));
	
	pActionLayerAdd = cActionLayerAdd::Ref::New(*this);
	pActionLayerRemove = cActionLayerRemove::Ref::New(*this);
	pActionLayerUp = cActionLayerUp::Ref::New(*this, pListLayer);
	pActionLayerDown = cActionLayerDown::Ref::New(*this, pListLayer);
	
	// layer settings
	helper.GroupBox(content, groupBox, "Layer Settings:");
	helper.EditString(groupBox, "Name:", "Name of the layer", pEditName, cTextName::Ref::New(*this));
	helper.EditPath(groupBox, "Skin:", "Background skin of the layer",
		igdeEnvironment::efpltSkin, pEditSkin, cPathSkin::Ref::New(*this));
	helper.EditVector(groupBox, "Offset:", "Offset of the layer", pEditOffset, cEditOffset::Ref::New(*this));
	helper.EditVector(groupBox, "Orientation:", "Orientation of the layer", pEditOrientation, cEditOrientation::Ref::New(*this));
	
	helper.FormLine(groupBox, "Color:",
		"Color and intensity the content layer is multiplied with", frameLine);
	helper.ColorBox(frameLine, "Color or layer content", pClrLayer, cEditColor::Ref::New(*this));
	helper.EditFloat(frameLine, "Intensity of the layer content", pEditIntensity, cTextIntensity::Ref::New(*this));
	
	helper.EditSliderText(groupBox, "Transparency:", "Transparency of the layer",
		0.0f, 1.0f, 3, 3, 0.1f, pSldTransparency, cSliderTransparency::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkMulBySkyLight, cActionMulBySkyLight::Ref::New(*this));
	helper.CheckBox(groupBox, pChkMulBySkyColor, cActionMulBySkyColor::Ref::New(*this));
	
	// light settings
	helper.GroupBox(content, groupBox, "Light Settings:");
	
	helper.FormLine(groupBox, "Light:", "Light color, intensity and ambient intensity", frameLine);
	helper.ColorBox(frameLine, "Color or layer light", pClrLight, cEditLightColor::Ref::New(*this));
	helper.EditFloat(frameLine, "Intensity of the layer light", pEditLightIntensity, cTextLightIntensity::Ref::New(*this));
	helper.EditFloat(frameLine, "Ambient intensity of the layer light", pEditAmbientIntensity, cTextAmbientIntensity::Ref::New(*this));
	
	helper.EditVector(groupBox, "Orientation:", "Orientation of the light relative to the layer",
		pEditLightOrientation, cEditLightOrientation::Ref::New(*this));
	
	// bodies
	helper.GroupBox(content, groupBox, "Bodies:");
	
	helper.FormLine(groupBox, "Body:", "Body to edit", frameLine);
	helper.EditSpinInteger(frameLine, "Body to edit", 0, 0, pSpinBody, cSpinBody::Ref::New(*this));
	
	pActionBodyAdd = cActionBodyAdd::Ref::New(*this);
	pActionBodyRemove = cActionBodyRemove::Ref::New(*this);
	pActionBodyUp = cActionBodyUp::Ref::New(*this, pSpinBody);
	pActionBodyDown = cActionBodyDown::Ref::New(*this, pSpinBody);
	
	helper.Button(frameLine, pBtnBody, cActionBody::Ref::New(*this, pBtnBody));
	
	helper.EditPath(groupBox, "Skin:", "Skin for body",
		igdeEnvironment::efpltSkin, pEditBodySkin, cPathBodySkin::Ref::New(*this));
	helper.EditVector(groupBox, "Orientation:", "Orientation of the body",
		pEditBodyOrienation, cEditBodyOrientation::Ref::New(*this));
	helper.EditVector2(groupBox, "Size:", "Size of the body in degrees",
		pEditBodySize, cEditBodySize::Ref::New(*this));
	helper.ColorBox(groupBox, "Color:", "Color the content body is multiplied with",
		pClrBody, cEditBodyColor::Ref::New(*this));
	
	// targets
	helper.GroupBoxFlow(content, groupBox, "Targets:");
	
	frameLine = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(frameLine);
	
	helper.ComboBox(frameLine, "Target:", "Select target to edit", pCBTarget, cComboTarget::Ref::New(*this));
	pCBTarget->AddItem("Offset X", nullptr, (void*)(intptr_t)deSkyLayer::etOffsetX);
	pCBTarget->AddItem("Offset Y", nullptr, (void*)(intptr_t)deSkyLayer::etOffsetY);
	pCBTarget->AddItem("Offset Z", nullptr, (void*)(intptr_t)deSkyLayer::etOffsetZ);
	pCBTarget->AddItem("Orientation X", nullptr, (void*)(intptr_t)deSkyLayer::etOrientationX);
	pCBTarget->AddItem("Orientation Y", nullptr, (void*)(intptr_t)deSkyLayer::etOrientationY);
	pCBTarget->AddItem("Orientation Z", nullptr, (void*)(intptr_t)deSkyLayer::etOrientationZ);
	pCBTarget->AddItem("Rotation X", nullptr, (void*)(intptr_t)deSkyLayer::etRotationX);
	pCBTarget->AddItem("Rotation Y", nullptr, (void*)(intptr_t)deSkyLayer::etRotationY);
	pCBTarget->AddItem("Rotation Z", nullptr, (void*)(intptr_t)deSkyLayer::etRotationZ);
	pCBTarget->AddItem("Color Red", nullptr, (void*)(intptr_t)deSkyLayer::etColorR);
	pCBTarget->AddItem("Color Green", nullptr, (void*)(intptr_t)deSkyLayer::etColorG);
	pCBTarget->AddItem("Color Blue", nullptr, (void*)(intptr_t)deSkyLayer::etColorB);
	pCBTarget->AddItem("Intensity", nullptr, (void*)(intptr_t)deSkyLayer::etIntensity);
	pCBTarget->AddItem("Transparency", nullptr, (void*)(intptr_t)deSkyLayer::etTransparency);
	pCBTarget->AddItem("Light Color Red", nullptr, (void*)(intptr_t)deSkyLayer::etLightColorR);
	pCBTarget->AddItem("Light Color Green", nullptr, (void*)(intptr_t)deSkyLayer::etLightColorG);
	pCBTarget->AddItem("Light Color Blue", nullptr, (void*)(intptr_t)deSkyLayer::etLightColorB);
	pCBTarget->AddItem("Light Intensity", nullptr, (void*)(intptr_t)deSkyLayer::etLightIntensity);
	pCBTarget->AddItem("Light Ambient Intensity", nullptr, (void*)(intptr_t)deSkyLayer::etAmbientIntensity);
	
	helper.FormLineStretchFirst(frameLine, "Link:", "Links", frameLine2);
	helper.ComboBox(frameLine2, "Links", pCBLinks, {});
	pActionLinkAdd = cActionLinkAdd::Ref::New(*this, pCBLinks);
	helper.Button(frameLine2, pBtnLinkAdd, pActionLinkAdd);
	
	helper.ListBox(groupBox, 8, "Links", pListLinks, cListLinks::Ref::New(*this));
	pListLinks->SetDefaultSorter();
	
	pActionLinkRemove = cActionLinkRemove::Ref::New(*this, pListLinks);
}

seWPLayer::~seWPLayer(){
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = nullptr;
	}
}



// Management
///////////////

void seWPLayer::SetSky(seSky *sky){
	if(sky == pSky){
		return;
	}
	
	if(pSky){
		pSky->RemoveListener(pListener);
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
	}
	
	UpdateLayerList();
	UpdateLinkList();
	OnSkyPathChanged();
}

void seWPLayer::OnSkyPathChanged(){
	if(pSky){
		pEditSkin->SetBasePath(pSky->GetDirectoryPath());
		pEditBodySkin->SetBasePath(pSky->GetDirectoryPath());
		
	}else{
		pEditSkin->SetBasePath("");
		pEditBodySkin->SetBasePath("");
	}
}

seLayer *seWPLayer::GetLayer() const{
	return pSky ? pSky->GetActiveLayer(): nullptr;
}

seBody *seWPLayer::GetBody() const{
	const seLayer * const layer = GetLayer();
	return layer ? layer->GetActiveBody() : nullptr;
}

void seWPLayer::UpdateLayerList(){
	pListLayer->RemoveAllItems();
	
	if(pSky){
		pSky->GetLayers().VisitIndexed([&](int i, seLayer *layer){
			decString text;
			text.Format("%d: %s", i, layer->GetName().GetString());
			pListLayer->AddItem(text, nullptr, layer);
		});
	}
	
	SelectActiveLayer();
}

void seWPLayer::SelectActiveLayer(){
	seLayer * const layer = GetLayer();
	const int selection = pListLayer->IndexOfItemWithData(layer);
	
	pListLayer->SetSelection(selection);
	if(selection != -1){
		pListLayer->MakeItemVisible(selection);
	}
	
	UpdateLayer();
}

void seWPLayer::UpdateLayer(){
	const seLayer * const layer = GetLayer();
	
	if(layer){
		pEditName->SetText(layer->GetName());
		pEditSkin->SetPath(layer->GetSkinPath());
		pEditOffset->SetVector(layer->GetOffset());
		pEditOrientation->SetVector(layer->GetOrientation());
		pClrLayer->SetColor(layer->GetColor());
		pEditIntensity->SetFloat(layer->GetIntensity());
		pSldTransparency->SetValue(layer->GetTransparency());
		pChkMulBySkyLight->SetChecked(layer->GetMuliplyBySkyLight());
		pChkMulBySkyColor->SetChecked(layer->GetMuliplyBySkyColor());
		
		pClrLight->SetColor(layer->GetLightColor());
		pEditLightIntensity->SetFloat(layer->GetLightIntensity());
		pEditAmbientIntensity->SetFloat(layer->GetAmbientIntensity());
		pEditLightOrientation->SetVector(layer->GetLightOrientation());
		
	}else{
		pEditName->ClearText();
		pEditSkin->ClearPath();
		pEditOffset->SetVector(decVector());
		pEditOrientation->SetVector(decVector());
		pClrLayer->SetColor(decColor());
		pEditIntensity->ClearText();
		pSldTransparency->SetValue(0.0);
		pChkMulBySkyLight->SetChecked(false);
		pChkMulBySkyColor->SetChecked(false);
		
		pClrLight->SetColor(decColor());
		pEditLightIntensity->ClearText();
		pEditAmbientIntensity->ClearText();
		pEditLightOrientation->SetVector(decVector());
	}
	
	const bool enabled = layer != nullptr;
	pEditName->SetEnabled(enabled);
	pEditSkin->SetEnabled(enabled);
	pEditOffset->SetEnabled(enabled);
	pEditOrientation->SetEnabled(enabled);
	pClrLayer->SetEnabled(enabled);
	pEditIntensity->SetEnabled(enabled);
	pSldTransparency->SetEnabled(enabled);
	pChkMulBySkyLight->SetEnabled(enabled);
	pChkMulBySkyColor->SetEnabled(enabled);
	
	pClrLight->SetEnabled(enabled);
	pEditLightIntensity->SetEnabled(enabled);
	pEditAmbientIntensity->SetEnabled(enabled);
	pEditLightOrientation->SetEnabled(enabled);
	
	UpdateBodyList();
	SelectActiveTarget();
}

void seWPLayer::UpdateBodyList(){
	seLayer * const layer = GetLayer();
	
	if(layer){
		pSpinBody->SetRange(0, layer->GetBodies().GetCount() - 1);
		
	}else{
		pSpinBody->SetRange(0, 0);
	}
	
	const bool enabled = layer != nullptr;
	pSpinBody->SetEnabled(enabled);
	
	pActionBodyAdd->Update();
	pActionBodyRemove->Update();
	
	SelectActiveBody();
}

void seWPLayer::SelectActiveBody(){
	seBody * const body = GetBody();
	
	if(body){
		seLayer &layer = *body->GetLayer();
		const int index = layer.GetBodies().IndexOf(body);
		pSpinBody->SetValue(index);
		
	}else{
		pSpinBody->SetValue(0);
	}
	
	pActionBodyUp->Update();
	pActionBodyDown->Update();
	
	UpdateBody();
}

void seWPLayer::UpdateBody(){
	seBody * const body = GetBody();
	
	if(body){
		pEditBodySkin->SetPath(body->GetSkinPath());
		pEditBodyOrienation->SetVector(body->GetOrientation());
		pEditBodySize->SetVector2(body->GetSize());
		pClrBody->SetColor(body->GetColor());
		
	}else{
		pEditBodySkin->ClearPath();
		pEditBodyOrienation->SetVector(decVector());
		pEditBodySize->SetVector2(decVector2(1.0f, 1.0f));
		pClrBody->SetColor(decColor());
	}
	
	const bool enabled = body != nullptr;
	pEditBodySkin->SetEnabled(enabled);
	pEditBodyOrienation->SetEnabled(enabled);
	pEditBodySize->SetEnabled(enabled);
	pClrBody->SetEnabled(enabled);
}

void seWPLayer::UpdateLinkList(){
	pCBLinks->RemoveAllItems();
	
	if(pSky){
		pSky->GetLinks().Visit([&](seLink *link){
			pCBLinks->AddItem(link->GetName(), nullptr, link);
		});
	}
	
	pCBLinks->SortItems();
}

void seWPLayer::SelectActiveTarget(){
	seLayer * const layer = GetLayer();
	int selection = 4;
	if(layer){
		selection = pCBTarget->IndexOfItemWithData((void*)(intptr_t)layer->GetActiveTarget());
	}
	
	pCBTarget->SetSelection(selection);
	
	UpdateTarget();
}

void seWPLayer::UpdateTarget(){
	seLayer * const layer = GetLayer();
	
	if(layer){
		const seControllerTarget &target = layer->GetTarget(layer->GetActiveTarget());
		
		pListLinks->RemoveAllItems();
		
		target.GetLinks().Visit([&](seLink *link){
			pListLinks->AddItem(link->GetName(), nullptr, link);
		});
		
		pListLinks->SortItems();
		
	}else{
		pListLinks->RemoveAllItems();
	}
	
	const bool enabled = layer != nullptr;
	pListLinks->SetEnabled(enabled);
	pBtnLinkAdd->SetEnabled(enabled);
}
