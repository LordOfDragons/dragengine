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

#ifndef _DEPARTICLEEMITTERPARAMETER_H_
#define _DEPARTICLEEMITTERPARAMETER_H_

#include "../../common/curve/decCurveBezier.h"


/**
 * \brief Particle Emitter Parameter.
 * 
 * Defines an emitter parameter. The cast value is picked randomly from a range specified
 * by value and spread. Spread indicates the total amount of spread around the value. The
 * spread therefore gives the distance from the smallest possible value to the highest
 * possible value. Hence the random value can be spread-half above or below value at most.
 */
class DE_DLL_EXPORT deParticleEmitterParameter{
private:
	float pValue;
	float pSpread;
	int pControllerValue;
	int pControllerSpread;
	decCurveBezier pCurveValue;
	decCurveBezier pCurveSpread;
	decCurveBezier pCurveProgress;
	decCurveBezier pCurveBeam;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new particle emitter parameter. */
	deParticleEmitterParameter();
	
	/** \brief Clean up particle emitter parameter. */
	virtual ~deParticleEmitterParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cast value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set cast value. */
	void SetValue(float value);
	
	/** \brief Cast value spread. */
	inline float GetSpread() const{ return pSpread; }
	
	/** \brief Set cast value spread. */
	void SetSpread(float spread);
	
	/** \brief Curve controller for the cast value. */
	inline int GetControllerValue() const{ return pControllerValue; }
	
	/** \brief Set curve controller for the cast value. */
	void SetControllerValue(int controller);
	
	/** \brief Curve controller for the cast value spread. */
	inline int GetControllerSpread() const{ return pControllerSpread; }
	
	/** \brief Set curve controller for the cast value spread. */
	void SetControllerSpread(int controller);
	
	/** \brief Cast value curve. */
	inline decCurveBezier &GetCurveValue(){ return pCurveValue; }
	inline const decCurveBezier &GetCurveValue() const{ return pCurveValue; }
	
	/** \brief Cast value spread curve. */
	inline decCurveBezier &GetCurveSpread(){ return pCurveSpread; }
	inline const decCurveBezier &GetCurveSpread() const{ return pCurveSpread; }
	
	/** \brief Progress curve. */
	inline decCurveBezier &GetCurveProgress(){ return pCurveProgress; }
	inline const decCurveBezier &GetCurveProgress() const{ return pCurveProgress; }
	
	/** \brief Beam curve. */
	inline decCurveBezier &GetCurveBeam(){ return pCurveBeam; }
	inline const decCurveBezier &GetCurveBeam() const{ return pCurveBeam; }
	/*@}*/
};

#endif
