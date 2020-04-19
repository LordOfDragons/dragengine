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

#include "igdeWAngleRange.h"

#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/common/exceptions.h>



// Class igdeWDDSAngleRange
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWAngleRange::igdeWAngleRange(){
	pColorDiskLine.Set( 1.0f, 0.0f, 0.0f, 1.0f );
	pColorDiskArea.Set( 1.0f, 0.0f, 0.0f, 0.2f );
	pLower = 0.0f;
	pUpper = 90.0f;
	pRadius = 0.5f;
	pVisible = true;
	
	pDDSDisk.SetEdgeColor( pColorDiskLine );
	pDDSDisk.SetFillColor( pColorDiskArea );
	
	pRebuildShape();
}

igdeWAngleRange::~igdeWAngleRange(){
	SetParentDebugDrawer( NULL );
}



// Management
///////////////

void igdeWAngleRange::SetParentDebugDrawer( deDebugDrawer *debugDrawer ){
	pDDSDisk.SetParentDebugDrawer( debugDrawer );
}



void igdeWAngleRange::SetPosition( const decVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		pDDSDisk.SetPosition( position );
	}
}

void igdeWAngleRange::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		pDDSDisk.SetOrientation( orientation );
	}
}



void igdeWAngleRange::SetDiskLineColor( const decColor &color ){
	if( ! color.IsEqualTo( pColorDiskLine ) ){
		pColorDiskLine = color;
		pDDSDisk.SetEdgeColor( pColorDiskLine );
	}
}

void igdeWAngleRange::SetDiskAreaColor( const decColor &color ){
	if( ! color.IsEqualTo( pColorDiskArea ) ){
		pColorDiskArea = color;
		pDDSDisk.SetFillColor( pColorDiskArea );
	}
}



void igdeWAngleRange::SetRange( float lower, float upper ){
	if( upper < lower ){
		upper = lower;
	}
	if( upper - lower > 360.0f ){
		upper = lower + 360.0f;
	}
	
	if( fabsf( lower - pLower ) > 1e-5f || fabsf( upper - pUpper ) > 1e-5f ){
		pLower = lower;
		pUpper = upper;
		pRebuildShape();
	}
}

void igdeWAngleRange::SetRadius( float radius ){
	if( radius < 0.001f ){
		radius = 0.001f;
	}
	
	if( fabsf( radius - pRadius ) > 1e-5f ){
		pRadius = radius;
		pRebuildShape();
	}
}



void igdeWAngleRange::SetVisible( bool visible ){
	if( visible != pVisible ){
		pVisible = visible;
		pDDSDisk.SetVisible( visible );
	}
}



// Private Functions
//////////////////////

void igdeWAngleRange::pRebuildShape(){
	deDebugDrawerShapeFace *ddFace = NULL;
	float meterPerPoint = 0.1f;
	float angle, angleDiff;
	float circumfence;
	int p, pointCount;
	bool useDisk;
	
	// determine if a disk or circle has to be used.
	angleDiff = pUpper - pLower;
	
	useDisk = angleDiff < 359.9999f;
	
	// determine the number of points to use for the disk
	circumfence = angleDiff * DEG2RAD;
	
	pointCount = ( int )ceilf( circumfence / meterPerPoint );
	
	// create the disk
	pDDSDisk.RemoveAllFaces();
	
	try{
		ddFace = new deDebugDrawerShapeFace;
		if( ! ddFace ) DETHROW( deeOutOfMemory );
		
		ddFace->SetNormal( decVector( 0.0f, 0.0f, 1.0f ) );
		
		if( useDisk ){
			ddFace->AddVertex( decVector() );
		}
		
		for( p=0; p<=pointCount; p++ ){
			angle = ( pLower + angleDiff * ( ( float )p / ( float )pointCount ) ) * DEG2RAD;
			
			ddFace->AddVertex( decVector( sinf( angle ) * pRadius, cosf( angle ) * pRadius, 0.0f ) );
		}
		
		if( useDisk ){
			ddFace->AddVertex( decVector() );
		}
		
		pDDSDisk.AddFace( ddFace );
		ddFace = NULL;
		
	}catch( const deException & ){
		if( ddFace ){
			delete ddFace;
		}
		throw;
	}
}
