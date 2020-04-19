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

#include "deBasePhysicsComponent.h"

// Class deBasePhysicsComponent
/////////////////////////////////

// Constructres, destructors
//////////////////////////////

deBasePhysicsComponent::deBasePhysicsComponent(){
}

deBasePhysicsComponent::~deBasePhysicsComponent(){
}



// Notifications
//////////////////

void deBasePhysicsComponent::PositionChanged(){
}

void deBasePhysicsComponent::ScalingChanged(){
}

void deBasePhysicsComponent::OrientationChanged(){
}

void deBasePhysicsComponent::ModelChanged(){
}

void deBasePhysicsComponent::RigChanged(){
}

void deBasePhysicsComponent::VisibilityChanged(){
}

void deBasePhysicsComponent::ExtendsChanged(){
}

void deBasePhysicsComponent::MeshDirty(){
}

void deBasePhysicsComponent::ParametersChanged(){
}



void deBasePhysicsComponent::DecalAdded( deDecal *decal ){
}

void deBasePhysicsComponent::DecalRemoved( deDecal *decal ){
}

void deBasePhysicsComponent::AllDecalsRemoved(){
}



// Collision Detection
////////////////////////

void deBasePhysicsComponent::FindDecalsAt( const decVector &point, deDecalList &list ){
}

void deBasePhysicsComponent::FindDecalsTouching( decShape *shape, deDecalList &list ){
}
