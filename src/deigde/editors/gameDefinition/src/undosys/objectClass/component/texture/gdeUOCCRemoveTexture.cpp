/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeUOCCRemoveTexture.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCCRemoveTexture
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCCRemoveTexture::gdeUOCCRemoveTexture( gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture ) :
pObjectClass( NULL ),
pComponent( NULL ),
pTexture( NULL )
{
	if( ! objectClass || ! component || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! objectClass->GetComponents().Has( component ) ){
		DETHROW( deeInvalidParam );
	}
	if( ! component->GetTextures().Has( texture ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Component remove texture" );
	
	pTexture = texture;
	texture->AddReference();
	
	pComponent = component;
	component->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCCRemoveTexture::~gdeUOCCRemoveTexture(){
	if( pTexture ){
		pTexture->FreeReference();
	}
	if( pComponent ){
		pComponent->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCCRemoveTexture::Undo(){
	pComponent->GetTextures().Add( pTexture );
	pObjectClass->NotifyComponentChanged( pComponent );
}

void gdeUOCCRemoveTexture::Redo(){
	gdeOCComponentTextureList &list = pComponent->GetTextures();
	
	if( pTexture == pComponent->GetActiveTexture() ){
		if( list.GetCount() > 1 ){
			if( list.GetAt( 0 ) == pTexture ){
				pComponent->SetActiveTexture( list.GetAt( 1 ) );
				
			}else{
				pComponent->SetActiveTexture( list.GetAt( 0 ) );
			}
			
		}else{
			pComponent->SetActiveTexture( NULL );
		}
		
		pObjectClass->GetGameDefinition()->NotifyOCComponentActiveTextureChanged(
			pObjectClass, pComponent );
	}
	
	list.Remove( pTexture );
	pObjectClass->NotifyComponentChanged( pComponent );
}
