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

#include <stdlib.h>
#include <stdio.h>

#include "deoglRComponent.h"
#include "deoglRComponentTexture.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/texture/deoglModelTexture.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
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
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRComponentTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRComponentTexture::deoglRComponentTexture( deoglRComponent &component, int index ) :
pComponent( component ),
pIndex( index ),

pSkin( NULL ),
pDynamicSkin( NULL ),
pSkinState( NULL ),

pUseSkin( NULL ),
pUseTextureNumber( 0 ),
pUseSkinTexture( NULL ),
pUseSkinState( NULL ),
pUseDoubleSided( false ),
pUseDecal( false ),

pParamBlockDepth( NULL ),
pParamBlockGeometry( NULL ),
pParamBlockEnvMap( NULL ),
pSharedSPBElement( NULL ),

pTUCDepth( NULL ),
pTUCGeometry( NULL ),
pTUCCounter( NULL ),
pTUCShadow( NULL ),
pTUCShadowCube( NULL ),
pTUCEnvMap( NULL ),
pTUCOutlineDepth( NULL ),
pTUCOutlineGeometry( NULL ),
pTUCOutlineCounter( NULL ),

pValidParamBlockDepth( false ),
pValidParamBlockGeometry( false ),
pValidParamBlockEnvMap( false ),
pDirtyParamBlockDepth( true ),
pDirtyParamBlockGeometry( true ),
pDirtyParamBlockEnvMap( true ),
pDirtySharedSPBElement( true ),

pDirtyTUCDepth( true ),
pDirtyTUCGeometry( true ),
pDirtyTUCCounter( true ),
pDirtyTUCShadow( true ),
pDirtyTUCShadowCube( true ),
pDirtyTUCEnvMap( true ),
pDirtyTUCOutlineDepth( true ),
pDirtyTUCOutlineGeometry( true ),
pDirtyTUCOutlineCounter( true )
{
	LEAK_CHECK_CREATE( component.GetRenderThread(), ComponentTexture );
}

class deoglRComponentTextureDeletion : public deoglDelayedDeletion{
public:
	deoglSkinState *skinState;
	deoglSPBlockUBO *paramBlockDepth;
	deoglSPBlockUBO *paramBlockGeometry;
	deoglSPBlockUBO *paramBlockEnvMap;
	deoglTexUnitsConfig *tucDepth;
	deoglTexUnitsConfig *tucGeometry;
	deoglTexUnitsConfig *tucCounter;
	deoglTexUnitsConfig *tucShadow;
	deoglTexUnitsConfig *tucShadowCube;
	deoglTexUnitsConfig *tucEnvMap;
	deoglTexUnitsConfig *tucOutlineGeometry;
	deoglTexUnitsConfig *tucOutlineDepth;
	deoglTexUnitsConfig *tucOutlineCounter;
	
	deoglRComponentTextureDeletion() :
	skinState( NULL ),
	paramBlockDepth( NULL ),
	paramBlockGeometry( NULL ),
	paramBlockEnvMap( NULL ),
	tucDepth( NULL ),
	tucGeometry( NULL ),
	tucCounter( NULL ),
	tucShadow( NULL ),
	tucShadowCube( NULL ),
	tucEnvMap( NULL ),
	tucOutlineGeometry( NULL ),
	tucOutlineDepth( NULL ),
	tucOutlineCounter( NULL ){
	}
	
	virtual ~deoglRComponentTextureDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( tucDepth ){
			tucDepth->RemoveUsage();
		}
		if( tucGeometry ){
			tucGeometry->RemoveUsage();
		}
		if( tucCounter ){
			tucCounter->RemoveUsage();
		}
		if( tucShadow ){
			tucShadow->RemoveUsage();
		}
		if( tucShadowCube ){
			tucShadowCube->RemoveUsage();
		}
		if( tucEnvMap ){
			tucEnvMap->RemoveUsage();
		}
		if( tucOutlineGeometry ){
			tucOutlineGeometry->RemoveUsage();
		}
		if( tucOutlineDepth ){
			tucOutlineDepth->RemoveUsage();
		}
		if( tucOutlineCounter ){
			tucOutlineCounter->RemoveUsage();
		}
		if( paramBlockDepth ){
			paramBlockDepth->FreeReference();
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

deoglRComponentTexture::~deoglRComponentTexture(){
	LEAK_CHECK_FREE( pComponent.GetRenderThread(), ComponentTexture );
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSharedSPBRTIGroup.RemoveAll();
	if( pSharedSPBElement ){
		pSharedSPBElement->FreeReference();
	}
	
	// drop reference otherwise deletion can cause other deletions to be generated
	// causing a deletion race
	if( pSkinState ){
		pSkinState->DropDelayedDeletionObjects();
	}
	
	// delayed deletion of opengl containing objects
	deoglRComponentTextureDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRComponentTextureDeletion;
		delayedDeletion->paramBlockDepth = pParamBlockDepth;
		delayedDeletion->paramBlockEnvMap = pParamBlockEnvMap;
		delayedDeletion->paramBlockGeometry = pParamBlockGeometry;
		delayedDeletion->skinState = pSkinState;
		delayedDeletion->tucDepth = pTUCDepth;
		delayedDeletion->tucEnvMap = pTUCEnvMap;
		delayedDeletion->tucCounter = pTUCCounter;
		delayedDeletion->tucGeometry = pTUCGeometry;
		delayedDeletion->tucShadow = pTUCShadow;
		delayedDeletion->tucShadowCube = pTUCShadowCube;
		delayedDeletion->tucOutlineGeometry = pTUCOutlineGeometry;
		delayedDeletion->tucOutlineDepth = pTUCOutlineDepth;
		delayedDeletion->tucOutlineCounter = pTUCOutlineCounter;
		pComponent.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pComponent.GetRenderThread().GetLogger().LogException( e );
		//throw; -> otherwise terminate
	}
}



// Notifications
//////////////////

void deoglRComponentTexture::SetTransform( const decTexMatrix2 &matrix ){
	pTransform = matrix;
}



void deoglRComponentTexture::SetSkin( deoglRSkin *skin ){
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
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.MarkTextureUseSkinDirty();
	pComponent.GetSkinRendered().SetDirty();
}

void deoglRComponentTexture::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
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
	pComponent.MarkTextureUseSkinDirty();
	pComponent.GetSkinRendered().SetDirty();
}

void deoglRComponentTexture::SetSkinState( deoglSkinState *skinState ){
	if( skinState == pSkinState ){
		return;
	}
	
	if( pSkinState ){
		// drop reference otherwise deletion can cause other deletions to be generated
		// causing a deletion race
		pSkinState->DropDelayedDeletionObjects();
		
		// delayed deletion of opengl containing objects
		deoglRComponentTextureDeletion *delayedDeletion = NULL;
		
		try{
			delayedDeletion = new deoglRComponentTextureDeletion;
			delayedDeletion->skinState = pSkinState;
			pComponent.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
			
		}catch( const deException &e ){
			if( delayedDeletion ){
				delete delayedDeletion;
			}
			pComponent.GetRenderThread().GetLogger().LogException( e );
			throw;
		}
	}
	
	pSkinState = skinState;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.MarkTextureUseSkinDirty();
}

void deoglRComponentTexture::CheckSkinDynamicChannels(){
	/*if( ! pSkin || ! pSkin->GetHasDynamicChannels() ){
		return;
	}*/
}

void deoglRComponentTexture::UpdateSkinState(){
	// NOTE this is called from the main thread during synchronization
	
	// if the component has a dynamic skin but not the texture it is still required to have
	// an own skin state for the texture. if the skin state of the component would be used
	// the skin channel renderable indices do not match since they have been created for
	// the component skin not the texture skin. the only correct solution is to always use
	// a unique skin state for textures if they do not have a dynamic skin on their own.
	// 
	// the following combinations can exist (assuming texture has an own skin set):
	// - componet has skin state, texture has no skin state
	//   -> component has always a skin state even if it has no dynamic skin
	// - component has skin state, texture has skin state
	//   -> the above mentioned case. component has skin state because it has dynamic skin.
	//      texture needs own skin state since it uses the dynamic skin of the component
	//      and the component skin state does not match the texture skin
	if( pSkin && ( pDynamicSkin || pComponent.GetDynamicSkin() || pSkin->GetCalculatedPropertyCount() > 0 ) ){
		if( ! pSkinState ){
			SetSkinState( new deoglSkinState( pComponent.GetRenderThread(), pComponent, pIndex ) );
			pComponent.SetDirtyRendereables();
		}
		
	}else{
		if( pSkinState ){
			SetSkinState( NULL );
			pComponent.SetDirtyRendereables();
		}
	}
}



void deoglRComponentTexture::UpdateUseSkin(){
	pUseSkin = NULL;
	pUseTextureNumber = -1;
	pUseSkinTexture = NULL;
	pUseSkinState = NULL;
	pUseDoubleSided = false;
	pUseDecal = false;
	
	const deoglRModel * const model = pComponent.GetModel();
	if( ! model ){
		return;
	}
	
	const deoglModelTexture &modelTexture = model->GetLODAt( 0 ).GetTextureAt( pIndex );
	pUseDoubleSided = modelTexture.GetDoubleSided();
	pUseDecal = modelTexture.GetDecal();
	
	if( pSkinState ){
		pUseSkinState = pSkinState;
		
	}else{
		pUseSkinState = pComponent.GetSkinState(); // for textures with no own skin
	}
	
	if( pSkin ){
		pUseSkin = pSkin;
		
		if( pUseSkin->GetTextureCount() == 0 ){
			pUseTextureNumber = -1;
			
		}else{
			pUseTextureNumber = 0;
		}
	}
	
	if( ! pUseSkin ){
		const int mappedTexture = pComponent.GetModelSkinMappings().GetAt( pIndex );
		
		if( mappedTexture > -1 ){
			pUseTextureNumber = mappedTexture;
			pUseSkin = pComponent.GetSkin();
		}
	}
	
	if( pUseSkin && pUseTextureNumber != -1 ){
		pUseSkinTexture = &pUseSkin->GetTextureAt( pUseTextureNumber );
	}
}



deoglSPBlockUBO *deoglRComponentTexture::GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estComponentGeometry:
	case deoglSkinTexture::estDecalGeometry:
		return GetParamBlockGeometry();
		
	case deoglSkinTexture::estComponentDepth:
	case deoglSkinTexture::estComponentDepthClipPlane:
	case deoglSkinTexture::estComponentDepthReversed:
	case deoglSkinTexture::estComponentDepthClipPlaneReversed:
	case deoglSkinTexture::estComponentCounter:
	case deoglSkinTexture::estComponentCounterClipPlane:
	case deoglSkinTexture::estComponentShadowProjection:
	case deoglSkinTexture::estComponentShadowOrthogonal:
	case deoglSkinTexture::estComponentShadowDistance:
	case deoglSkinTexture::estComponentShadowDistanceCube:
		return GetParamBlockDepth();
		
	case deoglSkinTexture::estComponentEnvMap:
		return GetParamBlockEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglSPBlockUBO *deoglRComponentTexture::GetParamBlockDepth(){
	if( ! pValidParamBlockDepth ){
		if( pParamBlockDepth ){
			pParamBlockDepth->FreeReference();
			pParamBlockDepth = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estComponentDepth );
			
			if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockDepth = new deoglSPBlockUBO( *pComponent.GetRenderThread()
					.GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{
				pParamBlockDepth = skinShader.CreateSPBInstParam();
			}
		}
		
		pValidParamBlockDepth = true;
		pDirtyParamBlockDepth = true;
	}
	
	if( pDirtyParamBlockDepth ){
		if( pParamBlockDepth && pUseSkinTexture ){
			pParamBlockDepth->MapBuffer();
			try{
				UpdateInstanceParamBlock( *pParamBlockDepth, 0,
					*pUseSkinTexture->GetShaderFor( deoglSkinTexture::estComponentDepth ) );
				
			}catch( const deException & ){
				pParamBlockDepth->UnmapBuffer();
				throw;
			}
			pParamBlockDepth->UnmapBuffer();
		}
		
		pDirtyParamBlockDepth = false;
	}
	
	return pParamBlockDepth;
}

deoglSPBlockUBO *deoglRComponentTexture::GetParamBlockGeometry(){
	if( ! pValidParamBlockGeometry ){
		if( pParamBlockGeometry ){
			pParamBlockGeometry->FreeReference();
			pParamBlockGeometry = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estComponentGeometry );
			
			if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockGeometry = new deoglSPBlockUBO( *pComponent.GetRenderThread()
					.GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{
				pParamBlockGeometry = skinShader.CreateSPBInstParam();
			}
		}
		
		pValidParamBlockGeometry = true;
		pDirtyParamBlockGeometry = true;
	}
	
	if( pDirtyParamBlockGeometry ){
		if( pParamBlockGeometry && pUseSkinTexture ){
			pParamBlockGeometry->MapBuffer();
			try{
				UpdateInstanceParamBlock( *pParamBlockGeometry, 0,
					*pUseSkinTexture->GetShaderFor( deoglSkinTexture::estComponentGeometry ) );
				
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

deoglSPBlockUBO *deoglRComponentTexture::GetParamBlockEnvMap(){
	return NULL;
}

deoglSharedSPBElement *deoglRComponentTexture::GetSharedSPBElement(){
	if( ! pSharedSPBElement ){
		if( pComponent.GetRenderThread().GetChoices().GetSharedSPBUseSSBO() ){
			pSharedSPBElement = pComponent.GetRenderThread().GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinInstanceSSBO ).AddElement();
			
		}else{
			if( pComponent.GetRenderThread().GetChoices().GetGlobalSharedSPBLists() ){
				pSharedSPBElement = pComponent.GetRenderThread().GetBufferObject()
					.GetSharedSPBList( deoglRTBufferObject::esspblSkinInstanceUBO ).AddElement();
				
			}else{
				// depends if using model or world backed shared SPB
				if( pComponent.GetModel() ){
					pSharedSPBElement = pComponent.GetModel()->GetSharedSPBListUBO().AddElement();
				}
			}
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

deoglSharedSPBRTIGroup &deoglRComponentTexture::GetSharedSPBRTIGroup( int lodLevel ){
	if( lodLevel < pSharedSPBRTIGroup.GetCount() && pSharedSPBRTIGroup.GetAt( lodLevel ) ){
		return *( ( deoglSharedSPBRTIGroup* )pSharedSPBRTIGroup.GetAt( lodLevel ) );
	}
	
	if( ! pComponent.GetModel() ){
		DETHROW( deeInvalidParam );
	}
	
	while( lodLevel >= pSharedSPBRTIGroup.GetCount() ){
		pSharedSPBRTIGroup.Add( NULL );
	}
	
	deoglSharedSPBRTIGroup * const group = pComponent.GetModel()->GetLODAt( lodLevel )
		.GetSharedSPBRTIGroupListAt( pIndex ).GetWith( GetSharedSPBElement()->GetSPB() );
	pSharedSPBRTIGroup.SetAt( lodLevel, group );
	group->FreeReference();
	return *group;
}



deoglTexUnitsConfig *deoglRComponentTexture::GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estComponentGeometry:
	case deoglSkinTexture::estDecalGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexture::estComponentDepth:
	case deoglSkinTexture::estComponentDepthClipPlane:
	case deoglSkinTexture::estComponentDepthReversed:
	case deoglSkinTexture::estComponentDepthClipPlaneReversed:
		return GetTUCDepth();
		
	case deoglSkinTexture::estComponentCounter:
	case deoglSkinTexture::estComponentCounterClipPlane:
		return GetTUCCounter();
		
	case deoglSkinTexture::estComponentShadowProjection:
	case deoglSkinTexture::estComponentShadowOrthogonal:
	case deoglSkinTexture::estComponentShadowDistance:
		return GetTUCShadow();
		
	case deoglSkinTexture::estComponentShadowDistanceCube:
		return GetTUCShadowCube();
		
	case deoglSkinTexture::estComponentEnvMap:
		return GetTUCEnvMap();
		
	case deoglSkinTexture::estOutlineGeometry:
		return GetTUCOutlineGeometry();
		
	case deoglSkinTexture::estOutlineDepth:
	case deoglSkinTexture::estOutlineDepthClipPlane:
	case deoglSkinTexture::estOutlineDepthReversed:
	case deoglSkinTexture::estOutlineDepthClipPlaneReversed:
		return GetTUCOutlineDepth();
		
	case deoglSkinTexture::estOutlineCounter:
	case deoglSkinTexture::estOutlineCounterClipPlane:
		return GetTUCOutlineCounter();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCDepth(){
	if( pDirtyTUCDepth ){
		if( pTUCDepth ){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		
		pTUCDepth = BareGetTUCFor( deoglSkinTexture::estComponentDepth );
		
		pDirtyTUCDepth = false;
	}
	
	return pTUCDepth;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCGeometry(){
	if( pDirtyTUCGeometry ){
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		
		pTUCGeometry = BareGetTUCFor( deoglSkinTexture::estComponentGeometry );
		
		pDirtyTUCGeometry = false;
	}
	
	return pTUCGeometry;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCCounter(){
	if( pDirtyTUCCounter ){
		if( pTUCCounter ){
			pTUCCounter->RemoveUsage();
			pTUCCounter = NULL;
		}
		
		pTUCCounter = BareGetTUCFor( deoglSkinTexture::estComponentCounter );
		
		pDirtyTUCCounter = false;
	}
	
	return pTUCCounter;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCShadow(){
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

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCShadowCube(){
	if( pDirtyTUCShadowCube ){
		if( pTUCShadowCube ){
			pTUCShadowCube->RemoveUsage();
			pTUCShadowCube = NULL;
		}
		
		pTUCShadowCube = BareGetTUCFor( deoglSkinTexture::estComponentShadowDistanceCube );
		
		pDirtyTUCShadowCube = false;
	}
	
	return pTUCShadowCube;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCEnvMap(){
	if( pDirtyTUCEnvMap ){
		if( pTUCEnvMap ){
			pTUCEnvMap->RemoveUsage();
			pTUCEnvMap = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglRenderThread &renderThread = pComponent.GetRenderThread();
			deoglRDynamicSkin *dynamicSkin = NULL;
			deoglSkinState *skinState = NULL;
			deoglTexUnitConfig unit[ 8 ];
			
			if( pSkinState ){
				skinState = pSkinState;
				
				if( pDynamicSkin ){
					dynamicSkin = pDynamicSkin;
					
				}else{
					dynamicSkin = pComponent.GetDynamicSkin();
				}
				
			}else{
				// for texture with no own skin
				skinState = pComponent.GetSkinState();
				dynamicSkin = pComponent.GetDynamicSkin();
			}
			
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
			
			pTUCEnvMap = renderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2 );
		}
		
		pDirtyTUCEnvMap = false;
	}
	
	return pTUCEnvMap;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCOutlineDepth(){
	if( pDirtyTUCOutlineDepth ){
		if( pTUCOutlineDepth ){
			pTUCOutlineDepth->RemoveUsage();
			pTUCOutlineDepth = NULL;
		}
		
		pTUCOutlineDepth = BareGetTUCFor( deoglSkinTexture::estOutlineDepth );
		
		pDirtyTUCOutlineDepth = false;
	}
	
	return pTUCOutlineDepth;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCOutlineGeometry(){
	if( pDirtyTUCOutlineGeometry ){
		if( pTUCOutlineGeometry ){
			pTUCOutlineGeometry->RemoveUsage();
			pTUCOutlineGeometry = NULL;
		}
		
		pTUCOutlineGeometry = BareGetTUCFor( deoglSkinTexture::estOutlineGeometry );
		
		pDirtyTUCOutlineGeometry = false;
	}
	
	return pTUCOutlineGeometry;
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCOutlineCounter(){
	if( pDirtyTUCOutlineCounter ){
		if( pTUCOutlineCounter ){
			pTUCOutlineCounter->RemoveUsage();
			pTUCOutlineCounter = NULL;
		}
		
		pTUCOutlineCounter = BareGetTUCFor( deoglSkinTexture::estOutlineCounter );
		
		pDirtyTUCOutlineCounter = false;
	}
	
	return pTUCOutlineCounter;
}

deoglTexUnitsConfig *deoglRComponentTexture::BareGetTUCFor(
deoglSkinTexture::eShaderTypes shaderType ) const{
	if( ! pUseSkinTexture ){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	if( pSkinState ){
		skinState = pSkinState;
		if( pDynamicSkin ){
			dynamicSkin = pDynamicSkin;
			
		}else{
			dynamicSkin = pComponent.GetDynamicSkin();
		}
		
	}else{
		// for texture with no own skin
		skinState = pComponent.GetSkinState();
		dynamicSkin = pComponent.GetDynamicSkin();
	}
	
	deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( shaderType );
	
	if( skinShader.GetUsedTextureTargetCount() > 0 ){
		skinShader.SetTUCCommon( &units[ 0 ], *pUseSkinTexture, skinState, dynamicSkin );
		skinShader.SetTUCPerObjectEnvMap( &units[ 0 ],
			pComponent.GetParentWorld()->GetSkyEnvironmentMap(),
			pComponent.GetRenderEnvMap(), pComponent.GetRenderEnvMapFade() );
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount() );
	}
	
	return tuc;
}

void deoglRComponentTexture::InvalidateParamBlocks(){
	pValidParamBlockDepth = false;
	pValidParamBlockGeometry = false;
	pValidParamBlockEnvMap = false;
	
	MarkParamBlocksDirty();
}

void deoglRComponentTexture::MarkParamBlocksDirty(){
	pDirtyParamBlockDepth = true;
	pDirtyParamBlockGeometry = true;
	pDirtyParamBlockEnvMap = true;
	pDirtySharedSPBElement = true;
}

void deoglRComponentTexture::MarkTUCsDirty(){
	pDirtyTUCDepth = true;
	pDirtyTUCGeometry = true;
	pDirtyTUCCounter = true;
	pDirtyTUCShadow = true;
	pDirtyTUCShadowCube = true;
	pDirtyTUCEnvMap = true;
	pDirtyTUCOutlineDepth = true;
	pDirtyTUCOutlineGeometry = true;
	pDirtyTUCOutlineCounter = true;
}



void deoglRComponentTexture::UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
int element, deoglSkinShader &skinShader ){
	if( ! pUseSkinTexture ){
		return;
	}
	
	deoglRDynamicSkin *useDynamicSkin = NULL;
	deoglSkinState *useSkinState = NULL;
	
	if( pSkinState ){
		useSkinState = pSkinState;
		if( pDynamicSkin ){
			useDynamicSkin = pDynamicSkin;
			
		}else{
			useDynamicSkin = pComponent.GetDynamicSkin();
		}
		
	}else{
		// for texture with no own skin
		useSkinState = pComponent.GetSkinState();
		useDynamicSkin = pComponent.GetDynamicSkin();
	}
	
	// update shader parameter block
	int target;
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
	if( target != -1 ){
		const decDVector &referencePosition = pComponent.GetParentWorld()->GetReferencePosition();
		decDMatrix matrix( pComponent.GetMatrix() );
		
		matrix.a14 -= referencePosition.x;
		matrix.a24 -= referencePosition.y;
		matrix.a34 -= referencePosition.z;
		
		paramBlock.SetParameterDataMat4x3( target, element, matrix );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixNormal );
	if( target != -1 ){
		paramBlock.SetParameterDataMat3x3( target, element,
			pComponent.GetMatrix().GetRotationMatrix().Invert() );
		//paramBlock.SetParameterDataMat3x3( target, element,
		//	pComponent.GetInverseMatrix().GetRotationMatrix() );
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixTexCoord );
	if( target != -1 ){
		decTexMatrix2 matrix( pTransform );
		
		const deoglSkinTextureProperty &propertyOffset =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordOffset );
		const deoglSkinTextureProperty &propertyScale =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordScale );
		const deoglSkinTextureProperty &propertyRotate =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordRotate );
		
		const decVector2 offset( propertyOffset.ResolveVector2(
			useSkinState, useDynamicSkin, pUseSkinTexture->GetTexCoordOffset() ) );
		const decVector2 scale( propertyScale.ResolveVector2(
			useSkinState, useDynamicSkin, pUseSkinTexture->GetTexCoordScale() ) );
		const float rotate = propertyRotate.ResolveAsFloat(
			useSkinState, useDynamicSkin, pUseSkinTexture->GetTexCoordRotate() );
		
		const bool hasOffset = ! decVector2().IsEqualTo( offset );
		const bool hasScale = ! decVector2( 1.0f, 1.0f ).IsEqualTo( scale );
		const bool hasRotate = fabsf( rotate ) > FLOAT_SAFE_EPSILON;
		
		if( hasScale || hasRotate ){
			matrix = decTexMatrix2::CreateCenterSRT( scale, rotate * TWO_PI, offset ) * matrix;
			
		}else if( hasOffset ){
			matrix = decTexMatrix2::CreateTranslation( offset ) * matrix;
		}
		
		paramBlock.SetParameterDataMat3x2( target, element, matrix );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutDoubleSided );
	if( target != -1 ){
		paramBlock.SetParameterDataBool( target, element,
			pComponent.GetModel()->GetLODAt( 0 ).GetTextureAt( pIndex ).GetDoubleSided() );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutEnvMapFade );
	if( target != -1 ){
		paramBlock.SetParameterDataFloat( target, element, pComponent.GetRenderEnvMapFadeFactor() );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutVariationSeed );
	if( target != -1 ){
		if( useSkinState ){
			paramBlock.SetParameterDataVec2( target, element, useSkinState->GetVariationSeed() );
			
		}else{
			paramBlock.SetParameterDataVec2( target, element, 0.0f, 0.0f );
		}
	}
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB( paramBlock, element,
		*pUseSkinTexture, useSkinState, useDynamicSkin );
}



void deoglRComponentTexture::PrepareSkinStateRenderables(){
	if( ! pSkinState ){
		return;
	}
	
	if( pDynamicSkin ){
		pSkinState->PrepareRenderables( pSkin, pDynamicSkin );
		
	}else{
		pSkinState->PrepareRenderables( pSkin, pComponent.GetDynamicSkin() );
	}
}
