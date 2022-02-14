/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLLAUNCHER_H_
#define _DELLLAUNCHER_H_

#include <delauncher/delLauncher.h>

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/logger/deLoggerReference.h>

class dellEngine;



/**
 * Live launcher.
 */
class dellLauncher{
public:
	class Launcher : public delLauncher{
	public:
		Launcher();
		virtual ~Launcher();
	};
	
	
	
private:
	decUnicodeArgumentList pArguments;
	decPath pWorkingDir;
	decPath pPathConfigUser;
	decStringList pEnvParamsStore;
	
	Launcher *pLauncher;
	
	decString pDelgaFile;
	decUnicodeArgumentList pGameArgs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	dellLauncher();
	
	/** Clean up launcher. */
	~dellLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{ return pArguments; }
	
	/** Add argument to the command line argument list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** Working directory. */
	inline const decPath GetWorkingDirectory() const{ return pWorkingDir; }
	
	/** Launcher. */
	inline Launcher &GetLauncher() const{ return *pLauncher; }
	
	/** Run. */
	void Run();
	/*@}*/
	
	
	
private:
	void pUpdateEnvironment();
};

#endif
