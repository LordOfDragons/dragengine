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
#include "../delayedoperation/deoglDelayedOperations.h"
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
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/resources/propfield/dePropFieldType.h>



// Class deoglPropFieldCluster::WorldComputeElement
/////////////////////////////////////////////////////

deoglPropFieldCluster::WorldComputeElement::WorldComputeElement( deoglPropFieldCluster &cluster ) :
deoglWorldComputeElement( eetPropFieldCluster, &cluster ),
pCluster( cluster ){
}

void deoglPropFieldCluster::WorldComputeElement::UpdateData( sDataElement &data ) const{
	const decDVector position( pCluster.GetPropFieldType().GetPropField().GetPosition() - GetReferencePosition() );
	
	data.SetExtends( position + pCluster.GetMinimumExtend(), position + pCluster.GetMaximumExtend() );
	data.SetEmptyLayerMask();
	data.flags = ( uint32_t )( deoglWorldCompute::eefPropFieldCluster
		| deoglWorldCompute::eefDynamic | deoglWorldCompute::eefGIDynamic );
	data.geometryCount = 1;
	data.highestLod = 0;
}

void deoglPropFieldCluster::WorldComputeElement::UpdateDataGeometries( sDataElementGeometry *data ) const{
	deoglSkinTexture * const skinTexture = pCluster.GetPropFieldType().GetUseSkinTexture();
	if( ! skinTexture ){
		return;
	}
	
	const deoglRModel * const model = pCluster.GetPropFieldType().GetModel();
	if( ! model ){
		return;
	}
	
	const deoglModelLOD &modelLOD = model->GetLODAt( 0 );
	if( ! modelLOD.GetVBOBlock() ){
		return;
	}
	
	const deoglModelTexture &modelTex = modelLOD.GetTextureAt( 0 );
	
	int filters = skinTexture->GetRenderTaskFilters() & ~RenderFilterOutline;
	if( modelTex.GetDecal() ){
		filters |= ertfDecal;
	}
	
	SetDataGeometry( *data, 0, filters, deoglSkinTexturePipelinesList::eptPropField,
		modelTex.GetDoubleSided() ? deoglSkinTexturePipelines::emDoubleSided : 0, skinTexture,
		modelLOD.GetVBOBlock()->GetVBO()->GetVAO(), pCluster.GetRTSInstance(), -1 );
	
	sInfoTUC info;
	info.geometry = pCluster.GetTUCGeometry();
	info.depth = pCluster.GetTUCDepth();
	info.counter = pCluster.GetTUCDepth();
	info.shadow = pCluster.GetTUCShadow();
	info.shadowCube = pCluster.GetTUCShadow();
	info.envMap = pCluster.GetTUCEnvMap();
	// info.giMaterial = pCluster.GetTUCGIMaterial(); // missing
	SetDataGeometryTUCs( *data, info );
}



// Class deoglPropFieldCluster
////////////////////////////////

#define CONVERT_FLOAT_TO_HALF convertFloatToHalf
//#define CONVERT_FLOAT_TO_HALF quickConvertFloatToHalf

// Constructor, destructor
////////////////////////////

deoglPropFieldCluster::deoglPropFieldCluster( deoglRPropFieldType &propFieldType ) :
pPropFieldType( propFieldType ),
pRenderThread( propFieldType.GetPropField().GetRenderThread() ),
pWorldComputeElement( deoglWorldComputeElement::Ref::New( new WorldComputeElement( *this ) ) ),

pInstances( NULL ),
pInstanceCount( 0 ),

pBendStateData( NULL ),
pBendStateDataSize( 0 ),

pTUCDepth( NULL ),
pTUCGeometry( NULL ),
pTUCShadow( NULL ),
pTUCEnvMap( NULL ),

pDirtyTUCs( true ),
pDirtyBendStates( true ),

pTBOInstances( 0 ),
pTBOBendStates( 0 ),
pVBOInstances( 0 ),
pVBOBendStates( 0 ),

pRTSInstance( NULL ),
pDirtyRTSInstance( true ){
}

deoglPropFieldCluster::~deoglPropFieldCluster(){
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
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLTexture( pTBOBendStates );
	dops.DeleteOpenGLTexture( pTBOInstances );
	dops.DeleteOpenGLBuffer( pVBOBendStates );
	dops.DeleteOpenGLBuffer( pVBOInstances );
}



// Management
///////////////

void deoglPropFieldCluster::SetExtends( const decVector &minExtend, const decVector &maxExtend ){
	if( minExtend.IsEqualTo( pMinExtend ) && maxExtend.IsEqualTo( pMaxExtend ) ){
		return;
	}
	
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
	
	pWorldComputeElement->ComputeUpdateElement();
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
	
	pDirtyRTSInstance = true;
}



void deoglPropFieldCluster::PrepareForRender(){
	pPrepareTBOs();
	pPrepareTUCs(); // requires TBO to be present
	pPrepareRTSInstance();
}

void deoglPropFieldCluster::PrepareBendStateData( const dePropFieldType &type ){
	// WARNING Called during synchronization by main thread.
	
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
	pPropFieldType.ClusterRequiresPrepareForRender();
}



deoglTexUnitsConfig *deoglPropFieldCluster::GetTUCForPipelineType( deoglSkinTexturePipelines::eTypes type ) const{
	switch( type ){
	case deoglSkinTexturePipelines::etGeometry:
	case deoglSkinTexturePipelines::etGeometryDepthTest:
		return GetTUCGeometry();
		
	case deoglSkinTexturePipelines::etDepth:
	case deoglSkinTexturePipelines::etDepthClipPlane:
	case deoglSkinTexturePipelines::etDepthReversed:
	case deoglSkinTexturePipelines::etDepthClipPlaneReversed:
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
	case deoglSkinTexturePipelines::etMask:
		return GetTUCDepth();
		
	case deoglSkinTexturePipelines::etShadowProjection:
	case deoglSkinTexturePipelines::etShadowProjectionCube:
	case deoglSkinTexturePipelines::etShadowOrthogonal:
	case deoglSkinTexturePipelines::etShadowDistance:
	case deoglSkinTexturePipelines::etShadowDistanceCube:
		return GetTUCShadow();
		
	case deoglSkinTexturePipelines::etEnvMap:
		return GetTUCEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglPropFieldCluster::BareGetTUCFor( deoglSkinTexturePipelines::eTypes type ) const{
	deoglSkinTexture * const skinTexture = pPropFieldType.GetUseSkinTexture();
	if( ! skinTexture ){
		return NULL;
	}
	
	deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	int target;
	
	deoglSkinShader &skinShader = *skinTexture->GetPipelines().
		GetAt( deoglSkinTexturePipelinesList::eptPropField ).
		GetWithRef( type ).GetShader();
	
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
		
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount(),
			skinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith( NULL, 0,
			skinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	tuc->EnsureRTSTexture();
	
	return tuc;
}

void deoglPropFieldCluster::MarkTUCsDirty(){
	pDirtyTUCs = true;
	pPropFieldType.ClusterRequiresPrepareForRender();
	UpdateWorldComputeTextures();
}

void deoglPropFieldCluster::DirtyRTSInstance(){
	pDirtyRTSInstance = true;
	UpdateWorldComputeTextures();
}



void deoglPropFieldCluster::AddToWorldCompute( deoglWorldCompute &worldCompute ){
	worldCompute.AddElement( pWorldComputeElement );
}

void deoglPropFieldCluster::UpdateWorldCompute(){
	pWorldComputeElement->ComputeUpdateElement();
}

void deoglPropFieldCluster::UpdateWorldComputeTextures(){
	pWorldComputeElement->ComputeUpdateElementGeometries();
}

void deoglPropFieldCluster::RemoveFromWorldCompute(){
	pWorldComputeElement->RemoveFromCompute();
}



// Private Functions
//////////////////////

void deoglPropFieldCluster::pPrepareTUCs(){
	if( ! pDirtyTUCs ){
		return;
	}
	pDirtyTUCs = false;
	
	// depth
	if( pTUCDepth ){
		pTUCDepth->RemoveUsage();
		pTUCDepth = NULL;
	}
	pTUCDepth = BareGetTUCFor( deoglSkinTexturePipelines::etDepth );
	
	// geometry
	if( pTUCGeometry ){
		pTUCGeometry->RemoveUsage();
		pTUCGeometry = NULL;
	}
	pTUCGeometry = BareGetTUCFor( deoglSkinTexturePipelines::etGeometry );
	
	// shadow
	if( pTUCShadow ){
		pTUCShadow->RemoveUsage();
		pTUCShadow = NULL;
	}
	pTUCShadow = BareGetTUCFor( deoglSkinTexturePipelines::etShadowProjection );
	
	// envmap
	if( pTUCEnvMap ){
		pTUCEnvMap->RemoveUsage();
		pTUCEnvMap = NULL;
	}
	
	deoglSkinTexture * const skinTexture = pPropFieldType.GetUseSkinTexture();
	
	if( skinTexture ){
		deoglRDynamicSkin *dynamicSkin = NULL;
		deoglSkinState *skinState = NULL;
		deoglTexUnitConfig unit[ 8 ];
		
		if( skinTexture->GetVariationU() || skinTexture->GetVariationV() ){
			unit[ 0 ].EnableArrayTextureFromChannel( pRenderThread, *skinTexture, deoglSkinChannel::ectColor,
				skinState, dynamicSkin, pRenderThread.GetDefaultTextures().GetColorArray() );
			
			unit[ 1 ].EnableArrayTextureFromChannel( pRenderThread, *skinTexture, deoglSkinChannel::ectEmissivity,
				skinState, dynamicSkin, pRenderThread.GetDefaultTextures().GetEmissivityArray() );
			
		}else{
			unit[ 0 ].EnableTextureFromChannel( pRenderThread, *skinTexture, deoglSkinChannel::ectColor,
				skinState, dynamicSkin, pRenderThread.GetDefaultTextures().GetColor() );
			
			unit[ 1 ].EnableTextureFromChannel( pRenderThread, *skinTexture, deoglSkinChannel::ectEmissivity,
				skinState, dynamicSkin, pRenderThread.GetDefaultTextures().GetEmissivity() );
		}
		
		pTUCEnvMap = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2,
			pPropFieldType.GetUseSkinTexture()->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
		pTUCEnvMap->EnsureRTSTexture();
	}
}



void deoglPropFieldCluster::pPrepareTBOs(){
	if( ! pVBOInstances ){
		pUpdateTBOInstances();
	}
	
	if( pDirtyBendStates ){
		pDirtyBendStates = false;
		pUpdateTBOBendStates();
	}
}

void deoglPropFieldCluster::pUpdateTBOInstances(){
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const int vboDataSize = pInstanceCount * 24; // sizeof( HALF_FLOAT ) * 12
	HALF_FLOAT * const vboData = ( HALF_FLOAT* )pRenderThread.GetBufferObject().GetTemporaryVBOData( vboDataSize );
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
	
	OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOInstances ) );
	if( ! pVBOInstances ){
		DETHROW( deeOutOfMemory );
	}
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBOInstances ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, vboDataSize, vboData, GL_STATIC_DRAW ) );
	
	OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBOInstances ) );
	if( ! pTBOInstances ){
		DETHROW( deeInvalidParam );
	}
	tsmgr.EnableBareTBO( 0, pTBOInstances );
	OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA16F, pVBOInstances ) );
	
	tsmgr.DisableStage( 0 );
	
// 	renderThread.GetLogger().LogInfoFormat( "PropField %p Type %p Cluster %p: Create TBO Instances (tbo=%u vbo=%u size=%i count=%i)",
// 		&pPropFieldType.GetPropField(), &pPropFieldType, this, pTBOInstances, pVBOInstances, vboDataSize, pInstanceCount );
}

void deoglPropFieldCluster::pUpdateTBOBendStates(){
	const int vboDataSize = pInstanceCount * 4; // sizeof( halfFloat ) * 2
	
	if( ! pVBOBendStates ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOBendStates ) );
		if( ! pVBOBendStates ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBOBendStates ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, vboDataSize, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, vboDataSize, pBendStateData, GL_STREAM_DRAW ) );
	
	if( ! pTBOBendStates ){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBOBendStates ) );
		if( ! pTBOBendStates ){
			DETHROW( deeInvalidParam );
		}
		tsmgr.EnableBareTBO( 0, pTBOBendStates );
		OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RG16F, pVBOBendStates ) );
		
		tsmgr.DisableStage( 0 );
		
// 		renderThread.GetLogger().LogInfoFormat( "PropField %p Type %p Cluster %p: Create TBO Bend States (tbo=%u vbo=%u size=%i count=%i)",
// 			&pPropFieldType.GetPropField(), &pPropFieldType, this, pTBOBendStates, pVBOBendStates, vboDataSize, pInstanceCount );
	}
}



void deoglPropFieldCluster::pPrepareRTSInstance(){
	if( ! pDirtyRTSInstance ){
		return;
	}
	pDirtyRTSInstance = false;
	
	if( pPropFieldType.GetModel() ){
		if( ! pRTSInstance ){
			pRTSInstance = pRenderThread.GetRenderTaskSharedPool().GetInstance();
			pUpdateRTSInstances();
		}
		
	}else{
		if( pRTSInstance ){
			pRTSInstance->ReturnToPool();
			pRTSInstance = NULL;
		}
	}
}

void deoglPropFieldCluster::pUpdateRTSInstances(){
	if( ! pRTSInstance ){
		return;
	}
	
	pRTSInstance->SetParameterBlock( pPropFieldType.GetParamBlock() );
	pRTSInstance->SetParameterBlockSpecial( nullptr );
	pRTSInstance->SetPointCount( 0 );
	pRTSInstance->SetPrimitiveType( GL_TRIANGLES );
	
	const deoglModelLOD &modelLod = pPropFieldType.GetModel()->GetLODAt( 0 );
	
	const deoglModelTexture &modelTexture = modelLod.GetTextureAt( 0 );
	pRTSInstance->SetIndexCount( modelTexture.GetFaceCount() * 3 );
	
	if( modelLod.GetVBOBlock() ){
		const deoglSharedVBOBlock &vboBlock = *modelLod.GetVBOBlock();
		pRTSInstance->SetFirstPoint( vboBlock.GetOffset() );
		pRTSInstance->SetFirstIndex( vboBlock.GetIndexOffset() + modelTexture.GetFirstFace() * 3 );
		pRTSInstance->SetSubInstanceCount( pInstanceCount );
		
	}else{
		pRTSInstance->SetFirstPoint( 0 );
		pRTSInstance->SetFirstIndex( 0 );
		pRTSInstance->SetSubInstanceCount( 0 );
	}
	
	/*
	// for imposters
	firstPoint = 0;
	firstIndex = 0;
	indexCount = 0;
	pointCount = 6;
	*/
}
