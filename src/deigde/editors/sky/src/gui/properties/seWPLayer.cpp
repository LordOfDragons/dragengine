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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, sky, layer)));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seSky *sky, seLayer *layer) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector->GetVector(), sky, layer)));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, seSky *sky, seLayer *layer) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector2->GetVector2(), sky, layer)));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, seSky *sky, seLayer *layer) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(colorBox->GetColor(), sky, layer)));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decColor &color, seSky *sky, seLayer *layer) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editPath->GetPath(), sky, layer)));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decString &path, seSky *sky, seLayer *layer) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(sky)));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(seSky *sky) = 0;
	
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
	
	virtual igdeUndo *OnAction(seSky *sky){
		seLayer * const layer = pPanel.GetLayer();
		if(layer){
			return OnActionLayer(sky, layer);
			
		}else{
			return NULL;
		}
	}
	
	virtual igdeUndo *OnActionLayer(seSky *sky, seLayer *layer) = 0;
	
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
	cListLayers(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		sky->SetActiveLayer(selection ? (seLayer*)selection->GetData() : NULL);
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
	cActionLayerAdd(seWPLayer &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a layer to the end of the list."){}
	
	virtual igdeUndo *OnAction(seSky *sky){
		const seLayer::Ref layer(seLayer::Ref::NewWith(pPanel.GetEnvironment()));
		return new seULayerAdd(sky, layer);
	}
};

class cActionLayerRemove : public cBaseActionLayer{
public:
	cActionLayerRemove(seWPLayer &panel) : cBaseActionLayer(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove the selected layer."){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		return new seULayerRemove(layer);
	}
};

class cActionLayerUp : public cBaseActionLayer{
	igdeListBox &pListBox;
public:
	cActionLayerUp(seWPLayer &panel, igdeListBox &listBox) : cBaseActionLayer(panel,
		"Move Up", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move layer up in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		return new seULayerMoveUp(layer);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pListBox.GetSelection() > 0);
	}
};

class cActionLayerDown : public cBaseActionLayer{
	igdeListBox &pListBox;
public:
	cActionLayerDown(seWPLayer &panel, igdeListBox &listBox) : cBaseActionLayer(panel,
		"Move Down", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move layer down in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		return new seULayerMoveDown(layer);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pListBox.GetSelection() >= 0 && pListBox.GetSelection() < pListBox.GetItemCount() - 1);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	cTextName(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seSky*, seLayer *layer){
		if(textField->GetText() == layer->GetName()){
			return NULL;
		}
		return new seULayerSetName(layer, textField->GetText());
	}
};

class cPathSkin : public cBasePathListener{
public:
	cPathSkin(seWPLayer &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, seSky*, seLayer *layer){
		if(layer->GetSkinPath() == path){
			return NULL;
		}
		return new seULayerSetSkin(layer, path);
	}
};

class cEditOffset : public cBaseEditVectorListener{
public:
	cEditOffset(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, seSky*, seLayer *layer){
		if(layer->GetOffset().IsEqualTo(vector)){
			return NULL;
		}
		return new seULayerSetOffset(layer, vector);
	}
};

class cEditOrientation : public cBaseEditVectorListener{
public:
	cEditOrientation(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, seSky*, seLayer *layer){
		if(layer->GetOrientation().IsEqualTo(vector)){
			return NULL;
		}
		return new seULayerSetOrientation(layer, vector);
	}
};

class cEditColor : public cBaseColorBoxListener{
public:
	cEditColor(seWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(const decColor &color, seSky*, seLayer *layer){
		if(layer->GetColor().IsEqualTo(color)){
			return NULL;
		}
		return new seULayerSetColor(layer, color);
	}
};

class cTextIntensity : public cBaseTextFieldListener{
public:
	cTextIntensity(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seSky*, seLayer *layer){
		const float value = textField->GetFloat();
		if(fabsf(layer->GetIntensity() - value) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new seULayerSetIntensity(layer, value);
	}
};

class cSliderTransparency : public igdeEditSliderTextListener{
	seWPLayer &pPanel;
	igdeUndo::Ref pUndo;
public:
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
			pUndo.TakeOver(new seULayerSetTransparency(layer, sliderText->GetValue()));
			sky->GetUndoSystem()->Add(pUndo);
		}
	}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		if(!pUndo){
			return;
		}
		
		igdeUndo::Ref undo(pUndo);
		pUndo = NULL;
		
		((seULayerSetTransparency&)(igdeUndo&)undo).SetNewTransparency(sliderText->GetValue());
		undo->Redo();
	}
};

class cActionMulBySkyLight : public cBaseActionLayer{
public:
	cActionMulBySkyLight(seWPLayer &panel) : cBaseActionLayer(panel, "Multiply By Sky Light",
		"Determines if the layer intensity is multiplied by the sky light intensity"){ }
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		return new seULayerToggleMulBySkyLight(layer);
	}
};

class cActionMulBySkyColor : public cBaseActionLayer{
public:
	cActionMulBySkyColor(seWPLayer &panel) : cBaseActionLayer(panel, "Multiply By Sky Color",
		"Determines if the layer color is multiplied by the sky light color"){ }
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		return new seULayerToggleMulBySkyColor(layer);
	}
};

class cEditLightColor : public cBaseColorBoxListener{
public:
	cEditLightColor(seWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(const decColor &color, seSky*, seLayer *layer){
		if(layer->GetLightColor().IsEqualTo(color)){
			return NULL;
		}
		return new seULayerSetLightColor(layer, color);
	}
};

class cTextLightIntensity : public cBaseTextFieldListener{
public:
	cTextLightIntensity(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seSky*, seLayer *layer){
		const float value = textField->GetFloat();
		if(fabsf(layer->GetLightIntensity() - value) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new seULayerSetLightIntensity(layer, value);
	}
};

class cTextAmbientIntensity : public cBaseTextFieldListener{
public:
	cTextAmbientIntensity(seWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seSky*, seLayer *layer){
		const float value = textField->GetFloat();
		if(fabsf(layer->GetAmbientIntensity() - value) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new seULayerSetAmbientIntensity(layer, value);
	}
};

class cEditLightOrientation : public cBaseEditVectorListener{
public:
	cEditLightOrientation(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, seSky*, seLayer *layer){
		if(layer->GetLightOrientation().IsEqualTo(vector)){
			return NULL;
		}
		return new seULayerSetLightOrientation(layer, vector);
	}
};


class cSpinBody : public igdeSpinTextFieldListener{
	seWPLayer &pPanel;
public:
	cSpinBody(seWPLayer &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		seLayer * const layer = pPanel.GetLayer();
		if(!layer){
			return;
		}
		
		const int index = textField->GetValue();
		
		if(index == -1){
			layer->SetActiveBody(NULL);
			
		}else{
			layer->SetActiveBody(layer->GetBodies().GetAt(index));
		}
	}
};

class cActionBody : public cBaseActionLayer{
	igdeButton::Ref &pButton;
public:
	cActionBody(seWPLayer &panel, igdeButton::Ref &button) :
	cBaseActionLayer(panel, "...", "Body menu"), pButton(button){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer*){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::NewWith(pPanel.GetEnvironment()));
		helper.MenuCommand(menu, pPanel.GetActionBodyAdd());
		helper.MenuCommand(menu, pPanel.GetActionBodyRemove());
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionBodyUp());
		helper.MenuCommand(menu, pPanel.GetActionBodyDown());
		menu->PopupBottom(pButton);
		return NULL;
	}
};

class cActionBodyAdd : public cBaseActionLayer{
public:
	cActionBodyAdd(seWPLayer &panel) : cBaseActionLayer(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add body to end of list"){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		const seBody::Ref refBody(seBody::Ref::NewWith(pPanel.GetEngine()));
		return new seUBodyAdd(layer, (seBody*)refBody.operator->());
	}
};

class cActionBodyRemove : public cBaseActionLayer{
public:
	cActionBodyRemove(seWPLayer &panel) : cBaseActionLayer(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected body from list"){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		if(!body){
			return NULL;
		}
		return new seUBodyRemove(body);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pPanel.GetBody());
	}
};

class cActionBodyUp : public cBaseActionLayer{
	igdeSpinTextField &pSpinTextField;
public:
	cActionBodyUp(seWPLayer &panel, igdeSpinTextField &spinTextField) :
	cBaseActionLayer(panel, "Move Up",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move body up in the list"),
	pSpinTextField(spinTextField){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		const int index = pSpinTextField.GetValue();
		if(body && index == pSpinTextField.GetLower()){
			return NULL;
		}
		return new seUBodyMoveUp(body);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pSpinTextField.GetValue() > pSpinTextField.GetLower());
	}
};

class cActionBodyDown : public cBaseActionLayer{
	igdeSpinTextField &pSpinTextField;
public:
	cActionBodyDown(seWPLayer &panel, igdeSpinTextField &spinTextField) :
	cBaseActionLayer(panel, "Move Down",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move body down in the list"),
	pSpinTextField(spinTextField){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		const int index = pSpinTextField.GetValue();
		if(body && index == pSpinTextField.GetUpper()){
			return NULL;
		}
		return new seUBodyMoveDown(body);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pSpinTextField.GetValue() < pSpinTextField.GetUpper());
	}
};

class cPathBodySkin : public cBasePathListener{
public:
	cPathBodySkin(seWPLayer &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		if(!body || body->GetSkinPath() == path){
			return NULL;
		}
		return new seUBodySetSkin(body, path);
	}
};

class cEditBodyOrientation : public cBaseEditVectorListener{
public:
	cEditBodyOrientation(seWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		if(!body || body->GetOrientation().IsEqualTo(vector)){
			return NULL;
		}
		return new seUBodySetOrientation(body, vector);
	}
};

class cEditBodySize : public cBaseEditVector2Listener{
public:
	cEditBodySize(seWPLayer &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, seSky*, seLayer*){
		seBody * const body = pPanel.GetBody();
		if(!body || body->GetSize().IsEqualTo(vector)){
			return NULL;
		}
		return new seUBodySetSize(body, vector);
	}
};

class cEditBodyColor : public cBaseColorBoxListener{
public:
	cEditBodyColor(seWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(const decColor &color, seSky*, seLayer *layer){
		seBody * const body = pPanel.GetBody();
		if(layer->GetColor().IsEqualTo(color)){
			return NULL;
		}
		return new seUBodySetColor(body, color);
	}
};


class cComboTarget : public igdeComboBoxListener{
	seWPLayer &pPanel;
public:
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
	cListLinks(seWPLayer &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseActionLayer{
	igdeComboBox &pComboLinks;
public:
	cActionLinkAdd(seWPLayer &panel, igdeComboBox &comboLinks) : cBaseActionLayer(panel,
		"", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"Add link to target if not present"),
	pComboLinks(comboLinks){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		const igdeListItem * const selection = pComboLinks.GetSelectedItem();
		if(!selection){
			return NULL;
		}
		
		const deSkyLayer::eTargets target = layer->GetActiveTarget();
		seLink * const link = (seLink*)selection->GetData();
		if(layer->GetTarget(target).GetLinks().Has(link)){
			return NULL;
		}
		
		return new seUTargetAddLink(layer, target, link);
	}
	
	void UpdateLayer(const seSky &, const seLayer &) override{
		SetEnabled(pComboLinks.GetSelectedItem());
	}
};

class cActionLinkRemove : public cBaseActionLayer{
	igdeListBox &pListLinks;
public:
	cActionLinkRemove(seWPLayer &panel, igdeListBox &listLinks) : cBaseActionLayer(panel,
		"Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected link from target"),
	pListLinks(listLinks){}
	
	virtual igdeUndo *OnActionLayer(seSky*, seLayer *layer){
		const igdeListItem * const selection = pListLinks.GetSelectedItem();
		if(!selection){
			return NULL;
		}
		
		const deSkyLayer::eTargets target = layer->GetActiveTarget();
		seLink * const link = (seLink*)selection->GetData();
		if(!layer->GetTarget(target).GetLinks().Has(link)){
			return NULL;
		}
		
		return new seUTargetRemoveLink(layer, target, link);
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
pWindowProperties(windowProperties),
pListener(NULL),
pSky(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine, frameLine2;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPLayerListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	// layer list
	helper.GroupBoxFlow(content, groupBox, "Layers:");
	helper.ListBox(groupBox, 8, "Layers", pListLayer, new cListLayers(*this));
	
	pActionLayerAdd.TakeOver(new cActionLayerAdd(*this));
	pActionLayerRemove.TakeOver(new cActionLayerRemove(*this));
	pActionLayerUp.TakeOver(new cActionLayerUp(*this, pListLayer));
	pActionLayerDown.TakeOver(new cActionLayerDown(*this, pListLayer));
	
	// layer settings
	helper.GroupBox(content, groupBox, "Layer Settings:");
	helper.EditString(groupBox, "Name:", "Name of the layer", pEditName, new cTextName(*this));
	helper.EditPath(groupBox, "Skin:", "Background skin of the layer",
		igdeEnvironment::efpltSkin, pEditSkin, new cPathSkin(*this));
	helper.EditVector(groupBox, "Offset:", "Offset of the layer", pEditOffset,
		new cEditOffset(*this));
	helper.EditVector(groupBox, "Orientation:", "Orientation of the layer", pEditOrientation,
		new cEditOrientation(*this));
	
	helper.FormLine(groupBox, "Color:",
		"Color and intensity the content layer is multiplied with", frameLine);
	helper.ColorBox(frameLine, "Color or layer content", pClrLayer, new cEditColor(*this));
	helper.EditFloat(frameLine, "Intensity of the layer content", pEditIntensity,
		new cTextIntensity(*this));
	
	helper.EditSliderText(groupBox, "Transparency:", "Transparency of the layer",
		0.0f, 1.0f, 3, 3, 0.1f, pSldTransparency, new cSliderTransparency(*this));
	
	helper.CheckBox(groupBox, pChkMulBySkyLight, new cActionMulBySkyLight(*this), true);
	helper.CheckBox(groupBox, pChkMulBySkyColor, new cActionMulBySkyColor(*this), true);
	
	// light settings
	helper.GroupBox(content, groupBox, "Light Settings:");
	
	helper.FormLine(groupBox, "Light:", "Light color, intensity and ambient intensity", frameLine);
	helper.ColorBox(frameLine, "Color or layer light", pClrLight, new cEditLightColor(*this));
	helper.EditFloat(frameLine, "Intensity of the layer light", pEditLightIntensity,
		new cTextLightIntensity(*this));
	helper.EditFloat(frameLine, "Ambient intensity of the layer light", pEditAmbientIntensity,
		new cTextAmbientIntensity(*this));
	
	helper.EditVector(groupBox, "Orientation:", "Orientation of the light relative to the layer",
		pEditLightOrientation, new cEditLightOrientation(*this));
	
	// bodies
	helper.GroupBox(content, groupBox, "Bodies:");
	
	helper.FormLine(groupBox, "Body:", "Body to edit", frameLine);
	helper.EditSpinInteger(frameLine, "Body to edit", 0, 0, pSpinBody, new cSpinBody(*this));
	
	pActionBodyAdd.TakeOver(new cActionBodyAdd(*this));
	pActionBodyRemove.TakeOver(new cActionBodyRemove(*this));
	pActionBodyUp.TakeOver(new cActionBodyUp(*this, pSpinBody));
	pActionBodyDown.TakeOver(new cActionBodyDown(*this, pSpinBody));
	
	helper.Button(frameLine, pBtnBody, new cActionBody(*this, pBtnBody), true);
	
	helper.EditPath(groupBox, "Skin:", "Skin for body",
		igdeEnvironment::efpltSkin, pEditBodySkin, new cPathBodySkin(*this));
	helper.EditVector(groupBox, "Orientation:", "Orientation of the body",
		pEditBodyOrienation, new cEditBodyOrientation(*this));
	helper.EditVector2(groupBox, "Size:", "Size of the body in degrees",
		pEditBodySize, new cEditBodySize(*this));
	helper.ColorBox(groupBox, "Color:", "Color the content body is multiplied with",
		pClrBody, new cEditBodyColor(*this));
	
	// targets
	helper.GroupBoxFlow(content, groupBox, "Targets:");
	
	frameLine.TakeOver(new igdeContainerForm(env));
	groupBox->AddChild(frameLine);
	
	helper.ComboBox(frameLine, "Target:", "Select target to edit", pCBTarget, new cComboTarget(*this));
	pCBTarget->AddItem("Offset X", NULL, (void*)(intptr_t)deSkyLayer::etOffsetX);
	pCBTarget->AddItem("Offset Y", NULL, (void*)(intptr_t)deSkyLayer::etOffsetY);
	pCBTarget->AddItem("Offset Z", NULL, (void*)(intptr_t)deSkyLayer::etOffsetZ);
	pCBTarget->AddItem("Orientation X", NULL, (void*)(intptr_t)deSkyLayer::etOrientationX);
	pCBTarget->AddItem("Orientation Y", NULL, (void*)(intptr_t)deSkyLayer::etOrientationY);
	pCBTarget->AddItem("Orientation Z", NULL, (void*)(intptr_t)deSkyLayer::etOrientationZ);
	pCBTarget->AddItem("Rotation X", NULL, (void*)(intptr_t)deSkyLayer::etRotationX);
	pCBTarget->AddItem("Rotation Y", NULL, (void*)(intptr_t)deSkyLayer::etRotationY);
	pCBTarget->AddItem("Rotation Z", NULL, (void*)(intptr_t)deSkyLayer::etRotationZ);
	pCBTarget->AddItem("Color Red", NULL, (void*)(intptr_t)deSkyLayer::etColorR);
	pCBTarget->AddItem("Color Green", NULL, (void*)(intptr_t)deSkyLayer::etColorG);
	pCBTarget->AddItem("Color Blue", NULL, (void*)(intptr_t)deSkyLayer::etColorB);
	pCBTarget->AddItem("Intensity", NULL, (void*)(intptr_t)deSkyLayer::etIntensity);
	pCBTarget->AddItem("Transparency", NULL, (void*)(intptr_t)deSkyLayer::etTransparency);
	pCBTarget->AddItem("Light Color Red", NULL, (void*)(intptr_t)deSkyLayer::etLightColorR);
	pCBTarget->AddItem("Light Color Green", NULL, (void*)(intptr_t)deSkyLayer::etLightColorG);
	pCBTarget->AddItem("Light Color Blue", NULL, (void*)(intptr_t)deSkyLayer::etLightColorB);
	pCBTarget->AddItem("Light Intensity", NULL, (void*)(intptr_t)deSkyLayer::etLightIntensity);
	pCBTarget->AddItem("Light Ambient Intensity", NULL, (void*)(intptr_t)deSkyLayer::etAmbientIntensity);
	
	helper.FormLineStretchFirst(frameLine, "Link:", "Links", frameLine2);
	helper.ComboBox(frameLine2, "Links", pCBLinks, NULL);
	pActionLinkAdd.TakeOver(new cActionLinkAdd(*this, pCBLinks));
	helper.Button(frameLine2, pBtnLinkAdd, pActionLinkAdd);
	
	helper.ListBox(groupBox, 8, "Links", pListLinks, new cListLinks(*this));
	pListLinks->SetDefaultSorter();
	
	pActionLinkRemove.TakeOver(new cActionLinkRemove(*this, pListLinks));
}

seWPLayer::~seWPLayer(){
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = NULL;
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
		sky->AddReference();
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
	return pSky ? pSky->GetActiveLayer(): NULL;
}

seBody *seWPLayer::GetBody() const{
	const seLayer * const layer = GetLayer();
	return layer ? layer->GetActiveBody() : NULL;
}

void seWPLayer::UpdateLayerList(){
	pListLayer->RemoveAllItems();
	
	if(pSky){
		const seLayerList &layers = pSky->GetLayers();
		const int count = layers.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			seLayer * const layer = layers.GetAt(i);
			text.Format("%d: %s", i, layer->GetName().GetString());
			pListLayer->AddItem(text, NULL, layer);
		}
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
	
	const bool enabled = body != NULL;
	pEditBodySkin->SetEnabled(enabled);
	pEditBodyOrienation->SetEnabled(enabled);
	pEditBodySize->SetEnabled(enabled);
	pClrBody->SetEnabled(enabled);
}

void seWPLayer::UpdateLinkList(){
	pCBLinks->RemoveAllItems();
	
	if(pSky){
		const seLinkList &links = pSky->GetLinks();
		const int count = links.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			seLink * const link = links.GetAt(i);
			pCBLinks->AddItem(link->GetName(), NULL, link);
		}
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
		const seLinkList &links = target.GetLinks();
		const int linkCount = links.GetCount();
		int i;
		
		pListLinks->RemoveAllItems();
		
		for(i=0; i<linkCount; i++){
			seLink * const link = links.GetAt(i);
			pListLinks->AddItem(link->GetName(), NULL, link);
		}
		
		pListLinks->SortItems();
		
	}else{
		pListLinks->RemoveAllItems();
	}
	
	const bool enabled = layer != nullptr;
	pListLinks->SetEnabled(enabled);
	pBtnLinkAdd->SetEnabled(enabled);
}
