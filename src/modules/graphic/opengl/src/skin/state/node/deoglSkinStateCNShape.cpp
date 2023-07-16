/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinStateCNShape.h"
#include "../deoglSkinState.h"
#include "../deoglSkinStateMapped.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>


// Class deoglSkinStateCNShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCNShape::deoglSkinStateCNShape( deSkinPropertyNodeShape &node ) :
deoglSkinStateConstructedNode( node, etShape ),
pShapeType( node.GetShapeType() ),
pFillColor( node.GetFillColor() ),
pLineColor( node.GetLineColor() ),
pThickness( node.GetThickness() )
{
	int i;
	for( i=0; i<deSkinPropertyNodeShape::ShapeMappedCount; i++ ){
		pShapeMapped[ i ] = node.GetShapeMappedFor( ( deSkinPropertyNodeShape::eShapeMapped )i );
	}
}

deoglSkinStateCNShape::deoglSkinStateCNShape( const deoglSkinStateCNShape &node ) :
deoglSkinStateConstructedNode( node ),
pShapeType( node.pShapeType ),
pFillColor( node.pFillColor ),
pLineColor( node.pLineColor ),
pThickness( node.pThickness )
{
	int i;
	for( i=0; i<deSkinPropertyNodeShape::ShapeMappedCount; i++ ){
		pShapeMapped[ i ] = node.pShapeMapped[ i ];
	}
}

deoglSkinStateCNShape::~deoglSkinStateCNShape(){
}



// Management
///////////////

int deoglSkinStateCNShape::GetShapeMappedFor( deSkinPropertyNodeShape::eShapeMapped type ) const{
	return pShapeMapped[ type ];
}



void deoglSkinStateCNShape::Update( deoglSkinState &state ){
	if( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorRed ] != -1 ){
		pFillColor.r = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorRed ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorGreen ] != -1 ){
		pFillColor.g = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorGreen ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorBlue ] != -1 ){
		pFillColor.b = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorBlue ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorAlpha ] != -1 ){
		pFillColor.a = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmFillColorAlpha ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorRed ] != -1 ){
		pLineColor.r = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorRed ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorGreen ] != -1 ){
		pLineColor.g = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorGreen ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorBlue ] != -1 ){
		pLineColor.b = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorBlue ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorAlpha ] != -1 ){
		pLineColor.a = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmLineColorAlpha ] ).GetValue();
	}
	if( pShapeMapped[ deSkinPropertyNodeShape::esmThickness ] != -1 ){
		pThickness = state.GetMappedAt( pShapeMapped[ deSkinPropertyNodeShape::esmThickness ] ).GetValue();
	}
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNShape::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNShape( *this ) );
}
