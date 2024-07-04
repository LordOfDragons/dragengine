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

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/modules/service/deBaseServiceModule.h>

class decBaseFileReader;


/**
 * Mod.io Service Module.
 */
class deModio : public deBaseServiceModule{
private:
	int pRequiresEventHandlingCount;
	deVirtualFileSystem::Ref pVFSMods;
	decStringSet pDisabledMods;
	decObjectList pModConfigs;
	decObjectDictionary pModVFSContainers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deModio( deLoadableModule &loadableModule );
	
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
	void SetModConfigs( const decObjectList &configs );
	
	/** Disabled modifications. */
	inline const decStringSet &GetDisabledMods() const{ return pDisabledMods; }
	void SetDisabledMods( const decStringSet &mods );
	
	/** Mod disabled. */
	bool GetModDisabled( const decString &id ) const;
	void SetModDisabled( const decString &id, bool disabled );
	
	/** Active modifications changed. Update VFS and save configuration. */
	void ActiveModsChanged();
	
	/** Mod VFS container or nullptr. */
	deVFSContainer *GetModVFSContainer( const decString &id ) const;
	
	
	
	/**
	 * Create service peer.
	 * 
	 * If service name is not supported nullptr is returned.
	 */
	deBaseServiceService *CreateService( deService *service,
		const char *name, const deServiceObject::Ref &data ) override;
	
	/** Frame update. */
	void FrameUpdate( float elapsed ) override;
	
	/** Make module add stage specific VFS containers. */
	void AddVFSContainers( deVirtualFileSystem &vfs, const char *stage ) override;
	
	void AddRequiresEventHandlingCount();
	void RemoveRequiresEventHandlingCount();
	/*@}*/
	
	
	
private:
	void pLoadConfig();
	void pLoadConfigV0( decBaseFileReader &reader );
	void pSaveConfig();
	void pDeleteConfig();
	void pUpdateVFS();
};

#endif
