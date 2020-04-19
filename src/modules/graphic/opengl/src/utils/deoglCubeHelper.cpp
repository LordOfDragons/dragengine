/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>
#include <stdlib.h>

#include "deoglCubeHelper.h"

#include <dragengine/common/exceptions.h>



// Class deoglCubeHelper
//////////////////////////

// Management
///////////////

void deoglCubeHelper::CalcFaceVisibility( const decDVector &boxMin,
const decDVector &boxMax, bool *result ){
	if( ! result ){
		DETHROW( deeInvalidParam );
	}
	
	// the tests done are quick tests which are conservative but not exact. this allows the
	// tests to run quick while only resulting in a few false positives. for this test the
	// 8 corners of the box are calculated and tested against the 2 45 degree diagonal planes
	// for each axis as well as the axes themselves. the points are then classified for
	// visibility according to the result of the plane tests
	const decDVector corners[ 8 ] = {
		decDVector( boxMin.x, boxMin.y, boxMin.z ),
		decDVector( boxMax.x, boxMin.y, boxMin.z ),
		decDVector( boxMin.x, boxMax.y, boxMin.z ),
		decDVector( boxMax.x, boxMax.y, boxMin.z ),
		decDVector( boxMin.x, boxMin.y, boxMax.z ),
		decDVector( boxMax.x, boxMin.y, boxMax.z ),
		decDVector( boxMin.x, boxMax.y, boxMax.z ),
		decDVector( boxMax.x, boxMax.y, boxMax.z ) };
	
	// looks funny but is simple. side counts for each plane how many points pass the test.
	// this works without branching by using the standard defined conversion of bool to int
	// which is false to 0 and true to 1.
	// - side == 8: all corners are on the positive plane side
	// - side == 0: all corners are on the negative plane side
	// - otherwise: corners fall on both sides of plane
	// 
	// the first 6 side values are the 6 diagonal planes. the next 3 are the axes planes.
	// the axes planes are used too to reduce the false positive with little extra work
	int i, side[ 9 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for( i=0; i<8; i++ ){
		side[ 0 ] += corners[ i ].x >= -corners[ i ].y;
		side[ 1 ] += corners[ i ].x >=  corners[ i ].y;
		side[ 2 ] += corners[ i ].y >= -corners[ i ].z;
		side[ 3 ] += corners[ i ].y >=  corners[ i ].z;
		side[ 4 ] += corners[ i ].z >= -corners[ i ].x;
		side[ 5 ] += corners[ i ].z >=  corners[ i ].x;
		side[ 6 ] += corners[ i ].x >= 0.0;
		side[ 7 ] += corners[ i ].y >= 0.0;
		side[ 8 ] += corners[ i ].z >= 0.0;
	}
	
	// this helps to make the test equations easier to understand and verify. anyPass is true
	// if at least one point passes the side test. anyFail is true if at least one point fails
	// the side test. all other situations are not required
	bool anyPass[ 9 ], anyFail[ 9 ];
	for( i=0; i<9; i++ ){
		anyPass[ i ] = side[ i ] > 0;
		anyFail[ i ] = side[ i ] < 8;
	}
	
	// now we can calculate the final result which requires checking 4 planes. the far plane
	// is not used. it is expected from the caller such objects have been culled already
	// 
	// here a chart to find the right test equations.
	// 
	// side | coord-frame | plane orientation | pass if on side
	//   0  |    ^Y >X    |         \         |      right
	//   1  |    ^Y >X    |         /         |      right
	//   2  |    ^Y >Z    |         \         |       up
	//   3  |    ^Y >Z    |         /         |       up
	//   4  |    ^Z >X    |         \         |       up
	//   5  |    ^Z >X    |         /         |       up
	//   6  |     ^X      |         -         |       up
	//   7  |     ^Y      |         -         |       up
	//   8  |     ^Z      |         -         |       up
	// 
	// order: X+, X-, Y+, Y-, Z+, Z-
	result[ 0 ] = anyPass[ 6 ] && anyPass[ 0 ] && anyPass[ 1 ] && anyPass[ 4 ] && anyFail[ 5 ];
	result[ 1 ] = anyFail[ 6 ] && anyFail[ 0 ] && anyFail[ 1 ] && anyFail[ 4 ] && anyPass[ 5 ];
	result[ 2 ] = anyPass[ 7 ] && anyPass[ 0 ] && anyFail[ 1 ] && anyPass[ 2 ] && anyPass[ 3 ];
	result[ 3 ] = anyFail[ 7 ] && anyFail[ 0 ] && anyPass[ 1 ] && anyFail[ 2 ] && anyFail[ 3 ];
	result[ 4 ] = anyPass[ 8 ] && anyPass[ 2 ] && anyFail[ 3 ] && anyPass[ 4 ] && anyPass[ 5 ];
	result[ 5 ] = anyFail[ 8 ] && anyFail[ 2 ] && anyPass[ 3 ] && anyFail[ 4 ] && anyFail[ 5 ];
}
