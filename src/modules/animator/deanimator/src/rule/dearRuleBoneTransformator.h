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

#ifndef _DEARRULEBONETRANSFORMATOR_H_
#define _DEARRULEBONETRANSFORMATOR_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>

#include "dearRule.h"



/**
 * \brief Base animator rule.
 */
class dearRuleBoneTransformator : public dearRule{
private:
	const deAnimatorRuleBoneTransformator &pBoneTransformator;
	
	int pTargetBone;
	
	dearControllerTarget pTargetTranslation;
	dearControllerTarget pTargetRotation;
	dearControllerTarget pTargetScaling;
	
	const deAnimatorRuleBoneTransformator::eCoordinateFrames pCoordinateFrame;
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pUseAxis;
	const decVector pMinTranslation;
	const decVector pMaxTranslation;
	const decVector pMinRotation;
	const decVector pMaxRotation;
	const decVector pMinScaling;
	const decVector pMaxScaling;
	const decVector pAxis;
	const float pMinAngle;
	const float pMaxAngle;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRuleBoneTransformator( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleBoneTransformator &rule );
	
	/** \brief Clean up rule. */
	virtual ~dearRuleBoneTransformator();
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
