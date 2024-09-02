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

#include "gdeOCSnapPoint.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCSnapPoint
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSnapPoint::gdeOCSnapPoint() :
pSnapDistance( 0.1f ),
pSnapToRotation( true ){
}

gdeOCSnapPoint::gdeOCSnapPoint( const gdeOCSnapPoint &snapPoint ) :
pName( snapPoint.pName ),

pPosition( snapPoint.pPosition ),
pRotation( snapPoint.pRotation ),

pSnapDistance( snapPoint.pSnapDistance ),
pSnapToRotation( snapPoint.pSnapToRotation ){
}

gdeOCSnapPoint::~gdeOCSnapPoint(){
}



// Management
///////////////

void gdeOCSnapPoint::SetName( const char *name ){
	pName = name;
}



void gdeOCSnapPoint::SetPosition( const decDVector &position ){
	pPosition = position;
}

void gdeOCSnapPoint::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}



void gdeOCSnapPoint::SetSnapDistance( float distance ){
	pSnapDistance = decMath::max( distance, 0.0f );
}

void gdeOCSnapPoint::SetSnapToRotation( bool snapToRotation ){
	pSnapToRotation = snapToRotation;
}
