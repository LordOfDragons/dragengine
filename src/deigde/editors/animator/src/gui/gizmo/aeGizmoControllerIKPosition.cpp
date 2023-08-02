/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "aeGizmoControllerIKPosition.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../visitors/aeElementVisitor.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class aeGizmoControllerIKPosition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeGizmoControllerIKPosition::aeGizmoControllerIKPosition(
	igdeEnvironment &environment, aeController &controller ) :
igdeGizmoMove( environment ),
pController( controller )
{
	SetScale( decVector( 0.25f, 0.25f, 0.25f ) );
	
	decLayerMask layerMask;
	layerMask.SetBit( aeAnimator::eclGizmo );
	SetCollisionFilter( decCollisionFilter( layerMask ) );
	
	SetColliderUserPointer( ( aeElementVisitable* )this );
}

aeGizmoControllerIKPosition::~aeGizmoControllerIKPosition(){
}



// Management
///////////////

decDVector aeGizmoControllerIKPosition::GetObjectPosition(){
	return decDVector( pController.GetVector() );
}

void aeGizmoControllerIKPosition::SetObjectPosition( const decDVector &position ){
	pController.SetVector( position.ToVector() );
}



// Visiting
/////////////

void aeGizmoControllerIKPosition::VisitElement( aeElementVisitor &visitor ){
	visitor.VisitGizmo( this );
}
