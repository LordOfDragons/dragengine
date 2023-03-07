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
pPassCount( 0 ),
pPass( -1 ),
pUseSPBInstanceFlags( false ),
pRenderVSStereo( false ),
pSteps( nullptr )
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
	
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etRead ) );
	pSSBOCounters->SetRowMajor( rowMajor );
	pSSBOCounters->SetParameterCount( 2 );
	pSSBOCounters->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // uvec3
	pSSBOCounters->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pSSBOCounters->SetElementCount( 1 );
	pSSBOCounters->MapToStd140();
	
	Clear();
}

deoglComputeRenderTask::~deoglComputeRenderTask(){
	if( pSteps && pSSBOSteps->IsBufferMapped() ){
		pSSBOSteps->UnmapBufferRead();
	}
}



// Management
///////////////

#define DO_READ_BACK_TIMINGS

void deoglComputeRenderTask::BeginPrepare( int passCount ){
	DEASSERT_TRUE( pPass == -1 )
	DEASSERT_TRUE( passCount > 0 )
	DEASSERT_TRUE( passCount < 8 )  // hard limit in shader
	
	if( pSteps ){
		if( pSSBOSteps->IsBufferMapped() ){
			pSSBOSteps->UnmapBufferRead();
		}
		pSteps = nullptr;
	}
	
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
	pUseSPBInstanceFlags = false;
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
	
	pUBOConfig->UnmapBuffer();
	pClearCounters();
	
	if( count > pSSBOSteps->GetElementCount() ){
		pSSBOSteps->SetElementCount( count );
		pSSBOSteps->EnsureBuffer();
	}
}

void deoglComputeRenderTask::SortSteps(){
	DEASSERT_TRUE( pPass == -1 )
	
		decTimer timer;
	const deoglSPBMapBufferRead mapped( pSSBOCounters, 0, 1 );
	const sCounters &counters = *( sCounters* )pSSBOCounters->GetMappedBuffer();
	pStepCount = counters.counter;
	if( pStepCount == 0 ){
		return;
	}
	
	pRenderThread.GetRenderers().GetCompute().SortRenderTask( *this );
#ifdef DO_READ_BACK_TIMINGS
	pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.SortSteps: sort %dys. %d steps",
		(int)(timer.GetElapsedTime()*1e6f), pStepCount);
#endif
}

void deoglComputeRenderTask::ReadBackSteps(){
	DEASSERT_TRUE( pPass == -1 )
	
	if( pStepCount == 0 ){
		return;
	}
	
		decTimer timer;
	pSSBOSteps->MapBufferRead( 0, pStepCount );
	pSteps = ( const sStep* )pSSBOSteps->GetMappedBuffer();
#ifdef DO_READ_BACK_TIMINGS
	const int elapsed = (int)(timer.GetElapsedTime()*1e6f);
	int i, igcount=0;
	for(i=0; i<pStepCount; i++){
		igcount++;
		i += decMath::max(pSteps[i].subInstanceCount - 1, 0);
	}
	pRenderThread.GetLogger().LogInfoFormat("ComputeRenderTask.ReadBackSteps: read %d (%d groups) in %dys",
		pStepCount, igcount, elapsed);
#endif
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
		
		logger.LogInfoFormat( "- %d: p=%d t=%d v=%d i=%d [pc=%d fp=%d ic=%d fi=%d] si[i=%d f=%x]", i,
			s.pipeline, s.tuc, s.vao, s.instance, rtsi.GetPointCount(), rtsi.GetFirstPoint(),
			rtsi.GetIndexCount(), rtsi.GetFirstIndex(), s.spbInstance, s.specialFlags );
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
}

void deoglComputeRenderTask::pClearCounters(){
	pSSBOCounters->ClearDataUInt( pSSBOCounters->GetElementCount(), 0, 1, 1, 0 ); // workGroupSize.xyz, count
}
