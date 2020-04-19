/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include "lpeWindowMain.h"
#include "lpeWindowMainListener.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>

#include <dragengine/common/exceptions.h>



// Class lpeWindowMainListener
////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeWindowMainListener::lpeWindowMainListener( lpeWindowMain &window ) :
pWindow( window ){
}

lpeWindowMainListener::~lpeWindowMainListener(){
}



// Management
///////////////

void lpeWindowMainListener::StateChanged( lpeLangPack *langpack ){
	pWindow.GetActionLangPackSave()->Update();
}

void lpeWindowMainListener::UndoChanged( lpeLangPack *langpack ){
	pWindow.GetActionEditUndo()->Update();
	pWindow.GetActionEditRedo()->Update();
}



void lpeWindowMainListener::EntryStructureChanged( lpeLangPack *langpack ){
	pWindow.GetActionEntryAdd()->Update();
	pWindow.GetActionEntryRemove()->Update();
}

void lpeWindowMainListener::EntrySelectionChanged( lpeLangPack *langpack ){
	pWindow.GetActionEntryRemove()->Update();
}
