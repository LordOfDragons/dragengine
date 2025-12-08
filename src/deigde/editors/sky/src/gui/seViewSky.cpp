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

#include "seViewSky.h"
#include "seWindowMain.h"
#include "../configuration/seConfiguration.h"
#include "../sky/seSky.h"

#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>


// Events
///////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	seViewSky &pView;
	
public:
	cCameraInteraction(seViewSky &view) : pView(view){
		SetEnabledAll(false);
		SetEnableRotate(true);
	}
	
public:
	virtual void OnCameraChanged(){
		if(pView.GetSky()){
			pView.GetSky()->NotifyCameraChanged();
		}
	}
};

}


// Class seViewSky
////////////////////

// Constructor, destructor
////////////////////////////

seViewSky::seViewSky(seWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pSky(NULL)
{
	pCameraInteraction.TakeOver(new cCameraInteraction(*this));
	
	AddListener(pCameraInteraction);
}

seViewSky::~seViewSky(){
	SetSky(NULL);
}



// Management
///////////////

void seViewSky::ResetView(){
}

void seViewSky::SetSky(seSky *sky){
	if(pSky == sky){
		return;
	}
	
	pCameraInteraction->SetCamera(NULL);
	SetRenderWorld(NULL);
	
	
	pSky = sky;
	
	if(sky){
		sky->AddReference();
		SetRenderWorld(sky->GetCamera()->GetEngineCamera());
		pCameraInteraction->SetCamera(sky->GetCamera());
	}
}



void seViewSky::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pSky){
		pSky->Update(elapsed);
	}
}

void seViewSky::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pSky){
		SetRenderWorld(pSky->GetCamera()->GetEngineCamera());
	}
}
