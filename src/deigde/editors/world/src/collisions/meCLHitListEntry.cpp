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

#include "meCLHitListEntry.h"
#include "../world/meWorld.h"
#include "../world/object/meObjectSnapPoint.h"
#include "../world/terrain/meHeightTerrainSector.h"

#include <dragengine/common/exceptions.h>


// Class meCLHitListEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

meCLHitListEntry::meCLHitListEntry() :
pHTNavSpacePoint(-1),
pDistance(0.0f){
}


// Management
///////////////

void meCLHitListEntry::Clear(){
	pObject = nullptr;
	pObjectShape = nullptr;
	pDecal = nullptr;
	pNavSpace = nullptr;
	pHTNavSpacePoint = -1;
	pHTSector = nullptr;
	pSnapPoint = nullptr;
	pDistance = 0.0f;
	pNormal.Set(0.0f, 1.0f, 0.0f);
}

void meCLHitListEntry::SetObject(meObject *object){
	pObject = object;
}

void meCLHitListEntry::SetObjectShape(meObjectShape *objectShape){
	pObjectShape = objectShape;
}

void meCLHitListEntry::SetDecal(meDecal *decal){
	pDecal = decal;
}

void meCLHitListEntry::SetNavigationSpace(meNavigationSpace *navspace){
	pNavSpace = navspace;
}

void meCLHitListEntry::SetHTNavSpacePoint(int point){
	pHTNavSpacePoint = point;
}

void meCLHitListEntry::SetHTSector(meHeightTerrainSector *htsector){
	pHTSector = htsector;
}

void meCLHitListEntry::SetSnapPoint(meObjectSnapPoint *snapPoint){
	pSnapPoint = snapPoint;
}

void meCLHitListEntry::SetDistance(float distance){
	pDistance = distance;
}

void meCLHitListEntry::SetNormal(const decVector &normal){
	pNormal = normal;
}

void meCLHitListEntry::SortDecals(){
	// TODO
	DETHROW( deeInvalidParam );
}

int meCLHitListEntry::CompareTo(const meCLHitListEntry &entry) const{
	const float difference = entry.GetDistance() - pDistance;
	
	if(difference > FLOAT_SAFE_EPSILON){
		return -1;
	}
	if(difference < -FLOAT_SAFE_EPSILON){
		return 1;
	}
	
	if(entry.pDecal == pDecal
	&& entry.pObject == pObject
	&& entry.pObjectShape == pObjectShape
	&& entry.pNavSpace == pNavSpace
	&& entry.pHTNavSpacePoint == pHTNavSpacePoint
	&& entry.pHTSector == pHTSector
	&& entry.pSnapPoint == pSnapPoint){
		int myIndex = 0;
		int otherIndex = 0;
		
		if(pObject){
			myIndex = pObject->IndexOfDecal(pDecal);
			otherIndex = entry.pObject->IndexOfDecal(entry.pDecal);
			
			if(myIndex < otherIndex){
				return 1;
				
			}else if(myIndex > otherIndex){
				return -1;
			}
		}
	}
	
	return 0;
}

bool meCLHitListEntry::IsSame(const meCLHitListEntry &entry) const{
	return pObject == entry.GetObject()
		&& pObjectShape == entry.GetObjectShape()
		&& pDecal == entry.GetDecal()
		&& pNavSpace == entry.GetNavigationSpace()
		&& pHTNavSpacePoint == entry.GetHTNavSpacePoint()
		&& pHTSector == entry.GetHTSector()
		&& pSnapPoint == entry.GetSnapPoint();
}
