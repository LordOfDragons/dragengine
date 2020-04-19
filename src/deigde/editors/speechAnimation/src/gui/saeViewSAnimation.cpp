/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeViewSAnimation.h"
#include "saeWindowMain.h"
#include "../configuration/saeConfiguration.h"
#include "../sanimation/saeSAnimation.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>



namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	saeViewSAnimation &pView;
	
public:
	cCameraInteraction( saeViewSAnimation &view ) : pView( view ){ }
	
public:
	virtual igdeMouseCameraListener::eInteraction ChooseInteraction(){
		if( ! pView.GetSAnimation() ){
			return eiNone;
		}
		return igdeMouseCameraListener::ChooseInteraction();
	}
	
	virtual void OnCameraChanged(){
		if( pView.GetSAnimation() ){
			pView.GetSAnimation()->NotifyCameraChanged();
		}
	}
};

}



// Class saeViewSAnimation
////////////////////////////

// Constructor, destructor
////////////////////////////

saeViewSAnimation::saeViewSAnimation( saeWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pSAnimation( NULL )
{
	pFontStats.TakeOver( windowMain.GetEngine()->GetFontManager()->
		LoadFont( "/igde/fonts/sans_10.defont", "/" ) );
	
	pCameraInteraction.TakeOver( new cCameraInteraction( *this ) );
	
	AddListener( pCameraInteraction );
}

saeViewSAnimation::~saeViewSAnimation(){
	SetSAnimation( NULL );
}



// Management
///////////////

void saeViewSAnimation::ResetView(){
}

void saeViewSAnimation::SetSAnimation( saeSAnimation *sanimation ){
	if( sanimation == pSAnimation ){
		return;
	}
	
	pCameraInteraction->SetCamera( NULL );
	
	SetRenderWorld( NULL );
	
	if( pSAnimation ){
		pSAnimation->FreeReference();
	}
	
	pSAnimation = sanimation;
	
	if( sanimation ){
		sanimation->AddReference();
		SetRenderWorld( sanimation->GetCamera()->GetEngineCamera() );
		pCameraInteraction->SetCamera( sanimation->GetCamera() );
	}
}



void saeViewSAnimation::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pSAnimation ){
		pSAnimation->Update( elapsed );
	}
}

void saeViewSAnimation::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( pSAnimation ){
		SetRenderWorld( pSAnimation->GetCamera()->GetEngineCamera() );
	}
}
