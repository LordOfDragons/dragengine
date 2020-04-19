/* 
 * Drag[en]gine IGDE World Editor
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
#include <string.h>

#include "meUObjectTextureColorTint.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectTextureColorTint
////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectTextureColorTint::meUObjectTextureColorTint( meObjectTexture *texture, const decColor &newColor ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	meObject * const object = texture->GetObject();
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = object->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Objext Texture Color Tint" );
	
	pTexture = NULL;
	
	pOldColor = texture->GetColorTint();
	pNewColor = newColor;
	
	pTexture = texture;
	texture->AddReference();
}

meUObjectTextureColorTint::~meUObjectTextureColorTint(){
	if( pTexture ){
		pTexture->FreeReference();
	}
}



// Management
///////////////

void meUObjectTextureColorTint::Undo(){
	pTexture->SetColorTint( pOldColor );
}

void meUObjectTextureColorTint::Redo(){
	pTexture->SetColorTint( pNewColor );
}
