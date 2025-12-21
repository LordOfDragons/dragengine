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

#include "peeViewEmitter.h"
#include "peeWindowMain.h"
#include "../configuration/peeConfiguration.h"
#include "../emitter/peeEmitter.h"

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
	peeViewEmitter &pView;
	
public:
	typedef deTObjectReference<cCameraInteraction> Ref;
	
	cCameraInteraction(peeViewEmitter &view) : pView(view){
		SetEnabledAll(false);
	}
	
public:
	virtual void OnCameraChanged(){
		if(pView.GetEmitter()){
			pView.GetEmitter()->NotifyCameraChanged();
		}
	}
};

}



// Class peeViewEmitter
/////////////////////////

// Constructor, destructor
////////////////////////////

peeViewEmitter::peeViewEmitter(peeWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	pCameraInteraction = cCameraInteraction::Ref::New(*this);
	
	AddListener(pCameraInteraction);
}

peeViewEmitter::~peeViewEmitter(){
	SetEmitter(nullptr);
}



// Management
///////////////

void peeViewEmitter::ResetView(){
}

void peeViewEmitter::SetEmitter(peeEmitter *emitter){
	if(emitter == pEmitter){
		return;
	}
	
	pCameraInteraction->SetCamera(nullptr);
	SetRenderWorld(nullptr);
	pEmitter = emitter;
	
	if(emitter){
		SetRenderWorld(emitter->GetCamera()->GetEngineCamera());
		pCameraInteraction->SetCamera(emitter->GetCamera());
	}
}



void peeViewEmitter::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pEmitter){
		pEmitter->Update(elapsed);
	}
}

void peeViewEmitter::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pEmitter){
		SetRenderWorld(pEmitter->GetCamera()->GetEngineCamera());
	}
}
