/* 
 * Drag[en]gine IGDE
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeWCoordSysArrows.h"
#include "../../igdeShapeBuilder.h"

#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/common/exceptions.h>



// Class igdeWCoordSysArrows
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWCoordSysArrows::igdeWCoordSysArrows(){
	pScale.Set( 1.0f, 1.0f, 1.0f );
	pColorAxisX.Set( 1.0f, 0.0f, 0.0f );
	pColorAxisY.Set( 0.0f, 1.0f, 0.0f );
	pColorAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pArrowLength = 0.1f;
	pArrowSize = 0.005f;
	pVisible = true;
	
	pDDSAxisX.SetEdgeColor( pColorAxisX );
	pDDSAxisX.SetEdgeColor( pColorAxisX );
	pDDSAxisY.SetEdgeColor( pColorAxisY );
	pDDSAxisY.SetEdgeColor( pColorAxisY );
	pDDSAxisZ.SetEdgeColor( pColorAxisZ );
	pDDSAxisZ.SetEdgeColor( pColorAxisZ );
	
	pRebuildArrows();
}

igdeWCoordSysArrows::~igdeWCoordSysArrows(){
	SetParentDebugDrawer( NULL );
}



// Management
///////////////

void igdeWCoordSysArrows::SetParentDebugDrawer( deDebugDrawer *debugDrawer ){
	pDDSAxisX.SetParentDebugDrawer( debugDrawer );
	pDDSAxisY.SetParentDebugDrawer( debugDrawer );
	pDDSAxisZ.SetParentDebugDrawer( debugDrawer );
}



void igdeWCoordSysArrows::SetPosition( const decVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		pDDSAxisX.SetPosition( position );
		pDDSAxisY.SetPosition( position );
		pDDSAxisZ.SetPosition( position );
	}
}

void igdeWCoordSysArrows::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		
		pDDSAxisX.SetOrientation( orientation );
		pDDSAxisY.SetOrientation( orientation );
		pDDSAxisZ.SetOrientation( orientation );
	}
}

void igdeWCoordSysArrows::SetScale( const decVector &scale ){
	if( ! scale.IsEqualTo( pScale ) ){
		pScale = scale;
		
		pDDSAxisX.SetScale( scale );
		pDDSAxisY.SetScale( scale );
		pDDSAxisZ.SetScale( scale );
	}
}



void igdeWCoordSysArrows::SetAxisXColor( const decColor &color ){
	if( ! color.IsEqualTo( pColorAxisX ) ){
		pColorAxisX = color;
		
		pDDSAxisX.SetEdgeColor( pColorAxisX );
		pDDSAxisX.SetEdgeColor( pColorAxisX );
	}
}

void igdeWCoordSysArrows::SetAxisYColor( const decColor &color ){
	if( ! color.IsEqualTo( pColorAxisY ) ){
		pColorAxisY = color;
		
		pDDSAxisY.SetEdgeColor( pColorAxisY );
		pDDSAxisY.SetEdgeColor( pColorAxisY );
	}
}

void igdeWCoordSysArrows::SetAxisZColor( const decColor &color ){
	if( ! color.IsEqualTo( pColorAxisZ ) ){
		pColorAxisZ = color;
		
		pDDSAxisZ.SetEdgeColor( pColorAxisZ );
		pDDSAxisZ.SetEdgeColor( pColorAxisZ );
	}
}

void igdeWCoordSysArrows::SetArrowLength( float length ){
	if( length < 0.001f ){
		length = 0.001f;
	}
	
	if( fabsf( length - pArrowLength ) > 1e-5f ){
		pArrowLength = length;
		pRebuildArrows();
	}
}

void igdeWCoordSysArrows::SetArrowSize( float size ){
	if( size < 0.001f ){
		size = 0.001f;
	}
	
	if( fabsf( size - pArrowSize ) > 1e-5f ){
		pArrowSize = size;
		pRebuildArrows();
	}
}

void igdeWCoordSysArrows::SetVisible( bool visible ){
	if( visible != pVisible ){
		pVisible = visible;
		
		pDDSAxisX.SetVisible( visible );
		pDDSAxisY.SetVisible( visible );
		pDDSAxisZ.SetVisible( visible );
	}
}



// Private Functions
//////////////////////

void igdeWCoordSysArrows::pRebuildArrows(){
	const float headLength = pArrowSize * 3.0f;
	const float tailSize = pArrowSize * 0.1f;
	igdeShapeBuilder builder;
	
	builder.CreateArrow( pDDSAxisX, decVector(), decVector( pArrowLength, 0.0f, 0.0f ), pArrowSize, headLength, tailSize );
	builder.CreateArrow( pDDSAxisY, decVector(), decVector( 0.0f, pArrowLength, 0.0f ), pArrowSize, headLength, tailSize );
	builder.CreateArrow( pDDSAxisZ, decVector(), decVector( 0.0f, 0.0f, pArrowLength ), pArrowSize, headLength, tailSize );
}
