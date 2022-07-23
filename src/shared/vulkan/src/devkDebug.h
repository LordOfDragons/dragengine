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

#ifndef _DEVKDEBUG_H_
#define _DEVKDEBUG_H_

#include "devkBasics.h"

class devkInstance;


/**
 * Vulkan debug support.
 */
class devkDebug{
private:
	devkInstance &pInstance;
	
	bool pEnabled;
	
	VkDebugReportCallbackEXT pReportCallback;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug. */
	devkDebug( devkInstance &instance );
	
	/** Clean up debug. */
	~devkDebug();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline devkInstance &GetInstance() const{ return pInstance; }
	
	/** Debug mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Enable or disable debug mode. */
	void SetEnabled( bool enabled );
	
	
	
private:
	void pRegisterReportCallback();
	void pUnregisterReportCallback();
	
	void DebugMessage( const char* layerPrefix, const char* message );
	
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
		VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
		uint64_t object, size_t location, int32_t messageCode,
		const char* pLayerPrefix, const char* pMessage, void* pUserData );
};

#endif

