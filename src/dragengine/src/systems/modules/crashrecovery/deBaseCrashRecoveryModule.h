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

#ifndef _DEBASECRASHRECOVERYMODULE_H_
#define _DEBASECRASHRECOVERYMODULE_H_

#include "../deBaseModule.h"



/**
 * \brief Base Crash Recovery Module.
 *
 * Crash Recovery Modules are responsible for handling errors during the execution
 * of the game engine. If anyone of the single type modules fails the crash recovery
 * module is fired up to allow the user to locate the error and recover from it.
 * Errors in the crash recovery module itself lead to an immediate termination of
 * the engine hence this is the module to pay special attention to in terms of
 * error safeness.
 */
class DE_DLL_EXPORT deBaseCrashRecoveryModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module. */
	deBaseCrashRecoveryModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseCrashRecoveryModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize crash recovery module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down crash recovery module and cleans up. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Take over the run time in the case of an error.
	 * 
	 * The modules has to either provide its own event queues (in the case of ToolKits)
	 * or to operate the event queues of the deOS object itself. Once the engine has
	 * reached a state where execution can continue as before this function has to exit
	 * with a true value. If false is returned a non recoverable error is assumed and
	 * the engine is shut down gracefully.
	 */
	virtual bool RecoverFromError() = 0;
	/*@}*/
};

#endif
