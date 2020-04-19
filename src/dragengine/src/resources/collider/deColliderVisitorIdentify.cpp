/* 
 * Drag[en]gine Game Engine
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

#include "deCollider.h"
#include "deColliderVolume.h"
#include "deColliderRig.h"
#include "deColliderComponent.h"
#include "deColliderVisitorIdentify.h"
#include "../../common/exceptions.h"



// Class deColliderVisitorIdentify
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderVisitorIdentify::deColliderVisitorIdentify() :
pCollider( NULL ),
pType( ectUnknown ){
}

deColliderVisitorIdentify::~deColliderVisitorIdentify(){
}



// Management
///////////////

deColliderVolume &deColliderVisitorIdentify::CastToVolume() const{
	if( pType != ectVolume ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deColliderVolume* )pCollider );
}

deColliderRig &deColliderVisitorIdentify::CastToRig() const{
	if( pType != ectRig ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deColliderRig* )pCollider );
}

deColliderComponent &deColliderVisitorIdentify::CastToComponent() const{
	if( pType != ectComponent ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deColliderComponent* )pCollider );
}



void deColliderVisitorIdentify::Reset(){
	pCollider = NULL;
	pType = ectUnknown;
}



// Visiting
/////////////

void deColliderVisitorIdentify::VisitCollider( deCollider &collider ){
	pCollider = &collider;
	pType = ectUnknown;
}

void deColliderVisitorIdentify::VisitVolume( deColliderVolume &collider ){
	pCollider = &collider;
	pType = ectVolume;
}

void deColliderVisitorIdentify::VisitRig( deColliderRig &collider ){
	pCollider = &collider;
	pType = ectRig;
}

void deColliderVisitorIdentify::VisitComponent( deColliderComponent &collider ){
	pCollider = &collider;
	pType = ectComponent;
}
