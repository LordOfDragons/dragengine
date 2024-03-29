/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSHADERMANAGER_H_
#define _DEOGLSHADERMANAGER_H_

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglShaderUnitSourceCode;
class deoglShaderProgram;
class deoglShaderDefines;
class deoglShaderSources;
class deoglShaderLanguage;
class deVirtualFileSystem;
class decPath;



/**
 * Shader Manager.
 */
class deoglShaderManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglShaderLanguage *pLanguage;
	
	decObjectDictionary pUnitSourceCodes;
	decObjectDictionary pSources;
	decObjectList pPrograms;
	
	decString pPathShaderSources;
	decString pPathShaders;
	
	decStringList pCacheValidationString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader manager object. */
	deoglShaderManager( deoglRenderThread &renderThread );
	/** Cleans up the shader manager object. */
	~deoglShaderManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the language used to compiled and drive shaders. */
	inline deoglShaderLanguage *GetLanguage() const{ return pLanguage; }
	
	/** Validate caches. */
	void ValidateCaches();
	/*@}*/
	
	
	
	/** \name Unit Source Codes */
	/*@{*/
	/** Retrieves the number of shader unit source codes. */
	int GetUnitSourceCodeCount() const;
	
	/** Determines if a shader unit source code with the given file path exists. */
	bool HasUnitSourceCodeWithPath( const char *filePath ) const;
	
	/** Retrieves the shader unit source code with the given name or NULL if not found. */
	deoglShaderUnitSourceCode *GetUnitSourceCodeWithPath( const char *filePath ) const;
	
	/** Adds a shader unit source code. */
	void AddUnitSourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Removes all shader unit source codes. */
	void RemoveAllUnitSourceCodes();
	
	/**
	 * Scan shader unit source code directory for files and loads them.
	 * \details Main thread call. Uses game engine logger to log problems.
	 */
	void LoadUnitSourceCodes();
	/*@}*/
	
	
	
	/** \name Sources */
	/*@{*/
	/** Count of shader sources. */
	int GetSourcesCount() const;
	
	/** Named shader sources is exists. */
	bool HasSourcesNamed( const char *name ) const;
	
	/** Named shader sources or nullptr. */
	const deoglShaderSources *GetSourcesNamed( const char *name );
	
	/** Shader sources as list for debugging. */
	decObjectList GetSourcesAsList() const;
	
	/**
	 * Scan shader directory for shader files and loads them.
	 * \details Main thread call. Uses game engine logger to log problems.
	 */
	void LoadSources();
	/*@}*/
	
	
	
	/** \name Programs */
	/*@{*/
	/** Count of programs. */
	int GetProgramCount() const;
	
	/** Program at index. */
	const deoglShaderProgram *GetProgramAt( int index ) const;
	
	/** Program with sources and defines exists. */
	bool HasProgramWith( const deoglShaderSources *sources, const deoglShaderDefines &defines ) const;
	
	/** Program with sources and defines. If absent it is first loaded and compiled. */
	const deoglShaderProgram *GetProgramWith( const deoglShaderSources *sources, const deoglShaderDefines &defines );
	/*@}*/
	
	
	
private:
	void pLoadUnitSourceCodesIn( const char *directory );
	void pLoadSourcesIn( const char *directory );
};

#endif
