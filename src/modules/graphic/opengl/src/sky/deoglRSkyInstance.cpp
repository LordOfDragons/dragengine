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

#include "deoglRSky.h"
#include "deoglRSkyInstance.h"
#include "deoglRSkyInstanceLayer.h"
#include "deoglSkyLayerTracker.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../world/deoglRWorld.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class deoglRSkyInstance
////////////////////

// Constructor, destructor
////////////////////////////

deoglRSkyInstance::deoglRSkyInstance( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pParentWorld( NULL ),
pRSky( NULL ),
pOrder( 0 ),
pControllerStates( NULL ),
pControllerStateCount( 0 ),
pLayers( NULL ),
pLayerCount( 0 ),
pEnvMapTimer( 0.0f ),
pSkyNeedsUpdate( false ),
pWorldMarkedRemove( false )
{
	LEAK_CHECK_CREATE( renderThread, SkyInstance );
}

deoglRSkyInstance::~deoglRSkyInstance(){
	LEAK_CHECK_FREE( pRenderThread, SkyInstance );
	
	if( pLayers ){
		while( pLayerCount > 0 ){
			delete pLayers[ --pLayerCount ];
		}
		delete [] pLayers;
	}
	
	if( pControllerStates ){
		delete [] pControllerStates;
	}
	
	if( pRSky ){
		pRSky->FreeReference();
	}
}



// Management
///////////////

void deoglRSkyInstance::SetParentWorld( deoglRWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	DropAllGIStates();
	
	pParentWorld = world;
	
	pSkyNeedsUpdate = true;
}



void deoglRSkyInstance::SetRSky( deoglRSky *rsky ){
	// called during synchronization time
	if( rsky == pRSky ){
		return;
	}
	
	if( pRSky ){
		pRSky->FreeReference();
	}
	
	pRSky = rsky;
	
	if( rsky ){
		rsky->AddReference();
	}
	
	RebuildLayers();
}



void deoglRSkyInstance::SetOrder( int order ){
	if( order == pOrder ){
		return;
	}
	
	pOrder = order;
	
	if( pParentWorld ){
		pParentWorld->MarkSkyOrderDirty();
	}
}

void deoglRSkyInstance::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



float deoglRSkyInstance::GetControllerStateAt( int index ) const{
	if( index < 0 || index >= pControllerStateCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllerStates[ index ];
}

void deoglRSkyInstance::UpdateControllerStates( const deSkyInstance &instance ){
	const int count = instance.GetControllerCount();
	
	if( count != pControllerStateCount ){
		if( pControllerStates ){
			delete [] pControllerStates;
			pControllerStates = NULL;
			pControllerStateCount = 0;
		}
		
		if( count > 0 ){
			pControllerStates = new float[ count ];
			pControllerStateCount = count;
		}
	}
	
	int i;
	for( i=0; i<count; i++ ){
		const deSkyController &controller = instance.GetControllerAt( i );
		const float minimum = controller.GetMinimumValue();
		const float range = controller.GetMaximumValue() - minimum;
		
		if( range > FLOAT_SAFE_EPSILON ){
			pControllerStates[ i ] = ( controller.GetCurrentValue() - minimum ) / range;
			
		}else{
			pControllerStates[ i ] = minimum;
		}
	}
}



deoglRSkyInstanceLayer &deoglRSkyInstance::GetLayerAt( int index ) const{
	if( index < 0 || index >= pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return *pLayers[ index ];
}

void deoglRSkyInstance::RebuildLayers(){
	pSkyNeedsUpdate = true;
	
	if( pLayers ){
		while( pLayerCount > 0 ){
			delete pLayers[ --pLayerCount ];
		}
		delete [] pLayers;
		pLayers = NULL;
		pLayerCount = 0;
	}
	
	if( ! pRSky ){
		return;
	}
	
	const int layerCount = pRSky->GetLayerCount();
	if( layerCount == 0 ){
		return;
	}
	
	pLayers = new deoglRSkyInstanceLayer*[ layerCount ];
	for( pLayerCount=0; pLayerCount<layerCount; pLayerCount++ ){
		pLayers[ pLayerCount ] = new deoglRSkyInstanceLayer( *this, pLayerCount );
	}
}

void deoglRSkyInstance::UpdateLayers(){
	int i;
	
	pTotalSkyLightIntensity = 0.0f;
	pTotalSkyAmbientIntensity = 0.0f;
	pTotalSkyLightColor.SetZero();
	
	for( i=0; i<pLayerCount; i++ ){
		pLayers[ i ]->Update();
		
		const float ambientIntensity = pLayers[ i ]->GetAmbientIntensity();
		const float lightIntensity = ambientIntensity + pLayers[ i ]->GetLightIntensity();
		
		pTotalSkyLightIntensity += lightIntensity;
		pTotalSkyAmbientIntensity += ambientIntensity;
		pTotalSkyLightColor += pLayers[ i ]->GetLightColor() * lightIntensity;
		
		pSkyNeedsUpdate |= pLayers[ i ]->GetSkyNeedsUpdate();
	}
	
	if( pTotalSkyLightIntensity > FLOAT_SAFE_EPSILON ){
		pTotalSkyLightColor /= pTotalSkyLightIntensity;
		pTotalSkyLightColor.a = 1.0f;
	}
	
	for( i=0; i<pLayerCount; i++ ){
		pLayers[ i ]->UpdateWithModifiers();
	}
}



void deoglRSkyInstance::SetEnvironmentMapTimer( float timer ){
	pEnvMapTimer = timer;
}



void deoglRSkyInstance::PrepareForRender(){
	// TODO dynamic skin using update timer
}



void deoglRSkyInstance::NotifySkyChanged(){
	if( pSkyNeedsUpdate && pParentWorld ){
		pParentWorld->EnvMapsNotifySkyChanged();
	}
	
	pSkyNeedsUpdate = false;
}

void deoglRSkyInstance::DropGIState( const deoglGIState *giState ){
	int i;
	for( i=0; i<pLayerCount; i++ ){
		pLayers[ i ]->RemoveAllGICascades( *giState );
	}
}

void deoglRSkyInstance::DropAllGIStates(){
	int i;
	for( i=0; i<pLayerCount; i++ ){
		pLayers[ i ]->RemoveAllGICascades();
	}
}



void deoglRSkyInstance::PrepareQuickDispose(){
	pParentWorld = NULL;
}

void deoglRSkyInstance::NotifyUpdateStaticComponent( deoglRComponent *component ){
	int i;
	for( i=0; i<pLayerCount; i++ ){
		pLayers[ i ]->NotifyUpdateStaticComponent( component );
	}
}



// Render world usage
///////////////////////

void deoglRSkyInstance::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}
