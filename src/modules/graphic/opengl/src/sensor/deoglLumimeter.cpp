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
#include <stdlib.h>
#include <string.h>

#include "deoglLumimeter.h"
#include "deoglRLumimeter.h"

#include "../deGraphicOpenGl.h"

#include <dragengine/resources/sensor/deLumimeter.h>
#include <dragengine/common/exceptions.h>



// Class deoglLumimeter
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglLumimeter::deoglLumimeter(deGraphicOpenGl &ogl, const deLumimeter &lumimeter) :
pOgl(ogl),
pLumimeter(lumimeter),

pLuminance(0.0f),

pDirtyLumimeter(true),
pDirtyOctree(true)
{
	try{
		pRLumimeter = deoglRLumimeter::Ref::New(ogl.GetRenderThread());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglLumimeter::~deoglLumimeter(){
	pCleanUp();
}



// Management
///////////////

void deoglLumimeter::SyncToRender(){
	if(pDirtyLumimeter){
		pRLumimeter->SetPosition(pLumimeter.GetPosition());
		pDirtyLumimeter = false;
	}
	
	if(pDirtyOctree){
		pRLumimeter->UpdateOctreeNode();
		pDirtyOctree = false;
	}
	
	// TODO somehow sync measurements...
}



// Notifications
//////////////////

void deoglLumimeter::PositionChanged(){
	pDirtyLumimeter = true;
	pDirtyOctree = true;
}

void deoglLumimeter::DirectionChanged(){
	pDirtyLumimeter = true;
	pDirtyOctree = true;
}

void deoglLumimeter::ConeParameterChanged(){
	pDirtyLumimeter = true;
	pDirtyOctree = true;
}



// Measuring
//////////////

float deoglLumimeter::MeasureLuminance(){
	return pLuminance;
}

decColor deoglLumimeter::MeasureColor(){
	return pColor;
}



// Private Functions
//////////////////////

void deoglLumimeter::pCleanUp(){
}
