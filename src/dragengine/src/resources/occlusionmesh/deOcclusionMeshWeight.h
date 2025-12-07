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

#ifndef _DEOCCLUSIONMESHWEIGHT_H_
#define _DEOCCLUSIONMESHWEIGHT_H_

#include "../../dragengine_export.h"


/**
 * \brief Occlusion Mesh Weight.
 * A single weight in a weight set. The bone index has to be 0 or larger, the weight in the range from 0 to 1.
 */
class DE_DLL_EXPORT deOcclusionMeshWeight{
private:
	int pBone;
	float pWeight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new weight. */
	deOcclusionMeshWeight();
	
	/** \brief Clean up weight. */
	~deOcclusionMeshWeight();
	/*@}*/
	
	
	
	/** \name Weights Management */
	/*@{*/
	/** \brief Index of the bone. */
	inline int GetBone() const{return pBone;}
	
	/** \brief Set index of the bone. */
	void SetBone(int bone);
	
	/** \brief Weight in the range from 0 to 1. */
	inline float GetWeight() const{return pWeight;}
	
	/** \brief Set weight in the range from 0 to 1. */
	void SetWeight(float weight);
	/*@}*/
};

#endif
