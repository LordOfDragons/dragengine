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

#include "decShapeBox.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShapeBox
//////////////////////

// Constructors, destructors
//////////////////////////////

decShapeBox::decShapeBox( const decVector &halfExtends ){
	SetHalfExtends( halfExtends );
	pTapering.Set( 1.0f, 1.0f );
}

decShapeBox::decShapeBox( const decVector &halfExtends, const decVector &position ) : decShape( position ){
	SetHalfExtends( halfExtends );
	pTapering.Set( 1.0f, 1.0f );
}

decShapeBox::decShapeBox( const decVector &halfExtends, const decVector &position,
const decQuaternion &orientation ) : decShape( position, orientation ){
	SetHalfExtends( halfExtends );
	pTapering.Set( 1.0f, 1.0f );
}

decShapeBox::decShapeBox( const decVector &halfExtends, const decVector2 &tapering ){
	SetHalfExtends( halfExtends );
	SetTapering( tapering );
}

decShapeBox::decShapeBox( const decVector &halfExtends, const decVector2 &tapering,
const decVector &position ) : decShape( position ){
	SetHalfExtends( halfExtends );
	SetTapering( tapering );
}

decShapeBox::decShapeBox( const decVector &halfExtends, const decVector2 &tapering,
const decVector &position, const decQuaternion &orientation ) : decShape( position, orientation ){
	SetHalfExtends( halfExtends );
	SetTapering( tapering );
}

decShapeBox::~decShapeBox(){
}



// Management
///////////////

void decShapeBox::SetHalfExtends( const decVector &halfExtends ){
	pHalfExtends = halfExtends.Largest( decVector( 0.0f, 0.0f, 0.0f ) );
}

void decShapeBox::SetTapering( const decVector2 &tapering ){
	pTapering = tapering.Largest( decVector2( 0.0f, 0.0f ) );
}



decShape *decShapeBox::Copy() const{
	return new decShapeBox( pHalfExtends, pTapering, GetPosition(), GetOrientation() );
}



// Visiting
/////////////

void decShapeBox::Visit( decShapeVisitor &visitor ){
	visitor.VisitShapeBox( *this );
}
