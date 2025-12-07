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

#ifndef _DEANIMATIONKEYFRAMEVERTEXPOSITIONSET_H_
#define _DEANIMATIONKEYFRAMEVERTEXPOSITIONSET_H_

#include "../../common/math/decMath.h"


/**
 * \brief Animation vertex position set keyframe.
 * \version 1.17
 */
class DE_DLL_EXPORT deAnimationKeyframeVertexPositionSet{
private:
	float pTime;
	float pWeight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation vertex position set keyframe. */
	deAnimationKeyframeVertexPositionSet();
	
	/** \brief Clean up animation vertex position set keyframe. */
	~deAnimationKeyframeVertexPositionSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Time in seconds. */
	inline float GetTime() const{ return pTime; }
	
	/** \brief Set time in seconds. */
	void SetTime(float time);
	
	/** \brief Weight. */
	inline float GetWeight() const{ return pWeight; }
	
	/** \brief Set weight. */
	void SetWeight(float weight);
	/*@}*/
};

#endif
