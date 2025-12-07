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

#ifndef _DEARRULEINVERSEKINEMATIC_H_
#define _DEARRULEINVERSEKINEMATIC_H_

#include "dearRule.h"

class dearIKWorkState;
class deAnimatorRuleInverseKinematic;



/**
 * Inverse kinematic rule.
 */
class dearRuleInverseKinematic : public dearRule{
private:
	struct sParameters{
		float blendFactor;
		decVector goalPosition;
		decQuaternion goalOrientation;
		decVector localPosition;
		decQuaternion localOrientation;
		float reachRange;
		decVector reachCenter;
		bool hasSolverBone;
		bool hasIKLimits;
		float rotationCrossThreshold;
		float goalReachedThreshold;
		float improvedThreshold;
	};
	
	
	const deAnimatorRuleInverseKinematic &pInverseKinematic;
	dearIKWorkState *pChain;
	int pChainCount;
	float pChainLength;
	
	dearControllerTarget pTargetGoalPosition;
	dearControllerTarget pTargetGoalOrientation;
	dearControllerTarget pTargetLocalPosition;
	dearControllerTarget pTargetLocalOrientation;
	dearControllerTarget pTargetReachRange;
	dearControllerTarget pTargetReachCenter;
	
	int pSolverBone;
	int pReachBone;
	
	const bool pAdjustPosition;
	const bool pAdjustOrientation;
	const bool pUseSolverBone;
	
	const decVector pGoalPosition;
	const decQuaternion pGoalOrientation;
	const decVector pLocalPosition;
	const decQuaternion pLocalOrientation;
	
	const float pReachRange;
	const decVector pReachCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleInverseKinematic(dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleInverseKinematic &rule);
	
	/** Clean up rule. */
	virtual ~dearRuleInverseKinematic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Solver bone index. */
	inline int GetSolverBone() const{return pSolverBone;}
	
	
	
	/** Apply to animator. */
	virtual void Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist);
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateChain();
	void pSetChainCount(int count);
	void pInitIKLimits();
	void pUpdateChainWeights();
	void pUpdateReachBone();
	void pLimitReach(dearBoneStateList &stalist, decVector &goalPosition,
		float range, const decVector &center);
	void pApplyAdjustOrientation(decMatrix &goalMatrix,
		decVector &tipPosition, const decVector &localPosition);
	void pInitWorkingStates(dearBoneStateList &stalist, const sParameters &params);
	void pUpdateInverseGlobal();
	void pInitLength(const decVector &localPosition);
	
	void pSolveSingleBone(dearBoneStateList &stalist, const sParameters &params);
	void pSolveCCD(dearBoneStateList &stalist, const sParameters &params);
	void pSolveFabrik(dearBoneStateList &stalist, const sParameters &params);
	void pUpdateBonesFromWorkingState(dearBoneStateList &stalist, const sParameters &params);
	bool pCalcRotation(const sParameters &params, const decVector &goalPosition,
		const decVector &tipPosition, const decVector &bonePosition,
		decVector &rotationAxis, float &rotationAngle);
	decMatrix pGlobalRotationMatrix(const decVector &pivot, const decQuaternion &rotation) const;
	decMatrix pGlobalRotationMatrix(const decVector &pivot, const decVector &axis, float angle) const;
	
	decQuaternion pGlobalOrientationToBoneOrientation(int index,
		const decQuaternion &globalOrientation, const decQuaternion &baseInverseRotation) const;
	
	decQuaternion pApplyIKResistance(int index, const decQuaternion &globalOrientation,
		const decQuaternion &baseInverseRotation, const decQuaternion &rotation) const;
	
	decQuaternion pApplyIKLimits(int index, const decQuaternion &orientation) const;
	
	decQuaternion pBoneOrientationToGlobalOrientation(int index,
		const decQuaternion &baseRotation, const decQuaternion &orientation) const;
	
	decQuaternion pApplyIKRestrictions(int index, const decMatrix &globalMatrix,
		const decQuaternion &baseRotation, const decQuaternion &baseInverseRotation,
		const decQuaternion &rotation) const;
};

#endif
