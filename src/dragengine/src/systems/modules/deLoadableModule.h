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

#ifndef _DELOADABLEMODULE_H_
#define _DELOADABLEMODULE_H_

#include "../deModuleSystem.h"
#include "../../deObject.h"
#include "../../common/string/decStringList.h"

class deInternalModule;
class deLibraryModule;
class deLoadableModuleVisitor;
class deModuleSystem;
class deBaseModule;


/**
 * \brief Loadable Module Support.
 * 
 * Loadable modules are a wrapper around an engine module. They are used
 * to hold information about an engine module that is loadable from a
 * particular source. Loadable module objects can hold modules which have
 * loaded successfully or with an error. If an error occurred the reason
 * is stored for easy resolution finding. Modules can also be locked to
 * avoid unloading them while they are in use by some lengthy process.
 * Provided are two ways to work with subclasses one being a visitor and
 * the other safe casting.
 */
class DE_DLL_EXPORT deLoadableModule : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLoadableModule> Ref;


public:
	/** \brief Error Codes. */
	enum eErrorCodes{
		/** \brief Module library loaded successfullym and the module is created and ready for usage. */
		eecSuccess,
		
		/** \brief Creating module failed. */
		eecCreateModuleFailed
	};
	
	
	
private:
	deModuleSystem *pSystem;
	
	decString pLoggingName;
	
	decString pName;
	deModuleSystem::eModuleTypes pType;
	
	decString pDescription;
	decString pAuthor;
	decString pVersion;
	decString pDirName;
	
	decStringList pPatternList;
	decString pDefaultExtension;
	
	int pPriority;
	bool pFallback;
	bool pNoSaving;
	bool pNoCompress;
	
	bool pEnabled;
	
	deBaseModule *pModule;
	int pLockCount;
	
	int pErrorCode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new loadable module. */
	deLoadableModule( deModuleSystem *system );
	
protected:
	/** \brief Clean up loadable module. */
	virtual ~deLoadableModule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Linked module system. */
	inline deModuleSystem *GetSystem() const{ return pSystem; }
	
	/** \brief Name to use as the source for this module for logging. */
	inline const decString &GetLoggingName() const{ return pLoggingName; }
	
	/** \brief Set name to use as the source for this module for logging. */
	void SetLoggingName( const char *name );
	
	/** \brief Set default logging name which is "Module &lt;module-name&gt;". */
	void SetDefaultLoggingName();
	
	/** \brief Unique name of the module. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set unique name of the module. */
	void SetName( const char *name );
	
	/** \brief Type of the module. */
	inline deModuleSystem::eModuleTypes GetType() const{ return pType; }
	
	/** \brief Set type of the module. */
	void SetType( deModuleSystem::eModuleTypes type );
	
	/** \brief Description of the module. */
	inline const decString GetDescription() const{ return pDescription; }
	
	/** \brief Set description of the module. */
	void SetDescription( const char *description );
	
	/** \brief Author of the module. */
	inline const decString &GetAuthor() const{ return pAuthor; }
	
	/** \brief Set author of the module. */
	void SetAuthor( const char *author );
	
	/** \brief Version of the module. */
	inline const decString &GetVersion() const{ return pVersion; }
	
	/** \brief Set version of the module. */
	void SetVersion( const char *version );
	
	/** \brief File pattern list recognized by this module if a multiple type module. */
	inline decStringList &GetPatternList(){ return pPatternList; }
	inline const decStringList &GetPatternList() const{ return pPatternList; }
	
	/** \brief Default extension to be used for saving files if this is a multiple type module. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set default extension to be used for saving files if this is a multiple type module. */
	void SetDefaultExtension( const char *extension );
	
	/** \brief Directory name for this module. */
	inline const decString &GetDirectoryName() const{ return pDirName; }
	
	/** \brief Set directory name for this module. */
	void SetDirectoryName( const char *dirName );
	
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
	void SetPriority( int priority );
	
	/** \brief Determines if this module is a fallback module. */
	inline bool GetIsFallback() const{ return pFallback; }
	
	/** \brief Sets if this module is a fallback module. */
	void SetIsFallback( bool fallback );
	
	/** \brief Module does not support saving. */
	inline bool GetNoSaving() const{ return pNoSaving; }
	
	/** \brief Set if module does not support saving. */
	void SetNoSaving( bool noSaving );
	
	/**
	 * \brief Files are compressed.
	 * \version 1.12
	 */
	inline bool GetNoCompress() const{ return pNoCompress; }
	
	/**
	 * \brief Set if files are compressed.
	 * \version 1.12
	 */
	void SetNoCompress( bool noCompress );
	
	/**
	 * \brief Determines if the module is enabled.
	 * 
	 * Disabling a module can be used to prevent a higher version module
	 * from being automatically selected. This is especially the case
	 * with multiple type modules where the launcher does not set the
	 * module version to be used explicitely. By default modules are enabled.
	 */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/**
	 * \brief Sets if the module is enabled.
	 * 
	 * Disabling a module can be used to prevent a higher version module
	 * from being automatically selected. This is especially the case
	 * with multiple type modules where the launcher does not set the
	 * module version to be used explicitely. By default modules are enabled.
	 */
	void SetEnabled( bool enabled );
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Module if loaded or NULL. */
	inline deBaseModule *GetModule() const{ return pModule; }
	
	/** \brief Set module if loaded of NULL. */
	void SetModule( deBaseModule *module );
	
	/**
	 * \brief Loads the module from the appropriate source.
	 * 
	 * Sets the error code in both the case of success and failure and stored the module
	 * for later retrieval.
	 */
	virtual void LoadModule() = 0;
	
	/** \brief Unloads the module. Sets the module to NULL and clears the error code. */
	virtual void UnloadModule() = 0;
	
	/** \brief Determines if the module is loaded and working. */
	bool IsLoaded() const;
	
	/** \brief Error code from the last load attempt. */
	inline int GetErrorCode() const{ return pErrorCode; }
	
	/** \brief Set error code. */
	void SetErrorCode( int code );
	
	/** \brief Determines if the module is locked. */
	inline bool IsLocked() const{ return pLockCount > 0; }
	
	/** \brief Locks the module and increases the lock counter. */
	virtual void Lock();
	
	/** \brief Decreases the lock counter and unlocks the module if the counter reaches 0. */
	virtual void Unlock();
	/*@}*/
	
	
	
	/** \name Type Testing and Safe Casting */
	/*@{*/
	/** \brief Determines if this is an internal module. */
	virtual bool IsInternalModule() const;
	
	/** \brief Determines if this is a library loaded module. */
	virtual bool IsLibraryModule() const;
	
	/** \brief Cast to an internal module. */
	virtual deInternalModule *CastToInternalModule();
	
	/** \brief Cast to a library loaded rule. */
	virtual deLibraryModule *CastToLibraryModule();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit module. */
	virtual void Visit( deLoadableModuleVisitor &visitor );
	/*@}*/
	
	
	
private:
	void pLMCleanUp();
};

#endif
