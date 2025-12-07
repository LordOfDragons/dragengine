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

#ifndef _DEDAIPATHFINDERPOINTLIST_H_
#define _DEDAIPATHFINDERPOINTLIST_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Path Finder Point List.
 */
class dedaiPathFinderPointList{
private:
	decVector *pPoints;
	int pPointCount;
	int pPointSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path finder point list. */
	dedaiPathFinderPointList();
	/** Cleans up the path finder point list. */
	~dedaiPathFinderPointList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of points. */
	inline int GetCount() const{ return pPointCount; }
	/** Determines if the list is empty. */
	inline bool IsEmpty() const{ return pPointCount == 0; }
	/** Retrieves the top point. */
	const decVector &GetFirst() const;
	/** Retrieves the given point from the start of the list. */
	const decVector &GetAt(int index) const;
	/** Add a point to the end of the list. */
	void Add(const decVector &point);
	/** Removes the first point. */
	void RemoveFirst();
	/** Remove the given number of points from the start of the list. */
	void RemoveFirst(int count);
	/** Removes the given point from the start of the list. */
	void RemoveFrom(int index);
	/** Removes all points. */
	void RemoveAll();
	
	/** Transform point list by a matrix. */
	void Transform(const decMatrix &matrix);
	/*@}*/
};

#endif
