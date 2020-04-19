/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdlib.h>
#include <string.h>
#include "meWindowMain.h"
#include "meWindowMainListener.h"
#include "../world/meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include "dragengine/common/exceptions.h"



// Class meWindowMainListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowMainListener::meWindowMainListener( meWindowMain &windowMain ) :
pWindowMain( windowMain ){
}

meWindowMainListener::~meWindowMainListener(){
}



// Notifications
//////////////////

void meWindowMainListener::ModeChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::StateChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::HTStateChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::UndoChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::LightingChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::EditingChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}



void meWindowMainListener::ObjectSelectionChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::ObjectChanged( meWorld*, meObject* ){
	pWindowMain.UpdateAllActions();
}



void meWindowMainListener::DecalSelectionChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::DecalChanged( meWorld*, meDecal* ){
	pWindowMain.UpdateAllActions();
}
