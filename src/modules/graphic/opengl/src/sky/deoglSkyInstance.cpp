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

#include "deoglRSkyInstance.h"
#include "deoglSky.h"
#include "deoglSkyInstance.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>



// Class deoglSkyInstance
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkyInstance::deoglSkyInstance( deGraphicOpenGl &ogl, const deSkyInstance &instance ) :
pOgl( ogl ),
pInstance( instance ),
pRInstance( NULL ),
pParentWorld( NULL ),
pOglSky( NULL ),
pEnvMapTimer( 0.0f ),
pDirtySky( true ),
pDirtyControllers( true ),
pDirtyLayerMask( true ),
pSkyUpdateState( 0 )
{
	pRInstance = new deoglRSkyInstance( pOgl.GetRenderThread() );
}

deoglSkyInstance::~deoglSkyInstance(){
	if( pRInstance ){
		pRInstance->FreeReference();
	}
}



// Management
///////////////

void deoglSkyInstance::SyncToRender(){
	if( pDirtySky ){
		pOglSky = NULL;
		if( pInstance.GetSky() ){
			pOglSky = ( deoglSky* )pInstance.GetSky()->GetPeerGraphic();
		}
		
		if( pOglSky ){
			pOglSky->SyncToRender();
			pSkyUpdateState = pOglSky->GetUpdateTracker();
			
			pRInstance->SetRSky( pOglSky->GetRSky() );
			
		}else{
			pRInstance->SetRSky( NULL );
		}
		
		pRInstance->SetOrder( pInstance.GetOrder() );
		
		pDirtySky = false;
		pDirtyControllers = true;
	}
	
	if( pOglSky && pOglSky->GetUpdateTracker() != pSkyUpdateState ){
		pSkyUpdateState = pOglSky->GetUpdateTracker();
		
		pOglSky->SyncToRender();
		pRInstance->RebuildLayers();
		
		pDirtyControllers = true;
	}
	
	pRInstance->SetEnvironmentMapTimer( pEnvMapTimer );
	
	if( pDirtyControllers ){
		pRInstance->UpdateControllerStates( pInstance );
		pRInstance->UpdateLayers();
	}
	
	if( pDirtyLayerMask ){
		pRInstance->SetLayerMask( pInstance.GetLayerMask() );
		pDirtyLayerMask = false;
	}
	
	pRInstance->NotifySkyChanged();
}



// Notifications
//////////////////

void deoglSkyInstance::Update( float elapsed ){
	pEnvMapTimer += elapsed;
	// Update dynamic skins: TODO
}



void deoglSkyInstance::SetParentWorld( deoglWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	pParentWorld = parentWorld;
}



void deoglSkyInstance::SkyChanged(){
	pDirtySky = true;
}

void deoglSkyInstance::OrderChanged(){
	pDirtySky = true;
}

void deoglSkyInstance::ControllerChanged( int ){
	pDirtyControllers = true;
}

void deoglSkyInstance::LayerMaskChanged(){
	pDirtyLayerMask = true;
}
