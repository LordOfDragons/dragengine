/* 
 * Drag[en]gine Game Engine
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
	inline int GetKeyframeCount() const{ return pKeyframeCount; }
	
	/** \brief Keyframe at the given index. */
	deAnimationKeyframe *GetKeyframe( int index ) const;
	
	/** \brief Adds a new keyframe */
	void AddKeyframe( deAnimationKeyframe *keyframe );
	
	/**
	 * \brief Index of the keyframe the biggest time right before or exactly at the given time.
	 * 
	 * This function is depracted and will be removed soon.
	 */
	int GetLeadingKeyframe( int startkf, float time ) const;
	/*@}*/
};

#endif
