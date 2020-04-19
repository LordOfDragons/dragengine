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

#include "gdeMAObjectClassPaste.h"
#include "../../gdeWindowMain.h"
#include "../../../clipboard/gdeClipboardDataObjectClass.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassPaste
////////////////////////////////

// Constructor
////////////////

gdeMAObjectClassPaste::gdeMAObjectClassPaste( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Paste Object Class",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassPaste::OnAction( gdeGameDefinition &gameDefinition ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataObjectClass::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataObjectClass &clipObjectClass =
		( const gdeClipboardDataObjectClass & )( igdeClipboardData& )clip;
	
	const gdeObjectClassList &list = gameDefinition.GetObjectClasses();
	decString name( clipObjectClass.GetObjectClass()->GetName() );
	
	if( list.HasNamed( name ) ){
		while( true ){
			if( ! igdeCommonDialogs::GetString( &pWindowMain, "Paste Object Class", "Name:", name ) ){
				return NULL;
			}
			
			if( list.HasNamed( name ) ){
				igdeCommonDialogs::Error( &pWindowMain, "Paste Object Class", "Object Class exists already." );
				
			}else{
				break;
			}
		}
	}
	
	deObjectReference objectClass;
	objectClass.TakeOver( new gdeObjectClass( *clipObjectClass.GetObjectClass() ) );
	( ( gdeObjectClass& )( deObject& )objectClass ).SetName( name );
	
	igdeUndo * const undo = new gdeUAddObjectClass(
		&gameDefinition, ( gdeObjectClass* )( deObject* )objectClass );
	undo->SetShortInfo( "Paste object class" );
	return undo;
}

void gdeMAObjectClassPaste::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataObjectClass::TYPE_NAME ) );
}
