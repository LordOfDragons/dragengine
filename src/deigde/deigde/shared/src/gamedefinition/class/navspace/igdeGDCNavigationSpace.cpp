/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
pType( deNavigationSpace::estMesh ),
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
pType( navspace.pType ),
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

void igdeGDCNavigationSpace::SetType( deNavigationSpace::eSpaceTypes type ){
	pType = type;
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
