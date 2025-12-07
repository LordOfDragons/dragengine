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

#include "igdeWCoordSysArrows.h"
#include "../../igdeShapeBuilder.h"

#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/common/exceptions.h>



// Class igdeWCoordSysArrows
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWCoordSysArrows::igdeWCoordSysArrows(){
	pScale.Set(1.0f, 1.0f, 1.0f);
	pColorAxisX.Set(1.0f, 0.0f, 0.0f);
	pColorAxisY.Set(0.0f, 1.0f, 0.0f);
	pColorAxisZ.Set(0.0f, 0.0f, 1.0f);
	pArrowLength = 0.1f;
	pArrowSize = 0.005f;
	pVisible = true;
	
	pDDSAxisX.SetEdgeColor(pColorAxisX);
	pDDSAxisX.SetEdgeColor(pColorAxisX);
	pDDSAxisY.SetEdgeColor(pColorAxisY);
	pDDSAxisY.SetEdgeColor(pColorAxisY);
	pDDSAxisZ.SetEdgeColor(pColorAxisZ);
	pDDSAxisZ.SetEdgeColor(pColorAxisZ);
	
	pRebuildArrows();
}

igdeWCoordSysArrows::~igdeWCoordSysArrows(){
	SetParentDebugDrawer(NULL);
}



// Management
///////////////

void igdeWCoordSysArrows::SetParentDebugDrawer(deDebugDrawer *debugDrawer){
	pDDSAxisX.SetParentDebugDrawer(debugDrawer);
	pDDSAxisY.SetParentDebugDrawer(debugDrawer);
	pDDSAxisZ.SetParentDebugDrawer(debugDrawer);
}



void igdeWCoordSysArrows::SetPosition(const decVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		
		pDDSAxisX.SetPosition(position);
		pDDSAxisY.SetPosition(position);
		pDDSAxisZ.SetPosition(position);
	}
}

void igdeWCoordSysArrows::SetOrientation(const decQuaternion &orientation){
	if(!orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		
		pDDSAxisX.SetOrientation(orientation);
		pDDSAxisY.SetOrientation(orientation);
		pDDSAxisZ.SetOrientation(orientation);
	}
}

void igdeWCoordSysArrows::SetScale(const decVector &scale){
	if(!scale.IsEqualTo(pScale)){
		pScale = scale;
		
		pDDSAxisX.SetScale(scale);
		pDDSAxisY.SetScale(scale);
		pDDSAxisZ.SetScale(scale);
	}
}



void igdeWCoordSysArrows::SetAxisXColor(const decColor &color){
	if(!color.IsEqualTo(pColorAxisX)){
		pColorAxisX = color;
		
		pDDSAxisX.SetEdgeColor(pColorAxisX);
		pDDSAxisX.SetEdgeColor(pColorAxisX);
	}
}

void igdeWCoordSysArrows::SetAxisYColor(const decColor &color){
	if(!color.IsEqualTo(pColorAxisY)){
		pColorAxisY = color;
		
		pDDSAxisY.SetEdgeColor(pColorAxisY);
		pDDSAxisY.SetEdgeColor(pColorAxisY);
	}
}

void igdeWCoordSysArrows::SetAxisZColor(const decColor &color){
	if(!color.IsEqualTo(pColorAxisZ)){
		pColorAxisZ = color;
		
		pDDSAxisZ.SetEdgeColor(pColorAxisZ);
		pDDSAxisZ.SetEdgeColor(pColorAxisZ);
	}
}

void igdeWCoordSysArrows::SetArrowLength(float length){
	if(length < 0.001f){
		length = 0.001f;
	}
	
	if(fabsf(length - pArrowLength) > 1e-5f){
		pArrowLength = length;
		pRebuildArrows();
	}
}

void igdeWCoordSysArrows::SetArrowSize(float size){
	if(size < 0.001f){
		size = 0.001f;
	}
	
	if(fabsf(size - pArrowSize) > 1e-5f){
		pArrowSize = size;
		pRebuildArrows();
	}
}

void igdeWCoordSysArrows::SetVisible(bool visible){
	if(visible != pVisible){
		pVisible = visible;
		
		pDDSAxisX.SetVisible(visible);
		pDDSAxisY.SetVisible(visible);
		pDDSAxisZ.SetVisible(visible);
	}
}



// Private Functions
//////////////////////

void igdeWCoordSysArrows::pRebuildArrows(){
	const float headLength = pArrowSize * 3.0f;
	const float tailSize = pArrowSize * 0.1f;
	igdeShapeBuilder builder;
	
	builder.CreateArrow(pDDSAxisX, decVector(), decVector(pArrowLength, 0.0f, 0.0f), pArrowSize, headLength, tailSize);
	builder.CreateArrow(pDDSAxisY, decVector(), decVector(0.0f, pArrowLength, 0.0f), pArrowSize, headLength, tailSize);
	builder.CreateArrow(pDDSAxisZ, decVector(), decVector(0.0f, 0.0f, pArrowLength), pArrowSize, headLength, tailSize);
}
