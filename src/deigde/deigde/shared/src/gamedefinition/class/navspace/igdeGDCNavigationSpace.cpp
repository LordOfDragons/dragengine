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

#include "igdeGDCNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCNavigationSpace
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCNavigationSpace::igdeGDCNavigationSpace() :
pLayer( 0 ),
pSnapDistance( 0.001f ),
pSnapAngle( 180.0 ),
pBlockingPriority( 0 ){
}

igdeGDCNavigationSpace::igdeGDCNavigationSpace( const igdeGDCNavigationSpace &navspace ) :
pPath( navspace.pPath ),
pPosition( navspace.pPosition ),
pOrientation( navspace.pOrientation ),
pBoneName( navspace.pBoneName ),
pLayer( navspace.pLayer ),
pSnapDistance( navspace. pSnapDistance ),
pSnapAngle( navspace.pSnapAngle ),
pBlockingPriority( navspace.pBlockingPriority ),
pBlockerShapeList( navspace.pBlockerShapeList )
{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = navspace.pPropertyNames[ i ];
	}
}

igdeGDCNavigationSpace::~igdeGDCNavigationSpace(){
}



// Management
///////////////

void igdeGDCNavigationSpace::SetPath( const char *path ){
	pPath = path;
}

void igdeGDCNavigationSpace::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCNavigationSpace::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCNavigationSpace::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}



void igdeGDCNavigationSpace::SetLayer( int layer ){
	pLayer = layer;
}

void igdeGDCNavigationSpace::SetBlockingPriority( int priority ){
	pBlockingPriority = priority;
}



void igdeGDCNavigationSpace::SetBlockerShapeList( const decShapeList &shapeList ){
	pBlockerShapeList = shapeList;
}



void igdeGDCNavigationSpace::SetSnapDistance( float distance ){
	pSnapDistance = decMath::max( distance, 0.001f );
}

void igdeGDCNavigationSpace::SetSnapAngle( float angle ){
	pSnapAngle = decMath::max( angle, 0.0f );
}



bool igdeGDCNavigationSpace::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCNavigationSpace::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCNavigationSpace::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool igdeGDCNavigationSpace::HasPropertyWithName( const char *name ) const{
	int j;
	for( j=0; j<=epAttachRotation; j++ ){
		if( pPropertyNames[ j ] == name ){
			return true;
		}
	}
	
	return false;
}
