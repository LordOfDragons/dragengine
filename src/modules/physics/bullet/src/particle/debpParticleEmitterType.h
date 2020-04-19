/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPPROPPARTICLEEMITTERTYPE_H_
#define _DEBPPROPPARTICLEEMITTERTYPE_H_

#include "LinearMath/btVector3.h"

#include <dragengine/resources/particle/deParticleEmitterType.h>

class deParticleEmitter;
class deParticleEmitterParameter;
class deParticleEmitterInstance;
class debpModel;
class debpSkin;



/**
 * @brief Particle Emitter Type.
 */
class debpParticleEmitterType{
public:
	enum eSampleCurves{
		escParticleCount,
		escSize,
		escMass,
		escRotation,
		escLinearVelocity,
		escAngularVelocity,
		escBrown,
		escDamp,
		escDrag,
		escGravityX,
		escGravityY,
		escGravityZ,
		escLocalGravity,
		escForceFieldDirect,
		escForceFieldSurface,
		escForceFieldVolume,
		escForceFieldSpeed,
		escElasticity,
		escRoughness,
		escEmitDirection,
		ESC_COUNT_PROGRESS,
		
		escTimeToLive = ESC_COUNT_PROGRESS,
		escInterval,
		escCastAngleX,
		escCastAngleY,
		escRed,
		escGreen,
		escBlue,
		escTransparency,
		escEmissivity,
		ESC_COUNT_CAST
	};
	
	enum eGraParameter{
		egpSize,
		egpRed,
		egpGreen,
		egpBlue,
		egpTransparency,
		egpEmissivity,
		EGP_COUNT
	};
	
private:
	deParticleEmitter *pEmitter;
	int pType;
	
	debpSkin *pSkin;
	debpModel *pModel;
	debpSkin *pModelSkin;
	
	float *pParameterValueSamples;
	float *pParameterSpreadSamples;
	float *pParameterProgressSamples;
	
	float pParamFactorLinVelo;
	float pParamFactorAngVelo;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new type. */
	debpParticleEmitterType();
	/** Cleans up the type. */
	~debpParticleEmitterType();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the emitter. */
	inline deParticleEmitter *GetEmitter() const{ return pEmitter; }
	/** Sets the emitter. */
	void SetEmitter( deParticleEmitter *emitter );
	/** Retrieves the type. */
	inline int GetType() const{ return pType; }
	/** Sets the type. */
	void SetType( int type );
	
	/** Retrieves the skin or NULL if not set. */
	inline debpSkin *GetSkin() const{ return pSkin; }
	/** Retrieves the model or NULL if not set. */
	inline debpModel *GetModel() const{ return pModel; }
	/** Retrieves the model skin or NULL if not set. */
	inline debpSkin *GetModelSkin() const{ return pModelSkin; }
	
	/** Retrieves the linear velocity parameter factor. */
	inline float GetParamFactorLinVelo() const{ return pParamFactorLinVelo; }
	/** Sets the linear velocity parameter factor. */
	void SetParamFactorLinVelo( float factor );
	/** Retrieves the angular velocity parameter factor. */
	inline float GetParamFactorAngVelo() const{ return pParamFactorAngVelo; }
	/** Sets the angular velocity parameter factor. */
	void SetParamFactorAngVelo( float factor );
	
	/** Update type. */
	void UpdateType();
	
	/** Update parameters. */
	void UpdateParameters();
	/** Evaluate progress parameter. */
	float EvaluateProgressParameter( int curve, float lifetime ) const;
	/** Evaluate cast parameter. */
	float EvaluateCastParameter( const deParticleEmitterInstance &instance, int curve, deParticleEmitterType::eParameters parameter ) const;
	/** Evaluate value parameter. */
	float EvaluateValueParameter( const deParticleEmitterInstance &instance, int curve, deParticleEmitterType::eParameters parameter ) const;
	/** Evaluate spread parameter. */
	float EvaluateSpreadParameter( const deParticleEmitterInstance &instance, int curve, deParticleEmitterType::eParameters parameter ) const;
	/*@}*/
	
private:
	void pSampleParameters( int curve, const deParticleEmitterParameter &parameter );
};

#endif
