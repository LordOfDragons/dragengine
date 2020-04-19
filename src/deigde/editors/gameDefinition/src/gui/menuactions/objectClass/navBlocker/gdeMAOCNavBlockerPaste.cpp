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

#include "gdeMAOCNavBlockerPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCNavBlocker.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../../../undosys/objectClass/navblocker/gdeUOCAddNavBlocker.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCNavBlockerPaste
/////////////////////////////////

// Constructor
////////////////

gdeMAOCNavBlockerPaste::gdeMAOCNavBlockerPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Navigation Blocker",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class navigation blocker" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCNavBlockerPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCNavBlocker::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCNavBlocker &clipOCNavigationBlocker =
		( const gdeClipboardDataOCNavBlocker & )( igdeClipboardData& )clip;
	
	deObjectReference navBlocker;
	navBlocker.TakeOver( new gdeOCNavigationBlocker( *clipOCNavigationBlocker.GetNavBlocker() ) );
	
	igdeUndo * const undo = new gdeUOCAddNavBlocker( &objectClass,
		( gdeOCNavigationBlocker* )( deObject* )navBlocker );
	undo->SetShortInfo( "Paste object class navigation blocker" );
	return undo;
}

void gdeMAOCNavBlockerPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCNavBlocker::TYPE_NAME ) );
}
