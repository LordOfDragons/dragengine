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

#ifndef _DEANIMATIONMOVE_H_
#define _DEANIMATIONMOVE_H_

#include "../../common/string/decString.h"
#include "../../common/math/decMath.h"

class deAnimation;
class deAnimationKeyframeList;


/**
 * \brief Animation Move.
 *
 */
class deAnimationMove{
private:
	decString pName;
	float pPlaytime;
	deAnimationKeyframeList **pLists;
	int pListCount, pListSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation move without name. */
	deAnimationMove();
	
	/** \brief Clean up animation move. */
	~deAnimationMove();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the move. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of the move */
	void SetName( const char *name );
	
	/** \brief Playtime of the move which is the largest time of all keyframes. */
	inline float GetPlaytime() const{ return pPlaytime; }
	
	/** \brief Set playtime for the move which is the largest time of all keyframes. */
	void SetPlaytime( float playtime );
	
	/** \brief Count of keyframe lists. */
	inline int GetKeyframeListCount() const{ return pListCount; }
	
	/** \brief Keyframe at the given index. */
	deAnimationKeyframeList *GetKeyframeList( int index ) const;
	
	/** \brief Adds a keyframe list */
	void AddKeyframeList( deAnimationKeyframeList *list );
	/*@}*/
};

#endif
