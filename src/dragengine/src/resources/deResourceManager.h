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

#ifndef _DERESOURCEMANAGER_H_
#define _DERESOURCEMANAGER_H_

#include "../common/string/decString.h"

class deAISystem;
class deAnimatorSystem;
class deAudioSystem;
class deEngine;
class deException;
class deGraphicSystem;
class deImageManager;
class deLogger;
class deModuleSystem;
class deNetworkSystem;
class dePhysicsSystem;
class deResource;
class deScriptingSystem;
class deSkinManager;
class deSynthesizerSystem;
class deVideoManager;


/**
 * \brief Base resource manager class.
 * 
 * Provides basic support for resource managers. The actual management
 * of resource the subclass should implement using the resource list
 * class provided (although you are free to do differently if you really
 * know what you are doing). The RemoveResource function is protected
 * and has to be used only by a deResource derived object to notify the
 * resource manager that this object has to be freed from memory. Do not
 * do any reference counting inside the subclass. If a resource is ready
 * to be freed it will tell you by calling RemoveResource.
 */
class deResourceManager{
public:
	/** \brief Possible type of a resource manager. */
	enum eResourceType{
		/** \brief Animation resource */
		ertAnimation,
		
		/** \brief Animator resource */
		ertAnimator,
		
		/** \brief Animator instance resource. */
		ertAnimatorInstance,
		
		/** \brief Archive resource. */
		ertArchive,
		
		/** \brief Billboard resource. */
		ertBillboard,
		
		/** \brief \brief Canvas resource. */
		ertCanvas,
		
		/** \brief Scene Camera resource */
		ertCamera,
		
		/** \brief \brief Capture canvas resource. */
		ertCaptureCanvas,
		
		/** \brief World Collider resource */
		ertCollider,
		
		/** \brief Component resource */
		ertComponent,
		
		/** \brief Connection resource. */
		ertConnection,
		
		/** \brief Decal resource. */
		ertDecal,
		
		/** \brief Debug Drawer resource */
		ertDebugDrawer,
		
		/** \brief Dynamic Skin resource. */
		ertDynamicSkin,
		
		/** \brief Effect resource */
		ertEffect,
		
		/** \brief Environment map probe resource */
		ertEnvMapProbe,
		
		/** \brief Font resource */
		ertFont,
		
		/** \brief Force Field resource. */
		ertForceField,
		
		/** \brief Graphic Context resource. */
		ertGraphicContext,
		
		/** \brief Height Terrain resource. */
		ertHeightTerrain,
		
		/** \brief Image resource */
		ertImage,
		
		/** \brief Language Pack resource */
		ertLanguagePack,
		
		/** \brief Scene Light resource */
		ertLight,
		
		/** \brief \brief Locomotion resource. */
		ertLocomotion,
		
		/** \brief Lumimeter resource. */
		ertLumimeter,
		
		/** \brief Microphone resource. */
		ertMicrophone,
		
		/** \brief Model resource */
		ertModel,
		
		/** \brief \brief Navigation blocker. */
		ertNavigationBlocker,
		
		/** \brief Navigation space resource. */
		ertNavigationSpace,
		
		/** \brief Navigator resource. */
		ertNavigator,
		
		/** \brief Network state resource. */
		ertNetworkState,
		
		/** \brief Occluder resource. */
		ertOccluder,
		
		/** \brief Occlusion mesh resource. */
		ertOcclusionMesh,
		
		/** \brief Particle Emitter resource. */
		ertParticleEmitter,
		
		/** \brief Particle emitter instance resource. */
		ertParticleEmitterInstance,
		
		/** \brief Prop Field resource. */
		ertPropField,
		
		/** \brief Render Target resource. */
		ertRenderTarget,
		
		/** \brief Render Window resource. */
		ertRenderWindow,
		
		/** \brief Rig resource */
		ertRig,
		
		/** \brief Scene resource */
		ertScene,
		
		/** \brief Server resource. */
		ertServer,
		
		/** \brief Skin resource */
		ertSkin,
		
		/** \brief Sky resource */
		ertSky,
		
		/** \brief Sky instance resource */
		ertSkyInstance,
		
		/** \brief Smoke Emitter resource. */
		ertSmokeEmitter,
		
		/** \brief Sound resource. */
		ertSound,
		
		/** \brief Sound level meter resource. */
		ertSoundLevelMeter,
		
		/** \brief Speaker resource. */
		ertSpeaker,
		
		/** \brief \brief Synthesizer resource. */
		ertSynthesizer,
		
		/** \brief \brief Synthesizer instance resource. */
		ertSynthesizerInstance,
		
		/** \brief Touch Sensor resource. */
		ertTouchSensor,
		
		/** \brief Video resource. */
		ertVideo,
		
		/** \brief Video player resource. */
		ertVideoPlayer,
		
		/** \brief World resource */
		ertWorld
	};
	
	
	
private:
	deEngine *pEngine;
	decString pLoggingName;
	eResourceType pResourceType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource manager. */
	deResourceManager( deEngine *engine, eResourceType type );
	
	/** \brief Clean up resource manager. */
	virtual ~deResourceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Resources type stored in this manager. */
	inline int GetResourceType() const{ return pResourceType; }
	
	/** \brief Name to use as the source for this module for logging. */
	inline const decString &GetLoggingName() const{ return pLoggingName; }
	
	/** \brief Set name to use as the source for this module for logging. */
	void SetLoggingName( const char *name );
	
	/** \brief Release leaking resource and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/**
	 * \name Quick System Access
	 * 
	 * Shortcuts for retrieving various systems from the linked game engine.
	 */
	/*@{*/
	deModuleSystem *GetModuleSystem() const;
	deGraphicSystem *GetGraphicSystem() const;
	dePhysicsSystem *GetPhysicsSystem() const;
	deAnimatorSystem *GetAnimatorSystem() const;
	deSynthesizerSystem *GetSynthesizerSystem() const;
	deAISystem *GetAISystem() const;
	deAudioSystem *GetAudioSystem() const;
	deNetworkSystem *GetNetworkSystem() const;
	deScriptingSystem *GetScriptingSystem() const;
	deImageManager *GetImageManager() const;
	deVideoManager *GetVideoManager() const;
	deSkinManager *GetSkinManager() const;
	/*@}*/
	
	
	
	/** \name System Peer Support */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	
	
	
	/** \brief Physics System Peers of all stored resources have to be created. */
	virtual void SystemPhysicsLoad();
	
	/** \brief Physics System Peers of all stored resources have to be freed. */
	virtual void SystemPhysicsUnload();
	
	
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	virtual void SystemScriptingLoad();
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	virtual void SystemScriptingUnload();
	
	
	
	/** \brief Animator System Peers of all stored resources have to be created. */
	virtual void SystemAnimatorLoad();
	
	/** \brief Animator System Peers of all stored resources have to be freed. */
	virtual void SystemAnimatorUnload();
	
	
	
	/** \brief Audio System Peers of all stored resources have to be created. */
	virtual void SystemAudioLoad();
	
	/** \brief Audio System Peers of all stored resources have to be freed. */
	virtual void SystemAudioUnload();
	
	
	
	/** \brief Network System Peers of all stored resources have to be created. */
	virtual void SystemNetworkLoad();
	
	/** \brief Network System Peers of all stored resources have to be freed. */
	virtual void SystemNetworkUnload();
	
	
	
	/** \brief AI System Peers of all stored resources have to be created. */
	virtual void SystemAILoad();
	
	/** \brief AI System Peers of all stored resources have to be freed. */
	virtual void SystemAIUnload();
	
	
	
	/** \brief Synthesizer system peers of all stored resources have to be created. */
	virtual void SystemSynthesizerLoad();
	
	/** \brief Synthesizer system peers of all stored resources have to be freed. */
	virtual void SystemSynthesizerUnload();
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Output information message on the console. */
	void LogInfo( const char *message );
	
	/** \brief Output formated information message on the console. */
	void LogInfoFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** \brief Output formated information message on the console. */
	void LogInfoFormatUsing( const char *message, va_list args );
	
	/** \brief Output warning message on the console. */
	void LogWarn( const char *message );
	
	/** \brief Output formated warning message on the console. */
	void LogWarnFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** \brief Output formated warning message on the console. */
	void LogWarnFormatUsing( const char *message, va_list args );
	
	/** \brief Output error message on the console. */
	void LogError( const char *message );
	
	/** \brief Output formated error message on the console. */
	void LogErrorFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** \brief Output formated error message on the console. */
	void LogErrorFormatUsing( const char *message, va_list args );
	
	/** \brief Output exception as error message on the console. */
	void LogException( const deException &exception );
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning For use by deResource only.
	 */
	/*@{*/
	/** \brief Removes the given resource from the manager without freeing it. */
	virtual void RemoveResource( deResource *resource ) = 0;
	/*@}*/
};

#endif
