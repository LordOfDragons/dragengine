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

#ifndef _AERULEANIMATIONSELECT_H_
#define _AERULEANIMATIONSELECT_H_

#include "aeRule.h"

#include <dragengine/common/string/decStringList.h>



/**
 * Animator rule animation select.
 */
class aeRuleAnimationSelect : public aeRule{
public:
	typedef deTObjectReference<aeRuleAnimationSelect> Ref;
	
private:
	decStringList pMoves;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeControllerTarget pTargetMoveTime;
	aeControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create animator select rule. */
	aeRuleAnimationSelect();
	
	/** Create copy of animator select rule. */
	aeRuleAnimationSelect( const aeRuleAnimationSelect &copy );
	
	/** Clean up animator select rule. */
	virtual ~aeRuleAnimationSelect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Moves. */
	inline const decStringList &GetMoves() const{ return pMoves; }
	
	/** Set moves. */
	void SetMoves( const decStringList &moves );
	
	
	
	/** Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	
	
	/** Move time target. */
	inline aeControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const aeControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
	/** Select target. */
	inline aeControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const aeControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** Create engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	
	
	/** Update targets. */
	virtual void UpdateTargets();
	
	/** Number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	
	/** Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	
	/** Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	
	
	/** Create copy of rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy another animator difference rule to this animator difference rule. */
	virtual aeRuleAnimationSelect &operator=( const aeRuleAnimationSelect &copy );
	/*@}*/
};

#endif
