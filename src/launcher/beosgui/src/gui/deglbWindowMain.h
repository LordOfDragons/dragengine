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

#ifndef _DEGLB_WINDOWMAIN_H_
#define _DEGLB_WINDOWMAIN_H_

#include <Window.h>
#include <MenuBar.h>
#include <StringView.h>
#include <StatusBar.h>
#include <MessageRunner.h>

class deException;
class decUnicodeString;
class deglbLauncher;
class deglbPanelGames;
class deglbPanelEngine;
class deglbWindowLogger;


/**
 * \brief Main Window.
 */
class deglbWindowMain : public BWindow{
public:
	enum eMessages{
		MSG_FILE_QUIT = 'fqit',
		MSG_VIEW_GAMES = 'vgam',
		MSG_VIEW_ENGINE = 'venp',
		MSG_VIEW_LOGGING = 'vlog',
		MSG_SETTINGS_ENGINE = 'seng',
		MSG_SETTINGS_ENGINE_DONE = 'senG',
		MSG_TIMER_PULSE = 'tpls'
	};
	
	
private:
	deglbLauncher *pLauncher;
	
	BMenuBar *pMenuBar;
	
	deglbPanelGames *pPanelGames;
	deglbPanelEngine *pPanelEngine;
	
	BStringView *pStatusText;
	BStatusBar *pProgressBar;
	
	deglbWindowLogger *pWindowLogger;
	
	BMessageRunner *pPulseRunner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window. */
	deglbWindowMain(int argc, char **argv);
	
	/** \brief Clean up main window. */
	~deglbWindowMain() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline deglbLauncher *GetLauncher() const{ return pLauncher; }
	
	/** \brief Run command line actions. Returns true to start the application loop or false to exit. */
	bool RunCommandLineActions();
	
	/** \brief Display exception error. */
	void DisplayException(const deException &exception);
	
	/** \brief Games panel. */
	inline deglbPanelGames *GetPanelGames() const{ return pPanelGames; }
	
	/** \brief Engine panel. */
	inline deglbPanelEngine *GetPanelEngine() const{ return pPanelEngine; }
	
	/** \brief Logger window or nullptr if not visible. */
	inline deglbWindowLogger *GetWindowLogger() const{ return pWindowLogger; }
	
	/**
	 * \brief Show logger window.
	 *
	 * If not existing yet it is created otherwise it is focused and raised ontop of all windows.
	 */
	void ShowWindowLogger();
	
	/** \brief Set visibility of the progress bar in the status bar. */
	void SetProgressVisible(bool visible);
	
	/** \brief Set progress bar progress. */
	void SetProgress(float progress);
	
	/** \brief Set progress text. */
	void SetProgressText(const decUnicodeString &text);
	
	/** \brief Reload games and patches. */
	void ReloadGamesAndPatches();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	bool QuitRequested() override;
	/*@}*/
};

#endif
