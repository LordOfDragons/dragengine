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
#include <stdlib.h>
#include <string.h>

#include "deSkinPropertyNodeShape.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeShape
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeShape::deSkinPropertyNodeShape() :
pShapeType( estRectangle ),
pThickness( 1.0f ){
}

deSkinPropertyNodeShape::~deSkinPropertyNodeShape(){
}



// Management
///////////////

void deSkinPropertyNodeShape::SetShapeType( eShapeTypes type ){
	if( type < estRectangle || type > estEllipse ){
		DETHROW( deeInvalidParam );
	}
	pShapeType = type;
}

void deSkinPropertyNodeShape::SetFillColor( const decColor &color ){
	pFillColor = color;
}

void deSkinPropertyNodeShape::SetLineColor( const decColor &color ){
	pLineColor = color;
}

void deSkinPropertyNodeShape::SetThickness( float thickness ){
	pThickness = decMath::max( thickness, 0.0f );
}



// Visiting
/////////////

void deSkinPropertyNodeShape::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitShape( *this );
}
