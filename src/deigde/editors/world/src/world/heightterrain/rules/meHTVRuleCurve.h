/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHTVRULECURVE_H_
#define _MEHTVRULECURVE_H_

#include <dragengine/common/curve/decCurveBezier.h>

#include "meHTVRule.h"



/**
 * \brief Height Terrain Vegetation Rule Curve.
 */
class meHTVRuleCurve : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Mapped value. */
		eosValue,
		/** Value. */
		eisValue
	};
	
private:
	decCurveBezier pCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	meHTVRuleCurve();
	
	/** \brief Create copy of rule. */
	meHTVRuleCurve( const meHTVRuleCurve &rule );
	
protected:
	/** \brief Clean up rule. */
	virtual ~meHTVRuleCurve();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set curve. */
	void SetCurve( const decCurveBezier &curve );
	
	/** \brief Value of output slot. */
	virtual float GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv );
	
	/** \brief Vector of output slot. */
	virtual decVector GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv );
	
	/** \brief Copy rule. */
	virtual meHTVRule *Copy() const;
	/*@}*/
};

#endif
