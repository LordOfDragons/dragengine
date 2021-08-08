/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLGAMEPROFILE_H_
#define _DELLGAMEPROFILE_H_

#include "dellGPModuleList.h"
#include "dellGPDisableModuleVersionList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class dellLauncher;



/**
 * @brief Game Profile.
 */
class dellGameProfile : public deObject{
private:
	decString pName;
	
	decString pModuleGraphic;
	decString pModuleInput;
	decString pModulePhysics;
	decString pModuleAnimator;
	decString pModuleAI;
	decString pModuleCrashRecovery;
	decString pModuleAudio;
	decString pModuleSynthesizer;
	decString pModuleNetwork;
	decString pModuleVR;
	
	decString pModuleGraphicVersion;
	decString pModuleInputVersion;
	decString pModulePhysicsVersion;
	decString pModuleAnimatorVersion;
	decString pModuleAIVersion;
	decString pModuleCrashRecoveryVersion;
	decString pModuleAudioVersion;
	decString pModuleSynthesizerVersion;
	decString pModuleNetworkVersion;
	decString pModuleVRVersion;
	
	dellGPModuleList pModuleList;
	
	dellGPDisableModuleVersionList pDisableModuleVersionList;
	
	decString pRunArgs;
	bool pReplaceRunArgs;
	bool pFullScreen;
	int pWidth;
	int pHeight;
	
	bool pValid;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile. */
	dellGameProfile( const char *name = "" );
	
	/** \brief Create copy of game profile. */
	dellGameProfile( const dellGameProfile& profile );
	
	/** Cleans up the game profile. */
	virtual ~dellGameProfile();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the profile name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the profile name. */
	void SetName( const char *name );
	
	/** Retrieves the graphic module or NULL if not set. */
	inline const decString &GetModuleGraphic() const{ return pModuleGraphic; }
	/** Sets the graphic module or NULL if not set. */
	void SetModuleGraphic( const char *moduleName );
	/** Retrieves the input module or NULL if not set. */
	inline const decString &GetModuleInput() const{ return pModuleInput; }
	/** Sets the input module or NULL if not set. */
	void SetModuleInput( const char *moduleName );
	/** Retrieves the physics module or NULL if not set. */
	inline const decString &GetModulePhysics() const{ return pModulePhysics; }
	/** Sets the physics module or NULL if not set. */
	void SetModulePhysics( const char *moduleName );
	/** Retrieves the animator module or NULL if not set. */
	inline const decString &GetModuleAnimator() const{ return pModuleAnimator; }
	/** Sets the animator module or NULL if not set. */
	void SetModuleAnimator( const char *moduleName );
	/** Retrieves the AI module or NULL if not set. */
	inline const decString &GetModuleAI() const{ return pModuleAI; }
	/** Sets the AI module or NULL if not set. */
	void SetModuleAI( const char *moduleName );
	/** Retrieves the crash recovery module or NULL if not set. */
	inline const decString &GetModuleCrashRecovery() const{ return pModuleCrashRecovery; }
	/** Sets the crash recovery module or NULL if not set. */
	void SetModuleCrashRecovery( const char *moduleName );
	/** Retrieves the audio module or NULL if not set. */
	inline const decString &GetModuleAudio() const{ return pModuleAudio; }
	/** Sets the audio module or NULL if not set. */
	void SetModuleAudio( const char *moduleName );
	
	/** \brief Synthesizer module or empty string if not set. */
	inline const decString &GetModuleSynthesizer() const{ return pModuleSynthesizer; }
	
	/** \brief Set synthesizer module or empty string if not set. */
	void SetModuleSynthesizer( const char *moduleName );
	
	/** Retrieves the network module or NULL if not set. */
	inline const decString &GetModuleNetwork() const{ return pModuleNetwork; }
	/** Sets the network module or NULL if not set. */
	void SetModuleNetwork( const char *moduleName );
	
	/** Retrieves the vr module or NULL if not set. */
	inline const decString &GetModuleVR() const{ return pModuleVR; }
	/** Sets the vr module or NULL if not set. */
	void SetModuleVR( const char *moduleName );
	
	/** \brief Graphic module version or an empty string to use the latest. */
	inline const decString &GetModuleGraphicVersion() const{ return pModuleGraphicVersion; }
	
	/** \brief Set graphic module version or an empty string to use the latest. */
	void SetModuleGraphicVersion( const char *moduleVersion );
	
	/** \brief Input module version or an empty string to use the latest. */
	inline const decString &GetModuleInputVersion() const{ return pModuleInputVersion; }
	
	/** \brief Set input module version or an empty string to use the latest. */
	void SetModuleInputVersion( const char *moduleVersion );
	
	/** \brief Physics module version or an empty string to use the latest. */
	inline const decString &GetModulePhysicsVersion() const{ return pModulePhysicsVersion; }
	
	/** \brief Set physics module version or an empty string to use the latest. */
	void SetModulePhysicsVersion( const char *moduleVersion );
	
	/** \brief Animator module version or an empty string to use the latest. */
	inline const decString &GetModuleAnimatorVersion() const{ return pModuleAnimatorVersion; }
	
	/** \brief Set animator module version or an empty string to use the latest. */
	void SetModuleAnimatorVersion( const char *moduleVersion );
	
	/** \brief AI module version or an empty string to use the latest. */
	inline const decString &GetModuleAIVersion() const{ return pModuleAIVersion; }
	
	/** \brief Set AI module version or an empty string to use the latest. */
	void SetModuleAIVersion( const char *moduleVersion );
	
	/** \brief Crash recovery module version or an empty string to use the latest. */
	inline const decString &GetModuleCrashRecoveryVersion() const{ return pModuleCrashRecoveryVersion; }
	
	/** \brief Set crash recovery module version or an empty string to use the latest. */
	void SetModuleCrashRecoveryVersion( const char *moduleVersion );
	
	/** \brief Audio module version or an empty string to use the latest. */
	inline const decString &GetModuleAudioVersion() const{ return pModuleAudioVersion; }
	
	/** \brief Set audio module version or an empty string to use the latest. */
	void SetModuleAudioVersion( const char *moduleVersion );
	
	/** \brief Synthesizer module version or an empty string to use the latest. */
	inline const decString &GetModuleSynthesizerVersion() const{ return pModuleSynthesizerVersion; }
	
	/** \brief Set synthesizer module version or an empty string to use the latest. */
	void SetModuleSynthesizerVersion( const char *moduleVersion );
	
	/** \brief Retrieve network module version or an empty string to use the latest. */
	inline const decString &GetModuleNetworkVersion() const{ return pModuleNetworkVersion; }
	
	/** \brief Set network module version or an empty string to use the latest. */
	void SetModuleNetworkVersion( const char *moduleVersion );
	
	/** \brief VR module version or an empty string to use the latest. */
	inline const decString &GetModuleVRVersion() const{ return pModuleVRVersion; }
	
	/** \brief Set vr module version or an empty string to use the latest. */
	void SetModuleVRVersion( const char *moduleVersion );
	
	/** Retrieves the module list. */
	inline dellGPModuleList &GetModuleList(){ return pModuleList; }
	inline const dellGPModuleList &GetModuleList() const{ return pModuleList; }
	
	/** \brief Retrieves the list of module versions to enable or disable. */
	inline dellGPDisableModuleVersionList &GetDisableModuleVersionList(){ return pDisableModuleVersionList; }
	inline const dellGPDisableModuleVersionList &GetDisableModuleVersionList() const{ return pDisableModuleVersionList; }
	
	/** Retrieves the run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArgs; }
	/** Sets the run arguments. */
	void SetRunArguments( const char *arguments );
	/** Determines if the run arguments replace the game specific run arguments or add to them. */
	inline bool GetReplaceRunArguments() const{ return pReplaceRunArgs; }
	/** Sets if the run arguments replace the game specific run arguments or add to them. */
	void SetReplaceRunArguments( bool replaceRunArguments );
	/** Retrieves the window width. */
	inline int GetWidth() const{ return pWidth; }
	/** Sets the window width. */
	void SetWidth( int width );
	/** Retrieves the window height. */
	inline int GetHeight() const{ return pHeight; }
	/** Sets the window height. */
	void SetHeight( int height );
	/** Determines if the window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	/** Sets if the window is full screen. */
	void SetFullScreen( bool fullScreen );
	
	/** Determines if the profile is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** \brief Copy from another profile. */
	void CopyFrom( const dellGameProfile &profile );
	
	/** Verify profile. */
	void Verify( dellLauncher &launcher );
	/** \brief Verify a module. */
	bool VerifyModule( dellLauncher &launcher, const char *moduleName, int requiredType ) const;
	/** Activates the profile activating the modules and setting the module properties. */
	void Activate( dellLauncher &launcher ) const;
	/*@}*/
};

#endif
