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

#include "seWPView.h"
#include "seWPViewListener.h"
#include "seWindowProperties.h"
#include "../../skin/seSkin.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSliderListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(seWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			OnChanged(*textField, *skin);
		}
	}
	
	virtual void OnChanged(igdeTextField &textField, seSkin &skin) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(seWPView &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			OnAction(*skin);
		}
	}
	
	virtual void OnAction(seSkin &skin) = 0;
	
	void Update() override{
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			Update(*skin);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const seSkin &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	seWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditPathListener> Ref;
	cBaseEditPathListener(seWPView &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			OnChanged(*editPath, *skin);
		}
	}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(seWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			OnChanged(*comboBox, *skin);
		}
	}
	
	virtual void OnChanged(igdeComboBox &comboBox, seSkin &skin) = 0;
};



class cComboPreviewMode : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPreviewMode> Ref;
	cComboPreviewMode(seWPView &panel) : cBaseComboBoxListener(panel){}
	
	virtual void OnChanged(igdeComboBox &comboBox, seSkin &skin){
		if(comboBox.GetSelectedItem()){
			skin.SetPreviewMode((seSkin::ePreviewMode)(intptr_t)comboBox.GetSelectedItem()->GetData());
		}
	}
};

class cEditModelPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditModelPath> Ref;
	cEditModelPath(seWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin){
		skin.SetModelPath(editPath.GetPath());
	}
};

class cEditRigPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditRigPath> Ref;
	cEditRigPath(seWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin){
		skin.SetRigPath(editPath.GetPath());
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditAnimationPath> Ref;
	cEditAnimationPath(seWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin){
		skin.SetAnimationPath(editPath.GetPath());
	}
};

class cComboMove : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	typedef deTObjectReference<cComboMove> Ref;
	cComboMove(seWPView &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	virtual void OnChanged(igdeComboBox &comboBox, seSkin &skin){
		if(!pPreventUpdate){
			skin.SetMoveName(comboBox.GetText());
		}
	}
};

class cActionPlayback : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlayback> Ref;
	
public:
	cActionPlayback(seWPView &panel) : cBaseAction(panel, "Play back animation",
		nullptr, "Animation is played back"){}
	
	void OnAction(seSkin &skin) override{
		skin.SetPlayback(!skin.GetPlayback());
	}
	
	void Update(const seSkin &skin) override{
		SetEnabled(true);
		SetSelected(skin.GetPlayback());
	}
};

class cActionRewind : public cBaseAction{
public:
	typedef deTObjectReference<cActionRewind> Ref;
	
public:
	cActionRewind(seWPView &panel) : cBaseAction(panel, "Rewind Textures", nullptr,
		"Rewind all textures to the initial state as if added to the game world"){ }
	
	void OnAction(seSkin &skin) override{
		skin.RewindTextures();
	}
};

class cActionCameraChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraChanged> Ref;
	
public:
	cActionCameraChanged(seWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(seSkin &skin) override{
		skin.NotifyCameraChanged();
	}
};

class cActionSkyChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionSkyChanged> Ref;
	
public:
	cActionSkyChanged(seWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(seSkin &skin) override{
		skin.NotifySkyChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnvObjChanged> Ref;
	
public:
	cActionEnvObjChanged(seWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(seSkin &skin) override{
		skin.NotifyEnvObjectChanged();
	}
};

}



// Class seWPView
///////////////////

// Constructor, destructor
////////////////////////////

seWPView::seWPView(seWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pPreventUpdate(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = seWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// resources
	helper.GroupBox(content, groupBox, "Preview:");
	
	helper.ComboBox(groupBox, "Mode:", "Preview mode.", pCBPreviewMode, cComboPreviewMode::Ref::New(*this));
	pCBPreviewMode->AddItem("Model", nullptr, (void*)(intptr_t)seSkin::epmModel);
	pCBPreviewMode->AddItem("Light", nullptr, (void*)(intptr_t)seSkin::epmLight);
	
	helper.EditPath(groupBox, "Model:", "Path to the model resource to use.",
		igdeEnvironment::efpltModel, pEditModelPath, cEditModelPath::Ref::New(*this));
	helper.EditPath(groupBox, "Rig:", "Path to the rig resource to use.",
		igdeEnvironment::efpltRig, pEditRigPath, cEditRigPath::Ref::New(*this));
	helper.EditPath(groupBox, "Animation:", "Path to the animation resource to use.",
		igdeEnvironment::efpltAnimation, pEditAnimPath, cEditAnimationPath::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "Move:", true, "Name of the animation move to play.",
		pCBAnimMoves, cComboMove::Ref::New(*this, pPreventUpdate));
	pCBAnimMoves->SetDefaultSorter();
	
	helper.CheckBox(groupBox, pChkPlayback, cActionPlayback::Ref::New(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnRewindTextures, cActionRewind::Ref::New(*this));
	
	// property panels
	helper.WPSky(content, pWPSky, cActionSkyChanged::Ref::New(*this), "Sky:");
	helper.WPWObject(content, pWPEnvObject, cActionEnvObjChanged::Ref::New(*this), "Environment Object:");
	helper.WPCamera(content, pWPCamera, cActionCameraChanged::Ref::New(*this), "Camera:");
}

seWPView::~seWPView(){
	SetSkin(nullptr);
}



// Management
///////////////

void seWPView::SetSkin(seSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPSky->SetSky(nullptr);
	pWPCamera->SetCamera(nullptr);
	
	if(pSkin){
		pSkin->RemoveListener(pListener);
		pSkin = nullptr;
	}
	
	pSkin = skin;
	
	decString defaultPath;
	
	if(skin){
		skin->AddListener(pListener);
		pWPSky->SetSky(skin->GetSky());
		pWPEnvObject->SetObject(skin->GetEnvObject());
		pWPCamera->SetCamera(skin->GetCamera());
		
		defaultPath = skin->GetDirectoryPath();
	}
	
	pEditModelPath->SetDefaultPath(defaultPath);
	pEditRigPath->SetDefaultPath(defaultPath);
	pEditAnimPath->SetDefaultPath(defaultPath);
	
	UpdateMoveList();
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdateCamera();
}



void seWPView::UpdateView(){
	if(pSkin){
		pCBPreviewMode->SetSelectionWithData((void*)(intptr_t)pSkin->GetPreviewMode());
		pEditModelPath->SetPath(pSkin->GetModelPath());
		pEditRigPath->SetPath(pSkin->GetRigPath());
		pEditAnimPath->SetPath(pSkin->GetAnimationPath());
		pCBAnimMoves->SetText(pSkin->GetMoveName());
		
	}else{
		pCBPreviewMode->SetSelectionWithData((void*)(intptr_t)seSkin::epmModel);
		pEditModelPath->ClearPath();
		pEditRigPath->ClearPath();
		pEditAnimPath->ClearPath();
		pCBAnimMoves->ClearText();
	}
	
	const bool enabled = pSkin;
	pCBPreviewMode->SetEnabled(enabled);
	pEditModelPath->SetEnabled(enabled);
	pEditRigPath->SetEnabled(enabled);
	pEditAnimPath->SetEnabled(enabled);
	pCBAnimMoves->SetEnabled(enabled);
	
	pBtnRewindTextures->GetAction()->Update();
	pChkPlayback->GetAction()->Update();
}

void seWPView::UpdateMoveList(){
	const deAnimator * const engAnimator = pSkin ? pSkin->GetEngineAnimator() : nullptr;
	const decString selection(pCBAnimMoves->GetText());
	
	pPreventUpdate = true;
	
	try{
		pCBAnimMoves->RemoveAllItems();
		
		if(engAnimator){
			const deAnimation * const animation = engAnimator->GetAnimation();
			if(animation){
				const int count = animation->GetMoveCount();
				int i;
				for(i=0; i<count; i++){
					pCBAnimMoves->AddItem(animation->GetMove(i)->GetName());
				}
			}
			
			pCBAnimMoves->SortItems();
			pCBAnimMoves->StoreFilterItems();
		}
		
		pCBAnimMoves->SetText(selection);
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void seWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void seWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void seWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
}
