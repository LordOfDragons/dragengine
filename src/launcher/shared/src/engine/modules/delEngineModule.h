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

#ifndef _DELENGINEMODULE_H_
#define _DELENGINEMODULE_H_

#include "parameter/delEMParameterList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/systems/deModuleSystem.h>


class delLauncher;
class deInternalModule;


/**
 * \brief Game Engine Module.
 */
class DE_DLL_EXPORT delEngineModule : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delEngineModule> Ref;
	
	
public:
	/** \brief Module Status. */
	enum eModuleStatus{
		/** \brief Everything is okay. */
		emsReady,
		
		/** \brief Not tested yet. */
		emsNotTested,
		
		/** \brief Some kind of problem. */
		emsBroken
	};
	
	
	
private:
	deModuleSystem::eModuleTypes pType;
	decString pName;
	decUnicodeString pDescription;
	decUnicodeString pAuthor;
	decString pVersion;
	decString pDirName;
	decString pPattern;
	int pPriority;
	bool pIsFallback;
	eModuleStatus pStatus;
	int pErrorCode;
	
	decString pLibFileName;
	int pLibFileSizeShould;
	int pLibFileSizeIs;
	decString pLibFileHashShould;
	decString pLibFileHashIs;
	decString pLibFileEntryPoint;
	
	delEMParameterList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine module. */
	delEngineModule();
	
	/** \brief Create engine module from internal module. */
	delEngineModule(const deInternalModule &module);
	
protected:
	/** \brief Clean up engine module. */
	virtual ~delEngineModule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module type. */
	inline deModuleSystem::eModuleTypes GetType() const{ return pType; }
	
	/** \brief Set module type. */
	void SetType(deModuleSystem::eModuleTypes type);
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decUnicodeString &GetDescription () const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const decUnicodeString &description);
	
	/** \brief Author. */
	inline const decUnicodeString &GetAuthor() const{ return pAuthor; }
	
	/** \brief Set author. */
	void SetAuthor(const decUnicodeString &author);
	
	/** \brief Version. */
	inline const decString &GetVersion() const{ return pVersion; }
	
	/** \brief Set version. */
	void SetVersion(const char *version);
	
	/** \brief Directory name. */
	inline const decString &GetDirectoryName() const{ return pDirName; }
	
	/** \brief Set directory name. */
	void SetDirectoryName(const char *dirname);
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set pattern. */
	void SetPattern(const char *pattern);
	
	/**
	 * \brief Priority of module.
	 * 
	 * Used to find best module. Higher priority takes precedence.
	 */
	inline int GetPriority() const{ return pPriority; }
	
	/**
	 * \brief Set module priority.
	 * 
	 * Used to find best module. Higher priority takes precedence.
	 */
	void SetPriority(int priority);
	
	/** \brief Fallback module. */
	inline bool GetIsFallback() const{ return pIsFallback; }
	
	/** \brief Set if fallback module. */
	void SetIsFallback(bool isFallback);
	
	/** \brief Module status. */
	inline eModuleStatus GetStatus() const{ return pStatus; }
	
	/** \brief Set module status. */
	void SetStatus(eModuleStatus status);
	
	/** \brief Module error code. */
	inline int GetErrorCode() const{ return pErrorCode; }
	
	/** \brief Set module error code. */
	void SetErrorCode(int errorCode);
	
	/** \brief Library file name. */
	inline const decString &GetLibFileName() const{ return pLibFileName; }
	
	/** \brief Set library file name. */
	void SetLibFileName(const char *name);
	
	/** \brief Size in bytes the library file should have. */
	inline int GetLibFileSizeShould() const{ return pLibFileSizeShould; }
	
	/** \brief Set he size in bytes the library file should have. */
	void SetLibFileSizeShould(int size);
	
	/** \brief Size in bytes the library file actually has. */
	inline int GetLibFileSizeIs() const{ return pLibFileSizeIs; }
	
	/** \brief Set size in bytes the library file actually has. */
	void SetLibFileSizeIs(int size);
	
	/** \brief Hash value the library file should have. */
	inline const decString &GetLibFileHashShould() const{ return pLibFileHashShould; }
	
	/** \brief Set hash value the library file should have. */
	void SetLibFileHashShould(const char *hash);
	
	/** \brief Hash value the library file actually has. */
	inline const decString &GetLibFileHashIs() const{ return pLibFileHashIs; }
	
	/** \brief Set hash value the library file actually has. */
	void SetLibFileHashIs(const char *hash);
	
	/** \brief Entry point name. */
	inline const decString &GetLibFileEntryPoint() const{ return pLibFileEntryPoint; }
	
	/** \brief Set entry point name. */
	void SetLibFileEntryPoint(const char *name);
	
	/** \brief Parameters. */
	inline delEMParameterList &GetParameters(){ return pParameters; }
	inline const delEMParameterList &GetParameters() const{ return pParameters; }
	
	/** \brief Calculate file size and hashes. */
	void CalcSizeAndHashes(delLauncher &launcher);
	/*@}*/
};

#endif
