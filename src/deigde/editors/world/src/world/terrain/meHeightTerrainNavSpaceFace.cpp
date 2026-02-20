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

#include "meHeightTerrainSector.h"
#include "meHeightTerrainNavSpaceFace.h"

#include <dragengine/common/exceptions.h>



// Class meHeightTerrainNavSpaceFace
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainNavSpaceFace::meHeightTerrainNavSpaceFace(){
}

meHeightTerrainNavSpaceFace::~meHeightTerrainNavSpaceFace(){
}



// Management
///////////////

void meHeightTerrainNavSpaceFace::SetType(meHeightTerrainNavSpaceType *type){
	pType = type;
}



void meHeightTerrainNavSpaceFace::OrderClockwise(int pointsPerRow){
	const int count = pNavPoints.GetCount();
	if(count < 2){
		return;
	}
	
	// sorting is done by calculating first the center of the face. since the face is
	// supposed to be convex the center has to lie inside the face. then the navigation
	// points are ordered using their angle towards the center. the 0 angle is along
	// the grid y axis which is pointing down if looked down from above. atan2 angles
	// then rotate counter-clockwise. thus to get clockwise ordering the angles have
	// to be sorted descending.
	const decVector2 center(pNavPoints.Inject(decVector2(), [&](const decVector2 &c, int p){
		return c + decVector2((float)(p % pointsPerRow), (float)(p / pointsPerRow));
	}) / (float)count);
	
	pNavPoints.Sort([&](int a, int b){
		const float angleA = atan2f((float)(a % pointsPerRow) - center.x, (float)(a / pointsPerRow) - center.y);
		const float angleB = atan2f((float)(b % pointsPerRow) - center.x, (float)(b / pointsPerRow) - center.y);
		return angleA >= angleB ? -1 : 1;
	});
}

bool meHeightTerrainNavSpaceFace::NavPointsMatch(const decTList<int> &navpoints) const{
	if(navpoints.GetCount() != pNavPoints.GetCount()){
		return false;
	}
	
	return navpoints.AllMatching([&](int navpoint){
		return pNavPoints.Has(navpoint);
	});
}

bool meHeightTerrainNavSpaceFace::HasNavPoint(int navpoint) const{
	return pNavPoints.Has(navpoint);
}

bool meHeightTerrainNavSpaceFace::HasNavEdge(int navpoint1, int navpoint2) const{
	if(pNavPoints.GetCount() < 2){
		return false;
	}
	
	const int index = pNavPoints.IndexOf(navpoint1);
	if(index == -1){
		return false;
	}
	
	const int count = pNavPoints.GetCount();
	return pNavPoints.GetAt((index + 1) % count) == navpoint2
		|| pNavPoints.GetAt((index - 1 + count) % count) == navpoint2;
}

bool meHeightTerrainNavSpaceFace::HasAllNavPointsIn(const decTList<int> &navpoints) const{
	if(pNavPoints.GetCount() > navpoints.GetCount()){
		return false;
	}
	
	return navpoints.AllMatching([&](int navpoint){
		return pNavPoints.Has(navpoint);
	});
}
