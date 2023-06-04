/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AERULEANIMATION_H_
#define _AERULEANIMATION_H_

#include "aeRule.h"



/**
 * Animator rule animation.
 */
class aeRuleAnimation : public aeRule{
public:
	typedef deTObjectReference<aeRuleAnimation> Ref;
	
private:
	decString pMoveName;
	float pMoveTime;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeControllerTarget pTargetMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animator rule. */
	aeRuleAnimation();
	/** Create a copy of a animator rule. */
	aeRuleAnimation( const aeRuleAnimation &copy );
	/** Clean up the animator rule. */
	virtual ~aeRuleAnimation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the animation move. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Set the animation move name. */
	void SetMoveName( const char *moveName );
	/** Retrieve the animation move time. */
	inline float GetMoveTime() const{ return pMoveTime; }
	/** Set the animation move time. */
	void SetMoveTime( float moveTime );
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** Determine if vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** Retrieve the move time target. */
	inline aeControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const aeControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
	/** Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another animation rule to this rule. */
	virtual aeRuleAnimation &operator=( const aeRuleAnimation &copy );
	/*@}*/
};

#endif
