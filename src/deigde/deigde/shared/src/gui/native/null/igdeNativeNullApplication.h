/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENATIVENULLAPPLICATION_H_
#define _IGDENATIVENULLAPPLICATION_H_

#include "../../../environment/igdeEnvironment.h"


class deException;
class igdeApplication;
class decUnicodeStringList;
#ifdef OS_W32
class decUnicodeArgumentList;
#endif


/**
 * Null native application.
 */
class igdeNativeNullApplication{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create application. */
	igdeNativeNullApplication( igdeApplication &owner );
	
	/** Clean up application. */
	virtual ~igdeNativeNullApplication();
	
	/** Create native application. */
	static igdeNativeNullApplication* CreateNativeApplication( igdeApplication &application );
	
	/** Destroy native application. */
	virtual void DestroyNativeApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get arguments from OS specific startup routine. */
	#ifdef OS_UNIX
	static void GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args );
	#elif defined OS_W32
	static void GetOSStartUpArguments( decUnicodeStringList &arguments,
		const decUnicodeArgumentList &windowsArguments );
	#endif
	
	/** Initialize application. */
	void Initialize( decUnicodeStringList &arguments );
	
	/** Run. */
	void Run();
	
	/** Quit. */
	void Quit();
	
	/** System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** Get main app font config. */
	void GetAppFontConfig( igdeFont::sConfiguration &config ) const;
	
	/** Show error. */
	void ShowError( const deException &exception ) const;
	
	/** Show window modal while keeping engine properly updating. */
	void RunModalWhileShown( igdeWindow &window );
	/*@}*/
	
	
	
private:
	igdeApplication &pOwner;
	bool pQuitRequested;
};

typedef igdeNativeNullApplication igdeNativeApplication;

#endif
