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

#ifndef _DEBASEPHYSICSMODULE_H_
#define _DEBASEPHYSICSMODULE_H_

#include "../deBaseModule.h"

class deBasePhysicsComponent;
class deBasePhysicsModel;
class deBasePhysicsSkin;
class deBasePhysicsRig;
class deBasePhysicsWorld;
class deBasePhysicsCollider;
class deBasePhysicsDecal;
class deBasePhysicsTouchSensor;
class deBasePhysicsHeightTerrain;
class deBasePhysicsPropField;
class deBasePhysicsForceField;
class deBasePhysicsParticleEmitter;
class deBasePhysicsParticleEmitterInstance;
class deBasePhysicsSmokeEmitter;
class deModel;
class deSkin;
class deRig;
class deWorld;
class deDecal;
class deComponent;
class deCollider;
class deTouchSensor;
class deHeightTerrain;
class dePropField;
class deForceField;
class deParticleEmitter;
class deParticleEmitterInstance;
class deSmokeEmitter;


/**
 * \brief Base Physics Module.
 * 
 * The physics module provides physical simulations on colliders.
 */
class DE_DLL_EXPORT deBasePhysicsModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module. */
	deBasePhysicsModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~deBasePhysicsModule() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize physics module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down physics module and cleans up. */
	virtual void CleanUp() = 0;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for the given component object. */
	virtual deBasePhysicsComponent *CreateComponent(deComponent *comp) = 0;
	
	/** \brief Create peer for the given model object. */
	virtual deBasePhysicsModel *CreateModel(deModel *model) = 0;
	
	/** \brief Create peer for the given skin object. */
	virtual deBasePhysicsSkin *CreateSkin(deSkin *skin) = 0;
	
	/** \brief Create peer for the given rig object. */
	virtual deBasePhysicsRig *CreateRig(deRig *rig) = 0;
	
	/** \brief Create peer for the given world object. */
	virtual deBasePhysicsWorld *CreateWorld(deWorld *world) = 0;
	
	/** \brief Create peer for the given collider. */
	virtual deBasePhysicsCollider *CreateCollider(deCollider *collider) = 0;
	
	/** \brief Create peer for the given decal. */
	virtual deBasePhysicsDecal *CreateDecal(deDecal *decal) = 0;
	
	/** \brief Create peer for the given touch sensor. */
	virtual deBasePhysicsTouchSensor *CreateTouchSensor(deTouchSensor *touchSensor) = 0;
	
	/** \brief Create peer for the given height terrain. */
	virtual deBasePhysicsHeightTerrain *CreateHeightTerrain(deHeightTerrain *heightTerrain) = 0;
	
	/** \brief Create peer for the given prop field. */
	virtual deBasePhysicsPropField *CreatePropField(dePropField *propField) = 0;
	
	/** \brief Create peer for the given force field. */
	virtual deBasePhysicsForceField *CreateForceField(deForceField *forceField) = 0;
	
	/** \brief Create peer for the given particle emitter. */
	virtual deBasePhysicsParticleEmitter *CreateParticleEmitter(deParticleEmitter *emitter) = 0;
	
	/** \brief Create peer for the given particle emitter instance. */
	virtual deBasePhysicsParticleEmitterInstance *CreateParticleEmitterInstance(
		deParticleEmitterInstance *instance) = 0;
	
	/** \brief Create peer for the given smoke emitter. */
	virtual deBasePhysicsSmokeEmitter *CreateSmokeEmitter(deSmokeEmitter *smokeEmitter) = 0;
	/*@}*/
};

#endif
