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

#include "gdeOCNavigationBlocker.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCNavigationBlocker
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCNavigationBlocker::gdeOCNavigationBlocker() :
pScaling( 1.0f, 1.0f, 1.0f ),
pEnabled( true ),
pLayer( 0 ),
pType( deNavigationSpace::estMesh ),
pBlockingPriority( 0 ){
}

gdeOCNavigationBlocker::gdeOCNavigationBlocker( const gdeOCNavigationBlocker &blocker ) :
pPosition( blocker.pPosition ),
pRotation( blocker.pRotation ),
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

gdeOCNavigationBlocker::~gdeOCNavigationBlocker(){
}



// Management
///////////////

void gdeOCNavigationBlocker::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCNavigationBlocker::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCNavigationBlocker::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}

void gdeOCNavigationBlocker::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void gdeOCNavigationBlocker::SetEnabled( bool enabled ){
	pEnabled = enabled;
}

void gdeOCNavigationBlocker::SetLayer( int layer ){
	pLayer = layer;
}

void gdeOCNavigationBlocker::SetType( deNavigationSpace::eSpaceTypes type ){
	pType = type;
}

void gdeOCNavigationBlocker::SetBlockingPriority( int priority ){
	pBlockingPriority = priority;
}



bool gdeOCNavigationBlocker::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCNavigationBlocker::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void gdeOCNavigationBlocker::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool gdeOCNavigationBlocker::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}
