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

#include "gdeVAONavBlocker.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAONavBlocker
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAONavBlocker::gdeVAONavBlocker(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCNavigationBlocker *ocnavblocker) :
gdeVAOSubObject(view, objectClass, propertyPrefix),
pOCNavBlocker(ocnavblocker),
pDDSBlocker(NULL)
{
	if(!ocnavblocker){
		DETHROW(deeInvalidParam);
	}
	ocnavblocker->AddReference();
	
	try{
		pCreateDebugDrawer();
		pBuildDDSBlocker();
		pUpdateDDShapeColor();
		UpdateDDVisibility();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

gdeVAONavBlocker::~gdeVAONavBlocker(){
	pCleanUp();
}



// Management
///////////////

void gdeVAONavBlocker::RebuildResources(){
	pBuildDDSBlocker();
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAONavBlocker::SelectedObjectChanged(){
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAONavBlocker::UpdateDDVisibility(){
	const gdeGameDefinition &gameDefinition = *pView.GetGameDefinition();
	
	pDebugDrawer->SetVisible(pView.GetShowNavBlockers()
		|| (gameDefinition.GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker
			&& gameDefinition.GetActiveOCNavigationBlocker() == pOCNavBlocker));
}



// Private functions
//////////////////////

void gdeVAONavBlocker::pCleanUp(){
	if(pDDSBlocker){
		pDDSBlocker->SetParentDebugDrawer(NULL);
		delete pDDSBlocker;
	}
	if(pDebugDrawer){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer(pDebugDrawer);
		pDebugDrawer = NULL;
	}
	
	if(pOCNavBlocker){
		pOCNavBlocker->FreeReference();
	}
}



void gdeVAONavBlocker::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// debug drawer
	pDebugDrawer.TakeOver(engine.GetDebugDrawerManager()->CreateDebugDrawer());
	pDebugDrawer->SetXRay(false);
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer(pDebugDrawer);
	
	// blocker
	pDDSBlocker = new igdeWDebugDrawerShape;
	pDDSBlocker->SetParentDebugDrawer(pDebugDrawer);
}

void gdeVAONavBlocker::pBuildDDSBlocker(){
	pDDSBlocker->RemoveAllShapes();
	
	const decShapeList &blockerShape = pOCNavBlocker->GetShapeList();
	if(blockerShape.GetCount() == 0){
		return;
	}
	
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	pDDSBlocker->SetEdgeColor(decColor(config.GetColorNavigationBlocker(), 1.0f));
	pDDSBlocker->SetFillColor(config.GetColorNavigationBlocker());
	pDDSBlocker->SetPosition(PropertyVector(pOCNavBlocker->GetPropertyName(
		gdeOCNavigationBlocker::epAttachPosition), pOCNavBlocker->GetPosition()));
	pDDSBlocker->SetOrientation(PropertyQuaternion(pOCNavBlocker->GetPropertyName(
		gdeOCNavigationBlocker::epAttachRotation), pOCNavBlocker->GetRotation()));
	pDDSBlocker->AddShapes(blockerShape);
}

void gdeVAONavBlocker::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if(pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker
	&& pView.GetGameDefinition()->GetActiveOCNavigationBlocker() == pOCNavBlocker){
		pDDSBlocker->SetEdgeColor(decColor(config.GetColorNavigationBlockerActive(), 1.0f));
		pDDSBlocker->SetFillColor(config.GetColorNavigationBlockerActive());
		
	}else{
		pDDSBlocker->SetEdgeColor(decColor(config.GetColorNavigationBlocker(), 1.0f));
		pDDSBlocker->SetFillColor(config.GetColorNavigationBlocker());
	}
}
