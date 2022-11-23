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

#include "deoglComponent.h"
#include "deoglRComponent.h"
#include "deoglRComponentLOD.h"
#include "deoglRComponentTexture.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/texture/deoglModelTexture.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../sky/deoglRSkyInstance.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../world/deoglRWorld.h"
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
pUseDynamicSkin( NULL ),
pUseDoubleSided( false ),
pUseDecal( false ),
pIsRendered( false ),
pRenderTaskFilters( 0 ),

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
pTUCLuminance( NULL ),
pTUCGIMaterial( NULL ),

pValidParamBlocks( false ),
pDirtyParamBlocks( true ),
pDirtyTUCs( true ),
pDirtyTUCsAll( true ),
pDirtyTUCsEnvMapUse( true )
{
	LEAK_CHECK_CREATE( component.GetRenderThread(), ComponentTexture );
}

deoglRComponentTexture::~deoglRComponentTexture(){
	LEAK_CHECK_FREE( pComponent.GetRenderThread(), ComponentTexture );
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSharedSPBRTIGroup.RemoveAll();
	pSharedSPBRTIGroupShadow.RemoveAll();
	if( pSharedSPBElement ){
		pSharedSPBElement->FreeReference();
	}
	
	if( pTUCDepth ){
		pTUCDepth->RemoveUsage();
	}
	if( pTUCGeometry ){
		pTUCGeometry->RemoveUsage();
	}
	if( pTUCCounter ){
		pTUCCounter->RemoveUsage();
	}
	if( pTUCShadow ){
		pTUCShadow->RemoveUsage();
	}
	if( pTUCShadowCube ){
		pTUCShadowCube->RemoveUsage();
	}
	if( pTUCEnvMap ){
		pTUCEnvMap->RemoveUsage();
	}
	if( pTUCOutlineGeometry ){
		pTUCOutlineGeometry->RemoveUsage();
	}
	if( pTUCOutlineDepth ){
		pTUCOutlineDepth->RemoveUsage();
	}
	if( pTUCOutlineCounter ){
		pTUCOutlineCounter->RemoveUsage();
	}
	if( pTUCLuminance ){
		pTUCLuminance->RemoveUsage();
	}
	if( pTUCGIMaterial ){
		pTUCGIMaterial->RemoveUsage();
	}
	if( pSkinState ){
		delete pSkinState;
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
	
	pIsRendered = false;
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.GetSkinRendered().SetDirty();
	pUpdateRenderTaskFilters();
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
	
	pIsRendered = false;
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.GetSkinRendered().SetDirty();
	pUpdateRenderTaskFilters();
}

void deoglRComponentTexture::SetSkinState( deoglSkinState *skinState ){
	if( skinState == pSkinState ){
		return;
	}
	
	if( pSkinState ){
		delete pSkinState;
	}
	
	pSkinState = skinState;
	
	pIsRendered = false;
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.GetSkinRendered().SetDirty();
	pUpdateRenderTaskFilters();
}

void deoglRComponentTexture::UpdateSkinState( deoglComponent &component ){
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
			component.DirtyRenderableMapping();
			component.DirtyTextureUseSkin();
		}
		
	}else{
		if( pSkinState ){
			SetSkinState( NULL );
			component.DirtyRenderableMapping();
			component.DirtyTextureUseSkin();
		}
	}
}



void deoglRComponentTexture::UpdateUseSkin(){
	pUseSkin = NULL;
	pUseTextureNumber = -1;
	pUseSkinTexture = NULL;
	pUseSkinState = NULL;
	pUseDynamicSkin = NULL;
	pUseDoubleSided = false;
	pUseDecal = false;
	
	pDirtyParamBlocks = true;
	pComponent.DirtyTextureParamBlocks();
	
	const deoglRModel * const model = pComponent.GetModel();
	if( ! model ){
		pIsRendered = false;
		InvalidateParamBlocks();
		pUpdateRenderTaskFilters();
		return;
	}
	
	const deoglModelTexture &modelTexture = model->GetLODAt( 0 ).GetTextureAt( pIndex );
	pUseDoubleSided = modelTexture.GetDoubleSided();
	pUseDecal = modelTexture.GetDecal();
	
	if( pSkinState ){
		pUseSkinState = pSkinState;
		
		if( pDynamicSkin ){
			pUseDynamicSkin = pDynamicSkin;
			
		}else{
			pUseDynamicSkin = pComponent.GetDynamicSkin();
		}
		
	}else{
		pUseSkinState = pComponent.GetSkinState(); // for textures with no own skin
		pUseDynamicSkin = pComponent.GetDynamicSkin();
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
	
	InvalidateParamBlocks();
	pUpdateIsRendered();
	pUpdateRenderTaskFilters();
}

decTexMatrix2 deoglRComponentTexture::CalcTexCoordMatrix() const{
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
	
	return matrix;
}



void deoglRComponentTexture::PrepareParamBlocks(){
	if( ! pValidParamBlocks ){
		// shared spb
		if( pSharedSPBElement ){
			pSharedSPBElement->FreeReference();
			pSharedSPBElement = NULL;
		}
		
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
		
		// shared spb render task instance group
		const int count = pComponent.GetLODCount();
		int i;
		
		pSharedSPBRTIGroup.RemoveAll();
		pSharedSPBRTIGroupShadow.RemoveAll();
		
		if( pSharedSPBElement ){
			const deoglRModel &model = pComponent.GetModelRef();
			deoglSharedSPB &spb = pSharedSPBElement->GetSPB();
			deoglSharedSPBRTIGroup::Ref group;
			
			for( i=0; i<count; i++ ){
				deoglModelLOD &modelLod = model.GetLODAt( i );
				deoglSharedSPBRTIGroupList &list = modelLod.GetSharedSPBRTIGroupListAt( pIndex );
				group.TakeOver( list.GetWith( spb ) );
				
				if( ! group ){
					group.TakeOver( list.AddWith( spb ) );
					group->GetRTSInstance()->SetSubInstanceSPB( &spb );
				}
				
				pSharedSPBRTIGroup.Add( group );
			}
			
			if( ( pRenderTaskFilters & ( ertfRender | ertfShadowNone | ertfDecal ) ) == ertfRender ){
				// combine shadow textures if possible
				for( i=0; i<count; i++ ){
					const int combineCount = pShadowCombineCount( i );
					if( combineCount < 2 ){
						pSharedSPBRTIGroupShadow.Add( nullptr );
						continue;
					}
					
					deoglModelLOD &modelLod = model.GetLODAt( i );
					deoglSharedSPBRTIGroupList &list = modelLod.GetSharedSPBRTIGroupListAt( pIndex );
					group.TakeOver( list.GetWith( spb, combineCount ) );
					
					if( ! group ){
						group.TakeOver( list.AddWith( spb, combineCount ) );
						group->GetRTSInstance()->SetSubInstanceSPB( &spb );
					}
					
					pSharedSPBRTIGroupShadow.Add( group );
				}
				
			}else{
				for( i=0; i<count; i++ ){
					pSharedSPBRTIGroupShadow.Add( nullptr );
				}
			}
			
			UpdateRTSInstances();
			
		}else{
			for( i=0; i<count; i++ ){
				pSharedSPBRTIGroup.Add( nullptr );
				pSharedSPBRTIGroupShadow.Add( nullptr );
			}
		}
		
		// done
		pDirtyParamBlocks = true;
		pValidParamBlocks = true;
	}
	
	if( pDirtyParamBlocks ){
		// shared spb
		if( pSharedSPBElement && pUseSkinTexture ){
			// it does not matter which shader type we use since all are required to use the
			// same shared spb instance layout
			const deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor(
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
		
		// done
		pDirtyParamBlocks = false;
	}
}

void deoglRComponentTexture::UpdateRTSInstances(){
	const deoglRModel &model = pComponent.GetModelRef();
	const int count = pSharedSPBRTIGroup.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSharedSPBRTIGroup * const group = ( deoglSharedSPBRTIGroup* )pSharedSPBRTIGroup.GetAt( i );
		if( ! group ){
			continue;
		}
		
		const deoglModelTexture &modelTexture = model.GetLODAt( i ).GetTextureAt( pIndex );
		deoglRenderTaskSharedInstance &rtsi = *group->GetRTSInstance();
		rtsi.SetFirstPoint( pComponent.GetPointOffset( i ) );
		rtsi.SetFirstIndex( pComponent.GetIndexOffset( i ) + modelTexture.GetFirstFace() * 3 );
		rtsi.SetIndexCount( modelTexture.GetFaceCount() * 3 );
		rtsi.SetDoubleSided( pUseDoubleSided );
	}
	
	// shadow combine groups
	for( i=0; i<count; i++ ){
		deoglSharedSPBRTIGroup * const group = ( deoglSharedSPBRTIGroup* )pSharedSPBRTIGroupShadow.GetAt( i );
		if( ! group ){
			continue;
		}
		
		const deoglModelLOD &modelLod = model.GetLODAt( i );
		deoglRenderTaskSharedInstance &rtsi = *group->GetRTSInstance();
		rtsi.SetDoubleSided( pUseDoubleSided );
		rtsi.SetFirstPoint( pComponent.GetPointOffset( i ) );
		rtsi.SetFirstIndex( pComponent.GetIndexOffset( i ) + modelLod.GetTextureAt( pIndex ).GetFirstFace() * 3 );
		
		int j, faceCount = 0;
		for( j=0; j<group->GetTextureCount(); j++ ){
			faceCount += modelLod.GetTextureAt( pIndex + j ).GetFaceCount();
		}
		rtsi.SetIndexCount( faceCount * 3 );
	}
}

deoglSharedSPBRTIGroup &deoglRComponentTexture::GetSharedSPBRTIGroup( int lodLevel ) const{
	deoglSharedSPBRTIGroup * const group = ( ( deoglSharedSPBRTIGroup* )pSharedSPBRTIGroup.GetAt(
		lodLevel >= 0 ? lodLevel : lodLevel + pComponent.GetLODCount() ) );
	if( ! group ){
		DETHROW( deeInvalidParam );
	}
	return *group;
}

deoglSharedSPBRTIGroup *deoglRComponentTexture::GetSharedSPBRTIGroupShadow( int lodLevel ) const{
	return ( ( deoglSharedSPBRTIGroup* )pSharedSPBRTIGroupShadow.GetAt(
		lodLevel >= 0 ? lodLevel : lodLevel + pComponent.GetLODCount() ) );
}



deoglTexUnitsConfig *deoglRComponentTexture::GetTUCForShaderType(
deoglSkinTexture::eShaderTypes shaderType ) const{
	switch( shaderType ){
	case deoglSkinTexture::estComponentGeometry:
	case deoglSkinTexture::estDecalGeometry:
	case deoglSkinTexture::estStereoComponentGeometry:
	case deoglSkinTexture::estStereoDecalGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexture::estComponentDepth:
	case deoglSkinTexture::estComponentDepthClipPlane:
	case deoglSkinTexture::estComponentDepthReversed:
	case deoglSkinTexture::estComponentDepthClipPlaneReversed:
	case deoglSkinTexture::estStereoComponentDepth:
	case deoglSkinTexture::estStereoComponentDepthClipPlane:
	case deoglSkinTexture::estStereoComponentDepthReversed:
	case deoglSkinTexture::estStereoComponentDepthClipPlaneReversed:
		return GetTUCDepth();
		
	case deoglSkinTexture::estComponentCounter:
	case deoglSkinTexture::estComponentCounterClipPlane:
	case deoglSkinTexture::estStereoComponentCounter:
	case deoglSkinTexture::estStereoComponentCounterClipPlane:
		return GetTUCCounter();
		
	case deoglSkinTexture::estComponentShadowProjection:
	case deoglSkinTexture::estComponentShadowOrthogonal:
	case deoglSkinTexture::estComponentShadowOrthogonalCascaded:
	case deoglSkinTexture::estComponentShadowDistance:
		return GetTUCShadow();
		
	case deoglSkinTexture::estComponentShadowDistanceCube:
		return GetTUCShadowCube();
		
	case deoglSkinTexture::estComponentEnvMap:
		return GetTUCEnvMap();
		
	case deoglSkinTexture::estOutlineGeometry:
	case deoglSkinTexture::estStereoOutlineGeometry:
		return GetTUCOutlineGeometry();
		
	case deoglSkinTexture::estOutlineDepth:
	case deoglSkinTexture::estOutlineDepthClipPlane:
	case deoglSkinTexture::estOutlineDepthReversed:
	case deoglSkinTexture::estOutlineDepthClipPlaneReversed:
	case deoglSkinTexture::estStereoOutlineDepth:
	case deoglSkinTexture::estStereoOutlineDepthClipPlane:
	case deoglSkinTexture::estStereoOutlineDepthReversed:
	case deoglSkinTexture::estStereoOutlineDepthClipPlaneReversed:
		return GetTUCOutlineDepth();
		
	case deoglSkinTexture::estOutlineCounter:
	case deoglSkinTexture::estOutlineCounterClipPlane:
	case deoglSkinTexture::estStereoOutlineCounter:
	case deoglSkinTexture::estStereoOutlineCounterClipPlane:
		return GetTUCOutlineCounter();
		
	case deoglSkinTexture::estComponentLuminance:
		return GetTUCLuminance();
		
	case deoglSkinTexture::estComponentGIMaterial:
		return GetTUCGIMaterial();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void deoglRComponentTexture::PrepareTUCs(){
	if( ! pDirtyTUCs ){
		return;
	}
	
	if( pDirtyTUCsAll ){
		// depth
		if( pTUCDepth ){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		pTUCDepth = BareGetTUCFor( deoglSkinTexture::estComponentDepth );
		
		// counter
		if( pTUCCounter ){
			pTUCCounter->RemoveUsage();
			pTUCCounter = NULL;
		}
		pTUCCounter = BareGetTUCFor( deoglSkinTexture::estComponentCounter );
		
		// shadow
		if( pTUCShadow ){
			pTUCShadow->RemoveUsage();
			pTUCShadow = NULL;
		}
		pTUCShadow = BareGetTUCFor( deoglSkinTexture::estComponentShadowProjection );
		
		// shadow cube
		if( pTUCShadowCube ){
			pTUCShadowCube->RemoveUsage();
			pTUCShadowCube = NULL;
		}
		pTUCShadowCube = BareGetTUCFor( deoglSkinTexture::estComponentShadowDistanceCube );
		
		// environment map
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
				unit[ 0 ].EnableArrayTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectColor, skinState, dynamicSkin,
					renderThread.GetDefaultTextures().GetColorArray() );
				
				unit[ 1 ].EnableArrayTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectEmissivity, skinState, dynamicSkin,
					renderThread.GetDefaultTextures().GetEmissivityArray() );
				
			}else{
				unit[ 0 ].EnableTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectColor, skinState, dynamicSkin,
					renderThread.GetDefaultTextures().GetColor() );
				
				unit[ 1 ].EnableTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectEmissivity, skinState, dynamicSkin,
					renderThread.GetDefaultTextures().GetEmissivity() );
			}
			
			pTUCEnvMap = renderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2,
				pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
			pTUCEnvMap->EnsureRTSTexture();
		}
		
		// outline depth
		if( pTUCOutlineDepth ){
			pTUCOutlineDepth->RemoveUsage();
			pTUCOutlineDepth = NULL;
		}
		pTUCOutlineDepth = BareGetTUCFor( deoglSkinTexture::estOutlineDepth );
		
		// outline geometry
		if( pTUCOutlineGeometry ){
			pTUCOutlineGeometry->RemoveUsage();
			pTUCOutlineGeometry = NULL;
		}
		pTUCOutlineGeometry = BareGetTUCFor( deoglSkinTexture::estOutlineGeometry );
		
		// outline counter
		if( pTUCOutlineCounter ){
			pTUCOutlineCounter->RemoveUsage();
			pTUCOutlineCounter = NULL;
		}
		pTUCOutlineCounter = BareGetTUCFor( deoglSkinTexture::estOutlineCounter );
		
		// global illumination material
		if( pTUCGIMaterial ){
			pTUCGIMaterial->RemoveUsage();
			pTUCGIMaterial = NULL;
		}
		pTUCGIMaterial = BareGetTUCFor( deoglSkinTexture::estComponentGIMaterial );
	}
	
	if( pDirtyTUCsAll || pDirtyTUCsEnvMapUse ){
		// geometry
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		pTUCGeometry = BareGetTUCFor( deoglSkinTexture::estComponentGeometry );
		
		// luminance
		if( pTUCLuminance ){
			pTUCLuminance->RemoveUsage();
			pTUCLuminance = NULL;
		}
		pTUCLuminance = BareGetTUCFor( deoglSkinTexture::estComponentLuminance );
	}
	
	// finished
	pDirtyTUCs = false;
	pDirtyTUCsAll = false;
	pDirtyTUCsEnvMapUse = false;
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

void deoglRComponentTexture::InvalidateParamBlocks(){
	pValidParamBlocks = false;
	pDirtyParamBlocks = true;
	pComponent.DirtyTextureParamBlocks();
	pComponent.DirtyLODRenderTaskConfigs();
}

void deoglRComponentTexture::MarkParamBlocksDirty(){
	pDirtyParamBlocks = true;
	pComponent.DirtyTextureParamBlocks();
}

void deoglRComponentTexture::MarkTUCsDirty(){
	pDirtyTUCs = true;
	pDirtyTUCsAll = true;
	pComponent.DirtyTextureTUCs();
	pComponent.DirtyLODRenderTaskConfigs();
}

void deoglRComponentTexture::MarkTUCsDirtyEnvMapUse(){
	pDirtyTUCs = true;
	pDirtyTUCsEnvMapUse = true;
	pComponent.DirtyTextureTUCs();
	pComponent.DirtyLODRenderTaskConfigs();
}



void deoglRComponentTexture::UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
int element, const deoglSkinShader &skinShader ){
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
		paramBlock.SetParameterDataMat3x2( target, element, CalcTexCoordMatrix() );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutDoubleSided );
	if( target != -1 ){
		paramBlock.SetParameterDataBool( target, element,
			pComponent.GetModelRef().GetLODAt( 0 ).GetTextureAt( pIndex ).GetDoubleSided() );
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
	
	skinShader.SetTexParamsInInstParamSPB( paramBlock, element, *pUseSkinTexture );
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB( paramBlock, element,
		*pUseSkinTexture, useSkinState, useDynamicSkin );
}



void deoglRComponentTexture::PrepareSkinStateRenderables( const deoglRenderPlanMasked *renderPlanMask ){
	if( ! pSkinState ){
		return;
	}
	
	if( pDynamicSkin ){
		pSkinState->PrepareRenderables( pSkin, pDynamicSkin, renderPlanMask );
		
	}else{
		pSkinState->PrepareRenderables( pSkin, pComponent.GetDynamicSkin(), renderPlanMask );
	}
	
	pUpdateIsRendered();
}

void deoglRComponentTexture::UpdateRenderableMapping(){
	if( ! pSkinState ){
		return;
	}
	
	pSkinState->RemoveAllRenderables();
	
	deoglRDynamicSkin * const dynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
	if( pSkin && dynamicSkin ){
		pSkinState->AddRenderables( *pSkin, *dynamicSkin );
	}
	
	MarkParamBlocksDirty();
	MarkTUCsDirty();
}



// Private Functions
//////////////////////

void deoglRComponentTexture::pUpdateIsRendered(){
	pIsRendered = false;
	
	if( ! pUseSkinTexture ){
		return;
	}
	
	if( pUseSkinTexture->GetRendered() ){
		pIsRendered = true;
		return;
	}
	
	if( ! pUseSkinState || ! pUseDynamicSkin ){
		return;
	}
	
	const deoglSkinChannel * const skinChannel = pUseSkinTexture->GetChannelAt( deoglSkinChannel::ectColor );
	if( ! skinChannel ){
		return;
	}
	
	const int skinRenderable = skinChannel->GetRenderable();
	if( skinRenderable < 0 || skinRenderable >= pUseSkinState->GetRenderableCount() ){
		return;
	}
	
	const deoglSkinStateRenderable &skinStateRenderable = *pUseSkinState->GetRenderableAt( skinRenderable );
	if( skinStateRenderable.GetHostRenderable() == -1 ){
		return;
	}
	
	pIsRendered = pUseDynamicSkin->GetRenderableAt( skinStateRenderable.GetHostRenderable() )->GetRenderPlan() != NULL;
}

void deoglRComponentTexture::pUpdateRenderTaskFilters(){
	pRenderTaskFilters = 0;
	if( ! pComponent.GetModel() ){
		return;
	}
	
	if( pUseSkinTexture ){
		pRenderTaskFilters |= pUseSkinTexture->GetRenderTaskFilters();
	}
	if( pIsRendered ){
		pRenderTaskFilters |= ertfRendered;
	}
	if( pUseDecal ){
		pRenderTaskFilters |= ertfDecal;
	}
	if( pUseDoubleSided ){
		pRenderTaskFilters |= ertfDoubleSided;
	}
}

int deoglRComponentTexture::pShadowCombineCount( int lodLevel ) const{
	const bool hasHoles = ( pRenderTaskFilters & ( ertfSolid | ertfHoles ) ) == ( ertfSolid | ertfHoles );
	if( hasHoles && pUseSkinState && ( pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV() ) ){
		return 1; // can not combine since variation is used
	}
	
	int i;
	for( i=0; i<pIndex; i++ ){
		const deoglRComponentTexture &texture = pComponent.GetTextureAt( i );
		const deoglSharedSPBRTIGroup * const group = texture.GetSharedSPBRTIGroupShadow( lodLevel );
		if( group && group->GetTextureCount() > pIndex - i ){
			return 1; // covered by a previous combined group
		}
	}
	
	const int mask = ertfRender | ertfSolid | ertfShadowNone | ertfHoles | ertfDecal | ertfDoubleSided;
	const int filter = pRenderTaskFilters & mask;
	const int count = pComponent.GetTextureCount();
	
	for( i=pIndex+1; i<count; i++ ){
		const deoglRComponentTexture &texture = pComponent.GetTextureAt( i );
		if( ( texture.pRenderTaskFilters & mask ) != filter ){
			break; // can not be combined because filters differ
		}
		if( hasHoles ){
			if( pTUCShadow != texture.pTUCShadow ){
				break; // can not be combined because TUC differs
			}
			if( ! pTransform.IsEqualTo( texture.pTransform ) ){
				break; // can not be combined because transform differs
			}
		}
	}
	
	return i - pIndex; // count of matching textures that can be combined
}
