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

#ifndef _DEOGLOCCLUSIONQUERY_H_
#define _DEOGLOCCLUSIONQUERY_H_

#include "../deoglBasics.h"

class deoglRenderThread;
class deoglShape;



/**
 * \brief Occlusion Query.
 *
 * Allows to query how many pixels/samples an operation touched.
 */
class deoglOcclusionQuery{
public:
	/** \brief Query type. */
	enum eQueryTypes{
		/** \brief Result is 1 if any samples passed otherwise 0. */
		eqtAny,
		
		/** \brief Result is the number of samples passed. */
		eqtCount
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	GLuint pQuery;
	eQueryTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create occlusion query. */
	deoglOcclusionQuery( deoglRenderThread &renderThread );
	
	/** \brief Clean up occlusion query. */
	~deoglOcclusionQuery();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Query is active. */
	bool IsActive() const;
	
	/**
	 * \brief Start query making it the active one.
	 * 
	 * If another query is active already it is first stopped before starting this query.
	 * 
	 * \throws deeInvalidParam \em type is \em eQueryTypes::eqtCount and Platform is Android.
	 *                         OpenGL ES does not support counting queries only any queries.
	 */
	void BeginQuery( eQueryTypes type );
	
	/**
	 * \brief End query.
	 * 
	 * Has no effect if this query is not the active one.
	 */
	void EndQuery();
	
	/** \brief Result of the query is present. */
	bool HasResult() const;
	
	/** \brief Result of the query blocking until the result is available. */
	int GetResult() const;
	
	/** \brief Result of the query blocking until the result is available. */
	bool GetResultAny() const;
	/*@}*/
};

#endif
