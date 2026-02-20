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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "deoglCubeHelper.h"

#include <dragengine/common/exceptions.h>



// Class deoglCubeHelper
//////////////////////////

// Management
///////////////

void deoglCubeHelper::CalcFaceVisibility(const decDVector &boxMin,
const decDVector &boxMax, bool *result){
	if(!result){
		DETHROW(deeInvalidParam);
	}
	
	// the tests done are quick tests which are conservative but not exact. this allows the
	// tests to run quick while only resulting in a few false positives. for this test the
	// 8 corners of the box are calculated and tested against the 2 45 degree diagonal planes
	// for each axis as well as the axes themselves. the points are then classified for
	// visibility according to the result of the plane tests
	const decDVector corners[8] = {
		decDVector(boxMin.x, boxMin.y, boxMin.z),
		decDVector(boxMax.x, boxMin.y, boxMin.z),
		decDVector(boxMin.x, boxMax.y, boxMin.z),
		decDVector(boxMax.x, boxMax.y, boxMin.z),
		decDVector(boxMin.x, boxMin.y, boxMax.z),
		decDVector(boxMax.x, boxMin.y, boxMax.z),
		decDVector(boxMin.x, boxMax.y, boxMax.z),
		decDVector(boxMax.x, boxMax.y, boxMax.z)};
	
	// looks funny but is simple. side counts for each plane how many points pass the test.
	// this works without branching by using the standard defined conversion of bool to int
	// which is false to 0 and true to 1.
	// - side == 8: all corners are on the positive plane side
	// - side == 0: all corners are on the negative plane side
	// - otherwise: corners fall on both sides of plane
	// 
	// the first 6 side values are the 6 diagonal planes. the next 3 are the axes planes.
	// the axes planes are used too to reduce the false positive with little extra work
	int i, side[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for(i=0; i<8; i++){
		side[0] += corners[i].x >= -corners[i].y;
		side[1] += corners[i].x >=  corners[i].y;
		side[2] += corners[i].y >= -corners[i].z;
		side[3] += corners[i].y >=  corners[i].z;
		side[4] += corners[i].z >= -corners[i].x;
		side[5] += corners[i].z >=  corners[i].x;
		side[6] += corners[i].x >= 0.0;
		side[7] += corners[i].y >= 0.0;
		side[8] += corners[i].z >= 0.0;
	}
	
	// this helps to make the test equations easier to understand and verify. anyPass is true
	// if at least one point passes the side test. anyFail is true if at least one point fails
	// the side test. all other situations are not required
	bool anyPass[9], anyFail[9];
	for(i=0; i<9; i++){
		anyPass[i] = side[i] > 0;
		anyFail[i] = side[i] < 8;
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
	result[0] = anyPass[6] && anyPass[0] && anyPass[1] && anyPass[4] && anyFail[5];
	result[1] = anyFail[6] && anyFail[0] && anyFail[1] && anyFail[4] && anyPass[5];
	result[2] = anyPass[7] && anyPass[0] && anyFail[1] && anyPass[2] && anyPass[3];
	result[3] = anyFail[7] && anyFail[0] && anyPass[1] && anyFail[2] && anyFail[3];
	result[4] = anyPass[8] && anyPass[2] && anyFail[3] && anyPass[4] && anyPass[5];
	result[5] = anyFail[8] && anyFail[2] && anyPass[3] && anyFail[4] && anyFail[5];
}

int deoglCubeHelper::CalcFaceVisibility(const decDVector &boxMin, const decDVector &boxMax){
	int result = 0;
	
	// the tests done are quick tests which are conservative but not exact. this allows the
	// tests to run quick while only resulting in a few false positives. for this test the
	// 8 corners of the box are calculated and tested against the 2 45 degree diagonal planes
	// for each axis as well as the axes themselves. the points are then classified for
	// visibility according to the result of the plane tests
	const decDVector corners[8] = {
		decDVector(boxMin.x, boxMin.y, boxMin.z),
		decDVector(boxMax.x, boxMin.y, boxMin.z),
		decDVector(boxMin.x, boxMax.y, boxMin.z),
		decDVector(boxMax.x, boxMax.y, boxMin.z),
		decDVector(boxMin.x, boxMin.y, boxMax.z),
		decDVector(boxMax.x, boxMin.y, boxMax.z),
		decDVector(boxMin.x, boxMax.y, boxMax.z),
		decDVector(boxMax.x, boxMax.y, boxMax.z)};
	
	// looks funny but is simple. side counts for each plane how many points pass the test.
	// this works without branching by using the standard defined conversion of bool to int
	// which is false to 0 and true to 1.
	// - side == 8: all corners are on the positive plane side
	// - side == 0: all corners are on the negative plane side
	// - otherwise: corners fall on both sides of plane
	// 
	// the first 6 side values are the 6 diagonal planes. the next 3 are the axes planes.
	// the axes planes are used too to reduce the false positive with little extra work
	int i, side[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for(i=0; i<8; i++){
		side[0] += corners[i].x >= -corners[i].y;
		side[1] += corners[i].x >=  corners[i].y;
		side[2] += corners[i].y >= -corners[i].z;
		side[3] += corners[i].y >=  corners[i].z;
		side[4] += corners[i].z >= -corners[i].x;
		side[5] += corners[i].z >=  corners[i].x;
		side[6] += corners[i].x >= 0.0;
		side[7] += corners[i].y >= 0.0;
		side[8] += corners[i].z >= 0.0;
	}
	
	// this helps to make the test equations easier to understand and verify. anyPass is true
	// if at least one point passes the side test. anyFail is true if at least one point fails
	// the side test. all other situations are not required
	bool anyPass[9], anyFail[9];
	for(i=0; i<9; i++){
		anyPass[i] = side[i] > 0;
		anyFail[i] = side[i] < 8;
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
	// order: X+, X-, Y-, Y+, Z+, Z- . yes, Y is flipped to line it up with opengl
	if(anyPass[6] && anyPass[0] && anyPass[1] && anyPass[4] && anyFail[5]){
		result |= 0x1;
	}
	if(anyFail[6] && anyFail[0] && anyFail[1] && anyFail[4] && anyPass[5]){
		result |= 0x2;
	}
	if(anyPass[7] && anyPass[0] && anyFail[1] && anyPass[2] && anyPass[3]){
		result |= 0x8;
	}
	if(anyFail[7] && anyFail[0] && anyPass[1] && anyFail[2] && anyFail[3]){
		result |= 0x4;
	}
	if(anyPass[8] && anyPass[2] && anyFail[3] && anyPass[4] && anyPass[5]){
		result |= 0x10;
	}
	if(anyFail[8] && anyFail[2] && anyPass[3] && anyFail[4] && anyFail[5]){
		result |= 0x20;
	}
	
	return result;
}
