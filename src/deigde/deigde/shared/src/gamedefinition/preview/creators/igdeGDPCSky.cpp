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

#include "igdeGDPCSky.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/sky/igdeGDSky.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../gui/wrapper/igdeWSky.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Class igdeGDPCSky
//////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPCSky::igdeGDPCSky(igdeEnvironment &environment, igdeGDSky *gdsky, const decPoint &size) :
igdeGDPreviewCreator(environment, size),
pGDSky(gdsky),
pSky(NULL)
{
	if(!gdsky){
		DETHROW(deeInvalidParam);
	}
}

igdeGDPCSky::~igdeGDPCSky(){
	if(pSky){
		delete pSky;
	}
}



// Management
///////////////

decString igdeGDPCSky::DebugPrefix(){
	return decString("GDPCSky(") + (pGDSky ? pGDSky->GetName() : decString("?")) + ")";
}

void igdeGDPCSky::PrepareCanvasForRender(){
	igdeEnvironment &environment = GetEnvironment();
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	// create world
	pWorld = engine.GetWorldManager()->CreateWorld();
	
	// create camera
	pCamera = engine.GetCameraManager()->CreateCamera();
	pCamera->SetFov(45.0f * DEG2RAD);
	pCamera->SetFovRatio(1.0f);
	pCamera->SetImageDistance(0.01f);
	pCamera->SetViewDistance(500.0f);
	pCamera->SetExposure(1.0f);
	pCamera->SetAdaptionTime(0.0f);
	pWorld->AddCamera(pCamera);
	
	// create sky
	pSky = new igdeWSky(environment);
	pSky->SetWorld(pWorld);
	pSky->SetGDSky(pGDSky);
	
	// adjust camera parameters to fit the sky lighting
	pCamera->SetLowestIntensity(pSky->GetMaxLightIntensity());
	pCamera->SetHighestIntensity(pSky->GetMaxLightIntensity());
	
	// create render world canvas
	deCanvasView &container = *GetCanvas();
	pCanvasRenderWorld = engine.GetCanvasManager()->CreateCanvasRenderWorld();
	pCanvasRenderWorld->SetCamera(pCamera);
	pCanvasRenderWorld->SetSize(container.GetSize());
	container.AddCanvas(pCanvasRenderWorld);
}

bool igdeGDPCSky::IsCanvasReadyForRender(){
	pWorld->Update(0.0f);
	return true;
}

void igdeGDPCSky::UpdateCanvasForRender(float elapsed){
	pWorld->Update(elapsed);
}
