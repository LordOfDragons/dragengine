/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "sePropertyNodeShape.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class sePropertyNodeShape
//////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeShape::sePropertyNodeShape( deEngine &engine ) :
sePropertyNode( entShape, engine, ShapeMappedCount ),
pShapeType( deSkinPropertyNodeShape::estRectangle ),
pThickness( 1.0f ){
}

sePropertyNodeShape::sePropertyNodeShape( const sePropertyNodeShape &node ) :
sePropertyNode( node ),
pShapeType( node.pShapeType ),
pFillColor( node.pFillColor ),
pLineColor( node.pLineColor ),
pThickness( node.pThickness ){
}

sePropertyNodeShape::~sePropertyNodeShape(){
}



// Management
///////////////

void sePropertyNodeShape::SetShapeType( deSkinPropertyNodeShape::eShapeTypes type ){
	if( type < deSkinPropertyNodeShape::estRectangle || type > deSkinPropertyNodeShape::estEllipse ){
		DETHROW( deeInvalidParam );
	}
	
	if( type == pShapeType ){
		return;
	}
	
	pShapeType = type;
	NotifyChanged();
}

void sePropertyNodeShape::SetFillColor( const decColor &color ){
	if( color.IsEqualTo( pFillColor ) ){
		return;
	}
	
	pFillColor = color;
	NotifyChanged();
}

void sePropertyNodeShape::SetLineColor( const decColor &color ){
	if( color.IsEqualTo( pLineColor ) ){
		return;
	}
	
	pLineColor = color;
	NotifyChanged();
}

void sePropertyNodeShape::SetThickness( float thickness ){
	thickness = decMath::max( thickness, 0.0f );
	if( fabsf( thickness - pThickness ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pThickness = thickness;
	NotifyChanged();
}



sePropertyNode *sePropertyNodeShape::Copy() const{
	return new sePropertyNodeShape( *this );
}
