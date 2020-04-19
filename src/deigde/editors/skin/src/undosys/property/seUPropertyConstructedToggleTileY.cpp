/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seUPropertyConstructedToggleTileY.h"
#include "../../skin/property/seProperty.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyConstructedToggleTileY
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyConstructedToggleTileY::seUPropertyConstructedToggleTileY( seProperty *property ) :
pProperty( NULL )
{
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Property constructed tile Y" );
	
	pProperty = property;
	property->AddReference();
}

seUPropertyConstructedToggleTileY::~seUPropertyConstructedToggleTileY(){
	if( pProperty ){
		pProperty->FreeReference();
	}
}



// Management
///////////////

void seUPropertyConstructedToggleTileY::Undo(){
	pProperty->SetNodeTileY( ! pProperty->GetNodeTileY() );
}

void seUPropertyConstructedToggleTileY::Redo(){
	pProperty->SetNodeTileY( ! pProperty->GetNodeTileY() );
}
