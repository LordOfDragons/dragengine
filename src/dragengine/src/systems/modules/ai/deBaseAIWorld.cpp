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

#include "deBaseAIWorld.h"



// Class deBaseAIWorld
////////////////////////

// Constructors, destructors
//////////////////////////////

deBaseAIWorld::deBaseAIWorld(){
}

deBaseAIWorld::~deBaseAIWorld(){
}



// Notifications
//////////////////

void deBaseAIWorld::Update( float elapsed ){
}



void deBaseAIWorld::NavigationSpaceAdded( deNavigationSpace *navspace ){
}

void deBaseAIWorld::NavigationSpaceRemoved( deNavigationSpace *navspace ){
}

void deBaseAIWorld::AllNavigationSpacesRemoved(){
}



void deBaseAIWorld::NavigationBlockerAdded( deNavigationBlocker *blocker ){
}

void deBaseAIWorld::NavigationBlockerRemoved( deNavigationBlocker *blocker ){
}

void deBaseAIWorld::AllNavigationBlockersRemoved(){
}



void deBaseAIWorld::NavigatorAdded( deNavigator *navigator ){
}

void deBaseAIWorld::NavigatorRemoved( deNavigator *navigator ){
}

void deBaseAIWorld::AllNavigatorsRemoved(){
}



void deBaseAIWorld::HeightTerrainChanged(){
}
