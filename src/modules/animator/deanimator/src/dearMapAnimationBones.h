/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEMAPANIMATIONBONES_H_
#define _DEMAPANIMATIONBONES_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class deAnimation;
class dearRule;
class dearBoneStateList;


/**
 * Map from rule bone index to animation bone indices.
 */
class dearMapAnimationBones{
private:
	int *pIndices;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create map. */
	dearMapAnimationBones();
	
	/** Clean up map. */
	~dearMapAnimationBones();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of entries. */
	inline int GetCount() const{ return pCount; }
	
	/** Clear map. */
	void Clear();
	
	/** Init map. */
	void Init( const dearRule &rule );
	
	/** Animation bone index for rule bone index or -1. */
	int GetAt( int ruleBoneIndex ) const;
	/*@}*/
};

#endif
