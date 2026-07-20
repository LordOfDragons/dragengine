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

#ifndef _DEOGLSHAPEHULL_H_
#define _DEOGLSHAPEHULL_H_

#include "deoglShape.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglConvexHull3D;


/**
 * Hull Shape.
 */
class deoglShapeHull : public deoglShape{
private:
	decTList<decVector> pPoints;
	decTList<int> pIndices;
	int pPointCountFaces, pPointCountLines;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	explicit deoglShapeHull(deoglRenderThread &renderThread);
	
	/** Creates a new shape with hull data. */
	deoglShapeHull(deoglRenderThread &renderThread, const decTList<decVector> &points,
		const decTList<int> &indices);
	
	/** Cleans up the shape. */
	~deoglShapeHull() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set hull data. */
	void SetHullData(const decTList<decVector> &points, const decTList<int> &indices);
	
	/** Add lines data. */
	void AddVBOLines(sVBOData *data) override;
	
	/** Add faces data. */
	void AddVBOFaces(sVBOData *data) override;
	/*@}*/
};

#endif