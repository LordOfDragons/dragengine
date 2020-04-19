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

#ifndef _DEARRULELIMIT_H_
#define _DEARRULELIMIT_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>

#include "dearRule.h"



/**
 * \brief Animator rule limit.
 */
class dearRuleLimit : public dearRule{
private:
	const deAnimatorRuleLimit &pLimit;
	
	int pTargetBone;
	
	const bool pEnablePositionXMin;
	const bool pEnablePositionXMax;
	const bool pEnablePositionYMin;
	const bool pEnablePositionYMax;
	const bool pEnablePositionZMin;
	const bool pEnablePositionZMax;
	const bool pEnablePositionAny;
	
	const bool pEnableRotationXMin;
	const bool pEnableRotationXMax;
	const bool pEnableRotationYMin;
	const bool pEnableRotationYMax;
	const bool pEnableRotationZMin;
	const bool pEnableRotationZMax;
	const bool pEnableRotationAny;
	
	const bool pEnableScalingXMin;
	const bool pEnableScalingXMax;
	const bool pEnableScalingYMin;
	const bool pEnableScalingYMax;
	const bool pEnableScalingZMin;
	const bool pEnableScalingZMax;
	const bool pEnableScalingAny;
	
	const bool pEnabledAny;
	
	const deAnimatorRuleLimit::eCoordinateFrames pCoordinateFrame;
	const decVector pMinPosition;
	const decVector pMaxPosition;
	const decVector pMinRotation;
	const decVector pMaxRotation;
	const decVector pMinScaling;
	const decVector pMaxScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRuleLimit( dearAnimatorInstance &instance, int firstLink, const deAnimatorRuleLimit &rule );
	
	/** \brief Clean up animator. */
	virtual ~dearRuleLimit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Target bone index. */
	inline int GetTargetBone() const{ return pTargetBone; }
	
	
	
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist );
	
	/** \brief Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateTargetBone();
};

#endif
