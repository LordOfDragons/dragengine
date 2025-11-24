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

#include "deoglRComponent.h"
#include "deoglRComponentLOD.h"
#include "deoglRComponentTexture.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../gi/deoglGIBVHDynamic.h"
#include "../memory/deoglMemoryManager.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLODVertPosSet.h"
#include "../model/texture/deoglModelTexture.h"
#include "../model/face/deoglModelFace.h"
#include "../rendering/deoglRenderGeometry.h"
#include "../rendering/task/config/deoglRenderTaskConfigTexture.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../vbo/deoglVBOLayout.h"

#include <dragengine/common/exceptions.h>



//#define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer timer;

extern float elapsedCompCalcVBO;
extern float elapsedCompBuildVBO;
extern float elapsedCompWeights;
extern float elapsedCompTVert;
extern float elapsedCompNorTan;
extern float elapsedCompNormalize;
#endif

// Class deoglRComponentLOD
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRComponentLOD::deoglRComponentLOD( deoglRComponent &component, int lodIndex ) :
pComponent( component ),
pLODIndex( lodIndex ),

pVAO( NULL ),
pVBOLayout( NULL ),
pVBOBlock( NULL ),

pWeights( nullptr ),

pPositions( NULL ),
pRealNormals( NULL ),
pNormals( NULL ),
pTangents( NULL ),
pFaceNormals( NULL ),

pDirtyModelWeights( true ),
pDirtyModelPositions( true ),
pDirtyModelNorTan( true ),
pDirtyDataWeights( true ),
pDirtyDataPositions( true ),
pDirtyDataNorTan( true ),

pDirtyVBO( true ),
pDirtyVAO( true ),

pMemUse( component.GetRenderThread().GetMemoryManager().GetConsumption().bufferObject.vbo ),

pVBOTransformVertices( 0 ),
pTBOTransformVertices( 0 ),
pTexTransformNormTan( NULL ),

pGIBVHDynamic( NULL ),
pDirtyGIBVHPositions( true )
{
	LEAK_CHECK_CREATE( component.GetRenderThread(), ComponentLOD );
}

deoglRComponentLOD::~deoglRComponentLOD(){
	LEAK_CHECK_FREE( pComponent.GetRenderThread(), ComponentLOD );
	pCleanUp();
}



// Management
///////////////

int deoglRComponentLOD::GetPointOffset() const{
	if( pVAO ){
		return 0;
	}
	if( ! pVBOBlock ){
		return 0;
	}
	return pVBOBlock->GetOffset();
}

int deoglRComponentLOD::GetIndexOffset() const{
	if( pVAO ){
		if( pComponent.GetRenderThread().GetChoices().GetSharedVBOUseBaseVertex() ){
			if( ! pVBOBlock ){
				return 0;
			}
			return pVBOBlock->GetIndexOffset();
			
		}else{
			return 0;
		}
		
	}else{
		if( ! pVBOBlock ){
			return 0;
		}
		return pVBOBlock->GetIndexOffset();
	}
}

deoglVAO *deoglRComponentLOD::GetUseVAO() const{
	if( pVAO ){
		return pVAO;
	}
	if( GetModelLODRef().GetVBOBlock() ){
		return GetModelLODRef().GetVBOBlock()->GetVBO()->GetVAO();
	}
	return nullptr;
}

void deoglRComponentLOD::InvalidateVAO(){
	pDirtyVAO = true;
// 	pVBOBlock = NULL;
	pComponent.DirtyLODVBOs();
}

void deoglRComponentLOD::InvalidateVBO(){
	pDirtyDataWeights = true;
	pDirtyDataPositions = true;
	pDirtyGIBVHPositions = true;
	pDirtyDataNorTan = true;
	pDirtyVBO = true;
	pComponent.DirtyLODVBOs();
}

#define SPECIAL_DEBUG_ON 1

#ifdef SPECIAL_DEBUG_ON
#include <dragengine/common/utils/decTimer.h>
static decTimer specialTimer;
int extDebugCompCount = 0;
float extDebugCompCalculateWeights = 0.0f;
float extDebugCompTransformVertices = 0.0f;
float extDebugCompCalculateNormalsAndTangents = 0.0f;
float extDebugCompBuildVBO = 0.0f;
float extDebugCompTBO = 0.0f;
#endif

void deoglRComponentLOD::UpdateVBO(){
	if( pDirtyVAO ){
		if( pVAO ){
			delete pVAO;
			pVAO = NULL;
		}
		pDirtyVAO = false;
		
		pDirtyVBO = true;
		pVBOBlock = NULL;
	}
	
	if( pDirtyVBO ){
		if( pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			
			pPrepareVBOLayout( modelLOD );
			
			switch( pComponent.GetRenderThread().GetChoices().GetGPUTransformVertices() ){
			case deoglRTChoices::egputvNone:
				UpdateVBOOnCPU();
				break;
				
			case deoglRTChoices::egputvAccurate:
				
			case deoglRTChoices::egputvApproximate:
				UpdateVBOOnGPUApproximate();
				break;
			}
			
			pUpdateVAO( modelLOD );
			
		}else{
			FreeVBO();
		}
		
		pDirtyVBO = false;
	}
}

void deoglRComponentLOD::FreeVBO(){
	pVBO = nullptr;
	pMemUse = 0;
}



void deoglRComponentLOD::UpdateVBOOnCPU(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	
	PrepareNormalsTangents();
	
	#ifdef SPECIAL_DEBUG_ON
	extDebugCompCount++;
	specialTimer.Reset();
	#endif
	pBuildVBO( modelLOD );
	#ifdef DO_TIMING
	elapsedCompBuildVBO += timer.GetElapsedTime();
	#endif
	#ifdef SPECIAL_DEBUG_ON
	extDebugCompBuildVBO += specialTimer.GetElapsedTime();
	#endif
}



void deoglRComponentLOD::WriteWeightMatricesSSBO(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int weightsCount = modelLOD.GetWeightsCount();
	if( weightsCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	
	PrepareWeights();
	
	if( ! pSSBOWeightMatrices ){
		pSSBOWeightMatrices.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etStream ) );
		deoglSPBlockSSBO &ssbo = pSSBOWeightMatrices;
		ssbo.SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ssbo.SetParameterCount( 1 );
		ssbo.GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 );
		ssbo.SetElementCount( weightsCount );
		ssbo.MapToStd140();
		ssbo.SetBindingPoint( 2 );
	}
	
	const deoglSPBMapBuffer mapped( pSSBOWeightMatrices );
	int i;
	for( i=0; i<weightsCount; i++ ){
		pSSBOWeightMatrices->SetParameterDataMat4x3( 0, i, pWeights[ i ] );
	}
}



void deoglRComponentLOD::UpdateVBOOnGPUApproximate(){
	PrepareWeights();
	
	WriteWeightMatricesSSBO();
	GPUApproxTransformVNT();
	
	#ifdef SPECIAL_DEBUG_ON
	extDebugCompCount++;
	extDebugCompTBO += specialTimer.GetElapsedTime();
	#endif
}

void deoglRComponentLOD::GPUApproxTransformVNT(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int pointCount = modelLOD.GetVertexCount();
	
	if( pointCount == 0 || ! modelLOD.GetVBOBlockWithWeight() ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	deoglSharedVBOBlock &vboBlock = *modelLOD.GetVBOBlockWithWeight();
	vboBlock.Prepare();
	
	pEnsureVBO();
	
	deoglRenderGeometry &renderGeometry = renderThread.GetRenderers().GetGeometry();
	const float * const vpsWeights = pComponent.GetVertexPositionSets();
	const int vpsCount = pComponent.GetVertexPositionSetCount();
	const GLuint vaoModelData = vboBlock.GetVBO()->GetVAO()->GetVAO();
	const GLuint vboModelData = vboBlock.GetVBO()->GetVBO();
	deoglRenderGeometry::sVertexPositionSetParams *params = nullptr;
	int firstPoint = vboBlock.GetOffset();
	bool inplace = false;
	
	int i, useVpsCount = 0;
	if( vpsCount > 0 ){
		params = renderGeometry.GetVertexPositionSetParams( vpsCount );
		
		for( i=0; i<vpsCount; i++ ){
			if( vpsWeights[ i ] < 0.001f ){
				continue;
			}
			
			const deoglModelLODVertPosSet &vps = modelLOD.GetVertexPositionSetAt( i );
			if( vps.GetPositionCount() == 0 ){
				continue;
			}
			
			deoglRenderGeometry::sVertexPositionSetParams &param = params[ useVpsCount++ ];
			param.firstPoint = vps.GetVBOOffset();
			param.pointCount = vps.GetPositionCount();
			param.weight = vpsWeights[ i ];
		}
	}
	
	if( useVpsCount > 0 ){
		deoglSharedVBOBlock &vboBlockVps = *modelLOD.GetVBOBlockVertPosSet();
		vboBlockVps.Prepare();
		
		const int blockOffset = vboBlockVps.GetOffset();
		for( i=0; i<useVpsCount; i++ ){
			params[ i ].firstPoint += blockOffset;
		}
		
		const GLuint vboVPSData = vboBlockVps.GetVBO()->GetVBO();
		
		renderGeometry.CopyVNT( vaoModelData, vboModelData, pVBO, firstPoint, pointCount );
		renderGeometry.VPSTransformVNT( vaoModelData, vboVPSData, params, useVpsCount, pVBO );
		
		inplace = true;
	}
	
	renderGeometry.ApproxTransformVNT( vaoModelData, vboModelData, pSSBOWeightMatrices,
		pVBO, firstPoint, pointCount, inplace );
}



void deoglRComponentLOD::PrepareGIDynamicBVH(){
	if( ! pGIBVHDynamic ){
		deoglModelLOD &modelLOD = GetModelLODRef();
		modelLOD.PrepareGILocalBVH();
		pGIBVHDynamic = new deoglGIBVHDynamic( *modelLOD.GetGIBVHLocal() );
	}
	
	if( pDirtyGIBVHPositions ){
		PreparePositions();
		if( pGIBVHDynamic->GetTBOVertex()->GetDataCount() > 0 ){
			pGIBVHDynamic->UpdateVertices( pPositions, GetModelLODRef().GetPositionCount() );
			pGIBVHDynamic->UpdateBVHExtends();
		}
		pDirtyGIBVHPositions = false;
	}
}

void deoglRComponentLOD::DropGIDynamicBVH(){
	if( pGIBVHDynamic ){
		delete pGIBVHDynamic;
		pGIBVHDynamic = NULL;
	}
}



const deoglRenderTaskConfig *deoglRComponentLOD::GetRenderTaskConfig( deoglSkinTexturePipelines::eTypes type ) const{
	switch( type ){
	case deoglSkinTexturePipelines::etShadowProjection:
		return &pRenderTaskConfigs[ 0 ];
		
	case deoglSkinTexturePipelines::etShadowProjectionCube:
		return &pRenderTaskConfigs[ 1 ];
		
	case deoglSkinTexturePipelines::etShadowOrthogonal:
		return &pRenderTaskConfigs[ 2 ];
		
	case deoglSkinTexturePipelines::etShadowOrthogonalCascaded:
		return &pRenderTaskConfigs[ 3 ];
		
	case deoglSkinTexturePipelines::etShadowDistance:
		return &pRenderTaskConfigs[ 4 ];
		
	case deoglSkinTexturePipelines::etShadowDistanceCube:
		return &pRenderTaskConfigs[ 5 ];
		
	default:
		return nullptr;
	}
}

void deoglRComponentLOD::UpdateRenderTaskConfigurations(){
	const deoglSkinTexturePipelines::eTypes typesShadow[ 6 ] = {
		deoglSkinTexturePipelines::etShadowProjection,
		deoglSkinTexturePipelines::etShadowProjectionCube,
		deoglSkinTexturePipelines::etShadowOrthogonal,
		deoglSkinTexturePipelines::etShadowOrthogonalCascaded,
		deoglSkinTexturePipelines::etShadowDistance,
		deoglSkinTexturePipelines::etShadowDistanceCube };
	int i;
	
	for( i=0; i<6; i++ ){
		pRenderTaskConfigs[ i ].RemoveAllTextures();
	}
	
	if( ! pComponent.GetModel() ){
		return;
	}
	
	const deoglVAO * const vao = GetUseVAO();
	if( ! vao ){
		return;
	}
	
	const deoglRenderTaskSharedVAO * const rtvao = vao->GetRTSVAO();
	const deoglModelLOD &modelLod = GetModelLODRef();
	const int count = pComponent.GetTextureCount();
	const int rtfmShadow = ertfRender | ertfDecal | ertfShadowNone;
	const int rtfShadow = ertfRender;
	
	for( i=0; i<count; i++ ){
		if( modelLod.GetTextureAt( i ).GetFaceCount() == 0 ){
			continue;
		}
		
		const deoglRComponentTexture &texture = pComponent.GetTextureAt( i );
		if( ( texture.GetRenderTaskFilters() & rtfmShadow ) != rtfShadow ){
			continue;
		}
		
		const deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
		if( ! skinTexture ){
			continue; // actually covered by filter above but better safe than sorry
		}
		
		const deoglSharedSPBRTIGroup * const group = texture.GetSharedSPBRTIGroupShadow( pLODIndex );
		deoglRenderTaskSharedInstance *rtsi;
		
		if( group ){
			rtsi = group->GetRTSInstance();
			i += group->GetTextureCount() - 1;
			
		}else{
			rtsi = texture.GetSharedSPBRTIGroup( pLODIndex ).GetRTSInstance();
		}
		
		deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType;
		int filterMask = ~0;
		
		if(texture.GetUseDecal()){
			pipelinesType = deoglSkinTexturePipelinesList::eptDecal;
			if(!pComponent.GetSolid()){
				filterMask &= ~ertfSolid;
			}
			
		}else{
			pipelinesType = deoglSkinTexturePipelinesList::eptComponent;
		}
		
		int j;
		for( j=0; j<6; j++ ){
			deoglRenderTaskConfigTexture &rct = pRenderTaskConfigs[ j ].AddTexture();
			rct.SetRenderTaskFilter(texture.GetRenderTaskFilters() & filterMask);
			rct.SetPipeline( skinTexture->GetPipelines().GetAt( pipelinesType ).GetWithRef( typesShadow[ j ] ).GetPipeline() );
			const deoglTexUnitsConfig *tuc = texture.GetTUCForPipelineType( typesShadow[ j ] );
			if( ! tuc ){
				tuc = pComponent.GetRenderThread().GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
			}
			rct.SetTexture( tuc->GetRTSTexture() );
			rct.SetVAO( rtvao );
			rct.SetInstance( rtsi );
			rct.SetGroupIndex( texture.GetSharedSPBElement()->GetIndex() );
		}
	}
}



// Private Functions
//////////////////////

deoglModelLOD *deoglRComponentLOD::GetModelLOD() const{
	const deoglRModel * const model = pComponent.GetModel();
	return model ? &model->GetLODAt( pLODIndex ) : NULL;
}

deoglModelLOD &deoglRComponentLOD::GetModelLODRef() const{
	return pComponent.GetModel()->GetLODAt( pLODIndex );
}

void deoglRComponentLOD::PrepareWeights(){
	if( pDirtyModelWeights ){
		oglMatrix3x4 *weights = nullptr;
		
		if( pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			const int weightsCount = modelLOD.GetWeightsCount();
			if( weightsCount > 0 ){
				weights = new oglMatrix3x4[ weightsCount ];
			}
			// NOTE weights count can be 0 if a higher level LOD has all weightless vertices.
			//      this case can be optimized to using static rendering
		}
		
		if( pWeights ){
			delete [] pWeights;
		}
		pWeights = weights;
		
		pDirtyModelWeights = false;
	}
	
	if( pDirtyDataWeights ){
		if( pWeights && pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			
			#ifdef SPECIAL_DEBUG_ON
			specialTimer.Reset();
			#endif
			pCalculateWeights( modelLOD );
			#ifdef SPECIAL_DEBUG_ON
			extDebugCompCalculateWeights += specialTimer.GetElapsedTime();
			#endif
			#ifdef DO_TIMING
			elapsedCompWeights += timer.GetElapsedTime();
			#endif
		}
		
		pDirtyDataWeights = false;
	}
}

void deoglRComponentLOD::PreparePositions(){
	PrepareWeights();
	
	if( pDirtyModelPositions ){
		oglVector3 *positions = NULL;
		
		if( pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			const int positionCount = modelLOD.GetPositionCount();
			if( positionCount > 0 ){
				positions = new oglVector3[ positionCount ];
			}
		}
		
		if( pPositions ){
			delete [] pPositions;
		}
		pPositions = positions;
		
		pDirtyModelPositions = false;
	}
	
	if( pDirtyDataPositions ){
		// pWeights can not be checked to be non-NULL since higher level LODs can contain
		// all weightless vertices although lower level LODs have weighted vertices. in this
		// situation all vertices are copied non-transformed
		if( pPositions && pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			
			#ifdef SPECIAL_DEBUG_ON
			specialTimer.Reset();
			#endif
			pTransformVertices( modelLOD );
			#ifdef SPECIAL_DEBUG_ON
			extDebugCompTransformVertices += specialTimer.GetElapsedTime();
			#endif
			#ifdef DO_TIMING
			elapsedCompTVert += timer.GetElapsedTime();
			#endif
		}
		
		pDirtyDataPositions = false;
	}
}

void deoglRComponentLOD::PrepareNormalsTangents(){
	PreparePositions();
	
	if( pDirtyModelNorTan ){
		oglVector3 *realNormals = NULL;
		oglVector3 *normals = NULL;
		oglVector3 *tangents = NULL;
		oglVector3 *faceNormals = NULL;
		
		if( pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			
			const int positionCount = modelLOD.GetPositionCount();
			const int normalCount = modelLOD.GetNormalCount();
			const int tangentCount = modelLOD.GetTangentCount();
			const int faceCount = modelLOD.GetFaceCount();
			
			try{
				if( positionCount > 0 ){
					realNormals = new oglVector3[ positionCount ];
				}
				if( normalCount > 0 ){
					normals = new oglVector3[ normalCount ];
				}
				if( tangentCount > 0 ){
					tangents = new oglVector3[ tangentCount ];
				}
				if( faceCount > 0 ){
					faceNormals = new oglVector3[ faceCount ];
				}
				
			}catch( const deException & ){
				if( faceNormals ){
					delete [] faceNormals;
				}
				if( tangents ){
					delete [] tangents;
				}
				if( realNormals ){
					delete [] realNormals;
				}
				if( normals ){
					delete [] normals;
				}
				throw;
			}
		}
		
		if( pRealNormals ){
			delete [] pRealNormals;
		}
		pRealNormals = realNormals;
		
		if( pNormals ){
			delete [] pNormals;
		}
		pNormals = normals;
		
		if( pTangents ){
			delete [] pTangents;
		}
		pTangents = tangents;
		
		if( pFaceNormals ){
			delete [] pFaceNormals;
		}
		pFaceNormals = faceNormals;
		
		pDirtyModelNorTan = false;
	}
	
	if( pDirtyDataNorTan ){
		if( pWeights && pPositions && pRealNormals && pNormals && pTangents && pFaceNormals
		&& pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			const deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			
			#ifdef SPECIAL_DEBUG_ON
			specialTimer.Reset();
			#endif
			pCalculateNormalsAndTangents( modelLOD );
			#ifdef SPECIAL_DEBUG_ON
			extDebugCompCalculateNormalsAndTangents += specialTimer.GetElapsedTime();
			#endif
			#ifdef DO_TIMING
			elapsedCompNorTan += timer.GetElapsedTime();
			#endif
		}
		
		pDirtyDataNorTan = false;
	}
}



// Private Functions
//////////////////////

void deoglRComponentLOD::pCleanUp(){
	DropGIDynamicBVH();
	
	if( pVBOLayout ){
		delete pVBOLayout;
	}
	
	if( pFaceNormals ){
		delete [] pFaceNormals;
	}
	if( pTangents ){
		delete [] pTangents;
	}
	if( pNormals ){
		delete [] pNormals;
	}
	if( pRealNormals ){
		delete [] pRealNormals;
	}
	if( pPositions ){
		delete [] pPositions;
	}
	if( pWeights ){
		delete [] pWeights;
	}
	
	pMemUse = 0;
	
	if( pTexTransformNormTan ){
		delete pTexTransformNormTan;
	}
	pFBOCalcNormalTangent = nullptr;
	if( pVAO ){
		delete pVAO;
	}
	
	deoglDelayedOperations &dops = pComponent.GetRenderThread().GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pTBOTransformVertices );
	dops.DeleteOpenGLBuffer( pVBOTransformVertices );
}



void deoglRComponentLOD::pEnsureVBO(){
	if( pVBO ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	const int pointCount = pComponent.GetModel()->GetLODAt( pLODIndex ).GetVertexCount();
	
	pVBO.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etGpu ) );
	
	deoglSPBlockSSBO &vbo = pVBO;
	vbo.SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	vbo.SetParameterCount( 4 );
	vbo.GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 position
	vbo.GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 realNormal
	vbo.GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 normal
	vbo.GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec3 tangent
	vbo.SetElementCount( pointCount );
	vbo.MapToStd140();
	vbo.SetBindingPoint( 1 );
	
	pVBO->EnsureBuffer();
	pMemUse = pVBOLayout->GetStride() * pointCount;
}

void deoglRComponentLOD::pBuildVBO( const deoglModelLOD &modelLOD ){
	pEnsureVBO();
	
	if( pPositions && pRealNormals && pNormals && pTangents ){
		pWriteVBOData( modelLOD );
	}
}

void deoglRComponentLOD::pWriteVBOData( const deoglModelLOD &modelLOD ){
	const bool * const negateTangents = modelLOD.GetNegateTangents();
	const oglModelVertex * const points = modelLOD.GetVertices();
	const int pointCount = modelLOD.GetVertexCount();
	const deoglSPBMapBuffer mapped( pVBO );
	int i;
	
	for( i=0; i<pointCount; i++ ){
		const oglModelVertex &point = points[ i ];
		pVBO->SetParameterDataVec3( 0, i, pPositions[ point.position ] );
		pVBO->SetParameterDataVec3( 1, i, pRealNormals[ point.position ] );
		pVBO->SetParameterDataVec3( 2, i, pNormals[ point.normal ] );
		pVBO->SetParameterDataVec4( 3, i, pTangents[ point.tangent ],
			negateTangents[ point.tangent ] ? -1.0f : 1.0f );
	}
}

void deoglRComponentLOD::pUpdateVAO( deoglModelLOD &modelLOD ){
	if( pVAO || ! pVBO || ! modelLOD.GetVBOBlock() ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	pVBOBlock = modelLOD.GetVBOBlock();
	const GLuint vboModel = pVBOBlock->GetVBO()->GetVBO();
	const deoglVBOLayout &vboLayout = pVBOBlock->GetVBO()->GetParentList()->GetLayout();
	
	pVAO = new deoglVAO( renderThread );
	OGL_CHECK( renderThread, pglBindVertexArray( pVAO->GetVAO() ) );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO->GetSSBO() ) );
	pVBOLayout->SetVAOAttributeAt( renderThread, 0, 0 ); // pos(0) => vao(0)
	pVBOLayout->SetVAOAttributeAt( renderThread, 1, 1 ); // realNormal(1) => vao(1)
	pVBOLayout->SetVAOAttributeAt( renderThread, 2, 2 ); // normal(2) => vao(2)
	pVBOLayout->SetVAOAttributeAt( renderThread, 3, 3 ); // tangent(3) => vao(3)
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, vboModel ) );
	vboLayout.SetVAOAttributeAt( renderThread, 4, 4, vboLayout.GetStride() * pVBOBlock->GetOffset() );
		/*renderThread.GetChoices().GetSharedVBOUseBaseVertex()
		? vboLayout.GetStride() * pVBOBlock->GetOffset() : 0 );*/
			// texcoord(3) => vao_tc_diffuse(3)
	
	if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() && pVBOBlock->GetVBO()->GetIBO() ){
		OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pVBOBlock->GetVBO()->GetIBO() ) );
		pVAO->SetIndexType( vboLayout.GetIndexType() );
		
	}else{
		// the IBO from the shared VBO block can not be used because the indices are offsetted.
		// until transforming is done on-the-fly we use the IBO from the model lod
		OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, modelLOD.GetIBO() ) );
		pVAO->SetIndexType( modelLOD.GetIBOType() );
	}
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	
	pVAO->EnsureRTSVAO();
	
	pComponent.UpdateRTSInstances();
}



void deoglRComponentLOD::pCalculateWeights( const deoglModelLOD &modelLOD ){
	const oglModelWeight * weightsEntries = modelLOD.GetWeightsEntries();
	const int * const weightsCounts = modelLOD.GetWeightsCounts();
	const int weightsCount = modelLOD.GetWeightsCount();
	int w, e, entryCount;
	float factor;
	
	//pComponent.UpdateBoneMatrices(); // done already by deoglComponent during synching
	
	const oglMatrix3x4 * const boneMatrices = pComponent.GetBoneMatrices();
	
	for( w=0; w<weightsCount; w++ ){
		oglMatrix3x4 &weightsMatrix = pWeights[ w ];
		entryCount = weightsCounts[ w ];
		
		if( entryCount == 0 ){
			weightsMatrix.a11 = 1.0f;
			weightsMatrix.a12 = 0.0f;
			weightsMatrix.a13 = 0.0f;
			weightsMatrix.a14 = 0.0f;
			weightsMatrix.a21 = 0.0f;
			weightsMatrix.a22 = 1.0f;
			weightsMatrix.a23 = 0.0f;
			weightsMatrix.a24 = 0.0f;
			weightsMatrix.a31 = 0.0f;
			weightsMatrix.a32 = 0.0f;
			weightsMatrix.a33 = 1.0f;
			weightsMatrix.a34 = 0.0f;
			
		}else if( entryCount == 1 ){
			const oglMatrix3x4 &boneMatrix = boneMatrices[ weightsEntries->bone ];
			
			weightsMatrix.a11 = boneMatrix.a11;
			weightsMatrix.a12 = boneMatrix.a12;
			weightsMatrix.a13 = boneMatrix.a13;
			weightsMatrix.a14 = boneMatrix.a14;
			weightsMatrix.a21 = boneMatrix.a21;
			weightsMatrix.a22 = boneMatrix.a22;
			weightsMatrix.a23 = boneMatrix.a23;
			weightsMatrix.a24 = boneMatrix.a24;
			weightsMatrix.a31 = boneMatrix.a31;
			weightsMatrix.a32 = boneMatrix.a32;
			weightsMatrix.a33 = boneMatrix.a33;
			weightsMatrix.a34 = boneMatrix.a34;
			
			weightsEntries++;
			
		}else{
			const oglMatrix3x4 &boneMatrix = boneMatrices[ weightsEntries->bone ];
			factor = weightsEntries->weight;
			
			weightsMatrix.a11 = boneMatrix.a11 * factor;
			weightsMatrix.a12 = boneMatrix.a12 * factor;
			weightsMatrix.a13 = boneMatrix.a13 * factor;
			weightsMatrix.a14 = boneMatrix.a14 * factor;
			weightsMatrix.a21 = boneMatrix.a21 * factor;
			weightsMatrix.a22 = boneMatrix.a22 * factor;
			weightsMatrix.a23 = boneMatrix.a23 * factor;
			weightsMatrix.a24 = boneMatrix.a24 * factor;
			weightsMatrix.a31 = boneMatrix.a31 * factor;
			weightsMatrix.a32 = boneMatrix.a32 * factor;
			weightsMatrix.a33 = boneMatrix.a33 * factor;
			weightsMatrix.a34 = boneMatrix.a34 * factor;
			
			weightsEntries++;
			
			for( e=1; e<entryCount; e++ ){
				const oglMatrix3x4 &boneMatrix2 = boneMatrices[ weightsEntries->bone ];
				factor = weightsEntries->weight;
				
				weightsMatrix.a11 += boneMatrix2.a11 * factor;
				weightsMatrix.a12 += boneMatrix2.a12 * factor;
				weightsMatrix.a13 += boneMatrix2.a13 * factor;
				weightsMatrix.a14 += boneMatrix2.a14 * factor;
				weightsMatrix.a21 += boneMatrix2.a21 * factor;
				weightsMatrix.a22 += boneMatrix2.a22 * factor;
				weightsMatrix.a23 += boneMatrix2.a23 * factor;
				weightsMatrix.a24 += boneMatrix2.a24 * factor;
				weightsMatrix.a31 += boneMatrix2.a31 * factor;
				weightsMatrix.a32 += boneMatrix2.a32 * factor;
				weightsMatrix.a33 += boneMatrix2.a33 * factor;
				weightsMatrix.a34 += boneMatrix2.a34 * factor;
				
				weightsEntries++;
			}
		}
	}
#if 0
	if( weightsCount > 0 ){
		deGraphicOpenGl &ogl = *pComponent.GetOgl();
		
		if( ! pVBOWeightMatrices ){
			OGL_CHECK( ogl, pglGenBuffers( 1, &pVBOWeightMatrices ) );
			if( ! pVBOWeightMatrices ){
				DETHROW( deeOutOfMemory );
			}
			OGL_CHECK( ogl, pglBindBuffer( GL_ARRAY_BUFFER, pVBOWeightMatrices ) );
			OGL_CHECK( ogl, pglBufferData( GL_ARRAY_BUFFER, sizeof( oglMatrix3x4 ) * weightsCount, pWeights, GL_STATIC_DRAW ) );
		}
		
		if( ! pTBOWeightMatrices ){
			deoglTextureStageManager &tsmgr = ogl.GetRenderThread().GetTexture().GetStages();
			
			OGL_CHECK( ogl, glGenTextures( 1, &pTBOWeightMatrices ) );
			if( ! pTBOWeightMatrices ){
				DETHROW( deeInvalidParam );
			}
			tsmgr.EnableBareTBO( 0, pTBOWeightMatrices );
			OGL_CHECK( ogl, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32F, pVBOWeightMatrices ) );
			tsmgr.DisableStage( 0 );
		}
	}
#endif
}

void deoglRComponentLOD::pTransformVertices( const deoglModelLOD &modelLOD ){
	const oglModelPosition * const positions = modelLOD.GetPositions();
	const int positionCount = modelLOD.GetPositionCount();
	int i;
	
	if( ! pWeights ){
		// happens if higher LOD has only weightless vertices while lower LOD has weighted
		// vertices. this extra check avoids potential bugs if pWeights is incorrectly NULL
		for( i=0; i<positionCount; i++ ){
			const decVector &orgpos = positions[ i ].position;
			oglVector3 &trpos = pPositions[ i ];
			trpos.x = orgpos.x;
			trpos.y = orgpos.y;
			trpos.z = orgpos.z;
		}
		return;
	}
	
	for( i=0; i<positionCount; i++ ){
		const oglModelPosition &modelPosition = positions[ i ];
		const decVector &orgpos = positions[ i ].position;
		oglVector3 &trpos = pPositions[ i ];
		
		if( modelPosition.weights == -1 ){
			trpos.x = orgpos.x;
			trpos.y = orgpos.y;
			trpos.z = orgpos.z;
			
		}else{
			const oglMatrix3x4 &matrix = pWeights[ modelPosition.weights ];
			
			trpos.x = matrix.a11 * orgpos.x + matrix.a12 * orgpos.y + matrix.a13 * orgpos.z + matrix.a14;
			trpos.y = matrix.a21 * orgpos.x + matrix.a22 * orgpos.y + matrix.a23 * orgpos.z + matrix.a24;
			trpos.z = matrix.a31 * orgpos.x + matrix.a32 * orgpos.y + matrix.a33 * orgpos.z + matrix.a34;
		}
	}
}

void deoglRComponentLOD::pCalculateNormalsAndTangents( const deoglModelLOD &modelLOD ){
	const decVector2 * const texcoords = modelLOD.GetTextureCoordinates();
	const oglModelVertex * const points = modelLOD.GetVertices();
	const deoglModelFace * const faces = modelLOD.GetFaces();
	const int positionCount = modelLOD.GetPositionCount();
	const int tangentCount = modelLOD.GetTangentCount();
	const int normalCount = modelLOD.GetNormalCount();
	const int faceCount = modelLOD.GetFaceCount();
	oglVector3 edge1, edge2;
	oglVector3 tangent;
	decVector2 d1, d2;
	float len, invlen;
	int i;
	
	// reset normals and tangents
	for( i=0; i<positionCount; i++ ){
		pRealNormals[ i ].x = 0.0f;
		pRealNormals[ i ].y = 0.0f;
		pRealNormals[ i ].z = 0.0f;
	}
	for( i=0; i<normalCount; i++ ){
		pNormals[ i ].x = 0.0f;
		pNormals[ i ].y = 0.0f;
		pNormals[ i ].z = 0.0f;
	}
	for( i=0; i<tangentCount; i++ ){
		pTangents[ i ].x = 0.0f;
		pTangents[ i ].y = 0.0f;
		pTangents[ i ].z = 0.0f;
	}
	
	// calculate normals and tangents
	for( i=0; i<faceCount; i++ ){
		const deoglModelFace &face = faces[ i ];
		const oglModelVertex &point1 = points[ face.GetVertex1() ];
		const oglModelVertex &point2 = points[ face.GetVertex2() ];
		const oglModelVertex &point3 = points[ face.GetVertex3() ];
		const oglVector3 &position1 = pPositions[ point1.position ];
		const oglVector3 &position2 = pPositions[ point2.position ];
		const oglVector3 &position3 = pPositions[ point3.position ];
		const decVector2 &texcoord1 = texcoords[ point1.texcoord ];
		const decVector2 &texcoord2 = texcoords[ point2.texcoord ];
		const decVector2 &texcoord3 = texcoords[ point3.texcoord ];
		
		oglVector3 &faceNormal = pFaceNormals[ i ];
		
		// calculate edges
		edge1.x = position2.x - position1.x;
		edge1.y = position2.y - position1.y;
		edge1.z = position2.z - position1.z;
		edge2.x = position3.x - position1.x;
		edge2.y = position3.y - position1.y;
		edge2.z = position3.z - position1.z;
		
		// calculate normal
		faceNormal.x = edge1.y * edge2.z - edge1.z * edge2.y;
		faceNormal.y = edge1.z * edge2.x - edge1.x * edge2.z;
		faceNormal.z = edge1.x * edge2.y - edge1.y * edge2.x;
		
		len = sqrtf( faceNormal.x * faceNormal.x + faceNormal.y * faceNormal.y + faceNormal.z * faceNormal.z );
		if( len != 0.0f ){
			invlen = 1.0f / len;
			faceNormal.x *= invlen;
			faceNormal.y *= invlen;
			faceNormal.z *= invlen;
			
		}else{
			faceNormal.x = 0.0f;
			faceNormal.y = 1.0f;
			faceNormal.z = 0.0f;
		}
		
		// add to real normals
		oglVector3 &vrn1 = pRealNormals[ point1.position ];
		vrn1.x += faceNormal.x;
		vrn1.y += faceNormal.y;
		vrn1.z += faceNormal.z;
		
		oglVector3 &vrn2 = pRealNormals[ point2.position ];
		vrn2.x += faceNormal.x;
		vrn2.y += faceNormal.y;
		vrn2.z += faceNormal.z;
		
		oglVector3 &vrn3 = pRealNormals[ point3.position ];
		vrn3.x += faceNormal.x;
		vrn3.y += faceNormal.y;
		vrn3.z += faceNormal.z;
		
		// add to normals
		oglVector3 &vn1 = pNormals[ point1.normal ];
		vn1.x += faceNormal.x;
		vn1.y += faceNormal.y;
		vn1.z += faceNormal.z;
		
		oglVector3 &vn2 = pNormals[ point2.normal ];
		vn2.x += faceNormal.x;
		vn2.y += faceNormal.y;
		vn2.z += faceNormal.z;
		
		oglVector3 &vn3 = pNormals[ point3.normal ];
		vn3.x += faceNormal.x;
		vn3.y += faceNormal.y;
		vn3.z += faceNormal.z;
		
		// calculate texture coordinate deltas
		d1 = texcoord2 - texcoord1;
		d2 = texcoord3 - texcoord1;
		
		// calculate tangent
		tangent.x = edge1.x * d2.y - edge2.x * d1.y;
		tangent.y = edge1.y * d2.y - edge2.y * d1.y;
		tangent.z = edge1.z * d2.y - edge2.z * d1.y;
		
		len = sqrtf( tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z );
		if( len != 0.0f ){
			invlen = 1.0f / len;
			tangent.x *= invlen;
			tangent.y *= invlen;
			tangent.z *= invlen;
			
		}else{
			tangent.x = 1.0f;
			tangent.y = 0.0f;
			tangent.z = 0.0f;
		}
		
		// add to tangents
		oglVector3 &vt1 = pTangents[ point1.tangent ];
		vt1.x += tangent.x;
		vt1.y += tangent.y;
		vt1.z += tangent.z;
		
		oglVector3 &vt2 = pTangents[ point2.tangent ];
		vt2.x += tangent.x;
		vt2.y += tangent.y;
		vt2.z += tangent.z;
		
		oglVector3 &vt3 = pTangents[ point3.tangent ];
		vt3.x += tangent.x;
		vt3.y += tangent.y;
		vt3.z += tangent.z;
	}
	
	// shaders do not require normalized normals and tangents but to prevent problems due to
	// zero length vectors we filter them out now in a fast way
	for( i=0; i<positionCount; i++ ){
		if( pRealNormals[ i ].x == 0.0f && pRealNormals[ i ].y == 0.0f && pRealNormals[ i ].z == 0.0f ){
			pRealNormals[ i ].x = 0.0f;
			pRealNormals[ i ].y = 1.0f;
			pRealNormals[ i ].z = 0.0f;
		}
	}
	
	for( i=0; i<normalCount; i++ ){
		if( pNormals[ i ].x == 0.0f && pNormals[ i ].y == 0.0f && pNormals[ i ].z == 0.0f ){
			pNormals[ i ].x = 0.0f;
			pNormals[ i ].y = 1.0f;
			pNormals[ i ].z = 0.0f;
		}
	}
	
	for( i=0; i<tangentCount; i++ ){
		if( pTangents[ i ].x == 0.0f && pTangents[ i ].y == 0.0f && pTangents[ i ].z == 0.0f ){
			pTangents[ i ].x = 1.0f;
			pTangents[ i ].y = 0.0f;
			pTangents[ i ].z = 0.0f;
		}
	}
}



void deoglRComponentLOD::pPrepareVBOLayout( const deoglModelLOD &modelLOD ){
	if( pVBOLayout ){
		return;
	}
	
	pVBOLayout = new deoglVBOLayout;
 	
 	// the layout of attributes. has to be ssbo compatible:
 	//
	// name        | offset | type  | components
	// ------------+--------+-------+------------
	// position    |      0 | float | x, y, z
	// real-normal |     16 | float | x, y, z
	// normal      |     32 | float | x, y, z
	// tangent     |     48 | float | x, y, z, w
	pVBOLayout->SetAttributeCount( 4 );
	pVBOLayout->SetStride( 64 );
 	pVBOLayout->SetSize( pVBOLayout->GetStride() * modelLOD.GetVertexCount() );
	pVBOLayout->SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	deoglVBOAttribute &attrPos = pVBOLayout->GetAttributeAt( 0 );
	attrPos.SetComponentCount( 3 );
	attrPos.SetDataType( deoglVBOAttribute::edtFloat );
	attrPos.SetOffset( 0 );
	
	deoglVBOAttribute &attrRealNormal = pVBOLayout->GetAttributeAt( 1 );
	attrRealNormal.SetComponentCount( 3 );
	attrRealNormal.SetDataType( deoglVBOAttribute::edtFloat );
	attrRealNormal.SetOffset( 16 );
	
	deoglVBOAttribute &attrNormal = pVBOLayout->GetAttributeAt( 2 );
	attrNormal.SetComponentCount( 3 );
	attrNormal.SetDataType( deoglVBOAttribute::edtFloat );
	attrNormal.SetOffset( 32 );
	
	deoglVBOAttribute &attrTangent = pVBOLayout->GetAttributeAt( 3 );
	attrTangent.SetComponentCount( 4 );
	attrTangent.SetDataType( deoglVBOAttribute::edtFloat );
	attrTangent.SetOffset( 48 );
}

void deoglRComponentLOD::pUpdateRenderTaskConfig( deoglRenderTaskConfig &config,
deoglSkinTexturePipelines::eTypes type, int renderTaskFlags, int renderTaskFlagMask, bool shadow ){
	config.RemoveAllTextures();
	
	if( ! pComponent.GetModel() ){
		return;
	}
	
	const deoglVAO * const vao = GetUseVAO();
	if( ! vao ){
		return;
	}
	
	const deoglRenderTaskSharedVAO * const rtvao = vao->GetRTSVAO();
	const deoglModelLOD &modelLod = GetModelLODRef();
	const int count = pComponent.GetTextureCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( modelLod.GetTextureAt( i ).GetFaceCount() == 0 ){
			continue;
		}
		
		const deoglRComponentTexture &texture = pComponent.GetTextureAt( i );
		if( ( texture.GetRenderTaskFilters() & renderTaskFlagMask ) != renderTaskFlags ){
			continue;
		}
		
		deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
		if( ! skinTexture ){
			continue; // actually covered by filter above but better safe than sorry
		}
		
		deoglRenderTaskSharedInstance *rtsi = texture.GetSharedSPBRTIGroup( pLODIndex ).GetRTSInstance();
		
		if( shadow ){
			const deoglSharedSPBRTIGroup * const group = texture.GetSharedSPBRTIGroupShadow( pLODIndex );
			if( group ){
				rtsi = group->GetRTSInstance();
				i += group->GetTextureCount() - 1;
			}
		}
		
		deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType;
		int filterMask = ~0;
		
		if(texture.GetUseDecal()){
			pipelinesType = deoglSkinTexturePipelinesList::eptDecal;
			if(!pComponent.GetSolid()){
				filterMask &= ~ertfSolid;
			}
			
		}else{
			pipelinesType = deoglSkinTexturePipelinesList::eptComponent;
		}
		
		deoglRenderTaskConfigTexture &rct = config.AddTexture();
		rct.SetRenderTaskFilter(texture.GetRenderTaskFilters() & filterMask);
		rct.SetPipeline( skinTexture->GetPipelines().GetAt( pipelinesType ).GetWithRef( type ).GetPipeline() );
		const deoglTexUnitsConfig *tuc = texture.GetTUCForPipelineType( type );
		if( ! tuc ){
			tuc = pComponent.GetRenderThread().GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
		}
		rct.SetTexture( tuc->GetRTSTexture() );
		rct.SetVAO( rtvao );
		rct.SetInstance( rtsi );
		rct.SetGroupIndex( texture.GetSharedSPBElement()->GetIndex() );
	}
}
