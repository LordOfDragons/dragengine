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
#include <stdlib.h>
#include <string.h>

#include "deoglRComponent.h"
#include "deoglRComponentLOD.h"
#include "deoglRComponentTexture.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../gi/deoglGIBVHDynamic.h"
#include "../memory/deoglMemoryManager.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
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
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture1d/deoglTexture1D.h"
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

pVBOData( NULL ),
pVBOPointCount( 0 ),
pVBOPointSize( 0 ),

pVBO( 0 ),
pVAO( NULL ),
pVBOLayout( NULL ),
pVBOBlock( NULL ),

pWeights( NULL ),

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

pMemoryConsumptionGPU( 0 ),

pVBOWeightMatrices( 0 ),
pTBOWeightMatrices( 0 ),
pVBOTransformVertices( 0 ),
pTBOTransformVertices( 0 ),
pTexTransformNormTan( NULL ),
pFBOCalcNormalTangent( NULL ),

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
		
	}else{
		if( ! pVBOBlock ){
			DETHROW( deeInvalidParam );
		}
		return pVBOBlock->GetOffset();
	}
}

int deoglRComponentLOD::GetIndexOffset() const{
	if( pVAO ){
		if( pComponent.GetRenderThread().GetChoices().GetSharedVBOUseBaseVertex() ){
			if( ! pVBOBlock ){
				DETHROW( deeInvalidParam );
			}
			return pVBOBlock->GetIndexOffset();
			
		}else{
			return 0;
		}
		
	}else{
		if( ! pVBOBlock ){
			DETHROW( deeInvalidParam );
		}
		return pVBOBlock->GetIndexOffset();
	}
}

deoglVAO *deoglRComponentLOD::GetUseVAO() const{
	return pVAO ? pVAO : GetModelLODRef().GetVBOBlock()->GetVBO()->GetVAO();
}

void deoglRComponentLOD::InvalidateVAO(){
	pDirtyVAO = true;
	pVBOBlock = NULL;
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
				UpdateVBOOnGPUAccurate();
				break;
				
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
	if( pVBO ){
		deoglMemoryConsumptionVBO &consumption = pComponent.GetRenderThread().GetMemoryManager().GetConsumption().GetVBO();
		
		consumption.DecrementGPU( pMemoryConsumptionGPU );
		consumption.DecrementCount();
		pMemoryConsumptionGPU = 0;
		
		pglDeleteBuffers( 1, &pVBO );
		pVBO = 0;
	}
	
	if( pVBOData ){
		delete [] pVBOData;
		pVBOData = NULL;
		pVBOPointCount = 0;
		pVBOPointSize = 0;
	}
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



void deoglRComponentLOD::UpdateVBOOnGPUAccurate(){
	PrepareWeights();
	
	WriteWeightMatricesTBO();
	GPUTransformVertices();
	GPUCalcNormalTangents();
	GPUWriteRenderVBO();
	
	#ifdef SPECIAL_DEBUG_ON
	extDebugCompCount++;
	extDebugCompTBO += specialTimer.GetElapsedTime();
	#endif
	
// 	#ifdef SPECIAL_DEBUG_ON
// 	specialTimer.Reset();
// 	#endif
// 	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
// 	pBuildVBO( modelLOD );
// 	#ifdef DO_TIMING
// 	elapsedCompBuildVBO += timer.GetElapsedTime();
// 	#endif
// 	#ifdef SPECIAL_DEBUG_ON
// 	extDebugCompBuildVBO += specialTimer.GetElapsedTime();
// 	#endif
}

void deoglRComponentLOD::WriteWeightMatricesTBO(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int weightsCount = modelLOD.GetWeightsCount();
	
	if( weightsCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	
	PrepareWeights();
	
	if( ! pVBOWeightMatrices ){
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOWeightMatrices ) );
		if( ! pVBOWeightMatrices ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOWeightMatrices ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( oglMatrix3x4 ) * weightsCount, pWeights, GL_STATIC_DRAW ) );
	
	if( ! pTBOWeightMatrices ){
		deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
		
		OGL_CHECK( renderThread, glGenTextures( 1, &pTBOWeightMatrices ) );
		if( ! pTBOWeightMatrices ){
			DETHROW( deeInvalidParam );
		}
		
		tsmgr.EnableBareTBO( 0, pTBOWeightMatrices );
		OGL_CHECK( renderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32F, pVBOWeightMatrices ) );
		tsmgr.DisableStage( 0 );
	}
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
}

void deoglRComponentLOD::GPUTransformVertices(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int positionCount = modelLOD.GetPositionCount();
	
	if( positionCount > 0 ){
		deoglRenderThread &renderThread = pComponent.GetRenderThread();
		
		deoglSharedVBOBlock &vboBlock = *modelLOD.GetVBOBlockPositionWeight();
		vboBlock.Prepare();
		
		if( ! pVBOTransformVertices ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOTransformVertices ) );
			if( ! pVBOTransformVertices ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOTransformVertices ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( oglVector3 ) * positionCount, NULL, GL_DYNAMIC_DRAW ) );
		
		renderThread.GetRenderers().GetGeometry().TransformPositions( *vboBlock.GetVBO()->GetVAO(),
			pTBOWeightMatrices, pVBOTransformVertices, vboBlock.GetOffset(), positionCount );
		
		// verify
		/*
		PreparePositions();
		oglVector3 * const temp = new oglVector3[ positionCount ];
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOTransformVertices ) );
		OGL_CHECK( renderThread, pglGetBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( oglVector3 ) * positionCount, temp ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		int i;
		for( i=0; i<positionCount; i++ ){
			pPositions[ i ].x = temp[ i ].x;
			pPositions[ i ].y = temp[ i ].y;
			pPositions[ i ].z = temp[ i ].z;
		}
		delete [] temp;
		*/
	}
}

void deoglRComponentLOD::GPUCalcNormalTangents(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int positionCount = modelLOD.GetPositionCount();
	
	if( positionCount > 0 ){
		deoglRenderThread &renderThread = pComponent.GetRenderThread();
		
		const int positionCount = modelLOD.GetPositionCount();
		const int tangentCount = modelLOD.GetTangentCount();
		const int normalCount = modelLOD.GetNormalCount();
		const int norTanCount = positionCount + normalCount + tangentCount;
		const int faceCount = modelLOD.GetFaceCount();
		
		const bool useFP32 = true; // false to use 16FP
		
		if( ! pTBOTransformVertices ){
			deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
			
			OGL_CHECK( renderThread, glGenTextures( 1, &pTBOTransformVertices ) );
			if( ! pTBOTransformVertices ){
				DETHROW( deeInvalidParam );
			}
			
			tsmgr.EnableBareTBO( 0, pTBOTransformVertices );
			OGL_CHECK( renderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGB32F, pVBOTransformVertices ) );
			tsmgr.DisableStage( 0 );
		}
		
		if( ! pTexTransformNormTan ){
			pTexTransformNormTan = new deoglTexture( pComponent.GetRenderThread() );
			pTexTransformNormTan->SetMipMapped( false );
			
			if( useFP32 ){
				pTexTransformNormTan->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB32F );
				
			}else{
				pTexTransformNormTan->SetFBOFormat( 3, true );
			}
			
			pTexTransformNormTan->CreateTexture();
		}
		
		if( norTanCount > pTexTransformNormTan->GetWidth() * pTexTransformNormTan->GetHeight() ){
			//ogl.LogInfoFormat( "pTexTransformNormTan nor=%i tan=%i tot=%i", normalCount, tangentCount, norTanCount );
			pTexTransformNormTan->SetSize( 256, ( ( norTanCount - 1 ) / 256 ) + 1 );
			pTexTransformNormTan->CreateTexture();
		}
		
		if( ! pFBOCalcNormalTangent ){
			pFBOCalcNormalTangent = new deoglFramebuffer( pComponent.GetRenderThread(), false );
			renderThread.GetFramebuffer().Activate( pFBOCalcNormalTangent );
			pFBOCalcNormalTangent->AttachColorTexture( 0, pTexTransformNormTan );
			const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
			pFBOCalcNormalTangent->Verify();
		}
		
		deoglSharedVBOBlock &vboBlock = *modelLOD.GetVBOBlockCalcNormalTangent();
		vboBlock.Prepare();
		
		renderThread.GetRenderers().GetGeometry().CalcNormalsTangents( *vboBlock.GetVBO()->GetVAO(),
			pTBOTransformVertices, pFBOCalcNormalTangent, pTexTransformNormTan->GetWidth(),
			pTexTransformNormTan->GetHeight(), positionCount, normalCount, tangentCount,
			vboBlock.GetOffset(), faceCount );
		
		renderThread.GetFramebuffer().Activate( NULL ); // otherwise we render with an FBO with a texture attached used also to sample
		
		// verify
		/*
		PrepareNormalsTangents();
		oglVector3 * const temp = new oglVector3[ pTexTransformNormTan->GetWidth() * pTexTransformNormTan->pHeight ];
		renderThread.GetTexture().GetStages().EnableBareTexture( 0, *pTexTransformNormTan );
		OGL_CHECK( renderThread, glGet_Tex_Image( GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, ( GLvoid* )temp ) ); // use texture.GetPixels()
		oglVector3 * const tempNormal = temp;
		oglVector3 * const tempTangent = temp + normalCount;
		int i;
		for( i=0; i<normalCount; i++ ){
			if( tempNormal[ i ].x == 0.0f && tempNormal[ i ].y == 0.0f && tempNormal[ i ].z == 0.0f ){
				tempNormal[ i ].x = 0.0f;
				tempNormal[ i ].y = 1.0f;
				tempNormal[ i ].z = 0.0f;
			}
		}
		for( i=0; i<tangentCount; i++ ){
			if( tempTangent[ i ].x == 0.0f && tempTangent[ i ].y == 0.0f && tempTangent[ i ].z == 0.0f ){
				tempTangent[ i ].x = 1.0f;
				tempTangent[ i ].y = 0.0f;
				tempTangent[ i ].z = 0.0f;
			}
		}
		
		int errorNormals = 0, errorTangents = 0;
		const float fperr = 0.01f; // with FP16 typically <1 degrees, in bad cases <4 degrees and in very worse cases higher
		for( i=0; i<normalCount; i++ ){
			const decVector tempNor1 = decVector( tempNormal[i].x, tempNormal[i].y, tempNormal[i].z ).Normalized();
			const decVector nor1 = decVector( pNormals[i].x, pNormals[i].y, pNormals[i].z ).Normalized();
			if( ! tempNor1.IsEqualTo( nor1, fperr ) ){
				//renderThread.GetLogger().LogInfoFormat( "normal %.2i: (%+f,%+f,%+f) (%+f,%+f,%+f) [%f|%f]\n", i, tempNormal[i].x, tempNormal[i].y, tempNormal[i].z, pNormals[i].x, pNormals[i].y, pNormals[i].z, (tempNor1-nor1).Length(), asinf((tempNor1-nor1).Length())/DEG2RAD );
				errorNormals++;
			}
		}
		for( i=0; i<tangentCount; i++ ){
			const decVector tempTan1 = decVector( tempTangent[i].x, tempTangent[i].y, tempTangent[i].z ).Normalized();
			const decVector tan1 = decVector( pTangents[i].x, pTangents[i].y, pTangents[i].z ).Normalized();
			if( ! tempTan1.IsEqualTo( tan1, fperr ) ){
				//renderThread.GetLogger().LogInfoFormat( "tangent %.2i: (%+f,%+f,%+f) (%+f,%+f,%+f) [%f|%f]\n", i, tempTangent[i].x, tempTangent[i].y, tempTangent[i].z, pTangents[i].x, pTangents[i].y, pTangents[i].z, (tempTan1-tan1).Length(), asin((tempTan1-tan1).Length())/DEG2RAD );
				errorTangents++;
			}
		}
		
		delete [] temp;
		renderThread.GetLogger().LogInfoFormat( "errorNormals=%i(%i) errorTangents=%i(%i)\n", errorNormals, normalCount, errorTangents, tangentCount );
		*/
	}
}

void deoglRComponentLOD::GPUWriteRenderVBO(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int positionCount = modelLOD.GetPositionCount();
	
	if( positionCount > 0 ){
		deoglRenderThread &renderThread = pComponent.GetRenderThread();
		const int normalCount = modelLOD.GetNormalCount();
		const int pointCount = modelLOD.GetVertexCount();
		
		deoglSharedVBOBlock &vboBlock = *modelLOD.GetVBOBlockWriteSkinnedVBO();
		vboBlock.Prepare();
		
		if( ! pVBO ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBO ) );
			if( ! pVBO ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
			pVBOLayout->GetStride() * pointCount, NULL, GL_DYNAMIC_DRAW ) );
		
		renderThread.GetRenderers().GetGeometry().WriteSkinnedVBO( *vboBlock.GetVBO()->GetVAO(),
			pTBOTransformVertices, *pTexTransformNormTan, pVBO, positionCount, normalCount,
			vboBlock.GetOffset(), pointCount );
		
		// verify
		/*
		PrepareNormalsTangents():
		struct sHackTest{ GLfloat x, y, z, nx, ny, nz, tx, ty, tz, tw; };
		const oglModelVertex * const points = modelLOD.GetVertices();
		const bool * const negateTangents = modelLOD.GetNegateTangents();
		sHackTest * const temp = new sHackTest[ pointCount ];
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		OGL_CHECK( renderThread, pglGetBufferSubData( GL_ARRAY_BUFFER, 0, pVBOLayout->GetStride() * pointCount, temp ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		
		int i;
		int errorPosition = 0, errorNormals = 0, errorTangents = 0, errorNegTans = 0;
		const float poserr = 0.00001f;
		const float fperr = 0.01f; // with FP16 typically <1 degrees, in bad cases <4 degrees and in very worse cases higher
		for( i=0; i<pointCount; i++ ){
			const oglModelVertex &point = points[ i ];
			const oglVector &position = pPositions[ point.position ];
			const oglVector &normal = pNormals[ point.normal ];
			const oglVector &tangent = pTangents[ point.tangent ];
			const float corrNegTan = ( negateTangents[ point.tangent ] ? -1.0f : 1.0f );
			
			const decVector testPos( temp[i].x, temp[i].y, temp[i].z );
			const decVector corrPos( position.x, position.y, position.z );
			if( ( testPos - corrPos ).Length() > poserr ){
				errorPosition++;
			}
			
			const decVector testNor = decVector( temp[i].nx, temp[i].ny, temp[i].nz ).Normalized();
			const decVector corrNor = decVector( normal.x, normal.y, normal.z ).Normalized();
			if( ! testNor.IsEqualTo( corrNor, fperr ) ){
				errorNormals++;
			}
			
			const decVector testTan = decVector( temp[i].tx, temp[i].ty, temp[i].tz ).Normalized();
			const decVector corrTan = decVector( tangent.x, tangent.y, tangent.z ).Normalized();
			if( ! testTan.IsEqualTo( corrTan, fperr ) ){
				//renderThread.GetLogger().LogInfoFormat( "tangent %i: (%f,%f,%f) (%f,%f,%f) %f\n", i, corrTan.x, corrTan.y, corrTan.z, testTan.x, testTan.y, testTan.z, (testTan-corrTan).Length() );
				errorTangents++;
			}
			
			if( fabsf( temp[i].tw - corrNegTan ) > 1e-5f ){
				errorNegTans++;
			}
		}
		
		delete [] temp;
		renderThread.GetLogger().LogInfoFormat( "errorPosition=%i(%i) errorNormals=%i errorTangents=%i errorNegTans=%i\n",
			errorPosition, pointCount, errorNormals, errorTangents, errorNegTans );
		*/
	}
}



void deoglRComponentLOD::UpdateVBOOnGPUApproximate(){
	PrepareWeights();
	
	WriteWeightMatricesTBO();
	GPUApproxTransformVNT();
	
	#ifdef SPECIAL_DEBUG_ON
	extDebugCompCount++;
	extDebugCompTBO += specialTimer.GetElapsedTime();
	#endif
}

void deoglRComponentLOD::GPUApproxTransformVNT(){
	deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
	const int pointCount = modelLOD.GetVertexCount();
	
	if( pointCount > 0 ){
		deoglRenderThread &renderThread = pComponent.GetRenderThread();
		
		deoglSharedVBOBlock &vboBlock = *modelLOD.GetVBOBlockWithWeight();
		vboBlock.Prepare();
		
		if( ! pVBO ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBO ) );
			if( ! pVBO ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
			pVBOLayout->GetStride() * pointCount, NULL, GL_DYNAMIC_DRAW ) );
		
		renderThread.GetRenderers().GetGeometry().ApproxTransformVNT( *vboBlock.GetVBO()->GetVAO(),
			pTBOWeightMatrices, pVBO, vboBlock.GetOffset(), pointCount );
		
		// verify
		/*
		PrepareNormalsTangents():
		struct sHackTest{ GLfloat x, y, z, nx, ny, nz, tx, ty, tz, tw; };
		const oglModelVertex * const points = modelLOD.GetVertices();
		const bool * const negateTangents = modelLOD.GetNegateTangents();
		sHackTest * const temp = new sHackTest[ pointCount ];
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		OGL_CHECK( renderThread, pglGetBufferSubData( GL_ARRAY_BUFFER, 0, pVBOLayout->GetStride() * pointCount, temp ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		
		int i;
		int errorPosition = 0, errorNormals = 0, errorTangents = 0, errorNegTans = 0;
		const float poserr = 0.00001f;
		const float fperr = 0.01f; // with FP16 typically <1 degrees, in bad cases <4 degrees and in very worse cases higher
		for( i=0; i<pointCount; i++ ){
			const oglModelVertex &point = points[ i ];
			const oglVector &position = pPositions[ point.position ];
			const oglVector &normal = pNormals[ point.normal ];
			const oglVector &tangent = pTangents[ point.tangent ];
			const float corrNegTan = ( negateTangents[ point.tangent ] ? -1.0f : 1.0f );
			
			const decVector testPos( temp[i].x, temp[i].y, temp[i].z );
			const decVector corrPos( position.x, position.y, position.z );
			if( ( testPos - corrPos ).Length() > poserr ){
				errorPosition++;
			}
			
			const decVector testNor = decVector( temp[i].nx, temp[i].ny, temp[i].nz ).Normalized();
			const decVector corrNor = decVector( normal.x, normal.y, normal.z ).Normalized();
			if( ! testNor.IsEqualTo( corrNor, fperr ) ){
				errorNormals++;
			}
			
			const decVector testTan = decVector( temp[i].tx, temp[i].ty, temp[i].tz ).Normalized();
			const decVector corrTan = decVector( tangent.x, tangent.y, tangent.z ).Normalized();
			if( ! testTan.IsEqualTo( corrTan, fperr ) ){
				//renderThread.GetLogger().LogInfoFormat( "tangent %i: (%f,%f,%f) (%f,%f,%f) %f\n", i, corrTan.x, corrTan.y, corrTan.z, testTan.x, testTan.y, testTan.z, (testTan-corrTan).Length() );
				errorTangents++;
			}
			
			if( fabsf( temp[i].tw - corrNegTan ) > 1e-5f ){
				errorNegTans++;
			}
		}
		
		delete [] temp;
		renderThread.GetLogger().LogInfoFormat( "errorPosition=%i(%i) errorNormals=%i errorTangents=%i errorNegTans=%i\n",
			errorPosition, pointCount, errorNormals, errorTangents, errorNegTans );
		*/
	}
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



const deoglRenderTaskConfig *deoglRComponentLOD::GetRenderTaskConfig( deoglSkinTexture::eShaderTypes type ) const{
	switch( type ){
	case deoglSkinTexture::estComponentShadowProjection:
		return &pRenderTaskConfigs[ 0 ];
		
	case deoglSkinTexture::estComponentShadowOrthogonal:
		return &pRenderTaskConfigs[ 1 ];
		
	case deoglSkinTexture::estComponentShadowOrthogonalCascaded:
		return &pRenderTaskConfigs[ 2 ];
		
	case deoglSkinTexture::estComponentShadowDistance:
		return &pRenderTaskConfigs[ 3 ];
		
	case deoglSkinTexture::estComponentShadowDistanceCube:
		return &pRenderTaskConfigs[ 4 ];
		
	default:
		return NULL;
	}
}

void deoglRComponentLOD::UpdateRenderTaskConfigurations(){
	const deoglSkinTexture::eShaderTypes typesShadow[ 5 ] = {
		deoglSkinTexture::estComponentShadowProjection,
		deoglSkinTexture::estComponentShadowOrthogonal,
		deoglSkinTexture::estComponentShadowOrthogonalCascaded,
		deoglSkinTexture::estComponentShadowDistance,
		deoglSkinTexture::estComponentShadowDistanceCube };
	int i;
	
	for( i=0; i<5; i++ ){
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
		
		int j;
		for( j=0; j<5; j++ ){
			deoglRenderTaskConfigTexture &rct = pRenderTaskConfigs[ j ].AddTexture();
			rct.SetRenderTaskFilter( texture.GetRenderTaskFilters() );
			rct.SetShader( skinTexture->GetShaderFor( typesShadow[ j ] )->GetShader()->GetRTSShader() );
			const deoglTexUnitsConfig *tuc = texture.GetTUCForShaderType( typesShadow[ j ] );
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
	return pComponent.GetModelRef().GetLODAt( pLODIndex );
}

void deoglRComponentLOD::PrepareWeights(){
	if( pDirtyModelWeights ){
		oglMatrix3x4 *weights = NULL;
		
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
		oglVector *positions = NULL;
		
		if( pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			const int positionCount = modelLOD.GetPositionCount();
			if( positionCount > 0 ){
				positions = new oglVector[ positionCount ];
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
		oglVector *realNormals = NULL;
		oglVector *normals = NULL;
		oglVector *tangents = NULL;
		oglVector *faceNormals = NULL;
		
		if( pComponent.GetModel() && pLODIndex >= 0 && pLODIndex < pComponent.GetModel()->GetLODCount() ){
			deoglModelLOD &modelLOD = pComponent.GetModel()->GetLODAt( pLODIndex );
			
			const int positionCount = modelLOD.GetPositionCount();
			const int normalCount = modelLOD.GetNormalCount();
			const int tangentCount = modelLOD.GetTangentCount();
			const int faceCount = modelLOD.GetFaceCount();
			
			try{
				if( positionCount > 0 ){
					realNormals = new oglVector[ positionCount ];
				}
				if( normalCount > 0 ){
					normals = new oglVector[ normalCount ];
				}
				if( tangentCount > 0 ){
					tangents = new oglVector[ tangentCount ];
				}
				if( faceCount > 0 ){
					faceNormals = new oglVector[ faceCount ];
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

class deoglRComponentLODDeletion : public deoglDelayedDeletion{
public:
	GLuint vbo;
	deoglVAO *vao;
	GLuint vboWeightMatrices;
	GLuint tboWeightMatrices;
	GLuint vboTransformVertices;
	GLuint tboTransformVertices;
	deoglTexture *texTransformNormTan;
	deoglFramebuffer *fboCalcNormalTangent;
	
	deoglRComponentLODDeletion() :
	vbo( 0 ),
	vao( NULL ),
	vboWeightMatrices( 0 ),
	tboWeightMatrices( 0 ),
	vboTransformVertices( 0 ),
	tboTransformVertices( 0 ),
	texTransformNormTan( NULL ),
	fboCalcNormalTangent( NULL ){
	}
	
	virtual ~deoglRComponentLODDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		if( tboWeightMatrices ){
			OGL_CHECK( renderThread, glDeleteTextures( 1, &tboWeightMatrices ) );
		}
		if( vboWeightMatrices ){
			OGL_CHECK( renderThread, pglDeleteBuffers( 1, &vboWeightMatrices ) );
		}
		if( tboTransformVertices ){
			OGL_CHECK( renderThread, pglDeleteBuffers( 1, &tboTransformVertices ) );
		}
		if( vboTransformVertices ){
			OGL_CHECK( renderThread, pglDeleteBuffers( 1, &vboTransformVertices ) );
		}
		if( texTransformNormTan ){
			delete texTransformNormTan;
		}
		if( fboCalcNormalTangent ){
			delete fboCalcNormalTangent;
		}
		if( vao ){
			delete vao;
		}
		if( vbo ){
			pglDeleteBuffers( 1, &vbo );
		}
	}
};

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
	
	if( pVBO ){
		deoglMemoryConsumptionVBO &consumption = pComponent.GetRenderThread().GetMemoryManager().GetConsumption().GetVBO();
		
		consumption.DecrementGPU( pMemoryConsumptionGPU );
		consumption.DecrementCount();
		pMemoryConsumptionGPU = 0;
	}
	if( pVBOData ){
		delete [] pVBOData;
		pVBOData = NULL;
		pVBOPointCount = 0;
		pVBOPointSize = 0;
	}
	
	// delayed deletion of opengl containing objects
	deoglRComponentLODDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRComponentLODDeletion;
		delayedDeletion->fboCalcNormalTangent = pFBOCalcNormalTangent;
		delayedDeletion->tboTransformVertices = pTBOTransformVertices;
		delayedDeletion->tboWeightMatrices = pTBOWeightMatrices;
		delayedDeletion->texTransformNormTan = pTexTransformNormTan;
		delayedDeletion->vao = pVAO;
		delayedDeletion->vbo = pVBO;
		delayedDeletion->vboTransformVertices = pVBOTransformVertices;
		delayedDeletion->vboWeightMatrices = pVBOWeightMatrices;
		pComponent.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pComponent.GetRenderThread().GetLogger().LogException( e );
		throw;
	}
}



void deoglRComponentLOD::pBuildVBO( const deoglModelLOD &modelLOD ){
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	deoglMemoryConsumptionVBO &consumption = renderThread.GetMemoryManager().GetConsumption().GetVBO();
	const int pointCount = modelLOD.GetVertexCount();
	deoglVBOpnt *newArray = NULL;
	int dataSize;
	
	// dynamic mesh vbo
	dataSize = pVBOLayout->GetStride() * pointCount;
	
	// create vbo if not existing already and bind it
	if( ! pVBO ){
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
		consumption.IncrementCount();
	}
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
	
	// build data array
	if( pointCount > pVBOPointSize ){
		newArray = new deoglVBOpnt[ pointCount ];
		if( pVBOData ){
			delete [] pVBOData;
		}
		pVBOData = newArray;
		pVBOPointSize = pointCount;
		
		// enlarge vbo
		consumption.DecrementGPU( pMemoryConsumptionGPU );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW ) );
		pMemoryConsumptionGPU = dataSize;
		consumption.IncrementGPU( pMemoryConsumptionGPU );
	}
	
	// write data to buffer
	if( pPositions && pRealNormals && pNormals && pTangents ){
		pWriteVBOData( modelLOD );
	}
	
	// update vbo
	//OGL_CHECK( ogl, pglBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, pVBOData ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, pVBOData, GL_STREAM_DRAW ) );
	
	// store number of points written
	pVBOPointCount = pointCount;
}

void deoglRComponentLOD::pWriteVBOData( const deoglModelLOD &modelLOD ){
	const bool * const negateTangents = modelLOD.GetNegateTangents();
	const oglModelVertex * const points = modelLOD.GetVertices();
	const int pointCount = modelLOD.GetVertexCount();
	deoglVBOpnt *data = pVBOData;
	//int converted;
	int i;
	
	for( i=0; i<pointCount; i++ ){
		if( i >= pVBOPointSize ){
			DETHROW( deeInvalidParam );
		}
		
		const oglModelVertex &point = points[ i ];
		
		// position
		const oglVector &position = pPositions[ point.position ];
		data->x = position.x;
		data->y = position.y;
		data->z = position.z;
		
		// real normal
		const oglVector &realNormal = pRealNormals[ point.position ];
		data->rnx = realNormal.x;
		data->rny = realNormal.y;
		data->rnz = realNormal.z;
		
		// normal
		const oglVector &normal = pNormals[ point.normal ];
		data->nx = normal.x;
		data->ny = normal.y;
		data->nz = normal.z;
#if 0
		}else{
			converted = ( int )( normal.x * 32767.0f );
			if( converted > 32767 ){
				data->nx = ( GLshort )32767;
				
			}else if( converted < -32768 ){
				data->nx = ( GLshort )-32768;
				
			}else{
				data->nx = ( GLshort )converted;
			}
			
			converted = ( int )( normal.y * 32767.0f );
			if( converted > 32767 ){
				data->ny = ( GLshort )32767;
				
			}else if( converted < -32768 ){
				data->ny = ( GLshort )-32768;
				
			}else{
				data->ny = ( GLshort )converted;
			}
			
			converted = ( int )( normal.z * 32767.0f );
			if( converted > 32767 ){
				data->nz = ( GLshort )32767;
				
			}else if( converted < -32768 ){
				data->nz = ( GLshort )-32768;
				
			}else{
				data->nz = ( GLshort )converted;
			}
		}
#endif
		
		// tangent
		const oglVector &tangent = pTangents[ point.tangent ];
		data->tx = tangent.x;
		data->ty = tangent.y;
		data->tz = tangent.z;
		
		if( negateTangents[ point.tangent ] ){
			data->tw = -1.0f;
			
		}else{
			data->tw = 1.0f;
		}
#if 0
		}else{
			converted = ( int )( tangent.x * 32767.0f );
			if( converted > 32767 ){
				data->tx = ( GLshort )32767;
				
			}else if( converted < -32768 ){
				data->tx = ( GLshort )-32768;
				
			}else{
				data->tx = ( GLshort )converted;
			}
			
			converted = ( int )( tangent.y * 32767.0f );
			if( converted > 32767 ){
				data->ty = ( GLshort )32767;
				
			}else if( converted < -32768 ){
				data->ty = ( GLshort )-32768;
				
			}else{
				data->ty = ( GLshort )converted;
			}
			
			converted = ( int )( tangent.z * 32767.0f );
			if( converted > 32767 ){
				data->tz = ( GLshort )32767;
				
			}else if( converted < -32768 ){
				data->tz = ( GLshort )-32768;
				
			}else{
				data->tz = ( GLshort )converted;
			}
		}
#endif
		
		data++;
	}
}

void deoglRComponentLOD::pUpdateVAO( deoglModelLOD &modelLOD ){
	if( pVAO || ! pVBO ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	pVBOBlock = modelLOD.GetVBOBlock();
	const GLuint vboModel = pVBOBlock->GetVBO()->GetVBO();
	const deoglVBOLayout &vboLayout = pVBOBlock->GetVBO()->GetParentList()->GetLayout();
	
	pVAO = new deoglVAO( renderThread );
	OGL_CHECK( renderThread, pglBindVertexArray( pVAO->GetVAO() ) );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
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
				const oglMatrix3x4 &boneMatrix = boneMatrices[ weightsEntries->bone ];
				factor = weightsEntries->weight;
				
				weightsMatrix.a11 += boneMatrix.a11 * factor;
				weightsMatrix.a12 += boneMatrix.a12 * factor;
				weightsMatrix.a13 += boneMatrix.a13 * factor;
				weightsMatrix.a14 += boneMatrix.a14 * factor;
				weightsMatrix.a21 += boneMatrix.a21 * factor;
				weightsMatrix.a22 += boneMatrix.a22 * factor;
				weightsMatrix.a23 += boneMatrix.a23 * factor;
				weightsMatrix.a24 += boneMatrix.a24 * factor;
				weightsMatrix.a31 += boneMatrix.a31 * factor;
				weightsMatrix.a32 += boneMatrix.a32 * factor;
				weightsMatrix.a33 += boneMatrix.a33 * factor;
				weightsMatrix.a34 += boneMatrix.a34 * factor;
				
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
			oglVector &trpos = pPositions[ i ];
			trpos.x = orgpos.x;
			trpos.y = orgpos.y;
			trpos.z = orgpos.z;
		}
		return;
	}
	
	for( i=0; i<positionCount; i++ ){
		const oglModelPosition &modelPosition = positions[ i ];
		const decVector &orgpos = positions[ i ].position;
		oglVector &trpos = pPositions[ i ];
		
		if( modelPosition.weight == -1 ){
			trpos.x = orgpos.x;
			trpos.y = orgpos.y;
			trpos.z = orgpos.z;
			
		}else{
			const oglMatrix3x4 &matrix = pWeights[ modelPosition.weight ];
			
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
	oglVector edge1, edge2;
	oglVector tangent;
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
		const oglVector &position1 = pPositions[ point1.position ];
		const oglVector &position2 = pPositions[ point2.position ];
		const oglVector &position3 = pPositions[ point3.position ];
		const decVector2 &texcoord1 = texcoords[ point1.texcoord ];
		const decVector2 &texcoord2 = texcoords[ point2.texcoord ];
		const decVector2 &texcoord3 = texcoords[ point3.texcoord ];
		
		oglVector &faceNormal = pFaceNormals[ i ];
		
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
		oglVector &vrn1 = pRealNormals[ point1.position ];
		vrn1.x += faceNormal.x;
		vrn1.y += faceNormal.y;
		vrn1.z += faceNormal.z;
		
		oglVector &vrn2 = pRealNormals[ point2.position ];
		vrn2.x += faceNormal.x;
		vrn2.y += faceNormal.y;
		vrn2.z += faceNormal.z;
		
		oglVector &vrn3 = pRealNormals[ point3.position ];
		vrn3.x += faceNormal.x;
		vrn3.y += faceNormal.y;
		vrn3.z += faceNormal.z;
		
		// add to normals
		oglVector &vn1 = pNormals[ point1.normal ];
		vn1.x += faceNormal.x;
		vn1.y += faceNormal.y;
		vn1.z += faceNormal.z;
		
		oglVector &vn2 = pNormals[ point2.normal ];
		vn2.x += faceNormal.x;
		vn2.y += faceNormal.y;
		vn2.z += faceNormal.z;
		
		oglVector &vn3 = pNormals[ point3.normal ];
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
		oglVector &vt1 = pTangents[ point1.tangent ];
		vt1.x += tangent.x;
		vt1.y += tangent.y;
		vt1.z += tangent.z;
		
		oglVector &vt2 = pTangents[ point2.tangent ];
		vt2.x += tangent.x;
		vt2.y += tangent.y;
		vt2.z += tangent.z;
		
		oglVector &vt3 = pTangents[ point3.tangent ];
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
	
	// for the time being we use a very simple layout without optimizations.
	// later on certain attributes will be optimized if their values fall
	// into certain categories. this allows to reduce the memory footprint
	// of VBOs in certain cases
	pVBOLayout = new deoglVBOLayout;
 	
 	// the layout of attributes is the basic geometry attributes. Here
 	// the data map for the attributes:
 	//
	// name        | offset | type  | components
	// ------------+--------+-------+------------
	// position    |      0 | float | x, y, z
	// real-normal |     12 | float | x, y, z
	// normal      |     24 | float | x, y, z
	// tangent     |     36 | float | x, y, z, w
	pVBOLayout->SetAttributeCount( 4 );
	pVBOLayout->SetStride( 52 ); // best performance with multiple of 32/64?
 	pVBOLayout->SetSize( pVBOLayout->GetStride() * modelLOD.GetVertexCount() );
	pVBOLayout->SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	deoglVBOAttribute &attrPos = pVBOLayout->GetAttributeAt( 0 );
	attrPos.SetComponentCount( 3 );
	attrPos.SetDataType( deoglVBOAttribute::edtFloat );
	attrPos.SetOffset( 0 );
	
	deoglVBOAttribute &attrRealNormal = pVBOLayout->GetAttributeAt( 1 );
	attrRealNormal.SetComponentCount( 3 );
	attrRealNormal.SetDataType( deoglVBOAttribute::edtFloat );
	attrRealNormal.SetOffset( 12 );
	
	deoglVBOAttribute &attrNormal = pVBOLayout->GetAttributeAt( 2 );
	attrNormal.SetComponentCount( 3 );
	attrNormal.SetDataType( deoglVBOAttribute::edtFloat );
	attrNormal.SetOffset( 24 );
	
	deoglVBOAttribute &attrTangent = pVBOLayout->GetAttributeAt( 3 );
	attrTangent.SetComponentCount( 4 );
	attrTangent.SetDataType( deoglVBOAttribute::edtFloat );
	attrTangent.SetOffset( 36 );
}

void deoglRComponentLOD::pUpdateRenderTaskConfig( deoglRenderTaskConfig &config,
deoglSkinTexture::eShaderTypes type, int renderTaskFlags, int renderTaskFlagMask, bool shadow ){
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
		
		const deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
		if( ! skinTexture ){
			continue; // actually covered by filter above but better safe than sorry
		}
		
		deoglRenderTaskSharedInstance *rtsi = texture.GetSharedSPBRTIGroup( pLODIndex ).GetRTSInstance();
		
		if( shadow ){
			deoglSharedSPBRTIGroup *group = texture.GetSharedSPBRTIGroupShadow( pLODIndex );
			if( group ){
				rtsi = group->GetRTSInstance();
				i += group->GetTextureCount() - 1;
			}
		}
		
		deoglRenderTaskConfigTexture &rct = config.AddTexture();
		rct.SetRenderTaskFilter( texture.GetRenderTaskFilters() );
		rct.SetShader( skinTexture->GetShaderFor( type )->GetShader()->GetRTSShader() );
		const deoglTexUnitsConfig *tuc = texture.GetTUCForShaderType( type );
		if( ! tuc ){
			tuc = pComponent.GetRenderThread().GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
		}
		rct.SetTexture( tuc->GetRTSTexture() );
		rct.SetVAO( rtvao );
		rct.SetInstance( rtsi );
		rct.SetGroupIndex( texture.GetSharedSPBElement()->GetIndex() );
	}
}
