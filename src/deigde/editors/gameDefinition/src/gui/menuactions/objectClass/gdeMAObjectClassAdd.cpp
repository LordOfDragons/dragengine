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

#include "gdeMAObjectClassAdd.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassAdd
//////////////////////////////

// Constructor
////////////////

gdeMAObjectClassAdd::gdeMAObjectClassAdd( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Add Object Class...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Add object class" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassAdd::OnAction( gdeGameDefinition &gameDefinition ){
	const gdeObjectClassList &list = gameDefinition.GetObjectClasses();
	decString name( "Object Class" );
	
	while( igdeCommonDialogs::GetString( &pWindowMain, "Add Object Class", "Name:", name ) ){
		if( list.HasNamed( name ) ){
			igdeCommonDialogs::Error( &pWindowMain, "Add Object Class", "Object Class exists already." );
			continue;
		}
		
		deObjectReference objectClass;
		objectClass.TakeOver( new gdeObjectClass( name ) );
		return new gdeUAddObjectClass( &gameDefinition, ( gdeObjectClass* )( deObject* )objectClass );
	}
	
	return NULL;
}
