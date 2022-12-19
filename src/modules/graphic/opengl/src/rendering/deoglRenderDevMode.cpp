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

#include "deoglRenderDevMode.h"
#include "debug/deoglRenderDebug.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanDebug.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListLight.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../collidelist/deoglCollideListPropField.h"
#include "../collidelist/deoglCollideListPropFieldType.h"
#include "../collidelist/deoglCollideListPropFieldCluster.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../deoglDebugFont.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../light/deoglRLight.h"
#include "../light/volume/deoglLightVolume.h"
#include "../memory/deoglMemoryManager.h"
#include "../occlusiontest/deoglOcclusionMap.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../propfield/deoglPropFieldCluster.h"
#include "../propfield/deoglRPropField.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeBox.h"
#include "../shapes/deoglShapeCylinder.h"
#include "../shapes/deoglShapeManager.h"
#include "../skin/deoglRSkin.h"
#include "../terrain/heightmap/deoglHTSCluster.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglHTViewSectorCluster.h"
#include "../terrain/heightmap/deoglRHeightTerrain.h"
#include "../terrain/heightmap/deoglRHTSector.h"
#include "../texture/arraytexture/deoglRenderableColorArrayTextureManager.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugTraceGroup.h"

#include <dragengine/common/exceptions.h>
#include "../utils/collision/deoglDCollisionBox.h"



// Definitions
////////////////

#define OFFSET_POINT	0
#define COUNT_POINT		1

#define OFFSET_LINE		1
#define COUNT_LINE		2

#define OFFSET_RECT		3
#define COUNT_RECT		4

#define SCALEX( cx )	( pScalePosition.x * ( float )( cx ) )
#define SCALEY( cy )	( pScalePosition.y * ( float )( cy ) )
#define POSX( px )		( pScalePosition.x * ( float )( px ) + pOffsetPosition.x )
#define POSY( py )		( pScalePosition.y * ( float )( py ) + pOffsetPosition.y )

enum eSPSolidColor2D{
	spsc2dColor,
	spsc2dPosTransform
};

enum eSPSolidColor3D{
	spsc3dColor,
	spsc3dMatrixMVP
};

enum eSPShape{
	spsMatrixMVP,
	spsColor,
	spsSCToDTC,
	spsMatrixMVP2
};



// Class deoglRenderDevMode
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderDevMode::deoglRenderDevMode( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines defines;
	
	pVBOShapes = 0;
	pVAOShapes = 0;
	
	try{
		pCreateShapesVAO();
		
		pipconf.Reset();
		pipconf.SetDepthMask( false );
		pipconf.EnableBlendBlend();
		
		// 2d solid color
		pipconf.SetShader( renderThread, "2D Solid Color", defines );
		pPipelineSolidColor2D = pipelineManager.GetWith( pipconf );
		
		// 3d solid color
		pipconf.SetShader( renderThread, "3D Solid Color", defines );
		pPipelineSolidColor3D = pipelineManager.GetWith( pipconf );
		
		// shape
		defines.SetDefines( "WITH_SELECTOR" );
		if( defren.GetUseInverseDepth() ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		//defines.SetDefines( "WITH_DEPTH" );
		pipconf.SetShader( renderThread, "DefRen Shape", defines );
		pPipelineShape = pipelineManager.GetWith( pipconf );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderDevMode::~deoglRenderDevMode(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderDevMode::RenderDevMode( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DevMode.RenderDevMode" );
	deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	
	// 3d information
	if( devMode.GetShowHeightTerrain() ){
		RenderHeightTerrainBoxes( plan );
	}
	
	if( devMode.GetShowPropFieldBox() || devMode.GetShowPropFieldClusters() > -1 ){
		RenderPropFieldInfo( plan );
	}
	
	if( devMode.GetShowVisComponent() ){
		RenderVisComponent( plan );
	}
	
	if( devMode.GetShowVisLight() ){
		RenderVisLight( plan );
	}
	
	if( devMode.GetShowComponentLODLevels() ){
		RenderComponentLodLevels( plan );
	}
	
	if( devMode.GetHighlightTransparentObjects() ){
		RenderHighlightTransparentObjects( plan );
	}
	
	if( devMode.GetShowEnvMaps() || devMode.GetShowEnvMapHull() ){
		RenderEnvMapInfo( plan );
	}
	
	if( devMode.GetShowLightFullBox() || devMode.GetShowLightBox() || devMode.GetShowLightRooms()
	|| devMode.GetShowLightVolume() || devMode.GetShowLightVisualInfo() != -1 ){
		RenderLightInfos( plan );
	}
	
	// 2d information
	RenderOverlayInfos( plan );
}

void deoglRenderDevMode::RenderVisComponent( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglCollideList &collideList = plan.GetCollideList();
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const int componentCount = collideList.GetComponentCount();
	const decColor colorWire( 0.0, 0.5, 1.0, 1.0 );
	const decColor colorSolid( 0.0, 0.5, 1.0, 0.1 );
	deoglDCollisionBox box;
	int c;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	shader.SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
	for( c=0; c<componentCount; c++ ){
		deoglRComponent &component = *collideList.GetComponentAt( c )->GetComponent();
		
		box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
		shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
		
		shapeBox.RenderFaces();
	}
	
	shader.SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
	for( c=0; c<componentCount; c++ ){
		deoglRComponent &component = *collideList.GetComponentAt( c )->GetComponent();
		
		box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
		shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
		
		shapeBox.RenderLines();
	}
	pglBindVertexArray( 0 );
}

void deoglRenderDevMode::RenderVisLight( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglCollideList &collideList = plan.GetCollideList();
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const int lightCount = collideList.GetLightCount();
	const decColor colorWire( 0.0, 0.5, 1.0, 1.0 );
	const decColor colorSolid( 0.0, 0.5, 1.0, 0.1 );
	deoglDCollisionBox box;
	int l;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	shader.SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
	for( l=0; l<lightCount; l++ ){
		deoglRLight &light = *collideList.GetLightAt( l )->GetLight();
		box.SetFromExtends( light.GetMinimumExtend(), light.GetMaximumExtend() );
		shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
		shapeBox.RenderFaces();
	}
	
	shader.SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
	for( l=0; l<lightCount; l++ ){
		deoglRLight &light = *collideList.GetLightAt( l )->GetLight();
		box.SetFromExtends( light.GetMinimumExtend(), light.GetMaximumExtend() );
		shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
		shapeBox.RenderLines();
	}
	pglBindVertexArray( 0 );
}

void deoglRenderDevMode::RenderComponentLodLevels( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglCollideList &collideList = plan.GetCollideList();
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const int componentCount = collideList.GetComponentCount();
	const decColor colorLevels[ 6 ] = {
		decColor( 1.0f, 0.0f, 0.0f ),
		decColor( 0.5f, 0.5f, 0.0f ),
		decColor( 0.0f, 1.0f, 0.0f ),
		decColor( 0.0f, 0.5f, 0.5f ),
		decColor( 0.0f, 0.0f, 1.0f ),
		decColor( 0.5f, 0.0f, 0.5f ) };
	const float alphaSolid = 0.1f;
	const float alphaWire = 1.0f;
	deoglDCollisionBox box;
	int c;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	for( c=0; c<componentCount; c++ ){
		deoglCollideListComponent &clComponent = *collideList.GetComponentAt( c );
		deoglRComponent &component = *clComponent.GetComponent();
		
		const int lodLevel = decMath::min( clComponent.GetLODLevel(), 5 );
		shader.SetParameterFloat( spsc3dColor, colorLevels[ lodLevel ].r, colorLevels[ lodLevel ].g, colorLevels[ lodLevel ].b, alphaSolid );
		
		box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
		shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
		
		shapeBox.RenderFaces();
	}
	
	for( c=0; c<componentCount; c++ ){
		deoglCollideListComponent &clComponent = *collideList.GetComponentAt( c );
		deoglRComponent &component = *clComponent.GetComponent();
		
		const int lodLevel = decMath::min( clComponent.GetLODLevel(), 5 );
		shader.SetParameterFloat( spsc3dColor, colorLevels[ lodLevel ].r, colorLevels[ lodLevel ].g, colorLevels[ lodLevel ].b, alphaWire );
		
		box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
		shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
		
		shapeBox.RenderLines();
	}
	pglBindVertexArray( 0 );
}

void deoglRenderDevMode::RenderHighlightTransparentObjects( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglCollideList &collideList = plan.GetCollideList();
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const int componentCount = collideList.GetComponentCount();
	const decColor colorWire( 1.0f, 0.0f, 0.0f, 1.0f );
	const decColor colorSolid( 1.0f, 0.0f, 0.0f, 0.1f );
	deoglDCollisionBox box;
	int c;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	shader.SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
	for( c=0; c<componentCount; c++ ){
		const deoglRComponent &component = *collideList.GetComponentAt( c )->GetComponent();
		
		if( component.GetSkin() && ! component.GetSkin()->GetIsSolid() ){
			box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
			shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
				* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
			
			shapeBox.RenderFaces();
		}
	}
	
	shader.SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
	for( c=0; c<componentCount; c++ ){
		const deoglRComponent &component = *collideList.GetComponentAt( c )->GetComponent();
		
		if( component.GetSkin() && ! component.GetSkin()->GetIsSolid() ){
			box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
			shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
				* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
			
			shapeBox.RenderLines();
		}
	}
	pglBindVertexArray( 0 );
}

void deoglRenderDevMode::RenderHeightTerrainBoxes( deoglRenderPlan &plan ){
	deoglRHeightTerrain *heightTerrain = plan.GetWorld()->GetHeightTerrain();
	if( ! heightTerrain ) return;
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const float sectorDim = heightTerrain->GetSectorSize();
	const int sectorCount = heightTerrain->GetSectorCount();
	const decColor colorWire( 0.75f, 0.75f, 0.0f, 1.0 );
	int s, c;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	shader.SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
	for( s=0; s<sectorCount; s++ ){
		const deoglRHTSector &sector = heightTerrain->GetSectorAt( s );
		
		if( sector.GetValid() ){
			// TODO test if inside view frustum
			
			const int clusterCount = sector.GetClusterCount() * sector.GetClusterCount();
			const deoglHTSCluster * const clusters = sector.GetClusters();
			const decPoint &scoord = sector.GetCoordinates();
			
			for( c=0; c<clusterCount; c++ ){
				shader.SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateTranslation( sectorDim * ( double )scoord.x, 0.0, sectorDim * ( double )scoord.y )
					* decDMatrix::CreateScale( clusters[ c ].GetHalfExtends() )
					* decDMatrix::CreateTranslation( clusters[ c ].GetCenter() ) * matrixVP );
				
				shapeBox.RenderLines();
			}
		}
	}
	pglBindVertexArray( 0 );
}

void deoglRenderDevMode::RenderPropFieldInfo( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShapeBox &shapeBox = *( ( deoglShapeBox* )shapeManager.GetShapeAt( deoglRTBufferObject::esBox ) );
	const deoglCollideList &collideList = plan.GetCollideList();
	const int propFieldCount = collideList.GetPropFieldCount();
	const deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	const int typeIndex = devMode.GetShowPropFieldClusters();
	const float alphaSolid = 0.05f;
	const float alphaWire = 1.0f;
	deoglDCollisionBox box;
	int i, j, k;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled *shader = pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	if( devMode.GetShowPropFieldBox() ){
		const decColor colorSolid( 0.0f, 1.0f, 0.0f, alphaSolid );
		const decColor colorWire( colorSolid.r, colorSolid.g, colorSolid.b, alphaWire );
		
		for( i=0; i<propFieldCount; i++ ){
			deoglRPropField &propField = *collideList.GetPropFieldAt( i )->GetPropField();
			
			box.SetFromExtends( propField.GetMinimumExtend(), propField.GetMaximumExtend() );
			
			shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
				* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
			
			shader->SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
			shapeBox.RenderFaces();
			
			shader->SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
			shapeBox.RenderLines();
		}
	}
	
	if( typeIndex > -1 ){
		const decColor colorSolid( 1.0f, 0.0f, 0.0f, alphaSolid );
		const decColor colorWire( colorSolid.r, colorSolid.g, colorSolid.b, alphaWire );
		
		for( i=0; i<propFieldCount; i++ ){
			const deoglCollideListPropField &clPropField = *collideList.GetPropFieldAt( i );
			deoglRPropField &propField = *clPropField.GetPropField();
			const decDVector propFieldOffset = propField.GetPosition();
			const int typeCount = clPropField.GetTypeCount();
			
			for( j=0; j<typeCount; j++ ){
				if( j == typeIndex ){
					const deoglCollideListPropFieldType &cltype = *clPropField.GetTypeAt( j );
					const int clusterCount = cltype.GetClusterCount();
					
					for( k=0; k<clusterCount; k++ ){
						const deoglPropFieldCluster &cluster = *cltype.GetClusterAt( k ).GetCluster();
						
						box.SetFromExtends( propFieldOffset + decDVector( cluster.GetMinimumExtend() ),
							propFieldOffset + decDVector( cluster.GetMaximumExtend() ) );
						
						shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
							* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
						
						shader->SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
						shapeBox.RenderFaces();
						
						shader->SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
						shapeBox.RenderLines();
					}
				}
			}
		}
	}
}



void deoglRenderDevMode::RenderLightInfos( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const decDMatrix &matrixVP = plan.GetCameraMatrix() * decDMatrix( plan.GetProjectionMatrix() );
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShapeBox &shapeBox = *( ( deoglShapeBox* )shapeManager.GetShapeAt( deoglRTBufferObject::esBox ) );
	deoglShapeCylinder &shapeCylinder = *( ( deoglShapeCylinder* )renderThread.GetBufferObject().GetShapeManager().GetShapeAt( deoglRTBufferObject::esCylinder ) );
	const deoglCollideList &collideList = plan.GetCollideList();
	const int lightCount = collideList.GetLightCount();
	const deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const float alphaSolid = 0.1f;
	const float alphaWire = 1.0f;
	deoglDCollisionBox box;
	int l;
	
	pPipelineSolidColor3D->Activate();
	deoglShaderCompiled *shader = pPipelineSolidColor3D->GetGlShader()->GetCompiled();
	
	shapeBox.ActivateVAO();
	
	if( devMode.GetShowLightBox() ){
		const decColor colorSolid( 1.0f, 0.0f, 1.0f, alphaSolid );
		const decColor colorWire( colorSolid.r, colorSolid.g, colorSolid.b, alphaWire );
		
		for( l=0; l<lightCount; l++ ){
			deoglRLight &light = *collideList.GetLightAt( l )->GetLight();
			
			box.SetFromExtends( light.GetMinimumExtend(), light.GetMaximumExtend() );
			
			shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
				* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
			
			shader->SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
			shapeBox.RenderFaces();
			
			shader->SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
			shapeBox.RenderLines();
		}
	}
	
	if( devMode.GetShowLightFullBox() ){
		const decColor colorSolid( 1.0f, 0.5f, 0.0f, alphaSolid );
		const decColor colorWire( colorSolid.r, colorSolid.g, colorSolid.b, alphaWire );
		
		for( l=0; l<lightCount; l++ ){
			deoglRLight &light = *collideList.GetLightAt( l )->GetLight();
			
			box.SetFromExtends( light.GetFullMinExtend(), light.GetFullMaxExtend() );
			
			shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
				* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
			
			shader->SetParameterFloat( spsc3dColor, colorSolid.r, colorSolid.g, colorSolid.b, colorSolid.a );
			shapeBox.RenderFaces();
			
			shader->SetParameterFloat( spsc3dColor, colorWire.r, colorWire.g, colorWire.b, colorWire.a );
			shapeBox.RenderLines();
		}
	}
	
	pPipelineShape->Activate();
	shader = pPipelineShape->GetGlShader()->GetCompiled();
	
	if( devMode.GetShowLightVolume() ){
		shader->SetParameterFloat( spsSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
		
		for( l=0; l<lightCount; l++ ){
			const deoglCollideListLight &cllight = *collideList.GetLightAt( l );
			const deoglRLight &light = *cllight.GetLight();
			const deoglLightVolume &lightVolume = *light.GetLightVolume();
			
			pglBindVertexArray( lightVolume.GetVAO() );
			shader->SetParameterDMatrix4x4( spsMatrixMVP, light.GetMatrix() * matrixVP );
			shader->SetParameterDMatrix4x4( spsMatrixMVP2, light.GetMatrix() * matrixVP );
			
			if( cllight.GetCameraInside() ){
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.0f, 0.0f, 0.01f ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) );
				
			}else{
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.5f, 0.0f, 0.01f ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.5f, 0.0f, 1.0f ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) );
			}
		}
	}
	
	if( devMode.GetShowLightVisualInfo() >= 0 && devMode.GetShowLightVisualInfo() < collideList.GetLightCount() ){
		const decQuaternion orientationCylinder = decMatrix::CreateRotationX( DEG2RAD * 90.0f ).ToQuaternion();
		const deoglCollideListLight &cllight = *collideList.GetLightAt( devMode.GetShowLightVisualInfo() );
		const deoglRLight &light = *cllight.GetLight();
		decMatrix matrix1, matrix2;
		decDMatrix matrixMVP;
		
		if( light.GetLightType() == deLight::eltSpot ){
			const deoglComponentSet &listStatic = light.GetStaticComponentList();
			const deoglComponentSet &listDynamic = light.GetDynamicComponentList();
			const float cutOffDist = light.GetRange();
			int i, count;
			
			// static solid components
			shapeBox.ActivateVAO();
			
			count = listStatic.GetCount();
			for( i=0; i<count; i++ ){
				deoglRComponent &component = *listStatic.GetAt( i );
				
				if( component.GetSolid() ){
					box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
					
					shader->SetParameterFloat( spsSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
					shader->SetParameterDMatrix4x4( spsMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
						* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
					shader->SetParameterDMatrix4x4( spsMatrixMVP2, matrixMVP );
					
					shader->SetParameterColor4( spsColor, decColor( 0.0f, 0.5f, 0.0f, 0.1f ) );
					shapeBox.RenderFaces();
					
					shader->SetParameterColor4( spsColor, decColor( 0.0f, 0.5f, 0.0f, 1.0f ) );
					shapeBox.RenderLines();
				}
			}
			
			// static transparent components
			for( i=0; i<count; i++ ){
				deoglRComponent &component = *listStatic.GetAt( i );
				
				if( ! component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow() ){
					box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
					
					shader->SetParameterFloat( spsSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
					shader->SetParameterDMatrix4x4( spsMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
						* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
					shader->SetParameterDMatrix4x4( spsMatrixMVP2, matrixMVP );
					
					shader->SetParameterColor4( spsColor, decColor( 0.0f, 1.0f, 0.0f, 0.1f ) );
					shapeBox.RenderFaces();
					
					shader->SetParameterColor4( spsColor, decColor( 0.0f, 1.0f, 0.0f, 1.0f ) );
					shapeBox.RenderLines();
				}
			}
			
			// dynamic components
			count = listDynamic.GetCount();
			for( i=0; i<count; i++ ){
				deoglRComponent &component = *listDynamic.GetAt( i );
				
				//if( component.GetSolid() ){
					box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
					
					shader->SetParameterFloat( spsSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
					shader->SetParameterDMatrix4x4( spsMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
						* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
					shader->SetParameterDMatrix4x4( spsMatrixMVP2, matrixMVP );
					
					shader->SetParameterColor4( spsColor, decColor( 0.0f, 0.5f, 1.0f, 0.1f ) );
					shapeBox.RenderFaces();
					
					shader->SetParameterColor4( spsColor, decColor( 0.0f, 0.5f, 1.0f, 1.0f ) );
					shapeBox.RenderLines();
				//}
			}
			
			// spot light cone
			shapeCylinder.ActivateVAO();
			
			shapeCylinder.CalcMatrices( matrix1, matrix2, decVector( 0.0f, 0.0f, cutOffDist * 0.5f ),
				orientationCylinder, cutOffDist * 0.5f, 0.0f,
				sinf( light.GetSpotAngle() * 0.5f ) * cutOffDist * 2.0f );
			
			matrixMVP = light.GetMatrix() * matrixVP;
			
			shader->SetParameterFloat( spsSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
			shader->SetParameterDMatrix4x4( spsMatrixMVP, decDMatrix( matrix1 ) * matrixMVP );
			shader->SetParameterDMatrix4x4( spsMatrixMVP2, decDMatrix( matrix2 ) * matrixMVP );
			
			if( cllight.GetCameraInside() ){
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.0f, 0.0f, 0.1f ) );
				shapeCylinder.RenderFaces();
				
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
				shapeCylinder.RenderLines();
				
			}else{
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.5f, 0.0f, 0.1f ) );
				shapeCylinder.RenderFaces();
				
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.5f, 0.0f, 1.0f ) );
				shapeCylinder.RenderLines();
			}
		}
	}
	
	pglBindVertexArray( 0 );
}



void deoglRenderDevMode::RenderEnvMapInfo( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const decDMatrix &matrixV = plan.GetCameraMatrix();
	const decDMatrix &matrixVP = matrixV * decDMatrix( plan.GetProjectionMatrix() );
	const deoglEnvironmentMapList &list = plan.GetWorld()->GetEnvMapList();
	const deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const int envmapCount = list.GetCount();
	const deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShapeBox &shapeBox = *( ( deoglShapeBox* )shapeManager.GetShapeAt( deoglRTBufferObject::esBox ) );
	int i;
	
	if( devMode.GetShowEnvMaps() ){
		const float alphaSolid = 0.1f;
		const float alphaWire = 1.0f;
		const decColor colorPosition( 1.0f, 0.0f, 0.0f );
		const decColor colorInfluence( 1.0f, 0.0f, 0.0f );
		const decColor colorReflection( 0.0f, 1.0f, 0.0f );
		const decColor colorMask( 0.0f, 0.0f, 1.0f );
		deoglDCollisionBox box;
		
		pPipelineSolidColor3D->Activate();
		deoglShaderCompiled *shader = pPipelineSolidColor3D->GetGlShader()->GetCompiled();
		
		shapeBox.ActivateVAO();
		
		for( i=0; i<envmapCount; i++ ){
			const deoglEnvironmentMap &envmap = *list.GetAt( i );
			if( envmap.GetSkyOnly() ){
				continue;
			}
			
			// position
			box.SetCenter( envmap.GetPosition() );
			box.SetHalfSize( decDVector( 0.02f, 0.02f, 0.02f ) );
			
			shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, decDMatrix::CreateScale( box.GetHalfSize() )
				* decDMatrix::CreateTranslation( box.GetCenter() ) * matrixVP );
			
			shader->SetParameterFloat( spsc3dColor, colorPosition.r, colorPosition.g, colorPosition.b, alphaSolid );
			shapeBox.RenderFaces();
			
			shader->SetParameterFloat( spsc3dColor, colorPosition.r, colorPosition.g, colorPosition.b, alphaWire );
			shapeBox.RenderLines();
			
			// influence box
			if( envmap.GetHasInfluenceBox() ){
				shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, envmap.GetMatrixInfluenceBox() * matrixVP );
				
				shader->SetParameterFloat( spsc3dColor, colorInfluence.r, colorInfluence.g, colorInfluence.b, alphaSolid );
				shapeBox.RenderFaces();
				
				shader->SetParameterFloat( spsc3dColor, colorInfluence.r, colorInfluence.g, colorInfluence.b, alphaWire );
				shapeBox.RenderLines();
			}
			
			// reflection shape
			if( envmap.GetHasReflectionBox() ){
				shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, envmap.GetMatrixReflectionBox() * matrixVP );
				
				shader->SetParameterFloat( spsc3dColor, colorReflection.r, colorReflection.g, colorReflection.b, alphaSolid );
				shapeBox.RenderFaces();
				
				shader->SetParameterFloat( spsc3dColor, colorReflection.r, colorReflection.g, colorReflection.b, alphaWire );
				shapeBox.RenderLines();
			}
			
			// mask shapes
			const int maskCount = envmap.GetReflectionMaskBoxMatrixCount();
			int j;
			for( j=0; j<maskCount; j++ ){
				shader->SetParameterDMatrix4x4( spsc3dMatrixMVP, envmap.GetReflectionMaskBoxMatrixAt( j ) * matrixVP );
				
				shader->SetParameterFloat( spsc3dColor, colorMask.r, colorMask.g, colorMask.b, alphaSolid );
				shapeBox.RenderFaces();
				
				shader->SetParameterFloat( spsc3dColor, colorMask.r, colorMask.g, colorMask.b, alphaWire );
				shapeBox.RenderLines();
			}
		}
	}
	
	// hull
	if( devMode.GetShowEnvMapHull() ){
		pPipelineShape->Activate();
		deoglShaderCompiled *shader = pPipelineShape->GetGlShader()->GetCompiled();
		
		double closestDist = 0.0;
		int closestEnvMap = -1;
		
		shader->SetParameterFloat( spsSCToDTC, defren.GetPixelSizeU(), defren.GetPixelSizeV() );
		
		for( i=0; i<envmapCount; i++ ){
			deoglEnvironmentMap &envmap = *list.GetAt( i );
			const decDVector lpos = matrixV * envmap.GetPosition();
			
			if( lpos.z > 0.0 && fabs( lpos.x ) < lpos.z && fabs( lpos.y ) < lpos.z ){
				const double dist = lpos.Length();
				
				if( closestEnvMap == -1 || dist < closestDist ){
					closestEnvMap = i;
					closestDist = dist;
				}
			}
		}
		
		for( i=0; i<envmapCount; i++ ){
			deoglEnvironmentMap &envmap = *list.GetAt( i );
			const deoglLightVolume &lightVolume = *envmap.GetLightVolume();
			const decDMatrix matrix = decDMatrix::CreateTranslation( envmap.GetPosition() ) * matrixVP;
			
			pglBindVertexArray( lightVolume.GetVAO() );
			shader->SetParameterDMatrix4x4( spsMatrixMVP, matrix );
			shader->SetParameterDMatrix4x4( spsMatrixMVP2, matrix );
			
			if( i == closestEnvMap ){
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.0f, 0.0f, 0.05f ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) );
				
			}else{
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.5f, 0.0f, 0.05f ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				
				shader->SetParameterColor4( spsColor, decColor( 1.0f, 0.5f, 0.0f, 1.0f ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, lightVolume.GetPointCount() ) );
				OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) );
			}
		}
	}
	
	pglBindVertexArray( 0 );
}



void deoglRenderDevMode::RenderOverlayInfos( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	decPoint position, size;
	bool hasOverlay = false;
	const int vgap = 5;
	
	// determine first if we have any overlay information to show
	if( devMode.GetShowHeightTerrain() ){
		hasOverlay = true;
	}
	if( devMode.GetShowTranspLevelCount() ){
		hasOverlay = true;
	}
	if( devMode.GetShowMemoryInfo() ){
		hasOverlay = true;
	}
	if( plan.GetDebug() ){
		hasOverlay = true;
	}
	if( devMode.GetShowOccMapLevel() > -1 ){
		hasOverlay = true;
	}
	if( devMode.GetTextureDebugImage() ){
		hasOverlay = true;
	}
	if( devMode.GetShowDebugInfo() ){
		hasOverlay = true;
	}
	
	// if there are no overlay information leave now
	if( ! hasOverlay ){
		return;
	}
	
	// overwise prepare for rendering the overlay information
	OGL_CHECK( renderThread, pglBindVertexArray( pVAOShapes ) );
	
	pScalePosition.x = 2.0f / ( float )plan.GetViewportWidth();
	pScalePosition.y = -2.0f / ( float )plan.GetViewportHeight();
	pOffsetPosition.x = pScalePosition.x * 0.375f - 1.0f;
	pOffsetPosition.y = pScalePosition.y * 0.375f + 1.0f;
	
	position.x = 5;
	position.y = 30; //5;
	
	if( devMode.GetShowOccMapLevel() > -1 && plan.GetOcclusionMap() ){
		RenderOccMapLevel( plan );
	}
	
	if( devMode.GetTextureDebugImage() ){
		GetRenderThread().GetRenderers().GetDebug().DisplayTexture( plan, devMode.GetTextureDebugImage(), false );
	}
	
	// render every visible overlay information stacked one below the other
	if( devMode.GetShowTranspLevelCount() ){
		RenderTraspLevelCount( plan, position, size );
		position.y += size.y + vgap;
	}
	
	if( devMode.GetShowHeightTerrain() ){
		RenderHeightTerrainLODLevels( plan, position, size );
		position.y += size.y + vgap;
	}
	
	if( devMode.GetShowMemoryInfo() ){
		RenderMemoryInfo( plan, position, size );
		position.y += size.y + vgap;
	}
	
	if( plan.GetDebug() ){
		RenderRenderPlanDebugInfo( plan, position, size );
		position.y += size.y + vgap;
	}
	
	if( devMode.GetShowDebugInfo() ){
		RenderDebugInformation( plan, position, size );
		if( devMode.GetDebugInfoLog() ){
			LogDebugInformation();
		}
		position.y += size.y + vgap;
	}
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderDevMode::RenderOccMapLevel( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglOcclusionMap &occmap = *plan.GetOcclusionMap();
	deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	const int occMapBaseLevel = plan.GetOcclusionMapBaseLevel();
	//const int occMapBaseLevel = 0;
	const int occMapMaxLevel = occmap.GetLevelCount();
	int occMapLevel = occMapBaseLevel + devMode.GetShowOccMapLevel();
	
	if( occMapLevel > occMapMaxLevel ){
		occMapLevel = occMapMaxLevel;
	}
	
	GetRenderThread().GetRenderers().GetDebug().DisplayArrayTextureLayerLevel( plan, occmap.GetTexture(), 0, occMapLevel, false );
}

void deoglRenderDevMode::RenderHeightTerrainLODLevels( deoglRenderPlan &plan, const decPoint &position, decPoint &size ){
	deoglHTView *htview = plan.GetHeightTerrainView();
	if( ! htview ) return;
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglRHeightTerrain &heightTerrain = htview->GetHeightTerrain();
	const int sectorCount = heightTerrain.GetSectorCount();
	const decDVector &campos = plan.GetCameraPosition();
	decVector camrelpos, boxmin, boxmax;
	decPoint fromSector, toSector;
	int x, z, lodLevel;
	int x1, x2, y1, y2;
	int posx;
	int posy;
	int ci;
	int s;
	
	size.Set( 0, 0 );
	
	// determine the render parementers
	fromSector.x = 0;
	fromSector.y = 0;
	toSector.x = 0;
	toSector.y = 0;
	
	// shader
	pPipelineSolidColor2D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor2D->GetGlShader()->GetCompiled();
	
	// render sectors falling into the range
	for( s=0; s<sectorCount; s++ ){
		const deoglRHTSector &htsector = heightTerrain.GetSectorAt( s );
		
		if( htsector.GetValid() ){
			const decPoint &scoord = htsector.GetCoordinates();
			
			if( scoord.x >= fromSector.x && scoord.y >= fromSector.y && scoord.x <= toSector.x && scoord.y <= toSector.y ){
				const deoglHTViewSector &htvsector = *htview->GetSectorAt( s );
				const deoglHTSCluster * const htclusters = htsector.GetClusters();
				const int clusterCount = htsector.GetClusterCount();
				
				camrelpos = campos.ToVector(); // TODO adjust to sector
				
				// render an overlay graphic with dots for lod levels
				x1 = position.x;
				y1 = position.y;
				x2 = position.x + clusterCount * 2 + 11;
				y2 = position.y + clusterCount * 2 + 11;
				shader.SetParameterFloat( spsc2dColor, 0.0f, 0.0f, 0.0f, 1.0f );
				shader.SetParameterFloat( spsc2dPosTransform, SCALEX( x2 - x1 ), SCALEY( y2 - y1 ), POSX( x1 ), POSY( y1 ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
				
				x1 = position.x + 5;
				y1 = position.y + 5;
				x2 = position.x + 5 + clusterCount * 2 + 1;
				y2 = position.y + 5 + clusterCount * 2 + 1;
				shader.SetParameterFloat( spsc2dColor, 0.75f, 0.75f, 0.75f, 1.0f );
				shader.SetParameterFloat( spsc2dPosTransform, SCALEX( x2 - x1 ), SCALEY( y2 - y1 ), POSX( x1 ), POSY( y1 ) );
				OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
				
				size.x = clusterCount * 2 + 11;
				size.y = clusterCount * 2 + 11;
				
				ci = 0;
				for( z=0; z<clusterCount; z++ ){
					for( x=0; x<clusterCount; x++ ){
						lodLevel = htvsector.GetClusterAt( ci ).GetLodLevel();
						
						if( lodLevel < 0 ){
							shader.SetParameterFloat( spsc2dColor, 0.25f, 0.25f, 0.25f, 1.0f );
							
						}else if( lodLevel == 0 ){
							shader.SetParameterFloat( spsc2dColor, 1.0f, 0.0f, 0.0f, 1.0f );
							
						}else if( lodLevel == 1 ){
							shader.SetParameterFloat( spsc2dColor, 1.0f, 1.0f, 0.0f, 1.0f );
							
						}else if( lodLevel == 2 ){
							shader.SetParameterFloat( spsc2dColor, 0.0f, 1.0f, 1.0f, 1.0f );
							
						}else if( lodLevel == 3 ){
							shader.SetParameterFloat( spsc2dColor, 0.0f, 1.0f, 0.0f, 1.0f );
							
						}else if( lodLevel == 4 ){
							shader.SetParameterFloat( spsc2dColor, 1.0f, 0.0f, 1.0f, 1.0f );
							
						}else{
							shader.SetParameterFloat( spsc2dColor, 0.0f, 0.0f, 1.0f, 1.0f );
						}
						
						posx = position.x + 6 + x * 2;
						posy = position.y + 6 + z * 2;
						shader.SetParameterFloat( spsc2dPosTransform, 0.0f, 0.0f, POSX( posx ), POSY( posy ) );
						OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, OFFSET_POINT, COUNT_POINT ) );
						
						boxmin = htclusters[ ci ].GetCenter() - htclusters[ ci ].GetHalfExtends();
						boxmax = htclusters[ ci ].GetCenter() + htclusters[ ci ].GetHalfExtends();
						
						if( camrelpos.x >= boxmin.x && camrelpos.x < boxmax.x && camrelpos.z >= boxmin.z && camrelpos.z < boxmax.z ){
							x1 = posx - 1;
							y1 = posy - 1;
							x2 = posx + 1;
							y2 = posy + 1;
							shader.SetParameterFloat( spsc2dColor, 0.0f, 0.5f, 0.5f, 1.0f );
							shader.SetParameterFloat( spsc2dPosTransform, SCALEX( x2 - x1 ), SCALEY( y2 - y1 ), POSX( x1 ), POSY( y1 ) );
							OGL_CHECK( renderThread, glDrawArrays( GL_LINE_LOOP, OFFSET_RECT, COUNT_RECT ) );
						}
						
						ci++;
					}
				}
			}
		}
	}
}

void deoglRenderDevMode::RenderTraspLevelCount( deoglRenderPlan &plan, const decPoint &position, decPoint &size ){
	deoglRenderThread &renderThread = GetRenderThread();
	const int transpLevelCount = plan.GetTransparencyLayerCount();
	const decColor clrBorder( 0.0f, 0.0f, 0.0f );
	const decColor clrDotOn( 1.0f, 0.0f, 0.0f );
	const decColor clrDotOff( 0.25f, 0.25f, 0.25f );
	const float transparency = 0.85f;
	const int dotWidth = 6;
	const int dotHeight = 10;
	int x1, x2, y1, y2;
	int p;
	
	pPipelineSolidColor2D->Activate();
	deoglShaderCompiled &shader = *pPipelineSolidColor2D->GetGlShader()->GetCompiled();
	
	for( p=0; p<100; p++ ){
		x1 = position.x + dotWidth * p;
		y1 = position.y;
		x2 = x1 + dotWidth - 1;
		y2 = y1 + dotHeight - 1;
		shader.SetParameterFloat( spsc2dColor, clrBorder.r, clrBorder.g, clrBorder.b, transparency );
		shader.SetParameterFloat( spsc2dPosTransform, SCALEX( x2 - x1 ), SCALEY( y2 - y1 ), POSX( x1 ), POSY( y1 ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_LINE_LOOP, OFFSET_RECT, COUNT_RECT ) );
		
		x1++;
		y1++;
		if( p < transpLevelCount ){
			shader.SetParameterFloat( spsc2dColor, clrDotOn.r, clrDotOn.g, clrDotOn.b, transparency );
			
		}else{
			shader.SetParameterFloat( spsc2dColor, clrDotOff.r, clrDotOff.g, clrDotOff.b, transparency );
		}
		shader.SetParameterFloat( spsc2dPosTransform, SCALEX( x2 - x1 ), SCALEY( y2 - y1 ), POSX( x1 ), POSY( y1 ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
	}
	
	size.x = p * dotWidth;
	size.y = dotHeight;
}

void deoglRenderDevMode::RenderRenderPlanDebugInfo( deoglRenderPlan &plan, const decPoint &position, decPoint &size ){
	const decColor color1 = decColor( 1.0f, 1.0f, 1.0f );
	const deoglRenderPlanDebug &debug = *plan.GetDebug();
	deoglRenderDebug &renderDebug = GetRenderThread().GetRenderers().GetDebug();
	const int fontHeight = renderDebug.GetDebugFont().GetFontHeight();
	int y = position.y;
	int maxWidth = 0;
	decString text;
	
	renderDebug.BeginRenderText();
	
	/*
	"View:    O %5i       | T %7i       | L %3d       | E %3d"
	"CullPS:  O %5i(%3d%%) | T %7i(%3d%%) | L %3d(%3d%%) | S %4d"
	"Outside: O %5i(%3d%%) | T %7i(%3d%%)"
	"OccMap:  O %5i | T %7i | I %5i | Rt %2d.%1ims | Tt %2d.%1ims | Ot %2d.%1ims"
	"LOD:     O %5i(%3d%%) | T %7i(%3d%%)"
	"Render:  O %5i(%3d%%) | T %7i(%3d%%) | L %3d(%3d%%) | E %3d | P %2d"
	*/
	// information about visible objects in the view before any culling is done
	const int viewObjects = debug.GetViewObjects();
	const int viewTriangles = debug.GetViewTriangles();
	const int viewLights = debug.GetViewLights();
	const int viewEnvMaps = debug.GetViewEnvMaps();
	
	text.Format( "View:    O %5i       | T %7i       | L %3d       | E %3d", viewObjects, viewTriangles, viewLights, viewEnvMaps );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// information about visible objects in the view after culling
	const int cullPSObjects = debug.GetCullPSObjects();
	const int cullPSTriangles = debug.GetCullPSTriangles();
	const int cullPSLights = debug.GetCullPSLights();
	double cullPSObjectsRatio = 0.0;
	double cullPSTrianglesRatio = 0.0;
	double cullPSLightsRatio = 0.0;
	
	if( viewObjects > 0 ){
		cullPSObjectsRatio = 100.0 * ( ( double )cullPSObjects / ( double )viewObjects );
	}
	if( viewTriangles > 0 ){
		cullPSTrianglesRatio = 100.0 * ( ( double )cullPSTriangles / ( double )viewTriangles );
	}
	if( viewLights > 0 ){
		cullPSLightsRatio = 100.0 * ( ( double )cullPSLights / ( double )viewLights );
	}
	
	text.Format( "CullPS:  O %5i(%3d%%) | T %7i(%3d%%) | L %3d(%3d%%)", cullPSObjects,
		( int )cullPSObjectsRatio, cullPSTriangles, ( int )cullPSTrianglesRatio, cullPSLights,
		( int )cullPSLightsRatio );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// occlusion testing
	const int occmapObjects = debug.GetOccMapObjects();
	const int occmapTriangles = debug.GetOccMapTriangles();
	const float occmapRenderTime = debug.GetOccMapRenderTime();
	const float occTestTime = debug.GetOccTestTime();
	const float occTotalTime = occmapRenderTime + occTestTime;
	const int occTestCount = debug.GetOccTestCount();
	
	text.Format( "OccMap:  O %5i | T %7i | I %5i | Rt %2d.%1ims | Tt %2d.%1ims | Ot %2d.%1ims",
		occmapObjects, occmapTriangles, occTestCount,
		( int )( occmapRenderTime * 1000.0f ), ( int )( occmapRenderTime * 10000.0f ) % 10,
		( int )( occTestTime * 1000.0f ), ( int )( occTestTime * 10000.0f ) % 10,
		( int )( occTotalTime * 1000.0f ), ( int )( occTotalTime * 10000.0f ) % 10 );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// actually rendered elements
	
	const int renderObjects = debug.GetRenderedObjects();
	const int renderTriangles = debug.GetRenderedTriangles();
	const int renderLights = debug.GetRenderedLights();
	const int renderPasses = debug.GetRenderedPasses();
	const int renderEnvMaps = debug.GetRenderedEnvMaps();
	const int lodTriangles = debug.GetLODTriangles();
	double renderObjectsRatio = 0.0;
	double renderTrianglesRatio = 0.0;
	double renderLightsRatio = 0.0;
	double lodTrianglesRatio = 0.0;
	
	if( viewTriangles > 0 ){
		lodTrianglesRatio = 100.0 * ( ( double )renderTriangles / ( double )lodTriangles );
	}
	
	text.Format( "LOD:     O       (   ) | T %7i(%3d%%)", lodTriangles, ( int )lodTrianglesRatio );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	if( viewObjects > 0 ){
		renderObjectsRatio = 100.0 * ( ( double )renderObjects / ( double )viewObjects );
	}
	if( viewTriangles > 0 ){
		renderTrianglesRatio = 100.0 * ( ( double )renderTriangles / ( double )viewTriangles );
	}
	if( viewLights > 0 ){
		renderLightsRatio = 100.0 * ( ( double )renderLights / ( double )viewLights );
	}
	
	text.Format( "Render:  O %5i(%3d%%) | T %7i(%3d%%) | L %3d(%3d%%) | E %3d | P %2d", renderObjects,
		( int )renderObjectsRatio, renderTriangles, ( int )renderTrianglesRatio, renderLights,
		( int )renderLightsRatio, ( int )renderEnvMaps, renderPasses );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	renderDebug.EndRenderText();
	
	// finished
	size.x = maxWidth;
}

void deoglRenderDevMode::RenderMemoryInfo( deoglRenderPlan &plan, const decPoint &position, decPoint &size ){
	const decColor color1 = decColor( 1.0f, 1.0f, 1.0f );
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglMemoryConsumption &consumption = renderThread.GetMemoryManager().GetConsumption();
	deoglRenderDebug &renderDebug = GetRenderThread().GetRenderers().GetDebug();
	const int fontHeight = renderDebug.GetDebugFont().GetFontHeight();
	int y = position.y;
	int maxWidth = 0;
	decString text;
	
	const char * const fmtTex2D  = "Tex2D (%4d): %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M | C(%4d) %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M | D(%3d) %3" OGLPFLLU "M";
	const char * const fmtTexArr = "TexArr(%4d): %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M | C(%4d) %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M | D(%3d) %3" OGLPFLLU "M";
	const char * const fmtCube   = "Cube  (%4d): %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M | C(%4d) %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M | D(%3d) %3" OGLPFLLU "M";
	const char * const fmtRenBuf = "RenBuf(%4d): %4" OGLPFLLU "M                 | C(%4d) %4" OGLPFLLU "M                 | D(%3d) %3" OGLPFLLU "M";
	const char * const fmtSkin   = "Skins (%4d): %4" OGLPFLLU "M %4" OGLPFLLU "M(%2d%%) %3" OGLPFLLU "M";
	const char * const fmtRender = "Renderables :  2D-C(%2d) %3" OGLPFLLU "M | 2D-D(%2d) %3" OGLPFLLU "M | Arr-C(%2d) %3" OGLPFLLU "M | Arr-D(%2d) %3" OGLPFLLU "M";
	const char * const fmtVBO    = "VBO   (%4d): %4uM | S(%4d) %4uM | I(%4d) %4uM | S(%4d) %4uM | T(%4d) %4uM";
	const char * const fmtDefRen = "DefRen      : %3uM | T %3uM | R %3uM";
	
	// textures 2d
	const deoglMemoryConsumptionTexture &consumptionTexture2D = consumption.texture2D;
	const int textureCount = consumptionTexture2D.all.GetCount();
	const int textureColorCount = consumptionTexture2D.color.GetCount();
	const int textureDepthCount = consumptionTexture2D.depth.GetCount();
	unsigned long long textureGPU = consumptionTexture2D.all.GetConsumption();
	unsigned long long textureGPUCompressed = consumptionTexture2D.allCompressed.GetConsumption();
	unsigned long long textureGPUUncompressed = consumptionTexture2D.allUncompressed.GetConsumption();
	unsigned long long textureColorGPU = consumptionTexture2D.color.GetConsumption();
	unsigned long long textureColorGPUCompressed = consumptionTexture2D.colorCompressed.GetConsumption();
	unsigned long long textureColorGPUUncompressed = consumptionTexture2D.colorUncompressed.GetConsumption();
	unsigned long long textureDepthGPU = consumptionTexture2D.depth.GetConsumption();
	//unsigned long long textureDepthGPUCompressed = consumptionTexture2D.depthCompressed.GetConsumption();
	//unsigned long long textureDepthGPUUncompressed = consumptionTexture2D.depthUncompressed.GetConsumption();
	double textureRatioCompressed = 0.0;
	double textureColorRatioCompressed = 0.0;
	//double textureDepthRatioCompressed = 0.0;
	
	if( textureGPU > 0 ){
		textureRatioCompressed = 100.0 * ( ( double )textureGPUCompressed / ( double )textureGPU );
		if( textureRatioCompressed > 99 ){
			textureRatioCompressed = 99; // just so we can use at most 2 digits
		}
	}
	if( textureColorGPU > 0 ){
		textureColorRatioCompressed = 100.0 * ( ( double )textureColorGPUCompressed / ( double )textureColorGPU );
		if( textureColorRatioCompressed > 99 ){
			textureColorRatioCompressed = 99; // just so we can use at most 2 digits
		}
	}
	//if( textureDepthGPU > 0 ){
	//	textureDepthRatioCompressed = 100.0 * ( ( double )textureDepthGPUCompressed / ( double )textureDepthGPU );
	//}
	
	textureGPU /= 1000000ull;
	textureGPUCompressed /= 1000000ull;
	textureGPUUncompressed /= 1000000ull;
	textureColorGPU /= 1000000ull;
	textureColorGPUCompressed /= 1000000ull;
	textureColorGPUUncompressed /= 1000000ull;
	textureDepthGPU /= 1000000ull;
	//textureDepthGPUCompressed /= 1000000ull;
	//textureDepthGPUUncompressed /= 1000000ull;
	
	renderDebug.BeginRenderText();
	
	text.Format( fmtTex2D, textureCount, textureGPU, textureGPUCompressed, ( int )textureRatioCompressed,
		textureGPUUncompressed, textureColorCount, textureColorGPU, textureColorGPUCompressed,
		( int )textureColorRatioCompressed, textureColorGPUUncompressed, textureDepthCount, textureDepthGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// textures array
	const deoglMemoryConsumptionTexture &consumptionTextureArray = consumption.textureArray;
	const int textureArrayCount = consumptionTextureArray.all.GetCount();
	const int textureArrayColorCount = consumptionTextureArray.color.GetCount();
	const int textureArrayDepthCount = consumptionTextureArray.depth.GetCount();
	unsigned long long textureArrayGPU = consumptionTextureArray.all.GetConsumption();
	unsigned long long textureArrayGPUCompressed = consumptionTextureArray.allCompressed.GetConsumption();
	unsigned long long textureArrayGPUUncompressed = consumptionTextureArray.allUncompressed.GetConsumption();
	unsigned long long textureArrayColorGPU = consumptionTextureArray.color.GetConsumption();
	unsigned long long textureArrayColorGPUCompressed = consumptionTextureArray.colorCompressed.GetConsumption();
	unsigned long long textureArrayColorGPUUncompressed = consumptionTextureArray.colorUncompressed.GetConsumption();
	unsigned long long textureArrayDepthGPU = consumptionTextureArray.depth.GetConsumption();
	//unsigned long long textureArrayDepthGPUCompressed = consumptionTextureArray.depthCompressed.GetConsumption();
	//unsigned long long textureArrayDepthGPUUncompressed = consumptionTextureArray.depthUncompressed.GetConsumption();
	double textureArrayRatioCompressed = 0.0;
	double textureArrayColorRatioCompressed = 0.0;
	//double textureArrayDepthRatioCompressed = 0.0;
	
	if( textureArrayGPU > 0 ){
		textureArrayRatioCompressed = 100.0 * ( ( double )textureArrayGPUCompressed / ( double )textureArrayGPU );
		if( textureArrayRatioCompressed > 99 ){
			textureArrayRatioCompressed = 99; // just so we can use at most 2 digits
		}
	}
	if( textureArrayColorGPU > 0 ){
		textureArrayColorRatioCompressed = 100.0 * ( ( double )textureArrayColorGPUCompressed / ( double )textureArrayColorGPU );
		if( textureArrayColorRatioCompressed > 99 ){
			textureArrayColorRatioCompressed = 99; // just so we can use at most 2 digits
		}
	}
	//if( textureArrayDepthGPU > 0 ){
	//	textureArrayDepthRatioCompressed = 100.0 * ( ( double )textureArrayDepthGPUCompressed / ( double )textureArrayDepthGPU );
	//}
	
	textureArrayGPU /= 1000000ull;
	textureArrayGPUCompressed /= 1000000ull;
	textureArrayGPUUncompressed /= 1000000ull;
	textureArrayColorGPU /= 1000000ull;
	textureArrayColorGPUCompressed /= 1000000ull;
	textureArrayColorGPUUncompressed /= 1000000ull;
	textureArrayDepthGPU /= 1000000ull;
	//textureArrayDepthGPUCompressed /= 1000000ull;
	//textureArrayDepthGPUUncompressed /= 1000000ull;
	
	text.Format( fmtTexArr, textureArrayCount, textureArrayGPU, textureArrayGPUCompressed, ( int )textureArrayRatioCompressed,
		textureArrayGPUUncompressed, textureArrayColorCount, textureArrayColorGPU, textureArrayColorGPUCompressed,
		( int )textureArrayColorRatioCompressed, textureArrayColorGPUUncompressed, textureArrayDepthCount, textureArrayDepthGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// cube maps
	const deoglMemoryConsumptionTexture &consumptionTextureCube = consumption.textureCube;
	const int cubemapCount = consumptionTextureCube.all.GetCount();
	const int cubemapColorCount = consumptionTextureCube.color.GetCount();
	const int cubemapDepthCount = consumptionTextureCube.depth.GetCount();
	unsigned long long cubemapGPU = consumptionTextureCube.all.GetConsumption();
	unsigned long long cubemapGPUCompressed = consumptionTextureCube.allCompressed.GetConsumption();
	unsigned long long cubemapGPUUncompressed = consumptionTextureCube.allUncompressed.GetConsumption();
	unsigned long long cubemapColorGPU = consumptionTextureCube.color.GetConsumption();
	unsigned long long cubemapColorGPUCompressed = consumptionTextureCube.colorCompressed.GetConsumption();
	unsigned long long cubemapColorGPUUncompressed = consumptionTextureCube.colorUncompressed.GetConsumption();
	unsigned long long cubemapDepthGPU = consumptionTextureCube.depth.GetConsumption();
	//unsigned long long cubemapDepthGPUCompressed = consumptionTextureCube.depthCompressed.GetConsumption();
	//unsigned long long cubemapDepthGPUUncompressed = consumptionTextureCube.depthUncompressed.GetConsumption();
	double cubemapRatioCompressed = 0.0f;
	double cubemapColorRatioCompressed = 0.0f;
	//double cubemapDepthRatioCompressed = 0.0f;
	
	if( cubemapGPU > 0 ){
		cubemapRatioCompressed = 100.0 * ( ( double )cubemapGPUCompressed / ( double )cubemapGPU );
		if( cubemapRatioCompressed > 99 ){
			cubemapRatioCompressed = 99; // just so we can use at most 2 digits
		}
	}
	if( cubemapColorGPU > 0 ){
		cubemapColorRatioCompressed = 100.0 * ( ( double )cubemapColorGPUCompressed / ( double )cubemapColorGPU );
		if( cubemapColorRatioCompressed > 99 ){
			cubemapColorRatioCompressed = 99; // just so we can use at most 2 digits
		}
	}
	//if( cubemapDepthGPU > 0 ){
	//	cubemapDepthRatioCompressed = 100.0 * ( ( double )cubemapDepthGPUCompressed / ( double )cubemapDepthGPU );
	//}
	
	cubemapGPU /= 1000000ull;
	cubemapGPUCompressed /= 1000000ull;
	cubemapGPUUncompressed /= 1000000ull;
	cubemapColorGPU /= 1000000ull;
	cubemapColorGPUCompressed /= 1000000ull;
	cubemapColorGPUUncompressed /= 1000000ull;
	cubemapDepthGPU /= 1000000ull;
	//cubemapDepthGPUCompressed /= 1000000ull;
	//cubemapDepthGPUUncompressed /= 1000000ull;
	
	text.Format( fmtCube, cubemapCount, cubemapGPU, cubemapGPUCompressed, ( int )cubemapRatioCompressed, cubemapGPUUncompressed,
		cubemapColorCount, cubemapColorGPU, cubemapColorGPUCompressed, ( int )cubemapColorRatioCompressed, cubemapColorGPUUncompressed,
		cubemapDepthCount, cubemapDepthGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// renderbuffer
	const deoglMemoryConsumptionRenderBuffer &consumptionRenderbuffer = consumption.renderbuffer;
	const int renderbufferCount = consumptionRenderbuffer.all.GetCount();
	const int renderbufferColorCount = consumptionRenderbuffer.color.GetCount();
	const int renderbufferDepthCount = consumptionRenderbuffer.depth.GetCount();
	unsigned long long renderbufferGPU = consumptionRenderbuffer.all.GetConsumption();
	unsigned long long renderbufferColorGPU = consumptionRenderbuffer.color.GetConsumption();
	unsigned long long renderbufferDepthGPU = consumptionRenderbuffer.depth.GetConsumption();
	
	renderbufferGPU /= 1000000ull;
	renderbufferColorGPU /= 1000000ull;
	renderbufferDepthGPU /= 1000000ull;
	
	text.Format( fmtRenBuf, renderbufferCount, renderbufferGPU, renderbufferColorCount, renderbufferColorGPU,
		renderbufferDepthCount, renderbufferDepthGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// skin memory consumption
	const deoglMemoryConsumptionSkin &consumptionSkin = consumption.skin;
	const int skinCount = consumptionSkin.all.GetCount();
	unsigned long long skinGPU = consumptionSkin.all.GetConsumption();
	unsigned long long skinGPUCompressed = consumptionSkin.allCompressed.GetConsumption();
	unsigned long long skinGPUUncompressed = consumptionSkin.allUncompressed.GetConsumption();
	double percentageCompressed = 0.0;
	
	if( skinGPU > 0 ){
		percentageCompressed = 100.0 * ( ( double )skinGPUCompressed / ( double )skinGPU );
		if( percentageCompressed > 99 ){
			percentageCompressed = 99; // just so we can use at most 2 digits
		}
	}
	
	skinGPU /= 1000000ull;
	skinGPUCompressed /= 1000000ull;
	skinGPUUncompressed /= 1000000ull;
	
	text.Format( fmtSkin, skinCount, skinGPU, skinGPUCompressed, ( int )percentageCompressed, skinGPUUncompressed );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// renderable memory consumption
	const int renderable2DColorCount = renderThread.GetTexture().GetRenderableColorTexture().GetTextureCount();
	const int renderable2DDepthCount = renderThread.GetTexture().GetRenderableDepthTexture().GetTextureCount();
	const int renderableArrayColorCount = renderThread.GetTexture().GetRenderableColorArrayTexture().GetCount();
	const int renderableArrayDepthCount = renderThread.GetTexture().GetRenderableDepthArrayTexture().GetCount();
	unsigned long long renderable2DColorGPU = 0; //memmgr.GetRenderable2DColorGPU();
	unsigned long long renderable2DDepthGPU = 0; //memmgr.GetRenderable2DDepthGPU();
	unsigned long long renderableArrayColorGPU = 0; //memmgr.GetRenderableArrayGPU();
	unsigned long long renderableArrayDepthGPU = 0; //memmgr.GetRenderableArrayGPU();
	
	renderable2DColorGPU /= 1000000ull;
	renderable2DDepthGPU /= 1000000ull;
	renderableArrayColorGPU /= 1000000ull;
	renderableArrayDepthGPU /= 1000000ull;
	
	text.Format( fmtRender, renderable2DColorCount, renderable2DColorGPU, renderable2DDepthCount,
		renderable2DDepthGPU, renderableArrayColorCount, renderableArrayColorGPU,
		renderableArrayDepthCount, renderableArrayDepthGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// vbo
	const deoglMemoryConsumptionBufferObject &consumptionBO = consumption.bufferObject;
	const int vboCount = consumptionBO.vbo.GetCount();
	const int vboSharedCount = consumptionBO.vboShared.GetCount();
	const int iboCount = consumptionBO.ibo.GetCount();
	const int iboSharedCount = consumptionBO.iboShared.GetCount();
	const int tboCount = consumptionBO.tbo.GetCount();
	unsigned int vboGPU = consumptionBO.vbo.GetConsumptionMB();
	unsigned int vboSharedGPU = consumptionBO.vboShared.GetConsumptionMB();
	unsigned int iboGPU = consumptionBO.ibo.GetConsumptionMB();
	unsigned int iboSharedGPU = consumptionBO.iboShared.GetConsumptionMB();
	unsigned int tboGPU = consumptionBO.tbo.GetConsumptionMB();
	
	text.Format( fmtVBO, vboCount, vboGPU, vboSharedCount, vboSharedGPU, iboCount,
		iboGPU, iboSharedCount, iboSharedGPU, tboCount, tboGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// deferred rendering system
	const deoglMemoryConsumptionDeferredRendering &consumptionDefren = consumption.deferredRendering;
	unsigned int defrenGPU = consumptionDefren.target.GetConsumptionMB();
	unsigned int defrenGPUTexture = consumptionDefren.texture.GetConsumptionMB();
	unsigned int defrenGPURenBuf = consumptionDefren.renderBuffer.GetConsumptionMB();
	
	text.Format( fmtDefRen, defrenGPU, defrenGPUTexture, defrenGPURenBuf );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	// grand total of all above
	unsigned long long totalGPU;
	
	totalGPU = consumptionTexture2D.all.GetConsumption();
	totalGPU += consumptionTextureArray.all.GetConsumption();
	totalGPU += consumptionTextureCube.all.GetConsumption();
	totalGPU += consumptionRenderbuffer.all.GetConsumption();
	totalGPU += consumptionBO.vbo.GetConsumption();
	totalGPU += consumptionBO.ibo.GetConsumption();
	totalGPU += consumptionBO.ubo.GetConsumption();
	totalGPU += consumptionBO.ssbo.GetConsumption();
	totalGPU /= 1000000ull;
	
	text.Format( "Total %4" OGLPFLLU "M", totalGPU );
	renderDebug.AddRenderText( plan, text.GetString(), position.x, y, color1 );
	size.y += fontHeight;
	y += fontHeight;
	
	renderDebug.EndRenderText();
	
	size.x = maxWidth;
}



void deoglRenderDevMode::RenderDebugInformation( deoglRenderPlan &plan,
const decPoint &position, decPoint &size ){
	const deoglDebugInformationList &list = GetRenderThread().GetDebug().GetDebugInformationList();
	const int count = list.GetCount();
	if( count == 0 ){
		return;
	}
	
	const int maxWidth = plan.GetViewportWidth() - position.x;
	
	LayoutDebugInformation( plan, decPoint(), size, list, 0, maxWidth, true );
	
	deoglRenderDebug &renderDebug = GetRenderThread().GetRenderers().GetDebug();
	renderDebug.BeginRenderRectangle();
	renderDebug.BeginRenderText();
	
	int i;
	for( i=0; i<count; i++ ){
		const deoglDebugInformation &debugInformation = *list.GetAt( i );
		if( debugInformation.GetVisible() ){
			RenderDebugInformation( plan, position, debugInformation );
		}
	}
	
	renderDebug.EndRenderRectangle();
	renderDebug.EndRenderText();
}

void deoglRenderDevMode::LogDebugInformation(){
	const deoglDebugInformationList &list = GetRenderThread().GetDebug().GetDebugInformationList();
	const int count = list.GetCount();
	if( count == 0 ){
		return;
	}
	
	deoglRTLogger &logger = GetRenderThread().GetLogger();
	logger.LogInfo( "LogDebugInformation:" );
	LogDebugInformation( list, decString() );
}

void deoglRenderDevMode::LogDebugInformation( const deoglDebugInformationList &list, const decString &prefix ){
	deoglRTLogger &logger = GetRenderThread().GetLogger();
	const int count = list.GetCount();
	int i;
	
	const decString childPrefix( prefix + "  " );
	for( i=0; i<count; i++ ){
		const deoglDebugInformation &di = *list.GetAt( i );
		if( ! di.GetVisible() ){
			continue;
		}
		
		if( di.HasElapsedTime() && di.HasCounter() ){
			logger.LogInfoFormat( "%s- %s: %d.%01dms (%d)", prefix.GetString(), di.GetName().GetString(),
				( int )( di.GetElapsedTimeAccum() * 1000.0f ), ( int )( di.GetElapsedTimeAccum() * 10000.0f ) % 10, di.GetCounter() );
			
		}else if( di.HasElapsedTime() ){
			logger.LogInfoFormat( "%s- %s: %d.%01dms", prefix.GetString(), di.GetName().GetString(),
				( int )( di.GetElapsedTimeAccum() * 1000.0f ), ( int )( di.GetElapsedTimeAccum() * 10000.0f ) % 10 );
			
		}else{
			logger.LogInfoFormat( "%s- %s: -", prefix.GetString(), di.GetName().GetString() );
		}
		
		if( di.GetChildren().GetCount() > 0 ){
			LogDebugInformation( di.GetChildren(), childPrefix );
		}
	}
}

void deoglRenderDevMode::LayoutDebugInformation( deoglRenderPlan &plan,
const decPoint &position, decPoint &size, const deoglDebugInformationList &list,
int minWidth, int maxWidth, bool alignSidewards ){
	const int count = list.GetCount();
	bool siblingsHaveElapsedTime = false;
	bool siblingsHaveCounter = false;
	int maxNameWidth = 0;
	int i;
	
	if( ! alignSidewards ){
		ChildMaxNameLen( list, maxNameWidth, siblingsHaveElapsedTime, siblingsHaveCounter );
	}
	
	decPoint childPos( position );
	const int paddingSidewards = 2;
	int rowHeight = 0;
	
	size.x = 0;
	
	for( i=0; i<count; i++ ){
		deoglDebugInformation &child = *list.GetAt( i );
		if( ! child.GetVisible() ){
			continue;
		}
		
		if( alignSidewards ){
			maxNameWidth = child.GetName().GetLength() + 1;
			siblingsHaveElapsedTime = child.HasElapsedTime();
			siblingsHaveCounter = child.HasCounter();
			
			LayoutDebugInformation( plan, maxNameWidth, child, minWidth, maxWidth - childPos.x,
				siblingsHaveElapsedTime, siblingsHaveCounter );
			
			const decPoint &childSize = child.GetRenderSize();
			
			if( childPos.x + childSize.x > maxWidth ){
				childPos.x = position.x;
				childPos.y += rowHeight;
				child.SetRenderPosition( childPos );
				childPos.x += childSize.x + paddingSidewards;
				rowHeight = childSize.y;
				
			}else{
				child.SetRenderPosition( childPos );
				childPos.x = childPos.x + childSize.x + paddingSidewards;
				rowHeight = decMath::max( rowHeight, childSize.y );
			}
			
		}else{
			LayoutDebugInformation( plan, maxNameWidth, child, 0, maxWidth,
				siblingsHaveElapsedTime, siblingsHaveCounter );
			
			const decPoint &childSize = child.GetRenderSize();
			
			child.SetRenderPosition( childPos );
			childPos.y += childSize.y;
			size.x = decMath::max( size.x, childSize.x );
		}
	}
	
	if( alignSidewards ){
		childPos.y += rowHeight;
	}
	
	size.y = childPos.y - position.y;
}

void deoglRenderDevMode::ChildMaxNameLen( const deoglDebugInformationList &list,
int &maxNameWidth, bool &siblingsHaveElapsedTime, bool &siblingsHaveCounter ) const{
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglDebugInformation &child = *list.GetAt( i );
		if( ! child.GetVisible() ){
			continue;
		}
		
		maxNameWidth = decMath::max( maxNameWidth, child.GetName().GetLength() + 1 );
		siblingsHaveElapsedTime |= child.HasElapsedTime();
		siblingsHaveCounter |= child.HasCounter();
		
		ChildMaxNameLen( child.GetChildren(), maxNameWidth, siblingsHaveElapsedTime, siblingsHaveCounter );
	}
}

void deoglRenderDevMode::LayoutDebugInformation( deoglRenderPlan& plan, int maxNameWidth,
deoglDebugInformation& debugInformation, int minWidth, int maxWidth,
bool siblingsHaveElapsedTime, bool siblingsHaveCounter){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugInformationList &children = debugInformation.GetChildren();
	deoglRenderDebug &renderDebug = renderThread.GetRenderers().GetDebug();
	const int charWidth = renderDebug.GetDebugFont().GetGlyphs()[ 32 ].width;
	const int fontHeight = renderDebug.GetDebugFont().GetFontHeight();
	const int count = children.GetCount();
	int i;
	
	// text to display
	decString text( debugInformation.GetName() );
	for( i=debugInformation.GetName().GetLength()+1; i<maxNameWidth; i++ ){
		text.AppendCharacter( ' ' );
	}
	
	if( siblingsHaveElapsedTime ){
		if( debugInformation.HasElapsedTime() ){
			text.AppendFormat( " % 3i.%01i", ( int )( debugInformation.GetElapsedTime() * 1000.0f ),
				( int )( debugInformation.GetElapsedTime() * 10000.0f ) % 10 );
			
		}else{
			text += "   ---";
		}
	}
	
	if( siblingsHaveCounter ){
		if( debugInformation.HasCounter() ){
			text.AppendFormat( " % 5i", debugInformation.GetCounter() );
			
		}else{
			text += "     -";
		}
	}
	
	debugInformation.SetRenderText( text );
	
	decPoint size( decMath::max( minWidth, charWidth * text.GetLength() ), fontHeight );
	
	// layout children
	if( count > 0 ){
		const int childPaddingLeft = charWidth;
		const int childPaddingRight = 0;
		
		decPoint childrenPosition( childPaddingLeft, fontHeight );
		const int childrenMaxWidth = maxWidth - childPaddingRight - childrenPosition.x;
		decPoint childrenSize;
		
		LayoutDebugInformation( plan, childrenPosition, childrenSize,
			children, size.x, childrenMaxWidth, false );
		
		/*
		if( childPaddingLeft > 0 ){
			renderDebug.RenderRectangle( plan, position.x, childrenPosition.y, childrenPosition.x,
				childrenPosition.y + childrenSize.y, debugInformation.GetColorBackground() );
		}
		
		if( childPaddingRight > 0 ){
			renderDebug.RenderRectangle( plan, childrenPosition.x + childrenSize.x,
				childrenPosition.y, childrenPosition.x + childrenSize.x + childPaddingRight,
				childrenPosition.y + childrenSize.y, debugInformation.GetColorBackground() );
		}
		*/
		
		size.x = decMath::max( size.x, childPaddingLeft + childrenSize.x + childPaddingRight );
		size.y += childrenSize.y;
	}
	
	debugInformation.SetRenderSize( size );
}

void deoglRenderDevMode::RenderDebugInformation( deoglRenderPlan &plan,
const decPoint &parentPosition, const deoglDebugInformation &debugInformation ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugInformationList &children = debugInformation.GetChildren();
	deoglRenderDebug &renderDebug = renderThread.GetRenderers().GetDebug();
	const int fontHeight = renderDebug.GetDebugFont().GetFontHeight();
	const int count = children.GetCount();
	
	// render text
	const decPoint position( parentPosition + debugInformation.GetRenderPosition() );
	
	renderDebug.AddRenderRectangle( plan, position.x, position.y,
		position.x + debugInformation.GetRenderSize().x, position.y + fontHeight,
		debugInformation.GetColorBackground() );
	
// 	renderDebug.RenderText( plan, debugInformation.GetRenderText(),
// 		position.x, position.y, debugInformation.GetColorText() );
	renderDebug.AddRenderText( plan, debugInformation.GetRenderText(),
		position.x, position.y, debugInformation.GetColorText() );
	
	// render children
	if( count > 0 ){
		int i;
		for( i=0; i<count; i++ ){
			const deoglDebugInformation &child = *children.GetAt( i );
			if( child.GetVisible() ){
				RenderDebugInformation( plan, position, child );
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglRenderDevMode::pCleanUp(){
	deoglDelayedOperations &dops = GetRenderThread().GetDelayedOperations();
	dops.DeleteOpenGLVertexArray( pVAOShapes );
	dops.DeleteOpenGLBuffer( pVBOShapes );
}

void deoglRenderDevMode::pCreateShapesVAO(){
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread() );
	GLfloat vbodata[ 14 ];
	
	// the actual geometry points are obtain by calculating x'=x*scale+offset.
	
	// set up vbo data for a point shape
	vbodata[ 0 ] = 0.0f; // p1.x = x
	vbodata[ 1 ] = 0.0f; // p1.y = y
	
	// set up vbo data for a line shape
	vbodata[ 2 ] = 0.0f; // p1.x = x1
	vbodata[ 3 ] = 0.0f; // p1.y = y1
	
	vbodata[ 4 ] = 1.0f; // p2.x = x2
	vbodata[ 5 ] = 1.0f; // p2.y = y2
	
	// set up vbo data for a rectangular shape
	vbodata[ 6 ] = 0.0f; // p1.x = x1
	vbodata[ 7 ] = 0.0f; // p1.y = y1
	
	vbodata[ 8 ] = 0.0f; // p2.x = x1
	vbodata[ 9 ] = 1.0f; // p2.y = y2
	
	vbodata[ 10 ] = 1.0f; // p3.x = x2
	vbodata[ 11 ] = 1.0f; // p3.y = y2
	
	vbodata[ 12 ] = 1.0f; // p4.x = x2
	vbodata[ 13 ] = 0.0f; // p4.y = y1
	
	// create vbo and vao
	OGL_CHECK( renderThread, pglGenVertexArrays( 1, &pVAOShapes ) );
	if( ! pVAOShapes ) DETHROW( deeOutOfMemory );
	OGL_CHECK( renderThread, pglBindVertexArray( pVAOShapes ) );
	
	OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOShapes ) );
	if( ! pVBOShapes ) DETHROW( deeOutOfMemory );
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOShapes ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( vbodata ), ( const GLvoid * )&vbodata, GL_STATIC_DRAW ) );
	
	OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) );
	OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, ( const GLvoid * )0 ) );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}
