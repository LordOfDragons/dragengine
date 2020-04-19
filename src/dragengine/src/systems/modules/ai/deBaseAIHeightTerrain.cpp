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

#include "deBaseAIHeightTerrain.h"



// Class deBaseAIHeightTerrain
////////////////////////////////

// Constructres, destructors
//////////////////////////////

deBaseAIHeightTerrain::deBaseAIHeightTerrain(){
}

deBaseAIHeightTerrain::~deBaseAIHeightTerrain(){
}



// Notifications
//////////////////

void deBaseAIHeightTerrain::ParametersChanged(){
}

void deBaseAIHeightTerrain::SectorAdded( deHeightTerrainSector* ){
}

void deBaseAIHeightTerrain::SectorRemoved( int ){
}

void deBaseAIHeightTerrain::AllSectorsRemoved(){
}

void deBaseAIHeightTerrain::SectorChanged( int ){
}

void deBaseAIHeightTerrain::HeightChanged( const decPoint &, const decPoint &,
const decPoint &, const decPoint & ){
}



void deBaseAIHeightTerrain::NavSpaceAdded( int, deHeightTerrainNavSpace* ){
}

void deBaseAIHeightTerrain::NavSpaceRemoved( int, int ){
}

void deBaseAIHeightTerrain::AllNavSpacesRemoved( int ){
}

void deBaseAIHeightTerrain::NavSpaceLayerChanged( int, int ){
}

void deBaseAIHeightTerrain::NavSpaceTypeChanged( int, int ){
}

void deBaseAIHeightTerrain::NavSpaceSnappingChanged( int, int ){
}

void deBaseAIHeightTerrain::NavSpaceLayoutChanged( int, int ){
}
