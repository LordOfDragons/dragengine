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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeWConnection.h"
#include "../../igdeShapeBuilder.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeWConnection
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeWConnection::igdeWConnection() :
pColor( 0.0f, 0.0f, 0.0f ),
pArrowOnAnchor( false ),
pArrowOnTarget( true ),
pArrowSize( 0.05f ),
pArrowLength( 2.0f ),
pArrowRadius( 0.15f ),
pVisible( true )
{
	try{
		pDDSConnection.SetEdgeColor( pColor );
		pDDSConnection.SetFillColor( decColor( pColor.r, pColor.g, pColor.b, pColor.a * 0.2f ) );
		
		pDDSArrowAnchor.SetEdgeColor( pColor );
		pDDSArrowAnchor.SetFillColor( decColor( pColor.r, pColor.g, pColor.b, pColor.a * 0.2f ) );
		
		pDDSArrowTarget.SetEdgeColor( pColor );
		pDDSArrowTarget.SetFillColor( decColor( pColor.r, pColor.g, pColor.b, pColor.a * 0.2f ) );
		
		pRebuildArrow();
		pUpdateGeometries();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeWConnection::~igdeWConnection(){
	pCleanUp();
}



// Management
///////////////

void igdeWConnection::SetParentDebugDrawer( deDebugDrawer *debugDrawer ){
	pDDSConnection.SetParentDebugDrawer( debugDrawer );
	pDDSArrowAnchor.SetParentDebugDrawer( debugDrawer );
	pDDSArrowTarget.SetParentDebugDrawer( debugDrawer );
}



void igdeWConnection::SetAnchorPosition( const decVector &position ){
	if( position.IsEqualTo( pAnchorPosition ) ){
		return;
	}
	
	pAnchorPosition = position;
	pUpdateGeometries();
}

void igdeWConnection::SetTargetPosition( const decVector &position ){
	if( position.IsEqualTo( pTargetPosition ) ){
		return;
	}
	
	pTargetPosition = position;
	pUpdateGeometries();
}

void igdeWConnection::SetPositions( const decVector &anchorPosition, const decVector &targetPosition ){
	if( anchorPosition.IsEqualTo( pAnchorPosition ) && targetPosition.IsEqualTo( pTargetPosition ) ){
		return;
	}
	
	pAnchorPosition = anchorPosition;
	pTargetPosition = targetPosition;
	pUpdateGeometries();
}



void igdeWConnection::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	
	pDDSConnection.SetEdgeColor( color );
	pDDSConnection.SetFillColor( decColor( color.r, color.g, color.b, color.a * 0.2f ) );
	
	pDDSArrowAnchor.SetEdgeColor( color );
	pDDSArrowAnchor.SetFillColor( decColor( color.r, color.g, color.b, color.a * 0.2f ) );
	
	pDDSArrowTarget.SetEdgeColor( color );
	pDDSArrowTarget.SetFillColor( decColor( color.r, color.g, color.b, color.a * 0.2f ) );
}

void igdeWConnection::SetArrowOnAnchor( bool arrowOnAnchor ){
	if( arrowOnAnchor == pArrowOnAnchor ){
		return;
	}
	
	pArrowOnAnchor = arrowOnAnchor;
	pUpdateGeometries();
}

void igdeWConnection::SetArrowOnTarget( bool arrowOnTarget ){
	if( arrowOnTarget == pArrowOnTarget ){
		return;
	}
	
	pArrowOnTarget = arrowOnTarget;
	pUpdateGeometries();
}

void igdeWConnection::SetArrowSize( float size ){
	size = decMath::max( size, 0.001f );
	if( fabsf( size - pArrowSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pArrowSize = size;
	pRebuildArrow(); // because arrow radius is implicitely changed and this requires a rebuild
	pUpdateGeometries();
}

void igdeWConnection::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	pUpdateGeometries();
}



// Private Functions
//////////////////////

void igdeWConnection::pCleanUp(){
	SetParentDebugDrawer( NULL );
}

void igdeWConnection::pRebuildArrow(){
	igdeShapeBuilder builder;
	
	builder.CreateArrow( pDDSConnection, decVector( 0.0f, 0.0f, -0.5f ),
		decVector( 0.0f, 0.0f, 0.5f ), 0.5f, 1e-5f, 0.5f );
	
	builder.CreateArrowHead( pDDSArrowTarget, decVector( 0.0f, 0.0f, -1.0f ), decVector(), 0.5f );
	builder.CreateArrowHead( pDDSArrowAnchor, decVector( 0.0f, 0.0f, 1.0f ), decVector(), 0.5f );
}

void igdeWConnection::pUpdateGeometries(){
	const decVector direction = pTargetPosition - pAnchorPosition;
	float len = direction.Length();
	
	// calculate matrix
	decMatrix matrix;
	
	if( len < 1e-5f ){
		matrix.SetTranslation( pAnchorPosition );
		
	}else{
		const decVector dirNor = direction / len;
		
		if( fabsf( dirNor.y ) < 0.999f ){
			matrix.SetVU( dirNor, decVector( 0.0f, 1.0f, 0.0f ) );
			
		}else{
			matrix.SetVU( dirNor, decVector( 0.0f, 0.0f, 1.0f ) );
		}
	}
	
	// update geometry parameters
	const decQuaternion orientation = matrix.ToQuaternion();
	const float arrowLength = pArrowSize * pArrowLength * 0.5f;
	const float arrowSize = pArrowSize * pArrowRadius;
	
	pDDSArrowTarget.SetPosition( matrix.Transform( 0.0f, 0.0f, len ) );
	pDDSArrowTarget.SetOrientation( orientation );
	pDDSArrowTarget.SetScale( decVector( pArrowSize, pArrowSize, arrowLength ) );
	pDDSArrowTarget.SetVisible( pVisible && pArrowOnTarget );
	
	pDDSArrowAnchor.SetPosition( matrix.GetPosition() );
	pDDSArrowAnchor.SetOrientation( orientation );
	pDDSArrowAnchor.SetScale( decVector( pArrowSize, pArrowSize, arrowLength ) );
	pDDSArrowAnchor.SetVisible( pVisible && pArrowOnAnchor );
	
	float center = len * 0.5f;
	if( pArrowOnTarget ){
		len -= arrowLength;
		center -= arrowLength * 0.5f;
	}
	if( pArrowOnAnchor ){
		len -= arrowLength;
		center += arrowLength * 0.5f;
	}
	if( len < 1e-5f ){
		len = 1e-5f;
	}
	
	pDDSConnection.SetPosition( matrix.Transform( 0.0f, 0.0f, center ) );
	pDDSConnection.SetOrientation( orientation );
	pDDSConnection.SetScale( decVector( arrowSize, arrowSize, len ) );
	pDDSConnection.SetVisible( pVisible );
}
