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
