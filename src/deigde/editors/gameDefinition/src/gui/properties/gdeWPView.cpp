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

#include "gdeWPView.h"
#include "gdeWPViewListener.h"
#include "gdeWindowProperties.h"
#include "../gdeWindowMain.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../configuration/gdeConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	gdeWPView &pPanel;
	
public:
	cBaseAction(gdeWPView &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if(gameDefinition){
			OnAction(*gameDefinition);
		}
	}
	
	virtual void OnAction(gdeGameDefinition &gameDefinition) = 0;
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged(gdeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(gdeGameDefinition &gameDefinition) override{
		gameDefinition.NotifyCameraChanged();
	}
};

class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged(gdeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(gdeGameDefinition &gameDefinition) override{
		gameDefinition.NotifySkyChanged();
	}
};

}



// Class gdeWPView
////////////////////

// Constructor, destructor
////////////////////////////

gdeWPView::gdeWPView(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pGameDefinition(NULL),
pListener(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content;
	
	pListener.TakeOverWith(*this);
	
	content.TakeOverWith(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.WPCamera(content, pWPCamera, new cActionCameraChanged(*this),
		"Camera:", false, false, true);
	helper.WPSky(content, pWPSky, new cActionSkyChanged(*this),
		"Environment Object:", false, false, true);
}

gdeWPView::~gdeWPView(){
	SetGameDefinition(NULL);
	
}



// Management
///////////////

void gdeWPView::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	pWPSky->SetSky(NULL);
	pWPCamera->SetCamera(NULL);
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		gameDefinition->AddReference();
		
		pWPCamera->SetCamera(gameDefinition->GetCamera());
		pWPSky->SetSky(gameDefinition->GetSky());
	}
	
	UpdateView();
	UpdateSky();
	UpdateCamera();
}

void gdeWPView::UpdateView(){
}

void gdeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void gdeWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
