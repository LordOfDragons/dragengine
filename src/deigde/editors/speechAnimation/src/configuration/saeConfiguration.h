/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAECONFIGURATION_H_
#define _SAECONFIGURATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class saeWindowMain;



/**
 * \brief Editor Configuration.
 */
class saeConfiguration{
private:
	saeWindowMain &pWindowMain;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration. */
	saeConfiguration( saeWindowMain &windowMain );
	
	/** Cleans up the configuration. */
	~saeConfiguration();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline saeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
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
