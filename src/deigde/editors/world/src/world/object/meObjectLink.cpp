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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "meObject.h"
#include "meObjectLink.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeShapeBuilder.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWConnection.h>



// Class meObjectLink
///////////////////////

// Constructor, destructor
////////////////////////////

meObjectLink::meObjectLink(igdeEnvironment *environment, meObject *anchor, meObject *target){
	if(!environment || !anchor || !target){
		DETHROW(deeInvalidParam);
	}
	
	pEnvironment = environment;
	pWorld = NULL;
	
	pDebugDrawer = NULL;
	pDDSConnection = NULL;
	
	pAnchor = NULL;
	pTarget = NULL;
	
	try{
		pAnchor = anchor;
		anchor->AddReference();
		
		pTarget = target;
		target->AddReference();
		
		// create debug drawer and shapes
		pDebugDrawer = environment->GetEngineController()->GetEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSConnection = new igdeWConnection;
		pDDSConnection->SetArrowSize(0.05f);
		pDDSConnection->SetArrowOnAnchor(false);
		pDDSConnection->SetArrowOnTarget(true);
		pDDSConnection->SetColor(decColor(1.0f, 0.0f, 1.0f));
		pDDSConnection->SetParentDebugDrawer(pDebugDrawer);
		
		// update the rest
		pUpdateConnection();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meObjectLink::~meObjectLink(){
	pCleanUp();
}



// Management
///////////////

void meObjectLink::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
	}
	
	pWorld = world;
	
	if(world){
		world->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
	}
}

void meObjectLink::SetAnchorProperty(const char *property){
	pAnchorProperty = property;
}

void meObjectLink::SetTargetProperty(const char *property){
	pTargetProperty = property;
}

void meObjectLink::ObjectsMoved(){
	pUpdateConnection();
}



// Private Functions
//////////////////////

void meObjectLink::pCleanUp(){
	SetWorld(NULL);
	
	if(pTarget){
		pTarget->FreeReference();
	}
	if(pAnchor){
		pAnchor->FreeReference();
	}
	
	if(pDDSConnection){
		delete pDDSConnection;
	}
	if(pDebugDrawer){
		pDebugDrawer->FreeReference();
	}
}

void meObjectLink::pUpdateConnection(){
	const decDVector &anchorPosition = pAnchor->GetPosition();
	const decDVector &targetPosition = pTarget->GetPosition();
	
	pDebugDrawer->SetPosition(anchorPosition);
	pDDSConnection->SetTargetPosition((targetPosition - anchorPosition).ToVector());
}
