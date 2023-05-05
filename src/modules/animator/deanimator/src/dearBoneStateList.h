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

// include only once
#ifndef _DEARBONESTATELIST_H_
#define _DEARBONESTATELIST_H_

// includes
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearComponent;
// predefinitions
class dearBoneState;
class deComponent;
class deAnimation;
class deRig;
class deAnimator;



/**
 * Bone State List.
 *
 * List of bone states.
 */
class dearBoneStateList{
private:
	dearBoneState **pStates;
	int pStateCount;
	int pStateSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	dearBoneStateList();
	/** Cleans up the list. */
	~dearBoneStateList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of states. */
	inline int GetStateCount() const{ return pStateCount; }
	/** Sets the number of states. */
	void SetStateCount( int count );
	/** Retrieves the given state. */
	dearBoneState *GetStateAt( int index ) const;
	/** Retrieves the index of the state with the given rig bone name. */
	int IndexOfStateNamed( const char *name ) const;
	
	/** Creates a copy of this list. */
	dearBoneStateList *CreateCopy() const;
	
	/** Copy another state list to this list. */
	void SetFrom( const dearBoneStateList &stateList );
	
	/** Updates the states. */
	void UpdateStates();
	/** Mark dirty. */
	void MarkDirty();
	
	/** Updates the mappings. */
	void UpdateMappings( const deAnimator &animator );
	
	/** Apply states to an engine component. */
	void ApplyToComponent( deComponent *component ) const;
	
	/** Apply states to an engine component. */
	void ApplyToComponent( deComponent *component, deAnimatorRule::eBlendModes blendMode, float blendFactor ) const;
	
	/** Apply states to an animator module component. */
	void ApplyToComponent( dearComponent &component ) const;
	
	/** Apply states to an animator module component. */
	void ApplyToComponent( dearComponent &component, deAnimatorRule::eBlendModes blendMode, float blendFactor ) const;
	/*@}*/
};

// end of include only once
#endif
