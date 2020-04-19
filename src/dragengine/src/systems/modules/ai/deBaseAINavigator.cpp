/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
