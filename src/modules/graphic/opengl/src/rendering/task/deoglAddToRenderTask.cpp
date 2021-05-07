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

#include "deoglAddToRenderTask.h"
#include "deoglRenderTask.h"
#include "deoglRenderTaskInstance.h"
#include "deoglRenderTaskShader.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "shared/deoglRenderTaskSharedShader.h"
#include "../defren/deoglDeferredRendering.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListHTSector.h"
#include "../../collidelist/deoglCollideListPropField.h"
#include "../../collidelist/deoglCollideListPropFieldType.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentLOD.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../decal/deoglRDecal.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../model/texture/deoglModelTexture.h"
#include "../../occlusiontest/mesh/deoglDynamicOcclusionMesh.h"
#include "../../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../propfield/deoglPropFieldCluster.h"
#include "../../propfield/deoglRPropField.h"
#include "../../propfield/deoglRPropFieldType.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../skin/state/deoglSkinStateRenderable.h"
#include "../../target/deoglRenderTarget.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglHTSTexture.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglRHTSector.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglSharedVBO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../vbo/deoglSharedVBOList.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToRenderTask
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToRenderTask::deoglAddToRenderTask( deoglRenderThread &renderThread, deoglRenderTask &renderTask ) :
pRenderThread( renderThread  ),
pRenderTask( renderTask  )
{
	Reset();
}

deoglAddToRenderTask::~deoglAddToRenderTask(){
}



// Management
///////////////

void deoglAddToRenderTask::SetSkinShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	pSkinShaderType = shaderType;
}

void deoglAddToRenderTask::SetSkinShaderTypeRibbon( deoglSkinTexture::eShaderTypes shaderType ){
	pSkinShaderTypeRibbon = shaderType;
}

void deoglAddToRenderTask::SetSkinShaderTypeBeam( deoglSkinTexture::eShaderTypes shaderType ){
	pSkinShaderTypeBeam = shaderType;
}



void deoglAddToRenderTask::SetSolid( bool solid ){
	pSolid = solid;
}

void deoglAddToRenderTask::SetNoNotReflected( bool noNotReflected ){
	pNoNotReflected = noNotReflected;
}

void deoglAddToRenderTask::SetNoRendered( bool noRendered ){
	pNoRendered = noRendered;
}

void deoglAddToRenderTask::SetOutline( bool outline ){
	pOutline = outline;
}

void deoglAddToRenderTask::SetNoShadowNone( bool noShadowNone ){
	pNoShadowNone = noShadowNone;
}

void deoglAddToRenderTask::SetFilterHoles( bool filterHoles ){
	pFilterHoles = filterHoles;
}

void deoglAddToRenderTask::SetWithHoles( bool withHoles ){
	pWithHoles = withHoles;
}

void deoglAddToRenderTask::SetFilterDoubleSided( bool filterDoubleSided ){
	pFilterDoubleSided = filterDoubleSided;
}
void deoglAddToRenderTask::SetDoubleSided( bool doubleSided ){
	pDoubleSided = doubleSided;
}

void deoglAddToRenderTask::SetFilterDecal( bool filterDecal ){
	pFilterDecal = filterDecal;
}

void deoglAddToRenderTask::SetDecal( bool decal ){
	pDecal = decal;
}

void deoglAddToRenderTask::SetFilterCubeFace( int cubeFace ){
	pFilterCubeFace = cubeFace;
}

void deoglAddToRenderTask::SetUseSpecialParamBlock( bool use ){
	pUseSpecialParamBlock = use;
}

void deoglAddToRenderTask::SetEnforceShader( deoglRenderTaskSharedShader *shader ){
	pEnforceShader = shader;
}



void deoglAddToRenderTask::Reset(){
	pSkinShaderType = deoglSkinTexture::estComponentGeometry;
	
	pSolid = false;
	pNoShadowNone = false;
	pNoNotReflected = false;
	pNoRendered = false;
	pOutline = false;
	
	pFilterHoles = false;
	pWithHoles = false;
	
	pFilterDoubleSided = false;
	pDoubleSided = false;
	
	pFilterDecal = false;
	pDecal = false;
	
	pFilterCubeFace = -1;
	
	pUseSpecialParamBlock = false;
	
	pEnforceShader = NULL;
}

// #define SPECIAL_DEBUG_ON
#ifdef SPECIAL_DEBUG_ON
#include <dragengine/common/utils/decTimer.h>
static float debug1 = 0.0f;
static int debug1b = 0;
#endif

void deoglAddToRenderTask::AddComponent( deoglRComponent &component, int lodLevel ){
	if( ! component.GetParentWorld() || ! component.GetModel() ){
		return;
	}
	if( pFilterCubeFace != -1 && ! component.GetCubeFaceVisible( pFilterCubeFace ) ){
		return;
	}
	
	const deoglRModel &model = *component.GetModel();
	const deoglModelLOD &modelLOD = model.GetLODAt( lodLevel );
	const int textureCount = modelLOD.GetTextureCount();
	int t;
	
	for( t=0; t<textureCount; t++ ){
		const deoglModelTexture &texture = modelLOD.GetTextureAt( t );
		
		if( texture.GetFaceCount() > 0 ){
			#ifdef SPECIAL_DEBUG_ON
			decTimer timer;
			#endif
			AddComponentFaces( component, t, texture.GetFirstFace(), texture.GetFaceCount(), lodLevel );
			#ifdef SPECIAL_DEBUG_ON
			debug1 += timer.GetElapsedTime(); debug1b++;
			#endif
		}
	}
}

void deoglAddToRenderTask::AddComponent( const deoglCollideListComponent &clcomponent ){
	AddComponent( *clcomponent.GetComponent(), clcomponent.GetLODLevel() );
}

void deoglAddToRenderTask::AddComponents( const deoglCollideList &clist ){
	const int componentCount = clist.GetComponentCount();
	int c;
	
	#ifdef SPECIAL_DEBUG_ON
	decTimer timer;
	debug1 = 0.0f; debug1b = 0;
	#endif
	for( c=0; c<componentCount; c++ ){
		AddComponent( *clist.GetComponentAt( c ) );
	}
	#ifdef SPECIAL_DEBUG_ON
	pRenderThread.GetLogger().LogInfoFormat( "deoglAddToRenderTask::AddComponents(%i) = %iys",
		componentCount, (int)(timer.GetElapsedTime()*1e6f) );
	pRenderThread.GetLogger().LogInfoFormat( "deoglAddToRenderTask::AddComponent "
		"AddComponentFaces(%i) = %iys", debug1b, (int)(debug1*1e6f) );
	#endif
}

void deoglAddToRenderTask::AddComponentsHighestLod( const deoglCollideList &clist ){
	const int count = clist.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRComponent &component = *clist.GetComponentAt( i )->GetComponent();
		AddComponent( component, component.GetLODCount() - 1 );
	}
}

void deoglAddToRenderTask::AddComponentFaces( deoglRComponent &component, int texture, int lodLevel ){
	if( ! component.GetModel() ){
		return;
	}
	
	const deoglModelTexture &t = component.GetModel()->GetLODAt( lodLevel ).GetTextureAt( texture );
	if( t.GetFaceCount() > 0 ){
		AddComponentFaces( component, texture, t.GetFirstFace(), t.GetFaceCount(), lodLevel );
	}
}

void deoglAddToRenderTask::AddComponentFaces( deoglRComponent &component, int texture,
int firstFace, int faceCount, int lodLevel ){
	deoglRComponentTexture &componentTexture = component.GetTextureAt( texture );
	deoglSkinTexture * const skinTexture = componentTexture.GetUseSkinTexture();
	if( ! skinTexture ){
		return;
	}
	
	if( pFilterReject( skinTexture ) ){
		return;
	}
	
	const bool doubleSided = componentTexture.GetUseDoubleSided();
	if( pFilterDoubleSided && pDoubleSided != doubleSided ){
		return;
	}
	
	if( pFilterDecal && pDecal != componentTexture.GetUseDecal() ){
		return;
	}
	
	// hack style test for a camera renderable
	deoglSkinChannel *skinChannel = skinTexture->GetChannelAt( deoglSkinChannel::ectColor );
	deoglSkinState * const useSkinState = componentTexture.GetUseSkinState();
	
	if( skinChannel && useSkinState ){
		const deoglRDynamicSkin *dynamicSkin = component.GetDynamicSkin();
		const int skinRenderable = skinChannel->GetRenderable();
		
		if( skinRenderable >= 0 && skinRenderable < useSkinState->GetRenderableCount() && dynamicSkin ){
			const deoglSkinStateRenderable &skinStateRenderable = *useSkinState->GetRenderableAt( skinRenderable );
			
			if( skinStateRenderable.GetHostRenderable() != -1 ){
				if( dynamicSkin->GetRenderableAt( skinStateRenderable.GetHostRenderable() )->GetRenderPlan() ){
					return;
				}
			}
		}
	}
	
	// obtain render task vao and add faces
	deoglRenderTaskVAO * const rtvao = pGetTaskVAO( pSkinShaderType, skinTexture,
		componentTexture.GetTUCForShaderType( pSkinShaderType ), component.GetVAO( lodLevel ) );
	
	const deoglSharedSPBElement * const spbElement = componentTexture.GetSharedSPBElement();
	
	deoglRenderTaskInstanceGroup &rtiGroup = componentTexture.GetSharedSPBRTIGroup( lodLevel ).GetGroup();
	if( rtiGroup.GetTrackingNumber() != pRenderTask.GetTrackingNumber() ){
		pRenderTask.AddInstanceGroup( &rtiGroup );
	}
	
	deoglRenderTaskInstance *rti = rtvao->GetInstanceForIndex( rtiGroup.GetIndex() );
	if( ! rti ){
		rti = pRenderTask.InstanceFromPool();
		rti->SetGroup( &rtiGroup );
		rti->SetSubInstanceSPB( &spbElement->GetSPB() );
		rti->SetFirstPoint( component.GetPointOffset( lodLevel ) );
		rti->SetFirstIndex( component.GetIndexOffset( lodLevel ) + firstFace * 3 );
		rti->SetIndexCount( faceCount * 3 );
		rti->SetDoubleSided( doubleSided );
		rtvao->AddInstance( rti );
	}
	
	rti->AddSubInstance( spbElement->GetIndex(), component.GetSpecialFlags() );
}



void deoglAddToRenderTask::AddBillboards( const deoglCollideList &clist ){
	const int count = clist.GetBillboardCount();
	int i;
	
	for( i=0; i<count; i++ ){
		AddBillboard( *clist.GetBillboardAt( i ) );
	}
}

void deoglAddToRenderTask::AddBillboard( deoglRBillboard &billboard ){
	if( ! billboard.GetParentWorld() || ! billboard.GetSkin() ){
		return;
	}
	if( pFilterCubeFace != -1 && ! billboard.GetCubeFaceVisible( pFilterCubeFace ) ){
		return;
	}
	
	deoglSkinTexture &texture = billboard.GetSkin()->GetTextureAt( 0 );
	if( pFilterReject( &texture ) ){
		return;
	}
	if( pFilterDoubleSided && ! pDoubleSided ){
		return;
	}
	if( pFilterDecal && pDecal ){
		return;
	}
	
	// hack style test for a camera renderable
	deoglSkinChannel *skinChannel = texture.GetChannelAt( deoglSkinChannel::ectColor );
	deoglSkinState * const skinState = billboard.GetSkinState();
	
	if( skinChannel && skinState ){
		const deoglRDynamicSkin * const dynamicSkin = billboard.GetDynamicSkin();
		const int skinRenderable = skinChannel->GetRenderable();
		
		if( skinRenderable >= 0 && skinRenderable < skinState->GetRenderableCount() && dynamicSkin ){
			const deoglSkinStateRenderable &skinStateRenderable = *skinState->GetRenderableAt( skinRenderable );
			
			if( skinStateRenderable.GetHostRenderable() != -1 ){
				if( dynamicSkin->GetRenderableAt( skinStateRenderable.GetHostRenderable() )->GetRenderPlan() ){
					return;
				}
			}
		}
	}
	
	// obtain render task vao and add faces
	deoglRenderTaskVAO * const rtvao = pGetTaskVAO( pSkinShaderType, &texture,
		billboard.GetTUCForShaderType( pSkinShaderType ),
		pRenderThread.GetDeferredRendering().GetVAOBillboard() );
	
	const deoglSharedSPBElement * const spbElement = billboard.GetSharedSPBElement();
	
	deoglRenderTaskInstanceGroup &rtiGroup = billboard.GetSharedSPBRTIGroup().GetGroup();
	if( rtiGroup.GetTrackingNumber() != pRenderTask.GetTrackingNumber() ){
		pRenderTask.AddInstanceGroup( &rtiGroup );
	}
	
	deoglRenderTaskInstance *rti = rtvao->GetInstanceForIndex( rtiGroup.GetIndex() );
	if( ! rti ){
		rti = pRenderTask.InstanceFromPool();
		rti->SetGroup( &rtiGroup );
		rti->SetSubInstanceSPB( &spbElement->GetSPB() );
		rti->SetFirstPoint( 0 );
		rti->SetFirstIndex( 0 );
		rti->SetPointCount( 4 );
		rti->SetDoubleSided( true );
		rti->SetPrimitiveType( GL_TRIANGLE_FAN );
		rtvao->AddInstance( rti );
	}
	
	rti->AddSubInstance( spbElement->GetIndex(), billboard.GetSpecialFlags() );
}



void deoglAddToRenderTask::AddDecal( deoglRDecal &decal, int lodLevel ){
	if( ! decal.GetVisible() ){
		return;
	}
	
	deoglSharedVBOBlock * const vboBlock = decal.GetVBOBlock();
	if( ! vboBlock ){
		return;
	}
	
	decal.UpdateUseSkin();
	
	deoglSkinTexture * const skinTexture = decal.GetUseSkinTexture();
	if( pFilterRejectNoSolid( skinTexture ) ){
		return;
	}
	
	// NOTE this works only if decals to not overlap. if they do overlap the adding to the render task
	//      has to be done similar to particles in that a new step is used whenever the texture for
	//      the last step and the texture of the decal are different. this is less optimal but the
	//      only solution to properly respect decal sorting order.
	// TODO add step support to support the NOTE above
	
	// obtain render task vao and add faces
	deoglRenderTaskVAO * const rtvao = pGetTaskVAO( pSkinShaderType, skinTexture,
		decal.GetTUCForShaderType( pSkinShaderType ), vboBlock->GetVBO()->GetVAO() );
	
	const deoglSharedSPBElement * const spbElement = decal.GetSharedSPBElement();
	
	deoglRenderTaskInstanceGroup &rtiGroup = decal.GetRTIGroup();
	if( rtiGroup.GetTrackingNumber() != pRenderTask.GetTrackingNumber() ){
		pRenderTask.AddInstanceGroup( &rtiGroup );
	}
	
	deoglRenderTaskInstance *rti = rtvao->GetInstanceForIndex( rtiGroup.GetIndex() );
	if( ! rti ){
		rti = pRenderTask.InstanceFromPool();
		rti->SetGroup( &rtiGroup );
		rti->SetSubInstanceSPB( &spbElement->GetSPB() );
		rti->SetFirstPoint( vboBlock->GetOffset() );
		rti->SetPointCount( decal.GetPointCount() );
		rti->SetDoubleSided( true );
		rtvao->AddInstance( rti );
	}
	
	rti->AddSubInstance( spbElement->GetIndex(), 0 );
}

void deoglAddToRenderTask::AddDecals( deoglRComponent &component, int lodLevel ){
	const int decalCount = component.GetDecalCount();
	int i;
	
	for( i=0; i<decalCount; i++ ){
		AddDecal( *component.GetDecalAt( i ), lodLevel );
	}
}

void deoglAddToRenderTask::AddDecals( const deoglCollideList &clist ){
	const int componentCount = clist.GetComponentCount();
	int c;
	
	for( c=0; c<componentCount; c++ ){
		const deoglCollideListComponent &clcomponent = *clist.GetComponentAt( c );
		AddDecals( *clcomponent.GetComponent(), clcomponent.GetLODLevel() );
	}
}



void deoglAddToRenderTask::AddPropFieldType( deoglCollideListPropFieldType &clPropFieldType,
deoglRPropFieldType &propFieldType, bool imposters ){
	const int clusterCount = clPropFieldType.GetClusterCount();
	
	if( clusterCount == 0 || ! propFieldType.GetModel() ){
		return;
	}
	
	deoglSkinTexture * const skinTexture = propFieldType.GetUseSkinTexture();
	if( pFilterReject( skinTexture ) ){
		return;
	}
	
	deoglModelLOD &modelLOD = propFieldType.GetModel()->GetLODAt( 0 );
	const deoglModelTexture &modelTex = modelLOD.GetTextureAt( 0 );
	
	const bool doubleSided = modelTex.GetDoubleSided();
	if( pFilterDoubleSided && pDoubleSided != doubleSided ){
		return;
	}
	
	if( pFilterDecal && pDecal != modelTex.GetDecal() ){
		return;
	}
	
	// retrieve the shader and texture units configuration to use
	deoglSPBlockUBO *spbInstance = NULL;
	deoglSkinShader *skinShader = NULL;
	deoglShaderProgram *shader = NULL;
	
	skinShader = skinTexture->GetShaderFor( pSkinShaderType );
	
	if( pEnforceShader ){
		shader = pEnforceShader->GetShader();
		
	}else{
		if( skinShader ){
			shader = skinShader->GetShader();
			
			if( shader ){
				// tuc is specific to the individual clusters
				spbInstance = propFieldType.GetParamBlockFor( pSkinShaderType );
			}
		}
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	// obtain render task. this is the same for all clusters in the type
	deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	deoglSharedVBOBlock &vboBlock = *modelLOD.GetVBOBlock();
	deoglVAO *vao = NULL;
	
	if( imposters ){
		vao = defren.GetVAOBillboard();
		
	}else{
		vao = vboBlock.GetVBO()->GetVAO();
	}
	
	deoglRenderTaskShader &rtshader = *pRenderTask.AddShader( shader->GetRTSShader() );
	
	// the rest is specific for each cluster except for the vao which is also the same for all clusters in the type
	deoglPropFieldCluster ** const clusters = clPropFieldType.GetClusters();
	deoglRenderTaskInstance *renderTaskInstance;
	const bool instanceDoubleSided = doubleSided;
	int i, firstPoint, firstIndex, indexCount, pointCount;
	
	if( imposters ){
		firstPoint = 0;
		firstIndex = 0;
		indexCount = 0;
		pointCount = 6;
		
	}else{
		firstPoint = vboBlock.GetOffset();
		firstIndex = vboBlock.GetIndexOffset() + modelTex.GetFirstFace() * 3;
		indexCount = modelTex.GetFaceCount() * 3;
		pointCount = 0;
	}
	
	for( i=0; i<clusterCount; i++ ){
		// TODO later on clusters are going to have the ability to share TBOs to reduce the number
		//      of TBOs and VBOs reducing also the need for switching between clusters of the same
		//      type. in the current system though for each cluster a new instance is added to the
		//      render task. this is not wrong but could potentially be made better by grouping
		//      sub-instances if they fall onto the same VAO.
		//      for this before adding to a VAO the last instance can be checked if the spbInstance
		//      is the same. if so the last instance belongs to the same prop field type and thus
		//      a new sub-instance can be added (if supported)
		//if( i > 0 ) break; // TEMPORARY
		deoglPropFieldCluster &cluster = *clusters[ i ];
			// => RenderTaskSharedInstance in "cluster"
		
		cluster.UpdateTBOs();
		
		// retrieve the tuc. this is potentially different for clusters as they use shared TBOs
		deoglTexUnitsConfig * const tuc = cluster.GetTUCForShaderType( pSkinShaderType );
		if( ! tuc ){
			DETHROW( deeInvalidParam );
		}
		
		deoglRenderTaskVAO &rtvao = *rtshader.AddTexture( pRenderTask, tuc->GetRTSTexture() )->
			AddVAO( pRenderTask, vao->GetRTSVAO() );
		
		// add an instance for this cluster. all clusters will be handled using sub-instances
		renderTaskInstance = pRenderTask.InstanceFromPool();
		
		renderTaskInstance->SetParameterBlock( spbInstance );
		renderTaskInstance->SetParameterBlockSpecial( NULL );
		renderTaskInstance->SetFirstPoint( firstPoint );
		renderTaskInstance->SetPointCount( pointCount );
		renderTaskInstance->SetFirstIndex( firstIndex );
		renderTaskInstance->SetIndexCount( indexCount );
		renderTaskInstance->SetSubInstanceCount( cluster.GetInstanceCount() );
		renderTaskInstance->SetDoubleSided( instanceDoubleSided );
		renderTaskInstance->SetPrimitiveType( GL_TRIANGLES );
		
		rtvao.AddInstance( renderTaskInstance );
	}
}

void deoglAddToRenderTask::AddPropField( deoglCollideListPropField &clPropField, bool imposters ){
	const int propFieldTypeCount = clPropField.GetTypeCount();
	deoglRPropField &propField = *clPropField.GetPropField();
	int i;
	
	for( i=0; i<propFieldTypeCount; i++ ){
		AddPropFieldType( *clPropField.GetTypeAt( i ), propField.GetTypeAt( i ), imposters );
	}
}

void deoglAddToRenderTask::AddPropFields( const deoglCollideList &clist, bool imposters ){
	if( imposters ) return; // HACK
	const int propFieldCount = clist.GetPropFieldCount();
	int i;
	
	for( i=0; i<propFieldCount; i++ ){
		AddPropField( *clist.GetPropFieldAt( i ), imposters );
	}
}



void deoglAddToRenderTask::AddHeightTerrainSectorClusters( const deoglHTViewSector &sectorView, int texture ){
	deoglRHTSector &sector = sectorView.GetSector();
	
	if( ! sector.GetValid() || ! sector.GetValidTextures() ){
		return;
	}
	
	deoglHTSTexture &httexture = sector.GetTextureAt( texture );
	deoglSkinTexture * const skinTexture = httexture.GetUseSkinTexture();
	if( ! skinTexture || pFilterReject( skinTexture ) ){
		return;
	}
	
	// retrieve the shader and texture units configuration to use
	deoglSPBlockUBO *spbInstance = NULL;
	deoglSkinShader *skinShader = NULL;
	deoglShaderProgram *shader = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	skinShader = skinTexture->GetShaderFor( pSkinShaderType );
	
	if( pEnforceShader ){
		shader = pEnforceShader->GetShader();
		
	}else{
		if( skinShader ){
			shader = skinShader->GetShader();
			
			if( shader ){
				// tuc is specific to the individual clusters
				spbInstance = httexture.GetParamBlockFor( pSkinShaderType );
			}
		}
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	// obtain render task. this is the same for all clusters in the type
	deoglRenderTaskShader &rtshader = *pRenderTask.AddShader( shader->GetRTSShader() );
	
	// retrieve the tuc
	tuc = httexture.GetTUCForShaderType( pSkinShaderType );
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	deoglRenderTaskTexture &rttexture = *rtshader.AddTexture( pRenderTask, tuc->GetRTSTexture() );
	
	// the rest is specific for each cluster
	const int clusterCount = sector.GetClusterCount() * sector.GetClusterCount();
	deoglHTSCluster * const htsclusters = sector.GetClusters();
	sHTVSCluster * const htvsclusters = sectorView.GetClusters();
	deoglRenderTaskInstance *renderTaskInstance;
	int firstPoint, firstIndex, indexCount, i;
	
	for( i=0; i<clusterCount; i++ ){
		if( htvsclusters[ i ].lodLevel >= 0 ){
			const deoglHTSClusterLOD &clod = htsclusters[ i ].GetLODAt( htvsclusters[ i ].lodLevel );
			const int vboOffsetFaces = htsclusters[ i ].GetOffsetVBODataFaces();
			deoglVAO * const vao = htsclusters[ i ].GetVAO();
			
			/*
			// for RTSI usage call
			htsclusters[ i ].UpdateRTSInstances();
			
			// then 5 instance are usable one for the cluster and 4 for the borders.
			// these have to be updated below (no pre-update yet)
			*/
			
			firstPoint = 0;
			firstIndex = vboOffsetFaces + clod.firstBasePoint;
			indexCount = clod.basePointCount;
			
			deoglRenderTaskVAO &rtvao = *rttexture.AddVAO( pRenderTask, vao->GetRTSVAO() );
			
			// add an instance for this cluster
			renderTaskInstance = pRenderTask.InstanceFromPool();
			
			renderTaskInstance->SetParameterBlock( spbInstance );
			renderTaskInstance->SetParameterBlockSpecial( NULL );
			renderTaskInstance->SetFirstPoint( firstPoint );
			renderTaskInstance->SetPointCount( 0 );
			renderTaskInstance->SetFirstIndex( firstIndex );
			renderTaskInstance->SetIndexCount( indexCount );
			renderTaskInstance->SetSubInstanceCount( 0 );
			renderTaskInstance->SetDoubleSided( false );
			renderTaskInstance->SetPrimitiveType( GL_TRIANGLES );
			
			rtvao.AddInstance( renderTaskInstance );
			
			// add borders
			if( htvsclusters[ i ].lodLevel > 0 ){
				int j;
				
				for( j=0; j<4; j++ ){
					const int border = htvsclusters[ i ].borders[ j ];
					
					firstPoint = 0;
					firstIndex = vboOffsetFaces + clod.firstBorderPoint[ border ];
					indexCount = clod.borderPointCount[ border ];
					
					renderTaskInstance = pRenderTask.InstanceFromPool();
					
					renderTaskInstance->SetParameterBlock( spbInstance );
					renderTaskInstance->SetParameterBlockSpecial( NULL );
					renderTaskInstance->SetFirstPoint( firstPoint );
					renderTaskInstance->SetPointCount( 0 );
					renderTaskInstance->SetFirstIndex( firstIndex );
					renderTaskInstance->SetIndexCount( indexCount );
					renderTaskInstance->SetSubInstanceCount( 0 );
					renderTaskInstance->SetDoubleSided( false );
					renderTaskInstance->SetPrimitiveType( GL_TRIANGLES );
					
					rtvao.AddInstance( renderTaskInstance );
				}
			}
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrainSector( const deoglCollideListHTSector &clhtsector, bool firstMask ){
	deoglHTViewSector &viewSector = *clhtsector.GetSector();
	
	const int textureCount = viewSector.GetSector().GetTextureCount();
	if( textureCount == 0 ){
		return;
	}
	
	if( firstMask ){
		AddHeightTerrainSectorClusters( viewSector, 0 );
		
	}else{
		int i;
		for( i=1; i<textureCount; i++ ){
			AddHeightTerrainSectorClusters( viewSector, i );
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrains( const deoglCollideList &clist, bool firstMask ){
	const int sectorCount = clist.GetHTSectorCount();
	int i;
	
	for( i=0; i<sectorCount; i++ ){
		AddHeightTerrainSector( *clist.GetHTSectorAt( i ), firstMask );
	}
}



void deoglAddToRenderTask::AddOcclusionMesh( deoglRComponent &component,
deoglRenderTaskTexture *taskTexture ){
	deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
	if( ! occlusionMesh ){
		return;
	}
	if( pFilterCubeFace != -1 && ! component.GetCubeFaceVisible( pFilterCubeFace ) ){
		return;
	}
	if( pNoRendered && component.GetSkinRendered().GetTexturedCount() > 0 ){
		return;
	}
	
	const int singleFaceCount = occlusionMesh->GetSingleSidedFaceCount();
	const int doubleFaceCount = occlusionMesh->GetDoubleSidedFaceCount();
	if( singleFaceCount == 0 && doubleFaceCount == 0 ){
		return;
	}
	
	if( component.GetDynamicOcclusionMesh() ){
		component.GetDynamicOcclusionMesh()->Prepare();
		
	}else{
		occlusionMesh->GetVBOBlock()->Prepare();
	}
	
	if( singleFaceCount > 0 ){
		AddOcclusionMeshFaces( component, 0, singleFaceCount, false, taskTexture );
	}
	if( doubleFaceCount > 0 ){
		AddOcclusionMeshFaces( component, singleFaceCount, doubleFaceCount, true, taskTexture );
	}
}

void deoglAddToRenderTask::AddOcclusionMeshes( const deoglCollideList &clist ){
	deoglRenderTaskTexture *rttexture = NULL;
	
	if( pRenderTask.GetShaderCount() == 0 ){
		rttexture = pRenderTask.AddShader( pEnforceShader )->AddTexture( pRenderTask,
			pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage()->GetRTSTexture() );
		
	}else{
		rttexture = pRenderTask.GetRootShader()->GetRootTexture();
	}
	
	const int count = clist.GetComponentCount();
	int i;
	for( i=0; i<count; i++ ){
		AddOcclusionMesh( *clist.GetComponentAt( i )->GetComponent(), rttexture );
	}
}

void deoglAddToRenderTask::AddOcclusionMeshFaces( deoglRComponent &component,
int firstFace, int faceCount, bool doubleSided, deoglRenderTaskTexture *taskTexture ){
	if( ! pEnforceShader ){
		DETHROW( deeInvalidParam );
	}
	
	// obtain render task vao object to add instance to
	deoglDynamicOcclusionMesh * const dynoccmesh = component.GetDynamicOcclusionMesh();
	const deoglSharedVBOBlock &block = *component.GetOcclusionMesh()->GetVBOBlock();
	deoglVAO *vao = NULL;
	int pointOffset = 0;
	
	if( dynoccmesh ){
		vao = dynoccmesh->GetVAO();
		
	}else{
		vao = block.GetVBO()->GetVAO();
		pointOffset = block.GetOffset();
	}
	
	deoglRenderTaskVAO &rtvao = *taskTexture->AddVAO( pRenderTask, vao->GetRTSVAO() );
	
	// add to render vao
	deoglSharedSPBElement * const spbElement = component.GetOccMeshSharedSPBElement();
	deoglRenderTaskInstanceGroup &rtiGroup = component.GetOccMeshSharedSPBRTIGroup( doubleSided ).GetGroup();
	if( rtiGroup.GetTrackingNumber() != pRenderTask.GetTrackingNumber() ){
		pRenderTask.AddInstanceGroup( &rtiGroup );
	}
	
	deoglRenderTaskInstance *rti = rtvao.GetInstanceForIndex( rtiGroup.GetIndex() );
	if( ! rti ){
		rti = pRenderTask.InstanceFromPool();
		rti->SetGroup( &rtiGroup );
		rti->SetSubInstanceSPB( &spbElement->GetSPB() );
		rti->SetFirstPoint( pointOffset );
		rti->SetFirstIndex( block.GetIndexOffset() + firstFace * 3 );
		rti->SetIndexCount( faceCount * 3 );
		rti->SetDoubleSided( doubleSided );
		rtvao.AddInstance( rti );
	}
	
	rti->AddSubInstance( spbElement->GetIndex(), component.GetSpecialFlags() );
}



void deoglAddToRenderTask::AddParticles( const deoglCollideList &list ){
	AddParticles( list.GetParticleEmitterList() );
}

void deoglAddToRenderTask::AddParticles( const deoglParticleEmitterInstanceList &list ){
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		AddParticles( *list.GetAt( i ) );
	}
}

void deoglAddToRenderTask::AddParticles( deoglRParticleEmitterInstance &emitter ){
	const int count = emitter.GetTypeCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	
	emitter.ClearIBO();
	for( i=0; i<count; i++ ){
		AddParticles( emitter, emitter.GetTypeAt( i ) );
	}
	emitter.UpdateIBO();
}

void deoglAddToRenderTask::AddParticles( deoglRParticleEmitterInstance &emitter,
deoglRParticleEmitterInstanceType &type ){
	if( pOutline ){
		return;
	}
	
	deoglSkinTexture * const skinTexture = type.GetUseSkinTexture();
	if( ! skinTexture ){
		return;
	}
	
	const deoglRParticleEmitterType &etype = emitter.GetEmitter()->GetTypeAt( type.GetIndex() );
	const bool solid = skinTexture->GetSolid(); //;& ! etype.GetHasTransparency();
	const bool hasHoles = skinTexture->GetHasHoles(); //;& ! etype.GetHasTransparency();
	
	if( pSolid != solid ){
		return;
	}
	if( pFilterHoles && pWithHoles != hasHoles ){
		return;
	}
	if( pNoRendered && skinTexture->GetRendered() ){
		return;
	}
	if( pNoShadowNone && skinTexture->GetShadowNone() ){
		return;
	}
	if( pNoNotReflected && ! skinTexture->GetReflected() ){
		return;
	}
	
	const bool simulationRibbon = ( etype.GetSimulationType() == deParticleEmitterType::estRibbon );
	const bool simulationBeam = ( etype.GetSimulationType() == deParticleEmitterType::estBeam );
	const int particleCount = type.GetParticleCount();
	const bool doubleSided = true; // required for beams and ribbons but not for regular particles
	
	if( simulationRibbon || simulationBeam ){
		if( particleCount < 2 ){
			return;
		}
		
	}else{
		if( particleCount == 0 ){
			return;
		}
	}
	
	// determine the skin shader type to use
	deoglSkinTexture::eShaderTypes skinShaderType = pSkinShaderType;
	
	if( simulationRibbon ){
		skinShaderType = pSkinShaderTypeRibbon;
		
	}else if( simulationBeam ){
		skinShaderType = pSkinShaderTypeBeam;
	}
	
	// update index buffer
	const int firstParticle = type.GetFirstParticle();
	const deoglRParticleEmitterInstance::sParticle * const particles =
		emitter.GetParticles() + firstParticle;
	const int firstIndex = emitter.GetIBOUsedIndexCount();
	int i, indexCount = 0;
	GLenum primitiveType;
	
	if( simulationRibbon || simulationBeam ){
		// for ribbons and beams skip first particle
		for( i=1; i<particleCount; i++ ){
			const deoglRParticleEmitterInstance::sParticle &particle = particles[ i ];
			const int baseIndex = firstParticle + i;
			
			emitter.AddIBOEntries(
				baseIndex + particle.ribbonLine[ 0 ],
				baseIndex + particle.ribbonLine[ 1 ],
				baseIndex,
				baseIndex + particle.ribbonLine[ 2 ] );
		}
		
		indexCount = ( particleCount - 1 ) * 4;
		primitiveType = GL_LINES_ADJACENCY;
		
	}else{
		for( i=0; i<particleCount; i++ ){
			emitter.AddIBOEntry( firstParticle + i );
		}
		
		indexCount = particleCount;
		primitiveType = GL_POINTS;
	}
	
	// obtain render task vao and add particles
	deoglRenderTaskVAO * const rtvao = pGetTaskVAO( skinShaderType, skinTexture,
		type.GetTUCForShaderType( skinShaderType ), emitter.GetVAO() );
	
	// NOTE using RTSInstance for the time beeing has to be updated by hand
	
	deoglRenderTaskInstance * const rti = pRenderTask.InstanceFromPool();
	rti->SetParameterBlock( type.GetParamBlockFor( skinShaderType ) );
	rti->SetFirstIndex( firstIndex );
	rti->SetIndexCount( indexCount );
	rti->SetDoubleSided( doubleSided );
	rti->SetPrimitiveType( primitiveType );
	rtvao->AddInstance( rti );
}



// Private Functions
//////////////////////

bool deoglAddToRenderTask::pFilterReject( const deoglSkinTexture *skinTexture ) const{
	if( pOutline ){
		if( ! skinTexture->GetHasOutline() ){
			return true;
		}
		if( pSolid != skinTexture->GetIsOutlineSolid() ){
			return true;
		}
		
	}else{
		if( pSolid != skinTexture->GetSolid() ){
			return true;
		}
	}
	
	if( pFilterRejectNoSolid( skinTexture ) ){
		return true;
	}
	return false;
}

bool deoglAddToRenderTask::pFilterRejectNoSolid( const deoglSkinTexture *skinTexture ) const{
	if( ! skinTexture ){
		return true;
	}
	if( pFilterHoles && pWithHoles != skinTexture->GetHasHoles() ){
		return true;
	}
	if( pNoRendered && skinTexture->GetRendered() ){
		return true;
	}
	if( pNoShadowNone && skinTexture->GetShadowNone() ){
		return true;
	}
	if( pNoNotReflected && ! skinTexture->GetReflected() ){
		return true;
	}
	return false;
}

deoglRenderTaskVAO *deoglAddToRenderTask::pGetTaskVAO( deoglSkinTexture::eShaderTypes shaderType,
deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc, deoglVAO *vao ) const{
	deoglShaderProgram *shader = NULL;
	if( pEnforceShader ){
		shader = pEnforceShader->GetShader();
	}
	
	if( ! shader ){
		deoglSkinShader * const skinShader = skinTexture->GetShaderFor( shaderType );
		if( skinShader ){
			shader = skinShader->GetShader();
		}
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	return pRenderTask.AddShader( shader->GetRTSShader() )->
		AddTexture( pRenderTask, tuc->GetRTSTexture() )->
		AddVAO( pRenderTask, vao->GetRTSVAO() );
}
