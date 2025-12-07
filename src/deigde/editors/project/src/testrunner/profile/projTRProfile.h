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

#ifndef _PROJTRPROFILE_H_
#define _PROJTRPROFILE_H_

#include "projTRPParameterList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/systems/deModuleSystem.h>

class deEngine;



/**
 * \brief Profile.
 */
class projTRProfile : public deObject{
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
	
	projTRPParameterList pParameters;
	decStringDictionary pDisableModuleVersions;
	
	decString pRunArgs;
	bool pReplaceRunArgs;
	int pWidth;
	int pHeight;
	bool pFullScreen;
	
	bool pValid;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<projTRProfile> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile. */
	projTRProfile();
	
protected:
	/** \brief Clean up profile. */
	virtual ~projTRProfile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Graphic module. */
	inline const decString &GetModuleGraphic() const{ return pModuleGraphic; }
	
	/** \brief Set graphic module. */
	void SetModuleGraphic(const char *moduleName);
	
	/** \brief Input module. */
	inline const decString &GetModuleInput() const{ return pModuleInput; }
	
	/** \brief Set input module. */
	void SetModuleInput(const char *moduleName);
	
	/** \brief Physics module. */
	inline const decString &GetModulePhysics() const{ return pModulePhysics; }
	
	/** \brief Set physics module. */
	void SetModulePhysics(const char *moduleName);
	
	/** \brief Animator module. */
	inline const decString &GetModuleAnimator() const{ return pModuleAnimator; }
	
	/** \brief Set animator module. */
	void SetModuleAnimator(const char *moduleName);
	
	/** \brief AI module. */
	inline const decString &GetModuleAI() const{ return pModuleAI; }
	
	/** \brief Set AI module. */
	void SetModuleAI(const char *moduleName);
	
	/** \brief Crash recovery module. */
	inline const decString &GetModuleCrashRecovery() const{ return pModuleCrashRecovery; }
	
	/** \brief Set crash recovery module. */
	void SetModuleCrashRecovery(const char *moduleName);
	
	/** \brief Audio module. */
	inline const decString &GetModuleAudio() const{ return pModuleAudio; }
	
	/** \brief Set audio module. */
	void SetModuleAudio(const char *moduleName);
	
	/** \brief Synthesizer module. */
	inline const decString &GetModuleSynthesizer() const{ return pModuleSynthesizer; }
	
	/** \brief Set synthesizer module. */
	void SetModuleSynthesizer(const char *moduleName);
	
	/** \brief Network module. */
	inline const decString &GetModuleNetwork() const{ return pModuleNetwork; }
	
	/** \brief Set network module. */
	void SetModuleNetwork(const char *moduleName);
	
	/** \brief VR module. */
	inline const decString &GetModuleVR() const{ return pModuleVR; }
	
	/** \brief Set VR module. */
	void SetModuleVR(const char *moduleName);
	
	/** \brief Parameters. */
	inline projTRPParameterList &GetParameters(){ return pParameters; }
	inline const projTRPParameterList &GetParameters() const{ return pParameters; }
	
	/** \brief Disable module versionss. */
	inline decStringDictionary &GetDisableModuleVersions(){ return pDisableModuleVersions; }
	inline const decStringDictionary &GetDisableModuleVersions() const{ return pDisableModuleVersions; }
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArgs; }
	
	/** \brief Set run arguments. */
	void SetRunArguments(const char *arguments);
	
	/** \brief Run arguments replace game specific run arguments or add to them. */
	inline bool GetReplaceRunArguments() const{ return pReplaceRunArgs; }
	
	/** \brief Set if run arguments replace game specific run arguments or add to them. */
	void SetReplaceRunArguments(bool replaceRunArguments);
	
	/** \brief Window width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set window width. */
	void SetWidth(int width);
	
	/** \brief Window height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set window height. */
	void SetHeight(int height);
	
	/** \brief Window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if window is full screen. */
	void SetFullScreen(bool fullScreen);
	
	/** \brief Profile is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** \brief Verify profile. */
	void Verify(deEngine &engine);
	
	/** \brief Verify module. */
	bool VerifyModule(deEngine &engine, const char *moduleName,
		deModuleSystem::eModuleTypes requiredType) const;
	/*@}*/
};

#endif
