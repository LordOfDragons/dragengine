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

#include "deoglRDecal.h"
#include "deoglDecalMeshBuilder.h"
#include "deoglDecalMeshBuilderFace.h"
#include "../component/deoglRComponent.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../extensions/deoglExtResult.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/face/deoglModelFace.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/writer/deoglVBOWriterModel.h"
#include "../world/deoglRWorld.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDecal
//////////////////////

// Constructor, destructor
////////////////////////////

deoglRDecal::deoglRDecal( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pSize( 0.5f, 0.5f, 1.0f ),
pVisible( true ),

pParentComponent( NULL ),
pComponentMarkedRemove( false ),

pSharedSPBElement( NULL ),

pDirtySharedSPBElement( true )
{
	pSkin = NULL;
	pDynamicSkin = NULL;
	pSkinState = NULL;
	
	pUseSkin = NULL;
	pUseTextureNumber = -1;
	pUseSkinTexture = NULL;
	pUseDynamicSkin = NULL;
	pUseSkinState = NULL;
	
	pDirtyPrepareSkinStateRenderables = true;
	
	pVBOBlock = NULL;
	pPointCount = 0;
	
	pDirtyUseTexture = true;
	pDirtyVBO = true;
	
	pParamBlockGeometry = NULL;
	pParamBlockEnvMap = NULL;
	
	pTUCGeometry = NULL;
	pTUCShadow = NULL;
	pTUCEnvMap = NULL;
	
	pValidParamBlockGeometry = false;
	pValidParamBlockEnvMap = false;
	pDirtyParamBlockGeometry = true;
	pDirtyParamBlockEnvMap = true;
	
	pDirtyTUCGeometry = true;
	pDirtyTUCShadow = true;
	pDirtyTUCEnvMap = true;
	LEAK_CHECK_CREATE( renderThread, Decal );
}

class deoglRDecalDeletion : public deoglDelayedDeletion{
public:
	deoglSkinState *skinState;
	deoglSharedVBOBlock *vboBlock;
	deoglSPBlockUBO *paramBlockGeometry;
	deoglSPBlockUBO *paramBlockEnvMap;
	deoglTexUnitsConfig *tucGeometry;
	deoglTexUnitsConfig *tucShadow;
	deoglTexUnitsConfig *tucEnvMap;
	
	deoglRDecalDeletion() :
	skinState( NULL ),
	vboBlock( NULL ),
	paramBlockGeometry( NULL ),
	paramBlockEnvMap( NULL ),
	tucGeometry( NULL ),
	tucShadow( NULL ),
	tucEnvMap( NULL ){
	}
	
	virtual ~deoglRDecalDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( vboBlock ){
			vboBlock->GetVBO()->RemoveBlock( vboBlock );
			vboBlock->FreeReference();
		}
		if( tucGeometry ){
			tucGeometry->RemoveUsage();
		}
		if( tucShadow ){
			tucShadow->RemoveUsage();
		}
		if( tucEnvMap ){
			tucEnvMap->RemoveUsage();
		}
		if( paramBlockGeometry ){
			paramBlockGeometry->FreeReference();
		}
		if( paramBlockEnvMap ){
			paramBlockEnvMap->FreeReference();
		}
		if( skinState ){
			delete skinState;
		}
	}
};

deoglRDecal::~deoglRDecal(){
	LEAK_CHECK_FREE( pRenderThread, Decal );
	
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	if( pSharedSPBElement ){
		pSharedSPBElement->FreeReference();
	}
	
	// drop reference otherwise deletion can cause other deletions to be generated
	// causing a deletion race
	if( pSkinState ){
		pSkinState->DropDelayedDeletionObjects();
	}
	
	// delayed deletion of opengl containing objects
	deoglRDecalDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRDecalDeletion;
		delayedDeletion->paramBlockEnvMap = pParamBlockEnvMap;
		delayedDeletion->paramBlockGeometry = pParamBlockGeometry;
		delayedDeletion->skinState = pSkinState;
		delayedDeletion->tucEnvMap = pTUCEnvMap;
		delayedDeletion->tucGeometry = pTUCGeometry;
		delayedDeletion->tucShadow = pTUCShadow;
		delayedDeletion->vboBlock = pVBOBlock;
		pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pRenderThread.GetLogger().LogException( e );
		//throw; -> otherwise terminate
	}
}



// Management
///////////////

void deoglRDecal::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoglRDecal::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deoglRDecal::SetSize( const decVector &size ){
	pSize = size;
}

void deoglRDecal::SetTransform( const decTexMatrix2 &matrix ){
	pTransform = matrix;
}

void deoglRDecal::SetVisible( bool visible ){
	pVisible = visible;
}



void deoglRDecal::UpdateSkin( float elapsed ){
	if( ! pSkinState ){
		return;
	}
	
	pSkinState->AdvanceTime( elapsed );
	
	if( ! pUseSkinTexture ){
		return;
	}
	
	if( pUseSkinTexture->GetDynamicChannels() ){
		MarkParamBlocksDirty();
		MarkTUCsDirty();
		
	}else if( pUseSkinTexture->GetCalculatedProperties() ){
		MarkParamBlocksDirty();
	}
}

void deoglRDecal::SetDirtyVBO(){
	if( pDirtyVBO ){
		return;
	}
	
	pDirtyVBO = true;
	
	if( pParentComponent ){
		pParentComponent->DecalRequiresPrepareForRender();
	}
}



void deoglRDecal::SetSkin( deoglRSkin *skin ){
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
	
	pDirtyUseTexture = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	UpdateSkinState();
	if( pSkinState ){
		pSkinState->InitCalculatedProperties();
	}
	
	pRequiresPrepareForRender();
}

void deoglRDecal::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
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
	
	pDirtyUseTexture = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	UpdateSkinState();
	
	pRequiresPrepareForRender();
}

void deoglRDecal::SetSkinState( deoglSkinState *skinState ){
	if( skinState == pSkinState ){
		return;
	}
	
	if( pSkinState ){
		// drop reference otherwise deletion can cause other deletions to be generated
		// causing a deletion race
		pSkinState->DropDelayedDeletionObjects();
		
		// delayed deletion of opengl containing objects
		deoglRDecalDeletion *delayedDeletion = NULL;
		
		try{
			delayedDeletion = new deoglRDecalDeletion;
			delayedDeletion->skinState = pSkinState;
			pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
			
		}catch( const deException &e ){
			if( delayedDeletion ){
				delete delayedDeletion;
			}
			pRenderThread.GetLogger().LogException( e );
			throw;
		}
	}
	
	pSkinState = skinState;
	
	pDirtyUseTexture = true;
	pDirtyPrepareSkinStateRenderables = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	UpdateSkinState();
	
	pRequiresPrepareForRender();
}

void deoglRDecal::UpdateUseSkin(){
	if( ! pDirtyUseTexture ){
		return;
	}
	
	pUseSkin = NULL;
	pUseTextureNumber = -1;
	pUseSkinTexture = NULL;
	pUseDynamicSkin = NULL;
	pUseSkinState = NULL;
	
	if( pSkinState ){
		if( pDynamicSkin ){
			pUseSkinState = pSkinState;
			pUseDynamicSkin = pDynamicSkin;
			
		}else if( pParentComponent ){
			pUseSkinState = pSkinState;
			pUseDynamicSkin = pParentComponent->GetDynamicSkin();
		}
		
	}else if( pParentComponent ){
		// for textures with no own skin
		pUseSkinState = pParentComponent->GetSkinState();
		pUseDynamicSkin = pParentComponent->GetDynamicSkin();
	}
	
	if( pSkin ){
		pUseSkin = pSkin;
		
		if( pUseSkin->GetTextureCount() == 0 ){
			pUseTextureNumber = -1;
			
		}else{
			pUseTextureNumber = 0;
		}
	}
	
	if( pUseSkin && pUseTextureNumber != -1 ){
		pUseSkinTexture = &pUseSkin->GetTextureAt( pUseTextureNumber );
	}
	
	pDirtyUseTexture = false;
}

void deoglRDecal::UpdateSkinState(){
	// NOTE this is called from the main thread during synchronization
	
	bool changed = false;
	
	if( pSkin && ( pDynamicSkin || ( pParentComponent && pParentComponent->GetDynamicSkin() ) ) ){
		if( ! pSkinState ){
			pSkinState = new deoglSkinState( pRenderThread, *this );
			changed = true;
		}
		
	}else{
		if( pSkinState ){
			SetSkinState( NULL );
			changed = true;
		}
	}
	
	if( changed ){
		pDirtyUseTexture = true;
		DirtyPrepareSkinStateRenderables();
		
		InvalidateParamBlocks();
		MarkTUCsDirty();
		
		pRequiresPrepareForRender();
	}
}



void deoglRDecal::DirtyPrepareSkinStateRenderables(){
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyUseTexture = true;
	
	pRequiresPrepareForRender();
}

void deoglRDecal::PrepareSkinStateRenderables(){
	if( pSkinState ){
		pSkinState->PrepareRenderables( pSkin, pDynamicSkin );
	}
}



void deoglRDecal::SetParentComponent( deoglRComponent *component ){
	// NOTE this is called from the main thread during synchronization
	if( component == pParentComponent ){
		return;
	}
	
	pParentComponent = component;
	SetDirtyVBO();
	
	SetSkinState( NULL ); // required since UpdateSkinState can not figure out dynamic skin changed
	UpdateSkinState();
}

void deoglRDecal::SetComponentMarkedRemove( bool marked ){
	pComponentMarkedRemove = marked;
}



deoglSPBlockUBO *deoglRDecal::GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estDecalGeometry:
		return GetParamBlockGeometry();
		
	case deoglSkinTexture::estDecalEnvMap:
		return GetParamBlockEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglSPBlockUBO *deoglRDecal::GetParamBlockGeometry(){
	if( ! pValidParamBlockGeometry ){
		if( pParamBlockGeometry ){
			pParamBlockGeometry->FreeReference();
			pParamBlockGeometry = NULL;
		}
		
		if( pUseSkinTexture && ! deoglSkinShader::USE_SHARED_SPB ){
			pParamBlockGeometry = pUseSkinTexture->GetShaderFor(
				deoglSkinTexture::estDecalGeometry )->CreateSPBInstParam();
		}
		
		pValidParamBlockGeometry = true;
		pDirtyParamBlockGeometry = true;
	}
	
	if( pDirtyParamBlockGeometry ){
		if( pParamBlockGeometry ){
			pParamBlockGeometry->MapBuffer();
			try{
				UpdateInstanceParamBlock( *pParamBlockGeometry, 0,
					*pUseSkinTexture->GetShaderFor( deoglSkinTexture::estDecalGeometry ) );
				
			}catch( const deException & ){
				pParamBlockGeometry->UnmapBuffer();
				throw;
			}
			pParamBlockGeometry->UnmapBuffer();
		}
		
		pDirtyParamBlockGeometry = false;
	}
	
	return pParamBlockGeometry;
}

deoglSPBlockUBO *deoglRDecal::GetParamBlockEnvMap(){
	return NULL;
}

deoglSharedSPBElement *deoglRDecal::GetSharedSPBElement(){
	if( ! pSharedSPBElement ){
		if( pRenderThread.GetChoices().GetSharedSPBUseSSBO() ){
			pSharedSPBElement = pRenderThread.GetBufferObject().GetSharedSPBList(
				deoglRTBufferObject::esspblSkinInstanceSSBO ).AddElement();
			
		}else{
			pSharedSPBElement = pRenderThread.GetBufferObject().GetSharedSPBList(
				deoglRTBufferObject::esspblSkinInstanceUBO ).AddElement();
		}
	}
	
	if( pDirtySharedSPBElement ){
		if( pSharedSPBElement && pUseSkinTexture ){
			// it does not matter which shader type we use since all are required to use the
			// same shared spb instance layout
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor(
				deoglSkinTexture::estComponentGeometry );
			
			// update parameter block area belonging to this element
			deoglShaderParameterBlock &paramBlock = pSharedSPBElement->MapBuffer();
			try{
				UpdateInstanceParamBlock( paramBlock, pSharedSPBElement->GetIndex(), skinShader );
				
			}catch( const deException & ){
				paramBlock.UnmapBuffer();
				throw;
			}
			paramBlock.UnmapBuffer();
		}
		
		pDirtySharedSPBElement = false;
	}
	
	return pSharedSPBElement;
}

deoglTexUnitsConfig *deoglRDecal::GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estDecalGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexture::estDecalEnvMap:
		return GetTUCEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglRDecal::GetTUCGeometry(){
	if( pDirtyTUCGeometry ){
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		
		pTUCGeometry = BareGetTUCFor( deoglSkinTexture::estDecalGeometry );
		
		pDirtyTUCGeometry = false;
	}
	
	return pTUCGeometry;
}

deoglTexUnitsConfig *deoglRDecal::GetTUCShadow(){
	if( pDirtyTUCShadow ){
		if( pTUCShadow ){
			pTUCShadow->RemoveUsage();
			pTUCShadow = NULL;
		}
		
		pTUCShadow = BareGetTUCFor( deoglSkinTexture::estComponentShadowProjection );
		
		pDirtyTUCShadow = false;
	}
	
	return pTUCShadow;
}

deoglTexUnitsConfig *deoglRDecal::GetTUCEnvMap(){
	if( pDirtyTUCEnvMap ){
		UpdateUseSkin();
		
		if( pTUCEnvMap ){
			pTUCEnvMap->RemoveUsage();
			pTUCEnvMap = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglTexUnitConfig unit[ 8 ];
			
			if( pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV() ){
				unit[ 0 ].EnableArrayTextureFromChannel( pRenderThread, *pUseSkinTexture, deoglSkinChannel::ectColor,
					NULL, NULL, pRenderThread.GetDefaultTextures().GetColorArray() );
				
				unit[ 1 ].EnableArrayTextureFromChannel( pRenderThread, *pUseSkinTexture, deoglSkinChannel::ectEmissivity,
					NULL, NULL, pRenderThread.GetDefaultTextures().GetEmissivityArray() );
				
			}else{
				unit[ 0 ].EnableTextureFromChannel( pRenderThread, *pUseSkinTexture, deoglSkinChannel::ectColor,
					NULL, NULL, pRenderThread.GetDefaultTextures().GetColor() );
				
				unit[ 1 ].EnableTextureFromChannel( pRenderThread, *pUseSkinTexture, deoglSkinChannel::ectEmissivity,
					NULL, NULL, pRenderThread.GetDefaultTextures().GetEmissivity() );
			}
			
			pTUCEnvMap = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2 );
		}
		
		pDirtyTUCEnvMap = false;
	}
	
	return pTUCEnvMap;
}

deoglTexUnitsConfig *deoglRDecal::BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ){
	deoglTexUnitsConfig *tuc = NULL;
	
	UpdateUseSkin();
	
	if( pUseSkinTexture ){
		deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
		deoglEnvironmentMap *envmapSky = NULL;
		
		if( pParentComponent ){
			envmapSky = pParentComponent->GetParentWorld()->GetSkyEnvironmentMap();
		}
		
		deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( shaderType );
		
		if( skinShader.GetUsedTextureTargetCount() > 0 ){
			skinShader.SetTUCCommon( &units[ 0 ], *pUseSkinTexture, pUseSkinState, pUseDynamicSkin );
			skinShader.SetTUCPerObjectEnvMap( &units[ 0 ], envmapSky,
				pParentComponent->GetRenderEnvMap(), pParentComponent->GetRenderEnvMapFade() );
			tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith(
				&units[ 0 ], skinShader.GetUsedTextureTargetCount() );
		}
	}
	
	return tuc;
}

void deoglRDecal::InvalidateParamBlocks(){
	pValidParamBlockGeometry = false;
	pValidParamBlockEnvMap = false;
	
	MarkParamBlocksDirty();
}

void deoglRDecal::MarkParamBlocksDirty(){
	pDirtyParamBlockGeometry = true;
	pDirtyParamBlockEnvMap = true;
	pDirtySharedSPBElement = true;
}

void deoglRDecal::MarkTUCsDirty(){
	pDirtyTUCGeometry = true;
	pDirtyTUCShadow = true;
	pDirtyTUCEnvMap = true;
}

void deoglRDecal::UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
int element, deoglSkinShader &skinShader ){
	UpdateUseSkin();
	if( ! pUseSkinTexture ){
		return;
	}
	
	int target;
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
	if( target != -1 ){
		decDVector referencePosition;
		decDMatrix matrix;
		
		if( pParentComponent ){
			referencePosition = pParentComponent->GetParentWorld()->GetReferencePosition();
			matrix = pParentComponent->GetMatrix();
		}
		
		matrix.a14 -= referencePosition.x;
		matrix.a24 -= referencePosition.y;
		matrix.a34 -= referencePosition.z;
		
		paramBlock.SetParameterDataMat4x3( target, element, matrix );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixNormal );
	if( target != -1 ){
		if( pParentComponent ){
			paramBlock.SetParameterDataMat3x3( target, element,
				pParentComponent->GetMatrix().GetRotationMatrix().Invert() );
			
		}else{
			paramBlock.SetParameterDataMat3x3( target, element, decMatrix() );
		}
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixTexCoord );
	if( target != -1 ){
		paramBlock.SetParameterDataMat3x2( target, element, pTransform );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutDoubleSided );
	if( target != -1 ){
		paramBlock.SetParameterDataBool( target, element, true );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutEnvMapFade );
	if( target != -1 ){
		if( pParentComponent ){
			paramBlock.SetParameterDataFloat( target, element,
				pParentComponent->GetRenderEnvMapFadeFactor() );
			
		}else{
			paramBlock.SetParameterDataFloat( target, element, 0.0f );
		}
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutVariationSeed );
	if( target != -1 ){
		if( pUseSkinState ){
			paramBlock.SetParameterDataVec2( target, element, pUseSkinState->GetVariationSeed() );
			
		}else{
			paramBlock.SetParameterDataVec2( target, element, 0.0f, 0.0f );
		}
	}
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB( paramBlock, element,
		*pUseSkinTexture, pUseSkinState, pUseDynamicSkin );
}



void deoglRDecal::PrepareForRender( deoglRenderPlan &plan ){
	pPrepareVBO();
	
	if( pDirtyPrepareSkinStateRenderables ){
		PrepareSkinStateRenderables();
		pDirtyPrepareSkinStateRenderables = false;
	}
}

void deoglRDecal::PrepareQuickDispose(){
	pParentComponent = NULL;
}

void deoglRDecal::DynamicSkinRenderablesChanged(){
	if( ! pDynamicSkin || ! pSkin || ! pSkin->GetHasRenderables() ){
		return;
	}
	
	MarkParamBlocksDirty();
	MarkTUCsDirty();
}

void deoglRDecal::UpdateRenderableMapping(){
	if( ! pSkinState ){
		return;
	}
	
	// udpate mappings of dynamic skin of component itself
	pSkinState->RemoveAllRenderables();
	if( pSkin && pDynamicSkin ){
		pSkinState->AddRenderables( *pSkin, *pDynamicSkin );
	}
	
	pDirtyUseTexture = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	pRequiresPrepareForRender();
}




// Private Functions
//////////////////////

// #include <dragengine/common/utils/decTimer.h>
void deoglRDecal::pCreateMeshComponent(){
	if( pVBOBlock ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pParentComponent->GetModel() ){
		return;
	}
	
	decTimer timer;
	deoglDecalMeshBuilder meshBuilder( pRenderThread );
	meshBuilder.Init( *this, pSize.z );
	meshBuilder.BuildMeshForComponent( *pParentComponent );
// 	pRenderThread.GetLogger().LogInfoFormat( "deoglDecalMeshBuilder: decal=%p(%f,%f,%f) build=%.3fms",
// 		this, pPosition.x, pPosition.y, pPosition.z, timer.GetElapsedTime()*1e3f );
// 	meshBuilder.Debug();
	
	const int faceCount = meshBuilder.GetFaceCount();
	pPointCount = faceCount * 3;
	if( pPointCount == 0 ){
		return;
	}
	
	// copy mesh and project uvs
	deoglSharedVBOList &svbolist = pRenderThread.GetBufferObject()
		.GetSharedVBOListForType( deoglRTBufferObject::esvbolStaticModel );
	
	if( pPointCount > svbolist.GetMaxPointCount() ){
		pRenderThread.GetLogger().LogInfoFormat(
			"Decal: Too many points (%i) to fit into shared VBOs. "
			"Using over-sized VBO (performance not optimal).", pPointCount );
	}
	
	pVBOBlock = svbolist.AddData( pPointCount );
	
	// get decal matrix and projection axis
	const decMatrix decalMatrix( decMatrix::CreateWorld( pPosition, pOrientation ) );
	const decMatrix inverseDecalMatrix( decalMatrix.Invert() );
	
	/*
	// get texture coordinates
	const decVector2 tcs[ 4 ] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	
	// calculate vertices
	decVector halfSize( pSize * 0.5f );
	const decVector vertices[ 4 ] = {
		decalMatrix * decVector(  halfSize.x,  halfSize.y, 0.0f ),
		decalMatrix * decVector( -halfSize.x,  halfSize.y, 0.0f ),
		decalMatrix * decVector( -halfSize.x, -halfSize.y, 0.0f ),
		decalMatrix * decVector(  halfSize.x, -halfSize.y, 0.0f ) };
	
	// calculate decal normal and tangent
	const decVector edges[ 2 ] = { vertices[ 1 ] - vertices[ 0 ], vertices[ 2 ] - vertices[ 1 ] };
	const float d1v = tcs[ 1 ].y - tcs[ 0 ].y;
	const float d2v = tcs[ 2 ].y - tcs[ 0 ].y;
	
	const decVector decalTangent( decVector(
		edges[ 0 ].x * d2v - edges[ 1 ].x * d1v,
		edges[ 0 ].y * d2v - edges[ 1 ].y * d1v,
		edges[ 0 ].z * d2v - edges[ 1 ].z * d1v ).Normalized() );
	*/
	const decVector decalTangent( -decalMatrix.TransformRight().Normalized() );
	
	// write data
	deoglVBOWriterModel writerVBO( pRenderThread );
	
	writerVBO.Reset( pVBOBlock );
	
	const float invHalfSizeX = 1.0f / pSize.x;
	const float intHalfSizeY = 1.0f / pSize.y;
	int i;
	
	for( i=0; i<faceCount; i++ ){
		const deoglDecalMeshBuilderFace &face = *meshBuilder.GetFaceAt( i );
		const decVector &normal = face.GetFaceNormal();
		const decVector &vertex1 = meshBuilder.GetPointAt( face.GetPoint3() );
		const decVector &vertex2 = meshBuilder.GetPointAt( face.GetPoint2() );
		const decVector &vertex3 = meshBuilder.GetPointAt( face.GetPoint1() );
		
		const decVector backProject1( inverseDecalMatrix * vertex1 );
		writerVBO.WritePoint( vertex1, normal, decalTangent, false,
			decVector2( 0.5f - backProject1.x * invHalfSizeX, 0.5f - backProject1.y * intHalfSizeY ),
			normal );
		
		const decVector backProject2( inverseDecalMatrix * vertex2 );
		writerVBO.WritePoint( vertex2, normal, decalTangent, false,
			decVector2( 0.5f - backProject2.x * invHalfSizeX, 0.5f - backProject2.y * intHalfSizeY ),
			normal );
		
		const decVector backProject3( inverseDecalMatrix * vertex3 );
		writerVBO.WritePoint( vertex3, normal, decalTangent, false,
			decVector2( 0.5f - backProject3.x * invHalfSizeX, 0.5f - backProject3.y * intHalfSizeY ),
			normal );
	}
// 	pRenderThread.GetLogger().LogInfoFormat( "deoglDecalMeshBuilder: decal=%p(%f,%f,%f) vbo=%.3fms",
// 		this, pPosition.x, pPosition.y, pPosition.z, timer.GetElapsedTime()*1e3f );
}

void deoglRDecal::pPrepareVBO(){
	if( ! pDirtyVBO ){
		return;
	}
	
	if( pVBOBlock ){
		pVBOBlock->GetVBO()->RemoveBlock( pVBOBlock );
		pVBOBlock->FreeReference();
		pVBOBlock = NULL;
	}
	
	if( pParentComponent ){
		pCreateMeshComponent();
	}
	
	pDirtyVBO = false;
}

void deoglRDecal::pRequiresPrepareForRender(){
	if( pParentComponent ){
		pParentComponent->DecalRequiresPrepareForRender();
	}
}
