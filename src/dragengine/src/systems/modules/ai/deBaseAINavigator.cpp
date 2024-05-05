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

#include "deBaseAINavigator.h"
#include "../../../resources/navigation/navigator/deNavigatorPath.h"


// Class deBaseAINavigator
////////////////////////////

// Constructres, destructors
//////////////////////////////

deBaseAINavigator::deBaseAINavigator(){
}

deBaseAINavigator::~deBaseAINavigator(){
}



// Notifications
//////////////////

void deBaseAINavigator::LayerChanged(){
}

void deBaseAINavigator::SpaceTypeChanged(){
}

void deBaseAINavigator::CostsChanged(){
}

void deBaseAINavigator::TypesChanged(){
}

void deBaseAINavigator::ParametersChanged(){
}



bool deBaseAINavigator::NearestPoint( const decDVector &point, float radius,
decDVector &nearestPoint, int &nearestType ){
	return false;
}

bool deBaseAINavigator::LineCollide( const decDVector &origin, const decVector &direction,
float &distance ){
	return false;
}

void deBaseAINavigator::FindPath( deNavigatorPath &path, const decDVector &start,
const decDVector &goal ){
	path.RemoveAll();
}

bool deBaseAINavigator::PathCollideRay( const deNavigatorPath &path, deCollider &collider,
int &hitAfterPoint, float &hitDistance ){
	return false;
}

bool deBaseAINavigator::PathCollideRay( const deNavigatorPath &path, deCollider &collider,
const decDVector &startPosition, int nextPoint, float maxDistance, int &hitAfterPoint,
float &hitDistance ){
	return false;
}

bool deBaseAINavigator::PathCollideShape( const deNavigatorPath &path, deCollider &collider,
deCollider &agent, int &hitAfterPoint, float &hitDistance ){
	return false;
}

bool deBaseAINavigator::PathCollideShape( const deNavigatorPath &path, deCollider &collider,
deCollider &agent, const decDVector &startPosition, int nextPoint, float maxDistance,
int &hitAfterPoint, float &hitDistance ){
	return false;
}
