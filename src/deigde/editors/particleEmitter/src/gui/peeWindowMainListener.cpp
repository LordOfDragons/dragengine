/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeWindowMain.h"
#include "peeWindowMainListener.h"



// Class peeWindowMainListener
////////////////////////////////

// Constructor, destructor
////////////////////////////

peeWindowMainListener::peeWindowMainListener( peeWindowMain &window ) :
pWindow( window ){
}

peeWindowMainListener::~peeWindowMainListener(){
}



// Management
///////////////

void peeWindowMainListener::StateChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::UndoChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::EmitterChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::ControllerStructureChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::ActiveControllerChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::TypeStructureChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::TypeActiveTrailControllerChanged( peeEmitter*, peeType* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::TypeActiveEmitControllerChanged( peeEmitter*, peeType* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::ActiveTypeParameterChanged( peeEmitter*, peeType* ){
	pWindow.UpdateAllActions();
}

void peeWindowMainListener::ActiveTypeChanged( peeEmitter* ){
	pWindow.UpdateAllActions();
}
