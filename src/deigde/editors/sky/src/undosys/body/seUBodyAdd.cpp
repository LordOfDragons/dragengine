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

#include "seUBodyAdd.h"
#include "../../sky/layer/seLayer.h"
#include "../../sky/body/seBody.h"

#include <dragengine/common/exceptions.h>



// Class seUBodyAdd
/////////////////////

// Constructor, destructor
////////////////////////////

seUBodyAdd::seUBodyAdd( seLayer *layer, seBody *body ) :
pLayer( NULL ),
pBody( NULL )
{
	if( ! layer || ! body ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add Body" );
	
	pLayer = layer;
	layer->AddReference();
	
	pBody = body;
	body->AddReference();
}

seUBodyAdd::~seUBodyAdd(){
	if( pBody ){
		pBody->FreeReference();
	}
	if( pLayer ){
		pLayer->FreeReference();
	}
}



// Management
///////////////

void seUBodyAdd::Undo(){
	pLayer->RemoveBody( pBody );
}

void seUBodyAdd::Redo(){
	pLayer->AddBody( pBody );
	pLayer->SetActiveBody( pBody );
}
