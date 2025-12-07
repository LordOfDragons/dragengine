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
	void SetEmitter(deParticleEmitter *emitter);
	/** Retrieves the type. */
	inline int GetType() const{ return pType; }
	/** Sets the type. */
	void SetType(int type);
	
	/** Retrieves the skin or NULL if not set. */
	inline debpSkin *GetSkin() const{ return pSkin; }
	/** Retrieves the model or NULL if not set. */
	inline debpModel *GetModel() const{ return pModel; }
	/** Retrieves the model skin or NULL if not set. */
	inline debpSkin *GetModelSkin() const{ return pModelSkin; }
	
	/** Retrieves the linear velocity parameter factor. */
	inline float GetParamFactorLinVelo() const{ return pParamFactorLinVelo; }
	/** Sets the linear velocity parameter factor. */
	void SetParamFactorLinVelo(float factor);
	/** Retrieves the angular velocity parameter factor. */
	inline float GetParamFactorAngVelo() const{ return pParamFactorAngVelo; }
	/** Sets the angular velocity parameter factor. */
	void SetParamFactorAngVelo(float factor);
	
	/** Update type. */
	void UpdateType();
	
	/** Update parameters. */
	void UpdateParameters();
	/** Evaluate progress parameter. */
	float EvaluateProgressParameter(int curve, float lifetime) const;
	/** Evaluate cast parameter. */
	float EvaluateCastParameter(const deParticleEmitterInstance &instance, int curve, deParticleEmitterType::eParameters parameter) const;
	/** Evaluate value parameter. */
	float EvaluateValueParameter(const deParticleEmitterInstance &instance, int curve, deParticleEmitterType::eParameters parameter) const;
	/** Evaluate spread parameter. */
	float EvaluateSpreadParameter(const deParticleEmitterInstance &instance, int curve, deParticleEmitterType::eParameters parameter) const;
	/*@}*/
	
private:
	void pSampleParameters(int curve, const deParticleEmitterParameter &parameter);
};

#endif
