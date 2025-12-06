/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

#include <deigde/gui/igdeContainer.h>
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
