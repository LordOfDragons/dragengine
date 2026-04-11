/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "mePreviewCamera.h"
#include "mePreviewCameraListener.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>


// Class mePreviewCamera
//////////////////////////

// Constructor, destructor
////////////////////////////

mePreviewCamera::mePreviewCamera(meWindowMain &windowMain, deCanvasView &container) :
pWindowMain(windowMain),
pListener(mePreviewCameraListener::Ref::New(*this)),
pRenderWorldSize(windowMain.GetConfiguration().GetPreviewCameraSize())
{
	igdeFont::sConfiguration configuration;
	windowMain.GetEnvironment().GetApplicationFont(configuration);
	
	configuration.size *= 0.75f;
	pFontLabel = windowMain.GetEnvironment().GetSharedFont(configuration);
	
	pCreateContent(container);
}

mePreviewCamera::~mePreviewCamera(){
	SetWorld(nullptr);
}


// Management
///////////////

void mePreviewCamera::SetWorld(meWorld *world){
	if(pWorld == world){
		return;
	}
	
	pCameras.RemoveAll();
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		
		SetRenderWorldSize(pWindowMain.GetConfiguration().GetPreviewCameraSize());
		
		RebuildCameras();
	}
}

bool mePreviewCamera::HasObject(meObject *object) const{
	return pCameras.HasMatching([&](const mePreviewCameraCamera &pcc){
		return pcc.GetObject() == object;
	});
}

void mePreviewCamera::SetRenderWorldSize(const decPoint &size){
	if(pRenderWorldSize == size){
		return;
	}
	
	pRenderWorldSize = size;
	RebuildCameras();
}

void mePreviewCamera::RebuildCameras(){
	pCameras.RemoveAll();
	
	if(!pWorld){
		return;
	}
	
	int y = 0, width = 0;
	pWorld->GetObjects().Visit([&](meObject *object){
		if(!object->GetCamera()
		|| !object->GetCameraShowPreview()
		|| !object->GetCamera()->GetEngineCamera()){
			return;
		}
		
		auto camera = mePreviewCameraCamera::Ref::New(*this, object);
		camera->GetPanel()->SetPosition({0, y});
		y += camera->GetPanel()->GetSize().y;
		width = decMath::max(width, camera->GetPanel()->GetSize().x);
		
		pCameras.Add(camera);
	});
	
	pCanvasPanel->SetSize({width, y});
	
	decPoint position;
	if(pCanvasPanel->GetParentView()){
		position = pCanvasPanel->GetParentView()->GetSize() - pCanvasPanel->GetSize();
	}
	pCanvasPanel->SetPosition(position);
}

void mePreviewCamera::OnParentResized(){
	RebuildCameras();
}

void mePreviewCamera::OnFrameUpdate(float elapsed){
	pCanvasPanel->SetTransparency(pWindowMain.GetConfiguration().GetPreviewCameraTransparency());
	
	pCameras.Visit([&](mePreviewCameraCamera &camera){
		camera.Update(elapsed);
	});
}


// Private Functions
//////////////////////

void mePreviewCamera::pCreateContent(deCanvasView &container){
	deCanvasManager &canvasMgr = *pWindowMain.GetEngine()->GetCanvasManager();
	
	pCanvasPanel = canvasMgr.CreateCanvasView();
	pCanvasPanel->SetOrder((float)container.GetCanvasCount());
	container.AddCanvas(pCanvasPanel);
}
