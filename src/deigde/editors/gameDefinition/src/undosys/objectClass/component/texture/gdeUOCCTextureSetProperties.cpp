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

#include "gdeUOCCTextureSetProperties.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCCTextureSetProperties
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCCTextureSetProperties::gdeUOCCTextureSetProperties( gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture* texture, const decStringDictionary &newValue ) :
pObjectClass( NULL ),
pComponent( NULL ),
pTexture( NULL )
{
	if( ! objectClass || ! component || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Component texture set properties" );
	
	pOldValue = texture->GetProperties();
	pNewValue = newValue;
	
	pTexture = texture;
	texture->AddReference();
	
	pComponent = component;
	component->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCCTextureSetProperties::~gdeUOCCTextureSetProperties(){
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

void gdeUOCCTextureSetProperties::Undo(){
	if( ! pOldValue.Has( pTexture->GetActiveProperty() ) ){
		pTexture->SetActiveProperty( "" );
	}
	
	pTexture->GetProperties() = pOldValue;
	pObjectClass->NotifyComponentTexturePropertiesChanged( pComponent, pTexture );
}

void gdeUOCCTextureSetProperties::Redo(){
	if( ! pNewValue.Has( pTexture->GetActiveProperty() ) ){
		pTexture->SetActiveProperty( "" );
	}
	
	pTexture->GetProperties() = pNewValue;
	pObjectClass->NotifyComponentTexturePropertiesChanged( pComponent, pTexture );
}
