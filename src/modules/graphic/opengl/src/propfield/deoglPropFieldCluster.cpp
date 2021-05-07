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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPropField.h"
#include "deoglPropFieldCluster.h"
#include "deoglRPropField.h"
#include "deoglRPropFieldType.h"
#include "../deGraphicOpenGl.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../model/texture/deoglModelTexture.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../sky/deoglRSkyInstance.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../utils/deoglConvertFloatHalf.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/resources/propfield/dePropFieldType.h>



// Class deoglPropFieldCluster
/////////////////////

// Constructor, destructor
////////////////////////////

deoglPropFieldCluster::deoglPropFieldCluster( deoglRPropFieldType &propFieldType ) :
pPropFieldType( propFieldType ),
pRenderThread( propFieldType.GetPropField().GetRenderThread() ),

pInstances( NULL ),
pInstanceCount( 0 ),

pBendStateData( NULL ),
pBendStateDataSize( 0 ),

pTUCDepth( NULL ),
pTUCGeometry( NULL ),
pTUCShadow( NULL ),
pTUCEnvMap( NULL ),

pDirtyTUCDepth( true ),
pDirtyTUCGeometry( true ),
pDirtyTUCShadow( true ),
pDirtyTUCEnvMap( true ),
pDirtyBendStates( true ),

pTBOInstances( 0 ),
pTBOBendStates( 0 ),
pVBOInstances( 0 ),
pVBOBendStates( 0 ),

pRTSInstance( NULL ){
}

deoglPropFieldCluster::~deoglPropFieldCluster(){
	//deoglRenderThread &renderThread = pPropFieldType.GetPropField().GetRenderThread();
	// we can not access renderThread through pPropFieldType during the destructor. this is due
	// to the fact that pPropFieldType and debpPropField do not exist anymore while this object
	// is destroyed. the reason is that delayed deletion delets the clusters and at that time
	// the parent objects do not exist anymore. Render thread is thus stored aside during
	// construction time which is valid and prevents the problem
	
	if( pRTSInstance ){
		pRTSInstance->ReturnToPool();
	}
	
	if( pBendStateData ){
		delete [] pBendStateData;
	}
	if( pInstances ){
		delete [] pInstances;
	}
	
	if( pTUCDepth ){
		pTUCDepth->RemoveUsage();
		pTUCDepth = NULL;
	}
	if( pTUCGeometry ){
		pTUCGeometry->RemoveUsage();
		pTUCGeometry = NULL;
	}
	if( pTUCShadow ){
		pTUCShadow->RemoveUsage();
		pTUCShadow = NULL;
	}
	if( pTUCEnvMap ){
		pTUCEnvMap->RemoveUsage();
		pTUCEnvMap = NULL;
	}
	
	if( pTBOBendStates ){
		OGL_CHECK( pRenderThread, glDeleteTextures( 1, &pTBOBendStates ) );
	}
	if( pTBOInstances ){
		OGL_CHECK( pRenderThread, glDeleteTextures( 1, &pTBOInstances ) );
	}
	if( pVBOBendStates ){
		pglDeleteBuffers( 1, &pVBOBendStates );
	}
	if( pVBOInstances ){
		pglDeleteBuffers( 1, &pVBOInstances );
	}
}



// Management
///////////////

void deoglPropFieldCluster::SetExtends( const decVector &minExtend, const decVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglPropFieldCluster::SetInstanceCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pInstanceCount ){
		sInstance *instances = NULL;
		
		if( count > 0 ){
			instances = new sInstance[ count ];
		}
		
		if( pInstances ){
			delete [] pInstances;
		}
		pInstances = instances;
		pInstanceCount = count;
	}
}



void deoglPropFieldCluster::UpdateTBOs(){
	if( ! pVBOInstances ){
		UpdateTBOInstances();
	}
	if( pDirtyBendStates ){
		UpdateTBOBendStates();
		pDirtyBendStates = false;
	}
	
	if( pPropFieldType.GetModel() ){
		if( ! pRTSInstance ){
			pRTSInstance = pRenderThread.GetRenderTaskSharedPool().GetInstance();
			UpdateRTSInstances();
		}
		
	}else{
		if( pRTSInstance ){
			pRTSInstance->ReturnToPool();
			pRTSInstance = NULL;
		}
	}
}

#define CONVERT_FLOAT_TO_HALF convertFloatToHalf
//#define CONVERT_FLOAT_TO_HALF quickConvertFloatToHalf

void deoglPropFieldCluster::UpdateTBOInstances(){
	deoglRenderThread &renderThread = pPropFieldType.GetPropField().GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const int vboDataSize = pInstanceCount * 24; // sizeof( HALF_FLOAT ) * 12
	HALF_FLOAT * const vboData = ( HALF_FLOAT* )renderThread.GetBufferObject().GetTemporaryVBOData( vboDataSize );
	HALF_FLOAT *vboDataPtr = vboData;
	int i;
	
	for( i=0; i<pInstanceCount; i++ ){
		const sInstance &instance = pInstances[ i ];
		
		// pixel 1: pos.x,           pos.y,           pos.z,           rot.a13 * scale
		// pixel 2: rot.a11 * scale, rot.a21 * scale, rot.a31 * scale, rot.a23 * scale
		// pixel 3: rot.a12 * scale, rot.a22 * scale, rot.a32 * scale, rot.a33 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.position[ 0 ] );                    // pixel 1 r: pos.x
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.position[ 1 ] );                    // pixel 1 g: pos.y
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.position[ 2 ] );                    // pixel 1 b: pos.z
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 2 ] * instance.scaling ); // pixel 1 a: rot.a13 * scale
		
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 0 ] * instance.scaling ); // pixel 2 r: rot.a11 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 3 ] * instance.scaling ); // pixel 2 g: rot.a21 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 6 ] * instance.scaling ); // pixel 2 b: rot.a31 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 5 ] * instance.scaling ); // pixel 2 a: rot.a23 * scale
		
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 1 ] * instance.scaling ); // pixel 3 r: rot.a12 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 4 ] * instance.scaling ); // pixel 3 g: rot.a22 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 7 ] * instance.scaling ); // pixel 3 b: rot.a32 * scale
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( instance.rotation[ 8 ] * instance.scaling ); // pixel 3 a: rot.a33 * scale
	}
	
	OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOInstances ) );
	if( ! pVBOInstances ){
		DETHROW( deeOutOfMemory );
	}
	OGL_CHECK( renderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBOInstances ) );
	OGL_CHECK( renderThread, pglBufferData( GL_TEXTURE_BUFFER, vboDataSize, vboData, GL_STATIC_DRAW ) );
	
	OGL_CHECK( renderThread, glGenTextures( 1, &pTBOInstances ) );
	if( ! pTBOInstances ){
		DETHROW( deeInvalidParam );
	}
	tsmgr.EnableBareTBO( 0, pTBOInstances );
	OGL_CHECK( renderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA16F, pVBOInstances ) );
	
	tsmgr.DisableStage( 0 );
	
// 	renderThread.GetLogger().LogInfoFormat( "PropField %p Type %p Cluster %p: Create TBO Instances (tbo=%u vbo=%u size=%i count=%i)",
// 		&pPropFieldType.GetPropField(), &pPropFieldType, this, pTBOInstances, pVBOInstances, vboDataSize, pInstanceCount );
}

void deoglPropFieldCluster::UpdateTBOBendStates(){
	deoglRenderThread &renderThread = pPropFieldType.GetPropField().GetRenderThread();
	const int vboDataSize = pInstanceCount * 4; // sizeof( halfFloat ) * 2
	
	if( ! pVBOBendStates ){
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOBendStates ) );
		if( ! pVBOBendStates ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBOBendStates ) );
	OGL_CHECK( renderThread, pglBufferData( GL_TEXTURE_BUFFER, vboDataSize, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( renderThread, pglBufferData( GL_TEXTURE_BUFFER, vboDataSize, pBendStateData, GL_STREAM_DRAW ) );
	
	if( ! pTBOBendStates ){
		deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
		
		OGL_CHECK( renderThread, glGenTextures( 1, &pTBOBendStates ) );
		if( ! pTBOBendStates ){
			DETHROW( deeInvalidParam );
		}
		tsmgr.EnableBareTBO( 0, pTBOBendStates );
		OGL_CHECK( renderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RG16F, pVBOBendStates ) );
		
		tsmgr.DisableStage( 0 );
		
// 		renderThread.GetLogger().LogInfoFormat( "PropField %p Type %p Cluster %p: Create TBO Bend States (tbo=%u vbo=%u size=%i count=%i)",
// 			&pPropFieldType.GetPropField(), &pPropFieldType, this, pTBOBendStates, pVBOBendStates, vboDataSize, pInstanceCount );
	}
}

void deoglPropFieldCluster::PrepareBendStateData( const dePropFieldType &type ){
	const dePropFieldBendState * const engBendStates = type.GetBendStates();
	const int engBendStateCount = type.GetBendStateCount();
	const int vboDataSize = pInstanceCount * 4; // sizeof( halfFloat ) * 2
	
	if( vboDataSize > pBendStateDataSize ){
		if( pBendStateData ){
			delete [] pBendStateData;
			pBendStateData = NULL;
			pBendStateDataSize = 0;
		}
		
		if( vboDataSize > 0 ){
			pBendStateData = new char[ vboDataSize ];
			pBendStateDataSize = vboDataSize;
		}
	}
	
	HALF_FLOAT * const vboData = ( HALF_FLOAT* )pBendStateData;
	const HALF_FLOAT halfZero = CONVERT_FLOAT_TO_HALF( 0.0f );
	HALF_FLOAT *vboDataPtr = vboData;
	int i;
	
	for( i=0; i<pInstanceCount; i++ ){
		const sInstance &instance = pInstances[ i ];
		
		// pixel 1: bend.x, bend.z
		if( instance.bstate >= 0 && instance.bstate < engBendStateCount ){
			const dePropFieldBendState &engBendState = engBendStates[ instance.bstate ];
			
			*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( engBendState.GetBendX() ); // pixel 1 r: bend.x
			*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( engBendState.GetBendZ() ); // pixel 1 g: bend.z
			
		}else{
			*( vboDataPtr++ ) = halfZero; // pixel 1 r: bend.x
			*( vboDataPtr++ ) = halfZero; // pixel 1 g: bend.z
		}
	}
	
	pDirtyBendStates = true;
}



deoglTexUnitsConfig *deoglPropFieldCluster::GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ) {
	switch( shaderType ){
	case deoglSkinTexture::estPropFieldGeometry:
	case deoglSkinTexture::estPropFieldImposterGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexture::estPropFieldDepth:
	case deoglSkinTexture::estPropFieldImposterDepth:
	case deoglSkinTexture::estPropFieldDepthClipPlane:
	case deoglSkinTexture::estPropFieldImposterDepthClipPlane:
	case deoglSkinTexture::estPropFieldDepthReversed:
	case deoglSkinTexture::estPropFieldImposterDepthReversed:
	case deoglSkinTexture::estPropFieldDepthClipPlaneReversed:
	case deoglSkinTexture::estPropFieldImposterDepthClipPlaneReversed:
	case deoglSkinTexture::estPropFieldCounter:
	case deoglSkinTexture::estPropFieldCounterClipPlane:
	case deoglSkinTexture::estPropFieldShadowProjection:
	case deoglSkinTexture::estPropFieldShadowOrthogonal:
	case deoglSkinTexture::estPropFieldShadowDistance:
		return GetTUCShadow();
		
	case deoglSkinTexture::estPropFieldEnvMap:
		return GetTUCEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglPropFieldCluster::GetTUCDepth(){
	if( pDirtyTUCDepth ){
		if( pTUCDepth ){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		
		pTUCDepth = BareGetTUCFor( deoglSkinTexture::estPropFieldDepth );
		
		pDirtyTUCDepth = false;
	}
	
	return pTUCDepth;
}

deoglTexUnitsConfig *deoglPropFieldCluster::GetTUCGeometry(){
	if( pDirtyTUCGeometry ){
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		
		pTUCGeometry = BareGetTUCFor( deoglSkinTexture::estPropFieldGeometry );
		
		pDirtyTUCGeometry = false;
	}
	
	return pTUCGeometry;
}

deoglTexUnitsConfig *deoglPropFieldCluster::GetTUCShadow(){
	if( pDirtyTUCShadow ){
		if( pTUCShadow ){
			pTUCShadow->RemoveUsage();
			pTUCShadow = NULL;
		}
		
		pTUCShadow = BareGetTUCFor( deoglSkinTexture::estPropFieldShadowProjection );
		
		pDirtyTUCShadow = false;
	}
	
	return pTUCShadow;
}

deoglTexUnitsConfig *deoglPropFieldCluster::GetTUCEnvMap(){
	if( pDirtyTUCEnvMap ){
		if( pTUCEnvMap ){
			pTUCEnvMap->RemoveUsage();
			pTUCEnvMap = NULL;
		}
		
		deoglSkinTexture * const skinTexture = pPropFieldType.GetUseSkinTexture();
		
		if( skinTexture ){
			deoglRenderThread &renderThread = pPropFieldType.GetPropField().GetRenderThread();
			deoglRDynamicSkin *dynamicSkin = NULL;
			deoglSkinState *skinState = NULL;
			deoglTexUnitConfig unit[ 8 ];
			
			if( skinTexture->GetVariationU() || skinTexture->GetVariationV() ){
				unit[ 0 ].EnableArrayTextureFromChannel( renderThread, *skinTexture, deoglSkinChannel::ectColor,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetColorArray() );
				
				unit[ 1 ].EnableArrayTextureFromChannel( renderThread, *skinTexture, deoglSkinChannel::ectEmissivity,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetEmissivityArray() );
				
			}else{
				unit[ 0 ].EnableTextureFromChannel( renderThread, *skinTexture, deoglSkinChannel::ectColor,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetColor() );
				
				unit[ 1 ].EnableTextureFromChannel( renderThread, *skinTexture, deoglSkinChannel::ectEmissivity,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetEmissivity() );
			}
			
			pTUCEnvMap = renderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2,
				pPropFieldType.GetUseSkinTexture()->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
			pTUCEnvMap->EnsureRTSTexture();
		}
		
		pDirtyTUCEnvMap = false;
	}
	
	return pTUCEnvMap;
}

deoglTexUnitsConfig *deoglPropFieldCluster::BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const{
	deoglSkinTexture * const skinTexture = pPropFieldType.GetUseSkinTexture();
	if( ! skinTexture ){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pPropFieldType.GetPropField().GetRenderThread();
	deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	int target;
	
	deoglSkinShader &skinShader = *skinTexture->GetShaderFor( shaderType );
	
	if( skinShader.GetUsedTextureTargetCount() > 0 ){
		skinShader.SetTUCCommon( &units[ 0 ], *skinTexture, skinState, dynamicSkin );
		skinShader.SetTUCPerObjectEnvMap( &units[ 0 ],
			pPropFieldType.GetPropField().GetParentWorld()->GetSkyEnvironmentMap(), NULL, NULL );
		
		target = skinShader.GetTextureTarget( deoglSkinShader::ettSubInstance1 );
		if( target != -1 ){
			units[ target ].EnableTBO( pTBOInstances );
		}
		
		target = skinShader.GetTextureTarget( deoglSkinShader::ettSubInstance2 );
		if( target != -1 ){
			units[ target ].EnableTBO( pTBOBendStates );
		}
		
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount(),
			skinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	if( ! tuc ){
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith( NULL, 0,
			skinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	tuc->EnsureRTSTexture();
	
	return tuc;
}

void deoglPropFieldCluster::MarkTUCsDirty(){
	pDirtyTUCDepth = true;
	pDirtyTUCGeometry = true;
	pDirtyTUCShadow = true;
	pDirtyTUCEnvMap = true;
}

void deoglPropFieldCluster::UpdateRTSInstances(){
	if( pRTSInstance ){
		deoglModelLOD &modelLod = pPropFieldType.GetModel()->GetLODAt( 0 );
		const deoglSharedVBOBlock &vboBlock = *modelLod.GetVBOBlock();
		const deoglModelTexture &modelTexture = modelLod.GetTextureAt( 0 );
		
		pRTSInstance->SetParameterBlock( pPropFieldType.GetParamBlock() );
		pRTSInstance->SetParameterBlockSpecial( NULL );
		pRTSInstance->SetFirstPoint( vboBlock.GetOffset() );
		pRTSInstance->SetPointCount( 0 );
		pRTSInstance->SetFirstIndex( vboBlock.GetIndexOffset() + modelTexture.GetFirstFace() * 3 );
		pRTSInstance->SetIndexCount( modelTexture.GetFaceCount() * 3 );
		pRTSInstance->SetSubInstanceCount( pInstanceCount );
		pRTSInstance->SetDoubleSided( modelTexture.GetDoubleSided() );
		pRTSInstance->SetPrimitiveType( GL_TRIANGLES );
		
		/*
		// for imposters
		firstPoint = 0;
		firstIndex = 0;
		indexCount = 0;
		pointCount = 6;
		*/
	}
}
