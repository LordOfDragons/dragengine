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

#include <stdlib.h>
#include <string.h>

#include "peeWPType.h"
#include "peeWPTypeListener.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeType.h"

#include <dragengine/common/exceptions.h>



// Class peeWPTypeListener
////////////////////////////

// Constructor, destructor
////////////////////////////

peeWPTypeListener::peeWPTypeListener( peeWPType &panel ) :
pPanel( panel ){
}

peeWPTypeListener::~peeWPTypeListener(){
}



// Management
///////////////

void peeWPTypeListener::ControllerStructureChanged( peeEmitter* ){
	pPanel.UpdateControllerList();
}

void peeWPTypeListener::ControllerChanged( peeEmitter*, peeController* ){
	pPanel.UpdateControllerList();
}



void peeWPTypeListener::TypeStructureChanged( peeEmitter* ){
	pPanel.UpdateTypeList();
	pPanel.UpdateParameter();
}

void peeWPTypeListener::TypeChanged( peeEmitter*, peeType* ){
	pPanel.UpdateTypeList();
}

void peeWPTypeListener::TypeActiveTrailControllerChanged( peeEmitter*, peeType *type ){
	if( type->GetActive() ){
		pPanel.SelectActiveTrailController();
		pPanel.UpdateType();
	}
}

void peeWPTypeListener::TypeActiveEmitControllerChanged( peeEmitter*, peeType *type ){
	if( type->GetActive() ){
		pPanel.SelectActiveEmitController();
		pPanel.UpdateType();
	}
}

void peeWPTypeListener::TypeParameterChanged( peeEmitter*, peeType *type, peeParameter *parameter ){
	if( type->GetActive() ){
		pPanel.UpdateParameter();
	}
	pPanel.UpdateParameterUsage( *parameter );
}

void peeWPTypeListener::ActiveTypeParameterChanged( peeEmitter*, peeType *type ){
	if( type->GetActive() ){
		pPanel.SelectActiveParameter();
	}
}

void peeWPTypeListener::ActiveTypeChanged( peeEmitter* ){
	pPanel.SelectActiveType();
	pPanel.UpdateParameter();
}
