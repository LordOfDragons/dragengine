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

#include <stdio.h>
#include <string.h>

#include "deoglRPTSkyLightBuildRT.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../light/deoglRenderLight.h"
#include "../../light/deoglRenderLightSky.h"
#include "../../task/deoglRenderTaskTexture.h"
#include "../../task/shared/deoglRenderTaskSharedTexture.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListHTSector.h"
#include "../../../collidelist/deoglCollideListHTSCluster.h"
#include "../../../collidelist/deoglCollideListPropField.h"
#include "../../../collidelist/deoglCollideListPropFieldType.h"
#include "../../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../../rendering/lod/deoglLODCalculator.h"
#include "../../../rendering/task/deoglRenderTask.h"
#include "../../../rendering/task/deoglAddToRenderTask.h"
#include "../../../rendering/task/deoglRenderTaskPipeline.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../renderthread/deoglRTRenderers.h"
#include "../../../renderthread/deoglRTShader.h"
#include "../../../terrain/heightmap/deoglRHTSector.h"
#include "../../../terrain/heightmap/deoglHTViewSector.h"
#include "../../../terrain/heightmap/deoglHTSTexture.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../../shaders/deoglShaderProgram.h"

#include <dragengine/common/exceptions.h>


// Class deoglRPTSkyLightBuildRT
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTSkyLightBuildRT::deoglRPTSkyLightBuildRT( deoglRenderPlanSkyLight &plan,
deoglCollideList &tempCollideList, int fromLayer, int toLayer ) :
deParallelTask( &plan.GetPlan().GetRenderThread().GetOgl() ),
pPlan( plan ),
pTempCollideList( tempCollideList ),
pFromLayer( fromLayer ),
pToLayer( toLayer ),
pElapsedTime( 0.0f )
{
	SetMarkFinishedAfterRun( true );
}

deoglRPTSkyLightBuildRT::~deoglRPTSkyLightBuildRT(){
}



// Management
///////////////

void deoglRPTSkyLightBuildRT::Run(){
	if( IsCancelled() ){
		return;
	}
	
	const int shadowMapSize = pPlan.GetPlan().GetShadowSkySize();
	
	deoglLODCalculator lodCalculator;
	lodCalculator.SetMaxPixelError( 2 );
	
	try{
		const deoglPipeline * const pipeline = pPlan.GetPlan().GetRenderThread().
			GetRenderers().GetLight().GetRenderLightSky().GetPipelineOccMesh();
		decTimer timer;
		int i;
		
		for( i=pFromLayer; i<=pToLayer; i++ ){
			const deoglRenderPlanSkyLight::sShadowLayer &layer = pPlan.GetShadowLayerAt( i );
			deoglAddToRenderTask &addToRenderTask = *layer.addToRenderTask;
			
			pFilter( i );
			
			lodCalculator.SetComponentLODOrtho( pTempCollideList,
				layer.maxExtend.x - layer.minExtend.x, layer.maxExtend.y - layer.minExtend.y,
				shadowMapSize, shadowMapSize );
			
			layer.renderTask->Clear();
			
			addToRenderTask.SetSolid( true );
			addToRenderTask.SetNoShadowNone( true );
			addToRenderTask.SetForceDoubleSided( true );
			addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etShadowOrthogonal );
			
			// we render only double sided occlusion meshes here since for single sided
			// we can not be sure from what side the camera sees them in the shadow map.
			// only double sided occlusion meshes are guaranteed to work correctly.
			// this allows to speed up rendering in the majority of situations
			addToRenderTask.AddOcclusionMeshes( pTempCollideList, nullptr, pipeline );
			
			addToRenderTask.AddComponents( pTempCollideList );
			addToRenderTask.AddPropFields( pTempCollideList, false );
			addToRenderTask.AddPropFieldClusters( pTempCollideList, false );
			addToRenderTask.AddHeightTerrains( pTempCollideList, true );
			addToRenderTask.AddHeightTerrainSectorClusters( pTempCollideList, true );
		}
		
		pElapsedTime = timer.GetElapsedTime();
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Signal();
		throw;
	}
	
	pSemaphore.Signal();
}

void deoglRPTSkyLightBuildRT::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTSkyLightBuildRT::GetDebugName() const{
	return "RPTSkyLightBuildRT";
}




// Private Functions
//////////////////////

void deoglRPTSkyLightBuildRT::pFilter( int layerIndex ){
	const deoglCollideList &collideList = pPlan.GetCollideList();
	const int cascadeMask = 1 << layerIndex;
	int i, j, k;
	
	pTempCollideList.Clear();
	
	// components
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		const deoglCollideListComponent &clcomponent = *collideList.GetComponentAt( i );
		if( ( clcomponent.GetCascadeMask() & cascadeMask ) == cascadeMask ){
			pTempCollideList.AddComponent( clcomponent.GetComponent() );
		}
	}
	
	// height terrain
	const int htsectorCount = collideList.GetHTSectorCount();
	for( i=0; i<htsectorCount; i++ ){
		const deoglCollideListHTSector &sector = *collideList.GetHTSectorAt( i );
		const int clusterCount = sector.GetClusterCount();
		
		deoglCollideListHTSector *addSector = NULL;
		for( j=0; j<clusterCount; j++ ){
			const deoglCollideListHTSCluster &cluster = sector.GetClusterAt( j );
			if( ( cluster.GetCascadeMask() & cascadeMask ) == cascadeMask ){
				if( ! addSector ){
					addSector = pTempCollideList.AddHTSector( sector.GetSector() );
				}
				addSector->AddCluster( cluster.GetCoordinates() );
			}
		}
	}
	
	const int htsclusterCount = collideList.GetHTSClusterCount();
	for( i=0; i<htsclusterCount; i++ ){
		const deoglCollideListHTSCluster &cluster = *collideList.GetHTSClusterAt( i );
		if( ( cluster.GetCascadeMask() & cascadeMask ) == cascadeMask ){
			pTempCollideList.AddHTSCluster( cluster.GetCluster() );
		}
	}
	
	// prop fields
	const int propfieldCount = collideList.GetPropFieldCount();
	for( i=0; i<propfieldCount; i++ ){
		const deoglCollideListPropField &propfield = *collideList.GetPropFieldAt( i );
		const int typeCount = propfield.GetTypeCount();
		
		deoglCollideListPropField *addPropField = NULL;
		for( j=0; j<typeCount; j++ ){
			const deoglCollideListPropFieldType &type = *propfield.GetTypeAt( j );
			const int clusterCount = type.GetClusterCount();
			
			deoglCollideListPropFieldType *addType = NULL;
			for( k=0; k<clusterCount; k++ ){
				const deoglCollideListPropFieldCluster &cluster = type.GetClusterAt( k );
				if( ( cluster.GetCascadeMask() & cascadeMask ) == cascadeMask ){
					if( ! addPropField ){
						addPropField = pTempCollideList.AddPropField( propfield.GetPropField() );
					}
					if( ! addType ){
						addType = addPropField->AddType( type.GetType() );
					}
					addType->AddCluster( cluster.GetCluster() );
				}
			}
		}
	}
	
	const int propfieldClusterCount = collideList.GetPropFieldClusterCount();
	for( i=0; i<propfieldClusterCount; i++ ){
		const deoglCollideListPropFieldCluster &cluster = *collideList.GetPropFieldClusterAt( i );
		if( ( cluster.GetCascadeMask() & cascadeMask ) == cascadeMask ){
			pTempCollideList.AddPropFieldCluster( cluster.GetCluster() );
		}
	}
}
