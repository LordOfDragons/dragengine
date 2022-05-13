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

#ifndef _DEWORLD_H_
#define _DEWORLD_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"

class decCollisionFilter;
class deBaseScriptingCollider;
class deWorldManager;

class deBillboard;
class deCamera;
class deCollider;
class deComponent;
class deDebugDrawer;
class deEnvMapProbe;
class deForceField;
class deHeightTerrain;
class deLight;
class deLumimeter;
class deMicrophone;
class deNavigationBlocker;
class deNavigationSpace;
class deNavigator;
class deNetworkState;
class deParticleEmitterInstance;
class dePropField;
class deSmokeEmitter;
class deSpeaker;
class deSoundLevelMeter;
class deTouchSensor;
class deSkyInstance;

class deBaseGraphicWorld;
class deBasePhysicsWorld;
class deBaseAudioWorld;
class deBaseNetworkWorld;
class deBaseAIWorld;


/**
 * \brief Renderable World Class.
 *
 * Worlds are the top most class for rendering 3d worldries. They bundle a
 * set of World objects and a Sky object. Besides rendering they also are
 * able to do collision detection and physical responses to them.
 */
class deWorld : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deWorld> Ref;
	
	
	
private:
	deHeightTerrain *pHeightTerrain;
	decDVector pSize;
	
	decVector pGravity;
	
	decColor pAmbient;
	bool pDisableLights;
	decMatrix pLightColorMatrix;
	
	deSkyInstance *pSkyRoot;
	deSkyInstance *pSkyTail;
	int pSkyCount;
	
	deBillboard *pBillboardRoot;
	deBillboard *pBillboardTail;
	int pBillboardCount;
	
	deCamera *pCameraRoot;
	deCamera *pCameraTail;
	int pCameraCount;
	
	deCollider *pColliderRoot;
	deCollider *pColliderTail;
	int pColliderCount;
	
	deComponent *pComponentRoot;
	deComponent *pComponentTail;
	int pComponentCount;
	
	deDebugDrawer *pDebugDrawerRoot;
	deDebugDrawer *pDebugDrawerTail;
	int pDebugDrawerCount;
	
	deEnvMapProbe *pEnvMapProbeRoot;
	deEnvMapProbe *pEnvMapProbeTail;
	int pEnvMapProbeCount;
	
	deForceField *pForceFieldRoot;
	deForceField *pForceFieldTail;
	int pForceFieldCount;
	
	deLight *pLightRoot;
	deLight *pLightTail;
	int pLightCount;
	
	deLumimeter *pLumimeterRoot;
	deLumimeter *pLumimeterTail;
	int pLumimeterCount;
	
	deMicrophone *pMicrophoneRoot;
	deMicrophone *pMicrophoneTail;
	int pMicrophoneCount;
	
	deNavigationSpace *pNavSpaceRoot;
	deNavigationSpace *pNavSpaceTail;
	int pNavSpaceCount;
	
	deNavigationBlocker *pNavBlockerRoot;
	deNavigationBlocker *pNavBlockerTail;
	int pNavBlockerCount;
	
	deNavigator *pNavigatorRoot;
	deNavigator *pNavigatorTail;
	int pNavigatorCount;
	
	deNetworkState *pNetworkStateRoot;
	deNetworkState *pNetworkStateTail;
	int pNetworkStateCount;
	
	deParticleEmitterInstance *pParticleEmitterRoot;
	deParticleEmitterInstance *pParticleEmitterTail;
	int pParticleEmitterCount;
	
	dePropField *pPropFieldRoot;
	dePropField *pPropFieldTail;
	int pPropFieldCount;
	
	deSmokeEmitter *pSmokeEmitterRoot;
	deSmokeEmitter *pSmokeEmitterTail;
	int pSmokeEmitterCount;
	
	deSpeaker *pSpeakerRoot;
	deSpeaker *pSpeakerTail;
	int pSpeakerCount;
	
	deTouchSensor *pTouchSensorRoot;
	deTouchSensor *pTouchSensorTail;
	int pTouchSensorCount;
	
	deSoundLevelMeter *pSoundLevelMeterRoot;
	deSoundLevelMeter *pSoundLevelMeterTail;
	int pSoundLevelMeterCount;
	
	// peers
	deBaseGraphicWorld *pPeerGraphic;
	deBasePhysicsWorld *pPeerPhysics;
	deBaseAudioWorld *pPeerAudio;
	deBaseNetworkWorld *pPeerNetwork;
	deBaseAIWorld *pPeerAI;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new world object with the given resource manager. */
	deWorld( deWorldManager *manager );
	
protected:
	/**
	 * \brief Clean up world.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deWorld();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Removes all objects from the world. */
	void Clear();
	
	/** \brief Assigned height terrain or NULL. */
	inline deHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Set assigned height terrain or NULL to remove it. */
	void SetHeightTerrain( deHeightTerrain *heightTerrain );
	
	/** \brief Size of the world. */
	inline const decDVector &GetSize() const{ return pSize; }
	
	/** \brief Set size of the world. */
	void SetSize( const decDVector &size );
	
	/** \brief World gravity vector. */
	inline const decVector &GetGravity() const{ return pGravity; }
	
	/** \brief Set world gravity vector. */
	void SetGravity( const decVector &gravity );
	
	/** \brief Determines if all lights are disables. */
	inline bool GetDisableLights() const{ return pDisableLights; }
	
	/** \brief Sets if all lights are disabled. */
	void SetDisableLights( bool disable );
	
	/** \brief Ambient light color. */
	inline const decColor &GetAmbientLight() const{ return pAmbient; }
	
	/** \brief Set ambient light color. */
	void SetAmbientLight( const decColor &color );
	
	/** \brief Light color matrix. */
	inline const decMatrix &GetLightColorMatrix() const{ return pLightColorMatrix; }
	
	/** \brief Set light color matrix. */
	void SetLightColorMatrix( const decMatrix &matrix );
	
	/**
	 * \brief Update dynamic world parts.
	 * 
	 * This includes sending a notification to the graphic system peer to update internal
	 * data structures. Objects beeing part of the world are not updated with this call.
	 * This is to avoid slowing the engine down if a huge amount of
	 * objects are in the world.
	 * 
	 * \param elapsed Seconds elapsed since the last update
	 */
	void Update( float elapsed );
	
	/**
	 * \brief Process physics simulation using the physics module.
	 * 
	 * Apply collision detection on moving kinematic collider, physical simulation
	 * on dynamic colliders, particle and prop field simulations as well as moving
	 * colliders attached to other colliders.
	 */
	void ProcessPhysics( float elapsed );
	/*@}*/
	
	
	
	/** \name Skies */
	/*@{*/
	/** \brief Count of skies. */
	inline int GetSkyCount() const{ return pSkyCount; }
	
	/** \brief Root sky or NULL if there are none. */
	inline deSkyInstance *GetRootSky() const{ return pSkyRoot; }
	
	/**
	 * \brief Add sky.
	 * \throws deeInvalidParam \em sky is NULL.
	 * \throws deeInvalidParam \em sky has a parent world.
	 */
	void AddSky( deSkyInstance *sky );
	
	/**
	 * \brief Remove sky.
	 * \throws deeInvalidParam \em sky is NULL.
	 * \throws deeInvalidParam Parent world of \em sky is not this world.
	 */
	void RemoveSky( deSkyInstance *sky );
	
	/** \brief Remove all skies. */
	void RemoveAllSkies();
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** \brief Count of billboards. */
	inline int GetBillboardCount() const{ return pBillboardCount; }
	
	/** \brief Root billboard or NULL if there are none. */
	inline deBillboard *GetRootBillboard() const{ return pBillboardRoot; }
	
	/**
	 * \brief Add billboard.
	 * \throws deeInvalidParam \em billboard is NULL.
	 * \throws deeInvalidParam \em billboard has a parent world.
	 */
	void AddBillboard( deBillboard *billboard );
	
	/**
	 * \brief Remove billboard.
	 * \throws deeInvalidParam \em billboard is NULL.
	 * \throws deeInvalidParam Parent world of \em billboard is not this world.
	 */
	void RemoveBillboard( deBillboard *billboard );
	
	/** \brief Remove all billboards. */
	void RemoveAllBillboards();
	/*@}*/
	
	
	
	/** \name Cameras */
	/*@{*/
	/** \brief Count of cameras. */
	inline int GetCameraCount() const{ return pCameraCount; }
	
	/** \brief Root camera or NULL if there are none. */
	inline deCamera *GetRootCamera() const{ return pCameraRoot; }
	
	/**
	 * \brief Add camera.
	 * \throws deeInvalidParam \em camera is NULL.
	 * \throws deeInvalidParam \em camera has a parent world.
	 */
	void AddCamera( deCamera *camera );
	
	/**
	 * \brief Remove camera.
	 * \throws deeInvalidParam \em camera is NULL.
	 * \throws deeInvalidParam Parent world of \em camera is not this world.
	 */
	void RemoveCamera( deCamera *camera );
	
	/** \brief Remove all cameras. */
	void RemoveAllCameras();
	/*@}*/
	
	
	
	/** \name Colliders */
	/*@{*/
	/** \brief Count of colliders. */
	inline int GetColliderCount() const{ return pColliderCount; }
	
	/** \brief Root collider or NULL if there are none. */
	inline deCollider *GetRootCollider() const{ return pColliderRoot; }
	
	/**
	 * \brief Add collider.
	 * \throws deeInvalidParam \em collider is NULL.
	 * \throws deeInvalidParam \em collider has a parent world.
	 */
	void AddCollider( deCollider *collider );
	
	/**
	 * \brief Remove collider.
	 * \throws deeInvalidParam \em collider is NULL.
	 * \throws deeInvalidParam Parent world of \em collider is not this world.
	 */
	void RemoveCollider( deCollider *collider );
	
	/** \brief Remove all colliders. */
	void RemoveAllColliders();
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Root component or NULL if there are none. */
	inline deComponent *GetRootComponent() const{ return pComponentRoot; }
	
	/**
	 * \brief Add component.
	 * \throws deeInvalidParam \em component is NULL.
	 * \throws deeInvalidParam \em component has a parent world.
	 */
	void AddComponent( deComponent *component );
	
	/**
	 * \brief Remove component.
	 * \throws deeInvalidParam \em component is NULL.
	 * \throws deeInvalidParam Parent world of \em component is not this world.
	 */
	void RemoveComponent( deComponent *component );
	
	/** \brief Remove all components. */
	void RemoveAllComponents();
	/*@}*/
	
	
	
	/** \name Debug drawers */
	/*@{*/
	/** \brief Count of debug drawers. */
	inline int GetDebugDrawerCount() const{ return pDebugDrawerCount; }
	
	/** \brief Root debug drawer or NULL if there are none. */
	inline deDebugDrawer *GetRootDebugDrawer() const{ return pDebugDrawerRoot; }
	
	/**
	 * \brief Add debugDrawer.
	 * \throws deeInvalidParam \em debugDrawer is NULL.
	 * \throws deeInvalidParam \em debugDrawer has a parent world.
	 */
	void AddDebugDrawer( deDebugDrawer *debugDrawer );
	
	/**
	 * \brief Remove debugDrawer.
	 * \throws deeInvalidParam \em debugDrawer is NULL.
	 * \throws deeInvalidParam Parent world of \em debugDrawer is not this world.
	 */
	void RemoveDebugDrawer( deDebugDrawer *debugDrawer );
	
	/** \brief Remove all debug drawers. */
	void RemoveAllDebugDrawers();
	/*@}*/
	
	
	
	/** \name Environment map probes. */
	/*@{*/
	/** \brief Count of environment map probes. */
	inline int GetEnvMapProbeCount() const{ return pEnvMapProbeCount; }
	
	/** \brief Root environment map probe or NULL if there are none. */
	inline deEnvMapProbe *GetRootEnvMapProbe() const{ return pEnvMapProbeRoot; }
	
	/**
	 * \brief Add environment map probe.
	 * \throws deeInvalidParam \em envMapProbe is NULL.
	 * \throws deeInvalidParam \em envMapProbe has a parent world.
	 */
	void AddEnvMapProbe( deEnvMapProbe *envMapProbe );
	
	/**
	 * \brief Remove environment map probe.
	 * \throws deeInvalidParam \em envMapProbe is NULL.
	 * \throws deeInvalidParam Parent world of \em envMapProbe is not this world.
	 */
	void RemoveEnvMapProbe( deEnvMapProbe *envMapProbe );
	
	/** \brief Remove all environment map probes. */
	void RemoveAllEnvMapProbes();
	/*@}*/
	
	
	
	/** \name Force fields */
	/*@{*/
	/** \brief Count of force fields. */
	inline int GetForceFieldCount() const{ return pForceFieldCount; }
	
	/** \brief Root force field or NULL if there are none. */
	inline deForceField *GetRootForceField() const{ return pForceFieldRoot; }
	
	/**
	 * \brief Add force field.
	 * \throws deeInvalidParam \em forceField is NULL.
	 * \throws deeInvalidParam \em forceField has a parent world.
	 */
	void AddForceField( deForceField *forceField );
	
	/**
	 * \brief Remove force field.
	 * \throws deeInvalidParam \em forceField is NULL.
	 * \throws deeInvalidParam Parent world of \em forceField is not this world.
	 */
	void RemoveForceField( deForceField *forceField );
	
	/** \brief Remove all force fields. */
	void RemoveAllForceFields();
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** \brief Count of lights. */
	inline int GetLightCount() const{ return pLightCount; }
	
	/** \brief Root light or NULL if there are none. */
	inline deLight *GetRootLight() const{ return pLightRoot; }
	
	/**
	 * \brief Add light.
	 * \throws deeInvalidParam \em light is NULL.
	 * \throws deeInvalidParam \em light has a parent world.
	 */
	void AddLight( deLight *light );
	
	/**
	 * \brief Remove light.
	 * \throws deeInvalidParam \em light is NULL.
	 * \throws deeInvalidParam Parent world of \em light is not this world.
	 */
	void RemoveLight( deLight *light );
	
	/** \brief Remove all lights. */
	void RemoveAllLights();
	/*@}*/
	
	
	
	/** \name Lumimeters */
	/*@{*/
	/** \brief Count of lumimeters. */
	inline int GetLumimeterCount() const{ return pLumimeterCount; }
	
	/** \brief Root lumimeter or NULL if there are none. */
	inline deLumimeter *GetRootLumimeter() const{ return pLumimeterRoot; }
	
	/**
	 * \brief Add lumimeter.
	 * \throws deeInvalidParam \em lumimeter is NULL.
	 * \throws deeInvalidParam \em lumimeter has a parent world.
	 */
	void AddLumimeter( deLumimeter *lumimeter );
	
	/**
	 * \brief Remove lumimeter.
	 * \throws deeInvalidParam \em lumimeter is NULL.
	 * \throws deeInvalidParam Parent world of \em lumimeter is not this world.
	 */
	void RemoveLumimeter( deLumimeter *lumimeter );
	
	/** \brief Remove all lumimeters. */
	void RemoveAllLumimeters();
	/*@}*/
	
	
	
	/** \name Microphones */
	/*@{*/
	/** \brief Count of microphones. */
	inline int GetMicrophoneCount() const{ return pMicrophoneCount; }
	
	/** \brief Root microphone or NULL if there are none. */
	inline deMicrophone *GetRootMicrophone() const{ return pMicrophoneRoot; }
	
	/**
	 * \brief Add microphone.
	 * \throws deeInvalidParam \em microphone is NULL.
	 * \throws deeInvalidParam \em microphone has a parent world.
	 */
	void AddMicrophone( deMicrophone *microphone );
	
	/**
	 * \brief Remove microphone.
	 * \throws deeInvalidParam \em microphone is NULL.
	 * \throws deeInvalidParam Parent world of \em microphone is not this world.
	 */
	void RemoveMicrophone( deMicrophone *microphone );
	
	/** \brief Remove all microphones. */
	void RemoveAllMicrophones();
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief Count of navigation spaces. */
	inline int GetNavigationSpaceCount() const{ return pNavSpaceCount; }
	
	/** \brief Root navigation space or NULL if there are none. */
	inline deNavigationSpace *GetRootNavigationSpace() const{ return pNavSpaceRoot; }
	
	/**
	 * \brief Add navigation space.
	 * \throws deeInvalidParam \em navigationSpace is NULL.
	 * \throws deeInvalidParam \em navigationSpace has a parent world.
	 */
	void AddNavigationSpace( deNavigationSpace *navigationSpace );
	
	/**
	 * \brief Remove navigation space.
	 * \throws deeInvalidParam \em navigationSpace is NULL.
	 * \throws deeInvalidParam Parent world of \em navigationSpace is not this world.
	 */
	void RemoveNavigationSpace( deNavigationSpace *navigationSpace );
	
	/** \brief Remove all navigation spaces. */
	void RemoveAllNavigationSpaces();
	/*@}*/
	
	
	
	/** \name Navigation blockers */
	/*@{*/
	/** \brief Count of navigation blockers. */
	inline int GetNavigationBlockerCount() const{ return pNavBlockerCount; }
	
	/** \brief Root navigation blocker or NULL if there are none. */
	inline deNavigationBlocker *GetRootNavigationBlocker() const{ return pNavBlockerRoot; }
	
	/**
	 * \brief Add navigation blocker.
	 * \throws deeInvalidParam \em navigationBlocker is NULL.
	 * \throws deeInvalidParam \em navigationBlocker has a parent world.
	 */
	void AddNavigationBlocker( deNavigationBlocker *navigationBlocker );
	
	/**
	 * \brief Remove navigation blocker.
	 * \throws deeInvalidParam \em navigationBlocker is NULL.
	 * \throws deeInvalidParam Parent world of \em navigationBlocker is not this world.
	 */
	void RemoveNavigationBlocker( deNavigationBlocker *navigationBlocker );
	
	/** \brief Remove all navigation blockers. */
	void RemoveAllNavigationBlockers();
	/*@}*/
	
	
	
	/** \name Navigators */
	/*@{*/
	/** \brief Count of navigators. */
	inline int GetNavigatorCount() const{ return pNavigatorCount; }
	
	/** \brief Root navigator or NULL if there are none. */
	inline deNavigator *GetRootNavigator() const{ return pNavigatorRoot; }
	
	/**
	 * \brief Add navigator.
	 * \throws deeInvalidParam \em navigator is NULL.
	 * \throws deeInvalidParam \em navigator has a parent world.
	 */
	void AddNavigator( deNavigator *navigator );
	
	/**
	 * \brief Remove navigator.
	 * \throws deeInvalidParam \em navigator is NULL.
	 * \throws deeInvalidParam Parent world of \em navigator is not this world.
	 */
	void RemoveNavigator( deNavigator *navigator );
	
	/** \brief Remove all navigators. */
	void RemoveAllNavigators();
	/*@}*/
	
	
	
	/** \name Network states */
	/*@{*/
	/** \brief Count of network states. */
	inline int GetNetworkStateCount() const{ return pNetworkStateCount; }
	
	/** \brief Root network state or NULL if there are none. */
	inline deNetworkState *GetRootNetworkState() const{ return pNetworkStateRoot; }
	
	/**
	 * \brief Add network state.
	 * \throws deeInvalidParam \em networkState is NULL.
	 * \throws deeInvalidParam \em networkState has a parent world.
	 */
	void AddNetworkState( deNetworkState *networkState );
	
	/**
	 * \brief Remove network state.
	 * \throws deeInvalidParam \em networkState is NULL.
	 * \throws deeInvalidParam Parent world of \em networkState is not this world.
	 */
	void RemoveNetworkState( deNetworkState *networkState );
	
	/** \brief Remove all network states. */
	void RemoveAllNetworkStates();
	/*@}*/
	
	
	
	/** \name Particle emitters */
	/*@{*/
	/** \brief Count of particle emitters. */
	inline int GetParticleEmitterCount() const{ return pParticleEmitterCount; }
	
	/** \brief Root particle emitter or NULL if there are none. */
	inline deParticleEmitterInstance *GetRootParticleEmitter() const{ return pParticleEmitterRoot; }
	
	/**
	 * \brief Add particle emitter.
	 * \throws deeInvalidParam \em particleEmitter is NULL.
	 * \throws deeInvalidParam \em particleEmitter has a parent world.
	 */
	void AddParticleEmitter( deParticleEmitterInstance *particleEmitter );
	
	/**
	 * \brief Remove particle emitter.
	 * \throws deeInvalidParam \em particleEmitter is NULL.
	 * \throws deeInvalidParam Parent world of \em particleEmitter is not this world.
	 */
	void RemoveParticleEmitter( deParticleEmitterInstance *particleEmitter );
	
	/** \brief Remove all particle emitters. */
	void RemoveAllParticleEmitters();
	/*@}*/
	
	
	
	/** \name Prop fields */
	/*@{*/
	/** \brief Count of prop fields. */
	inline int GetPropFieldCount() const{ return pPropFieldCount; }
	
	/** \brief Root prop field or NULL if there are none. */
	inline dePropField *GetRootPropField() const{ return pPropFieldRoot; }
	
	/**
	 * \brief Add prop field.
	 * \throws deeInvalidParam \em propField is NULL.
	 * \throws deeInvalidParam \em propField has a parent world.
	 */
	void AddPropField( dePropField *propField );
	
	/**
	 * \brief Remove prop field.
	 * \throws deeInvalidParam \em propField is NULL.
	 * \throws deeInvalidParam Parent world of \em propField is not this world.
	 */
	void RemovePropField( dePropField *propField );
	
	/** \brief Remove all prop fields. */
	void RemoveAllPropFields();
	/*@}*/
	
	
	
	/** \name Speakers */
	/*@{*/
	/** \brief Count of speakers. */
	inline int GetSpeakerCount() const{ return pSpeakerCount; }
	
	/** \brief Root speaker or NULL if there are none. */
	inline deSpeaker *GetRootSpeaker() const{ return pSpeakerRoot; }
	
	/**
	 * \brief Add speaker.
	 * \throws deeInvalidParam \em speaker is NULL.
	 * \throws deeInvalidParam \em speaker has a parent world.
	 */
	void AddSpeaker( deSpeaker *speaker );
	
	/**
	 * \brief Remove speaker.
	 * \throws deeInvalidParam \em speaker is NULL.
	 * \throws deeInvalidParam Parent world of \em speaker is not this world.
	 */
	void RemoveSpeaker( deSpeaker *speaker );
	
	/** \brief Remove all speakers. */
	void RemoveAllSpeakers();
	/*@}*/
	
	
	
	/** \name Sound level meters */
	/*@{*/
	/** \brief Count of sound level meters. */
	inline int GetSoundLevelMeterCount() const{ return pSoundLevelMeterCount; }
	
	/** \brief Root sound level meter or NULL if there are none. */
	inline deSoundLevelMeter *GetRootSoundLevelMeter() const{ return pSoundLevelMeterRoot; }
	
	/**
	 * \brief Add sound level meter.
	 * \throws deeInvalidParam \em soundLevelMeter is NULL.
	 * \throws deeInvalidParam \em soundLevelMeter has a parent world.
	 */
	void AddSoundLevelMeter( deSoundLevelMeter *soundLevelMeter );
	
	/**
	 * \brief Remove sound level meter.
	 * \throws deeInvalidParam \em soundLevelMeter is NULL.
	 * \throws deeInvalidParam Parent world of \em soundLevelMeter is not this world.
	 */
	void RemoveSoundLevelMeter( deSoundLevelMeter *soundLevelMeter );
	
	/** \brief Remove all sound level meters. */
	void RemoveAllSoundLevelMeters();
	/*@}*/
	
	
	
	/** \name Touch sensors */
	/*@{*/
	/** \brief Count of touch sensors. */
	inline int GetTouchSensorCount() const{ return pTouchSensorCount; }
	
	/** \brief Root touch sensor or NULL if there are none. */
	inline deTouchSensor *GetRootTouchSensor() const{ return pTouchSensorRoot; }
	
	/**
	 * \brief Add touch sensor.
	 * \throws deeInvalidParam \em touchSensor is NULL.
	 * \throws deeInvalidParam \em touchSensor has a parent world.
	 */
	void AddTouchSensor( deTouchSensor *touchSensor );
	
	/**
	 * \brief Remove touch sensor.
	 * \throws deeInvalidParam \em touchSensor is NULL.
	 * \throws deeInvalidParam Parent world of \em touchSensor is not this world.
	 */
	void RemoveTouchSensor( deTouchSensor *touchSensor );
	
	/** \brief Remove all touch sensors. */
	void RemoveAllTouchSensors();
	/*@}*/
	
	
	
	/** \name Smoke emitters */
	/*@{*/
	/** \brief Count of smoke emitters. */
	inline int GetSmokeEmitterCount() const{ return pSmokeEmitterCount; }
	
	/** \brief Root smoke emitter or NULL if there are none. */
	inline deSmokeEmitter *GetRootSmokeEmitter() const{ return pSmokeEmitterRoot; }
	
	/**
	 * \brief Add smoke emitter.
	 * \throws deeInvalidParam \em smokeEmitter is NULL.
	 * \throws deeInvalidParam \em smokeEmitter has a parent world.
	 */
	void AddSmokeEmitter( deSmokeEmitter *smokeEmitter );
	
	/**
	 * \brief Remove smoke emitter.
	 * \throws deeInvalidParam \em smokeEmitter is NULL.
	 * \throws deeInvalidParam Parent world of \em smokeEmitter is not this world.
	 */
	void RemoveSmokeEmitter( deSmokeEmitter *smokeEmitter );
	
	/** \brief Remove all smoke emitters. */
	void RemoveAllSmokeEmitters();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * \brief Test a point for collision with colliders.
	 * 
	 * For each collision the collisionResponse function the given listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	void PointHits( const decDVector &point, deBaseScriptingCollider *listener,
	const decCollisionFilter &collisionFilter );
	
	/**
	 * \brief Tests a ray for collision with the element in the world.
	 * 
	 * For each collision the collisionResponse function the given listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	void RayHits( const decDVector &rayOrigin, const decVector &rayDirection, deBaseScriptingCollider *listener,
	const decCollisionFilter &collisionFilter );
	
	/**
	 * \brief Tests the collider for collision with world elements.
	 * 
	 * For each collision the collisionResponse function of the listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	void ColliderHits( deCollider *collider, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Tests the moving collider for collision with world elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function
	 * of the listener is called. To stop testing set StopTesting in the provided
	 * collision information object to true.
	 */
	void ColliderMoveHits( deCollider *collider, const decVector &displacement, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Tests the rotating collider for collision with world elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function
	 * of the listener is called. To stop testing set StopTesting in the provided
	 * collision information object to true.
	 */
	void ColliderRotateHits( deCollider *collider, const decVector &rotation, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Tests the moving and rotating collider for collision with world elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function
	 * of the listener is called. To stop testing set StopTesting in the provided
	 * collision information object to true.
	 */
	void ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
	const decVector &rotation, deBaseScriptingCollider *listener );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicWorld *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic( deBaseGraphicWorld *peer );
	
	/** \brief Physics system peer object. */
	inline deBasePhysicsWorld *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics( deBasePhysicsWorld *peer );
	
	/** \brief Audio system peer object. */
	inline deBaseAudioWorld *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer object. */
	void SetPeerAudio( deBaseAudioWorld *peer );
	
	/** \brief Network system peer object. */
	inline deBaseNetworkWorld *GetPeerNetwork() const{ return pPeerNetwork; }
	
	/** \brief Set network system peer object. */
	void SetPeerNetwork( deBaseNetworkWorld *peer );
	
	/** \brief Ai system peer object. */
	inline deBaseAIWorld *GetPeerAI() const{ return pPeerAI; }
	
	/** \brief Set ai system peer object. */
	void SetPeerAI( deBaseAIWorld *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pNotifyPhysicsChanged();
	void pNotifyLightingChanged();
};

#endif
