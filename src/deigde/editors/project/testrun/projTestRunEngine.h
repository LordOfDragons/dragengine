/* 
 * DEIGDE Project
 *
 * Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is projributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _PROJTESTRUNENGINE_H_
#define _PROJTESTRUNENGINE_H_

#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/systems/deModuleSystem.h>

class projTestRunProcess;
class deEngine;



/**
 * \brief Game engine handling for test running.
 */
class projTestRunEngine{
private:
	projTestRunProcess &pProcess;
	
	deEngine *pEngine;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine. */
	projTestRunEngine( projTestRunProcess &process );
	
	/** \brief Clean up engine. */
	~projTestRunEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Share path. */
	inline const decString &GetPathShare() const{ return pPathShare; }
	
	/** \brief Library path. */
	inline const decString &GetPathLib() const{ return pPathLib; }
	
	/** \brief Start engine. */
	void Start();
	
	/** \brief Put engine into virtual file system. */
	void PutIntoVFS();
	
	/** \brief Activate modules. */
	void ActivateModules();
	
	/** \brief Activate module. */
	void ActivateModule( deModuleSystem::eModuleTypes type, const char *name, const char *version = "" );
	
	/** \brief Set data directory. */
	void SetDataDirectory();
	
	/** \brief Set run arguments. */
	void SetRunArguments();
	
	/** \brief Init virtual file system. */
	void InitVFS();
	
	/** \brief Add disk directory. */
	void VFSAddDiskDir( const char *pathRoot, const char *pathDisk, bool readOnly );
	
	/** \brief Create main window. */
	void CreateMainWindow();
	
	/** \brief Run. */
	void Run();
	
	/** \brief Stop engine. */
	void Stop();
	/*@}*/
	
	
	
private:
};

#endif
