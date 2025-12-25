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

#include "skyeWPView.h"
#include "skyeWPViewListener.h"
#include "skyeWindowProperties.h"
#include "../skyeWindowMain.h"
#include "../../sky/skyeSky.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	skyeWPView &pPanel;
	
public:
	cBaseAction(skyeWPView &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	void OnAction() override{
		skyeSky * const sky = pPanel.GetSky();
		if(sky){
			OnAction(*sky);
		}
	}
	
	virtual void OnAction(skyeSky &sky) = 0;
};

class cActionCameraChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraChanged> Ref;
	cActionCameraChanged(skyeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(skyeSky &sky) override{
		sky.NotifyCameraChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnvObjChanged> Ref;
	cActionEnvObjChanged(skyeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(skyeSky &sky) override{
		sky.NotifyEnvObjectChanged();
	}
};

}



// Class skyeWPView
///////////////////

// Constructor, destructor
////////////////////////////

skyeWPView::skyeWPView(skyeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content;
	
	pListener = skyeWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.WPCamera(content, pWPCamera, cActionCameraChanged::Ref::New(*this),
		"Camera:", false, false);
	helper.WPWObject(content, pWPEnvObject, cActionEnvObjChanged::Ref::New(*this),
		"Environment Object:", false, false);
}

skyeWPView::~skyeWPView(){
	SetSky(nullptr);
}



// Management
///////////////

void skyeWPView::SetSky(skyeSky *sky){
	if(sky == pSky){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPCamera->SetCamera(nullptr);
	
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = nullptr;
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
		pWPCamera->SetCamera(sky->GetCamera());
		pWPEnvObject->SetObject(sky->GetEnvObject());
	}
	
	UpdateView();
	UpdateEnvObject();
	UpdateCamera();
}



void skyeWPView::UpdateView(){
}

void skyeWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void skyeWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
