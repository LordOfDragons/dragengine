/* 
 * Drag[en]gine IGDE World Editor
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

#include "meHeightTerrainSector.h"
#include "meHeightTerrainNavSpaceFace.h"

#include <dragengine/common/exceptions.h>



// struct sort entry
struct sSortEntry{
	int navpoint;
	float angle;
};



// Class meHeightTerrainNavSpaceFace
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainNavSpaceFace::meHeightTerrainNavSpaceFace() :
pType( NULL ){
}

meHeightTerrainNavSpaceFace::~meHeightTerrainNavSpaceFace(){
}



// Management
///////////////

void meHeightTerrainNavSpaceFace::SetType( meHeightTerrainNavSpaceType *type ){
	pType = type;
}



void meHeightTerrainNavSpaceFace::OrderClockwise( int pointsPerRow ){
	const int count = pNavPoints.GetCount();
	if( count < 2 ){
		return;
	}
	
	// sorting is done by calculating first the center of the face. since the face is
	// supposed to be convex the center has to lie inside the face. then the navigation
	// points are ordered using their angle towards the center. the 0 angle is along
	// the grid y axis which is pointing down if looked down from above. atan2 angles
	// then rotate counter-clockwise. thus to get clockwise ordering the angles have
	// to be sorted descending.
	sSortEntry * const entries = new sSortEntry[ count ];
	decVector2 center;
	int i;
	
	for( i=0; i<count; i++ ){
		sSortEntry &entry = entries[ i ];
		entry.navpoint = pNavPoints.GetAt( i );
		center.x += ( float )( entry.navpoint % pointsPerRow );
		center.y += ( float )( entry.navpoint / pointsPerRow );
	}
	center /= ( float )count;
	
	for( i=0; i<count; i++ ){
		sSortEntry &entry = entries[ i ];
		entry.angle = atan2f(
			( float )( entry.navpoint % pointsPerRow ) - center.x,
			( float )( entry.navpoint / pointsPerRow ) - center.y );
	}
	
	for( i=1; i<count; i++ ){
		if( entries[ i - 1 ].angle >= entries[ i ].angle ){
			continue;
		}
		
		const sSortEntry exchange( entries[ i - 1 ] );
		entries[ i - 1 ] = entries[ i ];
		entries[ i ] = exchange;
		if( i > 1 ){
			i -= 2;
		}
	}
	
	for( i=0; i<count; i++ ){
		pNavPoints.SetAt( i, entries[ i ].navpoint );
	}
	
	delete [] entries;
}

bool meHeightTerrainNavSpaceFace::NavPointsMatch( const decIntList &navpoints ) const{
	if( navpoints.GetCount() != pNavPoints.GetCount() ){
		return false;
	}
	
	const int count = navpoints.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ! pNavPoints.Has( navpoints.GetAt( i ) ) ){
			return false;
		}
	}
	
	return true;
}

bool meHeightTerrainNavSpaceFace::HasNavPoint( int navpoint ) const{
	return pNavPoints.Has( navpoint );
}

bool meHeightTerrainNavSpaceFace::HasNavEdge( int navpoint1, int navpoint2 ) const{
	if( pNavPoints.GetCount() < 2 ){
		return false;
	}
	
	const int index = pNavPoints.IndexOf( navpoint1 );
	if( index == -1 ){
		return false;
	}
	
	const int count = pNavPoints.GetCount();
	return pNavPoints.GetAt( ( index + 1 ) % count ) == navpoint2
		|| pNavPoints.GetAt( ( index - 1 + count ) % count ) == navpoint2;
}

bool meHeightTerrainNavSpaceFace::HasAllNavPointsIn( const decIntList &navpoints ) const{
	if( pNavPoints.GetCount() > navpoints.GetCount() ){
		return false;
	}
	
	const int count = pNavPoints.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ! navpoints.Has( pNavPoints.GetAt( i ) ) ){
			return false;
		}
	}
	
	return true;
}
