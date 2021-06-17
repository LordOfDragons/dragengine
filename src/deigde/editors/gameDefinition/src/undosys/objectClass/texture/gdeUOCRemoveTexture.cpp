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

#include "gdeUOCRemoveTexture.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../gamedef/objectClass/component/gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCRemoveTexture
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCRemoveTexture::gdeUOCRemoveTexture( gdeObjectClass *objectClass, gdeOCComponentTexture *texture ) :
pObjectClass( NULL ),
pTexture( NULL )
{
	if( ! objectClass || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! objectClass->GetTextures().Has( texture ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove texture" );
	
	pTexture = texture;
	texture->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCRemoveTexture::~gdeUOCRemoveTexture(){
	if( pTexture ){
		pTexture->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCRemoveTexture::Undo(){
	pObjectClass->GetTextures().Add( pTexture );
	pObjectClass->NotifyTexturesChanged();
}

void gdeUOCRemoveTexture::Redo(){
	gdeOCComponentTextureList &list = pObjectClass->GetTextures();
	
	if( pTexture == pObjectClass->GetActiveTexture() ){
		if( list.GetCount() > 1 ){
			if( list.GetAt( 0 ) == pTexture ){
				pObjectClass->SetActiveTexture( list.GetAt( 1 ) );
				
			}else{
				pObjectClass->SetActiveTexture( list.GetAt( 0 ) );
			}
			
		}else{
			pObjectClass->SetActiveTexture( NULL );
		}
		
		pObjectClass->GetGameDefinition()->NotifyOCActiveTextureChanged( pObjectClass );
	}
	
	list.Remove( pTexture );
	pObjectClass->NotifyTexturesChanged();
}
