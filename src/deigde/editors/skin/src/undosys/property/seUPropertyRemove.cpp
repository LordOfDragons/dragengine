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

#include "seUPropertyRemove.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyRemove
////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyRemove::seUPropertyRemove( seProperty *property ){
	if( ! property ) DETHROW( deeInvalidParam );
	
	seTexture *texture = property->GetTexture();
	if( ! texture ) DETHROW( deeInvalidParam );
	
	pProperty = NULL;
	pTexture = NULL;
	
	SetShortInfo( "Remove Property" );
	
	pProperty = property;
	property->AddReference();
	
	pTexture = texture;
	texture->AddReference();
}

seUPropertyRemove::~seUPropertyRemove(){
	if( pTexture ) pTexture->FreeReference();
	if( pProperty ) pProperty->FreeReference();
}



// Management
///////////////

void seUPropertyRemove::Undo(){
	pTexture->AddProperty( pProperty );
}

void seUPropertyRemove::Redo(){
	pTexture->RemoveProperty( pProperty );
}
