/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELGAMEPROFILE_H_
#define _DELGAMEPROFILE_H_

#include "delGPModuleList.h"
#include "delGPDisableModuleVersionList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

class delLauncher;
class delEngineInstance;



/**
 * \brief Game Profile.
 */
class delGameProfile : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGameProfile> Ref;
	
	
	
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
	
	decString pModuleGraphicVersion;
	decString pModuleInputVersion;
	decString pModulePhysicsVersion;
	decString pModuleAnimatorVersion;
	decString pModuleAIVersion;
	decString pModuleCrashRecoveryVersion;
	decString pModuleAudioVersion;
	decString pModuleSynthesizerVersion;
	decString pModuleNetworkVersion;
	
	delGPDisableModuleVersionList pDisableModuleVersionList;
	
	delGPModuleList pModuleList;
	
	decString pRunArgs;
	bool pReplaceRunArgs;
	bool pFullScreen;
	int pWidth;
	int pHeight;
	
	bool pValid;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new game profile. */
	delGameProfile( const char *name = "" );
	
	/** \brief Create a copy of a game profile. */
	delGameProfile( const delGameProfile &profile );
	
protected:
	/** \brief Clean up the game profile. */
	virtual ~delGameProfile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Profile name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set profile name. */
	void SetName( const char *name );
	
	/** \brief Graphic module or an empty string if not set. */
	inline const decString &GetModuleGraphic() const{ return pModuleGraphic; }
	
	/** \brief Set graphic module or an empty string if not set. */
	void SetModuleGraphic( const char *moduleName );
	
	/** \brief Input module or an empty string if not set. */
	inline const decString &GetModuleInput() const{ return pModuleInput; }
	
	/** \brief Set input module or an empty string if not set. */
	void SetModuleInput( const char *moduleName );
	
	/** \brief Physics module or an empty string if not set. */
	inline const decString &GetModulePhysics() const{ return pModulePhysics; }
	
	/** \brief Set physics module or an empty string if not set. */
	void SetModulePhysics( const char *moduleName );
	
	/** \brief Animator module or an empty string if not set. */
	inline const decString &GetModuleAnimator() const{ return pModuleAnimator; }
	
	/** \brief Set animator module or an empty string if not set. */
	void SetModuleAnimator( const char *moduleName );
	
	/** \brief AI module or an empty string if not set. */
	inline const decString &GetModuleAI() const{ return pModuleAI; }
	
	/** \brief Set AI module or an empty string if not set. */
	void SetModuleAI( const char *moduleName );
	
	/** \brief Crash recovery module or an empty string if not set. */
	inline const decString &GetModuleCrashRecovery() const{ return pModuleCrashRecovery; }
	
	/** \brief Set crash recovery module or an empty string if not set. */
	void SetModuleCrashRecovery( const char *moduleName );
	
	/** \brief Audio module or an empty string if not set. */
	inline const decString &GetModuleAudio() const{ return pModuleAudio; }
	
	/** \brief Set audio module or an empty string if not set. */
	void SetModuleAudio( const char *moduleName );
	
	/** \brief Synthesizer module or an empty string if not set. */
	inline const decString &GetModuleSynthesizer() const{ return pModuleSynthesizer; }
	
	/** \brief Set synthesizer module or an empty string if not set. */
	void SetModuleSynthesizer( const char *moduleName );
	
	/** \brief Network module or an empty string if not set. */
	inline const decString &GetModuleNetwork() const{ return pModuleNetwork; }
	
	/** \brief Set network module or an empty string if not set. */
	void SetModuleNetwork( const char *moduleName );
	
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
	
	/** \brief List of module versions to enable or disable. */
	inline delGPDisableModuleVersionList &GetDisableModuleVersionList(){ return pDisableModuleVersionList; }
	inline const delGPDisableModuleVersionList &GetDisableModuleVersionList() const{ return pDisableModuleVersionList; }
	
	/** \brief Lodule list. */
	inline delGPModuleList &GetModuleList(){ return pModuleList; }
	inline const delGPModuleList &GetModuleList() const{ return pModuleList; }
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArgs; }
	
	/** \brief Set run arguments. */
	void SetRunArguments( const char *arguments );
	
	/** \brief Run arguments replace the game specific run arguments or add to them. */
	inline bool GetReplaceRunArguments() const{ return pReplaceRunArgs; }
	
	/** \brief Set if run arguments replace the game specific run arguments or add to them. */
	void SetReplaceRunArguments( bool replaceRunArguments );
	
	/** \brief Window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if window is full screen. */
	void SetFullScreen( bool fullScreen );
	
	/** \brief Window width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set window width. */
	void SetWidth( int width );
	
	/** \brief Window height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set window height. */
	void SetHeight( int height );
	
	/** \brief Profile is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** \brief Verify profile. */
	void Verify( delLauncher &launcher );
	
	/** \brief Verify module. */
	bool VerifyModule( delLauncher &launcher, const char *moduleName,
		const char *moduleVersion, int requiredType ) const;
	
	/**
	 * \brief Activate profile.
	 * 
	 * This activates the modules and sets the module properties.
	 */
	void Activate( delLauncher &launcher, delEngineInstance &engineInstance ) const;
	/*@}*/
	
	
	
public:
	/** \name Operators */
	/*@{*/
	/** \brief Copy profile. */
	delGameProfile &operator=( const delGameProfile &profile );
	/*@}*/
};

#endif
