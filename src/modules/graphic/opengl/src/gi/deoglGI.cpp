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
pBVHShared( renderThread ),
pTraceRays( renderThread ),
pMaterials( renderThread )
{
	try{
		pCreateUBOParameter();
		pCreateUBOProbeIndex();
		pCreateUBOProbePosition();
		pCreateUBORayDirection();
		
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

deoglSPBlockUBO &deoglGI::NextUBOParameter(){
	pUBOParameter = ( deoglSPBlockUBO* )pUBOParameterSingleUse->Next();
	return pUBOParameter;
}

deoglSPBlockUBO &deoglGI::NextUBOProbeIndex(){
	pUBOProbeIndex = ( deoglSPBlockUBO* )pUBOProbeIndexSingleUse->Next();
	return pUBOProbeIndex;
}

deoglSPBlockUBO &deoglGI::NextUBOProbePosition(){
	pUBOProbePosition = ( deoglSPBlockUBO* )pUBOProbePositionSingleUse->Next();
	return pUBOProbePosition;
}

deoglSPBlockUBO &deoglGI::NextUBORayDirection(){
	pUBORayDirection = ( deoglSPBlockUBO* )pUBORayDirectionSingleUse->Next();
	return pUBORayDirection;
}



// Private Functions
//////////////////////

void deoglGI::pCleanUp(){
}

void deoglGI::pCreateUBOParameter(){
	const deoglSPBlockUBO::Ref ubo( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( pRenderThread ) ) );
	
	// memory consumption:
	// - 10 vec4 blocks = 40 components = 160 bytes
	ubo->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo->SetParameterCount( eupBVHOffset + 1 );
	ubo->GetParameterAt( eupSampleImageScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo->GetParameterAt( eupProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupRaysPerProbe ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupGridProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo->GetParameterAt( eupProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupGridProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo->GetParameterAt( eupBVHInstanceRootNode ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupIrradianceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo->GetParameterAt( eupDistanceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo->GetParameterAt( eupIrradianceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupDistanceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupMaxProbeDistance ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupDepthSharpness ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupGridOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo->GetParameterAt( eupGridCoordUnshift ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo->GetParameterAt( eupFieldSize ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo->GetParameterAt( eupBlendUpdateProbe ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupMoveMaxOffset ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo->GetParameterAt( eupMoveMinDistToSurface ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupRayMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo->GetParameterAt( eupMaterialMapsPerRow ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupMaterialMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupIrradianceGamma ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupInvIrradianceGamma ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupSelfShadowBias ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo->GetParameterAt( eupCascade ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupDetectionBox ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo->GetParameterAt( euppRayCacheProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo->GetParameterAt( eupBVHOffset ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo->MapToStd140();
	ubo->SetBindingPoint( 1 );
	
	pUBOParameterSingleUse.TakeOver( new deoglSPBSingleUse( pRenderThread, ubo ) );
}

void deoglGI::pCreateUBOProbeIndex(){
	const deoglSPBlockUBO::Ref ubo( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( pRenderThread ) ) );
	
	// memory consumption (UBO maximum at minimum 65536):
	// - 4096 probe indices = 4096 components = 16384 bytes
	ubo->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo->SetParameterCount( 1 );
	ubo->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, GI_MAX_PROBE_COUNT / 4 ); // ivec4
	ubo->MapToStd140();
	ubo->SetBindingPoint( 2 );
	
	pUBOProbeIndexSingleUse.TakeOver( new deoglSPBSingleUse( pRenderThread, ubo ) );
}

void deoglGI::pCreateUBOProbePosition(){
	const deoglSPBlockUBO::Ref ubo( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( pRenderThread ) ) );
	
	// memory consumption (UBO maximum at minimum 65536):
	// - 4096 probe positions = 16384 components = 65536 bytes
	ubo->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo->SetParameterCount( 1 );
	ubo->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, GI_MAX_PROBE_COUNT ); // vec4
	ubo->MapToStd140();
	ubo->SetBindingPoint( 3 );
	
	pUBOProbePositionSingleUse.TakeOver( new deoglSPBSingleUse( pRenderThread, ubo ) );
}

void deoglGI::pCreateUBORayDirection(){
	const deoglSPBlockUBO::Ref ubo( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( pRenderThread ) ) );
	
	// memory consumption (UBO maximum at minimum 65536):
	// - 512 rays = 2048 components = 8192 bytes
	ubo->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo->SetParameterCount( 1  );
	ubo->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, GI_MAX_RAYS_PER_PROBE ); // vec3
	ubo->MapToStd140();
	ubo->SetBindingPoint( 4 );
	
	pUBORayDirectionSingleUse.TakeOver( new deoglSPBSingleUse( pRenderThread, ubo ) );
}
