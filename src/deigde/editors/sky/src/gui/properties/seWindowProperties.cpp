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

#include "seWindowProperties.h"
#include "seWPSky.h"
#include "seWPLink.h"
#include "seWPLayer.h"
#include "seWPController.h"
#include "seWPUndoHistory.h"
#include "seWPView.h"
#include "../seWindowMain.h"
#include "../../sky/seSky.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class seWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWindowProperties::seWindowProperties( seWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelController.TakeOver( new seWPController( *this ) );
	AddChild( pPanelController, "Controller" );
	
	pPanelLink.TakeOver( new seWPLink( *this ) );
	AddChild( pPanelLink, "Link" );
	
	pPanelLayer.TakeOver( new seWPLayer( *this ) );
	AddChild( pPanelLayer, "Layer" );
	
	pPanelSky.TakeOver( new seWPSky( *this ) );
	AddChild( pPanelSky, "Sky" );
	
	pPanelView.TakeOver( new seWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new seWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 ); // controller
}

seWindowProperties::~seWindowProperties(){
}



// Management
///////////////

void seWindowProperties::SetSky( seSky *sky ){
	( ( seWPController& )( igdeWidget& )pPanelController ).SetSky( sky );
	( ( seWPLink& )( igdeWidget& )pPanelLink ).SetSky( sky );
	( ( seWPLayer& )( igdeWidget& )pPanelLayer ).SetSky( sky );
	( ( seWPSky& )( igdeWidget& )pPanelSky ).SetSky( sky );
	( ( seWPView& )( igdeWidget& )pPanelView ).SetSky( sky );
	( ( seWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetSky( sky );
}
