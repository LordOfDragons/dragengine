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

#include "deoglShapeSphere.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define RING_COUNT		9
#define SEGMENT_COUNT	20



// Class deoglShapeSphere
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeSphere::deoglShapeSphere(deoglRenderThread &renderThread) : deoglShape(renderThread){
	SetPointOffsetLines(0);
	SetPointCountLines((RING_COUNT * SEGMENT_COUNT + SEGMENT_COUNT * (RING_COUNT + 1)) * 2);
	
	SetPointOffsetFaces(GetPointCountLines());
	SetPointCountFaces(RING_COUNT * SEGMENT_COUNT * 3 * 2);
}

deoglShapeSphere::~deoglShapeSphere(){
}



// Management
///////////////

void deoglShapeSphere::CalcMatrix(decMatrix &matrix, const decVector &position, float radius){
	matrix = decMatrix::CreateScale(radius, radius, radius) * decMatrix::CreateTranslation(position);
}

void deoglShapeSphere::AddVBOLines(sVBOData *data){
	const float stepAngleSegment = PI * 2.0f / (float)(SEGMENT_COUNT);
	const float stepAngleRing = PI / (float)(RING_COUNT + 1);
	float radius, angle, height, x, z;
	int i, j, base;
	
	base = 0;
	for(i=0; i<RING_COUNT; i++){
		angle = stepAngleRing * (float)(i + 1); // first ring is actually at i=1
		radius = sinf(angle);
		height = cosf(angle);
		
		for(j=0; j<SEGMENT_COUNT; j++){
			angle = stepAngleSegment * (float)j;
			data[base++].SetSelFalse(sinf(angle) * radius, height, cosf(angle) * radius);
			
			angle = stepAngleSegment * (float)(j + 1);
			data[base++].SetSelFalse(sinf(angle) * radius, height, cosf(angle) * radius);
		}
	}
	
	for(i=0; i<SEGMENT_COUNT; i++){
		angle = stepAngleSegment * (float)i;
		x = sinf(angle);
		z = cosf(angle);
		
		for(j=0; j<RING_COUNT+1; j++){
			angle = stepAngleRing * (float)j;
			radius = sinf(angle);
			data[base++].SetSelFalse(x * radius, cosf(angle), z * radius);
			
			angle = stepAngleRing * (float)(j + 1);
			radius = sinf(angle);
			data[base++].SetSelFalse(x * radius, cosf(angle), z * radius);
		}
	}
}

void deoglShapeSphere::AddVBOFaces(sVBOData *data){
	const float stepAngleSegment = PI * 2.0f / (float)(SEGMENT_COUNT);
	const float stepAngleRing = PI / (float)(RING_COUNT + 1);
	float radius1, radius2, angle1, height1, height2, angle2;
	int i, j, base;
	
	base = 0;
	for(i=0; i<RING_COUNT+1; i++){
		angle1 = stepAngleRing * (float)i;
		radius1 = sinf(angle1);
		height1 = cosf(angle1);
		
		angle2 = stepAngleRing * (float)(i + 1);
		radius2 = sinf(angle2);
		height2 = cosf(angle2);
		
		if(i == 0){
			for(j=0; j<SEGMENT_COUNT; j++){
				angle1 = stepAngleSegment * (float)j;
				angle2 = stepAngleSegment * (float)(j + 1);
				
				data[base++].SetSelFalse(0.0f, 1.0f, 0.0f);
				data[base++].SetSelFalse(sinf(angle2) * radius2, height2, cosf(angle2) * radius2);
				data[base++].SetSelFalse(sinf(angle1) * radius2, height2, cosf(angle1) * radius2);
			}
			
		}else if(i == RING_COUNT){
			for(j=0; j<SEGMENT_COUNT; j++){
				angle1 = stepAngleSegment * (float)j;
				angle2 = stepAngleSegment * (float)(j + 1);
				
				data[base++].SetSelFalse(0.0f, -1.0f, 0.0f);
				data[base++].SetSelFalse(sinf(angle1) * radius1, height1, cosf(angle1) * radius1);
				data[base++].SetSelFalse(sinf(angle2) * radius1, height1, cosf(angle2) * radius1);
			}
			
		}else{
			for(j=0; j<SEGMENT_COUNT; j++){
				angle1 = stepAngleSegment * (float)j;
				angle2 = stepAngleSegment * (float)(j + 1);
				
				data[base++].SetSelFalse(sinf(angle1) * radius2, height2, cosf(angle1) * radius2);
				data[base++].SetSelFalse(sinf(angle1) * radius1, height1, cosf(angle1) * radius1);
				data[base++].SetSelFalse(sinf(angle2) * radius1, height1, cosf(angle2) * radius1);
				
				data[base++].SetSelFalse(sinf(angle1) * radius2, height2, cosf(angle1) * radius2);
				data[base++].SetSelFalse(sinf(angle2) * radius1, height1, cosf(angle2) * radius1);
				data[base++].SetSelFalse(sinf(angle2) * radius2, height2, cosf(angle2) * radius2);
			}
		}
	}
}
