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

#ifndef _DEPHYSICSSYSTEM_H_
#define _DEPHYSICSSYSTEM_H_

#include "deBaseSystem.h"

class deLoadableModule;
class deBasePhysicsModule;
class deComponent;
class deDecal;
class deModel;
class deSkin;
class deRig;
class deWorld;
class deCollider;
class deTouchSensor;
class deHeightTerrain;
class dePropField;
class deForceField;
class deParticleEmitter;
class deParticleEmitterInstance;
class deSmokeEmitter;


/**
 * \brief Physics System.
 *
 * Provides physical simulation support.
 */
class DE_DLL_EXPORT dePhysicsSystem : public deBaseSystem{
private:
	deBasePhysicsModule *pActiveModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new physics system linked to the given engine. */
	dePhysicsSystem(deEngine *engine);
	
	/** \brief Clean up physics system. */
	~dePhysicsSystem() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deBasePhysicsModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create peer object for the given component using the active module and assigns it. */
	void LoadComponent(deComponent *component);
	
	/** \brief Create peer object for the given model using the active module and assigns it. */
	void LoadModel(deModel *model);
	
	/** \brief Create peer object for the given skin using the active module and assigns it. */
	void LoadSkin(deSkin *skin);
	
	/** \brief Create peer object for the given rig using the active module and assigns it. */
	void LoadRig(deRig *rig);
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld(deWorld *world);
	
	/** \brief Create peer object for the given collider using the active module and assigns it. */
	void LoadCollider(deCollider *collider);
	
	/** \brief Create peer object for the given decal using the active module and assigns it. */
	void LoadDecal(deDecal *decal);
	
	/** \brief Create peer object for the given touch sensor using the active module and assigns it. */
	void LoadTouchSensor(deTouchSensor *touchSensor);
	
	/** \brief Create peer object for the given height terrain using the active module and assigns it. */
	void LoadHeightTerrain(deHeightTerrain *heightTerrain);
	
	/** \brief Create peer object for the given prop field using the active module and assigns it. */
	void LoadPropField(dePropField *propField);
	
	/** \brief Create peer object for the given force field using the active module and assigns it. */
	void LoadForceField(deForceField *forceField);
	
	/** \brief Create peer for the particle emitter using the active module and assigns it. */
	void LoadParticleEmitter(deParticleEmitter *emitter);
	
	/** \brief Create peer for the particle emitter instance using the active module and assigns it. */
	void LoadParticleEmitterInstance(deParticleEmitterInstance *instance);
	
	/** \brief Create peer for the smoke emitter using the active module and assigns it. */
	void LoadSmokeEmitter(deSmokeEmitter *smokeEmitter);
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	void SetActiveModule(deLoadableModule *module) override;
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	void ClearPermanents() override;
	
	/** \brief Carry out here actions right after the system started up. */
	void PostStart() override;
	
	/** \brief Carry out here actions right before the system shuts down. */
	void PreStop() override;
	/*@}*/
};

#endif
