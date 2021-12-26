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

#ifndef _DEBASEPHASICSWORLD_H_
#define _DEBASEPHASICSWORLD_H_

#include "../../../common/math/decMath.h"

class decCollisionFilter;
class decCollisionVolume;
class decCollisionSphere;
class deCollisionInfo;
class deCollider;
class deBaseScriptingCollider;
class deComponent;
class deTouchSensor;
class dePropField;
class deForceField;
class deParticleEmitterInstance;
class deSmokeEmitter;


/**
 * \brief Physics Module World Peer.
 
 * Worlds are used by the physics module to provide collision detection
 * and physical simulations against the contained scenes.
 */
class DE_DLL_EXPORT deBasePhysicsWorld{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsWorld();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Update dynamic parts of scene.
	 * \param elapsed Seconds elapsed since last update
	 */
	virtual void Update( float elapsed );
	
	/**
	 * \brief Process physics simulation using the physics module.
	 * 
	 * Apply collision detection on moving kinematic collider, physical simulation
	 * on dynamic colliders, particle and prop field simulations as well as moving
	 * colliders attached to other colliders.
	 */
	virtual void ProcessPhysics( float elapsed );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/** \brief Physics properties changed. */
	virtual void PhysicsChanged();
	
	/** \brief Height Terrain changed. */
	virtual void HeightTerrainChanged();
	
	/** \brief Component has been added. */
	virtual void ComponentAdded( deComponent *component );
	
	/** \brief Component has been removed. */
	virtual void ComponentRemoved( deComponent *component );
	
	/** \brief All components have been removed. */
	virtual void AllComponentsRemoved();
	
	/** \brief Collider has been added. */
	virtual void ColliderAdded( deCollider *collider );
	
	/** \brief Collider has been removed. */
	virtual void ColliderRemoved( deCollider *collider );
	
	/** \brief All colliders have been removed. */
	virtual void AllCollidersRemoved();
	
	/** \brief Touch sensor has been added. */
	virtual void TouchSensorAdded( deTouchSensor *touchSensor );
	
	/** \brief Touch sensor has been removed. */
	virtual void TouchSensorRemoved( deTouchSensor *touchSensor );
	
	/** \brief All touch sensors have been removed. */
	virtual void AllTouchSensorsRemoved();
	
	/** \brief Prop field has been added. */
	virtual void PropFieldAdded( dePropField *propField );
	
	/** \brief Prop field has been removed. */
	virtual void PropFieldRemoved( dePropField *propField );
	
	/** \brief All prop fields have been removed. */
	virtual void AllPropFieldsRemoved();
	
	/** \brief Force field has been added. */
	virtual void ForceFieldAdded( deForceField *forceField );
	
	/** \brief Force field has been removed. */
	virtual void ForceFieldRemoved( deForceField *forceField );
	
	/** \brief All force fields have been removed. */
	virtual void AllForceFieldsRemoved();
	
	/** \brief Prop field has been added. */
	virtual void ParticleEmitterAdded( deParticleEmitterInstance *emitter );
	
	/** \brief Prop field has been removed. */
	virtual void ParticleEmitterRemoved( deParticleEmitterInstance *emitter );
	
	/** \brief All prop fields have been removed. */
	virtual void AllParticleEmittersRemoved();
	
	/** \brief Prop field has been added. */
	virtual void SmokeEmitterAdded( deSmokeEmitter *smokeEmitter );
	
	/** \brief Prop field has been removed. */
	virtual void SmokeEmitterRemoved( deSmokeEmitter *smokeEmitter );
	
	/** \brief All prop fields have been removed. */
	virtual void AllSmokeEmittersRemoved();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * \brief Test a point for collision with colliders.
	 * 
	 * For each collision the collisionResponse function the given listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void PointHits( const decDVector &point, deBaseScriptingCollider *listener,
		const decCollisionFilter &collisionFilter );
	
	/**
	 * \brief Tests a ray for collision with the element in the world.
	 * 
	 * For each collision the collisionResponse function the given listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
		deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter );
	
	/**
	 * \brief Tests the collider for collision with world elements.
	 * 
	 * For each collision the collisionResponse function of the listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void ColliderHits( deCollider *collider, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Tests the moving collider for collision with world elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function
	 * of the listener is called. To stop testing set StopTesting in the provided
	 * collision information object to true.
	 */
	virtual void ColliderMoveHits( deCollider *collider, const decVector &displacement,
		deBaseScriptingCollider *listener );
	
	/**
	 * \brief Tests the rotating collider for collision with world elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function
	 * of the listener is called. To stop testing set StopTesting in the provided
	 * collision information object to true.
	 */
	virtual void ColliderRotateHits( deCollider *collider, const decVector &rotation,
		deBaseScriptingCollider *listener );
	
	/**
	 * \brief Tests the moving and rotating collider for collision with world elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function
	 * of the listener is called. To stop testing set StopTesting in the provided
	 * collision information object to true.
	 */
	virtual void ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
		const decVector &rotation, deBaseScriptingCollider *listener );
	/*@}*/
	
};

#endif
