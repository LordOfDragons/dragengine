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

#include "deoglHTSTexture.h"
#include "deoglRHeightTerrain.h"
#include "deoglRHTSector.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoglHTSTexture
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglHTSTexture::deoglHTSTexture( deoglRHTSector &sector, int index ) :
pSector( sector )
{
	if( index < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = index;
	
	pSkin = NULL;
	
	pUseSkinTexture = NULL;
	
	pParamBlockDepth = NULL;
	pParamBlockGeometry = NULL;
	pParamBlockEnvMap = NULL;
	
	pTUCDepth = NULL;
	pTUCGeometry = NULL;
	pTUCShadow = NULL;
	pTUCEnvMap = NULL;
	pTUCLuminance = NULL;
	
	pValidParamBlockDepth = false;
	pValidParamBlockGeometry = false;
	pValidParamBlockEnvMap = false;
	pDirtyParamBlockDepth = true;
	pDirtyParamBlockGeometry = true;
	pDirtyParamBlockEnvMap = true;
	
	pDirtyTUCDepth = true;
	pDirtyTUCGeometry = true;
	pDirtyTUCShadow = true;
	pDirtyTUCEnvMap = true;
	pDirtyTUCLuminance = true;
}

deoglHTSTexture::~deoglHTSTexture(){
	pCleanUp();
}



// Management
///////////////

void deoglHTSTexture::SetMatrix( const decTexMatrix &matrix ){
	pMatrix = matrix;
	MarkParamBlocksDirty();
}

void deoglHTSTexture::SetSkin ( deoglRSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	pSkin = skin;
	if( skin ){
		skin->AddReference();
	}
	
	pUseSkinTexture = NULL;
	if( skin && skin->GetTextureCount() > 0 ){
		pUseSkinTexture = &skin->GetTextureAt( 0 );
	}
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	//pComponent->MarkTextureUseSkinDirty();
}



deoglSPBlockUBO *deoglHTSTexture::GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estHeightMapGeometry:
	case deoglSkinTexture::estHeightMapLuminance:
		return GetParamBlockGeometry();
		
	case deoglSkinTexture::estHeightMapDepth:
	case deoglSkinTexture::estHeightMapDepthClipPlane:
	case deoglSkinTexture::estHeightMapDepthReversed:
	case deoglSkinTexture::estHeightMapDepthClipPlaneReversed:
	case deoglSkinTexture::estHeightMapTranspCount:
	case deoglSkinTexture::estHeightMapTranspCountClipPlane:
	case deoglSkinTexture::estHeightMapShadowProjection:
	case deoglSkinTexture::estHeightMapShadowOrthogonal:
	case deoglSkinTexture::estHeightMapShadowDistance:
		return GetParamBlockDepth();
		
	case deoglSkinTexture::estHeightMapEnvMap:
		return GetParamBlockEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglSPBlockUBO *deoglHTSTexture::GetParamBlockDepth(){
	if( ! pValidParamBlockDepth ){
		if( pParamBlockDepth ){
			pParamBlockDepth->FreeReference();
			pParamBlockDepth = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estHeightMapDepth );
			
			/*if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockDepth = new deoglSPBlockUBO( *pSector.GetHeightTerrain()
					.GetRenderThread().GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{*/
				pParamBlockDepth = skinShader.CreateSPBInstParam();
			//}
		}
		
		pValidParamBlockDepth = true;
		pDirtyParamBlockDepth = true;
	}
	
	if( pDirtyParamBlockDepth ){
		if( pParamBlockDepth ){
			UpdateInstanceParamBlock( *pParamBlockDepth, *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estHeightMapDepth ) );
		}
		
		pDirtyParamBlockDepth = false;
	}
	
	return pParamBlockDepth;
}

deoglSPBlockUBO *deoglHTSTexture::GetParamBlockGeometry(){
	if( ! pValidParamBlockGeometry ){
		if( pParamBlockGeometry ){
			pParamBlockGeometry->FreeReference();
			pParamBlockGeometry = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estHeightMapGeometry );
			
			/*if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockGeometry = new deoglSPBlockUBO( *pSector.GetHeightTerrain()
					.GetRenderThread().GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{*/
				pParamBlockGeometry = skinShader.CreateSPBInstParam();
			//}
		}
		
		pValidParamBlockGeometry = true;
		pDirtyParamBlockGeometry = true;
	}
	
	if( pDirtyParamBlockGeometry ){
		if( pParamBlockGeometry ){
			UpdateInstanceParamBlock( *pParamBlockGeometry, *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estHeightMapGeometry ) );
		}
		
		pDirtyParamBlockGeometry = false;
	}
	
	return pParamBlockGeometry;
}

deoglSPBlockUBO *deoglHTSTexture::GetParamBlockEnvMap(){
	return NULL;
}

void deoglHTSTexture::InvalidateParamBlocks(){
	pValidParamBlockDepth = false;
	pValidParamBlockGeometry = false;
	pValidParamBlockEnvMap = false;
	
	MarkParamBlocksDirty();
}

void deoglHTSTexture::MarkParamBlocksDirty(){
	pDirtyParamBlockDepth = true;
	pDirtyParamBlockGeometry = true;
	pDirtyParamBlockEnvMap = true;
}

void deoglHTSTexture::MarkTUCsDirty(){
	pDirtyTUCDepth = true;
	pDirtyTUCGeometry = true;
	pDirtyTUCShadow = true;
	pDirtyTUCEnvMap = true;
	pDirtyTUCLuminance = true;
}



deoglTexUnitsConfig *deoglHTSTexture::GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estHeightMapGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexture::estHeightMapDepth:
	case deoglSkinTexture::estHeightMapDepthClipPlane:
	case deoglSkinTexture::estHeightMapDepthReversed:
	case deoglSkinTexture::estHeightMapDepthClipPlaneReversed:
	case deoglSkinTexture::estHeightMapTranspCount:
	case deoglSkinTexture::estHeightMapTranspCountClipPlane:
		return GetTUCDepth();
		
	case deoglSkinTexture::estHeightMapShadowProjection:
	case deoglSkinTexture::estHeightMapShadowOrthogonal:
	case deoglSkinTexture::estHeightMapShadowDistance:
		return GetTUCShadow();
		
	case deoglSkinTexture::estHeightMapEnvMap:
		return GetTUCEnvMap();
		
	case deoglSkinTexture::estHeightMapLuminance:
		return GetTUCLuminance();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglHTSTexture::GetTUCDepth(){
	if( pDirtyTUCDepth ){
		if( pTUCDepth ){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		
		pTUCDepth = BareGetTUCFor( deoglSkinTexture::estHeightMapDepth );
		
		pDirtyTUCDepth = false;
	}
	
	return pTUCDepth;
}

deoglTexUnitsConfig *deoglHTSTexture::GetTUCGeometry(){
	if( pDirtyTUCGeometry ){
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		
		pTUCGeometry = BareGetTUCFor( deoglSkinTexture::estHeightMapGeometry );
		
		pDirtyTUCGeometry = false;
	}
	
	return pTUCGeometry;
}

deoglTexUnitsConfig *deoglHTSTexture::GetTUCShadow(){
	if( pDirtyTUCShadow ){
		if( pTUCShadow ){
			pTUCShadow->RemoveUsage();
			pTUCShadow = NULL;
		}
		
		pTUCShadow = BareGetTUCFor( deoglSkinTexture::estHeightMapShadowProjection );
		
		pDirtyTUCShadow = false;
	}
	
	return pTUCShadow;
}

deoglTexUnitsConfig *deoglHTSTexture::GetTUCEnvMap(){
	if( pDirtyTUCEnvMap ){
		if( pTUCEnvMap ){
			pTUCEnvMap->RemoveUsage();
			pTUCEnvMap = NULL;
		}
		
		deoglRenderThread &renderThread = pSector.GetHeightTerrain().GetRenderThread();
		
		if( pUseSkinTexture ){
			deoglRDynamicSkin *dynamicSkin = NULL;
			deoglSkinState *skinState = NULL;
			deoglTexUnitConfig unit[ 8 ];
			
			if( pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV() ){
				unit[ 0 ].EnableArrayTextureFromChannel( renderThread, *pUseSkinTexture, deoglSkinChannel::ectColor,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetColorArray() );
				
				unit[ 1 ].EnableArrayTextureFromChannel( renderThread, *pUseSkinTexture, deoglSkinChannel::ectEmissivity,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetEmissivityArray() );
				
			}else{
				unit[ 0 ].EnableTextureFromChannel( renderThread, *pUseSkinTexture, deoglSkinChannel::ectColor,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetColor() );
				
				unit[ 1 ].EnableTextureFromChannel( renderThread, *pUseSkinTexture, deoglSkinChannel::ectEmissivity,
					skinState, dynamicSkin, renderThread.GetDefaultTextures().GetEmissivity() );
			}
			
			pTUCEnvMap = renderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2,
				pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
			pTUCEnvMap->EnsureRTSTexture();
		}
		
		pDirtyTUCEnvMap = false;
	}
	
	return pTUCEnvMap;
}

deoglTexUnitsConfig *deoglHTSTexture::GetTUCLuminance(){
	if( pDirtyTUCLuminance ){
		if( pTUCLuminance ){
			pTUCLuminance->RemoveUsage();
			pTUCLuminance = NULL;
		}
		pTUCLuminance = BareGetTUCFor( deoglSkinTexture::estHeightMapLuminance );
		pDirtyTUCLuminance = false;
	}
	return pTUCLuminance;
}

deoglTexUnitsConfig *deoglHTSTexture::BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const{
	if( ! pUseSkinTexture ){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pSector.GetHeightTerrain().GetRenderThread();
	deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( shaderType );
	deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	int target;
	
	if( skinShader.GetUsedTextureTargetCount() > 0 ){
		skinShader.SetTUCCommon( &units[ 0 ], *pUseSkinTexture, skinState, dynamicSkin );
		skinShader.SetTUCPerObjectEnvMap( &units[ 0 ],
			pSector.GetHeightTerrain().GetParentWorld()->GetSkyEnvironmentMap(), NULL, NULL );
		
		target = skinShader.GetTextureTarget( deoglSkinShader::ettHeightMapMask );
		if( target != -1 ){
			deoglTexture ** const textureMasks = pSector.GetMaskTextures();
			const int arrayLayer = pIndex >> 2;
			
			if( textureMasks && textureMasks[ arrayLayer ] && textureMasks[ arrayLayer ]->GetTexture() ){
				units[ target ].EnableTexture( textureMasks[ arrayLayer ],
					renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
				
			}else{
				units[ target ].EnableTexture( renderThread.GetDefaultTextures().GetAO(),
					renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
			}
		}
		
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount(),
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	if( ! tuc ){
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith( NULL, 0,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	tuc->EnsureRTSTexture();
	
	return tuc;
}



void deoglHTSTexture::UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader ){
	deoglRDynamicSkin *useDynamicSkin = NULL;
	deoglSkinState *useSkinState = NULL;
	
	if( ! pUseSkinTexture ){
		return;
	}
	
	// prop field matrix
	const decDVector &referencePosition = pSector.GetHeightTerrain().GetParentWorld()->GetReferencePosition();
	const float sectorDim = pSector.GetHeightTerrain().GetSectorSize();
	const decPoint &scoord = pSector.GetCoordinates();
	const decDVector &htspos = decDVector( sectorDim * ( double )scoord.x, 0.0, sectorDim * ( double )scoord.y ) - referencePosition;
	decDMatrix matrixModel = decDMatrix::CreateTranslation( htspos );
	
	// update shader parameter block
	paramBlock.MapBuffer();
	try{
		int target;
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x3( target, matrixModel );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixNormal );
		if( target != -1 ){
			paramBlock.SetParameterDataMat3x3( target, decDMatrix() ); // inverse of 0-rotation
		}
		
		// per texture properties
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixTexCoord );
		if( target != -1 ){
			paramBlock.SetParameterDataMat3x2( target, pMatrix );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutHeightTerrainMaskTCTransform );
		if( target != -1 ){
			/*
			deImage * const image = pHTSector->GetSector()->GetTextureAt( pIndex )->GetMaskImage();
			int imageHeight = 1024;
			int imageWidth = 1024;
			
			if( image ){
				imageWidth = image->GetWidth();
				imageHeight = image->GetHeight();
			}
			
			paramBlock.SetParameterDataVec4( target, ( float )( imageWidth - 1 ) / sectorDim, ( float )( imageHeight - 1 ) / sectorDim,
				( float )( imageWidth - 1 ) * 0.5f, ( float )( imageHeight - 1 ) * 0.5f );
			*/
			paramBlock.SetParameterDataVec2( target, 1.0f / sectorDim, 1.0f / sectorDim );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutHeightTerrainMaskSelector );
		if( target != -1 ){
			paramBlock.SetParameterDataIVec2( target, 0 /*pIndex >> 2*/, pIndex & 0x3 );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutDoubleSided );
		if( target != -1 ){
			paramBlock.SetParameterDataBool( target, false );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutEnvMapFade );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, 0.0f ); // TODO
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutVariationSeed );
		if( target != -1 ){
			if( useSkinState ){
				paramBlock.SetParameterDataVec2( target, useSkinState->GetVariationSeed() );
				
			}else{
				paramBlock.SetParameterDataVec2( target, 0.0f, 0.0f );
			}
		}
		
		skinShader.SetTexParamsInInstParamSPB( paramBlock, *pUseSkinTexture );
		
		// per texture dynamic texture properties
		skinShader.SetDynTexParamsInInstParamSPB( paramBlock, *pUseSkinTexture, useSkinState, useDynamicSkin );
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



// Private functions
//////////////////////

void deoglHTSTexture::pCleanUp(){
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	if( pParamBlockDepth ){
		pParamBlockDepth->FreeReference();
		pParamBlockDepth = NULL;
	}
	if( pParamBlockGeometry ){
		pParamBlockGeometry->FreeReference();
		pParamBlockGeometry = NULL;
	}
	if( pParamBlockEnvMap ){
		pParamBlockEnvMap->FreeReference();
		pParamBlockEnvMap = NULL;
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
	if( pTUCLuminance ){
		pTUCLuminance->RemoveUsage();
		pTUCLuminance = NULL;
	}
}
