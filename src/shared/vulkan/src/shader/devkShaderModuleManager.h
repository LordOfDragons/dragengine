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

#ifndef _DEVKSHADERMODULEMANAGER_H_
#define _DEVKSHADERMODULEMANAGER_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>

class devkDevice;
class devkShaderModule;


/**
 * Shader module manager.
 */
class devkShaderModuleManager{
private:
	devkDevice &pDevice;
	
	decObjectList pSources;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader module manager. */
	devkShaderModuleManager( devkDevice &device );
	
	/** Clean up shader modules manager. */
	~devkShaderModuleManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of shader modules. */
	int GetCount() const;
	
	/** Shader module at index. Caller does not hold reference. */
	devkShaderModule *GetAt( int index ) const;
	
	/** Shader module with path or nullptr if absent. Caller does not hold reference. */
	devkShaderModule *GetWithPath( const char *path ) const;
	
	/** Shader module with path if present. */
	bool HasWithPath( const char *path ) const;
	
	/** Add shader module. */
	void Add( devkShaderModule *module );
	
	/** Remove all shader modules. */
	void Clear();
	
	/** Scan shader modules in directory. */
	void Load( const char *directory );
	/*@}*/
};

#endif
