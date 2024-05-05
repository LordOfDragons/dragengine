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
