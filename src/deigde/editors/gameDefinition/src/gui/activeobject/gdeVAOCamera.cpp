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
#include <stdio.h>

#include "gdeVAOCamera.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/camera/gdeOCCamera.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOCamera
//////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOCamera::gdeVAOCamera(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCCamera *occamera) :
gdeVAOSubObject(view, objectClass, propertyPrefix),
pOCCamera(occamera),
pDDSCenter(NULL),
pDDSCoordSystem(NULL)
{
	if(!occamera){
		DETHROW(deeInvalidParam);
	}
	
	
	try{
		pCreateDebugDrawer();
		pUpdateDDShapes();
		pUpdateDDShapeColor();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

gdeVAOCamera::~gdeVAOCamera(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOCamera::RebuildResources(){
	pReleaseResources();
	
	pUpdateDDShapes();
	pUpdateDDShapeColor();
}

void gdeVAOCamera::SelectedObjectChanged(){
	pUpdateDDShapeColor();
}



// Private functions
//////////////////////

void gdeVAOCamera::pCleanUp(){
	pReleaseResources();
	
	if(pDDSCoordSystem){
		delete pDDSCoordSystem;
	}
	if(pDDSCenter){
		delete pDDSCenter;
	}
	if(pDebugDrawer){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer(pDebugDrawer);
		pDebugDrawer = NULL;
	}
	
}



void gdeVAOCamera::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// create debug drawer
	pDebugDrawer.TakeOver(engine.GetDebugDrawerManager()->CreateDebugDrawer());
	pDebugDrawer->SetXRay(true);
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer(pDebugDrawer);
	
	// create center shape
	pDDSCenter = new igdeWDebugDrawerShape;
	pDDSCenter->AddSphereShape(0.05f, decVector());
	pDDSCenter->SetParentDebugDrawer(pDebugDrawer);
	
	// create coordinate system shape
	pDDSCoordSystem = new igdeWCoordSysArrows;
	pDDSCoordSystem->SetArrowLength(0.2f);
	pDDSCoordSystem->SetArrowSize(0.01f);
	pDDSCoordSystem->SetParentDebugDrawer(pDebugDrawer);
}

void gdeVAOCamera::pUpdateDDShapes(){
	const decVector &position = pOCCamera->GetPosition();
	const decQuaternion orientation(decQuaternion::CreateFromEuler(
		pOCCamera->GetRotation() * DEG2RAD));
	
	pDDSCenter->SetPosition(position);
	pDDSCenter->SetOrientation(orientation);
	
	pDDSCoordSystem->SetPosition(position);
	pDDSCoordSystem->SetOrientation(orientation);
}

void gdeVAOCamera::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if(pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCCamera
	&& pView.GetGameDefinition()->GetActiveOCCamera() == pOCCamera){
		pDDSCenter->SetEdgeColor(decColor(config.GetColorCameraActive(), 1.0f));
		pDDSCenter->SetFillColor(config.GetColorCameraActive());
		
	}else{
		pDDSCenter->SetEdgeColor(decColor(config.GetColorCamera(), 0.25f));
		pDDSCenter->SetFillColor(config.GetColorCamera());
	}
}



void gdeVAOCamera::pReleaseResources(){
}
