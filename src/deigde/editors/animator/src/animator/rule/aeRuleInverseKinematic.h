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

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>

/**
 * Animator rule inverse kinematic.
 */
class aeRuleInverseKinematic : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleInverseKinematic>;
	
private:
	aeControllerTarget::Ref pTargetGoalPosition;
	aeControllerTarget::Ref pTargetGoalOrientation;
	aeControllerTarget::Ref pTargetLocalPosition;
	aeControllerTarget::Ref pTargetLocalOrientation;
	aeControllerTarget::Ref pTargetReachRange;
	aeControllerTarget::Ref pTargetReachCenter;
	
public:
	igdeMetaPropertyVectorStorage::Storage goalPosition;
	igdeMetaPropertyVectorStorage::Storage goalOrientation;
	igdeMetaPropertyVectorStorage::Storage localPosition;
	igdeMetaPropertyVectorStorage::Storage localOrientation;
	igdeMetaPropertyBooleanStorage::Storage adjustOrientation;
	igdeMetaPropertyBooleanStorage::Storage useSolverBone;
	igdeMetaPropertyStringStorage::Storage solverBone;
	igdeMetaPropertyFloatStorage::Storage reachRange;
	igdeMetaPropertyStringStorage::Storage reachBone;
	igdeMetaPropertyVectorStorage::Storage reachCenter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create inverse kinematic rule. */
	aeRuleInverseKinematic(aeWindowMain &windowMain, const char *name);
	
	/** Create copy of an inverse kinematic rule. */
	aeRuleInverseKinematic(aeWindowMain &windowMain, const aeRuleInverseKinematic &copy);
	
	/** Clean up the animator rule. */
protected:
	~aeRuleInverseKinematic() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Goal position. */
	inline const decVector &GetGoalPosition() const{ return goalPosition; }
	
	/** Set goal position. */
	void SetGoalPosition(const decVector &value);
	
	/** Goal orientation. */
	inline const decVector &GetGoalOrientation() const{ return goalOrientation; }
	
	/** Set goal orientation. */
	void SetGoalOrientation(const decVector &value);
	
	/** Local position. */
	inline const decVector &GetLocalPosition() const{ return localPosition; }
	
	/** Set local position. */
	void SetLocalPosition(const decVector &value);
	
	/** Local orientation. */
	inline const decVector &GetLocalOrientation() const{ return localOrientation; }
	
	/** Set local orientation. */
	void SetLocalOrientation(const decVector &value);
	
	/** Adjust orientation. */
	inline bool GetAdjustOrientation() const{ return adjustOrientation; }
	
	/** Set if the orientation is adjusted. */
	void SetAdjustOrientation(bool value);
	
	/** Use solver bone to obtain target position and orientation. */
	inline bool GetUseSolverBone() const{ return useSolverBone; }
	
	/** Set if solver bone is used to obtain target position and orientation. */
	void SetUseSolverBone(bool value);
	
	/** Name of the solver bone if used. */
	inline const decString &GetSolverBone() const{ return solverBone; }
	
	/** Set the name of the solver bone. */
	void SetSolverBone(const char *boneName);
	
	
	
	/** Limit reach in meters. 0 disables reach limit. */
	inline float GetReachRange() const{ return reachRange; }
	
	/** Set limit reach in meters. 0 disables reach limit. */
	void SetReachRange(float range);
	
	/** Bone to use as reach origin or an empty string to use a constant position. */
	inline const decString &GetReachBone() const{ return reachBone; }
	
	/**
	 * Set bone to use as reach origin or an empty string to use a constant position.
	 * \throws deeInvalidParam \em bone is \em nullptr.
	 */
	void SetReachBone(const char *value);
	
	/** Reach center if no bone is used. */
	inline const decVector &GetReachCenter() const{ return reachCenter; }
	
	/** Set reach center if no bone is used. */
	void SetReachCenter(const decVector &value);
	
	
	
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
