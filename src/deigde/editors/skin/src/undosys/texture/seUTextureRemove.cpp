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

#include "seUTextureRemove.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"

#include <dragengine/common/exceptions.h>



// Class seUTextureRemove
///////////////////////////

// Constructor, destructor
////////////////////////////

seUTextureRemove::seUTextureRemove( seTexture *texture ){
	if( ! texture ) DETHROW( deeInvalidParam );
	
	seSkin *skin = texture->GetSkin();
	if( ! skin ) DETHROW( deeInvalidParam );
	
	pTexture = NULL;
	pSkin = NULL;
	
	SetShortInfo( "Remove Texture" );
	
	pTexture = texture;
	texture->AddReference();
	
	pSkin = skin;
	skin->AddReference();
}

seUTextureRemove::~seUTextureRemove(){
	if( pSkin ) pSkin->FreeReference();
	if( pTexture ) pTexture->FreeReference();
}



// Management
///////////////

void seUTextureRemove::Undo(){
	pSkin->AddTexture( pTexture );
}

void seUTextureRemove::Redo(){
	pSkin->RemoveTexture( pTexture );
}
