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

#include "gdeMAObjectClassSubclass.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassSubclass
///////////////////////////////////

// Constructor
////////////////

gdeMAObjectClassSubclass::gdeMAObjectClassSubclass( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Subclass Object Class",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongDown ),
	"Create new Object Class as Subclass of the selected Object Class" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassSubclass::OnAction( gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const category = gameDefinition.GetActiveObjectClass();
	if( ! category || gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotObjectClass ){
		return NULL;
	}
	
	const gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if( ! objectClass ){
		return NULL;
	}
	
	const gdeObjectClassList &list = gameDefinition.GetObjectClasses();
	decString name( objectClass->GetName() );
	
	while( igdeCommonDialogs::GetString( &pWindowMain, "Subclass Object Class", "Name:", name ) ){
		if( list.HasNamed( name ) ){
			igdeCommonDialogs::Error( &pWindowMain, "Subclass Object Class", "Object Class exists already." );
			continue;
		}
		
		deObjectReference subclass;
		subclass.TakeOver( new gdeObjectClass( name ) );
		gdeObjectClass &soc = ( gdeObjectClass& )( deObject& )subclass;
		
		soc.SetCategory( objectClass->GetCategory() );
		soc.SetHideTags( objectClass->GetHideTags() );
		soc.SetIsGhost( objectClass->GetIsGhost() );
		soc.SetCanInstantiate( objectClass->GetCanInstantiate() );
		soc.SetPartialHideTags( objectClass->GetPartialHideTags() );
		soc.SetScaleMode( objectClass->GetScaleMode() );
		
		deObjectReference objRefInherit;
		objRefInherit.TakeOver( new gdeOCInherit( objectClass->GetName() ) );
		gdeOCInherit& inherit = ( gdeOCInherit& )( deObject& )objRefInherit;
		inherit.SetPropertyPrefix( objectClass->GetDefaultInheritPropertyPrefix() );
		soc.GetInherits().Add( &inherit );
		
		return new gdeUAddObjectClass( &gameDefinition, &soc );
	}
	return NULL;
}

void gdeMAObjectClassSubclass::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass 
		&& gameDefinition->GetActiveObjectClass() != NULL );
}
