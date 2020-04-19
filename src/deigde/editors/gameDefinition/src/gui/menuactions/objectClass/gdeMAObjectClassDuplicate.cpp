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

#include "gdeMAObjectClassDuplicate.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassDuplicate
////////////////////////////////////

// Constructor
////////////////

gdeMAObjectClassDuplicate::gdeMAObjectClassDuplicate( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Duplicate Object Class...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongRight ),
	"Duplicate object class" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassDuplicate::OnAction( gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if( ! objectClass ){
		return NULL;
	}
	
	const gdeObjectClassList &list = gameDefinition.GetObjectClasses();
	decString name( objectClass->GetName() );
	
	while( igdeCommonDialogs::GetString( &pWindowMain, "Duplicate Object Class", "Name:", name ) ){
		if( list.HasNamed( name ) ){
			igdeCommonDialogs::Error( &pWindowMain, "Duplicate Object Class", "Object Class exists already." );
			continue;
		}
		
		deObjectReference duplicate;
		duplicate.TakeOver( new gdeObjectClass( *objectClass ) );
		( ( gdeObjectClass& )( deObject& )duplicate ).SetName( name );
		return new gdeUAddObjectClass( &gameDefinition, ( gdeObjectClass* )( deObject* )duplicate );
	}
	return NULL;
}

void gdeMAObjectClassDuplicate::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass 
		&& gameDefinition->GetActiveObjectClass() != NULL );
}
