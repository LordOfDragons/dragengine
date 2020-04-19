/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seViewSkin.h"
#include "seWindowMain.h"
#include "../skin/seSkin.h"

#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/resources/rendering/deRenderWindow.h>



// Definitions
////////////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	seViewSkin &pView;
	
public:
	cCameraInteraction( seViewSkin &view ) : pView( view ){ }
	
public:
	virtual void OnCameraChanged(){
		if( pView.GetSkin() ){
			pView.GetSkin()->NotifyCameraChanged();
		}
	}
};

}



// Class seViewSkin
/////////////////////

// Constructor, destructor
////////////////////////////

seViewSkin::seViewSkin( seWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pSkin( NULL )
{
	pCameraInteraction.TakeOver( new cCameraInteraction( *this ) );
	
	AddListener( pCameraInteraction );
}

seViewSkin::~seViewSkin(){
	SetSkin( NULL );
}



// Management
///////////////

void seViewSkin::ResetView(){
}

void seViewSkin::SetSkin( seSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	pCameraInteraction->SetCamera( NULL );
	
	SetRenderWorld( NULL );
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddReference();
		SetRenderWorld( skin->GetCamera()->GetEngineCamera() );
		pCameraInteraction->SetCamera( skin->GetCamera() );
	}
}



void seViewSkin::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pSkin ){
		pSkin->Update( elapsed );
	}
}

void seViewSkin::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( pSkin ){
		SetRenderWorld( pSkin->GetCamera()->GetEngineCamera() );
	}
}
