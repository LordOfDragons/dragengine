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

#ifndef _DEGLB_CONFIGURATION_H_
#define _DEGLB_CONFIGURATION_H_

#include "deglbConfigWindow.h"

#include <interface/GraphicsDefs.h>
#include <dragengine/common/string/decString.h>

class deglbLauncher;


/**
 * \brief Launcher Configuration.
 */
class deglbConfiguration{
private:
	deglbLauncher &pLauncher;
	bool pCanSave;
	
	deglbConfigWindow pWindowMain;
	
	rgb_color pClrValidBack, pClrValidText;
	rgb_color pClrProblemBack, pClrProblemText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	explicit deglbConfiguration(deglbLauncher &launcher);
	
	/** \brief Clean up configuration. */
	~deglbConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Load the configuration.
	 *
	 * Tries to locate the configuration file in the known places reading first the system
	 * wide configuration and then the user configuration if found. For all not found
	 * configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * \brief Save configuration to the user configuration directory.
	 *
	 * If the directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	
	/** \brief Main window configuration. */
	inline deglbConfigWindow &GetWindowMain(){ return pWindowMain; }
	inline const deglbConfigWindow &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Background color for valid elements. */
	inline rgb_color GetBackColorValid() const{ return pClrValidBack; }
	
	/** \brief Background color for problem elements. */
	inline rgb_color GetBackColorProblem() const{ return pClrProblemBack; }
	
	/** \brief Text color for valid elements. */
	inline rgb_color GetTextColorValid() const{ return pClrValidText; }
	
	/** \brief Text color for problem elements. */
	inline rgb_color GetTextColorProblem() const{ return pClrProblemText; }
	/*@}*/
};

#endif
