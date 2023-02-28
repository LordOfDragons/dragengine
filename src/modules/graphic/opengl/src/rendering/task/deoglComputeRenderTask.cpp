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



// Class deoglComputeRenderTask::cGuard
/////////////////////////////////////////

deoglComputeRenderTask::cGuard::cGuard( deoglComputeRenderTask &renderTask,
	const deoglWorldCompute &worldCompute, int passCount ) :
pRenderTask( renderTask ),
pWorldCompute( worldCompute )
{
	renderTask.BeginPrepare( passCount );
}

deoglComputeRenderTask::cGuard::~cGuard(){
	pRenderTask.EndPrepare( pWorldCompute );
}



// Class deoglComputeRenderTask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComputeRenderTask::deoglComputeRenderTask( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pPassCount( 0 ),
pPass( -1 ),
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
	
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOCounters->SetRowMajor( rowMajor );
	pSSBOCounters->SetParameterCount( 2 );
	pSSBOCounters->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // uvec3
	pSSBOCounters->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pSSBOCounters->SetElementCount( 1 );
	pSSBOCounters->MapToStd140();
	
	pSSBOStepsReadBack.TakeOver( new deoglSPBlockReadBackSSBO( pSSBOSteps ) );
	
	pSSBOCountersReadBack.TakeOver( new deoglSPBlockReadBackSSBO( pSSBOCounters ) );
	pSSBOCountersReadBack->SetElementCount( 1 );
	
	Clear();
}

deoglComputeRenderTask::~deoglComputeRenderTask(){
	if( pStepsResolved ){
		delete [] pStepsResolved;
	}
}



// Management
///////////////

void deoglComputeRenderTask::BeginPrepare( int passCount ){
	DEASSERT_TRUE( pPass == -1 )
	DEASSERT_TRUE( passCount > 0 )
	DEASSERT_TRUE( passCount < 8 )  // hard limit in shader
	
	if( passCount > pUBOConfig->GetElementCount() ){
		pUBOConfig->SetElementCount( passCount );
	}
	
	pUBOConfig->MapBuffer();
	
	pPassCount = passCount;
	pPass = 0;
}

void deoglComputeRenderTask::Clear(){
	pSkinPipelineType = deoglSkinTexturePipelines::etGeometry;
	pSkinPipelineLists = 0;
	pSkinPipelineModifier = 0;
	
	pSolid = false;
	pFilterSolid = true;
	
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

void deoglComputeRenderTask::EndPass( const deoglWorldCompute &worldCompute ){
	DEASSERT_TRUE( pPass >= 0 )
	DEASSERT_TRUE( pPass < pPassCount )
	
	int filter, mask;
	pRenderFilter( filter, mask );
	
	int pipelineModifier = pSkinPipelineModifier;
	if( pForceDoubleSided ){
		pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
	}
	
	pUBOConfig->SetParameterDataUInt( ecpElementGeometryCount, pPass, worldCompute.GetElementGeometryCount() );
	pUBOConfig->SetParameterDataUInt( ecpFilterCubeFace, pPass, pFilterCubeFace != -1 ? 0x100 | pFilterCubeFace : 0 );
	pUBOConfig->SetParameterDataUInt( ecpRenderTaskFilter, pPass, filter & mask );
	pUBOConfig->SetParameterDataUInt( ecpRenderTaskFilterMask, pPass, mask );
	pUBOConfig->SetParameterDataUInt( ecpFilterPipelineLists, pPass, pSkinPipelineLists );
	pUBOConfig->SetParameterDataUInt( ecpPipelineType, pPass, pSkinPipelineType );
	pUBOConfig->SetParameterDataUInt( ecpPipelineModifier, pPass, pipelineModifier );
	
	pPass++;
}

void deoglComputeRenderTask::EndPrepare( const deoglWorldCompute &worldCompute ){
	const int count = worldCompute.GetElementGeometryCount();
	DEASSERT_TRUE( pPass != -1 )
	DEASSERT_TRUE( count > 0 )
	
	pPass = -1;
	pStepsResolvedCount = 0;
	
	pUBOConfig->UnmapBuffer();
	pClearCounters();
	
	if( count > pSSBOSteps->GetElementCount() ){
		pSSBOSteps->SetElementCount( count );
		pSSBOSteps->EnsureBuffer();
	}
}

void deoglComputeRenderTask::BeginReadBackCounters(){
	DEASSERT_TRUE( pPass == -1 )
	pReadBackStepCount = 0;
	pSSBOCountersReadBack->TransferFrom( pSSBOCounters, 1 );
}

void deoglComputeRenderTask::BeginReadBackSteps(){
	DEASSERT_TRUE( pPass == -1 )
	
		decTimer timer;
	const deoglSPBMapBuffer mapped( pSSBOCountersReadBack, 0, 1 );
	const sCounters &counters = *( sCounters* )pSSBOCountersReadBack->GetMappedBuffer();
	pReadBackStepCount = counters.counter;
	if( pReadBackStepCount == 0 ){
		return;
	}
	
	if( pReadBackStepCount > pSSBOStepsReadBack->GetElementCount() ){
		pSSBOStepsReadBack->SetElementCount( pReadBackStepCount );
	}
	
	pSSBOStepsReadBack->TransferFrom( pSSBOSteps, pReadBackStepCount );
		pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.BeginReadBackSteps: read %dys. %d steps",
			(int)(timer.GetElapsedTime()*1e6f), pReadBackStepCount);
}

void deoglComputeRenderTask::ReadBackSteps(){
	DEASSERT_TRUE( pPass == -1 )
	
	pStepsResolvedCount = 0;
	
	if( pReadBackStepCount == 0 ){
		return;
	}
	
		decTimer timer;
	if( pReadBackStepCount > pStepsResolvedSize ){
		if( pStepsResolved ){
			delete [] pStepsResolved;
			pStepsResolved = nullptr;
			pStepsResolvedSize = 0;
		}
		
		pStepsResolved = new sStepResolved[ pReadBackStepCount ];
		pStepsResolvedSize = pReadBackStepCount;
	}
	
	const deoglSPBMapBuffer mapped( pSSBOStepsReadBack, 0, pReadBackStepCount );
	const sStep * const steps = ( const sStep* )pSSBOStepsReadBack->GetMappedBuffer();
		pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: read %d in %dys",
			pReadBackStepCount, (int)(timer.GetElapsedTime()*1e6f));
	
	const deoglRenderTaskSharedPool &rtsPool = pRenderThread.GetRenderTaskSharedPool();
	const deoglPipelineManager &pipManager = pRenderThread.GetPipelineManager();
	int i;
	
	for( i=0; i<pReadBackStepCount; i++ ){
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
	pStepsResolvedCount = pReadBackStepCount;
		pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: resolved %d in %dys",
			pReadBackStepCount, (int)(timer.GetElapsedTime()*1e6f));
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

void deoglComputeRenderTask::SetSkinPipelineModifier( int modifier ){
	pSkinPipelineModifier = modifier;
}



void deoglComputeRenderTask::SetSolid( bool solid ){
	pSolid = solid;
}

void deoglComputeRenderTask::SetFilterSolid( bool filterSolid ){
	pFilterSolid = filterSolid;
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



// Private Functions
//////////////////////

void deoglComputeRenderTask::pRenderFilter( int &filter, int &mask ) const{
	mask = ertfRender;
	filter = ertfRender;
	
	mask |= ertfOutline;
	if( pOutline ){
		filter |= ertfOutline;
	}
	
	if( pFilterSolid ){
		if( pOutline ){
			mask |= ertfOutlineSolid;
			if( pSolid ){
				filter |= ertfOutlineSolid;
			}
			
		}else{
			mask |= ertfSolid;
			if( pSolid ){
				filter |= ertfSolid;
			}
		}
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
