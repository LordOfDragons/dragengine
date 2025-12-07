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

#ifndef _DEANIMATIONKEYFRAME_H_
#define _DEANIMATIONKEYFRAME_H_

#include "../../common/math/decMath.h"


/**
 * \brief Animation bone keyframe.
 */
class DE_DLL_EXPORT deAnimationKeyframe{
private:
	float pTime;
	decVector pPosition;
	decVector pRotation;
	decVector pScale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation bone keyframe. */
	deAnimationKeyframe();
	
	/** \brief Clean up animation bone keyframe. */
	~deAnimationKeyframe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Time in seconds. */
	inline float GetTime() const{return pTime;}
	
	/** \brief Set time in seconds. */
	void SetTime(float time);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{return pPosition;}
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{return pRotation;}
	
	/** \brief Set rotation. */
	void SetRotation(const decVector &rotation);
	
	/** \brief Scaling. */
	inline const decVector &GetScale() const{return pScale;}
	
	/** \brief Set scaling. */
	void SetScale(const decVector &scale);
	/*@}*/
};

#endif
