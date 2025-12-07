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

#ifndef _DEMODELLODVERTEXPOSITIONSETPOSITION_H_
#define _DEMODELLODVERTEXPOSITIONSETPOSITION_H_

#include "../../common/math/decMath.h"


/**
 * \brief Model vertex position set position.
 * 
 * Position in a vertex position set.
 */
class DE_DLL_EXPORT deModelLodVertexPositionSetPosition{
private:
	int pVertex;
	decVector pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model vertex position set position. */
	deModelLodVertexPositionSetPosition();
	
	/** \brief Clean up model vertex position set position. */
	~deModelLodVertexPositionSetPosition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of vertex. */
	inline int GetVertex() const{ return pVertex; }
	
	/** \brief Set index of vertex. */
	void SetVertex(int vertex);
	
	/** \brief Vertex position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set vertex position. */
	void SetPosition(const decVector &position);
	/*@}*/
};

#endif
