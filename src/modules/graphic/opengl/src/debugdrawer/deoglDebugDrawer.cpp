/* 
 * Drag[en]gine OpenGL Graphic Module
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

deoglDebugDrawer::deoglDebugDrawer( deGraphicOpenGl &ogl, const deDebugDrawer &debugDrawer ) :
pOgl( ogl ),
pDebugDrawer( debugDrawer ),

pRDebugDrawer( NULL ),

pDirtyDebugDrawer( true ),
pDirtyShapes( true )
{
	try{
		pRDebugDrawer = new deoglRDebugDrawer( ogl.GetRenderThread() );
		
	}catch( const deException & ){
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
	if( pDirtyDebugDrawer ){
		pRDebugDrawer->SetMatrix( decDMatrix::CreateWorld( pDebugDrawer.GetPosition(),
			pDebugDrawer.GetOrientation(), pDebugDrawer.GetScale() ) );
		pRDebugDrawer->SetVisible( pDebugDrawer.GetVisible() );
		pRDebugDrawer->SetXRay( pDebugDrawer.GetXRay() );
		pDirtyDebugDrawer = false;
	}
	
	if( pDirtyShapes ){
		pRDebugDrawer->UpdateShapes( pDebugDrawer );
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
	if( pRDebugDrawer ){
		pRDebugDrawer->FreeReference();
	}
}
