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

#ifndef _DECACTIONDELGA_H_
#define _DECACTIONDELGA_H_

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>

class declLauncher;



/**
 * \brief Delga Action.
 */
class declActionDelga{
private:
	declLauncher &pLauncher;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	declActionDelga( declLauncher &launcher );
	
	/** \brief Clean up action. */
	~declActionDelga();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline declLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Print syntax. */
	void PrintSyntax();
	
	/** \brief Run action. */
	int Run();
	/*@}*/
	
	
	
private:
	int pContent();
	int pInstall();
};

#endif
