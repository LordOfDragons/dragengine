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

#include "decShapeCylinder.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShapeCylinder
//////////////////////////

// Constructors, destructors
//////////////////////////////

decShapeCylinder::decShapeCylinder( float halfHeight, float radius ){
	SetHalfHeight( halfHeight );
	SetRadius( radius );
	pTopAxisScaling.Set( 1.0f, 1.0f );
	pBottomAxisScaling.Set( 1.0f, 1.0f );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float radius, const decVector &position ) : decShape( position ){
	SetHalfHeight( halfHeight );
	SetRadius( radius );
	pTopAxisScaling.Set( 1.0f, 1.0f );
	pBottomAxisScaling.Set( 1.0f, 1.0f );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float radius, const decVector &position,
const decQuaternion &orientation ) : decShape( position, orientation ){
	SetHalfHeight( halfHeight );
	SetRadius( radius );
	pTopAxisScaling.Set( 1.0f, 1.0f );
	pBottomAxisScaling.Set( 1.0f, 1.0f );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float topRadius, float bottomRadius ){
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	pTopAxisScaling.Set( 1.0f, 1.0f );
	pBottomAxisScaling.Set( 1.0f, 1.0f );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float topRadius, float bottomRadius, const decVector &position ) : decShape( position ){
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	pTopAxisScaling.Set( 1.0f, 1.0f );
	pBottomAxisScaling.Set( 1.0f, 1.0f );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float topRadius, float bottomRadius,
const decVector &position, const decQuaternion &orientation ) : decShape( position, orientation ){
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	pTopAxisScaling.Set( 1.0f, 1.0f );
	pBottomAxisScaling.Set( 1.0f, 1.0f );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float topRadius, float bottomRadius, const decVector2 &topAxisScaling,
const decVector2 &bottomAxisScaling ){
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetTopAxisScaling( topAxisScaling );
	SetBottomAxisScaling( bottomAxisScaling );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float topRadius, float bottomRadius, const decVector2 &topAxisScaling,
const decVector2 &bottomAxisScaling, const decVector &position ) : decShape( position ){
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetTopAxisScaling( topAxisScaling );
	SetBottomAxisScaling( bottomAxisScaling );
}

decShapeCylinder::decShapeCylinder( float halfHeight, float topRadius, float bottomRadius, const decVector2 &topAxisScaling,
const decVector2 &bottomAxisScaling, const decVector &position, const decQuaternion &orientation ) : decShape( position, orientation ){
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetTopAxisScaling( topAxisScaling );
	SetBottomAxisScaling( bottomAxisScaling );
}

decShapeCylinder::~decShapeCylinder(){
}



// Management
///////////////

void decShapeCylinder::SetHalfHeight( float halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0f );
}



void decShapeCylinder::SetTopRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
}

void decShapeCylinder::SetBottomRadius( float radius ){
	pBottomRadius = decMath::max( radius, 0.0f );
}

void decShapeCylinder::SetRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
	pBottomRadius = pTopRadius;
}

void decShapeCylinder::SetTopAxisScaling( const decVector2 &axisScaling ){
	pTopAxisScaling = axisScaling.Largest( decVector2( 0.0f, 0.0f ) );
}

void decShapeCylinder::SetBottomAxisScaling( const decVector2 &axisScaling ){
	pBottomAxisScaling = axisScaling.Largest( decVector2( 0.0f, 0.0f ) );
}



decShape *decShapeCylinder::Copy() const{
	return new decShapeCylinder( pHalfHeight, pTopRadius, pBottomRadius,
		pTopAxisScaling, pBottomAxisScaling, GetPosition(), GetOrientation() );
}



// Visiting
/////////////

void decShapeCylinder::Visit( decShapeVisitor &visitor ){
	visitor.VisitShapeCylinder( *this );
}
