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

#ifndef _DEARRULESTATEMANIPULATOR_H_
#define _DEARRULESTATEMANIPULATOR_H_

#include "dearRule.h"
#include "dragengine/common/math/decMath.h"

class deAnimatorRuleStateManipulator;



/**
 * \brief State manipulator rule.
 */
class dearRuleStateManipulator : public dearRule{
private:
	//const deAnimatorRuleStateManipulator &pStateManipulator;
	
	dearControllerTarget pTargetPosition;
	dearControllerTarget pTargetRotation;
	dearControllerTarget pTargetSize;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableScale;
	const decVector pMinPosition;
	const decVector pMaxPosition;
	const decVector pMinRotation;
	const decVector pMaxRotation;
	const decVector pMinSize;
	const decVector pMaxSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRuleStateManipulator( dearAnimatorInstance &instance, int firstLink,
		const deAnimatorRuleStateManipulator &rule );
	
	/** \brief Clean up animator. */
	virtual ~dearRuleStateManipulator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist );
	/*@}*/
};

#endif
