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

#ifndef _DEANIMATORRULEANIMATIONDIFFERENCE_H_
#define _DEANIMATORRULEANIMATIONDIFFERENCE_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Animation Difference Rule Class.
 *
 * Retrieves two animation states from two different animation
 * frames and calculates the difference between the two states.
 * This difference is then blended over the existing state.
 * State 1 is considered the leading state whereas state 2 is
 * considered the reference state. The difference is taken from
 * the leading state minus the reference state. If both states
 * use the same move an additional flag can be set so you only
 * have to set the move of the leading state to set both moves.
 */
class deAnimatorRuleAnimationDifference : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleAnimationDifference> Ref;
	
	
	
private:
	decString pMove1Name;
	float pMove1Time;
	decString pMove2Name;
	float pMove2Time;
	bool pUseSameMove;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	deAnimatorControllerTarget pTargetLeadMoveTime;
	deAnimatorControllerTarget pTargetRefMoveTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleAnimationDifference();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleAnimationDifference();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the leading animation move. */
	inline const decString &GetLeadingMoveName() const{ return pMove1Name; }
	
	/** \brief Set leading animation move name. */
	void SetLeadingMoveName( const char *moveName );
	
	/** \brief Leading animation move time. */
	inline float GetLeadingMoveTime() const{ return pMove1Time; }
	
	/** \brief Set leading animation move time. */
	void SetLeadingMoveTime( float moveTime );
	
	/** \brief Name of the reference animation move. */
	inline const decString &GetReferenceMoveName() const{ return pMove2Name; }
	
	/** \brief Set reference animation move name. */
	void SetReferenceMoveName( const char *moveName );
	
	/** \brief Reference animation move time. */
	inline float GetReferenceMoveTime() const{ return pMove2Time; }
	
	/** \brief Set reference animation move time. */
	void SetReferenceMoveTime( float moveTime );
	
	/** \brief Reference move is the same as the leading move. */
	inline bool GetUseSameMove() const{ return pUseSameMove; }
	
	/** \brief Set if reference move is the same as the leading move. */
	void SetUseSameMove( bool useSameMove );
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Leading move time target. */
	inline deAnimatorControllerTarget &GetTargetLeadingMoveTime(){ return pTargetLeadMoveTime; }
	inline const deAnimatorControllerTarget &GetTargetLeadingMoveTime() const{ return pTargetLeadMoveTime; }
	
	/** \brief Reference move time target. */
	inline deAnimatorControllerTarget &GetTargetReferenceMoveTime(){ return pTargetRefMoveTime; }
	inline const deAnimatorControllerTarget &GetTargetReferenceMoveTime() const{ return pTargetRefMoveTime; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
