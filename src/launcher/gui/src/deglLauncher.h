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
	
	bool pCmdLineQuitNow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	deglLauncher(deglWindowMain *windowMain, int argc, char **argv);
	
	/** Clean up launcher. */
	virtual ~deglLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{return pArguments;}
	
	/** Configuration. */
	inline deglConfiguration &GetConfiguration(){return pConfiguration;}
	inline const deglConfiguration &GetConfiguration() const{return pConfiguration;}
	
	/** \brief Install delga command line argument. */
	inline const decString &GetCommandLineInstallDelga() const{return pCmdLineInstallDelga;}
	
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
	
	/** Quit now requested from the command line. */
	inline bool GetCmdLineQuitNow() const{return pCmdLineQuitNow;}
	
	
	
	/** Create game icon instance. */
	virtual delGameIcon *CreateGameIcon(int size, const char *path);
	/*@}*/
	
	
	
private:
	void pParseArguments();
	bool pParseWindowsURIScheme();
	void pInitLogger();
	static decString pUrlDecode(const char *url);
};

#endif
