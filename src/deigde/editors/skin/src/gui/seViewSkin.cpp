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

#include "seViewSkin.h"
#include "seWindowMain.h"
#include "../skin/seSkin.h"

#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/resources/rendering/deRenderWindow.h>



// Definitions
////////////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	seViewSkin &pView;
	
public:
	cCameraInteraction(seViewSkin &view) : pView(view){}
	
public:
	virtual void OnCameraChanged(){
		if(pView.GetSkin()){
			pView.GetSkin()->NotifyCameraChanged();
		}
	}
};

}



// Class seViewSkin
/////////////////////

// Constructor, destructor
////////////////////////////

seViewSkin::seViewSkin(seWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pSkin(NULL)
{
	pCameraInteraction.TakeOver(new cCameraInteraction(*this));
	
	AddListener(pCameraInteraction);
}

seViewSkin::~seViewSkin(){
	SetSkin(NULL);
}



// Management
///////////////

void seViewSkin::ResetView(){
}

void seViewSkin::SetSkin(seSkin *skin){
	if(pSkin == skin){
		return;
	}
	
	pCameraInteraction->SetCamera(NULL);
	
	SetRenderWorld(NULL);
	
	
	pSkin = skin;
	
	if(skin){
		skin->AddReference();
		SetRenderWorld(skin->GetCamera()->GetEngineCamera());
		pCameraInteraction->SetCamera(skin->GetCamera());
	}
}



void seViewSkin::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pSkin){
		pSkin->Update(elapsed);
	}
}

void seViewSkin::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pSkin){
		SetRenderWorld(pSkin->GetCamera()->GetEngineCamera());
	}
}
