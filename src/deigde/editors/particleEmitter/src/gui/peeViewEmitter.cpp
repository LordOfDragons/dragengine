/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeViewEmitter.h"
#include "peeWindowMain.h"
#include "../configuration/peeConfiguration.h"
#include "../emitter/peeEmitter.h"

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
	peeViewEmitter &pView;
	
public:
	cCameraInteraction( peeViewEmitter &view ) : pView( view ){
		SetEnabledAll( false );
	}
	
public:
	virtual void OnCameraChanged(){
		if( pView.GetEmitter() ){
			pView.GetEmitter()->NotifyCameraChanged();
		}
	}
};

}



// Class peeViewEmitter
/////////////////////////

// Constructor, destructor
////////////////////////////

peeViewEmitter::peeViewEmitter( peeWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pEmitter( NULL )
{
	pCameraInteraction.TakeOver( new cCameraInteraction( *this ) );
	
	AddListener( pCameraInteraction );
}

peeViewEmitter::~peeViewEmitter(){
	SetEmitter( NULL );
}



// Management
///////////////

void peeViewEmitter::ResetView(){
}

void peeViewEmitter::SetEmitter( peeEmitter *emitter ){
	if( emitter == pEmitter ){
		return;
	}
	
	pCameraInteraction->SetCamera( NULL );
	SetRenderWorld( NULL );
	
	if( pEmitter ){
		pEmitter->FreeReference();
	}
	
	pEmitter = emitter;
	
	if( emitter ){
		emitter->AddReference();
		SetRenderWorld( emitter->GetCamera()->GetEngineCamera() );
		pCameraInteraction->SetCamera( emitter->GetCamera() );
	}
}



void peeViewEmitter::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pEmitter ){
		pEmitter->Update( elapsed );
	}
}

void peeViewEmitter::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( pEmitter ){
		SetRenderWorld( pEmitter->GetCamera()->GetEngineCamera() );
	}
}
