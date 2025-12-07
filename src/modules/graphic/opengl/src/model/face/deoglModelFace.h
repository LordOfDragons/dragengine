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

#ifndef _DEOGLMODELFACE_H_
#define _DEOGLMODELFACE_H_

#include <dragengine/common/math/decMath.h>



/**
 * Model Face.
 */
class deoglModelFace{
private:
	int pVertices[3];
	decVector pFaceNormal;
	decVector pMinExtend;
	decVector pMaxExtend;
	decVector pCenter;
	int pTexture;
	int pIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model face. */
	deoglModelFace();
	/** Cleans up the model face. */
	~deoglModelFace();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the first vertex. */
	inline int GetVertex1() const{return pVertices[0];}
	/** Sets the first vertex. */
	void SetVertex1(int vertex);
	/** Retrieves the second vertex. */
	inline int GetVertex2() const{return pVertices[1];}
	/** Sets the second vertex. */
	void SetVertex2(int vertex);
	/** Retrieves the third vertex. */
	inline int GetVertex3() const{return pVertices[2];}
	/** Sets the third vertex. */
	void SetVertex3(int vertex);
	/** Retrieves the given vertex. */
	inline int GetVertexAt(int index) const{return pVertices[index];}
	/** Sets the given vertex. */
	void SetVertexAt(int index, int vertex);
	
	/** Retrieves the face normal. */
	inline const decVector &GetFaceNormal() const{return pFaceNormal;}
	/** Sets the face normal. */
	void SetFaceNormal(const decVector &normal);
	/** Retrieves the minimum extend. */
	inline const decVector &GetMinExtend() const{return pMinExtend;}
	/** Retrieves the maximum extend. */
	inline const decVector &GetMaxExtend() const{return pMaxExtend;}
	/** Sets the extends. */
	void SetExtends(const decVector &minExtend, const decVector &maxExtend);
	/** Retrieves the center. */
	inline const decVector &GetCenter() const{return pCenter;}
	/** Sets the center. */
	void SetCenter(const decVector &center);
	
	/** Retrieves the texture. */
	inline int GetTexture() const{return pTexture;}
	/** Sets the texture. */
	void SetTexture(int texture);
	/** Retrieves the index. */
	inline int GetIndex() const{return pIndex;}
	/** Sets the index. */
	void SetIndex(int index);
	/*@}*/
};

#endif
