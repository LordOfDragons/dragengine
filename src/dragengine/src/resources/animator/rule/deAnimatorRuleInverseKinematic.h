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

#ifndef _DEANIMATORRULEINVERSEKINEMATIC_H_
#define _DEANIMATORRULEINVERSEKINEMATIC_H_

#include "deAnimatorRule.h"
#include "../../../common/math/decMath.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Inverse Kinematic Rule Class.
 * 
 * Inverse kinematic is the process of adjusting a set of bones in a way that the
 * end of the bone-chain is located at a given point in space. A typical application
 * is to make an actor push a button no matter where in relation to the button he is
 * standing. This rule provides this behavior. You specify the reference-point in
 * component-space and a reference-offset in the bone-space of the tail bone ( the
 * distance of the reference-point to the bone ). Then you need to also specify
 * which bones make up the bone-chain. For this you need to specify two bones one
 * being the start of the chain and the other the end of it. The chain made up by
 * those two is then adjusted to make the reference-point coincident with an imaginary
 * point located reference-offset from the tail bone ( this imaginary point would be
 * the finger tip ). There are no additional controllers in this rule.
 */
class deAnimatorRuleInverseKinematic : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleInverseKinematic> Ref;
	
	
	
private:
	decVector pGoalPosition;
	decQuaternion pGoalOrientation;
	decVector pLocalPosition;
	decQuaternion pLocalOrientation;
	bool pAdjustOrientation;
	
	bool pUseSolverBone;
	decString pSolverBone;
	
	float pReachRange;
	decString pReachBone;
	decVector pReachCenter;
	
	deAnimatorControllerTarget pTargetGoalPosition;
	deAnimatorControllerTarget pTargetGoalOrientation;
	deAnimatorControllerTarget pTargetLocalPosition;
	deAnimatorControllerTarget pTargetLocalOrientation;
	deAnimatorControllerTarget pTargetReachRange;
	deAnimatorControllerTarget pTargetReachCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator rule. */
	deAnimatorRuleInverseKinematic();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleInverseKinematic();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Goal position. */
	inline const decVector &GetGoalPosition() const{ return pGoalPosition; }
	
	/** \brief Set goal position. */
	void SetGoalPosition( const decVector &position );
	
	/** \brief Goal orientation. */
	inline const decQuaternion &GetGoalOrientation() const{ return pGoalOrientation; }
	
	/** \brief Set goal orientation. */
	void SetGoalOrientation( const decQuaternion &orientation );
	
	/** \brief Local position. */
	inline const decVector &GetLocalPosition() const{ return pLocalPosition; }
	
	/** \brief Set local position. */
	void SetLocalPosition( const decVector &position );
	
	/** \brief Local orientation. */
	inline const decQuaternion &GetLocalOrientation() const{ return pLocalOrientation; }
	
	/** \brief Set local orientation. */
	void SetLocalOrientation( const decQuaternion &orientation );
	
	/** \brief Adjust orientation. */
	inline bool GetAdjustOrientation() const{ return pAdjustOrientation; }
	
	/** \brief Set if the orientation is adjusted. */
	void SetAdjustOrientation( bool adjustOrientation );
	
	
	
	/** \brief Use solver bone to obtain target position and orientation. */
	inline bool GetUseSolverBone() const{ return pUseSolverBone; }
	
	/** \brief Set if the solver bone is used to obtain target position and orientation. */
	void SetUseSolverBone( bool useSolverBone );
	
	/** \brief Name of the solver bone or empty string if not used. */
	inline const decString &GetSolverBone() const{ return pSolverBone; }
	
	/** \brief Set name of the solver bone or empty string if not used. */
	void SetSolverBone( const char *boneName );
	
	
	
	/** \brief Limit reach in meters. 0 disables reach limit. */
	inline float GetReachRange() const{ return pReachRange; }
	
	/** \brief Set limit reach in meters. 0 disables reach limit. */
	void SetReachRange( float range );
	
	/** \brief Bone to use as reach origin or an empty string to use a constant position. */
	inline const decString &GetReachBone() const{ return pReachBone; }
	
	/**
	 * \brief Set bone to use as reach origin or an empty string to use a constant position.
	 * \throws deeInvalidParam \em bone is NULL.
	 */
	void SetReachBone( const char *bone );
	
	/** \brief Reach center if no bone is used. */
	inline const decVector &GetReachCenter() const{ return pReachCenter; }
	
	/** \brief Set reach center if no bone is used. */
	void SetReachCenter( const decVector &center );
	
	
	
	/** \brief Goal position target. */
	inline deAnimatorControllerTarget &GetTargetGoalPosition(){ return pTargetGoalPosition; }
	inline const deAnimatorControllerTarget &GetTargetGoalPosition() const{ return pTargetGoalPosition; }
	
	/** \brief Goal orientation target. */
	inline deAnimatorControllerTarget &GetTargetGoalOrientation(){ return pTargetGoalOrientation; }
	inline const deAnimatorControllerTarget &GetTargetGoalOrientation() const{ return pTargetGoalOrientation; }
	
	/** \brief Local position target. */
	inline deAnimatorControllerTarget &GetTargetLocalPosition(){ return pTargetLocalPosition; }
	inline const deAnimatorControllerTarget &GetTargetLocalPosition() const{ return pTargetLocalPosition; }
	
	/** \brief Local orientation target. */
	inline deAnimatorControllerTarget &GetTargetLocalOrientation(){ return pTargetLocalOrientation; }
	inline const deAnimatorControllerTarget &GetTargetLocalOrientation() const{ return pTargetLocalOrientation; }
	
	/** \brief Reach range target. */
	inline deAnimatorControllerTarget &GetTargetReachRange(){ return pTargetReachRange; }
	inline const deAnimatorControllerTarget &GetTargetReachRange() const{ return pTargetReachRange; }
	
	/** \brief Reach center target. */
	inline deAnimatorControllerTarget &GetTargetReachCenter(){ return pTargetReachCenter; }
	inline const deAnimatorControllerTarget &GetTargetReachCenter() const{ return pTargetReachCenter; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
