/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEANIMATIONKEYFRAMEVERTEXPOSITIONSETLIST_H_
#define _DEANIMATIONKEYFRAMEVERTEXPOSITIONSETLIST_H_

#include "../../dragengine_export.h"

class deAnimation;
class deAnimationKeyframeVertexPositionSet;


/**
 * \brief Animation Keyframe List.
 * \version 1.17
 */
class DE_DLL_EXPORT deAnimationKeyframeVertexPositionSetList{
private:
	deAnimationKeyframeVertexPositionSet **pKeyframes;
	int pKeyframeCount;
	int pKeyframeSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation keyframe list. */
	deAnimationKeyframeVertexPositionSetList();
	
	/** \brief Clean up animation keyframe list. */
	~deAnimationKeyframeVertexPositionSetList();
	/*@}*/
	
	
	
	/** \name Keyframe Management */
	/*@{*/
	/** \brief Count of keyframes. */
	inline int GetKeyframeCount() const{ return pKeyframeCount; }
	
	/** \brief Keyframe at the given index. */
	deAnimationKeyframeVertexPositionSet *GetKeyframe( int index ) const;
	
	/** \brief Adds a new keyframe */
	void AddKeyframe( deAnimationKeyframeVertexPositionSet *keyframe );
	
	/**
	 * \brief Index of the keyframe the biggest time right before or exactly at the given time.
	 * 
	 * This function is depracted and will be removed soon.
	 */
	int GetLeadingKeyframe( int startkf, float time ) const;
	/*@}*/
};

#endif
