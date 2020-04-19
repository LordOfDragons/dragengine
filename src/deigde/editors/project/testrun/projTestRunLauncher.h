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

#ifndef _PROJTESTRUNLAUNCHER_H_
#define _PROJTESTRUNLAUNCHER_H_

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif
#include <dragengine/filesystem/deVirtualFileSystemReference.h>

class projTestRunProcess;



/**
 * \brief Launcher handlign simulation for test runner.
 * 
 * Simulates functionality done by a true launcher.
 */
class projTestRunLauncher{
private:
	projTestRunProcess &pProcess;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	
	deVirtualFileSystemReference pVFS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create launcher simulation. */
	projTestRunLauncher( projTestRunProcess &process );
	
	/** \brief Clean up launcher simulation. */
	~projTestRunLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief System config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	
	/** \brief User config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	
	/** \brief Virtual file system. */
	inline deVirtualFileSystem* GetVFS() const{ return pVFS; }
	
	/** \brief Locate path. */
	void LocatePath();
	
	/** \brief Create virtual file system. */
	void CreateVFS();
	/*@}*/
	
	
	
private:
};

#endif
