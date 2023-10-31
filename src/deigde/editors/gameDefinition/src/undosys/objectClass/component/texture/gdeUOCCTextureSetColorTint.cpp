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

#include "gdeUOCCTextureSetColorTint.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCCTextureSetColorTint
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCCTextureSetColorTint::gdeUOCCTextureSetColorTint( gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture* texture, const decColor &newValue ) :
pObjectClass( NULL ),
pComponent( NULL ),
pTexture( NULL )
{
	if( ! objectClass || ! component || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Component texture set color tint" );
	
	pOldValue = texture->GetColorTint();
	pNewValue = newValue;
	
	pTexture = texture;
	texture->AddReference();
	
	pComponent = component;
	component->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCCTextureSetColorTint::~gdeUOCCTextureSetColorTint(){
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

void gdeUOCCTextureSetColorTint::Undo(){
	pTexture->SetColorTint( pOldValue );
	pObjectClass->NotifyComponentTextureChanged( pComponent, pTexture );
}

void gdeUOCCTextureSetColorTint::Redo(){
	pTexture->SetColorTint( pNewValue );
	pObjectClass->NotifyComponentTextureChanged( pComponent, pTexture );
}
