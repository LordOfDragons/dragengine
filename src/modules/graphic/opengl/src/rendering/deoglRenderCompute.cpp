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
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines defines, commonDefines;
	
	pipconf.Reset();
	pipconf.SetType( deoglPipelineConfiguration::etCompute );
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	
	// find content
	defines = commonDefines;
	
	pipconf.SetShader( renderThread, "DefRen Plan FindContent", defines );
	pPipelineFindContentNode = pipelineManager.GetWith( pipconf );
}

deoglRenderCompute::~deoglRenderCompute(){
}



// Rendering
//////////////

void deoglRenderCompute::FindContent( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContent" );
	
	deoglWorldCSOctree &csoctree = plan.GetCompute().GetWorldCSOctree();
	if( csoctree.GetElementCount() == 0 ){
		return;
	}
	
	
	// find nodes
	pPipelineFindContentNode->Activate();
	
	csoctree.GetSSBONodes()->Activate();
	csoctree.GetSSBOElements()->Activate();
	// activate result SSBOs
	
	deoglShaderCompiled &programNode = pPipelineFindContentNode->GetGlShader();
	programNode.SetParameterUInt( 0, csoctree.GetNodeCount() ); // Count
	
	decDVector frustumMinExtend, frustumMaxExtend;
	// pCalculateFrustumBoundaryBox( plan, frustumMinExtend, frustumMaxExtend );
	
	// const deoglDCollisionFrustum &frustum = *plan.GetUseFrustum();
	programNode.SetParameterVector4( 1, decVector4() );
	
#if 0
	programNode.SetParameterFloat( 1, );
uniform vec4 pFrustumPlanes[ 6 ]; // xyz=normal, w=distance
uniform bvec3 pFrustumSelect[ 6 ]; // greaterThan(pFrustumPlanes[x], vec3(0))
	
	<parameter name='pFrustumPlanes'/>
	<parameter name='pFrustumSelect'/>
	<parameter name='pGIMinExtend'/>
	<parameter name='pGIMaxExtend'/>
	
	
	transformed.Activate();
	if( weightMatrices ){
		weightMatrices->Activate();
	}
	
	pPipelineApproxTransformVNT->GetGlShader().SetParameterUInt( 0, firstPoint, pointCount );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( pointCount - 1 ) / 64 + 1, 1, 1 ) );
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0) );
	transformed.Deactivate();
	if( weightMatrices ){
		weightMatrices->Deactivate();
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
#endif
}



// Protected Functions
////////////////////////
