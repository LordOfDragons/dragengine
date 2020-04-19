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

#ifndef _AERULEANIMATIONDIFFERENCE_H_
#define _AERULEANIMATIONDIFFERENCE_H_

#include "aeRule.h"



/**
 * \brief Animator rule animation difference.
 */
class aeRuleAnimationDifference : public aeRule{
private:
	decString pMove1Name;
	float pMove1Time;
	decString pMove2Name;
	float pMove2Time;
	bool pUseSameMove;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	aeControllerTarget pTargetLeadMoveTime;
	aeControllerTarget pTargetRefMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new animator difference rule. */
	aeRuleAnimationDifference();
	/** \brief Create a copy of an animator difference rule. */
	aeRuleAnimationDifference( const aeRuleAnimationDifference &copy );
	/** \brief Cleans up the animator difference rule. */
	virtual ~aeRuleAnimationDifference();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the name of the leading animation move. */
	inline const decString &GetLeadingMoveName() const{ return pMove1Name; }
	/** \brief Set the leading animation move name. */
	void SetLeadingMoveName( const char *moveName );
	/** \brief Retrieve the leading animation move time. */
	inline float GetLeadingMoveTime() const{ return pMove1Time; }
	/** \brief Set the leading animation move time. */
	void SetLeadingMoveTime( float moveTime );
	/** \brief Retrieve the name of the reference animation move. */
	inline const decString &GetReferenceMoveName() const{ return pMove2Name; }
	/** \brief Set the reference animation move name. */
	void SetReferenceMoveName( const char *moveName );
	/** \brief Retrieve the reference animation move time. */
	inline float GetReferenceMoveTime() const{ return pMove2Time; }
	/** \brief Set the reference animation move time. */
	void SetReferenceMoveTime( float moveTime );
	/** \brief Determine if the reference move is the same as the leading move. */
	inline bool GetUseSameMove() const{ return pUseSameMove; }
	/** \brief Set if the reference move is the same as the leading move. */
	void SetUseSameMove( bool useSameMove );
	
	/** \brief Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	/** \brief Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	/** \brief Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Retrieve the leading move time target. */
	inline aeControllerTarget &GetTargetLeadingMoveTime(){ return pTargetLeadMoveTime; }
	/** \brief Retrieve the reference move time target. */
	inline aeControllerTarget &GetTargetReferenceMoveTime(){ return pTargetRefMoveTime; }
	
	/** \brief Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** \brief Update targets. */
	virtual void UpdateTargets();
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animator difference rule to this animator difference rule. */
	virtual aeRuleAnimationDifference &operator=( const aeRuleAnimationDifference &copy );
	/*@}*/
};

#endif
