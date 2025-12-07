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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mePathFindTest.h"
#include "types/mePathFindTestType.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/navigator/deNavigatorType.h>
#include <dragengine/resources/navigation/navigator/deNavigatorManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"


 
// Class mePathFindTest
/////////////////////////

// Constructor, destructor
////////////////////////////

mePathFindTest::mePathFindTest(deEngine *engine){
	if(!engine){
		DETHROW(deeInvalidParam);
	}
	
	pEngine = engine;
	pWorld = NULL;
	
	pDDSPath = NULL;
	pEngNavigator = NULL;
	
	pLayer = 0;
	pBlockingCost = 1000.0f;
	pSpaceType = deNavigationSpace::estMesh;
	pShowPath = false;
	pDirtyPath = true;
	
	try{
		pEngNavigator = engine->GetNavigatorManager()->CreateNavigator();
		pEngNavigator->SetSpaceType(pSpaceType);
		pEngNavigator->SetMaxOutsideDistance(0.5f);
		pEngNavigator->SetBlockingCost(pBlockingCost);
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		pDebugDrawer->SetVisible(pShowPath);
		
		pDDSPath = new igdeWDebugDrawerShape;
		pDDSPath->SetEdgeColor(decColor(1.0f, 0.0f, 0.5f, 1.0f));
		pDDSPath->SetFillColor(decColor(1.0f, 0.0f, 0.5f, 0.25f));
		pDDSPath->SetParentDebugDrawer(pDebugDrawer);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

mePathFindTest::~mePathFindTest(){
	pCleanUp();
}



// Management
///////////////

void mePathFindTest::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->GetEngineWorld()->RemoveNavigator(pEngNavigator);
		pWorld->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
	}
	
	pWorld = world;
	
	if(world){
		world->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
		world->GetEngineWorld()->AddNavigator(pEngNavigator);
	}
	
	Invalidate();
}



void mePathFindTest::SetStartPosition(const decDVector &position){
	if(pStartPosition.IsEqualTo(position)){
		return;
	}
	
	pStartPosition = position;
	
	pDDSPath->SetPosition(position);
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetGoalPosition(const decDVector &position){
	if(pGoalPosition.IsEqualTo(position)){
		return;
	}
	
	pGoalPosition = position;
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetStartOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pStartOrientation)){
		return;
	}
	
	pStartOrientation = orientation;
	
	//const decQuaternion realor = decQuaternion::CreateFromEuler( orientation * DEG2RAD );
	//pEngNavigator->SetOrientation( realor );
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetGoalOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pGoalOrientation)){
		return;
	}
	
	pGoalOrientation = orientation;
	
	//const decQuaternion realor = decQuaternion::CreateFromEuler( orientation * DEG2RAD );
	//pEngNavigator->SetOrientation( realor );
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetLayer(int layer){
	if(layer == pLayer){
		return;
	}
	
	pLayer = layer;
	
	pEngNavigator->SetLayer(layer);
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetSpaceType(deNavigationSpace::eSpaceTypes spaceType){
	if(spaceType == pSpaceType){
		return;
	}
	
	pSpaceType = spaceType;
	
	pEngNavigator->SetSpaceType(spaceType);
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetPath(const deNavigatorPath &path){
	pPath = path;
}

void mePathFindTest::SetBlockingCost(float cost){
	if(fabsf(cost - pBlockingCost) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBlockingCost = cost;
	
	pEngNavigator->SetBlockingCost(cost);
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetShowPath(bool showPath){
	if(showPath == pShowPath){
		return;
	}
	
	pShowPath = showPath;
	pDebugDrawer->SetVisible(showPath);
	
	if(pWorld){
		pWorld->NotifyPathFindTestChanged();
	}
}



void mePathFindTest::NotifyTypesChanged(){
	pUpdateTypes();
	Invalidate();
	pWorld->NotifyPathFindTestChanged();
}



void mePathFindTest::Invalidate(){
	pDirtyPath = true;
}



void mePathFindTest::Update(){
	if(!pDirtyPath || !pShowPath){
		return;
	}
	
	pEngNavigator->FindPath(pPath, pStartPosition, pGoalPosition);
	pUpdateDDPath();
	pDirtyPath = false;
}



// Private Functions
//////////////////////

void mePathFindTest::pCleanUp(){
	SetWorld(NULL);
	
	
	if(pDDSPath){
		delete pDDSPath;
	}
}



void mePathFindTest::pUpdateDDPath(){
	if(!pDDSPath){
		return;
	}
	
	pDDSPath->RemoveAllFaces();
	
	const int count = pPath.GetCount();
	if(count == 0){
		return;
	}
	
	const int circlePointCount = 8;
	const float radius = 0.02f;
	const float circleStep = DEG2RAD * (360.0f / (float)circlePointCount);
	const decVector up(0.0f, 1.0f, 0.0f);
	
	deDebugDrawerShapeFace *ddsFace = NULL;
	decVector lastPoints[circlePointCount], points[circlePointCount];
	decVector prevPos, pos, nextPos;
	decMatrix matrix;
	decVector view;
	float len;
	int i, j;
	
	try{
		for(i=0; i<count; i++){
			pos = (pPath.GetAt(i) - pStartPosition).ToVector();
			
			if(i > 0){
				prevPos = (pPath.GetAt(i - 1) - pStartPosition).ToVector();
			}
			
			if(i < count - 1){
				nextPos = (pPath.GetAt(i + 1) - pStartPosition).ToVector();
				
			}else{
				nextPos = pos;
			}
			
			if(i == 0){
				view = pos - prevPos;
				len = view.Length();
				if(len < 0.001f){
					view.Set(0.0f, 0.0f, 1.0f);
					
				}else{
					view /= len;
				}
				
				matrix.SetWorld(decVector(), view, up);
				for(j=0; j<circlePointCount; j++){
					lastPoints[j] = matrix.Transform(radius * sinf(circleStep * (float)j), radius * cosf(circleStep * (float)j), 0.0f);
				}
			}
			
			view = nextPos - prevPos;
			len = view.Length();
			if(len < 0.001f){
				view.Set(0.0f, 0.0f, 1.0f);
				
			}else{
				view /= len;
			}
			
			matrix.SetWorld(pos, view, up);
			for(j=0; j<circlePointCount; j++){
				points[j] = matrix.Transform(radius * sinf(circleStep * (float)j), radius * cosf(circleStep * (float)j), 0.0f);
			}
			
			for(j=0; j<circlePointCount; j++){
				ddsFace = new deDebugDrawerShapeFace;
				ddsFace->AddVertex(lastPoints[j]);
				ddsFace->AddVertex(points[j]);
				ddsFace->AddVertex(points[(j + 1) % circlePointCount]);
				ddsFace->AddVertex(lastPoints[(j + 1) % circlePointCount]);
				pDDSPath->AddFace(ddsFace);
				ddsFace = NULL;
			}
			
			for(j=0; j<circlePointCount; j++){
				lastPoints[j] = points[j];
			}
		}
		
	}catch(const deException &){
		if(ddsFace){
			delete ddsFace;
		}
		throw;
	}
}

void mePathFindTest::pUpdateTypes(){
	const int count = pTypeList.GetCount();
	int i;
	
	pEngNavigator->RemoveAllTypes();
	
	for(i=0; i<count; i++){
		const mePathFindTestType &type = *pTypeList.GetAt(i);
		deNavigatorType &engType = *pEngNavigator->AddType(type.GetTypeNumber());
		
		engType.SetFixCost(type.GetFixCost());
		engType.SetCostPerMeter(type.GetCostPerMeter());
	}
	
	pEngNavigator->NotifyTypesChanged();
}
