/* 
 * Drag[en]gine Animator Module
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
	dearRuleInverseKinematic( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleInverseKinematic &rule );
	
	/** Clean up rule. */
	virtual ~dearRuleInverseKinematic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Solver bone index. */
	inline int GetSolverBone() const{ return pSolverBone; }
	
	
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateChain();
	void pSetChainCount( int count );
	void pInitIKLimits();
	void pUpdateChainWeights();
	void pUpdateReachBone();
	void pLimitReach( dearBoneStateList &stalist, decVector &goalPosition,
		float range, const decVector &center );
	void pApplyAdjustOrientation( decMatrix &goalMatrix,
		decVector &tipPosition, const decVector &localPosition );
	void pInitWorkingStates( dearBoneStateList &stalist, const sParameters &params );
	void pUpdateInverseGlobal();
	void pInitLength( const decVector &localPosition );
	
	void pSolveSingleBone( dearBoneStateList &stalist, const sParameters &params );
	void pSolveCCD( dearBoneStateList &stalist, const sParameters &params );
	void pSolveFabrik( dearBoneStateList &stalist, const sParameters &params );
	void pUpdateBonesFromWorkingState( dearBoneStateList &stalist, const sParameters &params );
	bool pCalcRotation( const sParameters &params, const decVector &goalPosition,
		const decVector &tipPosition, const decVector &bonePosition,
		decVector &rotationAxis, float &rotationAngle );
	decMatrix pGlobalRotationMatrix( const decVector &pivot, const decQuaternion &rotation ) const;
	decMatrix pGlobalRotationMatrix( const decVector &pivot, const decVector &axis, float angle ) const;
	
	decQuaternion pGlobalOrientationToBoneOrientation( int index,
		const decQuaternion &globalOrientation, const decQuaternion &baseInverseRotation ) const;
	
	decQuaternion pApplyIKResistance( int index, const decQuaternion &globalOrientation,
		const decQuaternion &baseInverseRotation, const decQuaternion &rotation ) const;
	
	decQuaternion pApplyIKLimits( int index, const decQuaternion &orientation ) const;
	
	decQuaternion pBoneOrientationToGlobalOrientation( int index,
		const decQuaternion &baseRotation, const decQuaternion &orientation ) const;
	
	decQuaternion pApplyIKRestrictions( int index, const decMatrix &globalMatrix,
		const decQuaternion &baseRotation, const decQuaternion &baseInverseRotation,
		const decQuaternion &rotation ) const;
};

#endif
