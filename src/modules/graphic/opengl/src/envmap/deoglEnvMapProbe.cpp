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

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglEnvMapProbe.h"
#include "deoglREnvMapProbe.h"
#include "deoglEnvironmentMap.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>



// Class deoglEnvMapProbe
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglEnvMapProbe::deoglEnvMapProbe(deGraphicOpenGl &ogl, const deEnvMapProbe &envMapProbe) :
pOgl(ogl),
pEnvMapProbe(envMapProbe),

pDirtyOctreeNode(true),
pDirtyEnvMapProbe(true),
pDirtyMatrix(true),
pDirtyInfluenceShape(true),
pDirtyReflectionShape(true)
{
	try{
		pREnvMapProbe.TakeOver(new deoglREnvMapProbe(ogl.GetRenderThread()));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglEnvMapProbe::~deoglEnvMapProbe(){
	pCleanUp();
}



// Management
///////////////

void deoglEnvMapProbe::SyncToRender(){
	if(pDirtyEnvMapProbe){
		pREnvMapProbe->SetInfluenceBorderSize(pEnvMapProbe.GetInfluenceBorderSize());
		pREnvMapProbe->GetEnvironmentMap()->SetInfluencePriority(pEnvMapProbe.GetInfluencePriority());
		pREnvMapProbe->GetEnvironmentMap()->SetLayerMask(pEnvMapProbe.GetLayerMask());
		pDirtyEnvMapProbe = false;
	}
	
	if(pDirtyMatrix){
		pREnvMapProbe->GetEnvironmentMap()->SetPosition(pEnvMapProbe.GetPosition());
		pREnvMapProbe->SetMatrix(decDMatrix::CreateScale(decDVector(pEnvMapProbe.GetScaling()))
			* decDMatrix::CreateWorld( pEnvMapProbe.GetPosition(), pEnvMapProbe.GetOrientation() ) );
		pDirtyMatrix = false;
	}
	
	if(pDirtyOctreeNode){
		pREnvMapProbe->GetEnvironmentMap()->UpdateOctreePosition();
		pDirtyOctreeNode = false;
	}
	
	if(pDirtyInfluenceShape){
		pREnvMapProbe->UpdateInfluenceShape(pEnvMapProbe.GetShapeListInfluence());
		pDirtyInfluenceShape = false;
	}
	
	if(pDirtyReflectionShape){
		pREnvMapProbe->UpdateReflectionShape(pEnvMapProbe.GetShapeListReflectionMask(),
			pEnvMapProbe.GetShapeReflection());
		pDirtyReflectionShape = false;
	}
}



// Notifications
//////////////////

void deoglEnvMapProbe::PositionChanged(){
	pDirtyMatrix = true;
	pDirtyOctreeNode = true;
	pDirtyInfluenceShape = true;
	pDirtyReflectionShape = true;
}

void deoglEnvMapProbe::OrientationChanged(){
	pDirtyMatrix = true;
	pDirtyOctreeNode = true;
	pDirtyInfluenceShape = true;
	pDirtyReflectionShape = true;
}

void deoglEnvMapProbe::ScalingChanged(){
	pDirtyMatrix = true;
	pDirtyOctreeNode = true;
	pDirtyInfluenceShape = true;
	pDirtyReflectionShape = true;
}

void deoglEnvMapProbe::ShapeListInfluenceChanged(){
	pDirtyMatrix = true;
	pDirtyOctreeNode = true;
	pDirtyInfluenceShape = true;
}

void deoglEnvMapProbe::ShapeReflectionChanged(){
	pDirtyReflectionShape = true;
}

void deoglEnvMapProbe::InfluenceBorderSizeChanged(){
	pDirtyEnvMapProbe = true;
}

void deoglEnvMapProbe::InfluencePriorityChanged(){
	pDirtyEnvMapProbe = true;
}

void deoglEnvMapProbe::ImageChanged(){
}



void deoglEnvMapProbe::LayerMaskChanged(){
	pDirtyEnvMapProbe = true;
}



// Private functions
//////////////////////

void deoglEnvMapProbe::pCleanUp(){
}
