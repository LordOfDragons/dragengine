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

#include "deoglDebugDrawer.h"
#include "deoglRDebugDrawer.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>



// Class deoglDebugDrawer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugDrawer::deoglDebugDrawer(deGraphicOpenGl &ogl, const deDebugDrawer &debugDrawer) :
pOgl(ogl),
pDebugDrawer(debugDrawer),

pDirtyDebugDrawer(true),
pDirtyShapes(true)
{
	try{
		pRDebugDrawer = deoglRDebugDrawer::Ref::New(ogl.GetRenderThread());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDebugDrawer::~deoglDebugDrawer(){
	pCleanUp();
}



// Management
///////////////

void deoglDebugDrawer::SyncToRender(){
	if(pDirtyDebugDrawer){
		pRDebugDrawer->SetMatrix(decDMatrix::CreateWorld(pDebugDrawer.GetPosition(),
			pDebugDrawer.GetOrientation(), pDebugDrawer.GetScale()));
		pRDebugDrawer->SetVisible(pDebugDrawer.GetVisible());
		pRDebugDrawer->SetXRay(pDebugDrawer.GetXRay());
		pDirtyDebugDrawer = false;
	}
	
	if(pDirtyShapes){
		pRDebugDrawer->UpdateShapes(pDebugDrawer);
		pDirtyShapes = false;
	}
}



// Notifications
//////////////////

void deoglDebugDrawer::PositionChanged(){
	pDirtyDebugDrawer = true;
}

void deoglDebugDrawer::OrientationChanged(){
	pDirtyDebugDrawer = true;
}

void deoglDebugDrawer::ScalingChanged(){
	pDirtyDebugDrawer = true;
}

void deoglDebugDrawer::VisibleChanged(){
	pDirtyDebugDrawer = true;
}

void deoglDebugDrawer::XRayChanged(){
	pDirtyDebugDrawer = true;
}



void deoglDebugDrawer::ShapeColorChanged(){
	pDirtyShapes = true;
}

void deoglDebugDrawer::ShapeGeometryChanged(){
	pDirtyShapes = true;
}

void deoglDebugDrawer::ShapeContentChanged(){
	pDirtyShapes = true;
}

void deoglDebugDrawer::ShapeLayoutChanged(){
	pDirtyShapes = true;
}



// Private Functions
//////////////////////

void deoglDebugDrawer::pCleanUp(){
}
