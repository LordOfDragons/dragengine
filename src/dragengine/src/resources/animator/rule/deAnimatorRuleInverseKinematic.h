/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
class DE_DLL_EXPORT deAnimatorRuleInverseKinematic : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleInverseKinematic> Ref;
	
	
private:
	decVector pGoalPosition;
	decQuaternion pGoalOrientation;
	decVector pLocalPosition;
	decQuaternion pLocalOrientation;
	bool pAdjustPosition;
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
	void SetGoalPosition(const decVector &position);
	
	/** \brief Goal orientation. */
	inline const decQuaternion &GetGoalOrientation() const{ return pGoalOrientation; }
	
	/** \brief Set goal orientation. */
	void SetGoalOrientation(const decQuaternion &orientation);
	
	/** \brief Local position. */
	inline const decVector &GetLocalPosition() const{ return pLocalPosition; }
	
	/** \brief Set local position. */
	void SetLocalPosition(const decVector &position);
	
	/** \brief Local orientation. */
	inline const decQuaternion &GetLocalOrientation() const{ return pLocalOrientation; }
	
	/** \brief Set local orientation. */
	void SetLocalOrientation(const decQuaternion &orientation);
	
	/**
	 * \brief Adjust position.
	 * \version 1.6
	 */
	inline bool GetAdjustPosition() const{ return pAdjustPosition; }
	
	/**
	 * \brief Set if the position is adjusted.
	 * \version 1.6
	 */
	void SetAdjustPosition(bool adjust);
	
	/** \brief Adjust orientation. */
	inline bool GetAdjustOrientation() const{ return pAdjustOrientation; }
	
	/** \brief Set if the orientation is adjusted. */
	void SetAdjustOrientation(bool adjust);
	
	
	
	/** \brief Use solver bone to obtain target position and orientation. */
	inline bool GetUseSolverBone() const{ return pUseSolverBone; }
	
	/** \brief Set if the solver bone is used to obtain target position and orientation. */
	void SetUseSolverBone(bool useSolverBone);
	
	/** \brief Name of the solver bone or empty string if not used. */
	inline const decString &GetSolverBone() const{ return pSolverBone; }
	
	/** \brief Set name of the solver bone or empty string if not used. */
	void SetSolverBone(const char *boneName);
	
	
	
	/** \brief Limit reach in meters. 0 disables reach limit. */
	inline float GetReachRange() const{ return pReachRange; }
	
	/** \brief Set limit reach in meters. 0 disables reach limit. */
	void SetReachRange(float range);
	
	/** \brief Bone to use as reach origin or an empty string to use a constant position. */
	inline const decString &GetReachBone() const{ return pReachBone; }
	
	/**
	 * \brief Set bone to use as reach origin or an empty string to use a constant position.
	 * \throws deeInvalidParam \em bone is NULL.
	 */
	void SetReachBone(const char *bone);
	
	/** \brief Reach center if no bone is used. */
	inline const decVector &GetReachCenter() const{ return pReachCenter; }
	
	/** \brief Set reach center if no bone is used. */
	void SetReachCenter(const decVector &center);
	
	
	
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
	virtual void Visit(deAnimatorRuleVisitor &visitor);
	/*@}*/
};

#endif
