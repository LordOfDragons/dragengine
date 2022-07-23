/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLLAUNCHER_H_
#define _DECLLAUNCHER_H_

#include "logger/declLoggerFiltered.h"

#include <delauncher/delLauncher.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/logger/deLogger.h>

#ifdef OS_BEOS
#include <delauncher/game/delGame.h>
#endif

class declConfiguration;


/**
 * Main Launcher Class.
 */
class declLauncher : public delLauncher{
private:
	decUnicodeArgumentList pArgList;
	declConfiguration *pConfiguration;
	deLogger::Ref pEngineLogger;
	deLogger::Ref pEngineLoggerDebug;
	
	decString pXMLFile;
	decUnicodeArgumentList pGameArgs;
	decString pWindowTitle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	declLauncher();
	
	/** Clean up launcher. */
	virtual ~declLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArgumentList() const{ return pArgList; }
	
	/** Add argument to the command line argument list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** Configuration. */
	inline declConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Engine logger. */
	inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
	
	/** Engine logger debug. */
	inline const deLogger::Ref &GetEngineLoggerDebug() const{ return pEngineLoggerDebug; }
	
	/** Print syntax. */
	void PrintSyntax();
	
	/** Init. */
	void Init();
	
	/** Run. */
	int Run();
	
	/** Clean up. */
	void CleanUp();
	
	/** Read input from user. */
	decString ReadInput() const;
	
	/** Read input from user returning true if 'y' or 'Y'. */
	bool ReadInputConfirm() const;
	
	/** Read input from user converting to integer number if valid else -1. */
	int ReadInputSelection() const;
	
#ifdef OS_BEOS
	/** Running game or nullptr. */
	delGame::Ref runningGame;
#endif
	/*@}*/
	
	
	
private:
	void pInitLogger();
};

#endif
