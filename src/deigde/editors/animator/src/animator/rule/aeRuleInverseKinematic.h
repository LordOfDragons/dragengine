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

#ifndef _AERULEINVERSEKINEMATIC_H_
#define _AERULEINVERSEKINEMATIC_H_

#include "aeRule.h"



/**
 * Animator rule inverse kinematic.
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
	
	aeControllerTarget::Ref pTargetGoalPosition;
	aeControllerTarget::Ref pTargetGoalOrientation;
	aeControllerTarget::Ref pTargetLocalPosition;
	aeControllerTarget::Ref pTargetLocalOrientation;
	aeControllerTarget::Ref pTargetReachRange;
	aeControllerTarget::Ref pTargetReachCenter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create inverse kinematic rule. */
	aeRuleInverseKinematic();
	
	/** Create copy of an inverse kinematic rule. */
	aeRuleInverseKinematic(const aeRuleInverseKinematic &copy);
	
	/** Clean up the animator rule. */
protected:
	~aeRuleInverseKinematic() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Goal position. */
	inline const decVector &GetGoalPosition() const{ return pGoalPosition; }
	
	/** Set goal position. */
	void SetGoalPosition(const decVector &position);
	
	/** Goal orientation. */
	inline const decVector &GetGoalOrientation() const{ return pGoalOrientation; }
	
	/** Set goal orientation. */
	void SetGoalOrientation(const decVector &orientation);
	
	/** Local position. */
	inline const decVector &GetLocalPosition() const{ return pLocalPosition; }
	
	/** Set local position. */
	void SetLocalPosition(const decVector &position);
	
	/** Local orientation. */
	inline const decVector &GetLocalOrientation() const{ return pLocalOrientation; }
	
	/** Set local orientation. */
	void SetLocalOrientation(const decVector &orientation);
	
	/** Adjust orientation. */
	inline bool GetAdjustOrientation() const{ return pAdjustOrientation; }
	
	/** Set if the orientation is adjusted. */
	void SetAdjustOrientation(bool adjustOrientation);
	
	/** Use solver bone to obtain target position and orientation. */
	inline bool GetUseSolverBone() const{ return pUseSolverBone; }
	
	/** Set if solver bone is used to obtain target position and orientation. */
	void SetUseSolverBone(bool useSolverBone);
	
	/** Name of the solver bone if used. */
	inline const decString &GetSolverBone() const{ return pSolverBone; }
	
	/** Set the name of the solver bone. */
	void SetSolverBone(const char *boneName);
	
	
	
	/** Limit reach in meters. 0 disables reach limit. */
	inline float GetReachRange() const{ return pReachRange; }
	
	/** Set limit reach in meters. 0 disables reach limit. */
	void SetReachRange(float range);
	
	/** Bone to use as reach origin or an empty string to use a constant position. */
	inline const decString &GetReachBone() const{ return pReachBone; }
	
	/**
	 * Set bone to use as reach origin or an empty string to use a constant position.
	 * \throws deeInvalidParam \em bone is \em nullptr.
	 */
	void SetReachBone(const char *bone);
	
	/** Reach center if no bone is used. */
	inline const decVector &GetReachCenter() const{ return pReachCenter; }
	
	/** Set reach center if no bone is used. */
	void SetReachCenter(const decVector &center);
	
	
	
	/** Goal position target. */
	inline const aeControllerTarget::Ref &GetTargetGoalPosition() const{ return pTargetGoalPosition; }
	
	/** Goal orientation target. */
	inline const aeControllerTarget::Ref &GetTargetGoalOrientation() const{ return pTargetGoalOrientation; }
	
	/** Local position target. */
	inline const aeControllerTarget::Ref &GetTargetLocalPosition() const{ return pTargetLocalPosition; }
	
	/** Local orientation target. */
	inline const aeControllerTarget::Ref &GetTargetLocalOrientation() const{ return pTargetLocalOrientation; }
	
	/** Reach range target. */
	inline const aeControllerTarget::Ref &GetTargetReachRange() const{ return pTargetReachRange; }
	
	/** Reach center target. */
	inline const aeControllerTarget::Ref &GetTargetReachCenter() const{ return pTargetReachCenter; }
	
	
	
	/** Create engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Update targets. */
	void UpdateTargets() override;
	
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	
	/** Remove a link from all targets using it. */
	void RemoveLinkFromTargets(aeLink *link) override;
	
	/** Remove all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another inverse kinematic rule to this inverse kinematic rule. */
	virtual aeRuleInverseKinematic &operator=(const aeRuleInverseKinematic &copy);
	/*@}*/
};

#endif
