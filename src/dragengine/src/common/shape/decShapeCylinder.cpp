/* 
 * Drag[en]gine Game Engine
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
