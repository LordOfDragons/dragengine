/* 
 * Drag[en]gine IGDE
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

#include "igdeActionContextMenu.h"
#include "../igdeContainer.h"
#include "../igdeWidget.h"
#include "../igdeUIHelper.h"
#include "../menu/igdeMenuCascade.h"
#include "../menu/igdeMenuCascadeReference.h"

#include <dragengine/common/exceptions.h>



// Class igdeActionContextMenu
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionContextMenu::igdeActionContextMenu( const char *text, igdeIcon *icon,
	const char *description ) :
igdeAction( text, icon, description ){
}

igdeActionContextMenu::igdeActionContextMenu( const char *text, igdeIcon *icon,
	const char *description, deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey ) :
igdeAction( text, icon, description, mnemonic, hotKey ){
}

igdeActionContextMenu::igdeActionContextMenu( igdeWidget *widget,
	const char *text, igdeIcon *icon, const char *description ) :
igdeAction( text, icon, description ),
pWidget( widget ){
}

igdeActionContextMenu::igdeActionContextMenu( igdeWidget *widget,
	const char *text, igdeIcon *icon, const char *description,
	deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey ) :
igdeAction( text, icon, description, mnemonic, hotKey ),
pWidget( widget ){
}

igdeActionContextMenu::~igdeActionContextMenu(){
}



// Management
///////////////

void igdeActionContextMenu::SetWidget( igdeWidget *widget ){
	pWidget = widget;
}



void igdeActionContextMenu::OnAction(){
	if( ! pWidget || ! pWidget->GetVisible() ){
		return;
	}
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( pWidget->GetEnvironment() ) );
	
	AddContextMenuEntries( menu );
	
	if( menu->GetChildCount() != 0 ){
		menu->PopupBottom( pWidget );
	}
}

void igdeActionContextMenu::AddContextMenuEntries( igdeMenuCascade& ){
}
