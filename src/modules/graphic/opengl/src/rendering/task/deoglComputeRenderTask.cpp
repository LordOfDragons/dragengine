/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglComputeRenderTask.h"
#include "shared/deoglRenderTaskSharedPool.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../pipeline/deoglPipelineManager.h"
#include "../../world/deoglWorldCompute.h"

#include <dragengine/common/exceptions.h>



// Class deoglComputeRenderTask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComputeRenderTask::deoglComputeRenderTask( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pRenderVSStereo( false ),
pStepsResolved( nullptr ),
pStepsResolvedSize( 0 ),
pStepsResolvedCount( 0 )
{
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pUBOConfig.TakeOver( new deoglSPBlockUBO( renderThread ) );
	pUBOConfig->SetRowMajor( rowMajor );
	pUBOConfig->SetParameterCount( 7 );
	pUBOConfig->GetParameterAt( ecpElementGeometryCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpFilterCubeFace ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpRenderTaskFilter ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpRenderTaskFilterMask ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpFilterPipelineLists ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpPipelineType ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpPipelineModifier ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->MapToStd140();
	pUBOConfig->SetBindingPoint( 0 );
	
	pSSBOSteps.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOSteps->SetRowMajor( rowMajor );
	pSSBOSteps->SetParameterCount( 7 );
	pSSBOSteps->GetParameterAt( etpPipeline ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpTuc ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpVao ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpSpbInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpSpecialFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpSubInstanceCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->MapToStd140();
	pSSBOSteps->SetBindingPoint( 4 );
	
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOCounters->SetRowMajor( rowMajor );
	pSSBOCounters->SetParameterCount( 2 );
	pSSBOCounters->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // uvec3
	pSSBOCounters->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pSSBOCounters->SetElementCount( 1 );
	pSSBOCounters->MapToStd140();
	pSSBOCounters->SetBindingPoint( 3 );
	pSSBOCounters->SetBindingPointAtomic( 0 );
	
	Clear();
}

deoglComputeRenderTask::~deoglComputeRenderTask(){
	if( pStepsResolved ){
		delete [] pStepsResolved;
	}
}



// Management
///////////////

void deoglComputeRenderTask::Clear(){
	pSkinPipelineType = deoglSkinTexturePipelines::etGeometry;
	pSkinPipelineLists = 0;
	
	pSolid = false;
	pNoShadowNone = false;
	pNoNotReflected = false;
	pNoRendered = false;
	pOutline = false;
	pForceDoubleSided = false;
	
	pFilterXRay = false;
	pXRay = false;
	
	pFilterHoles = false;
	pWithHoles = false;
	
	pFilterDecal = false;
	pDecal = false;
	
	pFilterCubeFace = -1;
	
	pUseSpecialParamBlock = false;
	
	pStepsResolvedCount = 0;
}



void deoglComputeRenderTask::SetRenderVSStereo( bool renderVSStereo ){
	pRenderVSStereo = renderVSStereo;
}

void deoglComputeRenderTask::SetRenderParamBlock( deoglSPBlockUBO *paramBlock ){
	pRenderParamBlock = paramBlock;
}



void deoglComputeRenderTask::SetSkinPipelineLists( int mask ){
	pSkinPipelineLists = mask;
}

void deoglComputeRenderTask::EnableSkinPipelineList( deoglSkinTexturePipelinesList::ePipelineTypes list ){
	pSkinPipelineLists |= 1 << list;
}

void deoglComputeRenderTask::DisableSkinPipelineList( deoglSkinTexturePipelinesList::ePipelineTypes list ){
	pSkinPipelineLists &= ~( 1 << list );
}

void deoglComputeRenderTask::SetSkinPipelineType( deoglSkinTexturePipelines::eTypes type ){
	pSkinPipelineType = type;
}



void deoglComputeRenderTask::SetSolid( bool solid ){
	pSolid = solid;
}

void deoglComputeRenderTask::SetNoNotReflected( bool noNotReflected ){
	pNoNotReflected = noNotReflected;
}

void deoglComputeRenderTask::SetNoRendered( bool noRendered ){
	pNoRendered = noRendered;
}

void deoglComputeRenderTask::SetOutline( bool outline ){
	pOutline = outline;
}

void deoglComputeRenderTask::SetForceDoubleSided( bool forceDoubleSided ){
	pForceDoubleSided = forceDoubleSided;
}

void deoglComputeRenderTask::SetFilterXRay( bool filterXRay ){
	pFilterXRay = filterXRay;
}

void deoglComputeRenderTask::SetXRay( bool xray ){
	pXRay = xray;
}

void deoglComputeRenderTask::SetNoShadowNone( bool noShadowNone ){
	pNoShadowNone = noShadowNone;
}

void deoglComputeRenderTask::SetFilterHoles( bool filterHoles ){
	pFilterHoles = filterHoles;
}

void deoglComputeRenderTask::SetWithHoles( bool withHoles ){
	pWithHoles = withHoles;
}

void deoglComputeRenderTask::SetFilterDecal( bool filterDecal ){
	pFilterDecal = filterDecal;
}

void deoglComputeRenderTask::SetDecal( bool decal ){
	pDecal = decal;
}

void deoglComputeRenderTask::SetFilterCubeFace( int cubeFace ){
	pFilterCubeFace = cubeFace;
}

void deoglComputeRenderTask::SetUseSpecialParamBlock( bool use ){
	pUseSpecialParamBlock = use;
}



void deoglComputeRenderTask::PrepareBuffers( const deoglWorldCompute &worldCompute ){
	const int count = worldCompute.GetElementGeometryCount();
	DEASSERT_TRUE( count > 0 )
	
	pStepsResolvedCount = 0;
	
	pPrepareConfig( worldCompute );
	pClearCounters();
	
	if( count > pSSBOSteps->GetElementCount() ){
		pSSBOSteps->SetElementCount( count );
		pSSBOSteps->EnsureBuffer();
	}
}

void deoglComputeRenderTask::ReadBackSteps( const deoglWorldCompute &worldCompute ){
	pStepsResolvedCount = 0;
	
	if( worldCompute.GetElementGeometryCount() == 0 ){
		return;
	}
	
		decTimer timer;
	const sCounters &counters = *( sCounters* )pSSBOCounters->ReadBuffer( 1 );
	const int count = counters.counter;
	if( count == 0 ){
		return;
	}
	
	if( count > pStepsResolvedSize ){
		if( pStepsResolved ){
			delete [] pStepsResolved;
			pStepsResolved = nullptr;
			pStepsResolvedSize = 0;
		}
		
		pStepsResolved = new sStepResolved[ count ];
		pStepsResolvedSize = count;
	}
	
	const sStep * const steps = ( const sStep* )pSSBOSteps->ReadBuffer( count );
		pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: read %d in %dys", count, (int)(timer.GetElapsedTime()*1e6f));
	
	const deoglRenderTaskSharedPool &rtsPool = pRenderThread.GetRenderTaskSharedPool();
	const deoglPipelineManager &pipManager = pRenderThread.GetPipelineManager();
	int i;
	
	for( i=0; i<count; i++ ){
		sStepResolved &resolved = pStepsResolved[ i ];
		const sStep &step = steps[ i ];
		
		resolved.pipeline = pipManager.GetAt( step.pipeline );
		resolved.texture = &rtsPool.GetTextureAt( step.tuc );
		resolved.vao = &rtsPool.GetVAOAt( step.vao );
		resolved.instance = &rtsPool.GetInstanceAt( step.instance );
		resolved.spbInstance = step.spbInstance;
		resolved.specialFlags = step.specialFlags;
		resolved.subInstanceCount = step.subInstanceCount;
	}
	pStepsResolvedCount = count;
		pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: resolved %d in %dys", count, (int)(timer.GetElapsedTime()*1e6f));
}



// Private Functions
//////////////////////

void deoglComputeRenderTask::pPrepareConfig( const deoglWorldCompute &worldCompute ){
	int filter, mask;
	pRenderFilter( filter, mask );
	
	int pipelineModifier = 0;
	if( pForceDoubleSided ){
		pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
	}
	
	deoglSPBlockUBO &ubo = pUBOConfig;
	const deoglSPBMapBuffer mapped( ubo );
	
	ubo.SetParameterDataUInt( ecpElementGeometryCount, worldCompute.GetElementGeometryCount() );
	ubo.SetParameterDataUInt( ecpFilterCubeFace, pFilterCubeFace != -1 ? 0x100 | pFilterCubeFace : 0 );
	ubo.SetParameterDataUInt( ecpRenderTaskFilter, filter & mask );
	ubo.SetParameterDataUInt( ecpRenderTaskFilterMask, mask );
	ubo.SetParameterDataUInt( ecpFilterPipelineLists, pSkinPipelineLists );
	ubo.SetParameterDataUInt( ecpPipelineType, pSkinPipelineType );
	ubo.SetParameterDataUInt( ecpPipelineModifier, pipelineModifier );
}

void deoglComputeRenderTask::pRenderFilter( int &filter, int &mask ) const{
	filter = ertfRender;
	mask = ertfRender;
	
	if( pOutline ){
		filter |= ertfOutline;
		if( pSolid ){
			filter |= ertfOutlineSolid;
		}
		mask |= ertfOutline | ertfOutlineSolid;
		
	}else{
		if( pSolid ){
			filter |= ertfSolid;
		}
		mask |= ertfSolid;
	}
	
	if( pFilterXRay ){
		mask |= ertfXRay;
		if( pXRay ){
			filter |= ertfXRay;
		}
	}
	
	if( pNoNotReflected ){
		filter |= ertfReflected;
		mask |= ertfReflected;
	}
	
	if( pNoRendered ){
		mask |= ertfRendered;
	}
	
	if( pNoShadowNone ){
		mask |= ertfShadowNone;
	}
	
	if( pFilterHoles ){
		mask |= ertfHoles;
		if( pWithHoles ){
			filter |= ertfHoles;
		}
	}
	
	if( pFilterDecal ){
		mask |= ertfDecal;
		if( pDecal ){
			filter |= ertfDecal;
		}
	}
}

void deoglComputeRenderTask::pClearCounters(){
	deoglSPBlockSSBO &ssbo = pSSBOCounters;
	const deoglSPBMapBuffer mapped( ssbo );
	int i;
	
	for( i=0; i<1; i++ ){
		ssbo.SetParameterDataUVec3( 0, i, 0, 1, 1 ); // work group size (x=0)
		ssbo.SetParameterDataUInt( 1, i, 0 ); // count
	}
}
