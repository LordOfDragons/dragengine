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

#include "gdeMAOCSpeakerPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCSpeaker.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../../../undosys/objectClass/speaker/gdeUOCAddSpeaker.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCSpeakerPaste
//////////////////////////////

// Constructor
////////////////

gdeMAOCSpeakerPaste::gdeMAOCSpeakerPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Speaker",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class speaker" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCSpeakerPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCSpeaker::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCSpeaker &clipOCSpeaker =
		( const gdeClipboardDataOCSpeaker & )( igdeClipboardData& )clip;
	
	deObjectReference speaker;
	speaker.TakeOver( new gdeOCSpeaker( *clipOCSpeaker.GetSpeaker() ) );
	
	igdeUndo * const undo = new gdeUOCAddSpeaker( &objectClass,
		( gdeOCSpeaker* )( deObject* )speaker );
	undo->SetShortInfo( "Paste object class speaker" );
	return undo;
}

void gdeMAOCSpeakerPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCSpeaker::TYPE_NAME ) );
}
