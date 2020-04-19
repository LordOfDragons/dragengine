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

#include "gdeMAOCSnapPointPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCSnapPoint.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../../../undosys/objectClass/snappoint/gdeUOCAddSnapPoint.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCSnapPointPaste
////////////////////////////////

// Constructor
////////////////

gdeMAOCSnapPointPaste::gdeMAOCSnapPointPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Snap Point",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class snap point" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCSnapPointPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCSnapPoint::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCSnapPoint &clipOCSnapPoint =
		( const gdeClipboardDataOCSnapPoint & )( igdeClipboardData& )clip;
	
	deObjectReference snapPoint;
	snapPoint.TakeOver( new gdeOCSnapPoint( *clipOCSnapPoint.GetSnapPoint() ) );
	
	igdeUndo * const undo = new gdeUOCAddSnapPoint( &objectClass,
		( gdeOCSnapPoint* )( deObject* )snapPoint );
	undo->SetShortInfo( "Paste object class snap point" );
	return undo;
}

void gdeMAOCSnapPointPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCSnapPoint::TYPE_NAME ) );
}
