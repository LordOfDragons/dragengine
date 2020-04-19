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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saeWindowProperties.h"
#include "saeWPSAnim.h"
#include "saeWPView.h"
#include "saeWPUndoHistory.h"
#include "../saeWindowMain.h"
#include "../../sanimation/saeSAnimation.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class saeWindowProperties
//////////////////////////////

// Constructor, destructor
////////////////////////////

saeWindowProperties::saeWindowProperties( saeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelSAnimation.TakeOver( new saeWPSAnim( *this ) );
	AddChild( pPanelSAnimation, "SAnimation" );
	
	pPanelView.TakeOver( new saeWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new saeWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 ); // speech animation panel
}

saeWindowProperties::~saeWindowProperties(){
}



// Management
///////////////

void saeWindowProperties::SetSAnimation( saeSAnimation *sanimation ){
	( ( saeWPSAnim& )( igdeWidget& )pPanelSAnimation ).SetSAnimation( sanimation );
	( ( saeWPView& )( igdeWidget& )pPanelView ).SetSAnimation( sanimation );
	( ( saeWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetSAnimation( sanimation );
}
