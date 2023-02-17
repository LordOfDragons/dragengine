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

#ifndef _DEOGLTIMERQUERY_H_
#define _DEOGLTIMERQUERY_H_

#include "../../deoglBasics.h"

class deoglRenderThread;



/**
 * Light weight wrapper around a timer query.
 * 
 * This implementation does not care which query is active nor if they are
 * correctly stopped before a new one starts.
 */
class deoglTimerQuery{
private:
	deoglRenderThread &pRenderThread;
	GLuint pQuery;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create timer query. */
	deoglTimerQuery( deoglRenderThread &renderThread );
	
	/** Clean up timer query. */
	~deoglTimerQuery();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Begin query. */
	void BeginQuery();
	
	/** End active query. */
	void EndQuery();
	
	/** Result of the query is present. */
	bool HasResult() const;
	
	/** Result of the query as nano-seconds. */
	unsigned int GetResult() const;
	/*@}*/
};

#endif
