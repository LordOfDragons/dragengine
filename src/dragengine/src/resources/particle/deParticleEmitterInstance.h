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

#ifndef _DEPARTICLEEMITTERINSTANCE_H_
#define _DEPARTICLEEMITTERINSTANCE_H_

#include "deParticleEmitterReference.h"
#include "../deResource.h"
#include "../../common/collection/decObjectSet.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decCollisionFilter.h"
#include "../../common/utils/decLayerMask.h"

class deSkin;
class deWorld;
class deCollider;
class deCollisionInfo;
class deParticleEmitterInstanceType;
class deParticleEmitterInstanceManager;
class deParticleEmitterController;
class deBaseGraphicParticleEmitterInstance;
class deBasePhysicsParticleEmitterInstance;
class deBaseScriptingParticleEmitterInstance;


/**
 * \brief Particle Emitter Instance.
 * Instance of a particle emitter.
 */
class DE_DLL_EXPORT deParticleEmitterInstance : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deParticleEmitterInstance> Ref;
	
	
	
private:
	deParticleEmitterReference pEmitter;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	
	decDVector pReferencePosition;
	
	bool pEnableCasting;
	bool pRemoveAfterLastParticleDied;
	
	float pTimeScale;
	float pWarmUpTime;
	
	float pBurstTime;
	
	deParticleEmitterController *pControllers;
	int pControllerCount;
	int pControllerSize;
	
	deParticleEmitterInstanceType *pTypes;
	int pTypeCount;
	
	decLayerMask pLayerMask;
	decCollisionFilter pCollisionFilter;
	decObjectSet pIgnoreColliders;
	
	deBaseGraphicParticleEmitterInstance *pPeerGraphic;
	deBasePhysicsParticleEmitterInstance *pPeerPhysics;
	deBaseScriptingParticleEmitterInstance *pPeerScripting;
	
	deWorld *pParentWorld;
	deParticleEmitterInstance *pLLWorldPrev;
	deParticleEmitterInstance *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new particle emitter instance. */
	deParticleEmitterInstance( deParticleEmitterInstanceManager *manager );
	
protected:
	/**
	 * \brief Clean up particle emitter instance.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deParticleEmitterInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set emitter or NULL if none is set. */
	inline deParticleEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter or NULL to unset it. */
	void SetEmitter( deParticleEmitter *emitter );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/**
	 * \brief Reference position. Particles positions are relative to this position.
	 * 
	 * This is called by the Physics Module to tell the Graphics Module where the particles
	 * have to be rendered. If the Graphic Module does the simulation by itself setting
	 * the reference position is not required.
	 */
	inline const decDVector &GetReferencePosition() const{ return pReferencePosition; }
	
	/**
	 * \brief Set reference position. Particles positions are relative to this position.
	 * 
	 * This is called by the Physics Module to tell the Graphics Module where the particles
	 * have to be rendered. If the Graphic Module does the simulation by itself setting
	 * the reference position is not required.
	 */
	void SetReferencePosition( const decDVector &position );
	
	/** \brief Determines if casting particles is enabled. */
	inline bool GetEnableCasting() const{ return pEnableCasting; }
	
	/** \brief Sets if casting particles is enabled. */
	void SetEnableCasting( bool enable );
	
	/** \brief Emitter instance is removed from the world if the last particle died. */
	inline bool GetRemoveAfterLastParticleDied() const{ return pRemoveAfterLastParticleDied; }
	
	/** \brief Set if emitter instance is removed from the world if the last particle died. */
	void SetRemoveAfterLastParticleDied( bool remove );
	
	/** \brief Scaling for the time to be added to controllers linked to time. */
	inline float GetTimeScale() const{ return pTimeScale; }
	
	/** \brief Set scaling for the time to be added to controllers linked to time. */
	void SetTimeScale( float scale );
	
	/** \brief Warm up time used when enabling casting. */
	inline float GetWarmUpTime() const{ return pWarmUpTime; }
	
	/** \brief Set warm up time used when enabling casting. */
	void SetWarmUpTime( float warmUpTime );
	
	/** \brief Burst time. Set by the physics module to be used by the graphics module. */
	inline float GetBurstTime() const{ return pBurstTime; }
	
	/** \brief Set burst time. Set by the physics module to be used by the graphics module. */
	void SetBurstTime( float burstTime );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &collisionFilter );
	
	
	
	/** \brief Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetControllerCount().
	 */
	deParticleEmitterController &GetControllerAt( int index );
	const deParticleEmitterController &GetControllerAt( int index ) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed( const char *name ) const;
	
	/** \brief Notify peer controller changed. */
	void NotifyControllerChangedAt( int index );
	
	
	
	/** \brief Count of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** \brief Type at the given index. */
	deParticleEmitterInstanceType &GetTypeAt( int index );
	const deParticleEmitterInstanceType &GetTypeAt( int index ) const;
	
	/** \brief Notifies the peers that the type at the given index changed. */
	void NotifyTypeChangedAt( int type );
	
	/** \brief Notifies the peers that the particles in a type changed. */
	void NotifyTypeParticlesChangedAt( int type );
	
	/** \brief Reset burst particles. */
	void ResetBurst();
	
	/** \brief Kill all particles. */
	void KillAllParticles();
	
	/** \brief Notifies the Scripting Module that the last particle died. */
	void NotifyLastParticleDied();
	
	/**
	 * \brief Let scripting module determine response for a custom particle collision.
	 */
	void CollisionResponse( deCollisionInfo *cinfo );
	/*@}*/
	
	
	
	/** \name Ignore colliders */
	/*@{*/
	/**
	 * \brief Number of colliders to ignore.
	 * \version 1.7
	 */
	int GetIgnoreColliderCount() const;
	
	/**
	 * \brief Collider to ignore at index.
	 * \version 1.7
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetIgnoreColliderCount()-1.
	 */
	deCollider *GetIgnoreColliderAt( int index ) const;
	
	/**
	 * \brief Collider to ignore is present.
	 * \version 1.7
	 */
	bool HasIgnoreCollider( deCollider *collider ) const;
	
	/**
	 * \brief Add collider to ignore.
	 * \version 1.7
	 * \throws deeInvalidParam \em collider is present.
	 */
	void AddIgnoreCollider( deCollider *collider );
	
	/**
	 * \brief Remove collider to ignore.
	 * \version 1.7
	 * \throws deeInvalidParam \em collider is absent.
	 */
	void RemoveIgnoreCollider( deCollider *collider );
	
	/**
	 * \brief Remove all colliders to ignore.
	 * \version 1.7
	 */
	void RemoveAllIgnoreColliders();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object or NULL if not assigned. */
	inline deBaseGraphicParticleEmitterInstance *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object or NULL if not assigned. */
	void SetPeerGraphic( deBaseGraphicParticleEmitterInstance *peer );
	
	/** \brief Physics system peer object or NULL if not assigned. */
	inline deBasePhysicsParticleEmitterInstance *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object or NULL if not assigned. */
	void SetPeerPhysics( deBasePhysicsParticleEmitterInstance *peer );
	
	/** \brief Scripting system peer object or NULL if not assigned. */
	inline deBaseScriptingParticleEmitterInstance *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer object or NULL if not assigned. */
	void SetPeerScripting( deBaseScriptingParticleEmitterInstance *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous particle emitter in the parent world linked list. */
	inline deParticleEmitterInstance *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next particle emitter in the parent world linked list. */
	void SetLLWorldPrev( deParticleEmitterInstance *instance );
	
	/** \brief Next particle emitter in the parent world linked list. */
	inline deParticleEmitterInstance *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next particle emitter in the parent world linked list. */
	void SetLLWorldNext( deParticleEmitterInstance *instance );
	/*@}*/
	
	
	
private:
	void pUpdateControllers();
	void pUpdateTypes();
};

#endif
