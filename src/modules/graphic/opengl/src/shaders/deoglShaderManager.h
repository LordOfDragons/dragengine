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

#ifndef _DEOGLSHADERMANAGER_H_
#define _DEOGLSHADERMANAGER_H_

#include "deoglShaderProgram.h"
#include "deoglShaderProgramUnit.h"
#include "compiler/deoglShaderCompileListener.h"

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deSemaphore.h>

class deoglRenderThread;
class deoglShaderUnitSourceCode;
class deoglShaderDefines;
class deoglShaderSources;
class deoglShaderLanguage;
class deVirtualFileSystem;
class decPath;


/**
 * Shader Manager.
 */
class deoglShaderManager{
public:
	class cGetProgramListener{
	public:
		cGetProgramListener() = default;
		virtual ~cGetProgramListener() = default;
		
		/** Get program finished. If compile shader failed program is nullptr. */
		virtual void GetProgramFinished(const deoglShaderProgram *program) = 0;
		/*@}*/
	};
	
	
private:
	class cCompileProgram : public deoglShaderCompileListener{
	private:
		deoglShaderManager &pManager;
		deoglShaderProgram::Ref pProgram;
		decPointerList pListeners;
		
	public:
		cCompileProgram(deoglShaderManager &manager, const deoglShaderProgram::Ref &program);
		
		inline const deoglShaderProgram::Ref &GetProgram() const{return pProgram;}
		
		void AddListener(cGetProgramListener *listener);
		
		void CompileFinished(deoglShaderProgram *program) override;
		/*@}*/
	};
	
	friend class cCompileProgram;
	
	
	deoglRenderThread &pRenderThread;
	
	deoglShaderLanguage *pLanguage;
	
	decObjectDictionary pUnitSourceCodes, pSources;
	decStringDictionary pIncludableSources;
	decObjectList pPrograms, pProgramUnits;
	
	decPointerList pCompilePrograms;
	deMutex pMutexCompilePrograms, pMutexLogging;
	deSemaphore pSemaphoreCompileFinished;
	
	decString pPathShaderSources;
	decString pPathShaders;
	
	decStringList pCacheValidationString;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader manager object. */
	deoglShaderManager(deoglRenderThread &renderThread);
	
	/** Cleans up the shader manager object. */
	~deoglShaderManager();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Retrieves the language used to compiled and drive shaders. */
	inline deoglShaderLanguage *GetLanguage() const{return pLanguage;}
	
	/** Validate caches. */
	void ValidateCaches();
	
	/** Logging mutex. */
	inline deMutex &GetMutexLogging(){return pMutexLogging;}
	
	/** Update. */
	void Update();
	/*@}*/
	
	
	/** \name Unit Source Codes */
	/*@{*/
	/** Count of shader unit source codes. */
	int GetUnitSourceCodeCount() const;
	
	/** Named shader unit source exists. */
	bool HasUnitSourceCodeNamed(const char *name) const;
	
	/** Named shader unit source code or nullptr. */
	deoglShaderUnitSourceCode *GetUnitSourceCodeNamed(const char *name) const;
	
	/** Removes all shader unit source codes. */
	void RemoveAllUnitSourceCodes();
	
	/**
	 * Scan shader unit source code directory for files and loads them.
	 * \details Main thread call. Uses game engine logger to log problems.
	 */
	void LoadUnitSourceCodes();
	
	/** Includable sources. */
	inline const decStringDictionary &GetIncludableSources() const{return pIncludableSources;}
	/*@}*/
	
	
	/** \name Sources */
	/*@{*/
	/** Count of shader sources. */
	int GetSourcesCount() const;
	
	/** Named shader sources is exists. */
	bool HasSourcesNamed(const char *name) const;
	
	/** Named shader sources or nullptr. */
	const deoglShaderSources *GetSourcesNamed(const char *name);
	
	/** Shader sources as list for debugging. */
	decObjectList GetSourcesAsList() const;
	
	/**
	 * Scan shader directory for shader files and loads them.
	 * \details Main thread call. Uses game engine logger to log problems.
	 */
	void LoadSources();
	/*@}*/
	
	
	/** \name Program Units */
	/*@{*/
	/** Count of program units. */
	int GetProgramUnitCount();
	
	/** Program unit at index. */
	deoglShaderProgramUnit *GetProgramUnitAt(int index);
	/*@}*/
	
	
	/** \name Programs */
	/*@{*/
	/** Count of programs. */
	int GetProgramCount();
	
	/** Program at index. */
	const deoglShaderProgram *GetProgramAt(int index);
	
	/** Program with sources and defines. If absent it is first loaded and compiled. */
	const deoglShaderProgram *GetProgramWith(const deoglShaderSources *sources,
		const deoglShaderDefines &defines);
	
	/** Asynchronous GetProgramWith(). */
	void GetProgramWithAsync(const deoglShaderSources *sources,
		const deoglShaderDefines &defines, cGetProgramListener *listener);
	
	/** Wait for all programs to finish compile. */
	void WaitAllProgramsCompiled();
	
	/** Resolve units in program. */
	void ResolveProgramUnits(deoglShaderProgram &program);
	/*@}*/
	
	
private:
	void pResolveProgramUnitsLocked(deoglShaderProgram &program);
	
	deoglShaderProgramUnit *pGetProgramUnitWithLocked(
		const char *name, const deoglShaderDefines &defines);
	
	deoglShaderProgram *pGetProgramWith(const deoglShaderSources *sources,
		const deoglShaderDefines &defines) const;
	
	cCompileProgram *pGetCompilingWith(const deoglShaderSources *sources,
		const deoglShaderDefines &defines) const;
	
	deoglShaderProgram::Ref pCreateProgram(const deoglShaderSources *sources,
		const deoglShaderDefines &defines);
	
	deoglShaderProgramUnit *pGetProgramUnitWith(const deoglShaderUnitSourceCode *sources,
		const deoglShaderDefines &defines) const;
	
	void pLoadUnitSourceCodesIn(const char *directory);
	void pLoadIncludableSourcesIn(const char *directory);
	void pLoadSourcesIn(const char *directory);
};

#endif
