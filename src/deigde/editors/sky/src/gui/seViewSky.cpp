/* 
 * Drag[en]gine IGDE Sky Editor
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
#include <stdlib.h>
#include <string.h>

#include "seViewSky.h"
#include "seWindowMain.h"
#include "../configuration/seConfiguration.h"
#include "../sky/seSky.h"

#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>


// Events
///////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	seViewSky &pView;
	
public:
	cCameraInteraction( seViewSky &view ) : pView( view ){
		SetEnabledAll( false );
		SetEnableRotate( true );
	}
	
public:
	virtual void OnCameraChanged(){
		if( pView.GetSky() ){
			pView.GetSky()->NotifyCameraChanged();
		}
	}
};

}


// Class seViewSky
////////////////////

// Constructor, destructor
////////////////////////////

seViewSky::seViewSky( seWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pSky( NULL )
{
	pCameraInteraction.TakeOver( new cCameraInteraction( *this ) );
	
	AddListener( pCameraInteraction );
}

seViewSky::~seViewSky(){
	SetSky( NULL );
}



// Management
///////////////

void seViewSky::ResetView(){
}

void seViewSky::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	pCameraInteraction->SetCamera( NULL );
	SetRenderWorld( NULL );
	
	if( pSky ){
		pSky->FreeReference();
	}
	
	pSky = sky;
	
	if( sky ){
		sky->AddReference();
		SetRenderWorld( sky->GetCamera()->GetEngineCamera() );
		pCameraInteraction->SetCamera( sky->GetCamera() );
	}
}



void seViewSky::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pSky ){
		pSky->Update( elapsed );
	}
}

void seViewSky::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( pSky ){
		SetRenderWorld( pSky->GetCamera()->GetEngineCamera() );
	}
}
