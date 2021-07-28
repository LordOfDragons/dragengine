/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEGLSIGNALHANDLER_H_
#define _DEGLSIGNALHANDLER_H_

class deglLauncher;


/**
 * Signal handler.
 */
class deglSignalHandler{
private:
	deglLauncher &pLauncher;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create signal handler. */
	deglSignalHandler( deglLauncher &launcher );
	
	/** Clean up signal handler. */
	~deglSignalHandler();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Launcher. */
	inline deglLauncher &GetLauncher() const{ return pLauncher; }
	/*@}*/
	
	
	
private:
	void pRegisterSignals();
};

#endif
