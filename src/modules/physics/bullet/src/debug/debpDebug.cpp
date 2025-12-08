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
#include <stdlib.h>

#include "debpDebug.h"
#include "debpDebugInformation.h"
#include "debpDebugInformationList.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/exceptions.h>



// Class debpDebug
////////////////////

// Constructor, destructor
////////////////////////////

debpDebug::debpDebug(dePhysicsBullet &bullet) :
pBullet(bullet),
pEnabled(false),
pDIColliderPrepareDetection(NULL),
pDIColliderDetectCustomCollision(NULL),
pDIColliderUpdateFromBody(NULL),
pDIColliderFinishDetection(NULL),
pDIColliderCollisionTests(NULL),
pDIColliderUpdateOctree(NULL),
pDITouchSensorApplyChanges(NULL),
pDIWorldStepSimulation(NULL),
pDIWorldUpdateOctrees(NULL),
pDIWorldCheckDynamicCollisions(NULL)
{
	pDIColliderPrepareDetection.TakeOver(new debpDebugInformation("Collider PrepareDetection:"));
	pDebugInfoList.Add(pDIColliderPrepareDetection);
	
	pDIColliderDetectCustomCollision.TakeOver(new debpDebugInformation("Collider DetectCustomCollision:"));
	pDebugInfoList.Add(pDIColliderDetectCustomCollision);
	
	pDIColliderUpdateFromBody.TakeOver(new debpDebugInformation("Collider UpdateFromBody:"));
	pDebugInfoList.Add(pDIColliderUpdateFromBody);
	
	pDIColliderFinishDetection.TakeOver(new debpDebugInformation("Collider FinishDetection:"));
	pDebugInfoList.Add(pDIColliderFinishDetection);
	
	pDIColliderCollisionTests.TakeOver(new debpDebugInformation("Collider CollisionTests:"));
	pDebugInfoList.Add(pDIColliderCollisionTests);
	
	pDIColliderUpdateOctree.TakeOver(new debpDebugInformation("Collider UpdateOctreePosition:"));
	pDebugInfoList.Add(pDIColliderUpdateOctree);
	
	pDITouchSensorApplyChanges.TakeOver(new debpDebugInformation("TouchSensor ApplyChanges:"));
	pDebugInfoList.Add(pDITouchSensorApplyChanges);
	
	pDIWorldStepSimulation.TakeOver(new debpDebugInformation("World StepSimulation:"));
	pDebugInfoList.Add(pDIWorldStepSimulation);
	
	pDIWorldUpdateOctrees.TakeOver(new debpDebugInformation("World UpdateOctrees:"));
	pDebugInfoList.Add(pDIWorldUpdateOctrees);
	
	pDIWorldCheckDynamicCollisions.TakeOver(new debpDebugInformation("World CheckDynamicCollisions:"));
	pDebugInfoList.Add(pDIWorldCheckDynamicCollisions);
}

debpDebug::~debpDebug(){
}



// Management
///////////////

void debpDebug::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
}



void debpDebug::BeginProcessPhysics(debpWorld *world){
	if(!pEnabled){
		return;
	}
	
	pDIColliderPrepareDetection->Clear();
	pDIColliderDetectCustomCollision->Clear();
	pDIColliderUpdateFromBody->Clear();
	pDIColliderFinishDetection->Clear();
	pDIColliderCollisionTests->Clear();
	pDIColliderUpdateOctree->Clear();
	pDITouchSensorApplyChanges->Clear();
	pDIWorldUpdateOctrees->Clear();
	pDIWorldStepSimulation->Clear();
	pDIWorldCheckDynamicCollisions->Clear();
}

void debpDebug::EndProcessPhysics(debpWorld *world){
	if(!pEnabled){
		return;
	}
	
	pUpdateDebugInformation();
	
	pBullet.LogInfoFormat("DebugInfo World %p", world);
	pLogDebugInformation();
}



// Private Functions
//////////////////////

void debpDebug::pUpdateDebugInformation(){
	const int count = pDebugInfoList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pUpdateDebugInformation(*pDebugInfoList.GetAt(i));
	}
}

void debpDebug::pUpdateDebugInformation(debpDebugInformation &debugInfo){
	decString text(debugInfo.GetName());
	
	if(debugInfo.HasElapsedTime()){
		text.AppendFormat(" %d.%01dms", (int)(debugInfo.GetElapsedTime() * 1000.0f),
			(int)(debugInfo.GetElapsedTime() * 10000.0f) % 10);
	}
	
	if(debugInfo.HasCounter()){
		text.AppendFormat(" %dx", debugInfo.GetCounter());
		
		if(debugInfo.GetCounter() > 0){
			const double average = (double)debugInfo.GetElapsedTime() / (double)debugInfo.GetCounter();
			text.AppendFormat("(%dys)", (int)(average * 1e6f));
		}
	}
	
	debugInfo.SetText(text);
}

void debpDebug::pLogDebugInformation(){
	const int count = pDebugInfoList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pBullet.LogInfo(pDebugInfoList.GetAt(i)->GetText());
	}
}
