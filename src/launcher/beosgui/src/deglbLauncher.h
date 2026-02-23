/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_LAUNCHER_H_
#define _DEGLB_LAUNCHER_H_

#include "deglbSignalHandler.h"
#include "config/deglbConfiguration.h"

#include <delauncher/delLauncher.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/icon/delGameIcon.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>

class deglbWindowMain;


/**
 * \brief Launcher.
 */
class deglbLauncher : public delLauncher{
private:
	deglbSignalHandler pSignalHandler;
	deglbConfiguration pConfiguration;
	
	deglbWindowMain *pWindowMain;
	
	decUnicodeArgumentList pArguments;
	delGame::Ref pCmdLineGame;
	
	decString pRunGame;
	decUnicodeArgumentList pRunGameArgList;
	decString pRunProfileName;
	
	bool pCmdLineQuitNow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create launcher. */
	deglbLauncher(deglbWindowMain *windowMain, int argc, char **argv);
	
	/** \brief Clean up launcher. */
	~deglbLauncher() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{ return pArguments; }
	
	/** \brief Configuration. */
	inline deglbConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const deglbConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** \brief Game has to be run due to command line arguments. */
	bool HasCommandLineRunGame() const;
	
	/** \brief Run game from command line. Returns true to start application loop or false to exit. */
	bool RunCommandLineGame();
	
	/**
	 * \brief Run game from command line stop check.
	 * 
	 * Called when a game stops. Checks if the last game stopped running and shuts down
	 * the application if run from the command line.
	 */
	void RunCommandLineGameStopCheck();
	
	/** \brief Pulse checking. */
	void PulseChecking();
	
	/** \brief Quit now requested from the command line. */
	inline bool GetCmdLineQuitNow() const{ return pCmdLineQuitNow; }
	
	
	
	/** \brief Create game icon instance. */
	delGameIcon::Ref CreateGameIcon(int size, const char *path) override;
	/*@}*/
	
	
	
private:
	void pParseArguments();
	bool pParseWindowsURIScheme();
	static decString pUrlDecode(const char *url);
};

#endif
