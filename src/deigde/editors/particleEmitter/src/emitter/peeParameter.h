/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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
	peeController *pControllerValue;
	peeController *pControllerSpread;
	decCurveBezier pCurveValue;
	decCurveBezier pCurveSpread;
	decCurveBezier pCurveProgress;
	decCurveBezier pCurveBeam;
	
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new parameter. */
	peeParameter( peeType &type, deParticleEmitterType::eParameters index, float value, float spread );
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
	void SetValue( float value );
	/** Retrieves the cast value spread. */
	inline float GetSpread() const{ return pSpread; }
	/** Sets the cast value spread. */
	void SetSpread( float spread );
	/** Retrieves the curve controller for the cast value. */
	inline peeController *GetControllerValue() const{ return pControllerValue; }
	/** Sets the curve controller for the cast value. */
	void SetControllerValue( peeController *controller );
	/** Retrieves the curve controller for the cast value spread. */
	inline peeController *GetControllerSpread() const{ return pControllerSpread; }
	/** Sets the curve controller for the cast value spread. */
	void SetControllerSpread( peeController *controller );
	/** Retrieves the cast value curve. */
	inline const decCurveBezier &GetCurveValue() const{ return pCurveValue; }
	/** Sets the cast value curve. */
	void SetCurveValue( const decCurveBezier &curve );
	/** Retrieves the cast value spread curve. */
	inline const decCurveBezier &GetCurveSpread() const{ return pCurveSpread; }
	/** Sets the cast value spread curve. */
	void SetCurveSpread( const decCurveBezier &curve );
	/** Retrieves the progress curve. */
	inline const decCurveBezier &GetCurveProgress() const{ return pCurveProgress; }
	/** Sets the progress curve. */
	void SetCurveProgress( const decCurveBezier &curve );
	/** Retrieves the beam curve. */
	inline const decCurveBezier &GetCurveBeam() const{ return pCurveBeam; }
	/** Sets the beam curve. */
	void SetCurveBeam( const decCurveBezier &curve );
	
	/** Determines if this is the active parameter. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if this is the active parameter. */
	void SetActive( bool active );
	
	/** Update engine parameter. */
	void UpdateEngineParameter( deParticleEmitterParameter &parameter );
	
	/** Notifies the listeners that the parameter changed. */
	void NotifyChanged();
	/*@}*/
};

#endif
