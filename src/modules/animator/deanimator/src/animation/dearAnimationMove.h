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

#ifndef _DEARANIMATIONMOVE_H_
#define _DEARANIMATIONMOVE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deAnimationMove;
class dearAnimationKeyframeList;
class dearAnimationKeyframeVPSList;



/** Animation peer move. */
class dearAnimationMove : public deObject{
private:
	decString pName;
	float pPlaytime;
	
	dearAnimationKeyframeList **pKeyframeLists;
	int pKeyframeListCount;
	
	dearAnimationKeyframeVPSList **pKeyframeVPSLists;
	int pKeyframeVPSListCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animation move. */
	dearAnimationMove( const deAnimationMove &move );
	
protected:
	/** Clean up the animation move. */
	virtual ~dearAnimationMove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Play time in seconds. */
	inline float GetPlaytime() const{ return pPlaytime; }
	
	/** Count of keyframe lists. */
	inline int GetKeyframeListCount() const{ return pKeyframeListCount; }
	
	/** Keyframe list at index. */
	dearAnimationKeyframeList *GetKeyframeListAt( int index ) const;
	
	/** Count of keyframe lists. */
	inline int GetKeyframeVPSListCount() const{ return pKeyframeVPSListCount; }
	
	/** Keyframe list at index. */
	dearAnimationKeyframeVPSList *GetKeyframeVPSListAt( int index ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateKeyframeLists( const deAnimationMove &move );
	void pCreateKeyframeVPSLists( const deAnimationMove &move );
};

#endif
