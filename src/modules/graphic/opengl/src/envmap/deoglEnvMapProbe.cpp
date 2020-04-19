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

deoglEnvMapProbe::deoglEnvMapProbe( deGraphicOpenGl &ogl, const deEnvMapProbe &envMapProbe ) :
pOgl( ogl ),
pEnvMapProbe( envMapProbe ),
pREnvMapProbe( NULL ),

pDirtyOctreeNode( true ),
pDirtyEnvMapProbe( true ),
pDirtyMatrix( true ),
pDirtyInfluenceShape( true ),
pDirtyReflectionShape( true )
{
	try{
		pREnvMapProbe = new deoglREnvMapProbe( ogl.GetRenderThread() );
		
	}catch( const deException & ){
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
	if( pDirtyEnvMapProbe ){
		pREnvMapProbe->SetInfluenceBorderSize( pEnvMapProbe.GetInfluenceBorderSize() );
		pREnvMapProbe->GetEnvironmentMap()->SetInfluencePriority( pEnvMapProbe.GetInfluencePriority() );
		pREnvMapProbe->GetEnvironmentMap()->SetLayerMask( pEnvMapProbe.GetLayerMask() );
		pDirtyEnvMapProbe = false;
	}
	
	if( pDirtyMatrix ){
		pREnvMapProbe->GetEnvironmentMap()->SetPosition( pEnvMapProbe.GetPosition() );
		pREnvMapProbe->SetMatrix( decDMatrix::CreateScale( decDVector( pEnvMapProbe.GetScaling() ) )
			* decDMatrix::CreateWorld( pEnvMapProbe.GetPosition(), pEnvMapProbe.GetOrientation() ) );
		pDirtyMatrix = false;
	}
	
	if( pDirtyOctreeNode ){
		pREnvMapProbe->GetEnvironmentMap()->UpdateOctreePosition();
		pDirtyOctreeNode = false;
	}
	
	if( pDirtyInfluenceShape ){
		pREnvMapProbe->UpdateInfluenceShape( pEnvMapProbe.GetShapeListInfluence() );
		pDirtyInfluenceShape = false;
	}
	
	if( pDirtyReflectionShape ){
		pREnvMapProbe->UpdateReflectionShape( pEnvMapProbe.GetShapeListReflectionMask(),
			pEnvMapProbe.GetShapeReflection() );
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
	if( pREnvMapProbe ){
		pREnvMapProbe->FreeReference();
	}
}
