/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENULLCRASHRECOVERYMODULE_H_
#define _IGDENULLCRASHRECOVERYMODULE_H_

#include <dragengine/systems/modules/deInternalModule.h>
#include <dragengine/systems/modules/crashrecovery/deBaseCrashRecoveryModule.h>



/**
 * \brief Null CrashRecovery Module.
 */
class igdeNullCrashRecoveryModule : public deBaseCrashRecoveryModule{
public:
	class cModule : public deInternalModule{
	public:
		cModule( deModuleSystem *system );
		virtual ~cModule();
		virtual void CreateModule();
	};
	
	
	
public:
	/** \name Constructor, destructor */
	/*@{*/
	/** \brief Create module. */
	igdeNullCrashRecoveryModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~igdeNullCrashRecoveryModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize module. */
	virtual bool Init();
	
	/** \brief Shut down module and cleans up. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Crash recovery. */
	virtual bool RecoverFromError();
	/*@}*/
};

#endif
