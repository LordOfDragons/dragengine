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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeWAngleRange.h"

#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/common/exceptions.h>



// Class igdeWDDSAngleRange
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWAngleRange::igdeWAngleRange(){
	pColorDiskLine.Set(1.0f, 0.0f, 0.0f, 1.0f);
	pColorDiskArea.Set(1.0f, 0.0f, 0.0f, 0.2f);
	pLower = 0.0f;
	pUpper = 90.0f;
	pRadius = 0.5f;
	pVisible = true;
	
	pDDSDisk.SetEdgeColor(pColorDiskLine);
	pDDSDisk.SetFillColor(pColorDiskArea);
	
	pRebuildShape();
}

igdeWAngleRange::~igdeWAngleRange(){
	SetParentDebugDrawer(nullptr);
}



// Management
///////////////

void igdeWAngleRange::SetParentDebugDrawer(deDebugDrawer *debugDrawer){
	pDDSDisk.SetParentDebugDrawer(debugDrawer);
}



void igdeWAngleRange::SetPosition(const decVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		pDDSDisk.SetPosition(position);
	}
}

void igdeWAngleRange::SetOrientation(const decQuaternion &orientation){
	if(!orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		pDDSDisk.SetOrientation(orientation);
	}
}



void igdeWAngleRange::SetDiskLineColor(const decColor &color){
	if(!color.IsEqualTo(pColorDiskLine)){
		pColorDiskLine = color;
		pDDSDisk.SetEdgeColor(pColorDiskLine);
	}
}

void igdeWAngleRange::SetDiskAreaColor(const decColor &color){
	if(!color.IsEqualTo(pColorDiskArea)){
		pColorDiskArea = color;
		pDDSDisk.SetFillColor(pColorDiskArea);
	}
}



void igdeWAngleRange::SetRange(float lower, float upper){
	if(upper < lower){
		upper = lower;
	}
	if(upper - lower > 360.0f){
		upper = lower + 360.0f;
	}
	
	if(fabsf(lower - pLower) > 1e-5f || fabsf(upper - pUpper) > 1e-5f){
		pLower = lower;
		pUpper = upper;
		pRebuildShape();
	}
}

void igdeWAngleRange::SetRadius(float radius){
	if(radius < 0.001f){
		radius = 0.001f;
	}
	
	if(fabsf(radius - pRadius) > 1e-5f){
		pRadius = radius;
		pRebuildShape();
	}
}



void igdeWAngleRange::SetVisible(bool visible){
	if(visible != pVisible){
		pVisible = visible;
		pDDSDisk.SetVisible(visible);
	}
}



// Private Functions
//////////////////////

void igdeWAngleRange::pRebuildShape(){
	deDebugDrawerShapeFace *ddFace = nullptr;
	float meterPerPoint = 0.1f;
	float angle, angleDiff;
	float circumfence;
	int p, pointCount;
	bool useDisk;
	
	// determine if a disk or circle has to be used.
	angleDiff = pUpper - pLower;
	
	useDisk = angleDiff < 359.9999f;
	
	// determine the number of points to use for the disk
	circumfence = angleDiff * DEG2RAD;
	
	pointCount = (int)ceilf(circumfence / meterPerPoint);
	
	// create the disk
	pDDSDisk.RemoveAllFaces();
	
	try{
		ddFace = new deDebugDrawerShapeFace;
		if(!ddFace) DETHROW(deeOutOfMemory);
		
		ddFace->SetNormal(decVector(0.0f, 0.0f, 1.0f));
		
		if(useDisk){
			ddFace->AddVertex(decVector());
		}
		
		for(p=0; p<=pointCount; p++){
			angle = (pLower + angleDiff * ((float)p / (float)pointCount)) * DEG2RAD;
			
			ddFace->AddVertex(decVector(sinf(angle) * pRadius, cosf(angle) * pRadius, 0.0f));
		}
		
		if(useDisk){
			ddFace->AddVertex(decVector());
		}
		
		pDDSDisk.AddFace(ddFace);
		ddFace = nullptr;
		
	}catch(const deException &){
		if(ddFace){
			delete ddFace;
		}
		throw;
	}
}
