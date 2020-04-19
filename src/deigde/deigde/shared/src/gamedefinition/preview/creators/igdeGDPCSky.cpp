/* 
 * Drag[en]gine IGDE
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

#include "igdeGDPCSky.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/sky/igdeGDSky.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../gui/wrapper/igdeWSky.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Class igdeGDPCSky
//////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPCSky::igdeGDPCSky( igdeEnvironment &environment, igdeGDSky *gdsky, const decPoint &size ) :
igdeGDPreviewCreator( environment, size ),
pGDSky( gdsky ),
pSky( NULL )
{
	if( ! gdsky ){
		DETHROW( deeInvalidParam );
	}
}

igdeGDPCSky::~igdeGDPCSky(){
	if( pSky ){
		delete pSky;
	}
}



// Management
///////////////

decString igdeGDPCSky::DebugPrefix(){
	return decString( "GDPCSky(" ) + ( pGDSky ? pGDSky->GetName() : "?" ) + ")";
}

void igdeGDPCSky::PrepareCanvasForRender(){
	igdeEnvironment &environment = GetEnvironment();
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	// create world
	pWorld.TakeOver( engine.GetWorldManager()->CreateWorld() );
	
	// create camera
	pCamera.TakeOver( engine.GetCameraManager()->CreateCamera() );
	pCamera->SetFov( 45.0f * DEG2RAD );
	pCamera->SetFovRatio( 1.0f );
	pCamera->SetImageDistance( 0.01f );
	pCamera->SetViewDistance( 500.0f );
	pCamera->SetExposure( 1.0f );
	pCamera->SetAdaptionTime( 0.0f );
	pWorld->AddCamera( pCamera );
	
	// create sky
	pSky = new igdeWSky( environment );
	pSky->SetWorld( pWorld );
	pSky->SetGDSky( pGDSky );
	
	// adjust camera parameters to fit the sky lighting
	pCamera->SetLowestIntensity( pSky->GetMaxLightIntensity() );
	pCamera->SetHighestIntensity( pSky->GetMaxLightIntensity() );
	
	// create render world canvas
	deCanvasView &container = *GetCanvas();
	pCanvasRenderWorld.TakeOver( engine.GetCanvasManager()->CreateCanvasRenderWorld() );
	pCanvasRenderWorld->SetCamera( pCamera );
	pCanvasRenderWorld->SetSize( container.GetSize() );
	container.AddCanvas( pCanvasRenderWorld );
}

bool igdeGDPCSky::IsCanvasReadyForRender(){
	pWorld->Update( 0.0f );
	return true;
}

void igdeGDPCSky::UpdateCanvasForRender( float elapsed ){
	pWorld->Update( elapsed );
}
