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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "saeViewSAnimation.h"
#include "saeWindowMain.h"
#include "../configuration/saeConfiguration.h"
#include "../sanimation/saeSAnimation.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>



namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	saeViewSAnimation &pView;
	
public:
	typedef deTObjectReference<cCameraInteraction> Ref;
	cCameraInteraction(saeViewSAnimation &view) : pView(view){}
	
	virtual igdeMouseCameraListener::eInteraction ChooseInteraction(){
		if(!pView.GetSAnimation()){
			return eiNone;
		}
		return igdeMouseCameraListener::ChooseInteraction();
	}
	
	virtual void OnCameraChanged(){
		if(pView.GetSAnimation()){
			pView.GetSAnimation()->NotifyCameraChanged();
		}
	}
};

}



// Class saeViewSAnimation
////////////////////////////

// Constructor, destructor
////////////////////////////

saeViewSAnimation::saeViewSAnimation(saeWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	pFontStats = windowMain.GetEngine()->GetFontManager()->LoadFont("/igde/fonts/sans_10.defont", "/");
	
	pCameraInteraction = cCameraInteraction::Ref::New(*this);
	
	AddListener(pCameraInteraction);
}

saeViewSAnimation::~saeViewSAnimation(){
	SetSAnimation(nullptr);
}



// Management
///////////////

void saeViewSAnimation::ResetView(){
}

void saeViewSAnimation::SetSAnimation(saeSAnimation *sanimation){
	if(sanimation == pSAnimation){
		return;
	}
	
	pCameraInteraction->SetCamera(nullptr);
	
	SetRenderWorld(nullptr);
	pSAnimation = sanimation;
	
	if(sanimation){
		SetRenderWorld(sanimation->GetCamera()->GetEngineCamera());
		pCameraInteraction->SetCamera(sanimation->GetCamera());
	}
}



void saeViewSAnimation::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pSAnimation){
		pSAnimation->Update(elapsed);
	}
}

void saeViewSAnimation::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pSAnimation){
		SetRenderWorld(pSAnimation->GetCamera()->GetEngineCamera());
	}
}
