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
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>


// Class deoglGI
//////////////////

// Constructor, destructor
////////////////////////////

deoglGI::deoglGI( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pBVH( renderThread ),
pTraceRays( renderThread ),
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
	// - 10 vec4 blocks = 40 components = 160 bytes
	// - 2048 probe indices = 2048 components = 8192 bytes
	// - 2048 probe positions = 8192 components = 32768 bytes
	// - 512 rays = 2048 components = 8192 bytes
	// 
	// total 49312 bytes (UBO maximum at minimum 65535)
	// 
	// for different probe counts (probeCount*20):
	// - 256 => 5120 bytes
	// - 512 => 10240 bytes
	// - 1024 => 20480 bytes
	// - 2048 => 40960 bytes
	// - 4096 => 81920 bytes
	// 
	// 2048 is the maximum number of probes to update fitting into the minimum UBO size
	// of 65535 required to be supported. 4096 can be used with larger supported UBO size.
	const int maxProbeCount = pTraceRays.GetProbeCount();
	
	ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo.SetParameterCount( eupRayDirection + 1 );
	ubo.GetParameterAt( eupSampleImageScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupRaysPerProbe ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupGridProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( eupProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupGridProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupBVHInstanceRootNode ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupIrradianceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupDistanceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupIrradianceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupDistanceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupMaxProbeDistance ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupDepthSharpness ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupGridOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupGridCoordUnshift ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( eupFieldOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupBlendUpdateProbe ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupMoveMaxOffset ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( eupMoveMinDistToSurface ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( eupRayMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( eupMaterialMapsPerRow ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupMaterialMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( eupProbeIndex ).SetAll( deoglSPBParameter::evtInt, 4, 1, maxProbeCount / 4 ); // ivec4
	ubo.GetParameterAt( eupProbePosition ).SetAll( deoglSPBParameter::evtFloat, 4, 1, maxProbeCount ); // vec4
	ubo.GetParameterAt( eupRayDirection ).SetAll( deoglSPBParameter::evtFloat, 3, 1, GI_MAX_RAYS_PER_PROBE ); // vec3
	ubo.MapToStd140();
	ubo.SetBindingPoint( 0 );
}
