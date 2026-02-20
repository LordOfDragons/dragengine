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

void deBaseAIHeightTerrain::SectorAdded(deHeightTerrainSector*){
}

void deBaseAIHeightTerrain::SectorRemoved(int){
}

void deBaseAIHeightTerrain::AllSectorsRemoved(){
}

void deBaseAIHeightTerrain::SectorChanged(int){
}

void deBaseAIHeightTerrain::HeightChanged(const decPoint &, const decPoint &,
const decPoint &, const decPoint &){
}



void deBaseAIHeightTerrain::NavSpaceAdded(int, deHeightTerrainNavSpace*){
}

void deBaseAIHeightTerrain::NavSpaceRemoved(int, int){
}

void deBaseAIHeightTerrain::AllNavSpacesRemoved(int){
}

void deBaseAIHeightTerrain::NavSpaceLayerChanged(int, int){
}

void deBaseAIHeightTerrain::NavSpaceTypeChanged(int, int){
}

void deBaseAIHeightTerrain::NavSpaceSnappingChanged(int, int){
}

void deBaseAIHeightTerrain::NavSpaceLayoutChanged(int, int){
}
