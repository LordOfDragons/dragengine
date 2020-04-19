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

#include "meColliderOwner.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class meColliderOwner
//////////////////////////

// Constructor, destructor
////////////////////////////

meColliderOwner::meColliderOwner( meObject *object ) :
pObject( object ),
pShape( NULL ),
pSnapPoint( NULL ),
pDecal( NULL ),
pNavigationSpace( NULL )
{
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
}

meColliderOwner::meColliderOwner( meObjectShape *shape ) :
pObject( NULL ),
pShape( shape ),
pSnapPoint( NULL ),
pDecal( NULL ),
pNavigationSpace( NULL )
{
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
}

meColliderOwner::meColliderOwner( meObjectSnapPoint *snapPoint ) :
pObject( NULL ),
pShape( NULL ),
pSnapPoint( snapPoint ),
pDecal( NULL ),
pNavigationSpace( NULL )
{
	if( ! snapPoint ){
		DETHROW( deeInvalidParam );
	}
}

meColliderOwner::meColliderOwner( meDecal *decal ) :
pObject( NULL ),
pShape( NULL ),
pSnapPoint( NULL ),
pDecal( decal ),
pNavigationSpace( NULL )
{
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
}

meColliderOwner::meColliderOwner( meNavigationSpace *navspace ) :
pObject( NULL ),
pShape( NULL ),
pSnapPoint( NULL ),
pDecal( NULL ),
pNavigationSpace( navspace )
{
	if( ! navspace ){
		DETHROW( deeInvalidParam );
	}
}



// Management
///////////////

const meColliderOwner *meColliderOwner::GetColliderOwner(
igdeEnvironment &environment, deCollider *collider ){
	if( ! collider ){
		return NULL;
	}
	
	return ( const meColliderOwner* )environment.GetColliderUserPointer( collider );
}
