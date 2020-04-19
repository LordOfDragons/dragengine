/* 
 * Drag[en]gine IGDE Sky Editor
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
#include <string.h>
#include <stdlib.h>

#include "seUBodyMoveDown.h"
#include "../../sky/body/seBody.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seUBodyMoveDown
//////////////////////////

// Constructor, destructor
////////////////////////////

seUBodyMoveDown::seUBodyMoveDown( seBody *body ) :
pBody( NULL ),
pIndex( 0 )
{
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	
	const seLayer * const layer = body->GetLayer();
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Move Body Down" );
	
	pIndex = layer->GetBodies().IndexOf( body );
	if( pIndex == -1 || pIndex == layer->GetBodies().GetCount() - 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pBody = body;
	body->AddReference();
}

seUBodyMoveDown::~seUBodyMoveDown(){
	if( pBody ){
		pBody->FreeReference();
	}
}



// Management
///////////////

void seUBodyMoveDown::Undo(){
	pBody->GetLayer()->MoveBodyTo( pBody, pIndex );
}

void seUBodyMoveDown::Redo(){
	pBody->GetLayer()->MoveBodyTo( pBody, pIndex + 1 );
}
