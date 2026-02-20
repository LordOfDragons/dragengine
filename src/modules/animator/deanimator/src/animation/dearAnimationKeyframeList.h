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

#ifndef _DEARANIMATIONKEYFRAMELIST_H_
#define _DEARANIMATIONKEYFRAMELIST_H_

#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/common/collection/decTList.h>

class dearAnimationKeyframe;



/**
 * Animation move keyframe list.
 */
class dearAnimationKeyframeList{
private:
	decTList<dearAnimationKeyframe> pKeyframes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new animation move keyframe list. */
	explicit dearAnimationKeyframeList(const deAnimationKeyframe::List &list);
	/** Cleans up the animation move keyframe list. */
	~dearAnimationKeyframeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Keyframes. */
	inline decTList<dearAnimationKeyframe> &GetKeyframes(){ return pKeyframes; }
	inline const decTList<dearAnimationKeyframe> &GetKeyframes() const{ return pKeyframes; }
	
	/**
	 * Retrieves the keyframe with the range containing the provided
	 *        time in seconds or NULL if there are no keyframes.
	 */
	const dearAnimationKeyframe *GetWithTime(float time) const;
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateKeyframes(const deAnimationKeyframe::List &list);
};

#endif
