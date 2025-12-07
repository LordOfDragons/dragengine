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

#ifndef _SAECONFIGURATION_H_
#define _SAECONFIGURATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class saeWindowMain;



/**
 * Editor Configuration.
 */
class saeConfiguration{
private:
	saeWindowMain &pWindowMain;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration. */
	saeConfiguration(saeWindowMain &windowMain);
	
	/** Cleans up the configuration. */
	~saeConfiguration();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline saeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** Set prevent saving. */
	void SetPreventSaving(bool preventSaving);
	
	/**
	 * Load the configuration. Tries to locate the configuration file in
	 * the known places reading first the system wide configuration and
	 * then the user configuration if found. For all not found configuration
	 * values the default value is used.
	 */
	void LoadConfiguration();
	/**
	 * Saves the configuration to the user configuration directory. If the
	 * directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	/*@}*/
	
private:
	void pCleanUp();
	void pReset();
};

#endif
