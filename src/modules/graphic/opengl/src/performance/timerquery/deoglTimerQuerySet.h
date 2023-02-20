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

#ifndef _DEOGLTIMERQUERYSET_H_
#define _DEOGLTIMERQUERYSET_H_

#include "../../deoglBasics.h"

class deoglRenderThread;



/**
 * Light weight wrapper around a set of timer queries.
 * 
 * Works similar to deoglTimerQuery but stores a set of timer queries for easier
 * access. This implementation does not care which query is active nor if they
 * are correctly stopped before a new one starts.
 */
class deoglTimerQuerySet{
private:
	deoglRenderThread &pRenderThread;
	GLuint *pQueries;
	int pQueryCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create timer query set. */
	deoglTimerQuerySet( deoglRenderThread &renderThrad );
	
	/** Clean up timer query set. */
	~deoglTimerQuerySet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Number of queries in the set. */
	inline int GetQueryCount() const{ return pQueryCount; }
	
	/** Set number of queries in the set. */
	void SetQueryCount( int count );
	
	/** Begin query at index. */
	void BeginQuery( int index );
	
	/** End active query. */
	void EndQuery();
	
	/** Result of the query at index is present. */
	bool HasResult( int index ) const;
	
	/** Result of query at index as nano-seconds. */
	unsigned int GetResult( int index ) const;
	/*@}*/
};

#endif
