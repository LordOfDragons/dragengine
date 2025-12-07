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

#ifndef _DEOCCLUSIONMESHVERTEX_H_
#define _DEOCCLUSIONMESHVERTEX_H_

#include "../../common/math/decMath.h"


/**
 * \brief Occlusion Mesh Vertex.
 */
class DE_DLL_EXPORT deOcclusionMeshVertex{
private:
	decVector pPosition;
	int pWeightSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model vertex. */
	deOcclusionMeshVertex();
	
	/** \brief Clean up model vertex. */
	~deOcclusionMeshVertex();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{return pPosition;}
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Index of the weight set or -1 if not used. */
	inline int GetWeightSet() const{return pWeightSet;}
	
	/** \brief Set index of the weight set or -1 if not used. */
	void SetWeightSet(int index);
	/*@}*/
};

#endif
