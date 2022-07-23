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

#ifndef _DECLLISTPROFILES_H_
#define _DECLLISTPROFILES_H_

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>

class declLauncher;



/**
 * \brief List Profiles Action.
 */
class declListProfiles{
private:
	declLauncher &pLauncher;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new action. */
	declListProfiles( declLauncher &launcher );
	
	/** Cleans up the action. */
	~declListProfiles();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Launcher. */
	inline declLauncher &GetLauncher() const{ return pLauncher; }
	
	/** Print syntax. */
	void PrintSyntax();
	
	/** Parse arguments. */
	void ParseArguments();
	
	/** Run action. */
	void Run();
	/*@}*/
};

#endif
