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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seWPView.h"
#include "seWPViewListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../sky/seSky.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	seWPView &pPanel;
	
public:
	cBaseAction( seWPView &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		seSky * const sky = pPanel.GetSky();
		if( sky ){
			OnAction( *sky );
		}
	}
	
	virtual void OnAction( seSky &sky ) = 0;
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged( seWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( seSky &sky ){
		sky.NotifyCameraChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	cActionEnvObjChanged( seWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( seSky &sky ){
		sky.NotifyEnvObjectChanged();
	}
};

}



// Class seWPView
///////////////////

// Constructor, destructor
////////////////////////////

seWPView::seWPView( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pSky( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content;
	
	pListener = new seWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.WPCamera( content, pWPCamera, new cActionCameraChanged( *this ),
		"Camera:", false, false, true );
	helper.WPWObject( content, pWPEnvObject, new cActionEnvObjChanged( *this ),
		"Environment Object:", false, false, true );
}

seWPView::~seWPView(){
	SetSky( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPView::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	pWPEnvObject->SetObjectWrapper( NULL );
	pWPCamera->SetCamera( NULL );
	
	if( pSky ){
		pSky->RemoveListener( pListener );
		pSky->FreeReference();
		pSky = NULL;
	}
	
	pSky = sky;
	
	if( sky ){
		sky->AddListener( pListener );
		sky->AddReference();
		
		pWPCamera->SetCamera( sky->GetCamera() );
		pWPEnvObject->SetObjectWrapper( sky->GetEnvObject() );
	}
	
	UpdateView();
	UpdateEnvObject();
	UpdateCamera();
}



void seWPView::UpdateView(){
}

void seWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObjectWrapper();
}

void seWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
