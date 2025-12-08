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

#ifndef _PEEPARAMETER_H_
#define _PEEPARAMETER_H_

#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>

class peeType;
class peeController;
class deParticleEmitterParameter;



/**
 * \brief Emitter Parameter.
 */
class peeParameter{
private:
	peeType &pType;
	deParticleEmitterType::eParameters pIndex;
	
	float pValue;
	float pSpread;
	peeController::Ref pControllerValue;
	peeController::Ref pControllerSpread;
	decCurveBezier pCurveValue;
	decCurveBezier pCurveSpread;
	decCurveBezier pCurveProgress;
	decCurveBezier pCurveBeam;
	
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new parameter. */
	peeParameter(peeType &type, deParticleEmitterType::eParameters index, float value, float spread);
	/** \brief Clean up the parameter. */
	~peeParameter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the type. */
	inline peeType &GetType() const{ return pType; }
	/** Retrieves the index. */
	inline deParticleEmitterType::eParameters GetIndex() const{ return pIndex; }
	
	/** Retrieves the cast value. */
	inline float GetValue() const{ return pValue; }
	/** Sets the cast value. */
	void SetValue(float value);
	/** Retrieves the cast value spread. */
	inline float GetSpread() const{ return pSpread; }
	/** Sets the cast value spread. */
	void SetSpread(float spread);
	/** Retrieves the curve controller for the cast value. */
	inline peeController *GetControllerValue() const{ return pControllerValue; }
	/** Sets the curve controller for the cast value. */
	void SetControllerValue(peeController::Ref controller);
	/** Retrieves the curve controller for the cast value spread. */
	inline peeController *GetControllerSpread() const{ return pControllerSpread; }
	/** Sets the curve controller for the cast value spread. */
	void SetControllerSpread(peeController::Ref controller);
	/** Retrieves the cast value curve. */
	inline const decCurveBezier &GetCurveValue() const{ return pCurveValue; }
	/** Sets the cast value curve. */
	void SetCurveValue(const decCurveBezier &curve);
	/** Retrieves the cast value spread curve. */
	inline const decCurveBezier &GetCurveSpread() const{ return pCurveSpread; }
	/** Sets the cast value spread curve. */
	void SetCurveSpread(const decCurveBezier &curve);
	/** Retrieves the progress curve. */
	inline const decCurveBezier &GetCurveProgress() const{ return pCurveProgress; }
	/** Sets the progress curve. */
	void SetCurveProgress(const decCurveBezier &curve);
	/** Retrieves the beam curve. */
	inline const decCurveBezier &GetCurveBeam() const{ return pCurveBeam; }
	/** Sets the beam curve. */
	void SetCurveBeam(const decCurveBezier &curve);
	
	/** Determines if this is the active parameter. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if this is the active parameter. */
	void SetActive(bool active);
	
	/** Update engine parameter. */
	void UpdateEngineParameter(deParticleEmitterParameter &parameter);
	
	/** Notifies the listeners that the parameter changed. */
	void NotifyChanged();
	/*@}*/
};

#endif
