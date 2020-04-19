/* 
 * Drag[en]gine IGDE Font Editor
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

#include "feWindowProperties.h"
#include "feWPFont.h"
#include "feWPGlyph.h"
#include "feWPUndoHistory.h"
#include "../feWindowMain.h"
#include "../../font/feFont.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class feWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

feWindowProperties::feWindowProperties( feWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	igdeEnvironment &env = GetEnvironment();
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelFont.TakeOver( new feWPFont( *this ) );
	AddChild( pPanelFont, "Font" );
	
	pPanelGlyph.TakeOver( new feWPGlyph( *this ) );
	AddChild( pPanelGlyph, "Glyph" );
	
	pPanelUndoHistory.TakeOver( new feWPUndoHistory( env ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 );
}

feWindowProperties::~feWindowProperties(){
}



// Management
///////////////

void feWindowProperties::SetFont( feFont *font ){
	( ( feWPFont& )( igdeWidget& )pPanelFont ).SetFont( font );
	( ( feWPGlyph& )( igdeWidget& )pPanelGlyph ).SetFont( font );
	( ( feWPUndoHistory* )pPanelUndoHistory.operator->() )->SetFont( font );
}
