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
	devkShaderModule(devkDevice &device, const char *path, decBaseFileReader &reader);
	
protected:
	/** Clean up shader module. */
	virtual ~devkShaderModule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{return pDevice;}
	
	/** Path. */
	inline const decString &GetPath() const{return pPath;}
	
	/** Shader source. */
	inline const char *GetSource() const{return pSource;}
	
	/** Shader source length. */
	inline int GetSourceLength() const{return pSourceLength;}
	
	/** Shader module. */
	inline VkShaderModule GetModule() const{return pModule;}
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

