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
#include "shared/deoglRenderTaskSharedTexture.h"
#include "shared/deoglRenderTaskSharedVAO.h"
#include "shared/deoglRenderTaskSharedInstance.h"
#include "../deoglRenderCompute.h"
#include "../deoglRenderGeometry.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../shaders/paramblock/deoglSPBMapBufferRead.h"
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
pState( esInitial ),
pPassCount( 0 ),
pPass( -1 ),
pUseSPBInstanceFlags( false ),
pRenderVSStereo( false ),
pPipelineDoubleSided( nullptr ),
pPipelineSingleSided( nullptr ),
pSteps( nullptr ),
pStepCount( 0 ),
pStepSize( 0 ),
pSkipSubInstanceGroups( false )
{
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pUBOConfig.TakeOver( new deoglSPBlockUBO( renderThread ) );
	pUBOConfig->SetRowMajor( rowMajor );
	pUBOConfig->SetParameterCount( 9 );
	pUBOConfig->GetParameterAt( ecpElementGeometryCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpFilterCubeFace ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpRenderTaskFilter ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpRenderTaskFilterMask ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpFilterPipelineLists ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpPipelineType ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpPipelineModifier ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpPipelineDoubleSided ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->GetParameterAt( ecpPipelineSingleSided ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pUBOConfig->MapToStd140();
	
	pSSBOSteps.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etRead ) );
	pSSBOSteps->SetRowMajor( rowMajor );
	pSSBOSteps->SetParameterCount( 8 );
	pSSBOSteps->GetParameterAt( etpPass ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpPipeline ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpTuc ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpVao ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpSpbInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpSpecialFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->GetParameterAt( etpSubInstanceCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOSteps->MapToStd140();
	pSSBOSteps->EnsureBuffer();
	
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( renderThread.GetRenderers().
		GetCompute().GetSSBOCounters(), deoglSPBlockSSBO::etRead ) );
	pSSBOCounters->SetElementCount( 1 );
	
	Clear();
}

deoglComputeRenderTask::~deoglComputeRenderTask(){
	if( pSteps ){
		delete [] pSteps;
	}
}



// Management
///////////////

#define DO_READ_BACK_TIMINGS
// #define DO_STATE_DEBUG

void deoglComputeRenderTask::BeginPrepare( int passCount ){
	DEASSERT_TRUE( pPass == -1 )
	DEASSERT_TRUE( passCount > 0 )
	DEASSERT_TRUE( passCount < 8 )  // hard limit in shader
	
	pState = esPreparing;
	#ifdef DO_STATE_DEBUG
		pRenderThread.GetLogger().LogInfoFormat( "ComputeRenderTask.BeginPrepare: this=%p state=%d", this, pState );
	#endif
	pStepCount = 0;
	
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
	pOcclusion = false;
	
	pFilterXRay = false;
	pXRay = false;
	
	pFilterHoles = false;
	pWithHoles = false;
	
	pFilterDecal = false;
	pDecal = false;
	
	pFilterCubeFace = -1;
	
	pPipelineDoubleSided = nullptr;
	pPipelineSingleSided = nullptr;
	
	pUseSpecialParamBlock = false;
	pUseSPBInstanceFlags = false;
}

void deoglComputeRenderTask::EndPass( const deoglWorldCompute &worldCompute ){
	DEASSERT_TRUE( pPass >= 0 )
	DEASSERT_TRUE( pPass < pPassCount )
	DEASSERT_TRUE( pState == esPreparing )
	
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
	// for occlusion only
	pUBOConfig->SetParameterDataUInt( ecpPipelineDoubleSided, pPass,
		pPipelineDoubleSided ? pPipelineDoubleSided->GetRTSIndex() : 0 );
	pUBOConfig->SetParameterDataUInt( ecpPipelineSingleSided, pPass,
		pPipelineSingleSided ? pPipelineSingleSided->GetRTSIndex() : 0 );
	
	pPass++;
}

void deoglComputeRenderTask::EndPrepare( const deoglWorldCompute &worldCompute ){
	const int count = worldCompute.GetElementGeometryCount();
	DEASSERT_TRUE( pPass != -1 )
	DEASSERT_TRUE( count > 0 )
	DEASSERT_TRUE( pState == esPreparing )
	
	pPass = -1;
	pState = esBuilding;
	#ifdef DO_STATE_DEBUG
		pRenderThread.GetLogger().LogInfoFormat( "ComputeRenderTask.EndPrepare: this=%p state=%d", this, pState );
	#endif
	
	pUBOConfig->UnmapBuffer();
	pClearCounters();
}

bool deoglComputeRenderTask::ReadBackSteps(){
	DEASSERT_TRUE( pPass == -1 )
	DEASSERT_TRUE( pState == esBuilding )
	
	int counterSteps;
	{ // scoping required to make sure buffer is not mapped if pClearCounters() is called
	const deoglSPBMapBufferRead mapped( pSSBOCounters, 0, 1 );
	const deoglRenderCompute::sCounters &counters =
		*( deoglRenderCompute::sCounters* )pSSBOCounters->GetMappedBuffer();
	counterSteps = counters.counter;
	}
	
	pStepCount = 0;
	
	if( counterSteps == 0 ){
		pState = esReady;
		#ifdef DO_STATE_DEBUG
			pRenderThread.GetLogger().LogInfoFormat( "ComputeRenderTask.ReadBackSteps(empty): this=%p state=%d", this, pState );
		#endif
		return true;
	}
	
	if( counterSteps > pSSBOSteps->GetElementCount() ){
		// SSBO has not been large enough so build and sort shader stopped writing steps to it.
		// enlarge the SSBO to be large enough then return false to request a rebuild
		pRenderThread.GetLogger().LogInfoFormat(
			"ComputeRenderTask.ReadBackSteps: ssbo not large enough, resized from %d to %d",
			pSSBOSteps->GetElementCount(), counterSteps );
		
		pSSBOSteps->SetElementCount( counterSteps + 10 );
		pSSBOSteps->EnsureBuffer();
		pClearCounters();
		pState = esBuilding;
		#ifdef DO_STATE_DEBUG
			pRenderThread.GetLogger().LogInfoFormat( "ComputeRenderTask.ReadBackSteps(rebuild): this=%p state=%d", this, pState );
		#endif
		return false;
	}
	
		decTimer timer;
	const deoglSPBMapBufferRead mapped( pSSBOSteps, 0, counterSteps );
	const sStep * const steps = ( const sStep* )pSSBOSteps->GetMappedBuffer();
	int i;
	
#ifdef DO_READ_BACK_TIMINGS
	pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: read %d in %dys",
		counterSteps, (int)(timer.GetElapsedTime()*1e6f));
#endif
	
	if( counterSteps > pStepSize ){
		if( pSteps ){
			delete [] pSteps;
			pSteps = nullptr;
		}
		pSteps = new sStep[ counterSteps ];
		pStepSize = counterSteps;
	}
	
	for( i=0; i<counterSteps; i++ ){
		pSteps[ pStepCount++ ] = steps[ i ];
		i += decMath::max( steps[ i ].subInstanceCount - 1, 0 );
	}
	
	// if steps are copied and the sub instance groups have been skipped already
	// pSkipSubInstanceGroups has to be set to false. if the SSBO is kept mapped and the steps
	// are used directly pSkipSubInstanceGroups has to be set to true. later solution can be
	// only used if direct state access is supported
	pSkipSubInstanceGroups = false;
	
	pState = esReady;
	#ifdef DO_STATE_DEBUG
		pRenderThread.GetLogger().LogInfoFormat( "ComputeRenderTask.ReadBackSteps: this=%p state=%d", this, pState );
	#endif
	
#ifdef DO_READ_BACK_TIMINGS
	pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: copied %d in %dys",
		pStepCount, (int)(timer.GetElapsedTime()*1e6f));
#endif
	return true;
}

void deoglComputeRenderTask::Render(){
	DEASSERT_TRUE( pPass == -1 )
	DEASSERT_TRUE( pState == esReady )
	
	pRenderThread.GetRenderers().GetGeometry().RenderTask( *this );
}



void deoglComputeRenderTask::SetUseSPBInstanceFlags( bool useFlags ){
	pUseSPBInstanceFlags = useFlags;
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

void deoglComputeRenderTask::SetOcclusion( bool occlusion ){
	pOcclusion = occlusion;
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

void deoglComputeRenderTask::SetPipelineDoubleSided( const deoglPipeline *pipeline ){
	pPipelineDoubleSided = pipeline;
}

void deoglComputeRenderTask::SetPipelineSingleSided( const deoglPipeline *pipeline ){
	pPipelineSingleSided = pipeline;
}

void deoglComputeRenderTask::SetUseSpecialParamBlock( bool use ){
	pUseSpecialParamBlock = use;
}



void deoglComputeRenderTask::DebugSimple( deoglRTLogger &logger, bool sorted ){
	int i, j;
	sStep *ss = nullptr;
	if( sorted && pStepCount > 0 && pSteps ){
		ss = new sStep[ pStepCount ];
		memcpy( ss, pSteps, sizeof( sStep ) * pStepCount );
		for( i=1; i<pStepCount - 1; i++ ){
			const sStep &p = ss[ i - 1 ];
			for( j=i; j<pStepCount; j++ ){
				const sStep &c = ss[ j ];
				if( c.pipeline == p.pipeline && c.tuc == p.tuc && c.vao == p.vao && c.instance == p.instance ) break;
			}
			if( j == pStepCount ){
				for( j=i; j<pStepCount; j++ ){
					const sStep &c = ss[ j ];
					if( c.pipeline == p.pipeline && c.tuc == p.tuc && c.vao == p.vao ) break;
				}
			}
			if( j == pStepCount ){
				for( j=i; j<pStepCount; j++ ){
					const sStep &c = ss[ j ];
					if( c.pipeline == p.pipeline && c.tuc == p.tuc ) break;
				}
			}
			if( j == pStepCount ){
				for( j=i; j<pStepCount; j++ ){
					const sStep &c = ss[ j ];
					if( c.pipeline == p.pipeline ) break;
				}
			}
			if( j < pStepCount ){
				const sStep t( ss[ i ] ); ss[ i ] = ss[ j ]; ss[ j ] = t;
			}
		}
	}
	
	logger.LogInfoFormat( "ComputeRenderTask %p", this );
	const deoglRenderTaskSharedPool &rtsPool = pRenderThread.GetRenderTaskSharedPool();
	for( i=0; i<pStepCount; i++ ){
		const sStep &s = ss ? ss[ i ] : pSteps[ i ];
		const deoglRenderTaskSharedInstance &rtsi = rtsPool.GetInstanceAt(s.instance);
		
		logger.LogInfoFormat( "- %d: p=%d t=%d v=%d i=%d [pc=%d fp=%d ic=%d fi=%d] si[c=%d i=%d f=%x]", i,
			s.pipeline, s.tuc, s.vao, s.instance, rtsi.GetPointCount(), rtsi.GetFirstPoint(),
			rtsi.GetIndexCount(), rtsi.GetFirstIndex(), s.subInstanceCount, s.spbInstance, s.specialFlags );
	}
	
	if( ss ) delete [] ss;
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
	
	mask |= ertfOcclusion;
	if( pOcclusion ){
		filter |= ertfOcclusion;
	}
}

void deoglComputeRenderTask::pClearCounters(){
	pSSBOCounters->ClearDataUInt( 0, 1, 1, 0 ); // workGroupSize.xyz, count
}
