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
