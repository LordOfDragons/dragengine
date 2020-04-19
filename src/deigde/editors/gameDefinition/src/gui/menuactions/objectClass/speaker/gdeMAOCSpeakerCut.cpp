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

#include "gdeMAOCSpeakerCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCSpeaker.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../../../undosys/objectClass/speaker/gdeUOCRemoveSpeaker.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCSpeakerCut
////////////////////////////

// Constructor
////////////////

gdeMAOCSpeakerCut::gdeMAOCSpeakerCut( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Cut Object Class Speaker",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
	"Cut object class speaker" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCSpeakerCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass ){
	if( gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCSpeaker ){
		return NULL;
	}
	
	gdeOCSpeaker * const speaker = gameDefinition.GetActiveOCSpeaker();
	if( ! speaker ){
		return NULL;
	}
	
	deObjectReference clipOCSpeaker;
	clipOCSpeaker.TakeOver( new gdeOCSpeaker( *speaker ) );
	
	igdeClipboardDataReference clipData;
	clipData.TakeOver( new gdeClipboardDataOCSpeaker( ( gdeOCSpeaker* )( deObject* )clipOCSpeaker ) );
	
	pWindowMain.GetClipboard().Set( clipData );
	
	return new gdeUOCRemoveSpeaker( &objectClass, speaker );
}

void gdeMAOCSpeakerCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled( gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSpeaker
		&& gameDefinition->GetActiveOCSpeaker() != NULL );
}
