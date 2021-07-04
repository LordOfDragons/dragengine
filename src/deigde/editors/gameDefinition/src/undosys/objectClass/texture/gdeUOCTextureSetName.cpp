/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "gdeUOCTextureSetName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/component/gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCTextureSetName
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCTextureSetName::gdeUOCTextureSetName( gdeObjectClass *objectClass,
gdeOCComponentTexture* texture, const char *newValue ) :
pObjectClass( NULL ),
pTexture( NULL )
{
	if( ! objectClass || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Texture set name" );
	
	pOldValue = texture->GetName();
	pNewValue = newValue;
	
	pTexture = texture;
	texture->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCTextureSetName::~gdeUOCTextureSetName(){
	if( pTexture ){
		pTexture->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCTextureSetName::Undo(){
	pTexture->SetName( pOldValue );
	pObjectClass->NotifyTextureChanged( pTexture );
}

void gdeUOCTextureSetName::Redo(){
	pTexture->SetName( pNewValue );
	pObjectClass->NotifyTextureChanged( pTexture );
}
