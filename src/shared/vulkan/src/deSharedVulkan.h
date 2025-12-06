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

