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

#include "decShape.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShape
///////////////////

// Constructors, destructors
//////////////////////////////

decShape::decShape(){
}

decShape::decShape( const decVector &position ) :
pPosition( position ){
}

decShape::decShape( const decVector &position, const decQuaternion &orientation ) :
pPosition( position ),
pOrientation( orientation ){
}

decShape::~decShape(){
}



// Management
///////////////

void decShape::SetPosition( const decVector &position ){
	pPosition = position;
}

void decShape::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}



decShape *decShape::Copy() const{
	return new decShape( pPosition, pOrientation );
}



// Visiting
/////////////

void decShape::Visit( decShapeVisitor &visitor ){
	visitor.VisitShape( *this );
}
