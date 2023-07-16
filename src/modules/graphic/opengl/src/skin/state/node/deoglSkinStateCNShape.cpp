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
#include "../../../rendering/deoglRenderCanvasContext.h"
#include "../../../rendering/deoglRenderConstructed.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTBufferObject.h"
#include "../../../renderthread/deoglRTRenderers.h"
#include "../../../vbo/deoglSharedVBO.h"
#include "../../../vbo/deoglSharedVBOBlock.h"
#include "../../../vbo/deoglSharedVBOList.h"
#include "../../../vbo/writer/deoglVBOWriterCanvasPaint.h"

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
pThickness( node.GetThickness() ),

pIsThick( pThickness >= 1.5f ),

pVBOBlock( nullptr ),
pDirtyVBOBlock( true ),
pVBOBlockPointCount( 0 )
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
pThickness( node.pThickness ),

pIsThick( node.pIsThick ),

pVBOBlock( nullptr ),
pDirtyVBOBlock( true ),
pVBOBlockPointCount( 0 )
{
	int i;
	for( i=0; i<deSkinPropertyNodeShape::ShapeMappedCount; i++ ){
		pShapeMapped[ i ] = node.pShapeMapped[ i ];
	}
}

deoglSkinStateCNShape::~deoglSkinStateCNShape(){
	if( pVBOBlock ){
		pVBOBlock->DelayedRemove();
		pVBOBlock->FreeReference();
	}
}



// Management
///////////////

int deoglSkinStateCNShape::GetShapeMappedFor( deSkinPropertyNodeShape::eShapeMapped type ) const{
	return pShapeMapped[ type ];
}



void deoglSkinStateCNShape::Update( deoglSkinState &state ){
	deoglSkinStateConstructedNode::Update( state );
	
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
		pDirtyVBOBlock = true;
	}
}

void deoglSkinStateCNShape::PrepareForRender( deoglSkinState &state ){
	deoglSkinStateConstructedNode::PrepareForRender( state );
	
	if( pFillColor.a < FLOAT_SAFE_EPSILON && pLineColor.a < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPrepareVBOBlock( state );
}

void deoglSkinStateCNShape::Render( deoglSkinState &state, const deoglRenderCanvasContext &context ){
	if( pFillColor.a < FLOAT_SAFE_EPSILON && pLineColor.a < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	deoglSkinStateConstructedNode::Render( state, context );
	
	const deoglRenderCanvasContext paintContext( context, *this );
	state.GetRenderThread().GetRenderers().GetConstructed().DrawNodeShape( paintContext, *this );
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNShape::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNShape( *this ) );
}



// Private Functions
//////////////////////

int deoglSkinStateCNShape::pRequiredPointCount(){
	switch( pShapeType ){
	case deSkinPropertyNodeShape::estRectangle:
		return pIsThick ? 14 : 8;
		
	case deSkinPropertyNodeShape::estEllipse:{
		const int points = decMath::max( 4, ( int )( ( GetSize().x + GetSize().y ) * 2.0f / 5.0f ) );
		return pIsThick ? points * 3 + 2 : points * 2;
		}
	}
	
	return 0;
}

void deoglSkinStateCNShape::pPrepareVBOBlock( deoglSkinState &state ){
	const int pointCount = pRequiredPointCount();
	if( pointCount == 0 ){
		pDrawCountFill = 0;
		pDrawCountLine = 0;
		return;
	}
	
	if( pointCount > pVBOBlockPointCount ){
		pDirtyVBOBlock = true;
		if( pVBOBlock ){
			pVBOBlock->GetVBO()->RemoveBlock( pVBOBlock );
			pVBOBlock = NULL;
		}
	}
	
	if( ! pVBOBlock ){
		pVBOBlock = state.GetRenderThread().GetBufferObject().GetSharedVBOListForType(
			deoglRTBufferObject::esvbolCanvasPaint ).AddData( pointCount );
		pVBOBlockPointCount = pointCount;
		pDirtyVBOBlock = true;
	}
	
	if( pDirtyVBOBlock ){
		pWriteVBOData( state );
		pVBOBlock->SetValid( false );
		pDirtyVBOBlock = false;
	}
}

void deoglSkinStateCNShape::pWriteVBOData( deoglSkinState &state ){
	deoglVBOWriterCanvasPaint writer( state.GetRenderThread() );
	writer.Reset( pVBOBlock );
	
	const float thickness = pThickness > 0.5f ? decMath::max( pThickness, 1.0f ) : 0.0f;
	const decVector2 pointCenter( 0.5f, 0.5f );
	const decVector2 size( decPoint( pSize.x, pSize.y ) );
	int i;
	
	pDrawOffsetFill = 0;
	pDrawCountFill = 0;
	pDrawOffsetLine = 0;
	pDrawCountLine = 0;
	
	switch( pShapeType ){
	case deSkinPropertyNodeShape::estRectangle:{
		int cornerPointCount = 1;
		decVector2 cornerCenter;
		
		// fill
		const decVector2 cornerSizeInner( decVector2().Largest( cornerCenter - decVector2( thickness, thickness ) ) );
		decVector2 * const points = new decVector2[ cornerPointCount * 4 + ( cornerPointCount * 4 + 1 ) * 2 ];
		decVector2 * const inner = points;
		decVector2 * const outer = points + cornerPointCount * 4;
		
		if( cornerPointCount > 1 ){
			pCalcArc( inner, cornerCenter, cornerSizeInner,
				DEG2RAD * 270.0f, DEG2RAD * 180.0f, cornerPointCount );
			pCalcArc( inner + cornerPointCount,
				decVector2( size.x - cornerCenter.x, cornerCenter.y ), cornerSizeInner,
				DEG2RAD * 180.0f, DEG2RAD * 90.0f, cornerPointCount );
			pCalcArc( inner + cornerPointCount * 2, size - cornerCenter, cornerSizeInner,
				DEG2RAD * 90.0f, 0.0f, cornerPointCount );
			pCalcArc( inner + cornerPointCount * 3,
				decVector2( cornerCenter.x, size.y - cornerCenter.y ), cornerSizeInner,
				0.0f, DEG2RAD * -90.0f, cornerPointCount );
			
		}else{
			inner[ 0 ].Set( thickness, thickness );
			inner[ 1 ].Set( size.x - thickness, thickness );
			inner[ 2 ].Set( size.x - thickness, size.y - thickness );
			inner[ 3 ].Set( thickness, size.y - thickness );
		}
		
		for( i=0; i<cornerPointCount * 4; i++ ){
			writer.WritePoint( inner[ i ] );
		}
		
		pDrawModeFill = GL_TRIANGLE_FAN;
		pDrawCountFill = cornerPointCount * 4;
		
		// outline
		pDrawOffsetLine = pDrawCountFill;
		
		if( thickness > 0.5f ){
			const float geomTypeReduce = pIsThick ? 0.0f : 1.0f;
			
			if( cornerPointCount > 1 ){
				pCalcArc( outer, cornerCenter, cornerCenter,
					DEG2RAD * 270.0f, DEG2RAD * 180.0f, cornerPointCount );
				pCalcArc( outer + cornerPointCount,
					decVector2( size.x - geomTypeReduce - cornerCenter.x, cornerCenter.y ), cornerCenter,
					DEG2RAD * 180.0f, DEG2RAD * 90.0f, cornerPointCount );
				pCalcArc( outer + cornerPointCount * 2,
					size - decVector2( geomTypeReduce, geomTypeReduce ) - cornerCenter, cornerCenter,
					DEG2RAD * 90.0f, 0.0f, cornerPointCount );
				pCalcArc( outer + cornerPointCount * 3,
					decVector2( cornerCenter.x, size.y - geomTypeReduce - cornerCenter.y ), cornerCenter,
					0.0f, DEG2RAD * -90.0f, cornerPointCount );
				
			}else{
				outer[ 0 ].Set( 0.0f, 0.0f );
				outer[ 1 ].Set( size.x - geomTypeReduce, 0.0f );
				outer[ 2 ].Set( size.x - geomTypeReduce, size.y - geomTypeReduce );
				outer[ 3 ].Set( 0.0f, size.y - geomTypeReduce );
			}
			
			if( pIsThick ){
				const int count = cornerPointCount * 4;
				for( i=0; i<=count; i++ ){
					writer.WritePoint( inner[ i % count ] );
					writer.WritePoint( outer[ i % count ] );
				}
				
				pDrawModeLine = GL_TRIANGLE_STRIP;
				pDrawCountLine = ( count + 1 ) * 2;
				
			}else{
				pDrawModeLine = GL_LINE_LOOP;
				pDrawCountLine = cornerPointCount * 4;
				
				for( i=0; i<cornerPointCount * 4; i++ ){
					writer.WritePoint( outer[ i ] + pointCenter );
				}
			}
		}
		
		delete [] points;
		}break;
		
	case deSkinPropertyNodeShape::estEllipse:{
		const int pointCount = decMath::max( 4, ( int )( ( GetSize().x + GetSize().y ) * 2.0f / 5.0f ) );
		const decVector2 ellipseSize( size * 0.5f );
		const decVector2 center( ellipseSize );
		float startAngle = 0.0f;
		float endAngle = TWO_PI;
		
		// fill
		const decVector2 ellipseSizeInner( decVector2().Largest( ellipseSize - decVector2( thickness, thickness ) ) );
		decVector2 * const points = new decVector2[ pointCount * 3 + 2 ];
		decVector2 * const inner = points;
		decVector2 * const outer = points + pointCount;
		
		pCalcArc( inner, center, ellipseSizeInner + decVector2( 1.0f, 1.0f ), startAngle, endAngle, pointCount );
		
		for( i=0; i<pointCount; i++ ){
			writer.WritePoint( inner[ i ] );
		}
		
		pDrawModeFill = GL_TRIANGLE_FAN;
		pDrawCountFill = pointCount;
		
		// outline
		pDrawOffsetLine = pDrawCountFill;
		
		if( thickness > 0.5f ){
			if( pIsThick ){
				const float lenStart = ( inner[ 0 ] - center ).Length() + thickness;
				const float lenEnd = ( inner[ pointCount - 1 ] - center ).Length() + thickness;
				if( lenStart > 0.5f ){
					startAngle += atanf( thickness * 0.5f / lenStart );
				}
				if( lenEnd > 0.5f ){
					endAngle -= atanf( thickness * 0.5f / lenEnd );
				}
				
				pCalcArc( outer, center, ellipseSize, startAngle, endAngle, pointCount );
				
				for( i=0; i<=pointCount; i++ ){
					writer.WritePoint( inner[ i % pointCount ] );
					writer.WritePoint( outer[ i % pointCount ] );
				}
				pDrawCountLine = ( pointCount + 1 ) * 2;
				
				pDrawModeLine = GL_TRIANGLE_STRIP;
				
			}else{
				pCalcArc( outer, center, ellipseSize, startAngle, endAngle, pointCount );
				
				for( i=0; i<pointCount; i++ ){
					writer.WritePoint( outer[ i ] );
				}
				
				pDrawModeLine = GL_LINE_LOOP;
				pDrawCountLine = pointCount;
			}
		}
		
		delete [] points;
		}break;
	}
}

void deoglSkinStateCNShape::pCalcArc( decVector2 *outPoints, const decVector2 &center,
const decVector2 &size, float startAngle, float stopAngle, int stepCount ){
	const float stepAngle = ( stopAngle - startAngle ) / ( stepCount - 1 );
	int i;
	
	for( i=0; i<stepCount; i++ ){
		const float arcAngle = startAngle + stepAngle * i;
		outPoints[ i ].x = center.x + size.x * sinf( arcAngle );
		outPoints[ i ].y = center.y + size.y * cosf( arcAngle );
	}
}
