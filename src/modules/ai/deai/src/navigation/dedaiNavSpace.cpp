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
#include <string.h>

#include "dedaiNavSpace.h"
#include "spaces/dedaiSpace.h"
#include "../deDEAIModule.h"
#include "../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>



// Class dedaiNavSpace
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavSpace::dedaiNavSpace(deDEAIModule &deai, const deNavigationSpace &navigationSpace) :
pDEAI(deai),
pNavigationSpace(navigationSpace),
pParentWorld(NULL),
pSpace(NULL)
{
	try{
		pSpace = new dedaiSpace(deai);
		pSpace->SetOwnerNavSpace(this);
		pSpace->SetPosition(navigationSpace.GetPosition());
		pSpace->SetOrientation(navigationSpace.GetOrientation());
		pSpace->SetLayerNumber(navigationSpace.GetLayer());
		pSpace->SetType(navigationSpace.GetType());
		pSpace->SetSnapDistance(navigationSpace.GetSnapDistance());
		pSpace->SetSnapAngle(navigationSpace.GetSnapAngle());
		pSpace->SetBlockerShape(navigationSpace.GetBlockerShapeList());
		pSpace->SetBlockingPriority(navigationSpace.GetBlockingPriority());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dedaiNavSpace::~dedaiNavSpace(){
	pCleanUp();
}



// Management
///////////////

void dedaiNavSpace::SetParentWorld(dedaiWorld *world){
	pParentWorld = world;
	pSpace->SetParentWorld(world);
}



// Notifications
//////////////////

void dedaiNavSpace::PositionChanged(){
	pSpace->SetPosition(pNavigationSpace.GetPosition());
}

void dedaiNavSpace::OrientationChanged(){
	pSpace->SetOrientation(pNavigationSpace.GetOrientation());
}

void dedaiNavSpace::LayerChanged(){
	pSpace->SetLayerNumber(pNavigationSpace.GetLayer());
}

void dedaiNavSpace::TypeChanged(){
	pSpace->SetType(pNavigationSpace.GetType());
}

void dedaiNavSpace::SnappingChanged(){
	pSpace->SetSnapDistance(pNavigationSpace.GetSnapDistance());
	pSpace->SetSnapAngle(pNavigationSpace.GetSnapAngle());
}

void dedaiNavSpace::BlockerShapeChanged(){
	pSpace->SetBlockerShape(pNavigationSpace.GetBlockerShapeList());
}

void dedaiNavSpace::BlockingPriorityChanged(){
	pSpace->SetBlockingPriority(pNavigationSpace.GetBlockingPriority());
}

void dedaiNavSpace::LayoutChanged(){
	pSpace->OwnerLayoutChanged();
}



// Private Functions
//////////////////////

void dedaiNavSpace::pCleanUp(){
	SetParentWorld(NULL);
	
	if(pSpace){
		delete pSpace;
	}
}
