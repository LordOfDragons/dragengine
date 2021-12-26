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

#ifndef _DEPARTICLEEMITTER_H_
#define _DEPARTICLEEMITTER_H_

#include "deParticleEmitterParameter.h"
#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/curve/decCurveBezier.h"

class deParticleEmitterManager;
class deParticleEmitterController;
class deParticleEmitterType;

class deBaseGraphicParticleEmitter;
class deBasePhysicsParticleEmitter;


/**
 * \brief Particle Emitter.
 * 
 * Emits particles in the form of billboards. The position and
 * orientation of the particle emitter defines where particles are
 * produced and in which direction they are cast. The cast parameters
 * are picked randomly between the minimum and maximum value. The state
 * of particles is stored individually by the physics module. The
 * Physics module has to provide the graphics related particle states
 * for the Graphics module in addition to any internal states required
 * for the simulation. This shared information has to be provided using
 * an array of sParticle elements. The position of the particles are
 * relative to the emitter position. The Scripting Module will be notified
 * by the Physics Module about collisions of particles with world.
 * 
 * \par Intermodule Communication
 * The Physics and Graphic module require communication dealing with
 * particles to make them fast. This communication breaks up into different
 * stages. First the Graphic Module stores in the particle emitter how the
 * particle states have to be stored. This is done using the graphic module
 * request parameters. Using these the Physics Module can provide the states
 * in the favorable way. Once the Physics Module has done a simulation step
 * it creates or updates a particle array. This array is then stored in the
 * particle emitter. The array stays valid until the next time the Physics
 * Module simulates the particles. In the mean time the Graphic Module can
 * use the particle array stored in the emitter to render the particles.
 * 
 * \par Graphic Module Requests
 * The Graphic Module can place a couple of requests in the particle emitter
 * for the Physics Module to respect. The data in the particles array are
 * stored in compressed form using mostly bytes for individual parameters
 * where possible. This not only reduces the amount of memory required and
 * transfered it also allows Graphic Modules to place particle data in
 * textures. Graphic Modules have in general two possible ways to handle a
 * large amount of particles and this is either by storing the data inside
 * an input-stream type GPU memory or inside textures. This system allows
 * the Graphic Module to drive particle rendering either way without the
 * Physics Module requiring to know. Due to the compressed nature though
 * limits are required to define a proper mapping. The linear velocity is
 * split into four values three defining the direction and the fourthed
 * defining the magnitude of the velocity. The direction is stored stored
 * with each component as bytes mapping -1 to char -127 and 1 to char 127.
 * The magnitude maps 0 m/s to byte 0 and the maximum linear velocity to
 * byte 255. The maximum velocity is set using SetMaxLinearVelocity and
 * defaults to 1 m/s. The rotation maps 0° to byte 0 and 360° to byte 255.
 * This gives a step of roughly 1.4° per byte which is not a big problem
 * for particles as they are in general small objects. A problem would arise
 * with large particles like smoke but for these objects exists resources
 * on their own simulating them better than particles can do. The angular
 * velocity maps -maxAngularVelocity to char -127 and maxAngularVelocity to
 * char 127. The maximum angular velocity can be set using
 * SetMaxAngularVelocity. For the velocities exists an additional option to
 * store them squared instead of their true value. Using squared velocities
 * a finer grained step size is used for velocities close to 0 while larger
 * velocities have a coarser grained step size. To request squared velocities
 * use SetSquareVelocities. The cast time parameters map the lower cast
 * value to char -127 and the upper cast value to char 127. No additional
 * limits are required since these cast value boundaries provide already a
 * mapping. If the upper and lower cast value is the same or nearly the same
 * any value can be used as all values map to the same cast parameter value.
 * Typically char 0 is though used. In addition to these request parameters
 * the Graphic Module can request to drive particle simulation on its own
 * instead of handing this off to the Physics Module. This can be done using
 * SetGraphicModuleSimulates. If set the Graphic Module is responsible to
 * drive the entire simulation. The Physics Module is going to ignore this
 * particle emitter altogether. This is useful for particle emitters not
 * affected by physics like for example spell casting particle effects.
 * For these simplified simulations can be used which often can be driven
 * entirely on the GPU.
 */
class DE_DLL_EXPORT deParticleEmitter : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deParticleEmitter> Ref;
	
	
	
private:
	deParticleEmitterController **pControllers;
	int pControllerCount;
	int pControllerSize;
	
	deParticleEmitterType *pTypes;
	int pTypeCount;
	
	bool pEmitBurst;
	float pBurstLifetime;
	
	bool pGraphicModuleSimlates;
	
	deBaseGraphicParticleEmitter *pPeerGraphic;
	deBasePhysicsParticleEmitter *pPeerPhysics;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new particle emitter. */
	deParticleEmitter( deParticleEmitterManager *manager );
	
protected:
	/**
	 * \brief Clean up particle emitter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Burst lifetime of the emitter. */
	inline float GetBurstLifetime() const{ return pBurstLifetime; }
	
	/** \brief Set burst lifetime of the emitter. */
	void SetBurstLifetime( float lifetime );
	
	/** \brief Particles are emit as burst (particle count curve) or continuous (interval curve). */
	inline bool GetEmitBurst() const{ return pEmitBurst; }
	
	/** \brief Set if particles are emit as burst (particle count curve) or continuous (interval curve). */
	void SetEmitBurst( bool emitBurst );
	
	/** \brief Count of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** \brief Set number of types. */
	void SetTypeCount( int count );
	
	/** \brief Type at the given index. */
	deParticleEmitterType &GetTypeAt( int index );
	const deParticleEmitterType &GetTypeAt( int index ) const;
	
	/** \brief Notifies that the type at the given index changed. */
	void NotifyTypeChangedAt( int type );
	
	/** \brief Graphic module takes care of simulation. */
	inline bool GetGraphicModuleSimulates() const{ return pGraphicModuleSimlates; }
	
	/** \brief Set if graphic module takes care of simulation. */
	void SetGraphicModuleSimulates( bool graphicModuleSimulates );
	/*@}*/
	
	
	
	/** \name Controller Management */
	/*@{*/
	/** \brief Count of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/** \brief Controller at index. */
	deParticleEmitterController *GetControllerAt( int index ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOfController( deParticleEmitterController *controller ) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed( const char *controller ) const;
	
	/** \brief Controller is present. */
	bool HasController( deParticleEmitterController *controller ) const;
	
	/** \brief Add controller. */
	void AddController( deParticleEmitterController *controller );
	
	/** \brief Remove controller. */
	void RemoveController( deParticleEmitterController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Notify peers controller changed. */
	void NotifyControllerChangedAt( int index );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object or NULL if not assigned. */
	inline deBaseGraphicParticleEmitter *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object or NULL if not assigned. */
	void SetPeerGraphic( deBaseGraphicParticleEmitter *peer );
	
	/** \brief Physics system peer object or NULL if not assigned. */
	inline deBasePhysicsParticleEmitter *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object or NULL if not assigned. */
	void SetPeerPhysics( deBasePhysicsParticleEmitter *peer );
	/*@}*/
};

#endif
