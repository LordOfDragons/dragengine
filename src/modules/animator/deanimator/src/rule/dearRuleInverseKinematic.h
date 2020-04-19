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
 * \brief Inverse kinematic rule.
 */
class dearRuleInverseKinematic : public dearRule{
private:
	const deAnimatorRuleInverseKinematic &pInverseKinematic;
	dearIKWorkState *pChain;
	int pChainCount;
	
	int pSolverBone;
	
	dearControllerTarget pTargetGoalPosition;
	dearControllerTarget pTargetGoalOrientation;
	dearControllerTarget pTargetLocalPosition;
	dearControllerTarget pTargetLocalOrientation;
	
	dearControllerTarget pTargetReachRange;
	dearControllerTarget pTargetReachCenter;
	
	int pReachBone;
	
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
	/** \brief Create rule. */
	dearRuleInverseKinematic( dearAnimatorInstance &instance, int firstLink,
		const deAnimatorRuleInverseKinematic &rule );
	
	/** \brief Clean up rule. */
	virtual ~dearRuleInverseKinematic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Solver bone index. */
	inline int GetSolverBone() const{ return pSolverBone; }
	
	
	
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist );
	
	/** \brief Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateChain();
	void pSetChainCount( int count );
	void pInitIKLimits();
	void pUpdateReachBone();
	void pLimitReach( dearBoneStateList &stalist, decVector &goalPosition,
		float range, const decVector &center );
};

#endif
