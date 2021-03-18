/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglGI.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>


// Class deoglGI
//////////////////

// Constructor, destructor
////////////////////////////

deoglGI::deoglGI( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pBVH( renderThread ),
pRays( renderThread ),
pMaterials( renderThread )
{
	try{
		pCreateUBO();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGI::~deoglGI(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglGI::pCleanUp(){
}

void deoglGI::pCreateUBO(){
	pUBO.TakeOver( new deoglSPBlockUBO( pRenderThread ) );
	deoglSPBlockUBO &ubo = GetUBO();
	
	// memory consumption:
	// 
	// - 5 * 16 = 80 (fixed size 4-component blocks)
	// - ( maxProbeCount / 4 ) * 16 (index array)
	// - maxProbeCount * 16 (probe position array)
	// - maxRaysPerProbe * 16 (ray direction array)
	// 
	// maxProbeCount is guaranteed to be multiple of 4 so index array can be reduced to
	// - maxProbeCount * 4
	// 
	// maxRaysPerProbe is guaranteed to be at least 1
	// 
	// size = 80 + 16 * ( maxRaysPerProbe + maxProbeCount * 1.25 )
	// 
	// - base size: 80
	// - rays:      1'024 (64), 4096 (256)
	// - probes:    5'120 (256), 20'480 (1024), 40'960 (2048), 81'920 (4096)
	const int maxProbeCount = pRays.GetProbeCount();
	const int raysPerProbe = pRays.GetRaysPerProbe();
	
	ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo.SetParameterCount( eupRayDirection + 1 );
	ubo.GetParameterAt( eupSampleImageScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupRaysPerProbe ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupGridProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( eupProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupGridProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupBVHInstanceRootNode ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupOcclusionMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupDistanceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupOcclusionMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupDistanceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupMaxProbeDistance ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupDepthSharpness ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupGridOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupGridCoordUnshift ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( eupFieldOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupBlendUpdateProbe ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupMaterialMapsPerRow ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupMaterialMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupMoveMaxOffset ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupMoveMinDistToSurface ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupProbeIndex ).SetAll( deoglSPBParameter::evtInt, 4, 1, maxProbeCount / 4 ); // ivec4
	ubo.GetParameterAt( eupProbePosition ).SetAll( deoglSPBParameter::evtFloat, 4, 1, maxProbeCount ); // vec4
	ubo.GetParameterAt( eupRayDirection ).SetAll( deoglSPBParameter::evtFloat, 3, 1, raysPerProbe ); // vec3
	ubo.MapToStd140();
	ubo.SetBindingPoint( 0 );
}
