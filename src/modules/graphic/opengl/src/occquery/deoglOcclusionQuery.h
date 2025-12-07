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

#ifndef _DEOGLOCCLUSIONQUERY_H_
#define _DEOGLOCCLUSIONQUERY_H_

#include "../deoglBasics.h"

class deoglRenderThread;
class deoglShape;



/**
 * Occlusion Query.
 *
 * Allows to query how many pixels/samples an operation touched.
 */
class deoglOcclusionQuery{
public:
	/** Query type. */
	enum eQueryTypes{
		/** Result is 1 if any samples passed otherwise 0. */
		eqtAny,
		
		/** Result is the number of samples passed. */
		eqtCount
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	GLuint pQuery;
	eQueryTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create occlusion query. */
	deoglOcclusionQuery(deoglRenderThread &renderThread);
	
	/** Clean up occlusion query. */
	~deoglOcclusionQuery();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Query is active. */
	bool IsActive() const;
	
	/**
	 * Start query making it the active one.
	 * 
	 * If another query is active already it is first stopped before starting this query.
	 * 
	 * \throws deeInvalidParam \em type is \em eQueryTypes::eqtCount and Platform is Android.
	 *                         OpenGL ES does not support counting queries only any queries.
	 */
	void BeginQuery(eQueryTypes type);
	
	/**
	 * End query.
	 * 
	 * Has no effect if this query is not the active one.
	 */
	void EndQuery();
	
	/** Result of the query is present. */
	bool HasResult() const;
	
	/** Result of the query blocking until the result is available. */
	int GetResult() const;
	
	/** Result of the query blocking until the result is available. */
	bool GetResultAny() const;
	/*@}*/
};

#endif
