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

#include "gdeOCNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCNavigationSpace
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCNavigationSpace::gdeOCNavigationSpace() :
pLayer(0),
pSnapDistance(0.001f),
pSnapAngle(180.0),
pType(deNavigationSpace::estMesh),
pBlockingPriority(0){
}

gdeOCNavigationSpace::gdeOCNavigationSpace(const gdeOCNavigationSpace &space) :
pPath(space.pPath),
pPosition(space.pPosition),
pRotation(space.pRotation),
pBoneName(space.pBoneName),
pLayer(space.pLayer),
pSnapDistance(space. pSnapDistance),
pSnapAngle(space.pSnapAngle),
pType(space.pType),
pBlockingPriority(space.pBlockingPriority),
pBlockerShapeList(space.pBlockerShapeList)
{
	int i;
	
	for(i=0; i<=epAttachRotation; i++){
		pPropertyNames[i] = space.pPropertyNames[i];
	}
}

gdeOCNavigationSpace::~gdeOCNavigationSpace(){
}



// Management
///////////////

void gdeOCNavigationSpace::SetPath(const char *path){
	pPath = path;
}

void gdeOCNavigationSpace::SetPosition(const decVector &position){
	pPosition = position;
}

void gdeOCNavigationSpace::SetRotation(const decVector &orientation){
	pRotation = orientation;
}

void gdeOCNavigationSpace::SetBoneName(const char *boneName){
	pBoneName = boneName;
}



void gdeOCNavigationSpace::SetLayer(int layer){
	pLayer = layer;
}

void gdeOCNavigationSpace::SetType(deNavigationSpace::eSpaceTypes type){
	pType = type;
}

void gdeOCNavigationSpace::SetBlockingPriority(int priority){
	pBlockingPriority = priority;
}



void gdeOCNavigationSpace::SetSnapDistance(float distance){
	pSnapDistance = decMath::max(distance, 0.001f);
}

void gdeOCNavigationSpace::SetSnapAngle(float angle){
	pSnapAngle = decMath::max(angle, 0.0f);
}



bool gdeOCNavigationSpace::IsPropertySet(eProperties property) const{
	return ! pPropertyNames[property].IsEmpty();
}

const decString &gdeOCNavigationSpace::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void gdeOCNavigationSpace::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}

bool gdeOCNavigationSpace::HasPropertyWithName(const char *name) const{
	int i;
	for(i=0; i<=epAttachRotation; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	
	return false;
}
