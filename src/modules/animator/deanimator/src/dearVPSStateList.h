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

#ifndef _DEARVPSSTATELIST_H_
#define _DEARVPSSTATELIST_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearComponent;
class dearVPSState;
class deComponent;
class deAnimator;


/** VPS State List. */
class dearVPSStateList{
private:
	dearVPSState *pStates;
	int pStateCount;
	int pStateSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create state list. */
	dearVPSStateList();
	
	/** Clean up state list. */
	~dearVPSStateList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of states. */
	inline int GetStateCount() const{ return pStateCount; }
	
	/** Set count of states. */
	void SetStateCount( int count );
	
	/** State at index. */
	dearVPSState &GetStateAt( int index ) const;
	
	/** Index of named state or -1. */
	int IndexOfStateNamed( const char *name ) const;
	
	/** Create copy of state list. */
	dearVPSStateList *CreateCopy() const;
	
	/** Copy another state list to this list. */
	void SetFrom( const dearVPSStateList &stateList );
	
	/** Mark dirty. */
	void MarkDirty();
	
	/** Update mappings. */
	void UpdateMappings( const deAnimator &animator, const deComponent *component );
	
	/** Apply states to engine component. */
	void ApplyToComponent( deComponent &component ) const;
	
	/** Apply states to engine component. */
	void ApplyToComponent( deComponent &component,
		deAnimatorRule::eBlendModes blendMode, float blendFactor ) const;
	
	/** Apply states to animator module component. */
	void ApplyToComponent( dearComponent &component ) const;
	
	/** Apply states to animator module component. */
	void ApplyToComponent( dearComponent &component,
		deAnimatorRule::eBlendModes blendMode, float blendFactor ) const;
	/*@}*/
};

#endif
