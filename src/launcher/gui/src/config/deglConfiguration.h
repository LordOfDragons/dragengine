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

#ifndef _DEGLCONFIGURATION_H_
#define _DEGLCONFIGURATION_H_

#include "deglConfigWindow.h"
#include "../gui/foxtoolkit.h"

#include <dragengine/common/string/decString.h>

class deglLauncher;



/**
 * Launcher Configuration.
 */
class deglConfiguration{
private:
	deglLauncher &pLauncher;
	bool pCanSave;
	
	deglConfigWindow pWindowMain;
	
	FXColor pClrValidBack;
	FXColor pClrValidText;
	FXColor pClrProblemBack;
	FXColor pClrProblemText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create configuration. */
	deglConfiguration( deglLauncher &launcher );
	
	/** Clean up configuration. */
	~deglConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Load the configuration. Tries to locate the configuration file in the known places
	 * reading first the system wide configuration and then the user configuration if found.
	 * For all not found configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * Saves the configuration to the user configuration directory. If the directory or
	 * files do not exist they are created.
	 */
	void SaveConfiguration();
	
	/** Main window configuration. */
	inline deglConfigWindow &GetWindowMain(){ return pWindowMain; }
	inline const deglConfigWindow &GetWindowMain() const{ return pWindowMain; }
	
	/** Background color for valid elements. */
	inline FXColor GetBackColorValid() const{ return pClrValidBack; }
	
	/** Background color for problem elements. */
	inline FXColor GetBackColorProblem() const{ return pClrProblemBack; }
	
	/** Text color for valid elements. */
	inline FXColor GetTextColorValid() const{ return pClrValidText; }
	
	/** Text color for problem elements. */
	inline FXColor GetTextColorProblem() const{ return pClrProblemText; }
	/*@}*/
};

#endif
