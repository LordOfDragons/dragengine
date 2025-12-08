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

#ifndef _DEBASESYSTEM_H_
#define _DEBASESYSTEM_H_

#include "../common/math/decMath.h"
#include "../common/string/decString.h"
#include "deModuleSystem.h"

class deEngine;
#include "modules/deLoadableModule.h"
class deException;


/**
 * \brief Base System Class.
 *
 * A system describes the behaviour and capabilities of a well defined
 * group of operations belonging to a specific set of tasks. There exist
 * two kinds of systems. The modules itself are handled by an external
 * class but each system also contains a list of modules usable with
 * that system. Except the scripting system all can be shut down and
 * restarted during engine-break time.
 *
 * \par Single Type Systems
 * This kind of systems can have only one module of their kind running
 * at the same time. This class provides the base for those kind of
 * systems. Those systems are also the only ones who can provide
 * peer objects for certain engine objects. Single type systems can also
 * be shut down and restarted during engine-break scenarios. The module
 * used by each system is defined at the beginning of execution and can
 * be changed during engine-break time.
 *
 * \par Multiple Type Systems
 * This kind of systems can have multiple modules running at the same
 * time. The deResourceManager class provides the base for systems of
 * that kind. Those system can not provide peer objects to engine objects.
 * This kind of systems is used for loading and saving file formats.
 * The modules used by those systems can be shut down and restarted
 * individually during engine-break time. What modules are used depends
 * on the file type to handle.
 */
class DE_DLL_EXPORT deBaseSystem{
private:
	const decString pSystemName;
	deEngine * const pEngine;
	deLoadableModule::Ref pActiveLoadableModule;
	const int pRequiredModuleType;
	bool pRunning;
	bool pFailed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new base system linked to the given engine. */
	deBaseSystem(deEngine *engine, const char *systemName, int requiredModuleType);
	
	/** \brief Clean up base system. */
	virtual ~deBaseSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Linked game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Active loadable module. */
	inline const deLoadableModule::Ref &GetActiveLoadableModule() const{ return pActiveLoadableModule; }
	
	/** \brief System is running. */
	inline bool GetIsRunning() const{ return pRunning; }
	
	/** \brief System has failed during runtime. */
	inline bool GetHasFailed() const{ return pFailed; }
	
	/** \brief System name. */
	inline const decString &GetSystemName() const{ return pSystemName; }
	
	/**
	 * Determines if the system can be started up or not. A system is considered
	 * to be startable if an active module has been selected and it is loaded.
	 */
	virtual bool CanStart();
	
	/** \brief Set if system has failed. */
	void SetHasFailed(bool hasFailed);
	
	/** \brief Start system and the active loadable module. */
	virtual void Start();
	
	/** \brief Stop system and the active loadable module. */
	virtual void Stop();
	
	/** \brief Check if at least one module is working and select the first one. */
	void CheckAndActivateFirst(deModuleSystem::eModuleTypes type);
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule(deLoadableModule *module);
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Output information message on the console. */
	void LogInfo(const char *message);
	
	/** \brief Output formated information message on the console. */
	void LogInfoFormat(const char *message, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated information message on the console. */
	void LogInfoFormatUsing(const char *message, va_list args);
	
	/** \brief Output warning message on the console. */
	void LogWarn(const char *message);
	
	/** \brief Output formated warning message on the console. */
	void LogWarnFormat(const char *message, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated warning message on the console. */
	void LogWarnFormatUsing(const char *message, va_list args);
	
	/** \brief Output error message on the console. */
	void LogError(const char *message);
	
	/** \brief Output formated error message on the console. */
	void LogErrorFormat(const char *message, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated error message on the console. */
	void LogErrorFormatUsing(const char *message, va_list args);
	
	/** \brief Output exception as error message on the console. */
	void LogException(const deException &exception);
	/*@}*/
};

#endif
