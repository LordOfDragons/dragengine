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
#include "task/deoglComputeRenderTask.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskInstance.h"
#include "task/deoglRenderTaskPipeline.h"
#include "task/deoglRenderTaskTexture.h"
#include "task/deoglRenderTaskVAO.h"
#include "task/shared/deoglRenderTaskSharedInstance.h"
#include "task/shared/deoglRenderTaskSharedTexture.h"
#include "task/shared/deoglRenderTaskSharedVAO.h"
#include "task/shared/deoglRenderTaskSharedPool.h"
#include "task/persistent/deoglPersistentRenderTask.h"
#include "task/persistent/deoglPersistentRenderTaskInstance.h"
#include "task/persistent/deoglPersistentRenderTaskPipeline.h"
#include "task/persistent/deoglPersistentRenderTaskTexture.h"
#include "task/persistent/deoglPersistentRenderTaskVAO.h"

#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../component/deoglComponentLOD.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../extensions/deoglExtensions.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../light/deoglRLight.h"
#include "../light/volume/deoglLightVolume.h"
#include "../pipeline/deoglPipeline.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTLogger.h"
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



// Class deoglRenderGeometry
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderGeometry::deoglRenderGeometry( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),
pVertexPositionSetParams( nullptr ),
pVertexPositionSetParamSize( 0 )
{
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines defines;
	
	try{
		pipconf.Reset();
		pipconf.SetType( deoglPipelineConfiguration::etCompute );
		
		// copy vertices, normals and tangents
		pipconf.SetShader( renderThread, "DefRen Copy VNT", defines );
		pPipelineCopyVNT = pipelineManager.GetWith( pipconf );
		
		// transform vertices using vertex position set
		pipconf.SetShader( renderThread, "DefRen VPS Transform VNT", defines );
		pPipelineVPSTransformVNT = pipelineManager.GetWith( pipconf );
		
		// approximate transform vertices, normals and tangents
		pipconf.SetShader( renderThread, "DefRen Approx Transform VNT", defines );
		pPipelineApproxTransformVNT = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( "TRANSFORM_INPLACE" );
		pipconf.SetShader( renderThread, "DefRen Approx Transform VNT", defines );
		pPipelineApproxTransformVNTInplace = pipelineManager.GetWith( pipconf );
		defines.RemoveDefines( "TRANSFORM_INPLACE" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
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

void deoglRenderGeometry::RenderTask( const deoglRenderTask &renderTask ){
	const int pipelineCount = renderTask.GetPipelineCount();
	if( pipelineCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Geometry.RenderTask" );
	deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	const bool renderVSStereo = renderTask.GetRenderVSStereo();
// 	const int strideIndirect = sizeof( oglDrawIndirectCommand );
	deoglShaderParameterBlock *spbSIIndexInstance = NULL;
	deoglVAO *curVAO = NULL;
	int i, j, k, l, m;
	
	renderThread.GetBufferObject().GetSharedVBOListList().PrepareAllLists(); // needs to be done better
	
	if( renderVSStereo ){
		OGL_CHECK( renderThread, pglBindBuffer( GL_DRAW_INDIRECT_BUFFER, renderTask.GetVBODrawIndirect() ) );
	}
	
	try{
		for( i=0; i<pipelineCount; i++ ){
			const deoglRenderTaskPipeline &rtpipeline = *renderTask.GetPipelineAt( i );
			const deoglPipeline &pipeline = *rtpipeline.GetPipeline();
			const deoglPipelineConfiguration &pipconf = pipeline.GetGlConfiguration();
			const int targetSPBInstanceIndexBase = pipconf.GetSPBInstanceIndexBase();
			const int targetDrawIDOffset = pipconf.GetDrawIDOffset();
			deoglShaderCompiled &shader = pipeline.GetGlShader();
			
			pipeline.Activate();
			
			if( renderParamBlock ){
				renderParamBlock->Activate();
			}
			
			if( targetDrawIDOffset != -1 ){
				shader.SetParameterInt( targetDrawIDOffset, 0 );
			}
			
			const int textureCount = rtpipeline.GetTextureCount();
			for( j=0; j<textureCount; j++ ){
				const deoglRenderTaskTexture &rttexture = *rtpipeline.GetTextureAt( j );
				const deoglTexUnitsConfig * const tuc = rttexture.GetTexture()->GetTUC();
				if( tuc ){
					tuc->Apply();
					if( tuc->GetParameterBlock() ){
						tuc->GetParameterBlock()->Activate();
					}
				}
				
				const int vaoCount = rttexture.GetVAOCount();
				for( k=0; k<vaoCount; k++ ){
					const deoglRenderTaskVAO &rtvao = *rttexture.GetVAOAt( k );
					const int instanceCount = rtvao.GetInstanceCount();
					if( instanceCount == 0 ){
						continue;
					}
					
					deoglVAO * const vao = rtvao.GetVAO()->GetVAO();
					if( vao != curVAO ){
						pglBindVertexArray( vao->GetVAO() );
						curVAO = vao;
					}
					
					const GLenum indexGLType = vao->GetIndexGLType();
					const int indexSize = vao->GetIndexSize();
					
					for( l=0; l<instanceCount; l++ ){
						const deoglRenderTaskInstance &rtinstance = *rtvao.GetInstanceAt( l );
						const deoglRenderTaskSharedInstance &instance = *rtinstance.GetInstance();
						
						if( instance.GetParameterBlock() ){
							instance.GetParameterBlock()->Activate();
						}
						if( instance.GetParameterBlockSpecial() ){
							instance.GetParameterBlockSpecial()->Activate();
						}
						if( instance.GetSubInstanceSPB() ){
							instance.GetSubInstanceSPB()->GetParameterBlock()->Activate();
						}
						
						if( rtinstance.GetSIIndexInstanceSPB() != spbSIIndexInstance ){
							if( rtinstance.GetSIIndexInstanceSPB() ){
								rtinstance.GetSIIndexInstanceSPB()->Activate();
							}
							spbSIIndexInstance = rtinstance.GetSIIndexInstanceSPB();
						}
						
						if( targetSPBInstanceIndexBase != -1 ){
							shader.SetParameterInt( targetSPBInstanceIndexBase,
								rtinstance.GetSIIndexInstanceFirst() );
						}
						
						GLenum primitiveType = instance.GetPrimitiveType();
						
						if( pglPatchParameteri && shader.GetHasTessellation() ){
							pglPatchParameteri( GL_PATCH_VERTICES, instance.GetTessPatchVertexCount() );
							primitiveType = GL_PATCHES;
						}
						
						if( renderVSStereo ){
							// this is not working. calls to pglMultiDrawArraysIndirect and
							// pglMultiDrawElementsIndirect are 2x slower than calling the
							// non-indirect counter parts. this is unusable
							#if 0
							if( instance.GetIndexCount() == 0 ){
								OGL_CHECK( renderThread, pglMultiDrawArraysIndirect( primitiveType,
									( void* )( intptr_t )( strideIndirect * rtinstance.GetDrawIndirectIndex() ),
									rtinstance.GetDrawIndirectCount(), strideIndirect ) );
								
							}else{
								OGL_CHECK( renderThread, pglMultiDrawElementsIndirect( primitiveType, indexGLType,
									( void* )( intptr_t )( strideIndirect * rtinstance.GetDrawIndirectIndex() ),
									rtinstance.GetDrawIndirectCount(), strideIndirect ) );
							}
							#endif
							
							const int subInstanceCount = rtinstance.GetSubInstanceCount() + instance.GetSubInstanceCount();
							
							if( subInstanceCount == 0 ){
								if( instance.GetIndexCount() == 0 ){
									const GLint first[ 2 ] = { instance.GetFirstPoint(), instance.GetFirstPoint() };
									const GLsizei count[ 2 ] = { instance.GetPointCount(), instance.GetPointCount() };
									OGL_CHECK( renderThread, pglMultiDrawArrays( primitiveType, first, count, 2 ) );
									
								}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
									const void * const offsetIndex = ( void* )( intptr_t )( indexSize * instance.GetFirstIndex() );
									const GLsizei count[ 2 ] = { instance.GetIndexCount(), instance.GetIndexCount() };
									const void * const indices[ 2 ] = { offsetIndex, offsetIndex };
									const GLint basevertex[ 2 ] = { instance.GetFirstPoint(), instance.GetFirstPoint() };
									OGL_CHECK( renderThread, pglMultiDrawElementsBaseVertex(
										primitiveType, count, indexGLType, indices, 2, basevertex ) );
									
								}else{
									const void * const offsetIndex = ( void* )( intptr_t )( indexSize * instance.GetFirstIndex() );
									const GLsizei count[ 2 ] = { instance.GetIndexCount(), instance.GetIndexCount() };
									const void * const indices[ 2 ] = { offsetIndex, offsetIndex };
									OGL_CHECK( renderThread, pglMultiDrawElements( primitiveType, count, indexGLType, indices, 2 ) );
								}
								
							}else{
								// there exists no instanced versions of glMultiDraw so we have
								// to hack it. we start with drawID 1 instead of 0 so we do not
								// have to reset it after the last draw call
								if( instance.GetIndexCount() == 0 ){
									for( m=1; m>=0; m-- ){
										shader.SetParameterInt( targetDrawIDOffset, m );
										OGL_CHECK( renderThread, pglDrawArraysInstanced( primitiveType,
											instance.GetFirstPoint(), instance.GetPointCount(), subInstanceCount ) );
									}
									
								}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
									for( m=1; m>=0; m-- ){
										shader.SetParameterInt( targetDrawIDOffset, m );
										OGL_CHECK( renderThread, pglDrawElementsInstancedBaseVertex(
											primitiveType, instance.GetIndexCount(), indexGLType,
											( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
											subInstanceCount, instance.GetFirstPoint() ) );
									}
									
								}else{
									for( m=1; m>=0; m-- ){
										shader.SetParameterInt( targetDrawIDOffset, m );
										OGL_CHECK( renderThread, pglDrawElementsInstanced(
											primitiveType, instance.GetIndexCount(), indexGLType,
											( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
											subInstanceCount ) );
									}
								}
							}
							
						}else{
							const int subInstanceCount = rtinstance.GetSubInstanceCount() + instance.GetSubInstanceCount();
							
							if( subInstanceCount == 0 ){
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
										instance.GetFirstPoint(), instance.GetPointCount(), subInstanceCount ) );
									
								}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
									OGL_CHECK( renderThread, pglDrawElementsInstancedBaseVertex(
										primitiveType, instance.GetIndexCount(), indexGLType,
										( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
										subInstanceCount, instance.GetFirstPoint() ) );
									
								}else{
									OGL_CHECK( renderThread, pglDrawElementsInstanced(
										primitiveType, instance.GetIndexCount(), indexGLType,
										( GLvoid* )( intptr_t )( indexSize * instance.GetFirstIndex() ),
										subInstanceCount ) );
								}
							}
						}
					}
				}
			}
		}
		
		pglBindVertexArray( 0 );
		pglBindBuffer( GL_DRAW_INDIRECT_BUFFER, 0 );
		
	}catch( const deException & ){
		pglBindVertexArray( 0 );
		pglBindBuffer( GL_DRAW_INDIRECT_BUFFER, 0 );
		throw;
	}
}

void deoglRenderGeometry::RenderTask( const deoglComputeRenderTask &renderTask ){
	const int stepCount = renderTask.GetStepCount();
	if( stepCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglComputeRenderTask::sStep * const steps = renderTask.GetSteps();
	if( ! steps ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( renderThread, "Geometry.RenderTaskCompute" );
	const deoglRenderTaskSharedPool &rtsPool = renderThread.GetRenderTaskSharedPool();
	deoglPipelineManager &pipManager = renderThread.GetPipelineManager();
	deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	const bool renderVSStereo = renderTask.GetRenderVSStereo();
// 	const int strideIndirect = sizeof( oglDrawIndirectCommand );
	const deoglShaderParameterBlock * const ssboIndexInstance = renderTask.GetSSBOSteps();
	const bool skipSubInstanceGroups = renderTask.GetSkipSubInstanceGroups();
	const uint32_t undefined = ~( uint32_t )0;
	uint32_t curPipeline = undefined;
	uint32_t curTUC = undefined;
	uint32_t curVAO = undefined;
	uint32_t curInstance = undefined;
	int i = 0, m = 0;
	
	deoglShaderCompiled *shader = nullptr;
	int targetSPBInstanceIndexBase = -1;
	int targetDrawIDOffset = -1;
	int spbInstanceIndexBase = 0;
	
	GLenum indexGLType = GL_NONE;
	GLenum primitiveType = GL_TRIANGLES;
	GLvoid *indexPointer = nullptr;
	int subInstanceCount = 0;
	int indexCount = 0;
	int firstIndex = 0;
	int indexSize = 0;
	int pointCount = 0;
	int firstPoint = 0;
	
	renderThread.GetBufferObject().GetSharedVBOListList().PrepareAllLists(); // needs to be done better
	
	if( renderVSStereo ){
// 		OGL_CHECK( renderThread, pglBindBuffer( GL_DRAW_INDIRECT_BUFFER, renderTask.GetVBODrawIndirect() ) );
	}
	
	try{
		for( i=0; i<stepCount; i++ ){
			const deoglComputeRenderTask::sStep &step = steps[ i ];
			
			// pipeline
			if( step.pipeline != curPipeline ){
				const deoglPipeline &pipeline = *pipManager.GetWithRTSIndex( step.pipeline );
				
				curPipeline = step.pipeline;
				curInstance = undefined;
				
				pipeline.Activate();
				
				const deoglPipelineConfiguration &pipconf = pipeline.GetGlConfiguration();
				targetSPBInstanceIndexBase = pipconf.GetSPBInstanceIndexBase();
				targetDrawIDOffset = pipconf.GetDrawIDOffset();
				shader = &pipeline.GetGlShader();
				
				if( renderParamBlock ){
					renderParamBlock->Activate( deoglSkinShader::eubRenderParameters );
				}
				if( ssboIndexInstance ){
					ssboIndexInstance->Activate( deoglSkinShader::essboInstanceIndex );
				}
				
				if( targetDrawIDOffset != -1 ){
					shader->SetParameterInt( targetDrawIDOffset, 0 );
				}
			}
			
			// texture
			if( step.tuc != curTUC ){
				const deoglTexUnitsConfig * const tuc = rtsPool.GetTextureAt( step.tuc ).GetTUC();
				
				curTUC = step.tuc;
				
				if( tuc ){
					tuc->Apply();
					if( tuc->GetParameterBlock() ){
						tuc->GetParameterBlock()->Activate();
					}
				}
			}
			
			// vao
			if( step.vao != curVAO ){
				const deoglVAO &vao = *rtsPool.GetVAOAt( step.vao ).GetVAO();
				
				curVAO = step.vao;
				curInstance = undefined;
				
				indexGLType = vao.GetIndexGLType();
				indexSize = vao.GetIndexSize();
				
				OGL_CHECK( renderThread, pglBindVertexArray( vao.GetVAO() ) );
			}
			
			// instance
			if( step.instance != curInstance ){
				const deoglRenderTaskSharedInstance &instance = rtsPool.GetInstanceAt( step.instance );
				
				curInstance = step.instance;
				
				if( instance.GetParameterBlock() ){
					instance.GetParameterBlock()->Activate();
				}
				if( instance.GetParameterBlockSpecial() ){
					instance.GetParameterBlockSpecial()->Activate();
				}
				if( instance.GetSubInstanceSPB() ){
					instance.GetSubInstanceSPB()->GetParameterBlock()->Activate();
				}
				
				primitiveType = instance.GetPrimitiveType();
				subInstanceCount = instance.GetSubInstanceCount();
				indexCount = instance.GetIndexCount();
				firstIndex = instance.GetFirstIndex();
				pointCount = instance.GetPointCount();
				firstPoint = instance.GetFirstPoint();
				indexPointer = ( GLvoid* )( intptr_t )( indexSize * firstIndex );
				
				if( pglPatchParameteri && shader->GetHasTessellation() ){
					OGL_CHECK( renderThread, pglPatchParameteri( GL_PATCH_VERTICES,
						instance.GetTessPatchVertexCount() ) );
					primitiveType = GL_PATCHES;
				}
			}
			
			if( targetSPBInstanceIndexBase != -1 ){
				shader->SetParameterInt( targetSPBInstanceIndexBase, spbInstanceIndexBase );
			}
			
			const int advanceSubInstanceCount = decMath::max( step.subInstanceCount, 1 );
			spbInstanceIndexBase += advanceSubInstanceCount;
			
			const int realSubInstanceCount = step.subInstanceCount + subInstanceCount;
			
			if( renderVSStereo ){
				#if 0
				if( instance.GetIndexCount() == 0 ){
					OGL_CHECK( renderThread, pglMultiDrawArraysIndirect( primitiveType,
						( void* )( intptr_t )( strideIndirect * rtinstance.GetDrawIndirectIndex() ),
						rtinstance.GetDrawIndirectCount(), strideIndirect ) );
					
				}else{
					OGL_CHECK( renderThread, pglMultiDrawElementsIndirect( primitiveType, indexGLType,
						( void* )( intptr_t )( strideIndirect * rtinstance.GetDrawIndirectIndex() ),
						rtinstance.GetDrawIndirectCount(), strideIndirect ) );
				}
				#endif
				
				if( realSubInstanceCount == 0 ){
					if( indexCount == 0 ){
						const GLint first[ 2 ] = { firstPoint, firstPoint };
						const GLsizei count[ 2 ] = { pointCount, pointCount };
						OGL_CHECK( renderThread, pglMultiDrawArrays( primitiveType, first, count, 2 ) );
						
					}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
						const GLsizei count[ 2 ] = { indexCount, indexCount };
						const GLvoid * const indices[ 2 ] = { indexPointer, indexPointer };
						const GLint basevertex[ 2 ] = { firstPoint, firstPoint };
						OGL_CHECK( renderThread, pglMultiDrawElementsBaseVertex(
							primitiveType, count, indexGLType, indices, 2, basevertex ) );
						
					}else{
						const GLsizei count[ 2 ] = { indexCount, indexCount };
						const GLvoid * const indices[ 2 ] = { indexPointer, indexPointer };
						OGL_CHECK( renderThread, pglMultiDrawElements( primitiveType, count, indexGLType, indices, 2 ) );
					}
					
				}else{
					// there exists no instanced versions of glMultiDraw so we have
					// to hack it. we start with drawID 1 instead of 0 so we do not
					// have to reset it after the last draw call
					if( indexCount == 0 ){
						for( m=1; m>=0; m-- ){
							shader->SetParameterInt( targetDrawIDOffset, m );
							OGL_CHECK( renderThread, pglDrawArraysInstanced( primitiveType,
								firstPoint, pointCount, realSubInstanceCount ) );
						}
						
					}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
						for( m=1; m>=0; m-- ){
							shader->SetParameterInt( targetDrawIDOffset, m );
							OGL_CHECK( renderThread, pglDrawElementsInstancedBaseVertex( primitiveType,
								indexCount, indexGLType, indexPointer, realSubInstanceCount, firstPoint ) );
						}
						
					}else{
						for( m=1; m>=0; m-- ){
							shader->SetParameterInt( targetDrawIDOffset, m );
							OGL_CHECK( renderThread, pglDrawElementsInstanced( primitiveType,
								indexCount, indexGLType, indexPointer, realSubInstanceCount ) );
						}
					}
					
					if( skipSubInstanceGroups ){
						i += advanceSubInstanceCount - 1;
					}
				}
				
			}else{
				if( realSubInstanceCount == 0 ){
					if( indexCount == 0 ){
						OGL_CHECK( renderThread, glDrawArrays( primitiveType, firstPoint, pointCount ) );
						
					}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
						// renderTaskInstance->GetFirstPoint() as base-vertex. required since
						// the indices are stored relative to the block of points for various
						// reasons. base-vertex is required to shift the indices to the correct
						// range of points in the vbo. FirstPoint contains already the index
						// to the first point in the block and thus is the right value we
						// need to shift the indices by
						
						OGL_CHECK( renderThread, pglDrawElementsBaseVertex( primitiveType,
							indexCount, indexGLType, indexPointer, firstPoint ) );
						
					}else{
						OGL_CHECK( renderThread, glDrawElements( primitiveType,
							indexCount, indexGLType, indexPointer ) );
					}
					
				}else{
					if( indexCount == 0 ){
						OGL_CHECK( renderThread, pglDrawArraysInstanced( primitiveType,
							firstPoint, pointCount, realSubInstanceCount ) );
						
					}else if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
						OGL_CHECK( renderThread, pglDrawElementsInstancedBaseVertex( primitiveType,
							indexCount, indexGLType, indexPointer, realSubInstanceCount, firstPoint ) );
						
					}else{
						OGL_CHECK( renderThread, pglDrawElementsInstanced( primitiveType,
							indexCount, indexGLType, indexPointer, realSubInstanceCount ) );
					}
					
					if( skipSubInstanceGroups ){
						i += advanceSubInstanceCount - 1;
					}
				}
			}
		}
		
		pglBindVertexArray( 0 );
		pglBindBuffer( GL_DRAW_INDIRECT_BUFFER, 0 );
		
	}catch( const deException & ){
		pglBindVertexArray( 0 );
		pglBindBuffer( GL_DRAW_INDIRECT_BUFFER, 0 );
		
		renderThread.GetLogger().LogErrorFormat( "Render compute task failed: step=%d m=%d"
			" pip=%d tex=%d vao=%d inst=%d tspbiib=%d tdido=%d indtype=%x primtype=%d sicnt=%d"
			" ssicnt=%d indcnt=%d indfir=%d indsize=%d pntcnt=%d pntfir=%d spbiib=%d",
			i, m, curPipeline, curTUC, curVAO, curInstance, targetSPBInstanceIndexBase,
			targetDrawIDOffset, indexGLType, primitiveType, subInstanceCount,
			steps[ i ].subInstanceCount, indexCount, firstIndex, indexSize,
			pointCount, firstPoint, spbInstanceIndexBase );
		throw;
	}
}

void deoglRenderGeometry::RenderTask( const deoglPersistentRenderTask &renderTask ){
	const decPointerLinkedList::cListEntry *iterPipeline = renderTask.GetRootPipeline();
	if( ! iterPipeline ){
		return;
	}
	
	const deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	const deoglShaderParameterBlock *spbSIIndexInstance = NULL;
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Geometry.RenderTask(Persistent)" );
	const deoglVAO *curVAO = NULL;
	
	renderThread.GetBufferObject().GetSharedVBOListList().PrepareAllLists(); // needs to be done better
	
	while( iterPipeline ){
		const deoglPersistentRenderTaskPipeline &rtpipeline = *( ( deoglPersistentRenderTaskPipeline* )iterPipeline->GetOwner() );
		const deoglPipeline &pipeline = *rtpipeline.GetPipeline();
		deoglShaderCompiled &shader = pipeline.GetGlShader();
		
		pipeline.Activate();
		
		if( renderParamBlock ){
			renderParamBlock->Activate();
		}
		
		const decPointerLinkedList::cListEntry *iterTexture = rtpipeline.GetRootTexture();
		while( iterTexture ){
			const deoglPersistentRenderTaskTexture &texture = *( ( deoglPersistentRenderTaskTexture* )iterTexture->GetOwner() );
			
			if( texture.GetParameterBlock() ){
				texture.GetParameterBlock()->Activate();
			}
			texture.GetTUC()->Apply();
			
			const decPointerLinkedList::cListEntry *iterVAO = texture.GetRootVAO();
			while( iterVAO ){
				const deoglPersistentRenderTaskVAO &rtvao = *( ( deoglPersistentRenderTaskVAO* )iterVAO->GetOwner() );
				const decPointerLinkedList::cListEntry *iterInstance = rtvao.GetRootInstance();
				if( ! iterInstance ){
					iterVAO = iterVAO->GetNext();
					continue;
				}
				
				const deoglVAO * const vao = rtvao.GetVAO();
				if( vao != curVAO ){
					pglBindVertexArray( vao->GetVAO() );
					curVAO = vao;
				}
				
				const GLenum indexGLType = vao->GetIndexGLType();
				const int indexSize = vao->GetIndexSize();
				
				while( iterInstance ){
					const deoglPersistentRenderTaskInstance &instance = *( ( deoglPersistentRenderTaskInstance* )iterInstance->GetOwner() );
					
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
					
					if( rtpipeline.GetSPBInstanceIndexBase() != -1 ){
						shader.SetParameterInt( rtpipeline.GetSPBInstanceIndexBase(), instance.GetSIIndexInstanceFirst() );
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
		iterPipeline = iterPipeline->GetNext();
	}
	
	pglBindVertexArray( 0 );
}



void deoglRenderGeometry::CopyVNT( GLuint vao, GLuint vbo, const deoglSPBlockSSBO &transformed,
int firstPoint, int pointCount ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Geometry.CopyVNT" );
	
	pPipelineCopyVNT->Activate();
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, vbo ) );
	transformed.Activate();
	
	pPipelineCopyVNT->GetGlShader().SetParameterUInt( 0, firstPoint, pointCount );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( pointCount - 1 ) / 64 + 1, 1, 1 ) );
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0) );
	transformed.Deactivate();
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

deoglRenderGeometry::sVertexPositionSetParams *deoglRenderGeometry::GetVertexPositionSetParams( int count ){
	DEASSERT_TRUE( count > 0 )
	
	if( count > pVertexPositionSetParamSize ){
		sVertexPositionSetParams * const newArray = new sVertexPositionSetParams[ count ];
		delete [] pVertexPositionSetParams;
		pVertexPositionSetParams = newArray;
		pVertexPositionSetParamSize = count;
	}
	
	return pVertexPositionSetParams;
}

void deoglRenderGeometry::VPSTransformVNT( GLuint vaoModelData, GLuint vboVertexPositionSetData,
const sVertexPositionSetParams *params, int paramCount, const deoglSPBlockSSBO &transformed ){
	DEASSERT_NOTNULL( params )
	DEASSERT_TRUE( paramCount > 0 )
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Geometry.VPSTransformVNT" );
	
	pPipelineVPSTransformVNT->Activate();
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, vboVertexPositionSetData ) );
	transformed.Activate();
	
	deoglShaderCompiled &shader = pPipelineVPSTransformVNT->GetGlShader();
	
	int i;
	for( i=0; i<paramCount; i++ ){
		shader.SetParameterUInt( 0, params[ i ].firstPoint, params[ i ].pointCount );
		shader.SetParameterFloat( 1, params[ i ].weight );
		
		OGL_CHECK( renderThread, pglDispatchCompute( ( params[ i ].pointCount - 1 ) / 64 + 1, 1, 1 ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	}
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0) );
	transformed.Deactivate();
	
	OGL_CHECK( renderThread, pglBindVertexArray( vaoModelData ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderGeometry::ApproxTransformVNT( GLuint vao, GLuint vbo,
const deoglSPBlockSSBO *weightMatrices, const deoglSPBlockSSBO &transformed,
int firstPoint, int pointCount, bool inplace ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Geometry.ApproxTransformVNT" );
	
	const deoglPipeline &pipeline = inplace ? *pPipelineApproxTransformVNTInplace : *pPipelineApproxTransformVNT;
	pipeline.Activate();
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, vbo ) );
	transformed.Activate();
	if( weightMatrices ){
		weightMatrices->Activate();
	}
	
	pipeline.GetGlShader().SetParameterUInt( 0, firstPoint, pointCount );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( pointCount - 1 ) / 64 + 1, 1, 1 ) );
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0) );
	transformed.Deactivate();
	if( weightMatrices ){
		weightMatrices->Deactivate();
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( vao ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}



// Private Functions
//////////////////////

void deoglRenderGeometry::pCleanUp(){
	if( pVertexPositionSetParams ){
		delete [] pVertexPositionSetParams;
	}
}
