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

#include "seUTextureAdd.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"

#include <dragengine/common/exceptions.h>



// Class seUTextureAdd
////////////////////////

// Constructor, destructor
////////////////////////////

seUTextureAdd::seUTextureAdd( seSkin *skin, seTexture *texture ){
	if( ! skin || ! texture ) DETHROW( deeInvalidParam );
	
	pSkin = NULL;
	pTexture = NULL;
	
	SetShortInfo( "Add Texture" );
	
	pTexture = texture;
	texture->AddReference();
	
	pSkin = skin;
	skin->AddReference();
}

seUTextureAdd::~seUTextureAdd(){
	if( pTexture ) pTexture->FreeReference();
	if( pSkin ) pSkin->FreeReference();
}



// Management
///////////////

void seUTextureAdd::Undo(){
	pSkin->RemoveTexture( pTexture );
}

void seUTextureAdd::Redo(){
	pSkin->AddTexture( pTexture );
	pSkin->SetActiveTexture( pTexture );
}
