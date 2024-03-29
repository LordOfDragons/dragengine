/* 
 * Drag[en]gine Shared Vulkan
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

#ifndef _DESHAREDVULKAN_H_
#define _DESHAREDVULKAN_H_

#include "devkBasics.h"
#include "devkInstance.h"

#include <dragengine/deObject.h>
#include <dragengine/common/file/decPath.h>

class deBaseModule;
class devkLoader;


/**
 * Shared Vulkan.
 */
class deSharedVulkan : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deSharedVulkan> Ref;
	
	
	
private:
	deBaseModule &pModule;
	devkLoader *pLoader;
	
	devkInstance::Ref pInstance;
	
	decPath pCachePath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * Create shared vulkan. If loading the vulkan library fails an exception is thrown.
	 */
	#ifdef WITH_DEBUG
	deSharedVulkan( deBaseModule &module, bool enableDebug = true );
	#else
	deSharedVulkan( deBaseModule &module, bool enableDebug = false );
	#endif
	
protected:
	/** Clean up shared vulkan. */
	virtual ~deSharedVulkan();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Owner module. */
	inline deBaseModule &GetModule() const{ return pModule; }
	
	/** Instance. */
	inline devkInstance &GetInstance() const{ return pInstance; }
	
	/** Cache path. */
	inline const decPath &GetCachePath() const{ return pCachePath; }
	
	/** Set cache path. */
	void SetCachePath( const decPath &path );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

