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
#include "shared/deoglRenderTaskSharedInstance.h"
#include "../defren/deoglDeferredRendering.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListHTSector.h"
#include "../../collidelist/deoglCollideListHTSCluster.h"
#include "../../collidelist/deoglCollideListPropField.h"
#include "../../collidelist/deoglCollideListPropFieldType.h"
#include "../../collidelist/deoglCollideListPropFieldCluster.h"
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
#include "../../terrain/heightmap/deoglHTViewSectorCluster.h"
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

void deoglAddToRenderTask::AddComponent( const deoglRComponent &component, int lodLevel ){
	if( ! component.GetParentWorld() || ! component.GetModel() ){
		return;
	}
	if( pFilterCubeFace != -1 && ! component.GetCubeFaceVisible( pFilterCubeFace ) ){
		return;
	}
	
	#ifdef SPECIAL_DEBUG_ON
	const deoglRModel &model = *component.GetModel();
	const deoglModelLOD &modelLOD = model.GetLODAt( lodLevel );
	#endif
	
	const int count = component.GetTextureCount();
	int i;
	
	for( i=0; i<count; i++ ){
		#ifdef SPECIAL_DEBUG_ON
		const deoglModelTexture &texture = modelLOD.GetTextureAt( i );
		if( texture.GetFaceCount() > 0 ){
			decTimer timer;
			AddComponentFaces( component, i, lodLevel );
			debug1 += timer.GetElapsedTime(); debug1b++;
		}
		#else
		AddComponentFaces( component, i, lodLevel );
		#endif
	}
}

void deoglAddToRenderTask::AddComponent( const deoglCollideListComponent &clcomponent ){
	AddComponent( *clcomponent.GetComponent(), clcomponent.GetLODLevel() );
}

void deoglAddToRenderTask::AddComponents( const deoglCollideList &clist ){
	const int count = clist.GetComponentCount();
	int i;
	
	#ifdef SPECIAL_DEBUG_ON
	decTimer timer;
	debug1 = 0.0f; debug1b = 0;
	#endif
	for( i=0; i<count; i++ ){
		AddComponent( *clist.GetComponentAt( i ) );
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

void deoglAddToRenderTask::AddComponentFaces( const deoglRComponent &component, int texture, int lodLevel ){
	if( ! component.GetModel() ){
		return;
	}
	
	const deoglModelTexture &modelTexture = component.GetModel()->GetLODAt( lodLevel ).GetTextureAt( texture );
	if( modelTexture.GetFaceCount() == 0 ){
		return;
	}
	
	const deoglRComponentTexture &componentTexture = component.GetTextureAt( texture );
	const deoglSkinTexture * const skinTexture = componentTexture.GetUseSkinTexture();
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
	const deoglSkinChannel *skinChannel = skinTexture->GetChannelAt( deoglSkinChannel::ectColor );
	const deoglSkinState * const useSkinState = componentTexture.GetUseSkinState();
	
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
	pGetTaskVAO( pSkinShaderType, skinTexture,
		componentTexture.GetTUCForShaderType( pSkinShaderType ), component.GetVAO( lodLevel ) )->
			AddInstance( pRenderTask, componentTexture.GetSharedSPBRTIGroup( lodLevel ).GetRTSInstance() )->
			AddSubInstance( componentTexture.GetSharedSPBElement()->GetIndex(), component.GetSpecialFlags() );
}



void deoglAddToRenderTask::AddBillboards( const deoglCollideList &clist ){
	const int count = clist.GetBillboardCount();
	int i;
	
	for( i=0; i<count; i++ ){
		AddBillboard( *clist.GetBillboardAt( i ) );
	}
}

void deoglAddToRenderTask::AddBillboard( const deoglRBillboard &billboard ){
	if( ! billboard.GetParentWorld() || ! billboard.GetSkin() ){
		return;
	}
	if( pFilterCubeFace != -1 && ! billboard.GetCubeFaceVisible( pFilterCubeFace ) ){
		return;
	}
	
	const deoglSkinTexture &texture = billboard.GetSkin()->GetTextureAt( 0 );
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
	const deoglSkinChannel *skinChannel = texture.GetChannelAt( deoglSkinChannel::ectColor );
	const deoglSkinState * const skinState = billboard.GetSkinState();
	
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
	pGetTaskVAO( pSkinShaderType, &texture, billboard.GetTUCForShaderType( pSkinShaderType ),
		pRenderThread.GetDeferredRendering().GetVAOBillboard() )->
			AddInstance( pRenderTask, billboard.GetSharedSPBRTIGroup().GetRTSInstance() )->
			AddSubInstance( billboard.GetSharedSPBElement()->GetIndex(), billboard.GetSpecialFlags() );
}



void deoglAddToRenderTask::AddDecal( const deoglRDecal &decal, int lodLevel ){
	if( ! decal.GetVisible() ){
		return;
	}
	
	const deoglSharedVBOBlock * const vboBlock = decal.GetVBOBlock();
	if( ! vboBlock ){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = decal.GetUseSkinTexture();
	if( pFilterRejectNoSolid( skinTexture ) ){
		return;
	}
	
	// NOTE this works only if decals to not overlap. if they do overlap the adding to the render task
	//      has to be done similar to particles in that a new step is used whenever the texture for
	//      the last step and the texture of the decal are different. this is less optimal but the
	//      only solution to properly respect decal sorting order.
	// TODO add step support to support the NOTE above
	
	pGetTaskVAO( pSkinShaderType, skinTexture,
		decal.GetTUCForShaderType( pSkinShaderType ), vboBlock->GetVBO()->GetVAO() )->
			AddInstance( pRenderTask, decal.GetRTSInstance() )->
			AddSubInstance( decal.GetSharedSPBElement()->GetIndex(), 0 );
}

void deoglAddToRenderTask::AddDecals( const deoglRComponent &component, int lodLevel ){
	const int count = component.GetDecalCount();
	int i;
	
	for( i=0; i<count; i++ ){
		AddDecal( *component.GetDecalAt( i ), lodLevel );
	}
}

void deoglAddToRenderTask::AddDecals( const deoglCollideList &clist ){
	const int count = clist.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglCollideListComponent &clcomponent = *clist.GetComponentAt( i );
		AddDecals( *clcomponent.GetComponent(), clcomponent.GetLODLevel() );
	}
}



void deoglAddToRenderTask::AddPropFieldType( const deoglCollideListPropFieldType &clPropFieldType,
const deoglRPropFieldType &propFieldType, bool imposters ){
	const int clusterCount = clPropFieldType.GetClusterCount();
	if( clusterCount == 0 || ! propFieldType.GetModel() ){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = propFieldType.GetUseSkinTexture();
	if( pFilterReject( skinTexture ) ){
		return;
	}
	
	const deoglModelLOD &modelLOD = propFieldType.GetModel()->GetLODAt( 0 );
	const deoglModelTexture &modelTex = modelLOD.GetTextureAt( 0 );
	
	const bool doubleSided = modelTex.GetDoubleSided();
	if( pFilterDoubleSided && pDoubleSided != doubleSided ){
		return;
	}
	
	if( pFilterDecal && pDecal != modelTex.GetDecal() ){
		return;
	}
	
	// retrieve the shader and texture units configuration to use
	const deoglSkinShader *skinShader = skinTexture->GetShaderFor( pSkinShaderType );
	const deoglShaderProgram *shader = NULL;
	
	if( pEnforceShader ){
		shader = pEnforceShader->GetShader();
		
	}else if( skinShader ){
		shader = skinShader->GetShader();
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	// obtain render task. this is the same for all clusters in the type
	const deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	const deoglVAO *vao = NULL;
	
	if( imposters ){
		vao = defren.GetVAOBillboard();
		
	}else{
		vao = modelLOD.GetVBOBlock()->GetVBO()->GetVAO();
	}
	
	deoglRenderTaskShader &rtshader = *pRenderTask.AddShader( shader->GetRTSShader() );
	
	// the rest is specific for each cluster except for the vao which is also the same
	// for all clusters in the type
	int i;
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
		const deoglPropFieldCluster &cluster = *clPropFieldType.GetClusterAt( i ).GetCluster();
		
		// retrieve the tuc. this is potentially different for clusters as they use shared TBOs
		const deoglTexUnitsConfig * const tuc = cluster.GetTUCForShaderType( pSkinShaderType );
		if( ! tuc ){
			DETHROW( deeInvalidParam );
		}
		
		rtshader.AddTexture( pRenderTask, tuc->GetRTSTexture() )->
			AddVAO( pRenderTask, vao->GetRTSVAO() )->
			AddInstance( pRenderTask, cluster.GetRTSInstance() );
	}
}

void deoglAddToRenderTask::AddPropField( const deoglCollideListPropField &clPropField, bool imposters ){
	const deoglRPropField &propField = *clPropField.GetPropField();
	const int count = clPropField.GetTypeCount();
	int i;
	
	for( i=0; i<count; i++ ){
		AddPropFieldType( *clPropField.GetTypeAt( i ), propField.GetTypeAt( i ), imposters );
	}
}

void deoglAddToRenderTask::AddPropFields( const deoglCollideList &clist, bool imposters ){
	if( imposters ) return; // HACK
	const int count = clist.GetPropFieldCount();
	int i;
	
	for( i=0; i<count; i++ ){
		AddPropField( *clist.GetPropFieldAt( i ), imposters );
	}
}



void deoglAddToRenderTask::AddHeightTerrainSectorClusters(
const deoglCollideListHTSector &clhtsector, int texture ){
	const deoglHTViewSector &htvsector = *clhtsector.GetSector();
	const deoglRHTSector &sector = htvsector.GetSector();
	if( ! sector.GetValid() || ! sector.GetValidTextures() ){
		return;
	}
	
	const deoglHTSTexture &httexture = sector.GetTextureAt( texture );
	const deoglSkinTexture * const skinTexture = httexture.GetUseSkinTexture();
	if( ! skinTexture || pFilterReject( skinTexture ) ){
		return;
	}
	
	// retrieve the shader and texture units configuration to use
	const deoglSkinShader * const skinShader = skinTexture->GetShaderFor( pSkinShaderType );
	const deoglShaderProgram *shader = NULL;
	
	if( pEnforceShader ){
		shader = pEnforceShader->GetShader();
		
	}else if( skinShader ){
		shader = skinShader->GetShader();
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	// obtain render task. this is the same for all clusters in the type
	deoglRenderTaskShader &rtshader = *pRenderTask.AddShader( shader->GetRTSShader() );
	
	// retrieve tuc
	const deoglTexUnitsConfig *tuc = httexture.GetTUCForShaderType( pSkinShaderType );
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	deoglRenderTaskTexture &rttexture = *rtshader.AddTexture( pRenderTask, tuc->GetRTSTexture() );
	
	// the rest is specific for each cluster
	const deoglHTSCluster * const htsclusters = sector.GetClusters();
	const int clusterCount = clhtsector.GetClusterCount();
	int i, j;
	
	for( i=0; i<clusterCount; i++ ){
		const deoglCollideListHTSCluster &clhtscluster = clhtsector.GetClusterAt( i );
		const deoglHTViewSectorCluster &htvscluster = htvsector.GetClusterAt( clhtscluster.GetIndex() );
		if( htvscluster.GetLodLevel() < 0 ){
			continue;
		}
		
		deoglRenderTaskVAO &rtvao = *rttexture.AddVAO( pRenderTask,
			htsclusters[ clhtscluster.GetIndex() ].GetVAO()->GetRTSVAO() );
		
		rtvao.AddInstance( pRenderTask, htvscluster.GetRTSInstanceAt( texture, 0 ) );
		
		if( htvscluster.GetLodLevel() > 0 ){
			for( j=1; j<5; j++ ){
				rtvao.AddInstance( pRenderTask, htvscluster.GetRTSInstanceAt( texture, j ) );
			}
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrainSector( const deoglCollideListHTSector &clhtsector, bool firstMask ){
	const int textureCount = clhtsector.GetSector()->GetSector().GetTextureCount();
	if( textureCount == 0 ){
		return;
	}
	
	if( firstMask ){
		AddHeightTerrainSectorClusters( clhtsector, 0 );
		
	}else{
		int i;
		for( i=1; i<textureCount; i++ ){
			AddHeightTerrainSectorClusters( clhtsector, i );
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



void deoglAddToRenderTask::AddOcclusionMesh( const deoglRComponent &component,
deoglRenderTaskTexture *taskTexture ){
	const deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
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
	
	if( singleFaceCount > 0 ){
		AddOcclusionMeshFaces( component, false, taskTexture );
	}
	if( doubleFaceCount > 0 ){
		AddOcclusionMeshFaces( component, true, taskTexture );
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

void deoglAddToRenderTask::AddOcclusionMeshFaces( const deoglRComponent &component,
bool doubleSided, deoglRenderTaskTexture *taskTexture ){
	if( ! pEnforceShader ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglDynamicOcclusionMesh * const dynoccmesh = component.GetDynamicOcclusionMesh();
	const deoglVAO *vao = NULL;
	
	if( dynoccmesh ){
		vao = dynoccmesh->GetVAO();
		
	}else{
		vao = component.GetOcclusionMesh()->GetVBOBlock()->GetVBO()->GetVAO();
	}
	
	taskTexture->AddVAO( pRenderTask, vao->GetRTSVAO() )->
		AddInstance( pRenderTask, component.GetOccMeshSharedSPBRTIGroup( doubleSided ).GetRTSInstance() )->
		AddSubInstance( component.GetOccMeshSharedSPBElement()->GetIndex(), component.GetSpecialFlags() );
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
	const deoglRParticleEmitterInstance::sParticle * const particles = emitter.GetParticles() + firstParticle;
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
	deoglRenderTaskVAO &rtvao = *pGetTaskVAO( skinShaderType, skinTexture,
		type.GetTUCForShaderType( skinShaderType ), emitter.GetVAO() );
	
	// NOTE using RTSInstance for the time beeing has to be updated by hand
	
	deoglRenderTaskSharedInstance &instance = *type.GetRTSInstance();
	instance.SetParameterBlock( type.GetParamBlock() );
	instance.SetFirstIndex( firstIndex );
	instance.SetIndexCount( indexCount );
	instance.SetDoubleSided( doubleSided );
	instance.SetPrimitiveType( primitiveType );
	
	rtvao.AddInstance( pRenderTask, &instance );
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
const deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc, deoglVAO *vao ) const{
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
