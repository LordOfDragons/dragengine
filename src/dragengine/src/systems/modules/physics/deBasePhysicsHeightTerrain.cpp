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

#include "deBasePhysicsHeightTerrain.h"



// Class deBasePhysicsHeightTerrain
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deBasePhysicsHeightTerrain::deBasePhysicsHeightTerrain(){
}

deBasePhysicsHeightTerrain::~deBasePhysicsHeightTerrain(){
}



// Management
///////////////

void deBasePhysicsHeightTerrain::ParametersChanged(){
}

void deBasePhysicsHeightTerrain::HeightChanged( const decPoint &, const decPoint &,
const decPoint &, const decPoint & ){
}



void deBasePhysicsHeightTerrain::CollisionFilterChanged(){
}



void deBasePhysicsHeightTerrain::SectorAdded( deHeightTerrainSector* ){
}

void deBasePhysicsHeightTerrain::SectorRemoved( int ){
}

void deBasePhysicsHeightTerrain::AllSectorsRemoved(){
}

void deBasePhysicsHeightTerrain::SectorChanged( int ){
}



void deBasePhysicsHeightTerrain::DecalAdded( int, deDecal* ){
}

void deBasePhysicsHeightTerrain::DecalRemoved( int, deDecal* ){
}

void deBasePhysicsHeightTerrain::AllDecalsRemoved( int ){
}



// Collision Detection
////////////////////////

void deBasePhysicsHeightTerrain::FindDecalsAt( const decDVector &, deDecalList & ){
}

void deBasePhysicsHeightTerrain::FindDecalsTouching( const decShape &, deDecalList & ){
}
