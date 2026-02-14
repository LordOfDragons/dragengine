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

#include "skyeWPLayer.h"
#include "skyeWPLayerListener.h"
#include "skyeWindowProperties.h"
#include "../../sky/skyeSky.h"
#include "../../sky/body/skyeBody.h"
#include "../../sky/controller/skyeControllerTarget.h"
#include "../../sky/layer/skyeLayer.h"
#include "../../sky/link/skyeLink.h"
#include "../../undosys/layer/skyeULayerAdd.h"
#include "../../undosys/body/skyeUBodyAdd.h"
#include "../../undosys/body/skyeUBodyRemove.h"
#include "../../undosys/body/skyeUBodyMoveUp.h"
#include "../../undosys/body/skyeUBodyMoveDown.h"
#include "../../undosys/body/skyeUBodySetSkin.h"
#include "../../undosys/body/skyeUBodySetOrientation.h"
#include "../../undosys/body/skyeUBodySetSize.h"
#include "../../undosys/body/skyeUBodySetColor.h"
#include "../../undosys/layer/skyeULayerRemove.h"
#include "../../undosys/layer/skyeULayerMoveUp.h"
#include "../../undosys/layer/skyeULayerMoveDown.h"
#include "../../undosys/layer/skyeULayerSetName.h"
#include "../../undosys/layer/skyeULayerSetSkin.h"
#include "../../undosys/layer/skyeULayerSetOffset.h"
#include "../../undosys/layer/skyeULayerSetOrientation.h"
#include "../../undosys/layer/skyeULayerSetColor.h"
#include "../../undosys/layer/skyeULayerSetIntensity.h"
#include "../../undosys/layer/skyeULayerSetTransparency.h"
#include "../../undosys/layer/skyeULayerToggleMulBySkyLight.h"
#include "../../undosys/layer/skyeULayerToggleMulBySkyColor.h"
#include "../../undosys/layer/skyeULayerSetLightColor.h"
#include "../../undosys/layer/skyeULayerSetLightIntensity.h"
#include "../../undosys/layer/skyeULayerSetAmbientIntensity.h"
#include "../../undosys/layer/skyeULayerSetLightOrientation.h"
#include "../../undosys/target/skyeUTargetAddLink.h"
#include "../../undosys/target/skyeUTargetRemoveLink.h"

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
	skyeWPLayer &pPanel;
	
public:
	cBaseTextFieldListener(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		skyeSky * const sky = pPanel.GetSky();
		skyeLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky *sky, skyeLayer *layer) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	skyeWPLayer &pPanel;
	
public:
	cBaseEditVectorListener(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		skyeSky * const sky = pPanel.GetSky();
		skyeLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, skyeSky *sky, skyeLayer *layer) = 0;
};

class cBaseEditVector2Listener : public igdeEditVector2Listener{
protected:
	skyeWPLayer &pPanel;
	
public:
	cBaseEditVector2Listener(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnVector2Changed(igdeEditVector2 *editVector2) override{
		skyeSky * const sky = pPanel.GetSky();
		skyeLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector2->GetVector2(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector2 &vector, skyeSky *sky, skyeLayer *layer) = 0;
};

class cBaseColorBoxListener : public igdeColorBoxListener{
protected:
	skyeWPLayer &pPanel;
	
public:
	cBaseColorBoxListener(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnColorChanged(igdeColorBox *colorBox) override{
		skyeSky * const sky = pPanel.GetSky();
		skyeLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(colorBox->GetColor(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decColor &color, skyeSky *sky, skyeLayer *layer) = 0;
};

class cBasePathListener : public igdeEditPathListener{
protected:
	skyeWPLayer &pPanel;
	
public:
	cBasePathListener(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		skyeSky * const sky = pPanel.GetSky();
		skyeLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editPath->GetPath(), sky, layer));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decString &path, skyeSky *sky, skyeLayer *layer) = 0;
};

class cBaseAction : public igdeAction{
protected:
	skyeWPLayer &pPanel;
	
public:
	cBaseAction(skyeWPLayer &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	cBaseAction(skyeWPLayer &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description),
	pPanel(panel){}
	
	cBaseAction(skyeWPLayer &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		skyeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(sky));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(skyeSky *sky) = 0;
	
	void Update() override{
		skyeSky * const sky = pPanel.GetSky();
		if(sky){
			UpdateSky(*sky);
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateSky(const skyeSky &){
		SetEnabled(true);
	}
};

class cBaseActionLayer : public cBaseAction{
public:
	cBaseActionLayer(skyeWPLayer &panel, const char *text, const char *description) :
	cBaseAction(panel, text, description){}
	
	cBaseActionLayer(skyeWPLayer &panel, igdeIcon *icon, const char *description) :
	cBaseAction(panel, icon, description){}
	
	cBaseActionLayer(skyeWPLayer &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(skyeSky *sky) override{
		skyeLayer * const layer = pPanel.GetLayer();
		if(layer){
			return OnActionLayer(sky, layer);
			
		}else{
			return {};
		}
	}
	
	virtual igdeUndo::Ref OnActionLayer(skyeSky *sky, skyeLayer *layer) = 0;
	
	void UpdateSky (const skyeSky &sky) override{
		skyeLayer * const layer = pPanel.GetLayer();
		if(layer){
			UpdateLayer(sky, *layer);
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateLayer(const skyeSky &, const skyeLayer &){
		SetEnabled(true);
	}
};



class cListLayers : public igdeListBoxListener{
	skyeWPLayer &pPanel;
public:
	using Ref = deTObjectReference<cListLayers>;
	cListLayers(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		skyeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		sky->SetActiveLayer(selection ? (skyeLayer*)selection->GetData() : nullptr);
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
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
	using Ref = deTObjectReference<cActionLayerAdd>;
	cActionLayerAdd(skyeWPLayer &panel) : cBaseAction(panel, "@Sky.Action.Layer.Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Sky.Action.Layer.Add.Description"){}
	
	igdeUndo::Ref OnAction(skyeSky *sky) override{
		const skyeLayer::Ref layer(skyeLayer::Ref::New(pPanel.GetEnvironment()));
		return skyeULayerAdd::Ref::New(sky, layer);
	}
};

class cActionLayerRemove : public cBaseActionLayer{
public:
	using Ref = deTObjectReference<cActionLayerRemove>;
	cActionLayerRemove(skyeWPLayer &panel) : cBaseActionLayer(panel, "@Sky.Action.Layer.Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Sky.Action.Layer.Remove.Description"){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		return skyeULayerRemove::Ref::New(layer);
	}
};

class cActionLayerUp : public cBaseActionLayer{
	igdeListBox &pListBox;
public:
	using Ref = deTObjectReference<cActionLayerUp>;
	cActionLayerUp(skyeWPLayer &panel, igdeListBox &listBox) : cBaseActionLayer(panel,
		"@Sky.Action.Layer.Up", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"@Sky.Action.Layer.Up.Description"),
	pListBox(listBox){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		return skyeULayerMoveUp::Ref::New(layer);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &) override{
		SetEnabled(pListBox.GetSelection() > 0);
	}
};

class cActionLayerDown : public cBaseActionLayer{
	igdeListBox &pListBox;
public:
	using Ref = deTObjectReference<cActionLayerDown>;
	cActionLayerDown(skyeWPLayer &panel, igdeListBox &listBox) : cBaseActionLayer(panel,
		"@Sky.Action.Layer.Down", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"@Sky.Action.Layer.Down.Description"),
	pListBox(listBox){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		return skyeULayerMoveDown::Ref::New(layer);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &) override{
		SetEnabled(pListBox.GetSelection() >= 0 && pListBox.GetSelection() < pListBox.GetItems().GetCount() - 1);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextName>;
	cTextName(skyeWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky*, skyeLayer *layer) override{
		if(textField->GetText() == layer->GetName()){
			return {};
		}
		return skyeULayerSetName::Ref::New(layer, textField->GetText());
	}
};

class cPathSkin : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathSkin>;
	cPathSkin(skyeWPLayer &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, skyeSky*, skyeLayer *layer) override{
		if(layer->GetSkinPath() == path){
			return {};
		}
		return skyeULayerSetSkin::Ref::New(layer, path);
	}
};

class cEditOffset : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditOffset>;
	cEditOffset(skyeWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, skyeSky*, skyeLayer *layer) override{
		if(layer->GetOffset().IsEqualTo(vector)){
			return {};
		}
		return skyeULayerSetOffset::Ref::New(layer, vector);
	}
};

class cEditOrientation : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditOrientation>;
	cEditOrientation(skyeWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, skyeSky*, skyeLayer *layer) override{
		if(layer->GetOrientation().IsEqualTo(vector)){
			return {};
		}
		return skyeULayerSetOrientation::Ref::New(layer, vector);
	}
};

class cEditColor : public cBaseColorBoxListener{
public:
	using Ref = deTObjectReference<cEditColor>;
	cEditColor(skyeWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(const decColor &color, skyeSky*, skyeLayer *layer) override{
		if(layer->GetColor().IsEqualTo(color)){
			return {};
		}
		return skyeULayerSetColor::Ref::New(layer, color);
	}
};

class cTextIntensity : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextIntensity>;
	cTextIntensity(skyeWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky*, skyeLayer *layer) override{
		const float value = textField->GetFloat();
		if(fabsf(layer->GetIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return skyeULayerSetIntensity::Ref::New(layer, value);
	}
};

class cSliderTransparency : public igdeEditSliderTextListener{
	skyeWPLayer &pPanel;
	igdeUndo::Ref pUndo;
public:
	using Ref = deTObjectReference<cSliderTransparency>;
	cSliderTransparency(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText) override{
		skyeSky * const sky = pPanel.GetSky();
		skyeLayer * const layer = pPanel.GetLayer();
		if(!sky || !layer){
			return;
		}
		
		if(pUndo){
			((skyeULayerSetTransparency&)(igdeUndo&)pUndo).SetNewTransparency(sliderText->GetValue());
			pUndo->Redo();
			
		}else{
			pUndo = skyeULayerSetTransparency::Ref::New(layer, sliderText->GetValue());
			sky->GetUndoSystem()->Add(pUndo);
		}
	}
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText) override{
		if(!pUndo){
			return;
		}
		
		igdeUndo::Ref undo(pUndo);
		pUndo = nullptr;
		
		((skyeULayerSetTransparency&)(igdeUndo&)undo).SetNewTransparency(sliderText->GetValue());
		undo->Redo();
	}
};

class cActionMulBySkyLight : public cBaseActionLayer{
public:
	using Ref = deTObjectReference<cActionMulBySkyLight>;
	cActionMulBySkyLight(skyeWPLayer &panel) : cBaseActionLayer(panel, "@Sky.Properties.Layer.MulBySkyLight",
		"@Sky.Properties.Layer.MulBySkyLight.Description"){ }
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		return skyeULayerToggleMulBySkyLight::Ref::New(layer);
	}
};

class cActionMulBySkyColor : public cBaseActionLayer{
public:
	using Ref = deTObjectReference<cActionMulBySkyColor>;
	cActionMulBySkyColor(skyeWPLayer &panel) : cBaseActionLayer(panel, "@Sky.Properties.Layer.MulBySkyColor",
		"@Sky.Properties.Layer.MulBySkyColor.Description"){ }
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		return skyeULayerToggleMulBySkyColor::Ref::New(layer);
	}
};

class cEditLightColor : public cBaseColorBoxListener{
public:
	using Ref = deTObjectReference<cEditLightColor>;
	cEditLightColor(skyeWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(const decColor &color, skyeSky*, skyeLayer *layer) override{
		if(layer->GetLightColor().IsEqualTo(color)){
			return {};
		}
		return skyeULayerSetLightColor::Ref::New(layer, color);
	}
};

class cTextLightIntensity : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextLightIntensity>;
	cTextLightIntensity(skyeWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky*, skyeLayer *layer) override{
		const float value = textField->GetFloat();
		if(fabsf(layer->GetLightIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return skyeULayerSetLightIntensity::Ref::New(layer, value);
	}
};

class cTextAmbientIntensity : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextAmbientIntensity>;
	cTextAmbientIntensity(skyeWPLayer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky*, skyeLayer *layer) override{
		const float value = textField->GetFloat();
		if(fabsf(layer->GetAmbientIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return skyeULayerSetAmbientIntensity::Ref::New(layer, value);
	}
};

class cEditLightOrientation : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditLightOrientation>;
	cEditLightOrientation(skyeWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, skyeSky*, skyeLayer *layer) override{
		if(layer->GetLightOrientation().IsEqualTo(vector)){
			return {};
		}
		return skyeULayerSetLightOrientation::Ref::New(layer, vector);
	}
};


class cSpinBody : public igdeSpinTextFieldListener{
	skyeWPLayer &pPanel;
public:
	using Ref = deTObjectReference<cSpinBody>;
	cSpinBody(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnValueChanged(igdeSpinTextField *textField) override{
		skyeLayer * const layer = pPanel.GetLayer();
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
	using Ref = deTObjectReference<cActionBody>;
	cActionBody(skyeWPLayer &panel, igdeButton::Ref &button) :
	cBaseActionLayer(panel, "...", "@Sky.Properties.Layer.Body.Menu"), pButton(button){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer*) override{
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
	using Ref = deTObjectReference<cActionBodyAdd>;
	cActionBodyAdd(skyeWPLayer &panel) : cBaseActionLayer(panel, "@Sky.Action.Body.Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Sky.Action.Body.Add.Description"){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		return skyeUBodyAdd::Ref::New(layer, skyeBody::Ref::New(pPanel.GetEngine()));
	}
};

class cActionBodyRemove : public cBaseActionLayer{
public:
	using Ref = deTObjectReference<cActionBodyRemove>;
	cActionBodyRemove(skyeWPLayer &panel) : cBaseActionLayer(panel, "@Sky.Action.Body.Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Sky.Action.Body.Remove.Description"){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer*) override{
		skyeBody * const body = pPanel.GetBody();
		if(!body){
			return {};
		}
		return skyeUBodyRemove::Ref::New(body);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &) override{
		SetEnabled(pPanel.GetBody());
	}
};

class cActionBodyUp : public cBaseActionLayer{
	igdeSpinTextField &pSpinTextField;
public:
	using Ref = deTObjectReference<cActionBodyUp>;
	cActionBodyUp(skyeWPLayer &panel, igdeSpinTextField &spinTextField) :
	cBaseActionLayer(panel, "@Sky.Action.Body.Up",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"@Sky.Action.Body.Up.Description"),
	pSpinTextField(spinTextField){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer*) override{
		skyeBody * const body = pPanel.GetBody();
		const int index = pSpinTextField.GetValue();
		if(body && index == pSpinTextField.GetLower()){
			return {};
		}
		return skyeUBodyMoveUp::Ref::New(body);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &) override{
		SetEnabled(pSpinTextField.GetValue() > pSpinTextField.GetLower());
	}
};

class cActionBodyDown : public cBaseActionLayer{
	igdeSpinTextField &pSpinTextField;
public:
	using Ref = deTObjectReference<cActionBodyDown>;
	cActionBodyDown(skyeWPLayer &panel, igdeSpinTextField &spinTextField) :
	cBaseActionLayer(panel, "@Sky.Action.Body.Down",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"@Sky.Action.Body.Down.Description"),
	pSpinTextField(spinTextField){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer*) override{
		skyeBody * const body = pPanel.GetBody();
		const int index = pSpinTextField.GetValue();
		if(body && index == pSpinTextField.GetUpper()){
			return {};
		}
		return skyeUBodyMoveDown::Ref::New(body);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &) override{
		SetEnabled(pSpinTextField.GetValue() < pSpinTextField.GetUpper());
	}
};

class cPathBodySkin : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathBodySkin>;
	cPathBodySkin(skyeWPLayer &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, skyeSky*, skyeLayer*) override{
		skyeBody * const body = pPanel.GetBody();
		if(!body || body->GetSkinPath() == path){
			return {};
		}
		return skyeUBodySetSkin::Ref::New(body, path);
	}
};

class cEditBodyOrientation : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditBodyOrientation>;
	cEditBodyOrientation(skyeWPLayer &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, skyeSky*, skyeLayer*) override{
		skyeBody * const body = pPanel.GetBody();
		if(!body || body->GetOrientation().IsEqualTo(vector)){
			return {};
		}
		return skyeUBodySetOrientation::Ref::New(body, vector);
	}
};

class cEditBodySize : public cBaseEditVector2Listener{
public:
	using Ref = deTObjectReference<cEditBodySize>;
	cEditBodySize(skyeWPLayer &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector2 &vector, skyeSky*, skyeLayer*) override{
		skyeBody * const body = pPanel.GetBody();
		if(!body || body->GetSize().IsEqualTo(vector)){
			return {};
		}
		return skyeUBodySetSize::Ref::New(body, vector);
	}
};

class cEditBodyColor : public cBaseColorBoxListener{
public:
	using Ref = deTObjectReference<cEditBodyColor>;
	cEditBodyColor(skyeWPLayer &panel) : cBaseColorBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(const decColor &color, skyeSky*, skyeLayer *layer) override{
		skyeBody * const body = pPanel.GetBody();
		if(layer->GetColor().IsEqualTo(color)){
			return {};
		}
		return skyeUBodySetColor::Ref::New(body, color);
	}
};


class cComboTarget : public igdeComboBoxListener{
	skyeWPLayer &pPanel;
public:
	using Ref = deTObjectReference<cComboTarget>;
	cComboTarget(skyeWPLayer &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pPanel.GetLayer() && comboBox->GetSelectedItem()){
			pPanel.GetLayer()->SetActiveTarget((deSkyLayer::eTargets)(intptr_t)
				comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cListLinks : public igdeListBoxListener{
	skyeWPLayer &pPanel;
public:
	using Ref = deTObjectReference<cListLinks>;
	cListLinks(skyeWPLayer &panel) : pPanel(panel){}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseActionLayer{
	igdeComboBox &pComboLinks;
public:
	using Ref = deTObjectReference<cActionLinkAdd>;
	cActionLinkAdd(skyeWPLayer &panel, igdeComboBox &comboLinks) : cBaseActionLayer(panel,
		"", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"@Sky.Action.Link.Add.Description"),
	pComboLinks(comboLinks){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		const igdeListItem * const selection = pComboLinks.GetSelectedItem();
		if(!selection){
			return {};
		}
		
		const deSkyLayer::eTargets target = layer->GetActiveTarget();
		skyeLink * const link = (skyeLink*)selection->GetData();
		if(layer->GetTarget(target)->GetLinks().Has(link)){
			return {};
		}
		
		return skyeUTargetAddLink::Ref::New(layer, target, link);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &) override{
		SetEnabled(pComboLinks.GetSelectedItem());
	}
};

class cActionLinkRemove : public cBaseActionLayer{
	igdeListBox &pListLinks;
public:
	using Ref = deTObjectReference<cActionLinkRemove>;
	cActionLinkRemove(skyeWPLayer &panel, igdeListBox &listLinks) : cBaseActionLayer(panel,
		"@Sky.Action.Link.Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Sky.Action.Link.Remove.Description"),
	pListLinks(listLinks){}
	
	igdeUndo::Ref OnActionLayer(skyeSky*, skyeLayer *layer) override{
		const igdeListItem * const selection = pListLinks.GetSelectedItem();
		if(!selection){
			return {};
		}
		
		const deSkyLayer::eTargets target = layer->GetActiveTarget();
		skyeLink * const link = (skyeLink*)selection->GetData();
		if(!layer->GetTarget(target)->GetLinks().Has(link)){
			return {};
		}
		
		return skyeUTargetRemoveLink::Ref::New(layer, target, link);
	}
	
	void UpdateLayer(const skyeSky &, const skyeLayer &layer) override{
		SetEnabled(layer.GetTarget(layer.GetActiveTarget())->GetLinks().IsNotEmpty());
	}
};

}



// Class skyeWPLayer
////////////////////

// Constructor, destructor
////////////////////////////

skyeWPLayer::skyeWPLayer(skyeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine, frameLine2;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = skyeWPLayerListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// layer list
	helper.GroupBoxFlow(content, groupBox, "@Sky.Properties.Layers");
	helper.ListBox(groupBox, 8, "@Sky.Properties.Layers.Description", pListLayer, cListLayers::Ref::New(*this));
	
	pActionLayerAdd = cActionLayerAdd::Ref::New(*this);
	pActionLayerRemove = cActionLayerRemove::Ref::New(*this);
	pActionLayerUp = cActionLayerUp::Ref::New(*this, pListLayer);
	pActionLayerDown = cActionLayerDown::Ref::New(*this, pListLayer);
	
	// layer settings
	helper.GroupBox(content, groupBox, "@Sky.Properties.Layer.Settings");
	helper.EditString(groupBox, "@Sky.Properties.Layer.Name", "@Sky.Properties.Layer.Name.Description", pEditName, cTextName::Ref::New(*this));
	helper.EditPath(groupBox, "@Sky.Properties.Layer.Skin", "@Sky.Properties.Layer.Skin.Description",
		igdeEnvironment::efpltSkin, pEditSkin, cPathSkin::Ref::New(*this));
	helper.EditVector(groupBox, "@Sky.Properties.Layer.Offset", "@Sky.Properties.Layer.Offset.Description", pEditOffset, cEditOffset::Ref::New(*this));
	helper.EditVector(groupBox, "@Sky.Properties.Layer.Orientation", "@Sky.Properties.Layer.Orientation.Description", pEditOrientation, cEditOrientation::Ref::New(*this));
	
	helper.FormLine(groupBox, "@Sky.Properties.Layer.Color",
		"@Sky.Properties.Layer.Color.Description", frameLine);
	helper.ColorBox(frameLine, "@Sky.Properties.Layer.Color.Color", pClrLayer, cEditColor::Ref::New(*this));
	helper.EditFloat(frameLine, "@Sky.Properties.Layer.Color.Intensity", pEditIntensity, cTextIntensity::Ref::New(*this));
	
	helper.EditSliderText(groupBox, "@Sky.Properties.Layer.Transparency", "@Sky.Properties.Layer.Transparency.Description",
		0.0f, 1.0f, 3, 3, 0.1f, pSldTransparency, cSliderTransparency::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkMulBySkyLight, cActionMulBySkyLight::Ref::New(*this));
	helper.CheckBox(groupBox, pChkMulBySkyColor, cActionMulBySkyColor::Ref::New(*this));
	
	// light settings
	helper.GroupBox(content, groupBox, "@Sky.Properties.Layer.Light");
	
	helper.FormLine(groupBox, "@Sky.Properties.Layer.Light.Label", "@Sky.Properties.Layer.Light.Description", frameLine);
	helper.ColorBox(frameLine, "@Sky.Properties.Layer.Light.Color", pClrLight, cEditLightColor::Ref::New(*this));
	helper.EditFloat(frameLine, "@Sky.Properties.Layer.Light.Intensity", pEditLightIntensity, cTextLightIntensity::Ref::New(*this));
	helper.EditFloat(frameLine, "@Sky.Properties.Layer.Light.AmbientIntensity", pEditAmbientIntensity, cTextAmbientIntensity::Ref::New(*this));
	
	helper.EditVector(groupBox, "@Sky.Properties.Layer.Light.Orientation", "@Sky.Properties.Layer.Light.Orientation.Description",
		pEditLightOrientation, cEditLightOrientation::Ref::New(*this));
	
	// bodies
	helper.GroupBox(content, groupBox, "@Sky.Properties.Layer.Bodies");
	
	helper.FormLineStretchFirst(groupBox, "@Sky.Properties.Layer.Body", "@Sky.Properties.Layer.Body.Description", frameLine);
	helper.EditSpinInteger(frameLine, "@Sky.Properties.Layer.Body.Description", 0, 0, pSpinBody, cSpinBody::Ref::New(*this));
	
	pActionBodyAdd = cActionBodyAdd::Ref::New(*this);
	pActionBodyRemove = cActionBodyRemove::Ref::New(*this);
	pActionBodyUp = cActionBodyUp::Ref::New(*this, pSpinBody);
	pActionBodyDown = cActionBodyDown::Ref::New(*this, pSpinBody);
	
	helper.Button(frameLine, pBtnBody, cActionBody::Ref::New(*this, pBtnBody));
	
	helper.EditPath(groupBox, "@Sky.Properties.Layer.Body.Skin", "@Sky.Properties.Layer.Body.Skin.Description",
		igdeEnvironment::efpltSkin, pEditBodySkin, cPathBodySkin::Ref::New(*this));
	helper.EditVector(groupBox, "@Sky.Properties.Layer.Body.Orientation", "@Sky.Properties.Layer.Body.Orientation.Description",
		pEditBodyOrienation, cEditBodyOrientation::Ref::New(*this));
	helper.EditVector2(groupBox, "@Sky.Properties.Layer.Body.Size", "@Sky.Properties.Layer.Body.Size.Description",
		pEditBodySize, cEditBodySize::Ref::New(*this));
	helper.ColorBox(groupBox, "@Sky.Properties.Layer.Body.Color", "@Sky.Properties.Layer.Body.Color.Description",
		pClrBody, cEditBodyColor::Ref::New(*this));
	
	// targets
	helper.GroupBoxFlow(content, groupBox, "@Sky.Properties.Layer.Targets");
	
	frameLine = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(frameLine);
	
	helper.ComboBox(frameLine, "@Sky.Properties.Layer.Target", "@Sky.Properties.Layer.Target.Description", pCBTarget, cComboTarget::Ref::New(*this));
	pCBTarget->SetAutoTranslateItems(true);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.OffsetX", nullptr, (void*)(intptr_t)deSkyLayer::etOffsetX);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.OffsetY", nullptr, (void*)(intptr_t)deSkyLayer::etOffsetY);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.OffsetZ", nullptr, (void*)(intptr_t)deSkyLayer::etOffsetZ);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.OrientationX", nullptr, (void*)(intptr_t)deSkyLayer::etOrientationX);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.OrientationY", nullptr, (void*)(intptr_t)deSkyLayer::etOrientationY);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.OrientationZ", nullptr, (void*)(intptr_t)deSkyLayer::etOrientationZ);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.RotationX", nullptr, (void*)(intptr_t)deSkyLayer::etRotationX);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.RotationY", nullptr, (void*)(intptr_t)deSkyLayer::etRotationY);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.RotationZ", nullptr, (void*)(intptr_t)deSkyLayer::etRotationZ);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.ColorRed", nullptr, (void*)(intptr_t)deSkyLayer::etColorR);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.ColorGreen", nullptr, (void*)(intptr_t)deSkyLayer::etColorG);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.ColorBlue", nullptr, (void*)(intptr_t)deSkyLayer::etColorB);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.Intensity", nullptr, (void*)(intptr_t)deSkyLayer::etIntensity);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.Transparency", nullptr, (void*)(intptr_t)deSkyLayer::etTransparency);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.LightColorRed", nullptr, (void*)(intptr_t)deSkyLayer::etLightColorR);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.LightColorGreen", nullptr, (void*)(intptr_t)deSkyLayer::etLightColorG);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.LightColorBlue", nullptr, (void*)(intptr_t)deSkyLayer::etLightColorB);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.LightIntensity", nullptr, (void*)(intptr_t)deSkyLayer::etLightIntensity);
	pCBTarget->AddItem("@Sky.Properties.Layer.Target.LightAmbientIntensity", nullptr, (void*)(intptr_t)deSkyLayer::etAmbientIntensity);
	
	helper.FormLineStretchFirst(frameLine, "@Sky.Properties.Layer.Target.Link", "@Sky.Properties.Layer.Target.Link.Description", frameLine2);
	helper.ComboBox(frameLine2, "@Sky.Properties.Layer.Target.Link.Links.Description", pCBLinks, {});
	pActionLinkAdd = cActionLinkAdd::Ref::New(*this, pCBLinks);
	helper.Button(frameLine2, pBtnLinkAdd, pActionLinkAdd);
	
	helper.ListBox(groupBox, 8, "@Sky.Properties.Layer.Target.Link.Links.Description", pListLinks, cListLinks::Ref::New(*this));
	pListLinks->SetDefaultSorter();
	
	pActionLinkRemove = cActionLinkRemove::Ref::New(*this, pListLinks);
}

skyeWPLayer::~skyeWPLayer(){
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = nullptr;
	}
}



// Management
///////////////

void skyeWPLayer::SetSky(skyeSky *sky){
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

void skyeWPLayer::OnSkyPathChanged(){
	if(pSky){
		pEditSkin->SetBasePath(pSky->GetDirectoryPath());
		pEditBodySkin->SetBasePath(pSky->GetDirectoryPath());
		
	}else{
		pEditSkin->SetBasePath("");
		pEditBodySkin->SetBasePath("");
	}
}

skyeLayer *skyeWPLayer::GetLayer() const{
	return pSky ? pSky->GetActiveLayer().Pointer(): nullptr;
}

skyeBody *skyeWPLayer::GetBody() const{
	const skyeLayer * const layer = GetLayer();
	return layer ? layer->GetActiveBody().Pointer() : nullptr;
}

void skyeWPLayer::UpdateLayerList(){
	pListLayer->UpdateRestoreSelection([&](){
		pListLayer->RemoveAllItems();
		
		if(pSky){
			pSky->GetLayers().VisitIndexed([&](int i, skyeLayer *layer){
				decString text;
				text.Format("%d: %s", i, layer->GetName().GetString());
				pListLayer->AddItem(text, nullptr, layer);
			});
		}
	}, 0);
}

void skyeWPLayer::SelectActiveLayer(){
	pListLayer->SetSelection(pListLayer->IndexOfItemWithData(GetLayer()));
	UpdateLayer();
}

void skyeWPLayer::UpdateLayer(){
	const skyeLayer * const layer = GetLayer();
	
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

void skyeWPLayer::UpdateBodyList(){
	skyeLayer * const layer = GetLayer();
	
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

void skyeWPLayer::SelectActiveBody(){
	skyeBody * const body = GetBody();
	
	if(body){
		skyeLayer &layer = *body->GetLayer();
		const int index = layer.GetBodies().IndexOf(body);
		pSpinBody->SetValue(index);
		
	}else{
		pSpinBody->SetValue(0);
	}
	
	pActionBodyUp->Update();
	pActionBodyDown->Update();
	
	UpdateBody();
}

void skyeWPLayer::UpdateBody(){
	skyeBody * const body = GetBody();
	
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

void skyeWPLayer::UpdateLinkList(){
	pCBLinks->RemoveAllItems();
	
	if(pSky){
		pSky->GetLinks().Visit([&](skyeLink *link){
			pCBLinks->AddItem(link->GetName(), nullptr, link);
		});
	}
	
	pCBLinks->SortItems();
}

void skyeWPLayer::SelectActiveTarget(){
	skyeLayer * const layer = GetLayer();
	int selection = 4;
	if(layer){
		selection = pCBTarget->IndexOfItemWithData((void*)(intptr_t)layer->GetActiveTarget());
	}
	
	pCBTarget->SetSelection(selection);
	
	UpdateTarget();
}

void skyeWPLayer::UpdateTarget(){
	skyeLayer * const layer = GetLayer();
	
	if(layer){
		const skyeControllerTarget &target = layer->GetTarget(layer->GetActiveTarget());
		
		pListLinks->RemoveAllItems();
		
		target.GetLinks().Visit([&](skyeLink *link){
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
