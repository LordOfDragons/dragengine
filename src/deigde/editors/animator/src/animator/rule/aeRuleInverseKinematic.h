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

#ifndef _AERULEINVERSEKINEMATIC_H_
#define _AERULEINVERSEKINEMATIC_H_

#include "aeRule.h"



/**
 * \brief Animator rule inverse kinematic.
 */
class aeRuleInverseKinematic : public aeRule{
public:
	typedef deTObjectReference<aeRuleInverseKinematic> Ref;
	
private:
	decVector pGoalPosition;
	decVector pGoalOrientation;
	decVector pLocalPosition;
	decVector pLocalOrientation;
	bool pAdjustOrientation;
	
	bool pUseSolverBone;
	decString pSolverBone;
	
	float pReachRange;
	decString pReachBone;
	decVector pReachCenter;
	
	aeControllerTarget pTargetGoalPosition;
	aeControllerTarget pTargetGoalOrientation;
	aeControllerTarget pTargetLocalPosition;
	aeControllerTarget pTargetLocalOrientation;
	aeControllerTarget pTargetReachRange;
	aeControllerTarget pTargetReachCenter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create inverse kinematic rule. */
	aeRuleInverseKinematic();
	
	/** \brief Create copy of an inverse kinematic rule. */
	aeRuleInverseKinematic( const aeRuleInverseKinematic &copy );
	
	/** \brief Clean up the animator rule. */
	virtual ~aeRuleInverseKinematic();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Goal position. */
	inline const decVector &GetGoalPosition() const{ return pGoalPosition; }
	
	/** \brief Set goal position. */
	void SetGoalPosition( const decVector &position );
	
	/** \brief Goal orientation. */
	inline const decVector &GetGoalOrientation() const{ return pGoalOrientation; }
	
	/** \brief Set goal orientation. */
	void SetGoalOrientation( const decVector &orientation );
	
	/** \brief Local position. */
	inline const decVector &GetLocalPosition() const{ return pLocalPosition; }
	
	/** \brief Set local position. */
	void SetLocalPosition( const decVector &position );
	
	/** \brief Local orientation. */
	inline const decVector &GetLocalOrientation() const{ return pLocalOrientation; }
	
	/** \brief Set local orientation. */
	void SetLocalOrientation( const decVector &orientation );
	
	/** \brief Adjust orientation. */
	inline bool GetAdjustOrientation() const{ return pAdjustOrientation; }
	
	/** \brief Set if the orientation is adjusted. */
	void SetAdjustOrientation( bool adjustOrientation );
	
	/** \brief Use solver bone to obtain target position and orientation. */
	inline bool GetUseSolverBone() const{ return pUseSolverBone; }
	
	/** \brief Set if solver bone is used to obtain target position and orientation. */
	void SetUseSolverBone( bool useSolverBone );
	
	/** \brief Name of the solver bone if used. */
	inline const decString &GetSolverBone() const{ return pSolverBone; }
	
	/** \brief Set the name of the solver bone. */
	void SetSolverBone( const char *boneName );
	
	
	
	/** \brief Limit reach in meters. 0 disables reach limit. */
	inline float GetReachRange() const{ return pReachRange; }
	
	/** \brief Set limit reach in meters. 0 disables reach limit. */
	void SetReachRange( float range );
	
	/** \brief Bone to use as reach origin or an empty string to use a constant position. */
	inline const decString &GetReachBone() const{ return pReachBone; }
	
	/**
	 * \brief Set bone to use as reach origin or an empty string to use a constant position.
	 * \throws deeInvalidParam \em bone is \em NULL.
	 */
	void SetReachBone( const char *bone );
	
	/** \brief Reach center if no bone is used. */
	inline const decVector &GetReachCenter() const{ return pReachCenter; }
	
	/** \brief Set reach center if no bone is used. */
	void SetReachCenter( const decVector &center );
	
	
	
	/** \brief Goal position target. */
	inline aeControllerTarget &GetTargetGoalPosition(){ return pTargetGoalPosition; }
	inline const aeControllerTarget &GetTargetGoalPosition() const{ return pTargetGoalPosition; }
	
	/** \brief Goal orientation target. */
	inline aeControllerTarget &GetTargetGoalOrientation(){ return pTargetGoalOrientation; }
	inline const aeControllerTarget &GetTargetGoalOrientation() const{ return pTargetGoalOrientation; }
	
	/** \brief Local position target. */
	inline aeControllerTarget &GetTargetLocalPosition(){ return pTargetLocalPosition; }
	inline const aeControllerTarget &GetTargetLocalPosition() const{ return pTargetLocalPosition; }
	
	/** \brief Local orientation target. */
	inline aeControllerTarget &GetTargetLocalOrientation(){ return pTargetLocalOrientation; }
	inline const aeControllerTarget &GetTargetLocalOrientation() const{ return pTargetLocalOrientation; }
	
	/** \brief Reach range target. */
	inline aeControllerTarget &GetTargetReachRange(){ return pTargetReachRange; }
	inline const aeControllerTarget &GetTargetReachRange() const{ return pTargetReachRange; }
	
	/** \brief Reach center target. */
	inline aeControllerTarget &GetTargetReachCenter(){ return pTargetReachCenter; }
	inline const aeControllerTarget &GetTargetReachCenter() const{ return pTargetReachCenter; }
	
	
	
	/** \brief Create engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	
	/** \brief Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another inverse kinematic rule to this inverse kinematic rule. */
	virtual aeRuleInverseKinematic &operator=( const aeRuleInverseKinematic &copy );
	/*@}*/
};

#endif
