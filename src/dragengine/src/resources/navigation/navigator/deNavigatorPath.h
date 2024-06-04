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

#ifndef _DENAVIGATORPATH_H_
#define _DENAVIGATORPATH_H_

#include "../../../common/math/decMath.h"


/**
 * \brief Navigator path.
 * 
 * Result of finding a path in a navigator. Stores path points
 * towards and including the goal. The path can be used for
 * collision checking or branching path.
 */
class DE_DLL_EXPORT deNavigatorPath{
private:
	decDVector *pPoints;
	int pCount;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigator path. */
	deNavigatorPath();
	
	/** \brief Create copy of navigator path. */
	deNavigatorPath( const deNavigatorPath &path );
	
	/** \brief Clean up navigator path. */
	~deNavigatorPath();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of points. */
	inline int GetCount() const{ return pCount; }
	
	/**
	 * \brief Path point at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetPointCount().
	 */
	const decDVector &GetAt( int index ) const;
	
	/**
	 * \brief Set path point at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetPointCount().
	 */
	void SetAt( int index, const decDVector &position );
	
	/** \brief Add path point to end of path. */
	void Add( const decDVector &point );
	
	/** \brief Add path points to end of path. */
	void AddPath( const deNavigatorPath &path );
	
	/**
	 * \brief Remove path point at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetPointCount().
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all path points. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set points. */
	deNavigatorPath &operator=( const deNavigatorPath &path );
	/*@}*/
};

#endif
