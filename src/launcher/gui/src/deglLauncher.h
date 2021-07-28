/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLLAUNCHER_H_
#define _DEGLLAUNCHER_H_

#include "deglSignalHandler.h"
#include "config/deglConfiguration.h"

#include <delauncher/delLauncher.h>
#include <delauncher/game/delGame.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>

class deglConfiguration;
class deglWindowMain;


/**
 * Launcher.
 */
class deglLauncher : public delLauncher{
private:
	deglSignalHandler pSignalHandler;
	deglConfiguration pConfiguration;
	
	deglWindowMain *pWindowMain;
	
	decUnicodeArgumentList pArguments;
	delGame::Ref pCmdLineGame;
	
	decString pRunGame;
	decUnicodeArgumentList pRunGameArgList;
	decString pRunProfileName;
	decString pCmdLineInstallDelga;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	deglLauncher( deglWindowMain *windowMain, int argc, char **argv );
	
	/** Clean up launcher. */
	virtual ~deglLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{ return pArguments; }
	
	/** Configuration. */
	inline deglConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const deglConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** \brief Install delga command line argument. */
	inline const decString &GetCommandLineInstallDelga() const{ return pCmdLineInstallDelga; }
	
	/** Game has to be run due to command line arguments. */
	bool HasCommandLineRunGame() const;
	
	/** Run game from command line. Returns true to start the application loop or false to exit */
	bool RunCommandLineGame();
	
	/**
	 * Run game from command line stop check. Called when a game stops. Checks if the last
	 * game stopped running and shuts down the application if run from the command line.
	 */
	void RunCommandLineGameStopCheck();
	
	/** Pulse checking. */
	void PulseChecking();
	
	
	/** Create game icon instance. */
	virtual delGameIcon *CreateGameIcon( int size, const char *path );
	/*@}*/
	
	
	
private:
	void pParseArguments();
	void pInitLogger();
};

#endif
