/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALGAMEPROFILE_H_
#define _DEALGAMEPROFILE_H_

#include "dealGPModuleList.h"
#include "dealGPDisableModuleVersionList.h"

#include "../../deObject.h"
#include "../../common/string/decString.h"
#include "../../common/string/decStringSet.h"

class dealLauncher;



/**
 * \brief Game Profile.
 */
class dealGameProfile : public deObject{
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
	
	dealGPDisableModuleVersionList pDisableModuleVersionList;
	
	dealGPModuleList pModuleList;
	
	decString pRunArgs;
	bool pReplaceRunArgs;
	int pWidth;
	int pHeight;
	bool pFullScreen;
	
	bool pValid;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new game profile. */
	dealGameProfile();
	
	/** \brief Create a copy of a game profile. */
	dealGameProfile( const dealGameProfile &profile );
	
	/** \brief Clean up the game profile. */
	virtual ~dealGameProfile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the profile name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set the profile name. */
	void SetName( const char *name );
	
	
	
	/** \brief Retrieve the graphic module or an empty string if not set. */
	inline const decString &GetModuleGraphic() const{ return pModuleGraphic; }
	
	/** \brief Set the graphic module or an empty string if not set. */
	void SetModuleGraphic( const char *moduleName );
	
	/** \brief Retrieve the input module or an empty string if not set. */
	inline const decString &GetModuleInput() const{ return pModuleInput; }
	
	/** \brief Set the input module or an empty string if not set. */
	void SetModuleInput( const char *moduleName );
	
	/** \brief Retrieve the physics module or an empty string if not set. */
	inline const decString &GetModulePhysics() const{ return pModulePhysics; }
	
	/** \brief Set the physics module or an empty string if not set. */
	void SetModulePhysics( const char *moduleName );
	
	/** \brief Retrieve the animator module or an empty string if not set. */
	inline const decString &GetModuleAnimator() const{ return pModuleAnimator; }
	
	/** \brief Set the animator module or an empty string if not set. */
	void SetModuleAnimator( const char *moduleName );
	
	/** \brief Retrieve the AI module or an empty string if not set. */
	inline const decString &GetModuleAI() const{ return pModuleAI; }
	
	/** \brief Set the AI module or an empty string if not set. */
	void SetModuleAI( const char *moduleName );
	
	/** \brief Retrieve the crash recovery module or an empty string if not set. */
	inline const decString &GetModuleCrashRecovery() const{ return pModuleCrashRecovery; }
	
	/** \brief Set the crash recovery module or an empty string if not set. */
	void SetModuleCrashRecovery( const char *moduleName );
	
	/** \brief Retrieve the audio module or an empty string if not set. */
	inline const decString &GetModuleAudio() const{ return pModuleAudio; }
	
	/** \brief Set the audio module or an empty string if not set. */
	void SetModuleAudio( const char *moduleName );
	
	/** \brief Synthesizer module or an empty string if not set. */
	inline const decString &GetModuleSynthesizer() const{ return pModuleSynthesizer; }
	
	/** \brief Set synthesizer module or an empty string if not set. */
	void SetModuleSynthesizer( const char *moduleName );
	
	/** \brief Retrieve the network module or an empty string if not set. */
	inline const decString &GetModuleNetwork() const{ return pModuleNetwork; }
	
	/** \brief Set the network module or an empty string if not set. */
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
	
	/** \brief Network module version or an empty string to use the latest. */
	inline const decString &GetModuleNetworkVersion() const{ return pModuleNetworkVersion; }
	
	/** \brief Set network module version or an empty string to use the latest. */
	void SetModuleNetworkVersion( const char *moduleVersion );
	
	
	
	/** \brief List of module versions to enable or disable. */
	inline dealGPDisableModuleVersionList &GetDisableModuleVersionList(){ return pDisableModuleVersionList; }
	inline const dealGPDisableModuleVersionList &GetDisableModuleVersionList() const{ return pDisableModuleVersionList; }
	
	/** \brief Module list. */
	inline dealGPModuleList &GetModuleList(){ return pModuleList; }
	inline const dealGPModuleList &GetModuleList() const{ return pModuleList; }
	
	
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArgs; }
	
	/** \brief Set run arguments. */
	void SetRunArguments( const char *arguments );
	
	/** \brief Run arguments replace the game specific run arguments or add to them. */
	inline bool GetReplaceRunArguments() const{ return pReplaceRunArgs; }
	
	/** \brief Set if run arguments replace the game specific run arguments or add to them. */
	void SetReplaceRunArguments( bool replaceRunArguments );
	
	/** \brief Window width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set window width. */
	void SetWidth( int width );
	
	/** \brief Window height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set window height. */
	void SetHeight( int height );
	
	/** \brief Window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if window is full screen. */
	void SetFullScreen( bool fullScreen );
	
	
	
	/** \brief Profile is valid. */
	inline bool GetValid() const{ return pValid; }
	
	
	
	/** \brief Copy from another profile. */
	void CopyFrom( const dealGameProfile &profile );
	
	/** \brief Verify profile. */
	void Verify( dealLauncher &launcher );
	
	/** \brief Verify a module. */
	bool VerifyModule( dealLauncher &launcher, const char *moduleName, const char *moduleVersion, int requiredType ) const;
	
	/** \brief Activate profile. This activates the modules and sets the module properties. */
	void Activate( dealLauncher &launcher ) const;
	/*@}*/
};

#endif
