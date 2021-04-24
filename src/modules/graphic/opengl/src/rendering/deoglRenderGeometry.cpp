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

#include "deoglRenderGeometry.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskInstance.h"
#include "task/deoglRenderTaskShader.h"
#include "task/deoglRenderTaskTexture.h"
#include "task/deoglRenderTaskVAO.h"
#include "task/persistent/deoglPersistentRenderTask.h"
#include "task/persistent/deoglPersistentRenderTaskInstance.h"
#include "task/persistent/deoglPersistentRenderTaskShader.h"
#include "task/persistent/deoglPersistentRenderTaskTexture.h"
#include "task/persistent/deoglPersistentRenderTaskVAO.h"

#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../component/deoglComponentLOD.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../extensions/deoglExtensions.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../light/deoglRLight.h"
#include "../light/volume/deoglLightVolume.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeManager.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include "../utils/collision/deoglDCollisionFrustum.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionVolume.h"



// Definitions
////////////////

#define FACTOR_EXPONENT		255.0f

enum eSPRenderTaskBare{
	sprtbMatrixMVP,
	sprtbMatrixMV
};

enum eSPRenderTaskDepth{
	sprtdMatrixMVP,
	sprtdMatrixMV,
	sprtdMatrixDiffuse,
	sprtdMaterialGamma,
	sprtdClipPlane,
	sprtdViewport
};

enum eSPRenderTaskGeometry{
	sprtgMatrixMVP,
	sprtgMatrixMV,
	sprtgMatrixNor,
	sprtgTexMatrixDiffuse,
	sprtgTexMatrixNormal,
	sprtgTexMatrixEmissive,
	sprtgTexMatrixAO,
	sprtgColor,
	sprtgColorGamma,
	sprtgAmbient,
	sprtgReflectivity,
	sprtgEnvMapParam,
	sprtgRoughness,
	sprtgEmissivity,
	sprtgViewport,
	sprtgDistortion,
	sprtgTemporary,
	sprtgMatrixEnvMap
};

enum eSPRenderTaskEnvMap{
	sprtemMatrixMVP,
	sprtemMatrixNor,
	sprtemTexMatrixDiffuse,
	sprtemTexMatrixEmissive,
	sprtemColor,
	sprtemColorGamma,
	sprtemAmbient,
	sprtemEmissivity,
	
	sprtemSkyLightView,
	sprtemSkyLightColor,
	sprtemSkyAmbientColor
};



// Class deoglRenderGeometry
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderGeometry::deoglRenderGeometry( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
// 	deoglConfiguration &config = ogl->GetConfiguration();
// 	const bool useEncodeDepth = config.GetUseEncodeDepth();
	pShaderParticle = NULL;
	
	pShaderTransformPositions = NULL;
	pShaderCalcNormalsTangents = NULL;
	pShaderWriteSkinnedVBO = NULL;
	
	pShaderApproxTransformVNT = NULL;
	
	#ifndef ANDROID
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
// 		sources = shaderManager.GetSourcesNamed( "DefRen Geometry Particle Old" );
// 		defines.AddDefine( "HAS_MAP_EMISSIVE", "1" );
// 		if( useEncodeDepth ){
// 			defines.AddDefine( "GEOM_ENCODED_DEPTH", "1" );
// 		}
// 		pShaderParticle = shaderManager.GetProgramWith( sources, defines );
// 		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Transform Positions" );
		pShaderTransformPositions = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Calculate Normals Tangents" );
		pShaderCalcNormalsTangents = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Write Skinned VBO" );
		pShaderWriteSkinnedVBO = shaderManager.GetProgramWith( sources, defines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Approx Transform VNT" );
		pShaderApproxTransformVNT = shaderManager.GetProgramWith( sources, defines );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	#endif
}

deoglRenderGeometry::~deoglRenderGeometry(){
	pCleanUp();
}



// Management
///////////////

void deoglRenderGeometry::SetAmbient( const decColor &color ){
	pAmbient = color; // already gamma corrected
}




// Rendering
//////////////

/*
if( uniformSetup == deoglRenderTaskShader::eusEnvMap ){
	const decColor &emissivity = skinTexture.GetEmissivity();
	const float emissivityIntensity = renderTaskTexture->emissivityIntensity;
	float colorGamma = skinTexture.GetColorGamma();
	
	shader.SetParameterColor4( sprtemAmbient, ambient );
	shader.SetParameterFloat( sprtemEmissivity, emissivity.r * emissivityIntensity, emissivity.g * emissivityIntensity, emissivity.b * emissivityIntensity );
	shader.SetParameterFloat( sprtemColorGamma, colorGamma, colorGamma, colorGamma, 1.0f );
	
	// sky light
	decVector skyLightDir = -( decMatrix::CreateRotation( 61.0f * DEG2RAD, 62.0f * DEG2RAD, 0.0f ) ).TransformView();
	const float skyLightIntensity = 16.0f * CANDELA_TO_INTENSITY;
	const float skyAmbientIntensity = 4.0f * CANDELA_TO_INTENSITY;
	
	decColor skyLightColor = decColor( 1.0f, 1.0f, 1.0f );
	skyLightColor.r = powf( skyLightColor.r, OGL_RENDER_GAMMA );
	skyLightColor.g = powf( skyLightColor.g, OGL_RENDER_GAMMA );
	skyLightColor.b = powf( skyLightColor.b, OGL_RENDER_GAMMA );
	
	decVector vector = params.plan->GetWorld()->GetWorld()->GetLightColorMatrix().Transform( skyLightColor.r, skyLightColor.g, skyLightColor.b );
	skyLightColor = decColor( vector.x, vector.y, vector.z );
	
	skyLightDir.Normalize();
	
	shader.SetParameterDVector3( sprtemSkyLightView, -params.matrixCamera.TransformNormal( skyLightDir ).ToVector() );
	shader.SetParameterColor3( sprtemSkyLightColor, skyLightColor * skyLightIntensity );
	shader.SetParameterColor3( sprtemSkyAmbientColor, skyLightColor * skyAmbientIntensity );
}
*/

/*
if( uniformSetup == deoglRenderTaskShader::eusEnvMap ){
	shader.SetParameterDMatrix4x4( sprtemMatrixMVP, matrixMV * params.matrixProjection );
	shader.SetParameterDMatrix3x3( sprtemMatrixNor, matrixMV.GetRotationMatrix().Invert().Transpose() );
	shader.SetParameterColor4( sprtemColor, renderTaskInstance->GetColorTint(), 1.0f );
	
	shader.SetParameterColor3( sprtemEmissivity, renderTaskInstance->GetEmissivity() );
	
	shader.SetParameterTexMatrix3x2( sprtemTexMatrixDiffuse, matrixTC );
	shader.SetParameterTexMatrix3x2( sprtemTexMatrixEmissive, matrixTC );
}
*/

void deoglRenderGeometry::RenderTask( const deoglRenderTask &renderTask ){
	const int renderTaskShaderCount = renderTask.GetShaderCount();
	if( renderTaskShaderCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	deoglShaderParameterBlock *spbSIIndexInstance = NULL;
	bool curDoubleSided = false;
	deoglVAO *curVAO = NULL;
	int i;
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	
	renderThread.GetBufferObject().GetSharedVBOListList().PrepareAllLists(); // needs to be done better
	
	for( i=0; i<renderTaskShaderCount; i++ ){
		const deoglRenderTaskShader &renderTaskShader = *renderTask.GetShaderAt( i );
		deoglShaderCompiled &shader = *renderTaskShader.GetShader()->GetCompiled();
		
		renderThread.GetShader().ActivateShader( renderTaskShader.GetShader() );
		
		if( renderParamBlock ){
			renderParamBlock->Activate();
		}
		
		deoglRenderTaskTexture *renderTaskTexture = renderTaskShader.GetRootTexture();
		
		while( renderTaskTexture ){
			if( renderTaskTexture->GetParameterBlock() ){
				renderTaskTexture->GetParameterBlock()->Activate();
			}
			renderTaskTexture->GetTUC()->Apply();
			
			deoglRenderTaskVAO *renderTaskVAO = renderTaskTexture->GetRootVAO();
			
			while( renderTaskVAO ){
				deoglRenderTaskInstance *renderTaskInstance = renderTaskVAO->GetRootInstance();
				if( ! renderTaskInstance ){
					renderTaskVAO = renderTaskVAO->GetNextVAO();
					continue;
				}
				
				deoglVAO * const vao = renderTaskVAO->GetVAO();
				if( vao != curVAO ){
					pglBindVertexArray( vao->GetVAO() );
					curVAO = vao;
				}
				
				const GLenum indexGLType = vao->GetIndexGLType();
				const int indexSize = vao->GetIndexSize();
				
				while( renderTaskInstance ){
					const bool doubleSided = renderTaskInstance->GetDoubleSided();
					
					if( renderTaskInstance->GetParameterBlock() ){
						renderTaskInstance->GetParameterBlock()->Activate();
					}
					if( renderTaskInstance->GetParameterBlockSpecial() ){
						renderTaskInstance->GetParameterBlockSpecial()->Activate();
					}
					if( renderTaskInstance->GetSubInstanceSPB() ){
						renderTaskInstance->GetSubInstanceSPB()->GetParameterBlock()->Activate();
					}
					
					if( renderTaskInstance->GetSIIndexInstanceSPB() != spbSIIndexInstance ){
						if( renderTaskInstance->GetSIIndexInstanceSPB() ){
							renderTaskInstance->GetSIIndexInstanceSPB()->Activate();
						}
						spbSIIndexInstance = renderTaskInstance->GetSIIndexInstanceSPB();
					}
					
					if( renderTaskShader.GetSPBInstanceIndexBase() != -1 ){
						shader.SetParameterInt( renderTaskShader.GetSPBInstanceIndexBase(),
							renderTaskInstance->GetSIIndexInstanceFirst() );
					}
					
					if( doubleSided != curDoubleSided ){
						if( doubleSided ){
							OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
							
						}else{
							OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
						}
						
						curDoubleSided = doubleSided;
					}
					
					GLenum primitiveType = renderTaskInstance->GetPrimitiveType();
					
					if( pglPatchParameteri && shader.GetHasTessellation() ){
						pglPatchParameteri( GL_PATCH_VERTICES,
							renderTaskInstance->GetTessPatchVertexCount() );
						primitiveType = GL_PATCHES;
					}
					
					if( renderTaskInstance->GetSubInstanceCount() == 0 ){
						if( renderTaskInstance->GetIndexCount() == 0 ){
							OGL_CHECK( renderThread, glDrawArrays( primitiveType,
								renderTaskInstance->GetFirstPoint(),
								renderTaskInstance->GetPointCount() ) );
							
						}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
							// renderTaskInstance->GetFirstPoint() as base-vertex. required since
							// the indices are stored relative to the block of points for various
							// reasons. base-vertex is required to shift the indices to the correct
							// range of points in the vbo. FirstPoint contains already the index
							// to the first point in the block and thus is the right value we
							// need to shift the indices by
							
							OGL_CHECK( renderThread, pglDrawElementsBaseVertex( primitiveType,
								renderTaskInstance->GetIndexCount(), indexGLType,
									( GLvoid* )( intptr_t )(
										indexSize * renderTaskInstance->GetFirstIndex() ),
								renderTaskInstance->GetFirstPoint() ) );
							
						}else{
							OGL_CHECK( renderThread, glDrawElements( primitiveType,
								renderTaskInstance->GetIndexCount(), indexGLType,
								( GLvoid* )( intptr_t )(
									indexSize * renderTaskInstance->GetFirstIndex() ) ) );
						}
						
					}else{
						if( renderTaskInstance->GetIndexCount() == 0 ){
							OGL_CHECK( renderThread, pglDrawArraysInstanced( primitiveType,
								renderTaskInstance->GetFirstPoint(),
								renderTaskInstance->GetPointCount(),
								renderTaskInstance->GetSubInstanceCount() ) );
							
						}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
							OGL_CHECK( renderThread, pglDrawElementsInstancedBaseVertex(
								primitiveType, renderTaskInstance->GetIndexCount(),
								indexGLType, ( GLvoid* )( intptr_t )(
									indexSize * renderTaskInstance->GetFirstIndex() ),
								renderTaskInstance->GetSubInstanceCount(),
								renderTaskInstance->GetFirstPoint() ) );
							
						}else{
							OGL_CHECK( renderThread, pglDrawElementsInstanced(
								primitiveType, renderTaskInstance->GetIndexCount(),
								indexGLType, ( GLvoid* )( intptr_t )(
									indexSize * renderTaskInstance->GetFirstIndex() ),
								renderTaskInstance->GetSubInstanceCount() ) );
						}
					}
					
					renderTaskInstance = renderTaskInstance->GetNextInstance();
				}
				
				renderTaskVAO = renderTaskVAO->GetNextVAO();
			}
			
			renderTaskTexture = renderTaskTexture->GetNextTexture();
		}
		
	}
	
	pglBindVertexArray( 0 );
}

void deoglRenderGeometry::RenderTask( const deoglPersistentRenderTask &renderTask ){
	decPointerLinkedList::cListEntry *iterShader = renderTask.GetRootShader();
	if( ! iterShader ){
		return;
	}
	
	deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	deoglShaderParameterBlock *spbSIIndexInstance = NULL;
	deoglRenderThread &renderThread = GetRenderThread();
	bool curDoubleSided = false;
	deoglVAO *curVAO = NULL;
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	
	renderThread.GetBufferObject().GetSharedVBOListList().PrepareAllLists(); // needs to be done better
	
	while( iterShader ){
		const deoglPersistentRenderTaskShader &rtshader = *( ( deoglPersistentRenderTaskShader* )iterShader->GetOwner() );
		deoglShaderCompiled &shader = *rtshader.GetShader()->GetCompiled();
		
		renderThread.GetShader().ActivateShader( rtshader.GetShader() );
		
		if( renderParamBlock ){
			renderParamBlock->Activate();
		}
		
		decPointerLinkedList::cListEntry *iterTexture = rtshader.GetRootTexture();
		while( iterTexture ){
			deoglPersistentRenderTaskTexture &texture = *( ( deoglPersistentRenderTaskTexture* )iterTexture->GetOwner() );
			
			if( texture.GetParameterBlock() ){
				texture.GetParameterBlock()->Activate();
			}
			texture.GetTUC()->Apply();
			
			decPointerLinkedList::cListEntry *iterVAO = texture.GetRootVAO();
			while( iterVAO ){
				deoglPersistentRenderTaskVAO &rtvao = *( ( deoglPersistentRenderTaskVAO* )iterVAO->GetOwner() );
				decPointerLinkedList::cListEntry *iterInstance = rtvao.GetRootInstance();
				if( ! iterInstance ){
					iterVAO = iterVAO->GetNext();
					continue;
				}
				
				deoglVAO * const vao = rtvao.GetVAO();
				if( vao != curVAO ){
					pglBindVertexArray( vao->GetVAO() );
					curVAO = vao;
				}
				
				const GLenum indexGLType = vao->GetIndexGLType();
				const int indexSize = vao->GetIndexSize();
				
				while( iterInstance ){
					deoglPersistentRenderTaskInstance &instance = *( ( deoglPersistentRenderTaskInstance* )iterInstance->GetOwner() );
					const bool doubleSided = instance.GetDoubleSided();
					
					if( instance.GetParameterBlock() ){
						instance.GetParameterBlock()->Activate();
					}
					if( instance.GetParameterBlockSpecial() ){
						instance.GetParameterBlockSpecial()->Activate();
					}
					if( instance.GetSubInstanceSPB() ){
						instance.GetSubInstanceSPB()->GetParameterBlock()->Activate();
					}
					
					if( instance.GetSIIndexInstanceSPB() != spbSIIndexInstance ){
						if( instance.GetSIIndexInstanceSPB() ){
							instance.GetSIIndexInstanceSPB()->Activate();
						}
						spbSIIndexInstance = instance.GetSIIndexInstanceSPB();
					}
					
					if( rtshader.GetSPBInstanceIndexBase() != -1 ){
						shader.SetParameterInt( rtshader.GetSPBInstanceIndexBase(),
							instance.GetSIIndexInstanceFirst() );
					}
					
					if( doubleSided != curDoubleSided ){
						if( doubleSided ){
							OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
							
						}else{
							OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
						}
						
						curDoubleSided = doubleSided;
					}
					
					GLenum primitiveType = instance.GetPrimitiveType();
					
					if( pglPatchParameteri && shader.GetHasTessellation() ){
						pglPatchParameteri( GL_PATCH_VERTICES, instance.GetTessPatchVertexCount() );
						primitiveType = GL_PATCHES;
					}
					
					if( instance.GetSubInstanceCount() == 0 ){
						if( instance.GetIndexCount() == 0 ){
							OGL_CHECK( renderThread, glDrawArrays( primitiveType,
								instance.GetFirstPoint(), instance.GetPointCount() ) );
							
						}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
							// renderTaskInstance->GetFirstPoint() as base-vertex. required since
							// the indices are stored relative to the block of points for various
							// reasons. base-vertex is required to shift the indices to the correct
							// range of points in the vbo. FirstPoint contains already the index
							// to the first point in the block and thus is the right value we
							// need to shift the indices by
							
							OGL_CHECK( renderThread, pglDrawElementsBaseVertex( primitiveType,
								instance.GetIndexCount(), indexGLType,
								( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
								instance.GetFirstPoint() ) );
							
						}else{
							OGL_CHECK( renderThread, glDrawElements( primitiveType,
								instance.GetIndexCount(), indexGLType,
								( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ) ) );
						}
						
					}else{
						if( instance.GetIndexCount() == 0 ){
							OGL_CHECK( renderThread, pglDrawArraysInstanced( primitiveType,
								instance.GetFirstPoint(), instance.GetPointCount(),
								instance.GetSubInstanceCount() ) );
							
						}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
							OGL_CHECK( renderThread, pglDrawElementsInstancedBaseVertex(
								primitiveType, instance.GetIndexCount(), indexGLType,
								( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
								instance.GetSubInstanceCount(), instance.GetFirstPoint() ) );
							
						}else{
							OGL_CHECK( renderThread, pglDrawElementsInstanced(
								primitiveType, instance.GetIndexCount(), indexGLType,
								( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
								instance.GetSubInstanceCount() ) );
						}
					}
					
					iterInstance = iterInstance->GetNext();
				}
				iterVAO = iterVAO->GetNext();
			}
			iterTexture = iterTexture->GetNext();
		}
		iterShader = iterShader->GetNext();
	}
	
	pglBindVertexArray( 0 );
}



void deoglRenderGeometry::TransformPositions( const deoglVAO &vao, GLuint tboWeightMatrices,
GLuint vboTransformed, int firstPoint, int pointCount ){
	#ifdef ANDROID
	DETHROW( deeInvalidAction );
	#endif
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	renderThread.GetShader().ActivateShader( pShaderTransformPositions );
	
	tsmgr.EnableTBO( 0, tboWeightMatrices, GetSamplerClampNearest() );
	
	if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	}
	OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, vboTransformed ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao.GetVAO() ) );
	
	OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, firstPoint, pointCount ) );
	OGL_CHECK( renderThread, pglEndTransformFeedback() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
}

void deoglRenderGeometry::CalcNormalsTangents( const deoglVAO &vao, GLuint tboPositions,
deoglFramebuffer *fbo, int outputWidth, int outputHeight, int positionCount,
int normalCount, int /*tangentCount*/, int firstPoint, int pointCount ){
	#ifdef ANDROID
	DETHROW( deeInvalidAction );
	#endif
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	// set states
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	// prepare fbo
	renderThread.GetFramebuffer().Activate( fbo );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, outputWidth, outputHeight ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	// clear the texture
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, clearColor ) );
	
	// set shader
	renderThread.GetShader().ActivateShader( pShaderCalcNormalsTangents );
	deoglShaderCompiled &shader = *pShaderCalcNormalsTangents->GetCompiled();
	
	shader.SetParameterFloat( 0, 2.0f / ( float )outputWidth, 2.0f / ( float )outputHeight,
		0.5f / ( float )outputWidth - 1.0f, 0.5f / ( float )outputHeight - 1.0f );
	shader.SetParameterInt( 1, outputWidth );
	shader.SetParameterInt( 2, positionCount, positionCount + normalCount );
	
	// bind textures
	tsmgr.EnableTBO( 0, tboPositions, GetSamplerClampNearest() );
	
	// render
	OGL_CHECK( renderThread, pglBindVertexArray( vao.GetVAO() ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, firstPoint, pointCount ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderGeometry::WriteSkinnedVBO( const deoglVAO &vao, GLuint tboPositions,
deoglTexture &texNorTan, GLuint vboSkinned, int positionCount, int normalCount,
int firstPoint, int pointCount ){
	#ifdef ANDROID
	DETHROW( deeInvalidAction );
	#endif
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	// set shader
	renderThread.GetShader().ActivateShader( pShaderWriteSkinnedVBO );
	deoglShaderCompiled &shader = *pShaderWriteSkinnedVBO->GetCompiled();
	
	shader.SetParameterInt( 0, texNorTan.GetWidth() );
	shader.SetParameterInt( 1, positionCount, positionCount + normalCount );
	
	// bind textures
	tsmgr.EnableTBO( 0, tboPositions, GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, texNorTan, GetSamplerClampNearest() );
	
	// set states
	if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	}
	OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, vboSkinned ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao.GetVAO() ) );
	
	OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, firstPoint, pointCount ) );
	OGL_CHECK( renderThread, pglEndTransformFeedback() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
}

void deoglRenderGeometry::ApproxTransformVNT( const deoglVAO &vao,
GLuint tboWeightMatrices, GLuint vboTransformed, int firstPoint, int pointCount ){
	#ifdef ANDROID
	// android OpenGL ES 3.0 does not support texture buffer objects
	DETHROW( deeInvalidAction );
	#endif
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	renderThread.GetShader().ActivateShader( pShaderApproxTransformVNT );
	
	tsmgr.EnableTBO( 0, tboWeightMatrices, GetSamplerClampNearest() );
	
	if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	}
	OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, vboTransformed ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao.GetVAO() ) );
	
	OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, firstPoint, pointCount ) );
	OGL_CHECK( renderThread, pglEndTransformFeedback() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
}



// Private Functions
//////////////////////

void deoglRenderGeometry::pCleanUp(){
	if( pShaderApproxTransformVNT ){
		pShaderApproxTransformVNT->RemoveUsage();
		pShaderApproxTransformVNT = NULL;
	}
	
	if( pShaderWriteSkinnedVBO ){
		pShaderWriteSkinnedVBO->RemoveUsage();
		pShaderWriteSkinnedVBO = NULL;
	}
	if( pShaderCalcNormalsTangents ){
		pShaderCalcNormalsTangents->RemoveUsage();
		pShaderCalcNormalsTangents = NULL;
	}
	if( pShaderTransformPositions ){
		pShaderTransformPositions->RemoveUsage();
		pShaderTransformPositions = NULL;
	}
	
	if( pShaderParticle ){
		pShaderParticle->RemoveUsage();
		pShaderParticle = NULL;
	}
}
