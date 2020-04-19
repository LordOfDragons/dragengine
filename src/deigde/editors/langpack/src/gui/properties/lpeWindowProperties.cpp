/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include "lpeWindowProperties.h"
#include "lpeWPUndoHistory.h"
#include "lpeWPLangPack.h"
#include "../lpeWindowMain.h"
#include "../../langpack/lpeLangPack.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class lpeWindowProperties
//////////////////////////////

// Constructor, destructor
////////////////////////////

lpeWindowProperties::lpeWindowProperties( lpeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pPanelLangPack( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelLangPack = new lpeWPLangPack( *this );
	AddChild( pPanelLangPack, "Language Pack" );
	
	pPanelUndoHistory.TakeOver( new lpeWPUndoHistory( env ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 );
}

lpeWindowProperties::~lpeWindowProperties(){
}



// Management
///////////////

void lpeWindowProperties::SetLangPack( lpeLangPack *langpack ){
	pPanelLangPack->SetLangPack( langpack );
	( ( lpeWPUndoHistory* )pPanelUndoHistory.operator->() )->SetLangPack( langpack );
}
