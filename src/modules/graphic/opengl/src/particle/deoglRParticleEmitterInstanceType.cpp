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

#include "deoglRParticleEmitter.h"
#include "deoglRParticleEmitterType.h"
#include "deoglRParticleEmitterInstance.h"
#include "deoglRParticleEmitterInstanceType.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../light/shader/deoglLightShader.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../sky/deoglRSkyInstance.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../world/deoglRWorld.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>



// Class deoglRParticleEmitterInstanceType
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRParticleEmitterInstanceType::deoglRParticleEmitterInstanceType(
deoglRParticleEmitterInstance &instance, int index ) :
pEmitterInstance( instance ),
pIndex( index ),

pFirstParticle( 0 ),
pParticleCount( 0 ),
pFirstIndex( 0 ),
pIndexCount( 0 ),

pDynamicSkin( NULL ),
pUseSkin( NULL ),
pUseTextureNumber( 0 ),
pUseSkinTexture( NULL ),

pParamBlock( NULL ),
pTUCDepth( NULL ),
pTUCCounter( NULL ),
pTUCGeometry( NULL ),
pTUCGeometryDepthTest( NULL ),

pValidParamBlock( false ),
pDirtyParamBlock( true ),

pDirtyTUCDepth( true ),
pDirtyTUCCounter( true ),
pDirtyTUCGeometry( true ),
pDirtyTUCGeometryDepthTest( true ),

pParamBlockLightInstance( NULL ),

pRTSInstance( NULL )
{
	LEAK_CHECK_CREATE( instance.GetRenderThread(), ParticleEmitterInstanceType );
	
	pRTSInstance = instance.GetRenderThread().GetRenderTaskSharedPool().GetInstance();
}

deoglRParticleEmitterInstanceType::~deoglRParticleEmitterInstanceType(){
	LEAK_CHECK_FREE( pEmitterInstance.GetRenderThread(), ParticleEmitterInstanceType );
	
	if( pRTSInstance ){
		pRTSInstance->ReturnToPool();
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pUseSkin ){
		pUseSkin->FreeReference();
	}
	if( pParamBlockLightInstance ){
		pParamBlockLightInstance->FreeReference();
	}
	if( pTUCGeometryDepthTest ){
		pTUCGeometryDepthTest->RemoveUsage();
	}
	if( pTUCGeometry ){
		pTUCGeometry->RemoveUsage();
	}
	if( pTUCDepth ){
		pTUCDepth->RemoveUsage();
	}
	if( pTUCCounter ){
		pTUCCounter->RemoveUsage();
	}
	if( pParamBlock ){
		pParamBlock->FreeReference();
	}
}



// Management
///////////////

void deoglRParticleEmitterInstanceType::SetFirstParticle( int first ){
	pFirstParticle = first;
}

void deoglRParticleEmitterInstanceType::SetParticleCount( int count ){
	pParticleCount = count;
}

void deoglRParticleEmitterInstanceType::SetFirstIndex( int index ){
	pFirstIndex = index;
}

void deoglRParticleEmitterInstanceType::SetIndexCount( int count ){
	pIndexCount = count;
}



void deoglRParticleEmitterInstanceType::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
	if( dynamicSkin == pDynamicSkin ){
		return;
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	
	pDynamicSkin = dynamicSkin;
	
	if( dynamicSkin ){
		dynamicSkin->AddReference();
	}
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
}

void deoglRParticleEmitterInstanceType::SetUseSkin( deoglRSkin *skin ){
	if( skin == pUseSkin ){
		return;
	}
	
	if( pUseSkin ){
		pUseSkin->FreeReference();
	}
	
	pUseSkin = skin;
	
	if( skin ){
		skin->AddReference();
	}
	
	// update texture
	if( pUseSkin && pUseSkin->GetTextureCount() > 0 ){
		pUseTextureNumber = 0;
		
	}else{
		pUseTextureNumber = -1;
	}
	
	if( pUseSkin && pUseTextureNumber != -1 ){
		pUseSkinTexture = &pUseSkin->GetTextureAt( pUseTextureNumber );
		
	}else{
		pUseSkinTexture = NULL;
	}
}



deoglSkinTexturePipelinesList::ePipelineTypes deoglRParticleEmitterInstanceType::GetSkinPipelinesType() const{
	switch( pEmitterInstance.GetEmitter()->GetTypeAt( pIndex ).GetSimulationType() ){
	case deParticleEmitterType::estParticle:
		return deoglSkinTexturePipelinesList::eptParticle;
		
	case deParticleEmitterType::estBeam:
		return deoglSkinTexturePipelinesList::eptParticleBeam;
		
	case deParticleEmitterType::estRibbon:
		return deoglSkinTexturePipelinesList::eptParticleRibbon;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

const deoglSkinTexturePipelines &deoglRParticleEmitterInstanceType::GetUseSkinPipelines() const{
	DEASSERT_NOTNULL( pUseSkinTexture )
	return pUseSkinTexture->GetPipelines().GetAt( GetSkinPipelinesType() );
}

deoglSPBlockUBO *deoglRParticleEmitterInstanceType::GetParamBlock(){
	if( ! pValidParamBlock ){
		if( pParamBlock ){
			pParamBlock->FreeReference();
			pParamBlock = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = GetUseSkinPipelines().
				GetWithRef( deoglSkinTexturePipelines::etGeometry ).GetShader();
			
			/*if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockDepth = new deoglSPBlockUBO( *pEmitterInstance.GetRenderThread()
					.GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{*/
				pParamBlock = skinShader.CreateSPBInstParam();
			//}
		}
		
		pValidParamBlock = true;
		pDirtyParamBlock = true;
	}
	
	if( pDirtyParamBlock ){
		if( pParamBlock ){
			deoglSkinShader &skinShader = GetUseSkinPipelines().
				GetWithRef( deoglSkinTexturePipelines::etGeometry ).GetShader();
			
			UpdateInstanceParamBlock( *pParamBlock, skinShader );
		}
		
		pDirtyParamBlock = false;
	}
	
	return pParamBlock;
}

deoglTexUnitsConfig *deoglRParticleEmitterInstanceType::GetTUCForPipelineType(
deoglSkinTexturePipelines::eTypes type ){
	switch( type ){
	case deoglSkinTexturePipelines::etGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexturePipelines::etGeometryDepthTest:
		return GetTUCGeometryDepthTest();
		
	case deoglSkinTexturePipelines::etDepth:
	case deoglSkinTexturePipelines::etDepthClipPlane:
	case deoglSkinTexturePipelines::etDepthReversed:
	case deoglSkinTexturePipelines::etDepthClipPlaneReversed:
	case deoglSkinTexturePipelines::etShadowProjection:
	case deoglSkinTexturePipelines::etShadowProjectionCube:
	case deoglSkinTexturePipelines::etShadowOrthogonal:
	case deoglSkinTexturePipelines::etShadowDistance:
	case deoglSkinTexturePipelines::etShadowDistanceCube:
	case deoglSkinTexturePipelines::etMask:
		return GetTUCDepth();
		
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
		return GetTUCCounter();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglRParticleEmitterInstanceType::GetTUCDepth(){
	if( pDirtyTUCDepth ){
		if( pTUCDepth ){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		
		pTUCDepth = BareGetTUCFor( deoglSkinTexturePipelines::etDepth );
		
		pDirtyTUCDepth = false;
	}
	
	return pTUCDepth;
}

deoglTexUnitsConfig *deoglRParticleEmitterInstanceType::GetTUCCounter(){
	if( pDirtyTUCCounter ){
		if( pTUCCounter ){
			pTUCCounter->RemoveUsage();
			pTUCCounter = NULL;
		}
		
		pTUCCounter = BareGetTUCFor( deoglSkinTexturePipelines::etCounter );
		
		pDirtyTUCCounter = false;
	}
	
	return pTUCCounter;
}

deoglTexUnitsConfig *deoglRParticleEmitterInstanceType::GetTUCGeometry(){
	if( pDirtyTUCGeometry ){
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		
		pTUCGeometry = BareGetTUCFor( deoglSkinTexturePipelines::etGeometry );
		
		pDirtyTUCGeometry = false;
	}
	
	return pTUCGeometry;
}

deoglTexUnitsConfig *deoglRParticleEmitterInstanceType::GetTUCGeometryDepthTest(){
	if( pDirtyTUCGeometryDepthTest ){
		if( pTUCGeometryDepthTest ){
			pTUCGeometryDepthTest->RemoveUsage();
			pTUCGeometryDepthTest = NULL;
		}
		
		pTUCGeometryDepthTest = BareGetTUCFor( deoglSkinTexturePipelines::etGeometryDepthTest );
		
		pDirtyTUCGeometryDepthTest = false;
	}
	
	return pTUCGeometryDepthTest;
}

deoglTexUnitsConfig *deoglRParticleEmitterInstanceType::BareGetTUCFor(
deoglSkinTexturePipelines::eTypes shaderType ) const{
	if( ! pUseSkinTexture ){
		return NULL;
	}
	
	deoglSkinShader &skinShader = GetUseSkinPipelines().GetWithRef( shaderType ).GetShader();
	deoglRenderThread &renderThread = pEmitterInstance.GetRenderThread();
	deoglTexUnitsConfig *tuc = NULL;
	
	if( skinShader.GetUsedTextureTargetCount() > 0 ){
		deoglRDynamicSkin *dynamicSkin = NULL;
		deoglSkinState *skinState = NULL;
		
		if( pDynamicSkin ){
			dynamicSkin = pDynamicSkin;
			//skinState = pSkinState;
			
		}else{
			//dynamicSkin = pEmitterInstance->GetDynamicSkin();
			//skinState = pEmitterInstance->GetSkinState();
		}
		
		// set common textures. for these adjust the samplers to linear without mip mapping. this is
		// required since particles are usually small and ribbons and beams usually narrow. in this
		// situation opengl starts mip mapping a lot resulting in poor results
		deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
		int target;
		
		skinShader.SetTUCCommon( &units[ 0 ], *pUseSkinTexture, skinState, dynamicSkin );
		
		for( target=0; target<deoglSkinShader::ETT_COUNT; target++ ){
			if( units[ target ].IsEnabled() ){
				if( units[ target ].GetSampler() == renderThread.GetShader()
				.GetTexSamplerConfig( deoglRTShader::etscClampNearestMipMap ) ){
					units[ target ].SetSampler( renderThread.GetShader()
						.GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
					
				}else if( units[ target ].GetSampler() == renderThread.GetShader()
				.GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap ) ){
					units[ target ].SetSampler( renderThread.GetShader()
						.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
					
				}else if( units[ target ].GetSampler() == renderThread.GetShader()
				.GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMapNearest ) ){
					units[ target ].SetSampler( renderThread.GetShader()
						.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
					
				}else if( units[ target ].GetSampler() == renderThread.GetShader()
				.GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) ){
					units[ target ].SetSampler( renderThread.GetShader()
						.GetTexSamplerConfig( deoglRTShader::etscRepeatLinear ) );
					
				}else{ // not mip mapped or we don't know what it is. either way leave it as it is
				}
			}
		}
		
		// set environment map textures
		skinShader.SetTUCPerObjectEnvMap( &units[ 0 ],
			pEmitterInstance.GetParentWorld()->GetSkyEnvironmentMap(),
			pEmitterInstance.GetRenderEnvMap(), pEmitterInstance.GetRenderEnvMap() );
		
		// set curve sampler texture
		target = skinShader.GetTextureTarget( deoglSkinShader::ettSamples );
		if( target != -1 ){
			units[ target ].EnableTexture(
				pEmitterInstance.GetEmitter()->GetTypeAt( pIndex ).GetTextureSamples(),
				renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
		}
		
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount(),
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	if( ! tuc ){
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith( NULL, 0,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	tuc ->EnsureRTSTexture();
	
	return tuc;
}

void deoglRParticleEmitterInstanceType::InvalidateParamBlocks(){
	pValidParamBlock = false;
	MarkParamBlocksDirty();
}

void deoglRParticleEmitterInstanceType::MarkParamBlocksDirty(){
	pDirtyParamBlock = true;
}

void deoglRParticleEmitterInstanceType::MarkTUCsDirty(){
	pDirtyTUCDepth = true;
	pDirtyTUCCounter = true;
	pDirtyTUCGeometry = true;
	pDirtyTUCGeometryDepthTest = true;
}



void deoglRParticleEmitterInstanceType::UpdateInstanceParamBlock(
deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader ){
	if( ! pUseSkinTexture ){
		return;
	}
	
	//deoglRDynamicSkin *useDynamicSkin = pDynamicSkin; //pComponent->GetDynamicSkin();
	//deoglSkinState *useSkinState = NULL; //pComponent->GetSkinState();
	
	// update shader parameter block
	paramBlock.MapBuffer();
	try{
		int target;
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
		if( target != -1 ){
			const decDVector &referencePosition = pEmitterInstance.GetParentWorld()->GetReferencePosition();
			const decDVector &particlePosition = pEmitterInstance.GetReferencePosition();
			const decDMatrix matrix = decDMatrix::CreateTranslation( particlePosition - referencePosition );
			
			paramBlock.SetParameterDataMat4x3( target, matrix );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutSamplesParams );
		if( target != -1 ){
			//const float width = 256.0f;
			//const float height = 4.0f;
			//paramBlock.SetParameterDataVec4( target, 255.0f / width, 0.5f / width, 1.0f / height, 0.5f / height );
			paramBlock.SetParameterDataVec4( target, 255.0f / 256.0f, 0.5f / 256.0f, 1.0f / 4.0f, 0.5f / 4.0f );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBurstFactor );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, pEmitterInstance.GetBurstTime() );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutRibbonSheetCount );
		if( target != -1 ){
			const int sheetCount = 3;
			paramBlock.SetParameterDataInt( target, sheetCount );
		}
		
		skinShader.SetTexParamsInInstParamSPB( paramBlock, *pUseSkinTexture );
		
		// per texture dynamic texture properties
		//skinShader.SetDynTexParamsInInstParamSPB( paramBlock, *pUseSkinTexture, useSkinState, useDynamicSkin );
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



deoglSPBlockUBO *deoglRParticleEmitterInstanceType::GetLightInstanceParameterBlock(){
	if( ! pParamBlockLightInstance ){
		pParamBlockLightInstance = pEmitterInstance.GetEmitter()->GetTypeAt( pIndex ).GetPipelines().
			GetWithRef( deoglLightPipelines::etNoShadow, 0 ).GetShader()->CreateSPBInstParam();
	}
	
	return pParamBlockLightInstance;
}

void deoglRParticleEmitterInstanceType::DropLightBlocks(){
	if( pParamBlockLightInstance ){
		pParamBlockLightInstance->FreeReference();
		pParamBlockLightInstance = NULL;
	}
}
