/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderDebugDrawer.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../debugdrawer/deoglDebugDrawerShape.h"
#include "../../debugdrawer/deoglRDebugDrawer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../pipeline/deoglPipelineManager.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shapes/deoglShape.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglVBOAttribute.h"
#include "../../vbo/deoglVBOLayout.h"
#include "../../visitors/deoglVolumeShape.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>


// Definitions
////////////////

enum eSPRender{
	sprMatrixModel,
	sprColor,
	sprMatrixModel2,
	sprDrawIDOffset
};


// Class deoglRenderDebugDrawer
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderDebugDrawer::deoglRenderDebugDrawer( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const bool renderStereoVRLayer = renderThread.GetChoices().GetRenderStereoVSLayer();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines commonDefines;
	const deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		GetRenderThread().GetShader().SetCommonDefines( commonDefines );
		
		
		
		pipconf.Reset();
		pipconf.SetDepthMask( false );
		pipconf.EnableBlendBlend();
		
		
		
		// shape x-ray
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		defines.SetDefines( "WITH_SELECTOR" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineShapeXRay = pipelineManager.GetWith( pipconf );
		
		// shape x-ray stereo
		defines.SetDefines( renderStereoVRLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderStereoVRLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineShapeXRayStereo = pipelineManager.GetWith( pipconf );
		
		
		
		// shape solid
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		defines.SetDefines( "WITH_DEPTH", "WITH_SELECTOR" );
		if( useInverseDepth ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineShapeSolid = pipelineManager.GetWith( pipconf );
		
		// shape solid stereo
		defines.SetDefines( renderStereoVRLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderStereoVRLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineShapeSolidStereo = pipelineManager.GetWith( pipconf );
		
		
		
		// mesh x-ray
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineMeshXRay = pipelineManager.GetWith( pipconf );
		
		// mesh x-ray stereo
		defines.SetDefines( renderStereoVRLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderStereoVRLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineMeshXRayStereo = pipelineManager.GetWith( pipconf );
		
		
		
		// mesh solid
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		defines.SetDefines( "WITH_DEPTH" );
		if( useInverseDepth ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineMeshSolid = pipelineManager.GetWith( pipconf );
		
		// mesh solid stereo
		defines.SetDefines( renderStereoVRLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderStereoVRLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineMeshSolidStereo = pipelineManager.GetWith( pipconf );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderDebugDrawer::~deoglRenderDebugDrawer(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderDebugDrawer::RenderDebugDrawers( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DebugDrawer.RenderDebugDrawers" );
	
	// prepare depth testing
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// render debug drawers
	const deoglRWorld &world = *plan.GetWorld();
	const decDVector &referencePosition = world.GetReferencePosition();
	const int count = world.GetDebugDrawerCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRDebugDrawer &debugDrawer = *world.GetDebugDrawerAt( i );
		
		// if not visible skip the debug drawer
		if( ! debugDrawer.GetVisible() ){
			continue;
		}
		
		// if there are no shapes or faces skip the debug drawer
		const bool hasShapes = debugDrawer.GetHasShapes();
		const bool hasFaces = debugDrawer.GetHasFaces();
		if( ! hasShapes && ! hasFaces ){
			continue;
		}
		
		decDMatrix matrixModel( debugDrawer.GetMatrix() );
		matrixModel.a14 -= referencePosition.x;
		matrixModel.a24 -= referencePosition.y;
		matrixModel.a34 -= referencePosition.z;
		
		// render shapes if there are any
		if( hasShapes ){
			pRenderDDSShapes( plan, matrixModel, debugDrawer );
		}
		
		// render faces if existing
		if( hasFaces ){
			pRenderDDSFaces( plan, matrixModel, debugDrawer );
		}
	}
	
	// cleanup
	pglBindVertexArray( 0 );
	tsmgr.DisableStage( 0 );
}



// Private Functions
//////////////////////

void deoglRenderDebugDrawer::pCleanUp(){
}



void deoglRenderDebugDrawer::pRenderDDSShapes( const deoglRenderPlan &plan,
const decDMatrix &matrixModel, deoglRDebugDrawer &debugDrawer ){
	const int shapeCount = debugDrawer.GetShapeCount();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglVolumeShape visitor( renderThread );
	int i, j;
	
	const deoglPipeline &pipeline = debugDrawer.GetXRay()
		? ( plan.GetRenderStereo() ? *pPipelineShapeXRayStereo : *pPipelineShapeXRay )
		: ( plan.GetRenderStereo() ? *pPipelineShapeSolidStereo : *pPipelineShapeSolid );
	pipeline.Activate();
	
	deoglShaderCompiled &shader = pipeline.GetGlShader();
	shader.SetParameterInt( sprDrawIDOffset, 0 );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	for( i=0; i<shapeCount; i++ ){
		const deoglDebugDrawerShape &ddshape = debugDrawer.GetShapeAt( i );
		
		// render debug drawer volume
		const decShapeList &shapeList = ddshape.GetShapeList();
		const int shapeShapeCount = shapeList.GetCount();
		
		if( shapeShapeCount == 0 ){
			continue;
		}
		
		// determine render color
		decColor edgeColor( ddshape.GetEdgeColor() );
		edgeColor.r = powf( edgeColor.r, OGL_RENDER_GAMMA );
		edgeColor.g = powf( edgeColor.g, OGL_RENDER_GAMMA );
		edgeColor.b = powf( edgeColor.b, OGL_RENDER_GAMMA );
		
		decColor fillColor( ddshape.GetFillColor() );
		fillColor.r = powf( fillColor.r, OGL_RENDER_GAMMA );
		fillColor.g = powf( fillColor.g, OGL_RENDER_GAMMA );
		fillColor.b = powf( fillColor.b, OGL_RENDER_GAMMA );
		
		const bool lineVisible = ( edgeColor.a > 0.001f );
		const bool fillVisible = ( fillColor.a > 0.001f );
		if( ! lineVisible && ! fillVisible ){
			continue;
		}
		
		// determine the matrix
		const decDMatrix matrixModelShape( decDMatrix( ddshape.GetMatrix() ) * matrixModel );
		
		// render shapes if existing
		for( j=0; j<shapeShapeCount; j++ ){
			shapeList.GetAt( j )->Visit( visitor );
			
			// set matrix
			shader.SetParameterDMatrix4x3( sprMatrixModel, decDMatrix( visitor.GetMatrix1() ) * matrixModelShape );
			shader.SetParameterDMatrix4x3( sprMatrixModel2, decDMatrix( visitor.GetMatrix2() ) * matrixModelShape );
			
			if( visitor.GetShape() && ( fillVisible || lineVisible ) ){
				visitor.GetShape()->ActivateVAO();
				
				if( fillVisible ){
					shader.SetParameterColor4( sprColor, fillColor );
					visitor.GetShape()->RenderFaces( plan );
				}
				
				if( lineVisible ){
					shader.SetParameterColor4( sprColor, edgeColor );
					visitor.GetShape()->RenderLines( plan );
				}
			}
		}
	}
}

void deoglRenderDebugDrawer::pRenderDDSFaces( const deoglRenderPlan &plan,
const decDMatrix &matrixModel, deoglRDebugDrawer &debugDrawer ){
	deoglVAO * const vao = debugDrawer.GetVAO();
	if( ! vao ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const int shapeCount = debugDrawer.GetShapeCount();
	int i;
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao->GetVAO() ) );
	
	const deoglPipeline &pipeline = debugDrawer.GetXRay()
		? ( plan.GetRenderStereo() ? *pPipelineMeshXRayStereo : *pPipelineMeshXRay )
		: ( plan.GetRenderStereo() ? *pPipelineMeshSolidStereo : *pPipelineMeshSolid );
	pipeline.Activate();
	
	deoglShaderCompiled &shader = pipeline.GetGlShader();
	shader.SetParameterInt( sprDrawIDOffset, 0 );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	for( i=0; i<shapeCount; i++ ){
		const deoglDebugDrawerShape &ddshape = debugDrawer.GetShapeAt( i );
		
		if( ddshape.GetFillPointCount() == 0 || ddshape.GetLinePointCount() == 0 ){
			continue;
		}
		
		decColor edgeColor( ddshape.GetEdgeColor() );
		edgeColor.r = powf( edgeColor.r, OGL_RENDER_GAMMA );
		edgeColor.g = powf( edgeColor.g, OGL_RENDER_GAMMA );
		edgeColor.b = powf( edgeColor.b, OGL_RENDER_GAMMA );
		
		decColor fillColor( ddshape.GetFillColor() );
		fillColor.r = powf( fillColor.r, OGL_RENDER_GAMMA );
		fillColor.g = powf( fillColor.g, OGL_RENDER_GAMMA );
		fillColor.b = powf( fillColor.b, OGL_RENDER_GAMMA );
		
		const bool lineVisible = ( edgeColor.a > 0.001f );
		const bool fillVisible = ( fillColor.a > 0.001f );
		if( ! lineVisible && ! fillVisible ){
			continue;
		}
		
		// set matrix
		shader.SetParameterDMatrix4x3( sprMatrixModel, decDMatrix( ddshape.GetMatrix() ) * matrixModel );
		
		// render fill
		if( fillVisible && ddshape.GetFillPointCount() > 0 ){
			shader.SetParameterColor4( sprColor, fillColor );
			
			if( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() ){
				const GLint first[ 2 ] = { ddshape.GetFillFirstPoint(), ddshape.GetFillFirstPoint() };
				const GLsizei count[ 2 ] = { ddshape.GetFillPointCount(), ddshape.GetFillPointCount() };
				OGL_CHECK( renderThread, pglMultiDrawArrays( GL_TRIANGLES, first, count, 2 ) );
				
			}else{
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, ddshape.GetFillFirstPoint(), ddshape.GetFillPointCount() ) );
			}
		}
		
		// render wires
		if( lineVisible && ddshape.GetLinePointCount() > 0 ){
			shader.SetParameterColor4( sprColor, edgeColor );
			
			if( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() ){
				const GLint first[ 2 ] = { ddshape.GetLineFirstPoint(), ddshape.GetLineFirstPoint() };
				const GLsizei count[ 2 ] = { ddshape.GetLinePointCount(), ddshape.GetLinePointCount() };
				OGL_CHECK( renderThread, pglMultiDrawArrays( GL_LINES, first, count, 2 ) );
				
			}else{
				OGL_CHECK( renderThread, glDrawArrays( GL_LINES, ddshape.GetLineFirstPoint(), ddshape.GetLinePointCount() ) );
			}
		}
	}
}
