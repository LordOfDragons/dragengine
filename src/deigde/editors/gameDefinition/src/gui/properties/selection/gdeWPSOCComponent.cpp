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

#include "gdeWPSOCComponent.h"
#include "gdeWPSOCComponentListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../gamedef/objectClass/component/gdeOCComponentTexture.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../undosys/objectClass/component/gdeUOCAddComponent.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetSkinPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetAnimationPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetAnimatorPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetPropertyName.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetColRespType.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetMove.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetOccMeshPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetAudioModelPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleDoNotScale.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetModelPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentTogglePartialHide.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleAttachTarget.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleRenderEnvMap.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleAffectsAudio.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleLightShadowIgnore.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleStatic.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetPlaybackController.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetPosition.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetRotation.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetBoneName.h"
#include "../../../undosys/objectClass/component/gdeUOCRemoveComponent.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetRigPath.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCAddTexture.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCRemoveTexture.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetColorTint.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetName.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetOffset.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetPathSkin.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetProperties.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetRotation.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetScale.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelManager.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*textField, pPanel.GetObjectClass(), component));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCComponent *component) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), component));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCComponent *component) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), component));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCComponent *component) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSOCComponent &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	cBaseAction(gdeWPSOCComponent &panel, const char *text, igdeIcon *icon, const char *description) :
		igdeAction(text, icon, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component){
			return;
		}
		
		igdeUndo::Ref undo(OnActionComponent(pPanel.GetObjectClass(), component));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component) = 0;
	
	virtual void Update(){
		SetEnabled(pPanel.GetComponent());
	}
};



class cEditPathModel : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathModel> Ref;
	cEditPathModel(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetModelPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCComponentSetModelPath::Ref::New(
			pPanel.GetObjectClass(), component, editPath->GetPath()));
	}
};

class cEditPathSkin : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathSkin> Ref;
	cEditPathSkin(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCComponentSetSkinPath::Ref::New(
			pPanel.GetObjectClass(), component, editPath->GetPath()));
	}
};

class cEditPathRig : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathRig> Ref;
	cEditPathRig(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetRigPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCComponentSetRigPath::Ref::New(
			pPanel.GetObjectClass(), component, editPath->GetPath()));
	}
};

class cEditPathAnimator : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathAnimator> Ref;
	cEditPathAnimator(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetAnimatorPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCComponentSetAnimatorPath::Ref::New(pPanel.GetObjectClass(),
				component, editPath->GetPath()));
	}
};

class cEditPathAnimation : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathAnimation> Ref;
	cEditPathAnimation(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetAnimationPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCComponentSetAnimationPath::Ref::New(pPanel.GetObjectClass(),
				component, editPath->GetPath()));
	}
};

class cEditMove : public igdeTextFieldListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditMove> Ref;
	cEditMove(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	void OnTextChanged (igdeTextField *textField) override{
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetMove() == textField->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCComponentSetMove::Ref::New(
			pPanel.GetObjectClass(), component, textField->GetText()));
	}
};

class cEditPathOcclusionMesh : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathOcclusionMesh> Ref;
	cEditPathOcclusionMesh(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetOcclusionMeshPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCComponentSetOccMeshPath::Ref::New(pPanel.GetObjectClass(),
				component, editPath->GetPath()));
	}
};

class cEditPathAudioModel : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditPathAudioModel> Ref;
	cEditPathAudioModel(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component || component->GetAudioModelPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCComponentSetAudioModelPath::Ref::New(pPanel.GetObjectClass(),
				component, editPath->GetPath()));
	}
};

class cTextPlaybackController : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPlaybackController> Ref;
	cTextPlaybackController(gdeWPSOCComponent &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component){
		if(component->GetPlaybackController() == textField.GetText()){
			return {};
		}
		return gdeUOCComponentSetPlaybackController::Ref::New(objectClass, component, textField.GetText());
	}
};

class cActionDoNotScale : public cBaseAction{
public:
	typedef deTObjectReference<cActionDoNotScale> Ref;
	cActionDoNotScale(gdeWPSOCComponent &panel) : cBaseAction(panel, "Do not scale",
		"Component scale does not change with scale of parent object class instance"){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentToggleDoNotScale::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionStatic : public cBaseAction{
public:
	typedef deTObjectReference<cActionStatic> Ref;
	cActionStatic(gdeWPSOCComponent &panel) : cBaseAction(panel, "Static",
		"Component is static (optimization for graphic module)"){ }
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentToggleStatic::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionRenderEnvMap : public cBaseAction{
public:
	typedef deTObjectReference<cActionRenderEnvMap> Ref;
	cActionRenderEnvMap(gdeWPSOCComponent &panel) : cBaseAction(panel, "Render Env-Map",
		"Component is included in environment maps"){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentToggleRenderEnvMap::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionAffectsAudio : public cBaseAction{
public:
	typedef deTObjectReference<cActionAffectsAudio> Ref;
	cActionAffectsAudio(gdeWPSOCComponent &panel) : cBaseAction(panel, "Affects audio",
		"Component affects audio"){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentToggleAffectsAudio::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionLightShadowIgnore : public cBaseAction{
public:
	typedef deTObjectReference<cActionLightShadowIgnore> Ref;
	cActionLightShadowIgnore(gdeWPSOCComponent &panel) : cBaseAction(panel, "Light Shadow Ignore",
		"Component is not casting shadows from lights present in the same object"){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentToggleLightShadowIgnore::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionPartialHide : public cBaseAction{
public:
	typedef deTObjectReference<cActionPartialHide> Ref;
	cActionPartialHide(gdeWPSOCComponent &panel) : cBaseAction(panel, "Partial hide",
		"Component is hidden if partial hide tags match"){ }
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentTogglePartialHide::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cActionAttachTarget : public cBaseAction{
public:
	typedef deTObjectReference<cActionAttachTarget> Ref;
	cActionAttachTarget(gdeWPSOCComponent &panel) : cBaseAction(panel, "Attach Target",
		"Resourcec can be attached to this component in the editor."){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		return gdeUOCComponentToggleAttachTarget::Ref::New(objectClass, component);
	}
	
	void Update() override{/* empty on purpose!*/}
};

class cComboCollisionResponseType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCollisionResponseType> Ref;
	cComboCollisionResponseType(gdeWPSOCComponent &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCComponent *component){
		const deCollider::eResponseType responseType = (deCollider::eResponseType)
			(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(component->GetColliderResponseType() == responseType){
			return {};
		}
		return gdeUOCComponentSetColRespType::Ref::New(objectClass, component, responseType);
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(gdeWPSOCComponent &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component) override{
		if(component->GetPosition().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCComponentSetPosition::Ref::New(objectClass, component, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(gdeWPSOCComponent &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component) override{
		if(component->GetRotation().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCComponentSetRotation::Ref::New(objectClass, component, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBoneName> Ref;
	cTextBoneName(gdeWPSOCComponent &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component) override{
		if(component->GetBoneName() == textField.GetText()){
			return {};
		}
		return gdeUOCComponentSetBoneName::Ref::New(objectClass, component, textField.GetText());
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNames> Ref;
	cComboPropertyNames(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetComponent()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNameTarget> Ref;
	cComboPropertyNameTarget(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCComponent * const component = pPanel.GetComponent();
		if(!component){
			return;
		}
		
		const gdeOCComponent::eProperties propertyName = pPanel.GetPropertyName();
		if(component->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCComponentSetPropertyName::Ref::New(pPanel.GetObjectClass(),
				component, propertyName, comboBox->GetText()));
	}
};


// textures

class cBaseComboBoxTextureListener : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cBaseComboBoxTextureListener> Ref;
	cBaseComboBoxTextureListener(gdeWPSOCComponent &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass, gdeOCComponent *component) override{
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture(comboBox, objectClass, component, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnChangedTexture(igdeComboBox &comboBox, gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture) = 0;
};

class cBaseActionTexture : public cBaseAction{
public:
	typedef deTObjectReference<cBaseActionTexture> Ref;
	
public:
	cBaseActionTexture(gdeWPSOCComponent &panel, const char *text, const char *description) :
		cBaseAction(panel, text, description){}
	
	cBaseActionTexture(gdeWPSOCComponent &panel, const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnActionTexture(objectClass, component, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture) = 0;
	
	void Update() override{
		SetEnabled(pPanel.GetTexture());
	}
};

class cBaseTextFieldTextureListener : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cBaseTextFieldTextureListener> Ref;
	cBaseTextFieldTextureListener(gdeWPSOCComponent &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass, gdeOCComponent *component) override{
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture(textField, objectClass, component, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture) = 0;
};

class cBaseEditVector2TextureListener : public igdeEditVector2Listener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVector2TextureListener> Ref;
	cBaseEditVector2TextureListener(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture){
			return;
		}
		
		igdeUndo::Ref undo(OnChangedTexture(editVector2->GetVector2(),
			pPanel.GetObjectClass(), pPanel.GetComponent(), texture));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture) = 0;
};


class cComboTextures : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTextures> Ref;
	cComboTextures(gdeWPSOCComponent &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCComponent *component){
		const igdeListItem * const selection = comboBox.GetSelectedItem();
		component->SetActiveTexture(selection ? (gdeOCComponentTexture*)selection->GetData() : nullptr);
		pPanel.GetGameDefinition()->NotifyOCComponentActiveTextureChanged(objectClass, component);
		return {};
	}
};

class cActionTextureAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureAdd> Ref;
	
private:
	const decString pTextureName;
	
public:
	cActionTextureAdd(gdeWPSOCComponent &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add texture"){}
	
	cActionTextureAdd(gdeWPSOCComponent &panel, const decString &textureName) :
		cBaseAction(panel, textureName, nullptr, "Add texture"), pTextureName(textureName){}
	
	virtual igdeUndo::Ref OnActionComponent(gdeObjectClass *objectClass, gdeOCComponent *component){
		decString name(pTextureName);
		
		if(name.IsEmpty()){
			name = "Texture";
			
			while(true){
				if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Texture", "Name:", name)){
					return {};
				}
				
				if(component->GetTextures().HasMatching([&](const gdeOCComponentTexture::Ref &t){
					return t->GetName() == name;
				})){
					igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Add Texture", "A texture with this name exists already.");
					
				}else{
					break;
				}
			}
		}
		
		const gdeOCComponentTexture::Ref texture(gdeOCComponentTexture::Ref::New(name));
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCCAddTexture::Ref::New(
			objectClass, component, texture));
		
		component->SetActiveTexture(texture);
		pPanel.GetGameDefinition()->NotifyOCComponentActiveTextureChanged(objectClass, component);
		return {};
	}
	
	void Update() override{
		const gdeOCComponent * const component = pPanel.GetComponent();
		SetEnabled(component && (pTextureName.IsEmpty() || !component->GetTextures().HasMatching([&](const gdeOCComponentTexture &t){
			return t.GetName() == pTextureName;
		})));
	}
};

class cActionTextureRemove : public cBaseActionTexture{
public:
	typedef deTObjectReference<cActionTextureRemove> Ref;
	cActionTextureRemove(gdeWPSOCComponent &panel) : cBaseActionTexture(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove selected texture"){}
	
	virtual igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture){
		return gdeUOCCRemoveTexture::Ref::New(objectClass, component, texture);
	}
};

class cActionTexturesMenu : public igdeActionContextMenu{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cActionTexturesMenu> Ref;
	cActionTexturesMenu(gdeWPSOCComponent &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Textures menu"), pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		pPanel.PrepareEngineModelPath();
		
		const decTObjectList<igdeAction> &actionsAddFromModel = pPanel.GetActionsTextureAddFromModel();
		igdeEnvironment &env = contextMenu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureAdd());
		
		if(actionsAddFromModel.IsNotEmpty()){
			igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
				env, "Add Texture From Model", env.GetStockIcon(igdeEnvironment::esiPlus), "Add Texture From Model"));
			
			actionsAddFromModel.Visit([&](const igdeAction::Ref &action){
				helper.MenuCommand(subMenu, action);
			});
			
			contextMenu.AddChild(subMenu);
		}
		
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureRemove());
	}
};

class cTextTextureEditName : public cBaseTextFieldTextureListener{
public:
	typedef deTObjectReference<cTextTextureEditName> Ref;
	cTextTextureEditName(gdeWPSOCComponent &panel) : cBaseTextFieldTextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture) override{
		if(texture->GetName() == textField.GetText()){
			return {};
		}
		
		if(component->GetTextures().HasMatching([&](const gdeOCComponentTexture &t){
			return t.GetName() == textField.GetText();
		})){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Rename texture", "A texture with this name exists already.");
			textField.SetText(texture->GetName());
			return {};
		}
		
		return gdeUOCCTextureSetName::Ref::New(objectClass, component, texture, textField.GetText());
	}
};

class cEditTextureEditPathSkin : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cEditTextureEditPathSkin> Ref;
	cEditTextureEditPathSkin(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture || texture->GetPathSkin() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCCTextureSetPathSkin::Ref::New(
			pPanel.GetObjectClass(), pPanel.GetComponent(), texture, editPath->GetPath()));
	}
};

class cEditTextureEditOffset : public cBaseEditVector2TextureListener{
public:
	typedef deTObjectReference<cEditTextureEditOffset> Ref;
	cEditTextureEditOffset(gdeWPSOCComponent &panel) : cBaseEditVector2TextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture) override{
		if(texture->GetOffset().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCCTextureSetOffset::Ref::New(objectClass, component, texture, vector);
	}
};

class cTextTextureEditRotation : public cBaseTextFieldTextureListener{
public:
	typedef deTObjectReference<cTextTextureEditRotation> Ref;
	cTextTextureEditRotation(gdeWPSOCComponent &panel) : cBaseTextFieldTextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture) override{
		const float value = textField.GetFloat();
		if(fabsf(texture->GetRotation() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCCTextureSetRotation::Ref::New(objectClass, component, texture, value);
	}
};

class cEditTextureEditScale : public cBaseEditVector2TextureListener{
public:
	typedef deTObjectReference<cEditTextureEditScale> Ref;
	cEditTextureEditScale(gdeWPSOCComponent &panel) : cBaseEditVector2TextureListener(panel){}
	
	igdeUndo::Ref OnChangedTexture(const decVector2 &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture) override{
		if(texture->GetScale().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCCTextureSetScale::Ref::New(objectClass, component, texture, vector);
	}
};

class cColorTextureTint : public igdeColorBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cColorTextureTint> Ref;
	cColorTextureTint(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture || texture->GetColorTint().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCCTextureSetColorTint::Ref::New(
			pPanel.GetObjectClass(), pPanel.GetComponent(), texture, colorBox->GetColor()));
	}
};


class cActionPropertyValueSet : public cBaseActionTexture {
public:
	typedef deTObjectReference<cActionPropertyValueSet> Ref;
	cActionPropertyValueSet(gdeWPSOCComponent &panel) : cBaseActionTexture(panel, "Set",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Set property value"){}
	
	virtual igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture){
		const decString &key = pPanel.GetTexturePropertyKey();
		if(!texture || key.IsEmpty()){
			return {};
		}
		
		const gdeObjectClass *propertyOwner = nullptr;
		const gdeProperty *property = nullptr;
		decString value;
		
		if(objectClass->DeepGetNamedProperty(key, propertyOwner, property)){
			value = property->GetDefaultValue();
		}
		
		if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Set Texture Property Value", "Value:", value)){
			return {};
		}
		
		decStringDictionary values(texture->GetProperties());
		values.SetAt(key, value);
		return gdeUOCCTextureSetProperties::Ref::New(objectClass, component, texture, values);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTexture() && !pPanel.GetTexturePropertyKey().IsEmpty());
	}
};

class cActionPropertyValueRemove : public cBaseActionTexture {
public:
	typedef deTObjectReference<cActionPropertyValueRemove> Ref;
	cActionPropertyValueRemove(gdeWPSOCComponent &panel) : cBaseActionTexture(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove property value"){}
	
	virtual igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture){
		const char * const key = pPanel.GetTexturePropertyValue();
		if(!key || !texture->GetProperties().Has(key)){
			return {};
		}
		
		decStringDictionary values(texture->GetProperties());
		values.Remove(key);
		return gdeUOCCTextureSetProperties::Ref::New(objectClass, component, texture, values);
	}
	
	void Update() override{
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		const char * const key = pPanel.GetTexturePropertyValue();
		SetEnabled(texture && key && texture->GetProperties().Has(key));
	}
};

class cActionPropertyValueClear : public cBaseActionTexture {
public:
	typedef deTObjectReference<cActionPropertyValueClear> Ref;
	cActionPropertyValueClear(gdeWPSOCComponent &panel) : cBaseActionTexture(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all property values"){}
	
	virtual igdeUndo::Ref OnActionTexture(gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture){
		if(texture->GetProperties().GetCount() == 0){
			return {};
		}
		return gdeUOCCTextureSetProperties::Ref::New(objectClass, component, texture, decStringDictionary());
	}
	
	void Update() override{
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		SetEnabled(texture && texture->GetProperties().GetCount() > 0);
	}
};

class cListTexturePropertyValues : public igdeIconListBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	typedef deTObjectReference<cListTexturePropertyValues> Ref;
	cListTexturePropertyValues(gdeWPSOCComponent &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeIconListBox *listBox, int index){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if(!texture){
			return;
		}
		
		const decString &key = listBox->GetItemAt(index)->GetText();
		if(!texture->GetProperties().Has(key)){
			return;
		}
		
		decString value(texture->GetProperties().GetAt(key));
		if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Edit Texture Property Value", "Value:", value)){
			return;
		}
		
		decStringDictionary values(texture->GetProperties());
		values.SetAt(key, value);
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCCTextureSetProperties::Ref::New(
			pPanel.GetObjectClass(), pPanel.GetComponent(), texture, values));
	}
	
	virtual void AddContextMenuEntries(igdeIconListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionTexturePropertyValueSet());
		helper.MenuCommand(menu, pPanel.GetActionTexturePropertyValueRemove());
		helper.MenuCommand(menu, pPanel.GetActionTexturePropertyValueClear());
	}
};

}



// Class gdeWPSOCComponent
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCComponent::gdeWPSOCComponent(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pDirtyEngModelTexNames(true)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSOCComponentListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// actions
	pActionTexturesMenu = cActionTexturesMenu::Ref::New(*this);
	pActionTextureAdd = cActionTextureAdd::Ref::New(*this);
	pActionTextureRemove = cActionTextureRemove::Ref::New(*this);
	pActionTexturePropertyValueClear = cActionPropertyValueClear::Ref::New(*this);
	pActionTexturePropertyValueRemove = cActionPropertyValueRemove::Ref::New(*this);
	pActionTexturePropertyValueSet = cActionPropertyValueSet::Ref::New(*this);
	
	
	// component
	helper.GroupBox(content, groupBox, "Object Class Component:");
	
	helper.EditPath(groupBox, "Model:", "Path to model file to use",
		igdeEnvironment::efpltModel, pEditPathModel, cEditPathModel::Ref::New(*this));
	helper.EditPath(groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pEditPathSkin, cEditPathSkin::Ref::New(*this));
	helper.EditPath(groupBox, "Rig:", "Path to rig file to use",
		igdeEnvironment::efpltRig, pEditPathRig, cEditPathRig::Ref::New(*this));
	helper.EditPath(groupBox, "Animator:", "Path to animator file to use",
		igdeEnvironment::efpltAnimator, pEditPathAnimator, cEditPathAnimator::Ref::New(*this));
	helper.EditPath(groupBox, "Animation:", "Path to animation file to use",
		igdeEnvironment::efpltAnimation, pEditPathAnimation, cEditPathAnimation::Ref::New(*this));
	helper.EditString(groupBox, "Move:", "Move to use from animation file", 15, pEditMove, cEditMove::Ref::New(*this));
	helper.EditPath(groupBox, "Occlusion Mesh:", "Path to occlusion mesh file to use",
		igdeEnvironment::efpltOcclusionMesh, pEditPathOcclusionMesh, cEditPathOcclusionMesh::Ref::New(*this));
	helper.EditPath(groupBox, "Audio Model:", "Path to audio model file to use",
		igdeEnvironment::efpltModel, pEditPathAudioModel, cEditPathAudioModel::Ref::New(*this));
	helper.EditString(groupBox, "Playback Controller:", "Name of animator playback controller",
		pEditPlaybackController, cTextPlaybackController::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Collision:", "Collision response type",
		pCBCollisionResponseType, cComboCollisionResponseType::Ref::New(*this));
	pCBCollisionResponseType->AddItem("Static", nullptr, (void*)(intptr_t)deCollider::ertStatic);
	pCBCollisionResponseType->AddItem("Kinematic", nullptr, (void*)(intptr_t)deCollider::ertKinematic);
	pCBCollisionResponseType->AddItem("Dynamic", nullptr, (void*)(intptr_t)deCollider::ertDynamic);
	
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, cTextBoneName::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkDoNotScale, cActionDoNotScale::Ref::New(*this));
	helper.CheckBox(groupBox, pChkPartialHide, cActionPartialHide::Ref::New(*this));
	helper.CheckBox(groupBox, pChkAttachTarget, cActionAttachTarget::Ref::New(*this));
	helper.CheckBox(groupBox, pChkRenderEnvMap, cActionRenderEnvMap::Ref::New(*this));
	helper.CheckBox(groupBox, pChkAffectsAudio, cActionAffectsAudio::Ref::New(*this));
	helper.CheckBox(groupBox, pChkLightShadowIgnore, cActionLightShadowIgnore::Ref::New(*this));
	helper.CheckBox(groupBox, pChkStatic, cActionStatic::Ref::New(*this));
	
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, cComboPropertyNames::Ref::New(*this));
	pCBPropertyNames->AddItem("Model", nullptr, (void*)(intptr_t)gdeOCComponent::epModel);
	pCBPropertyNames->AddItem("Skin", nullptr, (void*)(intptr_t)gdeOCComponent::epSkin);
	pCBPropertyNames->AddItem("Rig", nullptr, (void*)(intptr_t)gdeOCComponent::epRig);
	pCBPropertyNames->AddItem("Animator", nullptr, (void*)(intptr_t)gdeOCComponent::epAnimator);
	pCBPropertyNames->AddItem("Animation", nullptr, (void*)(intptr_t)gdeOCComponent::epAnimation);
	pCBPropertyNames->AddItem("Move", nullptr, (void*)(intptr_t)gdeOCComponent::epMove);
	pCBPropertyNames->AddItem("Playback controller", nullptr, (void*)(intptr_t)gdeOCComponent::epPlaybackController);
	pCBPropertyNames->AddItem("Occlusion mesh", nullptr, (void*)(intptr_t)gdeOCComponent::epOcclusionMesh);
	pCBPropertyNames->AddItem("Audio model", nullptr, (void*)(intptr_t)gdeOCComponent::epAudioModel);
	pCBPropertyNames->AddItem("Render env-map", nullptr, (void*)(intptr_t)gdeOCComponent::epRenderEnvMap);
	pCBPropertyNames->AddItem("Affects audio", nullptr, (void*)(intptr_t)gdeOCComponent::epAffectsAudio);
	pCBPropertyNames->AddItem("Light Shadow Ignore", nullptr, (void*)(intptr_t)gdeOCComponent::epLightShadowIgnore);
	pCBPropertyNames->AddItem("Attach position", nullptr,  (void*)(intptr_t)gdeOCComponent::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", nullptr, (void*)(intptr_t)gdeOCComponent::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, cComboPropertyNameTarget::Ref::New(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
	
	
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
	
	
	// texture property values
	helper.GroupBoxFlow(content, groupBox, "Texture Properties:", false, true);
	
	frameLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	helper.ComboBox(frameLine, "Property value to set", pTextureCBPropertyKeys, {});
	pTextureCBPropertyKeys->SetDefaultSorter();
	helper.Button(frameLine, pTextureBtnPropertyValueSet, pActionTexturePropertyValueSet);
	groupBox->AddChild(frameLine);
	
	const igdeUIHelper::sColumnHeader headersPropertyValues[2] = {
		igdeUIHelper::sColumnHeader("Key", nullptr, igdeApplication::app().DisplayScaled(150)),
		igdeUIHelper::sColumnHeader("Value", nullptr, igdeApplication::app().DisplayScaled(200))
	};
	helper.IconListBox(groupBox, pTextureListProperties,
		igdeApplication::app().DisplayScaled(decPoint(100, 120)),
		headersPropertyValues, 2, "Property values", cListTexturePropertyValues::Ref::New(*this));
	pTextureListProperties->SetDefaultSorter();
}

gdeWPSOCComponent::~gdeWPSOCComponent(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSOCComponent::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
	}
	
	UpdateComponent();
	UpdateTextureList();
	UpdatePropertyList();
	UpdateTexturePropertyList();
}



gdeObjectClass *gdeWPSOCComponent::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeOCComponent *gdeWPSOCComponent::GetComponent() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCComponent() : nullptr;
}

gdeOCComponentTexture *gdeWPSOCComponent::GetTexture() const{
	gdeOCComponent * const component = GetComponent();
	return component ? component->GetActiveTexture() : nullptr;
}

const decString &gdeWPSOCComponent::GetTextureProperty() const{
	const gdeOCComponentTexture * const texture = GetTexture();
	static decString emptyString;
	return texture ? texture->GetActiveProperty() : emptyString;
}

const gdeOCComponent::eProperties gdeWPSOCComponent::GetPropertyName() const{
	return (gdeOCComponent::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}

const decString &gdeWPSOCComponent::GetTexturePropertyKey() const{
	return pTextureCBPropertyKeys->GetText();
}

const char *gdeWPSOCComponent::GetTexturePropertyValue() const{
	const igdeListItem * const selection = pTextureListProperties->GetSelectedItem();
	return selection ? selection->GetText().GetString() : nullptr;
}



void gdeWPSOCComponent::UpdatePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if(objectClass){
		objectClass->AddPropertyNamesTo(properties, true);
	}
	
	const decString selection(pCBPropertyNameTarget->GetText());
	pCBPropertyNameTarget->RemoveAllItems();
	
	const int count = properties.GetCount();
	for(i=0; i<count; i++){
		pCBPropertyNameTarget->AddItem(properties.GetAt(i));
	}
	
	pCBPropertyNameTarget->SortItems();
	pCBPropertyNameTarget->SetText(selection);
}

void gdeWPSOCComponent::UpdateTexturePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if(objectClass){
		objectClass->AddTexturePropertyNamesTo(properties, true);
	}
	
	const decString selection(pTextureCBPropertyKeys->GetText());
	pTextureCBPropertyKeys->RemoveAllItems();
	
	const int count = properties.GetCount();
	for(i=0; i<count; i++){
		pTextureCBPropertyKeys->AddItem(properties.GetAt(i).GetString());
	}
	
	pTextureCBPropertyKeys->SortItems();
	pTextureCBPropertyKeys->SetText(selection);
}



void gdeWPSOCComponent::UpdateComponent(){
	const gdeOCComponent * const component = GetComponent();
	
	if(component){
		pEditPathModel->SetPath(component->GetModelPath());
		pEditPathSkin->SetPath(component->GetSkinPath());
		pEditPathRig->SetPath(component->GetRigPath());
		pEditPathAnimator->SetPath(component->GetAnimatorPath());
		pEditPathAnimation->SetPath(component->GetAnimationPath());
		pEditMove->SetText(component->GetMove());
		pEditPathOcclusionMesh->SetPath(component->GetOcclusionMeshPath());
		pEditPathAudioModel->SetPath(component->GetAudioModelPath());
		pEditPlaybackController->SetText(component->GetPlaybackController());
		pChkDoNotScale->SetChecked(component->GetDoNotScale());
		pChkStatic->SetChecked(component->GetStatic());
		pChkRenderEnvMap->SetChecked(component->GetRenderEnvMap());
		pChkAffectsAudio->SetChecked(component->GetAffectsAudio());
		pChkLightShadowIgnore->SetChecked(component->GetLightShadowIgnore());
		pChkPartialHide->SetChecked(component->GetPartialHide());
		pChkAttachTarget->SetChecked(component->GetAttachTarget());
		pCBCollisionResponseType->SetSelectionWithData(
			(void*)(intptr_t)component->GetColliderResponseType());
		pEditPosition->SetVector(component->GetPosition());
		pEditRotation->SetVector(component->GetRotation());
		pEditBoneName->SetText(component->GetBoneName());
		
		SetEngineModelPath(component->GetModelPath());
		
	}else{
		pEditPathModel->ClearPath();
		pEditPathSkin->ClearPath();
		pEditPathRig->ClearPath();
		pEditPathAnimator->ClearPath();
		pEditPathAnimation->ClearPath();
		pEditMove->ClearText();
		pEditPathOcclusionMesh->ClearPath();
		pEditPathAudioModel->ClearPath();
		pEditPlaybackController->ClearText();
		pChkDoNotScale->SetChecked(false);
		pChkStatic->SetChecked(false);
		pChkRenderEnvMap->SetChecked(false);
		pChkAffectsAudio->SetChecked(false);
		pChkLightShadowIgnore->SetChecked(false);
		pChkPartialHide->SetChecked(false);
		pChkAttachTarget->SetChecked(false);
		pCBCollisionResponseType->SetSelectionWithData((void*)(intptr_t)deCollider::ertStatic);
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditBoneName->ClearText();
		
		SetEngineModelPath("");
	}
	
	const bool enabled = component;
	pEditPathModel->SetEnabled(enabled);
	pEditPathSkin->SetEnabled(enabled);
	pEditPathRig->SetEnabled(enabled);
	pEditPathAnimator->SetEnabled(enabled);
	pEditPathAnimation->SetEnabled(enabled);
	pEditMove->SetEnabled(enabled);
	pEditPathOcclusionMesh->SetEnabled(enabled);
	pEditPathAudioModel->SetEnabled(enabled);
	pEditPlaybackController->SetEnabled(enabled);
	pChkDoNotScale->SetEnabled(enabled);
	pChkStatic->SetEnabled(enabled);
	pChkRenderEnvMap->SetEnabled(enabled);
	pChkAffectsAudio->SetEnabled(enabled);
	pChkLightShadowIgnore->SetEnabled(enabled);
	pChkPartialHide->SetEnabled(enabled);
	pChkAttachTarget->SetEnabled(enabled);
	pCBCollisionResponseType->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	
	UpdatePropertyName();
}

void gdeWPSOCComponent::UpdatePropertyName(){
	const gdeOCComponent * const component = GetComponent();
	
	if(component){
		pCBPropertyNameTarget->SetText(component->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(component);
}

void gdeWPSOCComponent::UpdateTextureList(){
	const gdeOCComponent * const component = GetComponent();
	
	pCBTextures->RemoveAllItems();
	
	if(component){
		const gdeOCComponentTexture::List &textures = component->GetTextures();
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

void gdeWPSOCComponent::SelectActiveTexture(){
	pCBTextures->SetSelectionWithData(GetTexture());
}

void gdeWPSOCComponent::UpdateTexture(){
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
	
	UpdateTextureProperties();
}

void gdeWPSOCComponent::UpdateTextureProperties(){
	const gdeOCComponentTexture * const texture = GetTexture();
	
	pTextureListProperties->RemoveAllItems();
	
	if(texture){
		const decStringList keys(texture->GetProperties().GetKeys());
		const decStringList values(texture->GetProperties().GetValues());
		const int count = keys.GetCount();
		decStringList details;
		int i;
		
		for(i=0; i<count; i++){
			details.RemoveAll();
			details.Add(values.GetAt(i));
			pTextureListProperties->AddItem(keys.GetAt(i), details);
		}
		
		pTextureListProperties->SortItems();
	}
}

void gdeWPSOCComponent::SelectActiveTextureProperty(){
	pTextureListProperties->SetSelection(pTextureListProperties->IndexOfItem(GetTextureProperty()));
	pTextureListProperties->EnsureSelectedItemVisible();
}

void gdeWPSOCComponent::SetEngineModelPath(const char *path){
	if(pEngModelPath == path){
		return;
	}
	
	pEngModelTexNames.RemoveAll();
	pEngModelPath = path;
	pDirtyEngModelTexNames = true;
}

void gdeWPSOCComponent::PrepareEngineModelPath(){
	if(!pDirtyEngModelTexNames){
		return;
	}
	
	pDirtyEngModelTexNames = false;
	pActionsTextureAddFromModel.RemoveAll();
	
	if(pEngModelPath.IsEmpty()){
		return;
	}
	
	deModel::Ref model;
	try{
		model = GetEngine()->GetModelManager()->LoadModel(pEngModelPath, "/");
		
		const int count = model->GetTextureCount();
		int i;
		for(i=0; i<count; i++){
			pEngModelTexNames.Add(model->GetTextureAt(i)->GetName());
		}
		
		pEngModelTexNames.SortAscending();
		
		for(i=0; i<count; i++){
			pActionsTextureAddFromModel.Add(cActionTextureAdd::Ref::New(
				*this, pEngModelTexNames.GetAt(i)));
		}
		
	}catch(const deException &){
		// do nothing. this is best effort only
	}
}
