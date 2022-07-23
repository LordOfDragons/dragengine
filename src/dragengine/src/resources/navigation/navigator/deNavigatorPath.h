/* 
 * Drag[en]gine Game Engine
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
