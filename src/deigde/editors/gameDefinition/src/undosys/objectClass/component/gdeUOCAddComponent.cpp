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

#include "gdeUOCAddComponent.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/component/gdeOCComponent.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCAddComponent
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCAddComponent::gdeUOCAddComponent( gdeObjectClass *objectClass, gdeOCComponent *component ) :
pObjectClass( NULL ),
pComponent( NULL )
{
	if( ! objectClass || ! component ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add component" );
	
	pComponent = component;
	component->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCAddComponent::~gdeUOCAddComponent(){
	if( pComponent ){
		pComponent->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCAddComponent::Undo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCComponent() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCComponent ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCComponent( NULL );
	}
	
	pObjectClass->GetComponents().Remove( pComponent );
	pObjectClass->NotifyComponentsChanged();
}

void gdeUOCAddComponent::Redo(){
	pObjectClass->GetComponents().Add( pComponent );
	pObjectClass->NotifyComponentsChanged();
}
