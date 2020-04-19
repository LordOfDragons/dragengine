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
