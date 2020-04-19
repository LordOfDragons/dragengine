/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCNavigationBlocker.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCNavigationBlocker
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCNavigationBlocker::igdeGDCNavigationBlocker() :
pScaling( 1.0f, 1.0f, 1.0f ),
pEnabled( true ),
pLayer( 0 ),
pType( deNavigationSpace::estMesh ),
pBlockingPriority( 0 ){
}

igdeGDCNavigationBlocker::igdeGDCNavigationBlocker( const igdeGDCNavigationBlocker &blocker ) :
pPosition( blocker.pPosition ),
pOrientation( blocker.pOrientation ),
pScaling( blocker.pScaling ),
pBoneName( blocker.pBoneName ),
pEnabled( blocker.pEnabled ),
pLayer( blocker.pLayer ),
pType( blocker.pType ),
pBlockingPriority( blocker.pBlockingPriority ),
pShapeList( blocker.pShapeList )
{
	int i;
	
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = blocker.pPropertyNames[ i ];
	}
}

igdeGDCNavigationBlocker::~igdeGDCNavigationBlocker(){
}



// Management
///////////////

void igdeGDCNavigationBlocker::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCNavigationBlocker::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCNavigationBlocker::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}

void igdeGDCNavigationBlocker::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void igdeGDCNavigationBlocker::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



void igdeGDCNavigationBlocker::SetLayer( int layer ){
	pLayer = layer;
}

void igdeGDCNavigationBlocker::SetType( deNavigationSpace::eSpaceTypes type ){
	pType = type;
}

void igdeGDCNavigationBlocker::SetBlockingPriority( int priority ){
	pBlockingPriority = priority;
}



void igdeGDCNavigationBlocker::SetShapeList( const decShapeList &shapeList ){
	pShapeList = shapeList;
}



bool igdeGDCNavigationBlocker::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCNavigationBlocker::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCNavigationBlocker::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool igdeGDCNavigationBlocker::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}
