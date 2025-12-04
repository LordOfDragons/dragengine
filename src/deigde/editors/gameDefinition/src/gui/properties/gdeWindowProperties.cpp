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

#include <deigde/gui/igdeContainer.h>
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
