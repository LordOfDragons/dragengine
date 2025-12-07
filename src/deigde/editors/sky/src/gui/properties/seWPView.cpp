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
#include "../seWindowMain.h"
#include "../../sky/seSky.h"

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
	seWPView &pPanel;
	
public:
	cBaseAction(seWPView &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seSky * const sky = pPanel.GetSky();
		if(sky){
			OnAction(*sky);
		}
	}
	
	virtual void OnAction(seSky &sky) = 0;
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged(seWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(seSky &sky) override{
		sky.NotifyCameraChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	cActionEnvObjChanged(seWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(seSky &sky) override{
		sky.NotifyEnvObjectChanged();
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
pSky(NULL),
pListener(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content;
	
	pListener = new seWPViewListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	helper.WPCamera(content, pWPCamera, new cActionCameraChanged(*this),
		"Camera:", false, false, true);
	helper.WPWObject(content, pWPEnvObject, new cActionEnvObjChanged(*this),
		"Environment Object:", false, false, true);
}

seWPView::~seWPView(){
	SetSky(NULL);
	
}



// Management
///////////////

void seWPView::SetSky(seSky *sky){
	if(sky == pSky){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPCamera->SetCamera(NULL);
	
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = NULL;
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
		sky->AddReference();
		
		pWPCamera->SetCamera(sky->GetCamera());
		pWPEnvObject->SetObject(sky->GetEnvObject());
	}
	
	UpdateView();
	UpdateEnvObject();
	UpdateCamera();
}



void seWPView::UpdateView(){
}

void seWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void seWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
