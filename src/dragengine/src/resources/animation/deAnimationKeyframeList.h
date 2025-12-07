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

#ifndef _DEANIMATIONKEYFRAMELIST_H_
#define _DEANIMATIONKEYFRAMELIST_H_

#include "../../dragengine_export.h"

class deAnimation;
class deAnimationKeyframe;


/**
 * \brief Animation Keyframe List.
 *
 */
class DE_DLL_EXPORT deAnimationKeyframeList{
private:
	deAnimationKeyframe **pKeyframes;
	int pKeyframeCount, pKeyframeSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation keyframe list. */
	deAnimationKeyframeList();
	
	/** \brief Clean up animation keyframe list. */
	~deAnimationKeyframeList();
	/*@}*/
	
	
	
	/** \name Keyframe Management */
	/*@{*/
	/** \brief Count of keyframes. */
	inline int GetKeyframeCount() const{return pKeyframeCount;}
	
	/** \brief Keyframe at the given index. */
	deAnimationKeyframe *GetKeyframe(int index) const;
	
	/** \brief Adds a new keyframe */
	void AddKeyframe(deAnimationKeyframe *keyframe);
	
	/**
	 * \brief Index of the keyframe the biggest time right before or exactly at the given time.
	 * 
	 * This function is depracted and will be removed soon.
	 */
	int GetLeadingKeyframe(int startkf, float time) const;
	/*@}*/
};

#endif
