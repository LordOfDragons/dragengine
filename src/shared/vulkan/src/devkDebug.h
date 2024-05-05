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

