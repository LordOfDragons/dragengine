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

#ifndef _DEVKSHADERMODULE_H_
#define _DEVKSHADERMODULE_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class devkDevice;
class decBaseFileReader;


/**
 * Vulkan shader module.
 */
class devkShaderModule : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkShaderModule> Ref;
	
	
	
private:
	devkDevice &pDevice;
	const decString pPath;
	char *pSource;
	int pSourceLength;
	
	VkShaderModule pModule;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader module. */
	devkShaderModule( devkDevice &device, const char *path, decBaseFileReader &reader );
	
protected:
	/** Clean up shader module. */
	virtual ~devkShaderModule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** Shader source. */
	inline const char *GetSource() const{ return pSource; }
	
	/** Shader source length. */
	inline int GetSourceLength() const{ return pSourceLength; }
	
	/** Shader module. */
	inline VkShaderModule GetModule() const{ return pModule; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

