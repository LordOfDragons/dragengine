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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saeWPView.h"
#include "saeWPViewListener.h"
#include "saeWindowProperties.h"
#include "../saeWindowMain.h"
#include "../../sanimation/saeSAnimation.h"
#include "../../configuration/saeConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPCamera.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	saeWPView &pPanel;
	
public:
	cBaseAction(saeWPView &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	void OnAction() override{
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if(sanimation){
			OnAction(*sanimation);
		}
	}
	
	virtual void OnAction(saeSAnimation &sanimation) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	saeWPView &pPanel;
	
public:
	cBaseEditPathListener(saeWPView &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if(sanimation){
			OnChanged(editPath->GetPath(), *sanimation);
		}
	}
	
	virtual void OnChanged(const decString &path, saeSAnimation &sanimation) = 0;
};



class cActionCameraChanged : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionCameraChanged>;
	cActionCameraChanged(saeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(saeSAnimation &sanimation) override{
		sanimation.NotifyCameraChanged();
	}
};

class cActionSkyChanged : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionSkyChanged>;
	cActionSkyChanged(saeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(saeSAnimation &sanimation) override{
		sanimation.NotifySkyChanged();
	}
};



class cEditDisplayModelPath : public cBaseEditPathListener{
public:
	using Ref = deTObjectReference<cEditDisplayModelPath>;
	cEditDisplayModelPath(saeWPView &panel) : cBaseEditPathListener(panel){}
	
	void OnChanged(const decString &path, saeSAnimation &sanimation) override{
		sanimation.SetDisplayModelPath(path);
	}
};

class cEditDisplaySkinPath : public cBaseEditPathListener{
public:
	using Ref = deTObjectReference<cEditDisplaySkinPath>;
	cEditDisplaySkinPath(saeWPView &panel) : cBaseEditPathListener(panel){}
	
	void OnChanged(const decString &path, saeSAnimation &sanimation) override{
		sanimation.SetDisplaySkinPath(path);
	}
};

class cEditDisplayRigPath : public cBaseEditPathListener{
public:
	using Ref = deTObjectReference<cEditDisplayRigPath>;
	cEditDisplayRigPath(saeWPView &panel) : cBaseEditPathListener(panel){}
	
	void OnChanged(const decString &path, saeSAnimation &sanimation) override{
		sanimation.SetDisplayRigPath(path);
	}
};

}



// Class saeWPView
////////////////////

// Constructor, destructor
////////////////////////////

saeWPView::saeWPView(saeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox;
	igdeAction::Ref action;
	
	pListener = saeWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// display
	helper.GroupBox(content, groupBox, "@SpeechAnimation.WPView.GroupDisplay");
	
	helper.EditPath(groupBox, "@SpeechAnimation.WPView.DisplayModel", "@SpeechAnimation.WPView.DisplayModel.Description",
		igdeEnvironment::efpltModel, pEditDisplayModelPath, cEditDisplayModelPath::Ref::New(*this));
	helper.EditPath(groupBox, "@SpeechAnimation.WPView.DisplaySkin", "@SpeechAnimation.WPView.DisplaySkin.Description",
		igdeEnvironment::efpltSkin, pEditDisplaySkinPath, cEditDisplaySkinPath::Ref::New(*this));
	helper.EditPath(groupBox, "@SpeechAnimation.WPView.DisplayRig", "@SpeechAnimation.WPView.DisplayRig.Description",
		igdeEnvironment::efpltRig, pEditDisplayRigPath, cEditDisplayRigPath::Ref::New(*this));
	
	// property panels
	action = cActionCameraChanged::Ref::New(*this);
	helper.WPCamera(content, pWPCamera, action, "@SpeechAnimation.WPView.Camera");
	
	action = cActionSkyChanged::Ref::New(*this);
	helper.WPSky(content, pWPSky, action, "@SpeechAnimation.WPView.Sky");
}

saeWPView::~saeWPView(){
	SetSAnimation(nullptr);
}



// Management
///////////////

void saeWPView::SetSAnimation(saeSAnimation *sanimation){
	if(sanimation == pSAnimation){
		return;
	}
	
	pWPSky->SetSky(nullptr);
	pWPCamera->SetCamera(nullptr);
	
	if(pSAnimation){
		pSAnimation->RemoveListener(pListener);
	}
	
	pSAnimation = sanimation;
	
	decString defaultPath;
	
	if(sanimation){
		sanimation->AddListener(pListener);
		pWPSky->SetSky(sanimation->GetSky());
		pWPCamera->SetCamera(sanimation->GetCamera());
		
		defaultPath = sanimation->GetDirectoryPath();
	}
	
	pEditDisplayModelPath->SetDefaultPath(defaultPath);
	pEditDisplaySkinPath->SetDefaultPath(defaultPath);
	pEditDisplayModelPath->SetDefaultPath(defaultPath);
	
	UpdateView();
	UpdateSky();
	UpdateCamera();
}

void saeWPView::UpdateView(){
	if(pSAnimation){
		pEditDisplayModelPath->SetPath(pSAnimation->GetDisplayModelPath());
		pEditDisplaySkinPath->SetPath(pSAnimation->GetDisplaySkinPath());
		pEditDisplayRigPath->SetPath(pSAnimation->GetDisplayRigPath());
		
	}else{
		pEditDisplayModelPath->ClearPath();
		pEditDisplaySkinPath->ClearPath();
		pEditDisplayRigPath->ClearPath();
	}
	
	const bool enabled = pSAnimation != nullptr;
	pEditDisplayModelPath->SetEnabled(enabled);
	pEditDisplaySkinPath->SetEnabled(enabled);
	pEditDisplayRigPath->SetEnabled(enabled);
}

void saeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void saeWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
