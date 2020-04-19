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

#include "seUTextureImport.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/seSkin.h"
#include "../../skin/property/seProperty.h"

#include <dragengine/common/exceptions.h>



// Class seUTextureImport
///////////////////////////

// Constructor, destructor
////////////////////////////

seUTextureImport::seUTextureImport( seTexture *texture, const seTexture *importTexture ){
	if( ! texture || ! importTexture ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	
	SetShortInfo( "Texture import" );
	
	pOldProperties = texture->GetPropertyList();
	pNewProperties = importTexture->GetPropertyList();
	
	pTexture = texture;
	texture->AddReference();
}

seUTextureImport::~seUTextureImport(){
	if( pTexture ){
		pTexture->FreeReference();
	}
}



// Management
///////////////

void seUTextureImport::Undo(){
	const int propertyCount = pOldProperties.GetCount();
	int i;
	
	pTexture->RemoveAllProperties();
	for( i=0; i<propertyCount; i++ ){
		pTexture->AddProperty( pOldProperties.GetAt( i ) );
	}
}

void seUTextureImport::Redo(){
	const int propertyCount = pNewProperties.GetCount();
	int i;
	
	pTexture->RemoveAllProperties();
	for( i=0; i<propertyCount; i++ ){
		pTexture->AddProperty( pNewProperties.GetAt( i ) );
	}
}
