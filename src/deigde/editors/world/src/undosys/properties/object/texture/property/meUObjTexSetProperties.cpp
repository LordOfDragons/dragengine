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

#include "meUObjTexSetProperties.h"
#include "../../../../../world/meWorld.h"
#include "../../../../../world/object/meObject.h"
#include "../../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjTexSetProperties
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjTexSetProperties::meUObjTexSetProperties( meObjectTexture *texture,
const decStringDictionary &newValues ) :
pTexture( texture ),
pNewValues( newValues )
{
	if( ! texture || ! texture->GetObject() || ! texture->GetObject()->GetWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object texture set properties" );
	
	pOldValues = texture->GetProperties();
	
	pTexture = texture;
	texture->AddReference();
}

meUObjTexSetProperties::~meUObjTexSetProperties(){
	if( pTexture ){
		pTexture->FreeReference();
	}
}



// Management
///////////////

void meUObjTexSetProperties::Undo(){
	pTexture->SetProperties( pOldValues );
}

void meUObjTexSetProperties::Redo(){
	pTexture->SetProperties( pNewValues );
}
