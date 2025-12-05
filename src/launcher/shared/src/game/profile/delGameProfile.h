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
class DE_DLL_EXPORT delGameProfile : public deObject{
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
	
	delGPDisableModuleVersionList pDisableModuleVersions;
	
	delGPModuleList pModules;
	
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
	
	/** \brief VR module or an empty string if not set. */
	inline const decString &GetModuleVR() const{ return pModuleVR; }
	
	/** \brief Set VR module or an empty string if not set. */
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
	
	/** \brief Set VR module version or an empty string to use the latest. */
	void SetModuleVRVersion( const char *moduleVersion );
	
	
	
	/** \brief List of module versions to enable or disable. */
	inline delGPDisableModuleVersionList &GetDisableModuleVersions(){ return pDisableModuleVersions; }
	inline const delGPDisableModuleVersionList &GetDisableModuleVersions() const{ return pDisableModuleVersions; }
	
	/** \brief Lodule list. */
	inline delGPModuleList &GetModules(){ return pModules; }
	inline const delGPModuleList &GetModules() const{ return pModules; }
	
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
