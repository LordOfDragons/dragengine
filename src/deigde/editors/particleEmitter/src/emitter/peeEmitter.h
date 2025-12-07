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

#ifndef _PEEEMITTER_H_
#define _PEEEMITTER_H_

#include <deigde/editableentity/igdeEditableEntity.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/math/decMath.h>

#include "peeParameter.h"
#include "peeControllerList.h"
#include "peeTypeList.h"
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>

class peeEmitterListener;
class peeLoadSaveSystem;

class igdeWSky;

class deSkin;
class deComponent;
class igdeCamera;
class deLogger;



/**
 * \brief Editable Emitter.
 */
class peeEmitter : public igdeEditableEntity{
public:
	/** Layer masks for colliders. */
	enum eColliderLayerMasks{
		/** Editing. */
		eclmEditing,
		/** Prop Fields. */
		eclmPropFields,
		/** Height Terrains. */
		eclmHeightTerrains,
		/** Objects. */
		eclmObjects,
		/** Decals. */
		eclmDecals,
		/** Wind. */
		eclmWind,
		/** Number of layer mask bits. */
		ECLM_COUNT
	};
	
private:
	peeLoadSaveSystem &pLoadSaveSystem;
	
	deWorld::Ref pEngWorld;
	
	deDebugDrawer::Ref pDDEmitter;
	
	igdeWSky *pSky;
	igdeWObject::Ref pEnvObject;
	
	igdeWCoordSysArrows pDDSEmitter;
	
	deParticleEmitter::Ref pEngEmitter;
	deParticleEmitterInstance::Ref pEngEmitterInstance;
	
	decVector pPosition;
	decVector pOrientation;
	
	float pBurstLifetime;
	bool pEmitBurst;
	bool pEnableCasting;
	
	float pWarmUpTime;
	float pBurstInterval;
	float pBurstTimer;
	
	igdeCamera *pCamera;
	
	peeControllerList pControllerList;
	peeController::Ref pActiveController;
	
	peeTypeList pTypeList;
	peeType::Ref pActiveType;
	
	decObjectSet pListeners;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new emitter. */
	peeEmitter(igdeEnvironment *environment, peeLoadSaveSystem &loadSaveSystem);
	/** \brief Clean up the emitter. */
	virtual ~peeEmitter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Load save system. */
	inline peeLoadSaveSystem &GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	
	/** Retrieves the engine world. */
	inline const deWorld::Ref &GetEngineWorld() const{ return pEngWorld; }
	/** Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the environment wrapper object. */
	inline const igdeWObject::Ref &GetEnvObject() const{ return pEnvObject; }
	
	/** Retrieves the engine emitter. */
	inline const deParticleEmitter::Ref &GetEngineEmitter() const{ return pEngEmitter; }
	/** Retrieves the engine emitter instance. */
	inline const deParticleEmitterInstance::Ref &GetEngineEmitterInstance() const{ return pEngEmitterInstance; }
	
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decVector &position);
	/** Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation. */
	void SetOrientation(const decVector &orientation);
	
	/** Retrieves the warm up time. */
	inline float GetWarmUpTime() const{ return pWarmUpTime; }
	/** Sets the warm up time. */
	void SetWarmUpTime(float warmUpTime);
	/** Retrieves the burst lifetime. */
	inline float GetBurstLifetime() const{ return pBurstLifetime; }
	/** Sets the burst lifetime. */
	void SetBurstLifetime(float lifetime);
	/** Determines if particles are emit as burst or continuous. */
	inline bool GetEmitBurst() const{ return pEmitBurst; }
	/** Sets if particles are emit as burst (using particle count curve) or continuous (using interval curve). */
	void SetEmitBurst(bool emitBurst);
	/** Determines if casting is enabled. */
	inline bool GetEnableCasting() const{ return pEnableCasting; }
	/** Sets if casting is enabled. */
	void SetEnableCasting(bool enableCasting);
	
	/** Retrieves the burst interval in seconds. */
	inline float GetBurstInterval() const{ return pBurstInterval; }
	/** Sets the burst interval in seconds. */
	void SetBurstInterval(float interval);
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the emitter. */
	void Update(float elapsed);
	/** Resets the emitter. */
	void Reset();
	
	/** Free emitter. */
	void FreeEmitter();
	/** Rebuild emitter. */
	void RebuildEmitter();
	/*@}*/
	
	/** \name Controllers */
	/*@{*/
	/** Retrieves the controller list read-only. */
	inline const peeControllerList &GetControllers() const{ return pControllerList; }
	/** Adds a new controller. */
	void AddController(peeController *controller);
	/** Inserts a new controller. */
	void InsertControllerAt(peeController *controller, int index);
	/** Moves a controller to a new position. */
	void MoveControllerTo(peeController *controller, int index);
	/** Removes a controller. */
	void RemoveController(peeController *controller);
	/** Removes all controllers. */
	void RemoveAllControllers();
	/** Retrieves the active texture or NULL if none is active. */
	inline const peeController::Ref &GetActiveController() const{ return pActiveController; }
	/** Determines if there is an active controller or not. */
	bool HasActiveController() const;
	/** Sets the active controller or NULL if none is active. */
	void SetActiveController(peeController *controller);
	/*@}*/
	
	/** \name Types */
	/*@{*/
	/** Retrieves the type list read-only. */
	inline const peeTypeList &GetTypeList() const{ return pTypeList; }
	/** Adds a new type. */
	void AddType(peeType *type);
	/** Inserts a new type. */
	void InsertTypeAt(peeType *type, int index);
	/** Moves a type to a new position. */
	void MoveTypeTo(peeType *type, int index);
	/** Removes a type. */
	void RemoveType(peeType *type);
	/** Removes all types. */
	void RemoveAllTypes();
	/** Retrieves the active type or NULL if none is active. */
	inline const peeType::Ref &GetActiveType() const{ return pActiveType; }
	/** Determines if there is an active type or not. */
	bool HasActiveType() const;
	/** Sets the active type or NULL if none is active. */
	void SetActiveType(peeType *type);
	/*@}*/
	
	/** \name Notifiers */
	/*@{*/
	/** Adds a listener. */
	void AddListener(peeEmitterListener *listener);
	/** Removes a listener. */
	void RemoveListener(peeEmitterListener *listener);
	
	/** Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** Notifies all that a emitter changed. */
	void NotifyEmitterChanged();
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** Notifies all that the view changed. */
	void NotifyViewChanged();
	/** Notifies all that the camera changed. */
	void NotifyCameraChanged();
	
	/** \brief Notify controller added or removed. */
	void NotifyControllerStructureChanged();
	
	/** \brief Notify controller changed. */
	void NotifyControllerChanged(peeController *controller);
	
	/** \brief Notify controller name changed. */
	void NotifyControllerNameChanged(peeController *controller);
	
	/** \brief Notify controller value changed. */
	void NotifyControllerValueChanged(peeController *controller);
	
	/** \brief Active controller changed. */
	void NotifyActiveControllerChanged();
	
	/** Notifies all that the type count or order changed. */
	void NotifyTypeStructureChanged();
	/** Notifies all that a type changed. */
	void NotifyTypeChanged(peeType *type);
	/** Notifies all that the active type trail controller changed. */
	void NotifyTypeActiveTrailControllerChanged(peeType *type);
	/** Notifies all that the active type emit controller changed. */
	void NotifyTypeActiveEmitControllerChanged(peeType *type);
	/** Notifies all that a type parameter changed. */
	void NotifyTypeParameterChanged(peeType *type, peeParameter *parameter);
	/** Notifies all that the active type parameter changed. */
	void NotifyActiveTypeParameterChanged(peeType *type);
	/** Notifies all that the active type changed. */
	void NotifyActiveTypeChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
