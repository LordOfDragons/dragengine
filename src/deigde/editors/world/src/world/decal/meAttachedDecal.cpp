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

#include "meDecal.h"
#include "meAttachedDecal.h"
#include "../object/meObject.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/decal/deDecalManager.h>
#include <dragengine/common/exceptions.h>
#include <deigde/gui/wrapper/igdeWObject.h>



// Class meAttachedDecal
//////////////////////////

// Constructor, destructor
////////////////////////////

meAttachedDecal::meAttachedDecal( deEngine *engine, meDecal *decal ) :
pEngine( engine ),
pDecal( decal ),
pParentObject( NULL ),
pAttached( false )
{
	if( ! engine || ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	pEngDecal.TakeOver( engine->GetDecalManager()->CreateDecal() );
}

meAttachedDecal::~meAttachedDecal(){
	RemoveFromParent();
	if( pParentObject ){
		pParentObject->FreeReference();
	}
}



// Management
///////////////

void meAttachedDecal::SetParentObject( meObject *object ){
	if( object == pParentObject ){
		return;
	}
	
	RemoveFromParent();
	
	if( pParentObject ){
		pParentObject->FreeReference();
		pParentObject = NULL;
	}
	
	pParentObject = object;
	
	if( object ){
		object->AddReference();
	}
	
	AttachToParent();
}



void meAttachedDecal::AttachToParent(){
	RemoveFromParent(); // just to make sure
	
	if( ! pParentObject ){
		return;
	}
	
	deComponent * const engComponent = pParentObject->GetObjectWrapper()->GetComponent();
	if( ! engComponent ){
		return;
	}
	
	const decDMatrix matrix(
		decDMatrix::CreateRT( pDecal->GetRotation() * DEG2RAD, pDecal->GetPosition() )
		* decDMatrix::CreateRT( pParentObject->GetRotation() * DEG2RAD, pParentObject->GetPosition() ).Invert() );
	const decVector size( decVector( 0.001f, 0.001f, 0.001f ).Largest( pDecal->GetSize() ) );
	
	pEngDecal->SetPosition( matrix.GetPosition().ToVector() );
	pEngDecal->SetOrientation( matrix.ToQuaternion() );
	pEngDecal->SetSize( size );
	
	engComponent->AddDecal( pEngDecal );
	pAttached = true;
}

void meAttachedDecal::RemoveFromParent(){
	if( ! pAttached ){
		return;
	}
	
	if( pParentObject ){
		deComponent * const engComponent = pParentObject->GetObjectWrapper()->GetComponent();
		if( engComponent ){
			engComponent->RemoveDecal( pEngDecal );
		}
	}
	
	pAttached = false;
}
