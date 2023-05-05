/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARANIMATIONKEYFRAMEVPSLIST_H_
#define _DEARANIMATIONKEYFRAMEVPSLIST_H_

#include <dragengine/common/math/decMath.h>

class deAnimationKeyframeVertexPositionSetList;
class dearAnimationKeyframeVPS;



/**
 * Animation move vertex position set keyframe list.
 */
class dearAnimationKeyframeVPSList{
private:
	dearAnimationKeyframeVPS *pKeyframes;
	int pKeyframeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create keyframe list. */
	dearAnimationKeyframeVPSList( const deAnimationKeyframeVertexPositionSetList &list );
	
	/** Clean up keyframe list. */
	~dearAnimationKeyframeVPSList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of keyframes. */
	inline int GetCount() const{ return pKeyframeCount; }
	
	/** Keyframe at index. */
	dearAnimationKeyframeVPS &GetAt( int index ) const;
	
	/** Keyframe with range containing time in seconds or nullptr if absent. */
	dearAnimationKeyframeVPS *GetWithTime( float time ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateKeyframes( const deAnimationKeyframeVertexPositionSetList &list );
};

#endif
