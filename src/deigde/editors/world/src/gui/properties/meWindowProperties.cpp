/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWindowProperties.h"
#include "meWPView.h"
#include "meWPSensors.h"
#include "meWPWorld.h"
#include "meWPHeightTerrain.h"
#include "meWPBrowser.h"
#include "meWPAdd.h"
#include "meWPUndoHistory.h"
#include "selection/meWPSelection.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>


// Class meWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowProperties::meWindowProperties( meWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),

pPropSelection( NULL ),
pPropView( NULL ),
pPropWorld( NULL ),
pPropSensors( NULL ),
pPropHT( NULL ),
pPropBrowser( NULL ),
pPropAdd( NULL )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPropWorld = new meWPWorld( *this );
	AddChild( pPropWorld, "World" );
	
	pPropSelection = new meWPSelection( *this );
	AddChild( pPropSelection, "Selection" );
	
	pPropBrowser = new meWPBrowser( *this );
	AddChild( pPropBrowser, "Browser" );
	
// 	pPropAdd = new meWPAdd( *this );
// 	AddChild( pPropAdd, "Add" );
	
	pPropHT = new meWPHeightTerrain( *this );
	AddChild( pPropHT, "Height Terrain" );
	
	pPropView = new meWPView( *this );
	AddChild( pPropView, "View" );
	
	pPropSensors = new meWPSensors( *this );
	AddChild( pPropSensors, "Sensors" );
	
	pPanelUndoHistory.TakeOver( new meWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "History" );
	
	SetActivePanel( 1 ); // selection
}

meWindowProperties::~meWindowProperties(){
}



// Management
///////////////

void meWindowProperties::SetWorld( meWorld *world ){
	pPropWorld->SetWorld( world );
	pPropView->SetWorld( world );
	pPropHT->SetWorld( world );
	pPropSelection->SetWorld( world );
	pPropBrowser->SetWorld( world );
// 	pPropAdd->SetWorld( world );
	( ( meWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetWorld( world );
}

void meWindowProperties::OnWorldPathChanged(){
	pPropHT->OnWorldPathChanged();
}

void meWindowProperties::OnGameProjectChanged(){
	pPropBrowser->OnGameProjectChanged();
}

void meWindowProperties::OnGameDefinitionChanged(){
	pPropView->OnGameDefinitionChanged();
	pPropWorld->OnGameDefinitionChanged();
	pPropSelection->OnGameDefinitionChanged();
	pPropBrowser->OnGameDefinitionChanged();
// 	pPropAdd->OnGameDefinitionChanged();
}

void meWindowProperties::SwitchToBrowser(){
	SetActivePanel( 2 );
}
