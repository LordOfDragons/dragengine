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

// include only once
#ifndef _DECBSPTREEFACE_H_
#define _DECBSPTREEFACE_H_

// includes
#include "decMath.h"



/**
 * @brief Generic BSP Tree Face.
 *
 * Stores a bsp tree face. The face has to be convex and can contain
 * more than 3 vertices. An optional user data parameter allows to
 * link external information to a face.
 */
class decBSPTreeFace{
private:
	decVector *pVertices;
	short pVertexCount;
	short pVertexSize;
	void *pUserData;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new bsp tree face. */
	decBSPTreeFace();
	/** Cleans up new bsp tree face. */
	virtual ~decBSPTreeFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{return pVertexCount;}
	/** Retrieves the vertex at the given position. */
	const decVector &GetVertexAt(int index) const;
	/** Adds a vertex. */
	void AddVertex(const decVector &vertex);
	/** Removes all vertices. */
	void RemoveAllVertices();
	/** Retrieves the user data. */
	inline void *GetUserData() const{return pUserData;}
	/** Sets the user data. */
	void SetUserData(void *userData);
	/*@}*/
};

// end of include only once
#endif
