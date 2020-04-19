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

#ifndef _DECRASHRECOVERYSYSTEM_H_
#define _DECRASHRECOVERYSYSTEM_H_

#include "deBaseSystem.h"

class deLoadableModule;
class deBaseCrashRecoveryModule;


/**
 * \brief Crash Recovery System.
 *
 * Provides modules for handling errors in one of the single type engine systems
 * during run time. Such modules take over the run time during an error and try
 * to reach a working state again so the engine can continue running.
 */
class deCrashRecoverySystem : public deBaseSystem{
private:
	deBaseCrashRecoveryModule *pActiveModule;
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new crash recovery system linked to the given engine. */
	deCrashRecoverySystem( deEngine *engine );
	
	/** \brief Clean up physics system. */
	~deCrashRecoverySystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deBaseCrashRecoveryModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Yield control to the active crash recovery module. */
	bool RecoverFromError();
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
};

#endif
