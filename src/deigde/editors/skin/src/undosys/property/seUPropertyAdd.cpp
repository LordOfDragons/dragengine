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

#include "seUPropertyAdd.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyAdd
/////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyAdd::seUPropertyAdd( seTexture *texture, const sePropertyList &properties ){
	if( ! texture || properties.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	
	if( properties.GetCount() == 1 ){
		SetShortInfo( "Add Property" );
		
	}else{
		SetShortInfo( "Add Properties" );
	}
	
	pProperties = properties;
	
	pTexture = texture;
	texture->AddReference();
}

seUPropertyAdd::~seUPropertyAdd(){
	pProperties.RemoveAll();
	if( pTexture ){
		pTexture->FreeReference();
	}
}



// Management
///////////////

void seUPropertyAdd::Undo(){
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pTexture->RemoveProperty( pProperties.GetAt( i ) );
	}
}

void seUPropertyAdd::Redo(){
	const int count = pProperties.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		pTexture->AddProperty( pProperties.GetAt( i ) );
	}
	
	pTexture->SetActiveProperty( pProperties.GetAt( 0 ) );
}
