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

#ifndef _DEARVPSSTATE_H_
#define _DEARVPSSTATE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearComponentVPSState;
class dearAnimationVPSState;
class deComponent;


/** Vertex position set state. */
class dearVPSState{
private:
	const char *pName;
	int pIndex;
	int pModelIndex;
	float pWeight;
	bool pProtect;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create state. */
	dearVPSState();
	
	/** Clean up state. */
	~dearVPSState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const char *GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Index of state. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index of state. */
	void SetIndex( int index );
	
	/** Model index. */
	inline int GetModelIndex() const{ return pModelIndex; }
	
	/** Set model index. */
	void SetModelIndex( int index );
	
	/** Weight. */
	inline float GetWeight() const{ return pWeight; }
	
	/** Set weight. */
	void SetWeight( float weight );
	
	/** State is protected. */
	inline bool GetProtected() const{ return pProtect; }
	
	/** Set if state is protected. */
	void SetProtected( bool prot );
	
	/** State is dirty. */
	inline bool GetDirty() const{ return pDirty; }
	
	/** Set if state is dirty. */
	void SetDirty( bool dirty );
	
	/** Copy another state to this state. */
	void SetFrom( const dearVPSState &state );
	
	/** Copy component state to this state. */
	void SetFrom( const deComponent &component );
	
	/** Copy component state to this state. */
	void SetFrom( const dearComponentVPSState &state );
	
	/** Update state by blending it with an empty state. */
	void BlendWithDefault( deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const dearVPSState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const dearAnimationVPSState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const deComponent &component, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const dearComponentVPSState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( float weight, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight );
	
	/** Full copy. */
	dearVPSState &operator=( const dearVPSState &state );
	/*@}*/
};

#endif
