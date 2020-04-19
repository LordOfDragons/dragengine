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

#include "gdeMAOCForceFieldPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCForceField.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/forceField/gdeOCForceField.h"
#include "../../../../undosys/objectClass/forceField/gdeUOCAddForceField.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCForceFieldPaste
/////////////////////////////////

// Constructor
////////////////

gdeMAOCForceFieldPaste::gdeMAOCForceFieldPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Force Field",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class force field" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCForceFieldPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCForceField::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCForceField &clipOCForceField =
		( const gdeClipboardDataOCForceField & )( igdeClipboardData& )clip;
	
	deObjectReference forceField;
	forceField.TakeOver( new gdeOCForceField( *clipOCForceField.GetForceField() ) );
	
	igdeUndo * const undo = new gdeUOCAddForceField( &objectClass,
		( gdeOCForceField* )( deObject* )forceField );
	undo->SetShortInfo( "Paste object class particle emitter" );
	return undo;
}

void gdeMAOCForceFieldPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCForceField::TYPE_NAME ) );
}
