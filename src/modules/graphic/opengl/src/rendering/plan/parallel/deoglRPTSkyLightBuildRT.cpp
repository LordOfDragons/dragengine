/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>

#include "deoglRPTSkyLightBuildRT.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListHTSector.h"
#include "../../../collidelist/deoglCollideListHTSCluster.h"
#include "../../../collidelist/deoglCollideListPropField.h"
#include "../../../collidelist/deoglCollideListPropFieldType.h"
#include "../../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../../rendering/task/deoglRenderTask.h"
#include "../../../rendering/task/deoglAddToRenderTask.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../terrain/heightmap/deoglRHTSector.h"
#include "../../../terrain/heightmap/deoglHTViewSector.h"
#include "../../../terrain/heightmap/deoglHTSTexture.h"

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
	
	try{
		decTimer timer;
		int i;
		
		for( i=pFromLayer; i<=pToLayer; i++ ){
			deoglRenderPlanSkyLight::sShadowLayer &layer = pPlan.GetShadowLayerAt( i );
			deoglAddToRenderTask &addToRenderTask = *layer.addToRenderTask;
			
			pFilter( i );
			
			layer.renderTask->Clear();
			
			addToRenderTask.SetSolid( true );
			addToRenderTask.SetNoShadowNone( true );
			
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
			addToRenderTask.AddComponents( pTempCollideList );
			
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldShadowOrthogonal );
			addToRenderTask.AddPropFields( pTempCollideList, false );
			
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapShadowOrthogonal );
			addToRenderTask.AddHeightTerrains( pTempCollideList, true );
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
}
