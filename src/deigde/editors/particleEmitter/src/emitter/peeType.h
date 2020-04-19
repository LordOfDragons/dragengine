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

#ifndef _PEETYPE_H_
#define _PEETYPE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/common/string/decString.h>

class peeEmitter;
class peeParameter;
class deComponent;
class deModel;
class deSkin;
class deEngine;



/**
 * \brief Particle emitter type.
 */
class peeType : public deObject{
private:
	peeEmitter *pEmitter;
	deEngine *pEngine;
	deSkin *pEngSkin;
	deModel *pEngCastModel;
	deSkin *pEngCastSkin;
	
	int pIndex;
	decString pName;
	
	peeParameter *pParameters[ 31 ];
	peeParameter *pActiveParameter;
	
	decString pSkinPath;
	decString pModelPath;
	decString pModelSkinPath;
	deParticleEmitterType::eCastFrom pCastFrom;
	deParticleEmitterType::eSimulationTypes pSimulationType;
	bool pIntervalAsDistance;
	
	decString pPathTrailEmitter;
	deParticleEmitter *pEngTrailEmitter;
	decString pTrailControllers[ 4 ];
	deParticleEmitterType::eEmitControllers pActiveTrailController;
	
	float pPhysicsSize;
	deParticleEmitterType::eCollisionResponses pCollisionResponse;
	decString pPathCollisionEmitter;
	deParticleEmitter *pEngCollisionEmitter;
	float pEmitMinImpulse;
	decString pEmitControllers[ 4 ];
	deParticleEmitterType::eEmitControllers pActiveEmitController;
	
	bool pActive;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new controller. */
	peeType( deEngine *engine, const char *name = "Type" );
	
	/** \brief Clean up controller. */
	virtual ~peeType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set parent emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	
	
	/** \brief Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Engine skin. */
	inline deSkin *GetEngineSkin() const{ return pEngSkin; }
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	
	/** \brief Set skin path. */
	void SetSkinPath( const char *path );
	
	/** \brief Model path. */
	inline const decString &GetModelPath() const{ return pModelPath; }
	
	/** \brief Set model path. */
	void SetModelPath( const char *path );
	
	/** \brief Model skin path. */
	inline const decString &GetModelSkinPath() const{ return pModelSkinPath; }
	
	/** \brief Set model skin path. */
	void SetModelSkinPath( const char *path );
	
	/** \brief What part of the model to cast particles from. */
	inline deParticleEmitterType::eCastFrom GetCastFrom() const{ return pCastFrom; }
	
	/** \brief Set what part of the model to cast particles from. */
	void SetCastFrom( deParticleEmitterType::eCastFrom castFrom );
	
	/** \brief Simulation type. */
	inline deParticleEmitterType::eSimulationTypes GetSimulationType() const{ return pSimulationType; }
	
	/** \brief Set simulation type. */
	void SetSimulationType( deParticleEmitterType::eSimulationTypes simulationType );
	
	/** \brief Interval parameter is used as distance not time. */
	inline bool GetIntervalAsDistance() const{ return pIntervalAsDistance; }
	
	/** \brief Set if interval parameter is used as distance not time. */
	void SetIntervalAsDistance( bool intervalAsDistance );
	
	/** \brief Path to trail particle emitter. */
	inline const decString &GetPathTrailEmitter() const{ return pPathTrailEmitter; }
	
	/** \brief Set path to trail particle emitter. */
	void SetPathTrailEmitter( const char *path );
	
	/** \brief Trail controller name or empty string if not set. */
	const decString &GetTrailController( deParticleEmitterType::eEmitControllers controller ) const;
	
	/** \brief Set trail controller name or empty string if not set. */
	void SetTrailController( deParticleEmitterType::eEmitControllers controller, const char *targetController );
	
	/** \brief Index of the active trail controller. */
	inline deParticleEmitterType::eEmitControllers GetActiveTrailController() const{ return pActiveTrailController; }
	
	/** \brief Set index of the active trail controller. */
	void SetActiveTrailController( deParticleEmitterType::eEmitControllers index );
	
	
	
	/** \brief Physics size. */
	inline float GetPhysicsSize() const{ return pPhysicsSize; }
	
	/** \brief Set physics size. */
	void SetPhysicsSize( float size );
	
	/** \brief Collision response. */
	inline deParticleEmitterType::eCollisionResponses GetCollisionResponse() const{ return pCollisionResponse; }
	
	/** \brief Set collision response. */
	void SetCollisionResponse( deParticleEmitterType::eCollisionResponses response );
	
	/** \brief Path to collision particle emitter. */
	inline const decString &GetPathCollisionEmitter() const{ return pPathCollisionEmitter; }
	
	/** \brief Set path to collision particle emitter. */
	void SetPathCollisionEmitter( const char *path );
	
	/** \brief Minimum impulse required for colliding particles to emit instances. */
	inline float GetEmitMinImpulse() const{ return pEmitMinImpulse; }
	
	/** \brief Set minimum impulse required for colliding particles to emit instances. */
	void SetEmitMinImpulse( float impulse );
	
	/** \brief Emit controller name or empty string if not set. */
	const decString &GetEmitController( deParticleEmitterType::eEmitControllers controller ) const;
	
	/** \brief Set emit controller name or empty string if not set. */
	void SetEmitController( deParticleEmitterType::eEmitControllers controller, const char *targetController );
	
	/** \brief Index of the active emit controller. */
	inline deParticleEmitterType::eEmitControllers GetActiveEmitController() const{ return pActiveEmitController; }
	
	/** \brief Set index of the active emit controller. */
	void SetActiveEmitController( deParticleEmitterType::eEmitControllers index );
	
	
	
	/** \brief Controller is the active controller. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if this is the active controller. */
	void SetActive( bool active );
	
	
	
	/** \brief Notify listeners that the type changed. */
	void NotifyChanged();
	
	/** \brief Notify listeners that the active trail controller changed. */
	void NotifyActiveTrailControllerChanged();
	
	/** \brief Notify listeners that the active emit controller changed. */
	void NotifyActiveEmitControllerChanged();
	
	
	
	/** \brief Update engine type. */
	void UpdateEngineType( deParticleEmitterType &type );
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Parameter. */
	peeParameter *GetParameterAt( deParticleEmitterType::eParameters parameter ) const;
	
	/** \brief Index of parameter or -1 of not found. */
	int IndexOfParameter( peeParameter *parameter ) const;
	
	/** \brief Active parameter. */
	inline peeParameter *GetActiveParameter() const{ return pActiveParameter; }
	
	/** \brief Set active parameter. */
	void SetActiveParameter( peeParameter *parameter );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSetParameter( deParticleEmitterType::eParameters index, float value, float spread );
	void pLoadSkin();
	void pLoadModel();
	void pLoadModelSkin();
	void pLoadCollisionEmitter();
	void pLoadTrailEmitter();
};

#endif
