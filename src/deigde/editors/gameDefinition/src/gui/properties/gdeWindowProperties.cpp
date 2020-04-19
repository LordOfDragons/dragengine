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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeWindowProperties.h"
#include "gdeWPGameDefinition.h"
#include "gdeWPSelection.h"
#include "gdeWPView.h"
#include "gdeWPUndoHistory.h"
#include "../gdeWindowMain.h"
#include "../../gamedef/gdeGameDefinition.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class gdeWindowProperties
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWindowProperties::gdeWindowProperties( gdeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelGameDefinition.TakeOver( new gdeWPGameDefinition( *this ) );
	AddChild( pPanelGameDefinition, "Game Definition" );
	
	pPanelSelection.TakeOver( new gdeWPSelection( *this ) );
	AddChild( pPanelSelection, "Selection" );
	
	pPanelView.TakeOver( new gdeWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new gdeWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 1 ); // selection
}

gdeWindowProperties::~gdeWindowProperties(){
}



// Management
///////////////

void gdeWindowProperties::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	( ( gdeWPGameDefinition& )( igdeWidget& )pPanelGameDefinition ).SetGameDefinition( gameDefinition );
	( ( gdeWPSelection& )( igdeWidget& )pPanelSelection ).SetGameDefinition( gameDefinition );
	( ( gdeWPView& )( igdeWidget& )pPanelView ).SetGameDefinition( gameDefinition );
	( ( gdeWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetGameDefinition( gameDefinition );
}

gdeWPSelection &gdeWindowProperties::GetPanelSelection() const{
	return ( gdeWPSelection& )( igdeWidget& )pPanelSelection;
}
