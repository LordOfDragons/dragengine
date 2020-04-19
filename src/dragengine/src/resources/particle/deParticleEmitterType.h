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

#ifndef _DEPARTICLEEMITTERTYPE_H_
#define _DEPARTICLEEMITTERTYPE_H_

#include "deParticleEmitterParameter.h"
#include "deParticleEmitterReference.h"
#include "../model/deModelReference.h"
#include "../skin/deSkinReference.h"
#include "../../common/math/decMath.h"
#include "../../common/curve/decCurveBezier.h"
#include "../../common/string/decString.h"


/**
 * \brief Particle emitter type.
 * 
 * Defines the properties and behavior of particles of the same type. Each particle type has
 * its own collision behavior. If the destroy on collision parameter is set particles are
 * destroyed hitting solid surfaces. Otherwise they bounce of solid surfaces according to
 * physical laws. If a collision particle emitter is set new instances of this particle emitter
 * are created whenever particles collide with solid surfaces no matter if they are destroyed
 * or bounce. To avoid spamming instances in the case particles are not destroyed upon colliding
 * a minimal collision impulse is required for instances to be created. Furthermore for the impact
 * some parameters can be assigned controllers. Upon creating an instance the specified controllers
 * will be set to the values of the colliding particle. The emission direction is controlled using
 * a blending factor. At 0 the emission direction matches the collision normal. At 1 the emission
 * direction matches the deflection direction. The deflection direction is the direction the
 * particle bounces off the surface if it is not set to be destroyed. Values in between blend
 * linear between these two directions. The default value is 0 hence along the collision normal.
 */
class deParticleEmitterType{
public:
	/** \brief Particle parameters. */
	enum eParameters{
		epTimeToLive,
		epInterval,
		epParticleCount,
		epCastAngleX, // 0..1 => 0° ... 360°
		epCastAngleY, // 0..1 => 0° ... 360°
		epSize,
		epRed,
		epGreen,
		epBlue,
		epTransparency,
		epEmissivity,
		epMass,
		epRotation, // 0..1 => 0° ... 360°
		epLinearVelocity,
		epAngularVelocity, // 0..1 => 0°/s ... 360°/s
		epBrownMotion,
		epDamping,
		epDrag,
		epGravityX,
		epGravityY,
		epGravityZ,
		epLocalGravity,
		epForceFieldDirect,
		epForceFieldSurface,
		epForceFieldMass,
		epForceFieldSpeed,
		epElasticity,
		epRoughness,
		epEmitDirection,
		epBeamStart,
		epBeamEnd
	};
	
	/** \brief Emit controllers. */
	enum eEmitControllers{
		eecLifetime,
		eecMass,
		eecLinearVelocity,
		eecAngularVelocity
	};
	
	/** \brief What model element to cast particles from. */
	enum eCastFrom{
		/** \brief Cast from model vertices. */
		ecfVertex,
		
		/** \brief Cast from model faces. */
		ecfFace,
		
		/** \brief Cast from model volumes. */
		ecfVolume
	};
	
	/** \brief Collision response. */
	enum eCollisionResponses{
		/** \brief Destroy on collision. */
		ecrDestroy,
		
		/** \brief Physical collision response. */
		ecrPhysical,
		
		/** \brief Custom collision response. */
		ecrCustom
	};
	
	/** \brief Simulation type. */
	enum eSimulationTypes{
		/** \brief Particle simulation. */
		estParticle,
		
		/** \brief Ribbon simulation. */
		estRibbon,
		
		/** \brief Beam simulation. */
		estBeam
	};
	
	
	
private:
	deParticleEmitterParameter pParameters[ epBeamEnd + 1 ];
	
	deSkinReference pSkin;
	deModelReference pModel;
	deSkinReference pModelSkin;
	eCastFrom pCastFrom;
	eSimulationTypes pSimulationType;
	bool pIntervalAsDistance;
	
	deParticleEmitterReference pTrailEmitter;
	int pTrailControllers[ eecAngularVelocity + 1 ];
	
	float pPhysicsSize;
	
	eCollisionResponses pCollisionResponse;
	deParticleEmitterReference pCollisionEmitter;
	float pEmitMinImpulse;
	int pEmitControllers[ eecAngularVelocity + 1 ];
	
	float pMaxLinearVelocity;
	float pMaxAngularVelocity;
	bool pSquaredVelocities;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create particle emitter type. */
	deParticleEmitterType();
	
	/** \brief Clean up particle emitter type. */
	~deParticleEmitterType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Skin. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( deSkin *skin );
	
	/** \brief Model used for casting or NULL. */
	inline deModel *GetModel() const{ return pModel; }
	
	/** \brief Set model used for casting or NULL. */
	void SetModel( deModel *model );
	
	/** \brief Skin for the model used for casting or NULL. */
	inline deSkin *GetModelSkin() const{ return pModelSkin; }
	
	/** \brief Set skin for the model used for casting or NULL. */
	void SetModelSkin( deSkin *skin );
	
	/** \brief What part of the model to cast particles from. */
	inline eCastFrom GetCastFrom() const{ return pCastFrom; }
	
	/** \brief Set what part of the model to cast particles from. */
	void SetCastFrom( eCastFrom castFrom );
	
	/** \brief Simulation type. */
	inline eSimulationTypes GetSimulationType() const{ return pSimulationType; }
	
	/** \brief Set simulation type. */
	void SetSimulationType( eSimulationTypes simulationType );
	
	/** \brief Interval parameter is used as distance not time. */
	inline bool GetIntervalAsDistance() const{ return pIntervalAsDistance; }
	
	/** \brief Set if interval parameter is used as distance not time. */
	void SetIntervalAsDistance( bool intervalAsDistance );
	
	
	
	/** \brief Particle emitter for trails behind particles or NULL. */
	inline deParticleEmitter *GetTrailEmitter() const{ return pTrailEmitter; }
	
	/** \brief Set particle emitter for trails behind particles or NULL. */
	void SetTrailEmitter( deParticleEmitter *emitter );
	
	/** \brief Trail controller index or -1 if not set. */
	int GetTrailController( eEmitControllers controller ) const;
	
	/** \brief Set trail controller index or -1 if not set. */
	void SetTrailController( eEmitControllers controller, int targetController );
	
	
	
	/** \brief Physics size. */
	inline float GetPhysicsSize() const{ return pPhysicsSize; }
	
	/** \brief Set physics size. */
	void SetPhysicsSize( float size );
	
	
	
	/** \brief Collision response. */
	inline eCollisionResponses GetCollisionResponse() const{ return pCollisionResponse; }
	
	/** \brief Set collision response. */
	void SetCollisionResponse( eCollisionResponses response );
	
	/** \brief Particle emitter for colliding particles or NULL. */
	inline deParticleEmitter *GetCollisionEmitter() const{ return pCollisionEmitter; }
	
	/** \brief Set particle emitter for colliding particles or NULL. */
	void SetCollisionEmitter( deParticleEmitter *emitter );
	
	/** \brief Minimum impulse required for colliding particles to emit instances. */
	inline float GetEmitMinImpulse() const{ return pEmitMinImpulse; }
	
	/** \brief Set minimum impulse required for colliding particles to emit instances. */
	void SetEmitMinImpulse( float impulse );
	
	/** \brief Emit controller index or -1 if not set. */
	int GetEmitController( eEmitControllers controller ) const;
	
	/** \brief Set emit controller index or -1 if not set. */
	void SetEmitController( eEmitControllers controller, int targetController );
	
	
	
	/** \brief Maximum linear velocity for mapping as requested by the graphic module. */
	inline float GetMaxLinearVelocity() const{ return pMaxLinearVelocity; }
	
	/** \brief Set maximum linear velocity for mapping as requested by the graphic module. */
	void SetMaxLinearVelocity( float maxLinearVelocity );
	
	/** \brief Maximum angular velocity for mapping as requested by the graphic module. */
	inline float GetMaxAngularVelocity() const{ return pMaxAngularVelocity; }
	
	/** \brief Set maximum angular velocity for mapping as requested by the graphic module. */
	void SetMaxAngularVelocity( float maxAngularVelocity );
	
	/** \brief Velocities have to be squared as requested by the graphic module. */
	inline bool GetSquareVelocities() const{ return pSquaredVelocities; }
	
	/** \brief Set if velocities have to be squared as requested by the graphic module. */
	void SetSquareVelocities( bool squareVelocities );
	
	
	
	/** \brief Parameter. */
	deParticleEmitterParameter &GetParameter( eParameters parameter );
	const deParticleEmitterParameter &GetParameter( eParameters parameter ) const;
	/*@}*/
};

#endif
