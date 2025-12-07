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
	cBaseAction(seWPView &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			OnAction(*skin);
		}
	}
	
	virtual void OnAction(seSkin &skin) = 0;
	
	virtual void Update(){
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
	cComboPreviewMode(seWPView &panel) : cBaseComboBoxListener(panel){}
	
	virtual void OnChanged(igdeComboBox &comboBox, seSkin &skin){
		if(comboBox.GetSelectedItem()){
			skin.SetPreviewMode((seSkin::ePreviewMode)(intptr_t)comboBox.GetSelectedItem()->GetData());
		}
	}
};

class cEditModelPath : public cBaseEditPathListener{
public:
	cEditModelPath(seWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin){
		skin.SetModelPath(editPath.GetPath());
	}
};

class cEditRigPath : public cBaseEditPathListener{
public:
	cEditRigPath(seWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin){
		skin.SetRigPath(editPath.GetPath());
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	cEditAnimationPath(seWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(igdeEditPath &editPath, seSkin &skin){
		skin.SetAnimationPath(editPath.GetPath());
	}
};

class cComboMove : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
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
	cActionPlayback(seWPView &panel) : cBaseAction(panel, "Play back animation",
		NULL, "Animation is played back"){}
	
	virtual void OnAction(seSkin &skin){
		skin.SetPlayback(!skin.GetPlayback());
	}
	
	virtual void Update(const seSkin &skin){
		SetEnabled(true);
		SetSelected(skin.GetPlayback());
	}
};

class cActionRewind : public cBaseAction{
public:
	cActionRewind(seWPView &panel) : cBaseAction(panel, "Rewind Textures", NULL,
		"Rewind all textures to the initial state as if added to the game world"){ }
	
	virtual void OnAction(seSkin &skin){
		skin.RewindTextures();
	}
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged(seWPView &panel) : cBaseAction(panel, "", NULL, ""){}
	
	virtual void OnAction(seSkin &skin){
		skin.NotifyCameraChanged();
	}
};

class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged(seWPView &panel) : cBaseAction(panel, "", NULL, ""){}
	
	virtual void OnAction(seSkin &skin){
		skin.NotifySkyChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	cActionEnvObjChanged(seWPView &panel) : cBaseAction(panel, "", NULL, ""){}
	
	virtual void OnAction(seSkin &skin){
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
pListener(NULL),
pPreventUpdate(false),
pSkin(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPViewListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// resources
	helper.GroupBox(content, groupBox, "Preview:");
	
	helper.ComboBox(groupBox, "Mode:", "Preview mode.", pCBPreviewMode, new cComboPreviewMode(*this));
	pCBPreviewMode->AddItem("Model", NULL, (void*)(intptr_t)seSkin::epmModel);
	pCBPreviewMode->AddItem("Light", NULL, (void*)(intptr_t)seSkin::epmLight);
	
	helper.EditPath(groupBox, "Model:", "Path to the model resource to use.",
		igdeEnvironment::efpltModel, pEditModelPath, new cEditModelPath(*this));
	helper.EditPath(groupBox, "Rig:", "Path to the rig resource to use.",
		igdeEnvironment::efpltRig, pEditRigPath, new cEditRigPath(*this));
	helper.EditPath(groupBox, "Animation:", "Path to the animation resource to use.",
		igdeEnvironment::efpltAnimation, pEditAnimPath, new cEditAnimationPath(*this));
	
	helper.ComboBoxFilter(groupBox, "Move:", true, "Name of the animation move to play.",
		pCBAnimMoves, new cComboMove(*this, pPreventUpdate));
	pCBAnimMoves->SetDefaultSorter();
	
	helper.CheckBox(groupBox, pChkPlayback, new cActionPlayback(*this), true);
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnRewindTextures, new cActionRewind(*this), true);
	
	// property panels
	helper.WPSky(content, pWPSky, new cActionSkyChanged(*this), "Sky:");
	helper.WPWObject(content, pWPEnvObject, new cActionEnvObjChanged(*this), "Environment Object:");
	helper.WPCamera(content, pWPCamera, new cActionCameraChanged(*this), "Camera:");
}

seWPView::~seWPView(){
	SetSkin(NULL);
	
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPView::SetSkin(seSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPSky->SetSky(NULL);
	pWPCamera->SetCamera(NULL);
	
	if(pSkin){
		pSkin->RemoveListener(pListener);
		pSkin->FreeReference();
		pSkin = NULL;
	}
	
	pSkin = skin;
	
	decString defaultPath;
	
	if(skin){
		skin->AddListener(pListener);
		skin->AddReference();
		
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
	const deAnimator * const engAnimator = pSkin ? pSkin->GetEngineAnimator() : NULL;
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
