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

#ifndef _DESOUNDLEVELMETERMANAGER_H_
#define _DESOUNDLEVELMETERMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSoundLevelMeter;


/**
 * \brief Sound level meter resourse manager.
 */
class DE_DLL_EXPORT deSoundLevelMeterManager : public deResourceManager{
private:
	deResourceList pSoundLevelMeters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound level meter resource manager. */
	deSoundLevelMeterManager(deEngine *engine);
	
	/** \brief Clean up sound level meter resource manager and report leaking resources. */
	~deSoundLevelMeterManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sound level meters. */
	int GetSoundLevelMeterCount() const;
	
	/** \brief Root sound level meter resource for iteration purpose. */
	deSoundLevelMeter *GetRootSoundLevelMeter() const;
	
	/** \brief Create sound level meter. */
	deSoundLevelMeter::Ref CreateSoundLevelMeter();
	
	/** \brief Report and release leaking resources. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Audio System Peers of all stored resources have to be created. */
	void SystemAudioLoad() override;
	
	/** \brief Audio System Peers of all stored resources have to be freed. */
	void SystemAudioUnload() override;
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	void SystemScriptingLoad() override;
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	void SystemScriptingUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning Only for use by deSoundLevelMeter. Never be call directly.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
