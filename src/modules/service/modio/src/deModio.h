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

#ifndef _DEMODIO_H_
#define _DEMODIO_H_

#include "modio.h"
#include "parameters/deModioParameterList.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/modules/service/deBaseServiceModule.h>

class decBaseFileReader;
class deModioUserConfig;
class deMPLogLevel;

/**
 * Mod.io Service Module.
 */
class deModio : public deBaseServiceModule{
private:
	int pRequiresEventHandlingCount;
	deVirtualFileSystem::Ref pVFSMods;
	decObjectDictionary pUserConfigs;
	decObjectList pModConfigs, pActivateConfigs;
	decString pCurUserId;
	
	deModioParameterList pParameters;
	deMPLogLevel *pParamLogLevel;
	
	decPath pPathConfig, pPathFailureState;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deModio(deLoadableModule &loadableModule);
	
	/** Delete module. */
	~deModio() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Set of supported service names. */
	decStringSet GetSupportedServices() override;
	
	/** Mods virtual file system. */
	inline const deVirtualFileSystem::Ref &GetVFSMods() const{ return pVFSMods; }
	
	/** Mod configurations (deModioModConfig). */
	inline const decObjectList &GetModConfigs() const{ return pModConfigs; }
	void SetModConfigs(const decObjectList &configs);
	
	/** User configurations (deModioUserConfig). */
	inline decObjectDictionary &GetUserConfigs(){ return pUserConfigs; }
	inline const decObjectDictionary &GetUserConfigs() const{ return pUserConfigs; }
	
	/** User configuration with ID or nullptr if not found. */
	deModioUserConfig *GetUserConfigIfPresent(const decString &id) const;
	
	/** User configuration with ID creating it if not found. */
	deModioUserConfig &GetUserConfig(const decString &id);
	
	/** Current user id. */
	inline const decString &GetCurUserId() const{ return pCurUserId; }
	
	/** Set current user id. Create user if not existing. */
	void SetCurUserId(const decString &id);
	
	/** User config changed. Save configuration. */
	void UserConfigChanged();
	
	/** Activate modifications. Sets current user, updates VFS and saves configuration. */
	void ActivateMods(const decString &userId);
	
	/** Active modifications (deModioUserConfig). */
	inline const decObjectList &GetActiveMods() const{ return pActivateConfigs; }
	
	/** Log level module parameter. */
	inline const deMPLogLevel &GetParamLogLevel() const{ return *pParamLogLevel; }
	
	
	/**
	 * Create service peer.
	 * 
	 * If service name is not supported nullptr is returned.
	 */
	deBaseServiceService *CreateService(deService *service,
		const char *name, const deServiceObject::Ref &data) override;
	
	/** Frame update. */
	void FrameUpdate(float elapsed) override;
	
	/** Make module add stage specific VFS containers. */
	void AddVFSContainers(deVirtualFileSystem &vfs, const char *stage) override;
	
	void AddRequiresEventHandlingCount();
	void RemoveRequiresEventHandlingCount();
	
	/** Store failure state. */
	void StoreFailureStateIfFailed();
	
	/** Clear failure state. */
	void ClearFailureState();
	/*@}*/
	
	
	/** \name Parameters */
	/*@{*/
	/** Number of parameters. */
	int GetParameterCount() const override;
	
	/**
	 * Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	void GetParameterInfo(int index, deModuleParameter &parameter) const override;
	
	/** Index of named parameter or -1 if not found. */
	int IndexOfParameterNamed(const char *name) const override;
	
	/** Value of named parameter. */
	decString GetParameterValue(const char *name) const override;
	
	/** Set value of named parameter. */
	void SetParameterValue(const char *name, const char *value) override;
	/*@}*/
	
	
	
private:
	void pInitPath();
	void pLoadConfig();
	void pLoadConfigV0(decBaseFileReader &reader);
	void pSaveConfig();
	void pDeleteConfig();
	void pCheckFailureState();
	bool pReadFailureState();
	void pUpdateVFS();
};

#endif
