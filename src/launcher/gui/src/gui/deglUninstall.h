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

#ifndef _DEGLUNINSTALL_H_
#define _DEGLUNINSTALL_H_

#include "foxtoolkit.h"

class deglWindowMain;
class delGame;
class delPatch;
class decString;


/**
 * Uninstall game or patch.
 */
class deglUninstall{
private:
	deglWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create uninstaller. */
	deglUninstall( deglWindowMain &window );
	
	/** Clean up uninstaller. */
	~deglUninstall();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Uninstall game. */
	bool UninstallGame( delGame &game );
	
	/** Uninstall patch. */
	bool UninstallPatch( delPatch &patch );
	/*@}*/
	
	
	
private:
	void pUninstallDelga( const decString &filename );
};

#endif
