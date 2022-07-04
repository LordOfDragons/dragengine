/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeWindowProperties.h"
#include "peeWPType.h"
#include "peeWPController.h"
#include "peeWPView.h"
#include "peeWPUndoHistory.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class peeWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

peeWindowProperties::peeWindowProperties( peeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelType.TakeOver( new peeWPType( *this ) );
	AddChild( pPanelType, "Type" );
	
	pPanelController.TakeOver( new peeWPController( *this ) );
	AddChild( pPanelController, "Controller" );
	
	pPanelView.TakeOver( new peeWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new peeWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 ); // type
}

peeWindowProperties::~peeWindowProperties(){
}



// Management
///////////////

void peeWindowProperties::SetEmitter( peeEmitter *emitter ){
	( ( peeWPType& )( igdeWidget& )pPanelType ).SetEmitter( emitter );
	( ( peeWPController& )( igdeWidget& )pPanelController ).SetEmitter( emitter );
	( ( peeWPView& )( igdeWidget& )pPanelView ).SetEmitter( emitter );
	( ( peeWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetEmitter( emitter );
}

void peeWindowProperties::OnEmitterPathChanged(){
	( ( peeWPType& )( igdeWidget& )pPanelType ).OnEmitterPathChanged();
}
