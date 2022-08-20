/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

deoglRenderDebugDrawer::deoglRenderDebugDrawer( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const bool renderStereoVRLayer = GetRenderThread().GetChoices().GetRenderStereoVSLayer();
	deoglShaderDefines commonDefines;
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		GetRenderThread().GetShader().SetCommonDefines( commonDefines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		defines.SetDefines( "WITH_SELECTOR" );
		pShaderShapeXRay = shaderManager.GetProgramWith( sources, defines );
		
		if( renderStereoVRLayer ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
			defines.SetDefines( "GS_RENDER_STEREO" );
		}
		pShaderShapeXRayStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		defines.SetDefines( "WITH_DEPTH", "WITH_SELECTOR" );
		if( defren.GetUseInverseDepth() ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		pShaderShapeSolid = shaderManager.GetProgramWith( sources, defines );
		
		if( renderStereoVRLayer ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
			defines.SetDefines( "GS_RENDER_STEREO" );
		}
		pShaderShapeSolidStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		pShaderMeshXRay = shaderManager.GetProgramWith( sources, defines );
		
		if( renderStereoVRLayer ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
			defines.SetDefines( "GS_RENDER_STEREO" );
		}
		pShaderMeshXRayStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines = commonDefines;
		defines.SetDefines( "WITH_DEPTH" );
		if( defren.GetUseInverseDepth() ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		pShaderMeshSolid = shaderManager.GetProgramWith( sources, defines );
		
		if( renderStereoVRLayer ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Shape Stereo" );
			defines.SetDefines( "GS_RENDER_STEREO" );
		}
		pShaderMeshSolidStereo = shaderManager.GetProgramWith( sources, defines );
		
		
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
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
// 	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	const deoglRWorld &world = *plan.GetWorld();
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	// prepare depth testing
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// render debug drawers
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
	deoglShaderProgram *shaderProgram;
	deoglShaderCompiled *shader;
	int i, j;
	
	if( debugDrawer.GetXRay() ){
		shaderProgram = plan.GetRenderStereo() ? pShaderShapeXRayStereo : pShaderShapeXRay;
		
	}else{
		shaderProgram = plan.GetRenderStereo() ? pShaderShapeSolidStereo : pShaderShapeSolid;
	}
	
	renderThread.GetShader().ActivateShader( shaderProgram );
	shader = shaderProgram->GetCompiled();
	
	shader->SetParameterInt( sprDrawIDOffset, 0 );
	
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
			shader->SetParameterDMatrix4x3( sprMatrixModel, decDMatrix( visitor.GetMatrix1() ) * matrixModelShape );
			shader->SetParameterDMatrix4x3( sprMatrixModel2, decDMatrix( visitor.GetMatrix2() ) * matrixModelShape );
			
			if( visitor.GetShape() && ( fillVisible || lineVisible ) ){
				visitor.GetShape()->ActivateVAO();
				
				if( fillVisible ){
					shader->SetParameterColor4( sprColor, fillColor );
					visitor.GetShape()->RenderFaces( plan );
				}
				
				if( lineVisible ){
					shader->SetParameterColor4( sprColor, edgeColor );
					visitor.GetShape()->RenderLines( plan );
				}
			}
		}
	}
}

void deoglRenderDebugDrawer::pRenderDDSFaces( const deoglRenderPlan &plan,
const decDMatrix &matrixModel, deoglRDebugDrawer &debugDrawer ){
	const int shapeCount = debugDrawer.GetShapeCount();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglVAO * const vao = debugDrawer.GetVAO();
	deoglShaderProgram *shaderProgram;
	deoglShaderCompiled *shader;
	int i;
	
	if( ! vao ){
		return;
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao->GetVAO() ) );
	
	if( debugDrawer.GetXRay() ){
		shaderProgram = plan.GetRenderStereo() ? pShaderMeshXRayStereo : pShaderMeshXRay;
		
	}else{
		shaderProgram = plan.GetRenderStereo() ? pShaderMeshSolidStereo : pShaderMeshSolid;
	}
	
	renderThread.GetShader().ActivateShader( shaderProgram );
	shader = shaderProgram->GetCompiled();
	
	shader->SetParameterInt( sprDrawIDOffset, 0 );
	
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
		shader->SetParameterDMatrix4x3( sprMatrixModel, decDMatrix( ddshape.GetMatrix() ) * matrixModel );
		
		// render fill
		if( fillVisible && ddshape.GetFillPointCount() > 0 ){
			shader->SetParameterColor4( sprColor, fillColor );
			
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
			shader->SetParameterColor4( sprColor, edgeColor );
			
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
