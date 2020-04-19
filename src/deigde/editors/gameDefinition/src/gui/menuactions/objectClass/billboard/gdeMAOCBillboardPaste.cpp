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

#include "gdeMAOCBillboardPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCBillboard.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../../../undosys/objectClass/billboard/gdeUOCAddBillboard.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCBillboardPaste
////////////////////////////////

// Constructor
////////////////

gdeMAOCBillboardPaste::gdeMAOCBillboardPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Billboard",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class billboard" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCBillboardPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCBillboard::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCBillboard &clipOCBillboard =
		( const gdeClipboardDataOCBillboard & )( igdeClipboardData& )clip;
	
	deObjectReference billboard;
	billboard.TakeOver( new gdeOCBillboard( *clipOCBillboard.GetBillboard() ) );
	
	igdeUndo * const undo = new gdeUOCAddBillboard( &objectClass,
		( gdeOCBillboard* )( deObject* )billboard );
	undo->SetShortInfo( "Paste object class billboard" );
	return undo;
}

void gdeMAOCBillboardPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCBillboard::TYPE_NAME ) );
}
