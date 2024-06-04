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

#ifndef _DEALCONFIGURATION_H_
#define _DEALCONFIGURATION_H_

#include "dealConfigWindow.h"

#include "../common/string/decString.h"

class dealLauncher;



/**
 * \brief Launcher configuration.
 */
class dealConfiguration{
private:
	dealLauncher &pLauncher;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathLogs;
	
	dealConfigWindow pWindowMain;
	
	int pClrValidBack;
	int pClrValidText;
	int pClrProblemBack;
	int pClrProblemText;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	dealConfiguration( dealLauncher &launcher );
	
	/** \brief Clean up configuration. */
	~dealConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief System config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	
	/** \brief Set system config path. */
	void SetPathConfigSystem( const char *path );
	
	/** \brief User config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	
	/** \brief Set user config path. */
	void SetPathConfigUser( const char *path );
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	
	/** \brief Set shares path. */
	void SetPathShares( const char *path );
	
	/** \brief Logs path. */
	inline const decString &GetPathLogs() const{ return pPathLogs; }
	
	/** \brief Set logs path. */
	void SetPathLogs( const char *path );
	
	
	
	/** \brief Locate path. */
	void LocatePath();
	
	/** \brief Initialize virtual file system. */
	void InitVirtualFileSystem();
	
	/** \brief Log some important value. */
	void LogImportantValues();
	
	/**
	 * \brief Load the configuration.
	 * \details Tries to locate the configuration file in the known places reading first
	 *          the system wide configuration and then the user configuration if found.
	 *          For all not found configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * \brief Save configuration to the user configuration directory.
	 * \details If the directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	
	/** \brief Main window configuration. */
	inline dealConfigWindow &GetWindowMain(){ return pWindowMain; }
	inline const dealConfigWindow &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Background color for valid elements. */
	inline int GetBackColorValid() const{ return pClrValidBack; }
	
	/** \brief Background color for problem elements. */
	inline int GetBackColorProblem() const{ return pClrProblemBack; }
	
	/** \brief Text color for valid elements. */
	inline int GetTextColorValid() const{ return pClrValidText; }
	
	/** \brief Text color for problem elements. */
	inline int GetTextColorProblem() const{ return pClrProblemText; }
	/*@}*/
	
private:
};

#endif
