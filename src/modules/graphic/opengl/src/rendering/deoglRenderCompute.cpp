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

#include "deoglRenderCompute.h"
#include "plan/deoglRenderPlan.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderCompute
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCompute::deoglRenderCompute( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines defines, commonDefines;
	
	pipconf.Reset();
	pipconf.SetType( deoglPipelineConfiguration::etCompute );
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	
	// SSBOs
	pSSBOUpdateElements.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOUpdateElements->SetRowMajor( rowMajor );
	pSSBOUpdateElements->SetParameterCount( 5 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeUpdateIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 );
	pSSBOUpdateElements->MapToStd140();
	pSSBOUpdateElements->SetBindingPoint( 1 );
	
	
	// update elements
	pipconf.SetShader( renderThread, "DefRen Plan Update Elements", commonDefines );
	pPipelineUpdateElements = pipelineManager.GetWith( pipconf );
	
	
	// find content
	defines = commonDefines;
	
	// pipconf.SetShader( renderThread, "DefRen Plan FindContent Node", defines );
	// pPipelineFindContentNode = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "DIRECT_ELEMENTS" );
	pipconf.SetShader( renderThread, "DefRen Plan FindContent Element", defines );
	pPipelineFindContentElement = pipelineManager.GetWith( pipconf );
}

deoglRenderCompute::~deoglRenderCompute(){
}



// Rendering
//////////////

void deoglRenderCompute::UpdateElements( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateElements" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	if( wcompute.GetUpdateElementCount() == 0 ){
		return;
	}
	
	pPipelineUpdateElements->Activate();
	
	plan.GetCompute().GetUBOFindConfig()->Activate();
	wcompute.GetSSBOElements()->Activate();
	pSSBOUpdateElements->Activate();
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( wcompute.GetUpdateElementCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
}

void deoglRenderCompute::FindContent( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContent" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
	const deoglRenderPlanCompute &planCompute = plan.GetCompute();
	
	
	// find nodes
#if 0
	pPipelineFindContentNode->Activate();
	
	planCompute.GetUBOFindConfig()->Activate();
	octree.GetSSBONodes()->Activate();
	octree.GetSSBOElements()->Activate();
	planCompute.GetSSBOSearchNodes()->Activate();
	planCompute.GetSSBOCounters()->ActivateAtomic();
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( octree.GetNodeCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT ) );
	
	
	pPipelineFindContentElement->Activate();
	
	planCompute.GetUBOFindConfig()->Activate();
	octree.GetSSBONodes()->Activate();
	octree.GetSSBOElements()->Activate();
	planCompute.GetSSBOSearchNodes()->Activate();
	planCompute.GetSSBOCounters()->Activate();
	planCompute.GetSSBOVisibleElements()->Activate();
	planCompute.GetSSBOCounters()->ActivateAtomic();
	
	planCompute.GetSSBOCounters()->ActivateDispatchIndirect();
	OGL_CHECK( renderThread, pglDispatchComputeIndirect( 0 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT ) );
	planCompute.GetSSBOCounters()->DeactivateDispatchIndirect();
#endif
	
	pPipelineFindContentElement->Activate();
	
	planCompute.GetUBOFindConfig()->Activate();
	wcompute.GetSSBOElements()->Activate();
	planCompute.GetSSBOVisibleElements()->Activate();
	planCompute.GetSSBOCounters()->ActivateAtomic();
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( wcompute.GetElementCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT ) );
}



// Protected Functions
////////////////////////
