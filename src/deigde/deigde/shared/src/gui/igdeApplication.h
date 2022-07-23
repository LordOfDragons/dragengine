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

#ifndef _IGDEAPPLICATION_H_
#define _IGDEAPPLICATION_H_


#include "../environment/igdeEnvironment.h"
#include "igdeWidgetReference.h"

#include <dragengine/dragengine_configuration.h>
#include <dragengine/common/string/decString.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif


class decUnicodeStringList;
class igdeMainWindow;
class igdeWindow;


/**
 * \brief IGDE UI Application.
 */
class igdeApplication{
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create application. */
	igdeApplication();
	
	/** \brief Clean up application. */
	virtual ~igdeApplication();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	igdeMainWindow *GetMainWindow() const;
	
	/**
	 * \brief Run application.
	 * 
	 * Calls Initialize() then runs the application until it exits. After exit calls CleanUp()
	 * before returning. Caller has to exit entry point with OK return code unless an exception
	 * is thrown. In case of exception CleanUp() is called properly.
	 */
#ifdef OS_UNIX
	void Run( int argCount, char **args );
#elif defined OS_W32
	void Run();
#else
#error "Unsupported OS"
#endif
	
	/** \brief System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** \brief Get main app font config. */
	void GetAppFontConfig( igdeFont::sConfiguration &config );
	
	/** \brief Application instance. */
	static igdeApplication &app();
	
	/** \brief Show window modal while keeping engine properly updating. */
	void RunModalWhileShown( igdeWindow &window );
	
	/** \brief For internal use only. */
	inline void *GetNativeApplication() const{ return pNativeApplication; }
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Initialize application.
	 * \returns true to run application or false to abord.
	 */
	virtual bool Initialize( const decUnicodeStringList &arguments ) = 0;
	
	/** \brief Set main window. */
	void SetMainWindow( igdeMainWindow *mainWindow, bool takeOver );
	
	/** \brief Clean up application. */
	virtual void CleanUp();
	
	
	
private:
	void pSharedRun( decUnicodeStringList &arguments );
	
	void *pNativeApplication;
	igdeWidgetReference pMainWindow;
	static igdeApplication *pApp;
};

#endif
