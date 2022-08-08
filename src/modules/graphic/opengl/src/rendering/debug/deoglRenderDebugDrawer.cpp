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
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
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
	sprMatrixMVP,
	sprColor,
	sprSCToDTC,
	// only for shaders with selector
	sprMatrixMVP2
};




// Class deoglRenderDebugDrawer
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderDebugDrawer::deoglRenderDebugDrawer( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Shape" );
		defines.SetDefine( "WITH_SELECTOR", "1" );
		if( defren.GetUseInverseDepth() ){
			defines.SetDefine( "INVERSE_DEPTH", "1" );
		}
		pShaderShapeXRay = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "WITH_DEPTH", "1" );
		pShaderShapeSolid = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		if( defren.GetUseInverseDepth() ){
			defines.SetDefine( "INVERSE_DEPTH", "1" );
		}
		pShaderMeshXRay = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "WITH_DEPTH", "1" );
		pShaderMeshSolid = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
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
	
	// render all debug shapes with a z-offset to avoid z-fighting for shapes overlapping rendered
	// geometry. doing this by default is okay since debug drawers are supposed to be rendered
	// after world geometry and thus winning over world geometry feels logic.
	// pglPolygonOffset can not be used since depth has to be compared in the fragment shader. this
	// is the case since the depth comes from a texture due to the depth buffer being undefined.
	// to solve this problem the shift is placed into the camera-projection matrix after the
	// projection. the z value has to be shifted slightly into the negative direction. the shift
	// has to be some depth units which is 1/(1<<precision) where precision is depth-buffer-buts
	// minus some bits to counter rounding errors. shifting by 1 or 2 depth bits should help.
	// too much and geometry pops up behind geometry
	const double depthScale = 1.0 - 1.0 / ( double )( 1 << 22 ); // 24bits minus 2 bits
	const double depthShift = -1.0f / ( double )( 1 << 21 ); // 24bits minus 3 bits
	
	// prepare depth testing
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// render debug drawers
	const decDMatrix matrixCP( plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() )
		* decDMatrix::CreateST( decDVector( 1.0, 1.0, depthScale ), decDVector( 0.0, 0.0, depthShift ) ) );
	
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
		
		const decDMatrix matrixDDCP( debugDrawer.GetMatrix() * matrixCP );
		
		// render shapes if there are any
		if( hasShapes ){
			pRenderDDSShapes( matrixDDCP, debugDrawer );
		}
		
		// render faces if existing
		if( hasFaces ){
			pRenderDDSFaces( matrixDDCP, debugDrawer );
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



void deoglRenderDebugDrawer::pRenderDDSShapes( const decDMatrix &matrixCP, deoglRDebugDrawer &debugDrawer ){
	const int shapeCount = debugDrawer.GetShapeCount();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglVolumeShape visitor( renderThread );
	deoglShaderCompiled *shader;
	int i, j;
	
	if( debugDrawer.GetXRay() ){
		renderThread.GetShader().ActivateShader( pShaderShapeXRay );
		shader = pShaderShapeXRay->GetCompiled();
		
	}else{
		renderThread.GetShader().ActivateShader( pShaderShapeSolid );
		shader = pShaderShapeSolid->GetCompiled();
		shader->SetParameterFloat( sprSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
	}
	
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
		const decDMatrix matrixMVP( decDMatrix( ddshape.GetMatrix() ) * matrixCP );
		
		// render shapes if existing
		for( j=0; j<shapeShapeCount; j++ ){
			shapeList.GetAt( j )->Visit( visitor );
			
			// set matrix
			shader->SetParameterDMatrix4x4( sprMatrixMVP, decDMatrix( visitor.GetMatrix1() ) * matrixMVP );
			shader->SetParameterDMatrix4x4( sprMatrixMVP2, decDMatrix( visitor.GetMatrix2() ) * matrixMVP );
			
			if( visitor.GetShape() && ( fillVisible || lineVisible ) ){
				visitor.GetShape()->ActivateVAO();
				
				if( fillVisible ){
					shader->SetParameterColor4( sprColor, fillColor );
					visitor.GetShape()->RenderFaces();
				}
				
				if( lineVisible ){
					shader->SetParameterColor4( sprColor, edgeColor );
					visitor.GetShape()->RenderLines();
				}
			}
		}
	}
}

void deoglRenderDebugDrawer::pRenderDDSFaces( const decDMatrix &matrixCP, deoglRDebugDrawer &debugDrawer ){
	const int shapeCount = debugDrawer.GetShapeCount();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglVAO * const vao = debugDrawer.GetVAO();
	deoglShaderCompiled *shader;
	int i;
	
	if( ! vao ){
		return;
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao->GetVAO() ) );
	
	if( debugDrawer.GetXRay() ){
		renderThread.GetShader().ActivateShader( pShaderMeshXRay );
		shader = pShaderMeshXRay->GetCompiled();
		
	}else{
		renderThread.GetShader().ActivateShader( pShaderMeshSolid );
		shader = pShaderMeshSolid->GetCompiled();
		shader->SetParameterFloat( sprSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
	}
	
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
		
		// determine the matrix
		const decDMatrix matrixMVP( decDMatrix( ddshape.GetMatrix() ) * matrixCP );
		
		// set matrix
		shader->SetParameterDMatrix4x4( sprMatrixMVP, matrixMVP );
		
		// render fill
		if( fillVisible && ddshape.GetFillPointCount() > 0 ){
			shader->SetParameterColor4( sprColor, fillColor );
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, ddshape.GetFillFirstPoint(), ddshape.GetFillPointCount() ) );
		}
		
		// render wires
		if( lineVisible && ddshape.GetLinePointCount() > 0 ){
			shader->SetParameterColor4( sprColor, edgeColor );
			OGL_CHECK( renderThread, glDrawArrays( GL_LINES, ddshape.GetLineFirstPoint(), ddshape.GetLinePointCount() ) );
		}
	}
}
