/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRBillboard.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../rendering/plan/deoglRenderPlanMasked.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBList.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElementMapBuffer.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/deoglCubeHelper.h"
#include "../visitors/deoglFindBestEnvMap.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>



// Class deoglRBillboard::WorldComputeElement
///////////////////////////////////////////////

deoglRBillboard::WorldComputeElement::WorldComputeElement( deoglRBillboard &billboard ) :
deoglWorldComputeElement( eetBillboard, &billboard ),
pBillboard( billboard ){
}

void deoglRBillboard::WorldComputeElement::UpdateData( sDataElement &data ) const{
	const decDVector &refpos = GetReferencePosition();
	data.SetExtends( pBillboard.GetMinimumExtend() - refpos, pBillboard.GetMaximumExtend() - refpos );
	data.SetLayerMask( pBillboard.GetLayerMask() );
	data.flags = ( uint32_t )( deoglWorldCompute::eefBillboard
		| deoglWorldCompute::eefDynamic | deoglWorldCompute::eefGIDynamic );
	data.geometryCount = 1;
	data.highestLod = 0;
}

void deoglRBillboard::WorldComputeElement::UpdateDataGeometries( sDataElementGeometry *data ) const{
	const deoglSkinTexture * const skinTexture = pBillboard.GetUseSkinTexture();
	if( ! skinTexture ){
		return;
	}
	
	int filters = skinTexture->GetRenderTaskFilters() & ~RenderFilterOutline;
	
	SetDataGeometry( *data, 0, filters, deoglSkinTexturePipelinesList::eptBillboard,
		deoglSkinTexturePipelines::emDoubleSided, skinTexture,
		pBillboard.GetRenderThread().GetDeferredRendering().GetVAOBillboard(),
		pBillboard.GetSharedSPBRTIGroup().GetRTSInstance(), pBillboard.GetSharedSPBElement()->GetIndex() );
	
	sInfoTUC info;
	info.geometry = pBillboard.GetTUCGeometry();
	info.depth = pBillboard.GetTUCDepth();
	info.counter = pBillboard.GetTUCCounter();
	info.envMap = pBillboard.GetTUCEnvMap();
	SetDataGeometryTUCs( *data, info );
}



// Class deoglRBillboard
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRBillboard::deoglRBillboard( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),
pOctreeNode( NULL ),
pWorldComputeElement( deoglWorldComputeElement::Ref::New( new WorldComputeElement( *this ) ) ),

pSkin( NULL ),
pUseSkinTexture( NULL ),
pDynamicSkin( NULL ),
pLocked( true ),
pSpherical( false ),
pSizeFixedToScreen( false ),
pVisible( true ),

pSkinState( NULL ),
pSkinRendered( renderThread, *this ),
pSkyShadowSplitMask( 0 ),
pSortDistance( 0.0f ),
pOccluded( false ),
pDirtyPrepareSkinStateRenderables( true ),
pDirtyRenderSkinStateRenderables( true ),

pRenderEnvMap( nullptr ),
pRenderEnvMapFade( nullptr ),
pRenderEnvMapFadePerTime( 1.0f ),
pRenderEnvMapFadeFactor( 1.0f ),
pDirtyRenderEnvMap( true ),

pSharedSPBElement( NULL ),

pTUCDepth( NULL ),
pTUCGeometry( NULL ),
pTUCCounter( NULL ),
pTUCEnvMap( NULL ),

pDirtySharedSPBElement( true ),
pDirtyTUCs( true ),

pSpecialFlags( 0 ),

pCullSphereRadius( 0.0f ),
pDirtyCulling( true ),
pRenderVisible( true ),

pMarked( false ),

pCSOctreeIndex( 0 ),

pWorldMarkedRemove( false ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ),

pLLPrepareForRenderWorld( this )
{
	try{
		pSkinState = new deoglSkinState( renderThread, *this );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, Billboard );
}

deoglRBillboard::~deoglRBillboard(){
	LEAK_CHECK_FREE( pRenderThread, Billboard );
	pCleanUp();
}



// Management
///////////////

void deoglRBillboard::SetParentWorld( deoglRWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	InvalidateRenderEnvMap();
	pSkinRendered.DropDelayedDeletionObjects();
	
	pWorldComputeElement->RemoveFromCompute();
	
	pParentWorld = parentWorld;
	
	if( pOctreeNode ){
		pOctreeNode->RemoveBillboard( this );
	}
	
	pDirtyRenderEnvMap = true;
}



void deoglRBillboard::SetOctreeNode( deoglWorldOctree *octreeNode ){
	pOctreeNode = octreeNode;
}

void deoglRBillboard::UpdateOctreeNode(){
	if( ! pParentWorld ){
		return;
	}
	
	// insert into parent world octree
	if( pVisible ){
		pParentWorld->GetOctree().InsertBillboardIntoTree( this );
		
		if( pWorldComputeElement->GetWorldCompute() ){
			pWorldComputeElement->ComputeUpdateElement();
			
		}else{
			pParentWorld->GetCompute().AddElement( pWorldComputeElement );
		}
		
	}else{
		pWorldComputeElement->RemoveFromCompute();
		if( pOctreeNode ){
			pOctreeNode->RemoveBillboard( this );
		}
	}
}



void deoglRBillboard::SetSkin( deoglRSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	pUseSkinTexture = skin && skin->GetTextureCount() > 0 ? &skin->GetTextureAt( 0 ) : NULL;
	
	if( skin ){
		skin->AddReference();
	}
	
	pDirtySharedSPBElement = true;
	pRequiresPrepareForRender();
	
	pSkinRendered.SetDirty();
	
	pWorldComputeElement->ComputeUpdateElementAndGeometries();
}

void deoglRBillboard::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
	// NOTE this is called from the main thread during synchronization
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
	
	pSkinRendered.SetDirty();
}



void deoglRBillboard::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoglRBillboard::SetAxis( const decVector &axis ){
	pAxis = axis;
}

void deoglRBillboard::SetSize( const decVector2 &size ){
	pSize = size;
}

void deoglRBillboard::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void deoglRBillboard::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}

void deoglRBillboard::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	
	if( visible ){
		pRequiresPrepareForRender();
	}
}

void deoglRBillboard::SetLocked( bool locked ){
	pLocked = locked;
}

void deoglRBillboard::SetSpherical( bool spherical ){
	pSpherical = spherical;
}

void deoglRBillboard::SetSizeFixedToScreen( bool sizeFixedToScreen ){
	pSizeFixedToScreen = sizeFixedToScreen;
}

void deoglRBillboard::UpdateSkin( float elapsed ){
	// WARNING called from main thread during synchronization time
	
	pSkinState->AdvanceTime( elapsed );
	if( pUseSkinTexture ){
		if( pUseSkinTexture->GetDynamicChannels() ){
			MarkParamBlocksDirty();
			MarkTUCsDirty();
			
		}else if( pUseSkinTexture->GetCalculatedProperties()
		|| pUseSkinTexture->GetConstructedProperties() ){
			MarkParamBlocksDirty();
		}
	}
	
	if( pRenderEnvMapFade ){
		pRenderEnvMapFadeFactor += pRenderEnvMapFadePerTime * elapsed;
		
		if( pRenderEnvMapFadeFactor >= 1.0f ){
			SetRenderEnvMapFade( NULL );
			pRenderEnvMapFadeFactor = 1.0f;
		}
	}
}

void deoglRBillboard::InitSkinStateStates(){
	pSkinState->InitAll();
}

void deoglRBillboard::UpdateSkinStateStates(){
	if( pSkinState ){
		pSkinState->UpdateAll();
	}
}

void deoglRBillboard::DirtyPrepareSkinStateRenderables(){
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyRenderSkinStateRenderables = true;
	pRequiresPrepareForRender();
}

void deoglRBillboard::UpdateRenderableMapping(){
	pSkinState->RemoveAllRenderables();
	if( pSkin && pDynamicSkin ){
		pSkinState->AddRenderables( *pSkin, *pDynamicSkin );
	}
	
	MarkParamBlocksDirty();
	MarkTUCsDirty();
}

void deoglRBillboard::DynamicSkinRenderablesChanged(){
	if( ! pDynamicSkin || ! pSkin || ! pSkin->GetHasRenderables() ){
		return;
	}
	
	MarkParamBlocksDirty();
	MarkTUCsDirty();
}

void deoglRBillboard::PrepareSkinStateRenderables( const deoglRenderPlanMasked *renderPlanMask ){
	if( pSkinState ){
		pSkinState->PrepareRenderables( pSkin, pDynamicSkin, renderPlanMask );
	}
}

void deoglRBillboard::RenderSkinStateRenderables( const deoglRenderPlanMasked *renderPlanMask ){
	if( pSkinState ){
		pSkinState->RenderRenderables( pSkin, pDynamicSkin, renderPlanMask );
	}
}

void deoglRBillboard::PrepareSkinStateConstructed(){
	if( pSkinState ){
		pSkinState->PrepareConstructedProperties();
	}
}

void deoglRBillboard::AddSkinStateRenderPlans( deoglRenderPlan &plan ){
	pSkinState->AddRenderPlans( plan );
	pSkinRendered.AddRenderPlans( plan );
}



void deoglRBillboard::UpdateExtends( const deBillboard &billboard ){
	const decDVector &position = billboard.GetPosition();
	const decVector2 &size = billboard.GetSize();
	const float width = size.x * 0.5f;
	const float height = size.y * 0.5f;
	
	if( width > height ){
		pMinExtend.x = position.x - width;
		pMinExtend.y = position.y - width;
		pMinExtend.z = position.z - width;
		pMaxExtend.x = position.x + width;
		pMaxExtend.y = position.y + width;
		pMaxExtend.z = position.z + width;
		
	}else{
		pMinExtend.x = position.x - height;
		pMinExtend.y = position.y - height;
		pMinExtend.z = position.z - height;
		pMaxExtend.x = position.x + height;
		pMaxExtend.y = position.y + height;
		pMaxExtend.z = position.z + height;
	}
}



deoglTexUnitsConfig *deoglRBillboard::GetTUCForPipelineType( deoglSkinTexturePipelines::eTypes type ) const{
	switch( type ){
	case deoglSkinTexturePipelines::etGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexturePipelines::etDepth:
	case deoglSkinTexturePipelines::etDepthClipPlane:
	case deoglSkinTexturePipelines::etDepthReversed:
	case deoglSkinTexturePipelines::etDepthClipPlaneReversed:
	case deoglSkinTexturePipelines::etMask:
		return GetTUCDepth();
		
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
		return GetTUCCounter();
		
	case deoglSkinTexturePipelines::etEnvMap:
		return GetTUCEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglRBillboard::BareGetTUCFor( deoglSkinTexturePipelines::eTypes type ) const{
	if( ! pUseSkinTexture ){
		return NULL;
	}
	
	deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
		GetAt( deoglSkinTexturePipelinesList::eptBillboard ).GetWithRef( type ).GetShader();
	deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	if( skinShader.GetUsedTextureTargetCount() > 0 ){
		skinShader.SetTUCCommon( &units[ 0 ], *pUseSkinTexture, skinState, dynamicSkin );
		skinShader.SetTUCPerObjectEnvMap( &units[ 0 ], pParentWorld->GetSkyEnvironmentMap(),
			pRenderEnvMap, pRenderEnvMapFade );
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount(),
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith( NULL, 0,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	tuc ->EnsureRTSTexture();
	
	return tuc;
}

void deoglRBillboard::InvalidateParamBlocks(){
	MarkParamBlocksDirty();
}

void deoglRBillboard::MarkParamBlocksDirty(){
	pDirtySharedSPBElement = true;
	pRequiresPrepareForRender();
}

void deoglRBillboard::MarkTUCsDirty(){
	pDirtyTUCs = true;
	pRequiresPrepareForRender();
	pWorldComputeElement->ComputeUpdateElementGeometries();
}



void deoglRBillboard::UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
int element, deoglSkinShader &skinShader ){
	if( ! pUseSkinTexture ){
		return;
	}
	
	// update shader parameter block
	int target;
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
	if( target != -1 ){
		const decDVector &referencePosition = pParentWorld->GetReferencePosition();
		decDMatrix matrix;
		
		matrix.a14 = pPosition.x - referencePosition.x;
		matrix.a24 = pPosition.y - referencePosition.y;
		matrix.a34 = pPosition.z - referencePosition.z;
		
		matrix.a21 = pAxis.x;
		matrix.a22 = pAxis.y;
		matrix.a23 = pAxis.z;
		
		paramBlock.SetParameterDataMat4x3( target, element, matrix );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixNormal );
	if( target != -1 ){
		paramBlock.SetParameterDataMat3x3( target, element, decMatrix() );
		//paramBlock.SetParameterDataMat3x3( target, element, decMatrix().GetRotationMatrix().Invert() );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBillboardPosTransform );
	if( target != -1 ){
		decVector2 scale( pSize.x * 0.5f, pSize.y * 0.5f );
		decVector2 offset( pOffset.x, pOffset.y );
		paramBlock.SetParameterDataVec4( target, scale.x, scale.y, offset.x, offset.y );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBillboardParams );
	if( target != -1 ){
		paramBlock.SetParameterDataBVec3( target, pLocked, pSpherical, pSizeFixedToScreen );
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixTexCoord );
	if( target != -1 ){
		decTexMatrix2 matrix;
		
		const deoglSkinTextureProperty &propertyOffset =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordOffset );
		const deoglSkinTextureProperty &propertyScale =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordScale );
		const deoglSkinTextureProperty &propertyRotate =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordRotate );
		
		const decVector2 offset( propertyOffset.ResolveVector2(
			pSkinState, pDynamicSkin, pUseSkinTexture->GetTexCoordOffset() ) );
		const decVector2 scale( propertyScale.ResolveVector2(
			pSkinState, pDynamicSkin, pUseSkinTexture->GetTexCoordScale() ) );
		const float rotate = propertyRotate.ResolveAsFloat(
			pSkinState, pDynamicSkin, pUseSkinTexture->GetTexCoordRotate() );
		
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
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutDoubleSided);
	if(target != -1){
		paramBlock.SetParameterDataBool(target, element, true);
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutEnvMapFade );
	if( target != -1 ){
		paramBlock.SetParameterDataFloat( target, element, pRenderEnvMapFadeFactor );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutVariationSeed );
	if( target != -1 ){
		if( pSkinState ){
			paramBlock.SetParameterDataVec2( target, element, pSkinState->GetVariationSeed() );
			
		}else{
			paramBlock.SetParameterDataVec2( target, element, 0.0f, 0.0f );
		}
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutInstSkinClipPlaneNormal );
	if( target != -1 ){
		paramBlock.SetParameterDataVec4( target, element, 0.0f, 0.0f, 1.0f, 0.0f );
	}
	
	skinShader.SetTexParamsInInstParamSPB( paramBlock, element, *pUseSkinTexture );
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB( paramBlock, element, *pUseSkinTexture, pSkinState, pDynamicSkin );
}



void deoglRBillboard::UpdateCubeFaceVisibility( const decDVector &cubePosition ){
	deoglCubeHelper::CalcFaceVisibility(
		pMinExtend - cubePosition, pMaxExtend - cubePosition, pCubeFaceVisible );
	
	// DEBUG
	/*
	pRenderThread.GetLogger().LogInfoFormat( "DEBUG: (%g,%g,%g) [%d, %d, %d, %d, %d, %d] {(%g,%g,%g), (%g,%g,%g)}",
		pMatrix.GetPosition().x, pMatrix.GetPosition().y, pMatrix.GetPosition().z,
		    pCubeFaceVisible[0], pCubeFaceVisible[1], pCubeFaceVisible[2],
		    pCubeFaceVisible[3], pCubeFaceVisible[4], pCubeFaceVisible[5],
		    (pMinExtend - cubePosition).x, (pMinExtend - cubePosition).y, (pMinExtend - cubePosition).z,
		    (pMaxExtend - cubePosition).x, (pMaxExtend - cubePosition).y, (pMaxExtend - cubePosition).z );
	*/
}

bool deoglRBillboard::GetCubeFaceVisible( int cubeFace ) const{
	if( cubeFace < 0 || cubeFace > 5 ){
		DETHROW( deeInvalidParam );
	}
	return pCubeFaceVisible[ cubeFace ];
}

void deoglRBillboard::SetSpecialFlagsFromFaceVisibility(){
	pSpecialFlags = 0;
	if( pCubeFaceVisible[ 0 ] ){
		pSpecialFlags |= 0x1;
	}
	if( pCubeFaceVisible[ 1 ] ){
		pSpecialFlags |= 0x2;
	}
	if( pCubeFaceVisible[ 2 ] ){
		pSpecialFlags |= 0x8;
	}
	if( pCubeFaceVisible[ 3 ] ){
		pSpecialFlags |= 0x4;
	}
	if( pCubeFaceVisible[ 4 ] ){
		pSpecialFlags |= 0x10;
	}
	if( pCubeFaceVisible[ 5 ] ){
		pSpecialFlags |= 0x20;
	}
}



const decDVector &deoglRBillboard::GetCullSphereCenter(){
	if( pDirtyCulling ){
		pUpdateCullSphere();
		pDirtyCulling = false;
	}
	
	return pCullSphereCenter;
}

float deoglRBillboard::GetCullSphereRadius(){
	if( pDirtyCulling ){
		pUpdateCullSphere();
		pDirtyCulling = false;
	}
	
	return pCullSphereRadius;
}

void deoglRBillboard::SetDirtyCulling(){
	pDirtyCulling = true;
}



void deoglRBillboard::StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &cameraPosition ){
	if( ! pUseSkinTexture ){
		return;
	}
	
	const decVector minExtend( pMinExtend - cameraPosition );
	const decVector maxExtend( pMaxExtend - cameraPosition );
	occlusionTest.AddInputData( minExtend, maxExtend, this );
}

void deoglRBillboard::OcclusionTestInvisible(){
	pRenderVisible = false;
}

void deoglRBillboard::SetRenderVisible( bool visible ){
	pRenderVisible = visible;
}

void deoglRBillboard::SetSortDistance( float distance ){
	pSortDistance = distance;
}

void deoglRBillboard::SetOccluded( bool occluded ){
	pOccluded = occluded;
}

void deoglRBillboard::SetSkyShadowSplitMask( int mask ){
	pSkyShadowSplitMask = mask;
}



// Render world usage
///////////////////////

void deoglRBillboard::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}

void deoglRBillboard::SetRenderEnvMap( deoglEnvironmentMap *envmap ){
	// note about the switch process. we have to wait setting the fading environment map until the
	// new environment map has been set. if this is not done the SetRenderEnvMapFade function tries
	// to add the billboard to the billboard list of the same environment map as the current one
	// resulting in an exception. to avoid this the fade environment map is first cleared, then
	// the environment set and then the fade environment map is set again
	// 
	// another note. if a new environment map is set while the fading is still in progress a hard
	// switch may still occur. a possible solution would be to delay the switch until the fading
	// is finished. for this we would have to keep the dirty flag set, which is currently set
	// outside somewhere
	if( envmap == pRenderEnvMap ){
		return;
	}
	
	deoglEnvironmentMap * const prevEnvMap = pRenderEnvMap;
	const deObjectReference guard( prevEnvMap );
	
	if( pRenderEnvMap ){
		pRenderEnvMap->GetBillboardList().RemoveIfExisting( this );
		pRenderEnvMap->FreeReference();
	}
	
	pRenderEnvMap = envmap;
	
	if( envmap ){
		envmap->AddReference();
		envmap->GetBillboardList().Add( this );
	}
	
	// now it is safe to set the fade env map
	SetRenderEnvMapFade( prevEnvMap );
	pRenderEnvMapFadeFactor = 0.0f;
	
	if( ! prevEnvMap ){ // in case SetRenderEnvMapFade did not mark all textures dirty yet
		MarkTUCsDirty();
	}
}

void deoglRBillboard::SetRenderEnvMapFade( deoglEnvironmentMap *envmap ){
	if( envmap == pRenderEnvMapFade ){
		return;
	}
	
	if( pRenderEnvMapFade ){
		pRenderEnvMapFade->GetBillboardList().RemoveIfExisting( this );
		pRenderEnvMapFade->FreeReference();
	}
	
	pRenderEnvMapFade = envmap;
	
	if( envmap ){
		envmap->AddReference();
		envmap->GetBillboardList().Add( this );
	}
	
	MarkTUCsDirty();
}

void deoglRBillboard::SetRenderEnvMapFadePerTime( float fadePerTime ){
	if( fadePerTime < 0.1f ){
		pRenderEnvMapFadePerTime = 0.1f;
		
	}else{
		pRenderEnvMapFadePerTime = fadePerTime;
	}
}

void deoglRBillboard::SetRenderEnvMapFadeFactor( float factor ){
	if( factor < 0.0f ){
		factor = 0.0f;
		
	}else if( factor > 1.0f ){
		factor = 1.0f;
	}
	
	if( fabsf( factor - pRenderEnvMapFadeFactor ) > 0.001f ){
		pRenderEnvMapFadeFactor = factor;
		
	}
}

void deoglRBillboard::WorldEnvMapLayoutChanged(){
	pDirtyRenderEnvMap = true;
}

void deoglRBillboard::UpdateRenderEnvMap(){
	if( ! pDirtyRenderEnvMap ){
		return;
	}
	pDirtyRenderEnvMap = false;
	
	if( deoglSkinShader::REFLECTION_TEST_MODE == deoglSkinShader::ertmSingleBlenderEnvMap ){
		return;
	}
	
	DEASSERT_NOTNULL( pParentWorld )
	
	// for the time being we simply pick the environment map that is closest to the billboard position.
	// this can lead to wrong picks and harshly switching environment maps but this is enough for the
	// first test.
	// 
	// for the time being we choose no region but visit all environment maps existing in the world.
	// a more optimal solution would be to search in an area somewhat larger than the distance to the
	// currently used environment map. the new environment map has to be closer than the current one
	// to be better. if no environment map exists yet a full search could be done.
	// 
	// better solutions could be using grid like connection between environment maps. this way the next
	// best environment map can be searched by following the grid.
	// 
	// for the time being the center of the bounding box is used as the reference point. for large
	// billboards we've got the problem of what environment map to use. this though has to be solved
	// later on most probably by splitting up the billboard into smaller billboards.
	deoglFindBestEnvMap visitor;
	decDVector position;
	
	position = ( pMinExtend + pMaxExtend ) * 0.5;
	
	visitor.SetPosition( position );
	//pParentWorld->VisitRegion( pMinExtend, pMaxExtend, visitor );
	visitor.VisitList( pParentWorld->GetEnvMapList() );
	
	if( visitor.GetEnvMap() ){
		SetRenderEnvMap( visitor.GetEnvMap() );
		
	}else if( pParentWorld->GetSkyEnvironmentMap() ){
		SetRenderEnvMap( pParentWorld->GetSkyEnvironmentMap() );
		
	}else{
		SetRenderEnvMap( nullptr );
		SetRenderEnvMapFade( nullptr );
		pRenderEnvMapFadeFactor = 1.0f;
	}
	//pOgl->LogInfoFormat( "update billboard %p render env map %p\n", pBillboard, pRenderEnvMap );
}

void deoglRBillboard::InvalidateRenderEnvMap(){
	if( ! pRenderEnvMap && ! pRenderEnvMapFade ){
		return;
	}
	
	SetRenderEnvMap( nullptr );
	SetRenderEnvMapFade( nullptr );
	pDirtyRenderEnvMap = true;
}

void deoglRBillboard::InvalidateRenderEnvMapIf( deoglEnvironmentMap *envmap ){
	if( pRenderEnvMap == envmap || pRenderEnvMapFade == envmap ){
		InvalidateRenderEnvMap();
	}
}

void deoglRBillboard::SetLLWorldPrev( deoglRBillboard *billboard ){
	pLLWorldPrev = billboard;
}

void deoglRBillboard::SetLLWorldNext( deoglRBillboard *billboard ){
	pLLWorldNext = billboard;
}



void deoglRBillboard::WorldReferencePointChanged(){
	MarkParamBlocksDirty();
	MarkParamBlocksDirty();
}



void deoglRBillboard::PrepareForRender( deoglRenderPlan&, const deoglRenderPlanMasked *mask ){
	PrepareSkinStateConstructed();
	
	if( pDirtyPrepareSkinStateRenderables ){
		PrepareSkinStateRenderables( mask );
		pDirtyPrepareSkinStateRenderables = false;
		pDirtyRenderSkinStateRenderables = true;
	}
	
	pPrepareTUCs();
	pPrepareParamBlocks();
}

void deoglRBillboard::PrepareForRenderRender( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
	if( pDirtyRenderSkinStateRenderables ){
		RenderSkinStateRenderables( mask );
		pDirtyRenderSkinStateRenderables = false;
	}
}

void deoglRBillboard::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



// Private Functions
//////////////////////

void deoglRBillboard::pCleanUp(){
	SetParentWorld( NULL );
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pRenderEnvMap ){
		pRenderEnvMap->FreeReference();
	}
	if( pRenderEnvMapFade ){
		pRenderEnvMapFade->FreeReference();
	}
	
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
	if( pTUCEnvMap ){
		pTUCEnvMap->RemoveUsage();
	}
	if( pSkinState ){
		delete pSkinState;
	}
}



void deoglRBillboard::pUpdateCullSphere(){
	deoglDCollisionSphere sphere;
	deoglDCollisionBox box;
	
	box.SetFromExtends( pMinExtend, pMaxExtend );
	box.GetEnclosingSphere( &sphere );
	
	pCullSphereCenter = sphere.GetCenter();
	pCullSphereRadius = ( float )sphere.GetRadius();
}

void deoglRBillboard::pPrepareTUCs(){
	if( ! pDirtyTUCs ){
		return;
	}
	
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
	
	// counter
	if( pTUCCounter ){
		pTUCCounter->RemoveUsage();
		pTUCCounter = NULL;
	}
	pTUCCounter = BareGetTUCFor( deoglSkinTexturePipelines::etCounter );
	
	// envmap
	if( pTUCEnvMap ){
		pTUCEnvMap->RemoveUsage();
		pTUCEnvMap = NULL;
	}
	
	if( pUseSkinTexture ){
		deoglTexUnitConfig unit[ 8 ];
		
		if( pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV() ){
			unit[ 0 ].EnableArrayTextureFromChannel( pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectColor, pSkinState, pDynamicSkin,
				pRenderThread.GetDefaultTextures().GetColorArray() );
			
			unit[ 1 ].EnableArrayTextureFromChannel( pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectEmissivity, pSkinState, pDynamicSkin,
				pRenderThread.GetDefaultTextures().GetEmissivityArray() );
			
		}else{
			unit[ 0 ].EnableTextureFromChannel( pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectColor, pSkinState, pDynamicSkin,
				pRenderThread.GetDefaultTextures().GetColor() );
			
			unit[ 1 ].EnableTextureFromChannel( pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectEmissivity, pSkinState, pDynamicSkin,
				pRenderThread.GetDefaultTextures().GetEmissivity() );
		}
		
		pTUCEnvMap = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
		pTUCEnvMap->EnsureRTSTexture();
	}
	
	// finished
	pDirtyTUCs = false;
}

void deoglRBillboard::pPrepareParamBlocks(){
	if( ! pSharedSPBElement ){
		if( pRenderThread.GetChoices().GetSharedSPBUseSSBO() ){
			pSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinInstanceSSBO ).AddElement();
			
		}else if( pRenderThread.GetChoices().GetGlobalSharedSPBLists() ){
			pSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinInstanceUBO ).AddElement();
			
		}else{
			pSharedSPBElement = pRenderThread.GetBufferObject().GetBillboardSPBListUBO().AddElement();
		}
		
		pDirtySharedSPBElement = true;
	}
	
	if( pDirtySharedSPBElement ){
		if( pSharedSPBElement && pUseSkinTexture ){
			// it does not matter which shader type we use since all are required to use the
			// same shared spb instance layout
			deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
				GetAt( deoglSkinTexturePipelinesList::eptBillboard ).
				GetWithRef( deoglSkinTexturePipelines::etGeometry ).GetShader();
			
			// update parameter block area belonging to this element
			UpdateInstanceParamBlock( deoglSharedSPBElementMapBuffer( *pSharedSPBElement ),
				pSharedSPBElement->GetIndex(), skinShader );
		}
		
		pDirtySharedSPBElement = false;
	}
	
	if( ! pSharedSPBRTIGroup && pSharedSPBElement ){
		deoglSharedSPBRTIGroupList &list = pRenderThread.GetBufferObject().GetBillboardRTIGroups();
		deoglSharedSPB &spb = pSharedSPBElement->GetSPB();
		pSharedSPBRTIGroup.TakeOver( list.GetWith( spb ) );
		
		if( ! pSharedSPBRTIGroup ){
			pSharedSPBRTIGroup.TakeOver( list.AddWith( spb ) );
			
			deoglRenderTaskSharedInstance &rtsi = *pSharedSPBRTIGroup->GetRTSInstance();
			rtsi.SetSubInstanceSPB( &spb );
			rtsi.SetFirstPoint( 0 );
			rtsi.SetFirstIndex( 0 );
			rtsi.SetPointCount( 4 );
			rtsi.SetPrimitiveType( GL_TRIANGLE_FAN );
		}
	}
}

void deoglRBillboard::pRequiresPrepareForRender(){
	if( ! pLLPrepareForRenderWorld.GetList() && pParentWorld ){
		pParentWorld->AddPrepareForRenderBillboard( this );
	}
}
