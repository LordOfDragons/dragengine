/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeWPView.h"
#include "gdeWPViewListener.h"
#include "gdeWindowProperties.h"
#include "../gdeWindowMain.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../configuration/gdeConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	gdeWPView &pPanel;
	
public:
	cBaseAction( gdeWPView &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if( gameDefinition ){
			OnAction( *gameDefinition );
		}
	}
	
	virtual void OnAction( gdeGameDefinition &gameDefinition ) = 0;
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged( gdeWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( gdeGameDefinition &gameDefinition ){
		gameDefinition.NotifyCameraChanged();
	}
};

class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged( gdeWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( gdeGameDefinition &gameDefinition ){
		gameDefinition.NotifySkyChanged();
	}
};

}



// Class gdeWPView
////////////////////

// Constructor, destructor
////////////////////////////

gdeWPView::gdeWPView( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pGameDefinition( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content;
	
	pListener = new gdeWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.WPCamera( content, pWPCamera, new cActionCameraChanged( *this ),
		"Camera:", false, false, true );
	helper.WPSky( content, pWPSky, new cActionSkyChanged( *this ),
		"Environment Object:", false, false, true );
}

gdeWPView::~gdeWPView(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPView::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	if( gameDefinition == pGameDefinition ){
		return;
	}
	
	pWPSky->SetSky( NULL );
	pWPCamera->SetCamera( NULL );
	
	if( pGameDefinition ){
		pGameDefinition->RemoveListener( pListener );
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddListener( pListener );
		gameDefinition->AddReference();
		
		pWPCamera->SetCamera( gameDefinition->GetCamera() );
		pWPSky->SetSky( gameDefinition->GetSky() );
	}
	
	UpdateView();
	UpdateSky();
	UpdateCamera();
}

void gdeWPView::UpdateView(){
}

void gdeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void gdeWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
