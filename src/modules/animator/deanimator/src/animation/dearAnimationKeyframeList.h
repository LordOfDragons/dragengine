/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEARANIMATIONKEYFRAMELIST_H_
#define _DEARANIMATIONKEYFRAMELIST_H_

#include <dragengine/common/math/decMath.h>

class deAnimationKeyframeList;
class dearAnimationKeyframe;



/**
 * Animation move keyframe list.
 */
class dearAnimationKeyframeList{
private:
	dearAnimationKeyframe *pKeyframes;
	int pKeyframeCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new animation move keyframe list. */
	dearAnimationKeyframeList( const deAnimationKeyframeList &list );
	/** Cleans up the animation move keyframe list. */
	~dearAnimationKeyframeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of keyframes. */
	inline int GetCount() const{ return pKeyframeCount; }
	/** Retrieves a keyframe by index. */
	dearAnimationKeyframe &GetAt( int index ) const;
	/**
	 * Retrieves the keyframe with the range containing the provided
	 *        time in seconds or NULL if there are no keyframes.
	 */
	dearAnimationKeyframe *GetWithTime( float time ) const;
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateKeyframes( const deAnimationKeyframeList &list );
};

#endif
