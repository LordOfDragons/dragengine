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
#include <stdlib.h>
#include <string.h>

#include "deoglTriangleSorter.h"

#include <dragengine/common/exceptions.h>


// Class deoglTriangleSorter
//////////////////////////////

// Management
///////////////

const decVector &deoglTriangleSorter::GetTriangleVertex1(int triangle) const{
	return pSortedTriangles.GetAt(triangle)->vertex1;
}

const decVector &deoglTriangleSorter::GetTriangleVertex2(int triangle) const{
	return pSortedTriangles.GetAt(triangle)->vertex2;
}

const decVector &deoglTriangleSorter::GetTriangleVertex3(int triangle) const{
	return pSortedTriangles.GetAt(triangle)->vertex3;
}

void deoglTriangleSorter::AddTriangle(const decVector &vertex1,
const decVector &vertex2, const decVector &vertex3){
	pTriangles.Add({vertex1, vertex2, vertex3});
}

void deoglTriangleSorter::RemoveAllTriangles(){
	pTriangles.RemoveAll();
	pSortedTriangles.RemoveAll();
}



// Sorting
////////////

void deoglTriangleSorter::SortLinear(const decVector &position, const decVector &view){
	// calculate distance to reference point using view direction as projection axis
	pTriangles.Visit([&](sTriangle &t){
		const decVector triangleCenter((t.vertex1 + t.vertex2 + t.vertex3) / 3.0f);
		const decVector triangleView(triangleCenter - position);
		t.distance = triangleView * view;
	});
	
	// init pointer list
	pSortedTriangles.RemoveAll();
	pSortedTriangles.EnlargeCapacity(pTriangles.GetCount());
	pTriangles.Visit([&](const sTriangle &t){
		pSortedTriangles.Add(&t);
	});
	
	// sort by distance
	pSortedTriangles.Sort([](const sTriangle *a, const sTriangle *b){
		return a->distance < b->distance;
	});
}

void deoglTriangleSorter::SortRadial(const decVector &position){
	// calculate distance to reference point. squared distance is enough so we do
	// not have to calculate the square root.
	pTriangles.Visit([&](sTriangle &t){
		const decVector triangleCenter((t.vertex1 + t.vertex2 + t.vertex3) / 3.0f);
		const decVector direction(triangleCenter - position);
		t.distance = direction * direction;
	});
	
	// init pointer list
	pSortedTriangles.RemoveAll();
	pSortedTriangles.EnlargeCapacity(pTriangles.GetCount());
	pTriangles.Visit([&](const sTriangle &t){
		pSortedTriangles.Add(&t);
	});
	
	// sort by distance
	pSortedTriangles.Sort([](const sTriangle *a, const sTriangle *b){
		return a->distance < b->distance;
	});
	
	// debug
	/*
	printf("[TRIANGLESORTER] (%i Triangles)\n", pSortedTriangles.GetCount());
	pSortedTriangles.Visit([&](const sTriangle *t){
		printf("%g, ", t->distance);
	});
	printf("\n\n");
	*/
}
