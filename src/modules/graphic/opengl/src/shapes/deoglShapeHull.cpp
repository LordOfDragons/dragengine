/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglShapeHull.h"

#include <dragengine/common/exceptions.h>


// Class deoglShapeHull
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeHull::deoglShapeHull(deoglRenderThread &renderThread) :
deoglShape(renderThread)
{
	SetPointOffsetLines(0);
	SetPointCountLines(0);
	
	SetPointOffsetFaces(0);
	SetPointCountFaces(0);
}

deoglShapeHull::deoglShapeHull(deoglRenderThread &renderThread,
	const decTList<decVector> &points, const decTList<int> &indices) :
deoglShape(renderThread)
{
	SetHullData(points, indices);
}

deoglShapeHull::~deoglShapeHull(){
}


// Management
///////////////

void deoglShapeHull::SetHullData(const decTList<decVector> &points, const decTList<int> &indices){
	pPoints = points;
	pIndices = indices;
	
	pPointCountFaces = pIndices.GetCount();
	pPointCountLines = pIndices.GetCount() * 2;
	
	SetPointOffsetLines(0);
	SetPointCountLines(pPointCountLines);
	
	SetPointOffsetFaces(pPointCountLines);
	SetPointCountFaces(pPointCountFaces);
}

void deoglShapeHull::AddVBOLines(sVBOData *data){
	pIndices.VisitIndexed(0, pIndices.GetCount(), 3, [&](int i, int index){
		const decVector &point1 = pPoints[pIndices[i]];
		const decVector &point2 = pPoints[pIndices[i + 1]];
		const decVector &point3 = pPoints[pIndices[i + 2]];
		
		// edge 1-2
		(data++)->SetSelFalse(point1.x, point1.y, point1.z);
		(data++)->SetSelFalse(point2.x, point2.y, point2.z);
		
		// edge 2-3
		(data++)->SetSelFalse(point2.x, point2.y, point2.z);
		(data++)->SetSelFalse(point3.x, point3.y, point3.z);
		
		// edge 3-1
		(data++)->SetSelFalse(point3.x, point3.y, point3.z);
		(data++)->SetSelFalse(point1.x, point1.y, point1.z);
	});
}

void deoglShapeHull::AddVBOFaces(sVBOData *data){
	pIndices.VisitIndexed(0, pIndices.GetCount(), 3, [&](int i, int index){
		const decVector &point1 = pPoints[pIndices[i]];
		const decVector &point2 = pPoints[pIndices[i + 1]];
		const decVector &point3 = pPoints[pIndices[i + 2]];
		
		(data++)->SetSelFalse(point1.x, point1.y, point1.z);
		(data++)->SetSelFalse(point2.x, point2.y, point2.z);
		(data++)->SetSelFalse(point3.x, point3.y, point3.z);
	});
}