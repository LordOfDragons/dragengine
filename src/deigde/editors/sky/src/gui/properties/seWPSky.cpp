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

#include "seWPSky.h"
#include "seWPSkyListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../sky/seSky.h"
#include "../../undosys/sky/seUSkySetBgColor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////


namespace{

class cColorBackground : public igdeColorBoxListener{
protected:
	seWPSky &pPanel;
	
public:
	cColorBackground( seWPSky &panel ) : pPanel( panel ){ }
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		seSky * const sky = pPanel.GetSky();
		if( ! sky ){
			return;
		}
		
		if( colorBox->GetColor().IsEqualTo( sky->GetBgColor() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUSkySetBgColor( sky, colorBox->GetColor() ) );
		if( undo ){
			sky->GetUndoSystem()->Add( undo );
		}
	}
};

}



// Class seWPSky
//////////////////

// Constructor, destructor
////////////////////////////

seWPSky::seWPSky( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pSky( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPSkyListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.GroupBox( content, groupBox, "Sky:" );
	
	helper.ColorBox( groupBox, "Bg Color:", "Background Color.", pClrBg, new cColorBackground( *this ) );
}

seWPSky::~seWPSky(){
	SetSky( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPSky::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	if( pSky ){
		pSky->RemoveListener( pListener );
		pSky->FreeReference();
		pSky = NULL;
	}
	
	pSky = sky;
	
	if( sky ){
		sky->AddListener( pListener );
		sky->AddReference();
	}
	
	UpdateSky();
}



void seWPSky::UpdateSky(){
	if( pSky ){
		pClrBg->SetColor( pSky->GetBgColor() );
		
	}else{
		pClrBg->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	}
	
	const bool enabled = pSky != NULL;
	
	pClrBg->SetEnabled( enabled );
}
