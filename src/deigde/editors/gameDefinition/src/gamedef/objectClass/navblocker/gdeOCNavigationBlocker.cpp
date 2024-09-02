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
