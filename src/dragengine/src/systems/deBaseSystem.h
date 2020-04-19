/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASESYSTEM_H_
#define _DEBASESYSTEM_H_

#include "../common/math/decMath.h"
#include "../common/string/decString.h"
#include "deModuleSystem.h"

class deEngine;
class deLoadableModule;
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
class deBaseSystem{
private:
	char *pSystemName;
	deEngine *pEngine;
	deLoadableModule *pActiveLoadableModule;
	int pRequiredModuleType;
	bool pRunning;
	bool pFailed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new base system linked to the given engine. */
	deBaseSystem( deEngine *engine, const char *systemName, int requiredModuleType );
	
	/** \brief Clean up base system. */
	virtual ~deBaseSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Linked game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Active loadable module. */
	inline deLoadableModule *GetActiveLoadableModule() const{ return pActiveLoadableModule; }
	
	/** \brief System is running. */
	inline bool GetIsRunning() const{ return pRunning; }
	
	/** \brief System has failed during runtime. */
	inline bool GetHasFailed() const{ return pFailed; }
	
	/** \brief System name. */
	inline const char *GetSystemName() const{ return ( const char * )pSystemName; }
	
	/**
	 * Determines if the system can be started up or not. A system is considered
	 * to be startable if an active module has been selected and it is loaded.
	 */
	virtual bool CanStart();
	
	/** \brief Set if system has failed. */
	void SetHasFailed( bool hasFailed );
	
	/** \brief Start system and the active loadable module. */
	virtual void Start();
	
	/** \brief Stop system and the active loadable module. */
	virtual void Stop();
	
	/** \brief Check if at least one module is working and select the first one. */
	void CheckAndActivateFirst( deModuleSystem::eModuleTypes type );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
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
	void LogInfo( const char *message );
	
	/** \brief Output formated information message on the console. */
	void LogInfoFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated information message on the console. */
	void LogInfoFormatUsing( const char *message, va_list args );
	
	/** \brief Output warning message on the console. */
	void LogWarn( const char *message );
	
	/** \brief Output formated warning message on the console. */
	void LogWarnFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated warning message on the console. */
	void LogWarnFormatUsing( const char *message, va_list args );
	
	/** \brief Output error message on the console. */
	void LogError( const char *message );
	
	/** \brief Output formated error message on the console. */
	void LogErrorFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated error message on the console. */
	void LogErrorFormatUsing( const char *message, va_list args );
	
	/** \brief Output exception as error message on the console. */
	void LogException( const deException &exception );
	/*@}*/
};

#endif
