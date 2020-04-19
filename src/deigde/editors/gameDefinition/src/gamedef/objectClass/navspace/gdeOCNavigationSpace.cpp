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
#include <stdlib.h>
#include <string.h>

#include "gdeOCNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCNavigationSpace
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCNavigationSpace::gdeOCNavigationSpace() :
pLayer( 0 ),
pSnapDistance( 0.001f ),
pSnapAngle( 180.0 ),
pBlockingPriority( 0 ){
}

gdeOCNavigationSpace::gdeOCNavigationSpace( const gdeOCNavigationSpace &space ) :
pPath( space.pPath ),
pPosition( space.pPosition ),
pRotation( space.pRotation ),
pBoneName( space.pBoneName ),
pLayer( space.pLayer ),
pSnapDistance( space. pSnapDistance ),
pSnapAngle( space.pSnapAngle ),
pBlockingPriority( space.pBlockingPriority ),
pBlockerShapeList( space.pBlockerShapeList )
{
	int i;
	
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = space.pPropertyNames[ i ];
	}
}

gdeOCNavigationSpace::~gdeOCNavigationSpace(){
}



// Management
///////////////

void gdeOCNavigationSpace::SetPath( const char *path ){
	pPath = path;
}

void gdeOCNavigationSpace::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCNavigationSpace::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCNavigationSpace::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}



void gdeOCNavigationSpace::SetLayer( int layer ){
	pLayer = layer;
}

void gdeOCNavigationSpace::SetBlockingPriority( int priority ){
	pBlockingPriority = priority;
}



void gdeOCNavigationSpace::SetSnapDistance( float distance ){
	pSnapDistance = decMath::max( distance, 0.001f );
}

void gdeOCNavigationSpace::SetSnapAngle( float angle ){
	pSnapAngle = decMath::max( angle, 0.0f );
}



bool gdeOCNavigationSpace::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCNavigationSpace::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void gdeOCNavigationSpace::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool gdeOCNavigationSpace::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}
