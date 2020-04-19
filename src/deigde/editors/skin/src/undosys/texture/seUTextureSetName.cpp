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

#include "seUTextureSetName.h"
#include "../../skin/texture/seTexture.h"

#include <dragengine/common/exceptions.h>



// Class seUTextureSetName
////////////////////////////

// Constructor, destructor
////////////////////////////

seUTextureSetName::seUTextureSetName( seTexture *texture, const char *newName ){
	if( ! texture || ! newName ) DETHROW( deeInvalidParam );
	
	pTexture = NULL;
	
	SetShortInfo( "Set Texture Name" );
	
	pOldName = texture->GetName();
	pNewName = newName;
	
	pTexture = texture;
	texture->AddReference();
}

seUTextureSetName::~seUTextureSetName(){
	if( pTexture ) pTexture->FreeReference();
}



// Management
///////////////

void seUTextureSetName::Undo(){
	pTexture->SetName( pOldName.GetString() );
}

void seUTextureSetName::Redo(){
	pTexture->SetName( pNewName.GetString() );
}
