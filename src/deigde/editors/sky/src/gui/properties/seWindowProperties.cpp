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

void seWindowProperties::OnSkyPathChanged(){
	( ( seWPLayer& )( igdeWidget& )pPanelLayer ).OnSkyPathChanged();
}
