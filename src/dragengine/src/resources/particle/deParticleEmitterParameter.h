/* 
 * Drag[en]gine Game Engine
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
class deParticleEmitterParameter{
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
	void SetValue( float value );
	
	/** \brief Cast value spread. */
	inline float GetSpread() const{ return pSpread; }
	
	/** \brief Set cast value spread. */
	void SetSpread( float spread );
	
	/** \brief Curve controller for the cast value. */
	inline int GetControllerValue() const{ return pControllerValue; }
	
	/** \brief Set curve controller for the cast value. */
	void SetControllerValue( int controller );
	
	/** \brief Curve controller for the cast value spread. */
	inline int GetControllerSpread() const{ return pControllerSpread; }
	
	/** \brief Set curve controller for the cast value spread. */
	void SetControllerSpread( int controller );
	
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
